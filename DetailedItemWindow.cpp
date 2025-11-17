#include "DetailedItemWindow.h"
#include "ui_DetailedItemWindow.h"
#include <QMessageBox>

DetailedItemWindow::DetailedItemWindow(CatalogueItem& item,
                                       Patron& user,
                                       LoanList& loansRef,
                                       FIFO& holdQueueRef,
                                       LoanManager& loanManagerRef,
                                       QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DetailedItemWindow)
    , currentItem(item)
    , currentUser(user)
    , loans(loansRef)
    , holdQueue(holdQueueRef)
    , loanManager(loanManagerRef)
{
    ui->setupUi(this);
    refreshDisplay();
    updateButtons();
    this->resize(800, 600);
}

DetailedItemWindow::~DetailedItemWindow()
{
    delete ui;
}

// Show item details using the HTML string from CatalogueItem::toString
void DetailedItemWindow::refreshDisplay()
{
    ui->itemDetails->setTextFormat(Qt::RichText);
    ui->itemDetails->setText(QString::fromStdString(currentItem.toString()));
}

// Enable or disable buttons based on availability and holds
void DetailedItemWindow::updateButtons()
{
    std::string itemId        = currentItem.getId();
    std::string currentUserId = currentUser.getId();

    bool isAvailable    = currentItem.isAvailable();
    bool isFirstInQueue = false;
    bool userHasHold    = false;

    // Check if this item has any holds
    if (holdQueue.hasHold(itemId)) {
        // Check if this user has a hold on this item
        int pos = holdQueue.getPosition(currentUserId, itemId);
        userHasHold = (pos > 0);

        // Check if this user is first in the queue
        try {
            if (holdQueue.peekNextPatron(itemId) == currentUserId) {
                isFirstInQueue = true;
            }
        } catch (const std::out_of_range&) {
            // Queue became empty unexpectedly; ignore
        }
    }

    int holdPosition = holdQueue.getPosition(currentUserId, itemId);

    // Same rule as BrowseWindow:
    // User can check out if:
    //  - item is available and there are no holds, or
    //  - user is first in the hold queue
    bool canCheckout = ((isAvailable && !holdQueue.hasHold(itemId)) || isFirstInQueue);

    // Checkout button is always visible, but only enabled when allowed
    ui->checkoutButton->setVisible(true);
    ui->checkoutButton->setEnabled(canCheckout);

    // Place Hold button handling
    if (isAvailable) {
        // If the item is available, there is no reason to place a hold
        ui->placeHoldButton->setVisible(false);
        return;
    }

    ui->placeHoldButton->setVisible(true);

    if (userHasHold) {
        // User already has a hold on this item
        ui->placeHoldButton->setText(
            QString("Hold Placed (#%1)").arg(holdPosition)
        );
        ui->placeHoldButton->setEnabled(false);
        ui->placeHoldButton->setStyleSheet("background-color: darkgreen; color: white;");
    } else {
        // User has no hold yet
        ui->placeHoldButton->setText("Place Hold");
        // If they can check out now, do not let them place a hold
        ui->placeHoldButton->setEnabled(!canCheckout);
        ui->placeHoldButton->setStyleSheet("");
    }
}

// Handle clicking the "Check Out" button
void DetailedItemWindow::on_checkoutButton_clicked()
{
    if (LoanUiHelpers::handleCheckout(currentUser,
                                      currentItem,
                                      loanManager,
                                      holdQueue,
                                      this)) {
        updateButtons();
        refreshDisplay();
        emit accountDataChanged();
    }
}

// Handle clicking the "Place Hold" button
void DetailedItemWindow::on_placeHoldButton_clicked()
{
    if (LoanUiHelpers::handlePlaceHold(currentUser,
                                       currentItem,
                                       loans,
                                       holdQueue,
                                       this)) {
        updateButtons();
        refreshDisplay();
        emit accountDataChanged();
    }
}

// Handle clicking the "Close" button
void DetailedItemWindow::on_closeButton_clicked()
{
    emit backRequested();
}
