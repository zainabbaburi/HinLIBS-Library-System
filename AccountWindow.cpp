#include "AccountWindow.h"
#include "ui_AccountWindow.h"
#include "CatalogueItem.h"
#include "FIFO.h"
#include "Loan.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidgetItem>
#include <QVariant>
#include <QMessageBox>
#include <memory>

inline std::string trim(const std::string &s) {
    auto wsfront = std::find_if_not(s.begin(), s.end(), ::isspace);
    auto wsback  = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

AccountWindow::AccountWindow(CatalogueList& catalogueRef,
                             Patron& currentUser,
                             LoanList& loansRef,
                             FIFO& holdQueueRef,
                             LoanManager& loanManagerRef,
                             QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AccountWindow)
    , catalogue(catalogueRef)
    , user(currentUser)
    , loans(loansRef)
    , holdQueue(holdQueueRef)
    , loanManager(loanManagerRef)
{
    ui->setupUi(this);

    // Update account info when loans or holds change
    connect(&loanManager, &LoanManager::holdsUpdated,
            this, &AccountWindow::refreshAccountInfo);
    connect(&loanManager, &LoanManager::loanUpdated,
            this, &AccountWindow::refreshAccountInfo);

    // Basic user info labels
    ui->userNameLabel->setText(
        QString("Welcome, %1!").arg(QString::fromStdString(user.getName())));
    ui->userEmailLabel->setText(
        QString("Email: %1").arg(QString::fromStdString(user.getEmail())));

    refreshAccountInfo();
}

AccountWindow::~AccountWindow()
{
    delete ui;
}

// Reload both checked-out items and holds
void AccountWindow::refreshAccountInfo()
{
    loadCheckedOutItems();
    loadHolds();
}

// Populate checked-out items list for this user
void AccountWindow::loadCheckedOutItems()
{
    ui->checkedOutListWidget->clear();

    int checkedOutCount = 0;
    std::string currentUserId = user.getId();

    // Walk through all loans and filter to this user
    for (size_t i = 0; i < loans.size(); i++) {
        auto loan = loans.get(i);
        if (trim(loan->getUserId()) == trim(currentUserId)) {

            // Find the matching catalogue item by ID
            std::shared_ptr<CatalogueItem> item = nullptr;
            int itemIndex = -1;

            for (size_t j = 0; j < catalogue.size(); ++j) {
                if (trim(catalogue.get(j)->getId()) == trim(loan->getItemId())) {
                    item = catalogue.get(j);
                    itemIndex = static_cast<int>(j);
                    break;
                }
            }

            if (item) {
                checkedOutCount++;

                // Build row widget for this loan
                QWidget* itemWidget = new QWidget();
                QHBoxLayout* layout = new QHBoxLayout(itemWidget);

                QVBoxLayout* infoLayout = new QVBoxLayout();
                QLabel* titleLabel = new QLabel(
                    QString::fromStdString(item->getTitle()), itemWidget);

                int daysRemaining = loan->getDaysRemaining();
                QString daysText;

                // Item due date info
                if (daysRemaining > 0) {
                    daysText = QString("%1 days remaining").arg(daysRemaining);
                } else if (daysRemaining == 0) {
                    daysText = "Due today";
                } else {
                    daysText = QString("Overdue by %1 day(s)").arg(-daysRemaining);
                }

                QLabel* infoLabel = new QLabel(
                    QString("Year: %1 | Due: %2 | %3")
                        .arg(item->getYear())
                        .arg(QString::fromStdString(loan->getDueDateString()))
                        .arg(daysText),
                    itemWidget);

                titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
                infoLayout->addWidget(titleLabel);
                infoLayout->addWidget(infoLabel);

                QPushButton* returnBtn = new QPushButton("Return", itemWidget);
                // Store index so we can find the item when button is clicked
                returnBtn->setProperty("itemIndex", QVariant::fromValue(itemIndex));
                connect(returnBtn, &QPushButton::clicked,
                        this, &AccountWindow::onReturnButtonClicked);

                layout->addLayout(infoLayout, 3);
                layout->addWidget(returnBtn, 1);

                QListWidgetItem* listItem = new QListWidgetItem(ui->checkedOutListWidget);
                listItem->setSizeHint(itemWidget->sizeHint());
                ui->checkedOutListWidget->addItem(listItem);
                ui->checkedOutListWidget->setItemWidget(listItem, itemWidget);
            }
        }
    }

    // Update header label
    ui->checkedOutCountLabel->setText(
        QString("Checked Out Items (%1)").arg(checkedOutCount));

    // Show message if there are no items
    if (checkedOutCount == 0) {
        QLabel* noItemsLabel = new QLabel("You don't have any items checked out.");
        noItemsLabel->setAlignment(Qt::AlignCenter);
        noItemsLabel->setStyleSheet("color: gray; padding: 20px;");

        QListWidgetItem* listItem = new QListWidgetItem(ui->checkedOutListWidget);
        listItem->setSizeHint(noItemsLabel->sizeHint());
        ui->checkedOutListWidget->addItem(listItem);
        ui->checkedOutListWidget->setItemWidget(listItem, noItemsLabel);
    }
}

// Populate holds list for this user
void AccountWindow::loadHolds()
{
    ui->holdsListWidget->clear();
    std::string currentUserId = user.getId();
    int holdCount = 0;

    auto queue = holdQueue.getQueue();

    for (const auto& holdRecord : queue) {
        std::string holdPatronId = trim(holdRecord.first);
        std::string holdItemId   = trim(holdRecord.second);

        if (holdPatronId == currentUserId) {

            std::shared_ptr<CatalogueItem> item = nullptr;
            int itemIndex = -1;
            for (size_t i = 0; i < catalogue.size(); ++i) {
                if (trim(catalogue.get(i)->getId()) == holdItemId) {
                    item = catalogue.get(i);
                    itemIndex = static_cast<int>(i);
                    break;
                }
            }

            if (item) {
                int position = holdQueue.getPosition(holdPatronId, holdItemId);
                holdCount++;

                QWidget* itemWidget = new QWidget();
                QHBoxLayout* layout = new QHBoxLayout(itemWidget);

                QVBoxLayout* infoLayout = new QVBoxLayout();
                QLabel* titleLabel = new QLabel(
                    QString::fromStdString(item->getTitle()), itemWidget);
                QLabel* infoLabel = new QLabel(
                    QString("Your Position: #%1").arg(position), itemWidget);

                titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
                infoLayout->addWidget(titleLabel);
                infoLayout->addWidget(infoLabel);

                QPushButton* cancelBtn = new QPushButton("Cancel Hold", itemWidget);
                cancelBtn->setProperty("itemIndex", QVariant::fromValue(itemIndex));
                connect(cancelBtn, &QPushButton::clicked,
                        this, &AccountWindow::onCancelHoldButtonClicked);

                layout->addLayout(infoLayout, 3);
                layout->addWidget(cancelBtn, 1);

                QListWidgetItem* listItem = new QListWidgetItem(ui->holdsListWidget);
                listItem->setSizeHint(itemWidget->sizeHint());
                ui->holdsListWidget->addItem(listItem);
                ui->holdsListWidget->setItemWidget(listItem, itemWidget);
            }
        }
    }

    // Update header label
    ui->holdsCountLabel->setText(QString("Holds (%1)").arg(holdCount));

    // Show message if there are no holds
    if (holdCount == 0) {
        QLabel* noHoldsLabel = new QLabel("You don't have any holds placed.");
        noHoldsLabel->setAlignment(Qt::AlignCenter);
        noHoldsLabel->setStyleSheet("color: gray; padding: 20px;");

        QListWidgetItem* listItem = new QListWidgetItem(ui->holdsListWidget);
        listItem->setSizeHint(noHoldsLabel->sizeHint());
        ui->holdsListWidget->addItem(listItem);
        ui->holdsListWidget->setItemWidget(listItem, noHoldsLabel);
    }
}

// Handle "Return" button click for a checked-out item
void AccountWindow::onReturnButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int itemIndex = button->property("itemIndex").toInt();
    auto item = catalogue.get(itemIndex);
    if (!item) return;

    // Ask user to confirm
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Return Item",
        QString("Are you sure you want to return:\n%1?")
            .arg(QString::fromStdString(item->getTitle())),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes)
        return;

    // Find matching active loan for this item and user
    std::shared_ptr<Loan> targetLoan = nullptr;
    auto activeLoans = loans.activeForUser(user.getId());

    for (const auto& loan : activeLoans) {
        if (!loan) continue;
        if (loan->getItemId() == item->getId()) {
            targetLoan = loan;
            break;
        }
    }

    if (!targetLoan) {
        QMessageBox::warning(this, "Error",
                             "Could not find an active loan for this item.");
        return;
    }

    // Let LoanManager handle the return
    RemoveLoanResult result = loanManager.removeLoan(user, targetLoan->getId());

    if (result != RemoveLoanResult::Success) {
        QString errorMsg =
            (result == RemoveLoanResult::LoanNotFound)
                ? "Loan record not found."
                : "Failed to remove the loan for this item.";
        QMessageBox::critical(this, "Error", errorMsg);
        return;
    }

    // Refresh lists and notify others
    refreshAccountInfo();
    emit accountDataChanged();
}

// Handle "Cancel Hold" button click
void AccountWindow::onCancelHoldButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int itemIndex = button->property("itemIndex").toInt();
    auto item = catalogue.get(itemIndex);
    if (!item) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Cancellation",
        "Are you sure you want to cancel the hold on: "
            + QString::fromStdString(item->getTitle()) + "?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Remove hold from queue and update user count
    holdQueue.remove(user.getId(), item->getId());
    user.decrementHoldCount();

    QMessageBox::information(
        this,
        "Hold Cancelled",
        QString("Your hold on '%1' has been successfully cancelled.")
            .arg(QString::fromStdString(item->getTitle()))
    );

    refreshAccountInfo();
    emit accountDataChanged();
}

// Helper to force a refresh from outside
void AccountWindow::updateLoansList()
{
    refreshAccountInfo();
}
