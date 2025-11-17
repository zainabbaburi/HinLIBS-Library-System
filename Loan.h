#ifndef LOAN_H
#define LOAN_H

#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "Collection.h"
#include "CatalogueItem.h"

class Loan {
private:
    std::string loanId;
    std::string userId;
    std::string itemId;
    std::chrono::system_clock::time_point loanDate;
    std::chrono::system_clock::time_point dueDate;
    bool returned;

    // Sets default loan duration
    static constexpr int LOAN_DURATION_DAYS = 14;

public:
    Loan(const std::string& loanId,
         const std::string& userId,
         const std::string& itemId,
         const std::chrono::system_clock::time_point& loanDate,
         const std::chrono::system_clock::time_point& dueDate);

    // Getters
    std::string getId() const;
    std::string getUserId() const;
    std::string getItemId() const;
    std::chrono::system_clock::time_point getLoanDate() const;
    std::chrono::system_clock::time_point getDueDate() const;
    bool isReturned() const;

    static int getDefaultDurationDays() { return LOAN_DURATION_DAYS; }

    // Setters
    void markReturned();
    void setDueDate(const std::chrono::system_clock::time_point& newDate);

    // Utility
    int getDaysRemaining() const;
    std::string getDueDateString() const;
    std::string toString(const CatalogueList& catalogue) const;
    static std::string generateLoanId();
};

#endif // LOAN_H
