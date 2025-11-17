#ifndef LOANMANAGER_H
#define LOANMANAGER_H

#pragma once

#include <string>
#include "Collection.h"
#include "User.h"
#include "Loan.h"
#include "CatalogueItem.h"
#include <QString>
#include <QObject>
#include "FIFO.h"

// Result when creating a loan
enum class LoanResult {
    Success,
    ItemUnavailable,
    LimitReached,
    Error
};

// Result when removing (returning) a loan
enum class RemoveLoanResult {
    Success,
    LoanNotFound,
    NotOwnedByPatron,
    Error
};

class LoanManager : public QObject {
    Q_OBJECT
public:
    LoanManager(LoanList& loansRef,
                CatalogueList& catalogueRef,
                FIFO& holdQueueRef,
                UserList& usersRef);

    // Create a loan for this patron and item
    LoanResult createLoan(Patron& patron,
                          CatalogueItem& item,
                          Loan*& outLoan);

    // Remove (return) a loan by id for this patron
    RemoveLoanResult removeLoan(Patron& patron, const std::string& loanId);
    void processHolds(std::shared_ptr<CatalogueItem> itemPtr);
    static QString resultToString(LoanResult result);

signals:
    void loanUpdated();
    void holdsUpdated();

private:
    LoanList& loans;
    CatalogueList& catalogue;
    FIFO& holdQueue;
    UserList& users;
};

#endif // LOANMANAGER_H

