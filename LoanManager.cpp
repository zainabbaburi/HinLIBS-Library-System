#include "LoanManager.h"
#include <chrono>
#include <QDebug>

// Constructor wires references to shared lists and queues
LoanManager::LoanManager(LoanList& loansRef,
                         CatalogueList& catalogueRef,
                         FIFO& holdQueueRef,
                         UserList& userListRef)
    : loans(loansRef),
      catalogue(catalogueRef),
      holdQueue(holdQueueRef),
      users(userListRef)
{
}

// Creates a new loan for a patron
LoanResult LoanManager::createLoan(Patron& patron,
                                   CatalogueItem& item,
                                   Loan*& outLoan)
{
    outLoan = nullptr;

    // Item must be available
    if (!item.isAvailable()) {
        return LoanResult::ItemUnavailable;
    }

    // Enforce limit of 3 active loans (holds are unlimited)
    const int MAX_LOANS = 3;
    if (patron.getNumLoans() >= MAX_LOANS) {
        return LoanResult::LimitReached;
    }

    try {
        using Clock = std::chrono::system_clock;
        auto now = Clock::now();
        auto due = now + std::chrono::hours{24 * Loan::getDefaultDurationDays()};

        // Create a new Loan object
        auto sp = std::make_shared<Loan>(
            Loan::generateLoanId(),
            patron.getId(),
            item.getId(),
            now,
            due
        );

        // Store loan and update item/patron state
        loans.add(sp);
        item.setAvailable(false);
        patron.incrementLoanCount();
        outLoan = sp.get();

        std::string itemId   = item.getId();
        std::string patronId = patron.getId();

        // If this patron had a hold on this item, remove that hold
        auto queue = holdQueue.getQueue();
        for (auto& record : queue) {
            if (record.first == patronId && record.second == itemId) {
                holdQueue.remove(patronId, itemId);
                patron.decrementHoldCount();
                emit holdsUpdated();
                break;
            }
        }

        emit loanUpdated();
        return LoanResult::Success;
    }
    catch (const std::exception&) {
        return LoanResult::Error;
    }
}

// Handles item state after a loan is returned
void LoanManager::processHolds(std::shared_ptr<CatalogueItem> itemPtr)
{
    if (!itemPtr) return;

    std::string itemId = itemPtr->getId();

    // In all cases, item becomes available in the system.
    // The UI enforces that only the first patron in the hold queue
    // can actually check it out if there are holds.
    if (!holdQueue.hasHold(itemId)) {
        itemPtr->setAvailable(true);
    } else {
        itemPtr->setAvailable(true);
    }

    emit holdsUpdated();
    emit loanUpdated();
}

// Removes a loan when a patron returns an item
RemoveLoanResult LoanManager::removeLoan(Patron& patron, const std::string& loanId)
{
    // Look up the loan by ID
    auto loan = loans.findById(loanId);
    if (!loan) return RemoveLoanResult::LoanNotFound;

    // Ensure the loan belongs to this patron
    if (loan->getUserId() != patron.getId())
        return RemoveLoanResult::NotOwnedByPatron;

    // Find the related catalogue item
    auto itemPtr = catalogue.findById(loan->getItemId());
    if (!itemPtr) return RemoveLoanResult::Error;

    // Mark loan as returned and remove from the list
    loan->markReturned();
    if (!loans.removeById(loanId))
        return RemoveLoanResult::Error;

    // Decrement the patron's active loan count
    patron.decrementLoanCount();

    // Update holds and item availability after the return
    processHolds(itemPtr);

    return RemoveLoanResult::Success;
}

// Convert a LoanResult to a readable string
QString LoanManager::resultToString(LoanResult result) {
    switch (result) {
        case LoanResult::Success:         return "Success";
        case LoanResult::ItemUnavailable: return "Item is unavailable";
        case LoanResult::LimitReached:    return "Loan limit reached";
        case LoanResult::Error:           return "An unknown error occurred";
        default:                          return "Unknown result";
    }
}
