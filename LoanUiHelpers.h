#ifndef LOANUIHELPERS_H
#define LOANUIHELPERS_H

#pragma once
#include <QMessageBox>
#include "User.h"
#include "CatalogueItem.h"
#include "LoanManager.h"
#include "FIFO.h"
#include "Collection.h"

namespace LoanUiHelpers {

// Handles the logic for checking out an item
inline bool handleCheckout(Patron& user,
                           CatalogueItem& item,
                           LoanManager& loanManager,
                           FIFO& holdQueue,
                           QWidget* parent)
{
    std::string itemId = item.getId();

    // If the item has a hold, only the first person in queue can check it out
    if (holdQueue.hasHold(itemId)) {
        if (holdQueue.peekNextPatron(itemId) != user.getId()) {
            QMessageBox::warning(parent, "Checkout Denied",
                                 "This item is reserved for another patron.");
            return false;
        }
    } else {
        // If the item isn’t available, deny checkout
        if (!item.isAvailable()) {
            QMessageBox::warning(parent, "Checkout Failed",
                                 "This item is currently not available.");
            return false;
        }
    }

    // Try to create a loan record through the LoanManager
    Loan* loanPtr = nullptr;
    LoanResult result = loanManager.createLoan(user, item, loanPtr);

    // Handle all possible outcomes
    if (result == LoanResult::ItemUnavailable) {
        QMessageBox::warning(parent, "Checkout Failed",
                             "This item is currently not available.");
        return false;
    }

    if (result == LoanResult::LimitReached) {
        QMessageBox::warning(parent, "Checkout Failed",
                             "You have reached your maximum loan limit (3).");
        return false;
    }

    // If anything else went wrong, show a generic error
    if (result != LoanResult::Success || !loanPtr) {
        QMessageBox::critical(parent, "Checkout Error",
                              "Loan could not be created due to an unexpected error.");
        return false;
    }

    // Success message showing what was checked out and when it’s due
    QMessageBox::information(
        parent, "Checkout Successful",
        QString("You checked out: %1\nDue: %2")
            .arg(QString::fromStdString(item.getTitle()))
            .arg(QString::fromStdString(loanPtr->getDueDateString()))
    );

    return true;
}

// Handles the logic for placing a hold on an item
inline bool handlePlaceHold(Patron& user,
                            CatalogueItem& item,
                            LoanList& loans,
                            FIFO& holdQueue,
                            QWidget* parent)
{
    // Don’t allow holds if the item is currently available
    if (item.isAvailable()) {
        QMessageBox::warning(parent, "Hold Not Necessary",
                             "This item is available now! Please use the 'Check Out' button instead.");
        return false;
    }

    // Prevent holds on items the user already has checked out
    auto activeLoans = loans.activeForUser(user.getId());
    for (const auto& loan : activeLoans) {
        if (loan && loan->getItemId() == item.getId()) {
            QMessageBox::warning(parent, "Hold Not Allowed",
                                 "You already have this item checked out.");
            return false;
        }
    }

    std::string patronId = user.getId();
    std::string itemId   = item.getId();

    // Add this hold to the global queue and update the user’s hold count
    holdQueue.enqueue(patronId, itemId);
    user.incrementHoldCount();

    // Get the user’s current position in the hold queue
    int position = holdQueue.getPosition(patronId, itemId);

    // Inform the user that the hold was placed successfully
    QMessageBox::information(
        parent, "Hold Placed",
        QString("You have placed a hold on: %1\nPosition: %2")
            .arg(QString::fromStdString(item.getTitle()))
            .arg(position)
    );

    return true;
}

}

#endif // LOANUIHELPERS_H
