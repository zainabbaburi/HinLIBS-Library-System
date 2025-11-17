#include "BrowseWindow.h"
#include "ui_BrowseWindow.h"
#include "CatalogueItem.h"
#include "DetailedItemWindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidgetItem>
#include <QVariant>
#include <QMessageBox>
#include <algorithm>
#include <cctype>
#include <iostream>

// Build the browse window and set up search behaviour
BrowseWindow::BrowseWindow(CatalogueList& catalogueRef,
                           Patron& currentUser,
                           LoanList& loansRef,
                           FIFO& holdQueueRef,
                           LoanManager& loanManagerRef,
                           QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BrowseWindow)
    , catalogue(catalogueRef)
    , loans(loansRef)
    , user(currentUser)
    , holdQueue(holdQueueRef)
    , loanManager(loanManagerRef)
    , detailedItemWindow(nullptr)
{
    ui->setupUi(this);
    connect(ui->searchLineEdit, &QLineEdit::textChanged,
            this, &BrowseWindow::onSearchTextChanged);

    performSearch("");
}

// Clean up UI (and child window if it exists)
BrowseWindow::~BrowseWindow()
{
    delete ui;
    if (detailedItemWindow) {
        delete detailedItemWindow;
    }
}

// React to search box changes
void BrowseWindow::onSearchTextChanged(const QString& text)
{
    performSearch(text);
}

// Rebuild the results list based on the current search text
void BrowseWindow::performSearch(const QString& query)
{
    ui->resultsListWidget->clear();
    std::string searchQuery = query.toLower().toStdString();

    for (size_t i = 0; i < catalogue.size(); i++) {
        auto item = catalogue.get(i);
        std::string title = item->getTitle();
        std::transform(title.begin(), title.end(), title.begin(), ::tolower);

        // Skip items that don't match the search text
        if (!query.isEmpty() && title.find(searchQuery) == std::string::npos)
            continue;

        QWidget* resultWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(resultWidget);

        // Item text (title + year + status)
        QVBoxLayout* infoLayout = new QVBoxLayout();
        QString titleText = QString::fromStdString(item->getTitle());
        QString statusText;

        std::string itemId = item->getId();
        std::string currentUserId = user.getId();
        bool isFirstInQueue = false;
        bool userHasHold   = false;

        // Check if this user has a hold and whether they are first
        if (holdQueue.hasHold(itemId)) {
            userHasHold = (holdQueue.getPosition(currentUserId, itemId) > 0);

            try {
                if (holdQueue.peekNextPatron(itemId) == currentUserId)
                    isFirstInQueue = true;
            } catch (const std::out_of_range&) {}
        }

        // Decide status label text
        if (item->isAvailable() && !holdQueue.hasHold(itemId)) {
            statusText = "<span style='color: #4CAF50'>Available</span>";
        } else if (userHasHold) {
            statusText = "<span style='color: #FFB300'>Reserved for You</span>";
        } else {
            statusText = "<span style='color: #E57373'>Checked Out</span>";
        }

        QString infoText = QString("Year: %1 | %2").arg(item->getYear()).arg(statusText);
        QLabel* titleLabel = new QLabel(titleText, resultWidget);
        QLabel* infoLabel = new QLabel(infoText, resultWidget);
        titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
        infoLayout->addWidget(titleLabel);
        infoLayout->addWidget(infoLabel);

        // Buttons for this item: details / checkout / hold
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* detailsBtn  = new QPushButton("Details", resultWidget);
        QPushButton* checkoutBtn = new QPushButton("Check Out", resultWidget);
        QPushButton* holdBtn     = new QPushButton("Place Hold", resultWidget);

        detailsBtn->setProperty("itemIndex", QVariant::fromValue((int)i));
        checkoutBtn->setProperty("itemIndex", QVariant::fromValue((int)i));
        holdBtn->setProperty("itemIndex", QVariant::fromValue((int)i));

        connect(detailsBtn, &QPushButton::clicked, this, &BrowseWindow::onDetailsButtonClicked);
        connect(checkoutBtn, &QPushButton::clicked, this, &BrowseWindow::onCheckoutButtonClicked);
        connect(holdBtn, &QPushButton::clicked, this, &BrowseWindow::onPlaceHoldButtonClicked);

        // Enable/disable actions based on status
        if ((item->isAvailable() && !holdQueue.hasHold(itemId)) || isFirstInQueue) {
            // Item is free, or this user is first in the hold queue
            checkoutBtn->setEnabled(true);
            holdBtn->setEnabled(false);
        } else {
            checkoutBtn->setEnabled(false);
            // Don't allow multiple holds from the same user
            holdBtn->setEnabled(!userHasHold);
        }

        buttonLayout->addWidget(detailsBtn);
        buttonLayout->addWidget(checkoutBtn);
        buttonLayout->addWidget(holdBtn);

        layout->addLayout(infoLayout, 3);
        layout->addLayout(buttonLayout, 1);

        QListWidgetItem* listItem = new QListWidgetItem(ui->resultsListWidget);
        listItem->setSizeHint(resultWidget->sizeHint());
        ui->resultsListWidget->addItem(listItem);
        ui->resultsListWidget->setItemWidget(listItem, resultWidget);
    }

    // If there were no matching items, show a simple message
    if (ui->resultsListWidget->count() == 0) {
        QLabel* noResultsLabel = new QLabel("No items found matching your search.");
        noResultsLabel->setAlignment(Qt::AlignCenter);
        noResultsLabel->setStyleSheet("color: gray; font-size: 12px; padding: 20px;");

        QListWidgetItem* listItem = new QListWidgetItem(ui->resultsListWidget);
        listItem->setSizeHint(noResultsLabel->sizeHint());
        ui->resultsListWidget->addItem(listItem);
        ui->resultsListWidget->setItemWidget(listItem, noResultsLabel);
    }
}

// Open the detailed view for a selected item
void BrowseWindow::onDetailsButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    int index = button->property("itemIndex").toInt();
    auto item = catalogue.get(index);
    emit detailsRequested(*item);
}

// Try to check out the selected item
void BrowseWindow::onCheckoutButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int itemIndex = button->property("itemIndex").toInt();
    auto item = catalogue.get(itemIndex);
    if (!item) return;

    if (LoanUiHelpers::handleCheckout(user, *item, loanManager, holdQueue, this)) {
        refreshResults();
    }
}

// Place a hold on the selected item for the current user
void BrowseWindow::onPlaceHoldButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int index = button->property("itemIndex").toInt();
    auto item = catalogue.get(index);
    if (!item) return;

    if (LoanUiHelpers::handlePlaceHold(user, *item, loans, holdQueue, this)) {
        refreshResults();
    }
}

// Re-run the current search to update the list
void BrowseWindow::refreshResults()
{
    performSearch(ui->searchLineEdit->text());
}
