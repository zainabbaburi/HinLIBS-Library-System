#include "Loan.h"
#include <sstream>
#include <iomanip>
#include <chrono>

// Constructor
Loan::Loan(const std::string& loanId,
           const std::string& userId,
           const std::string& itemId,
           const std::chrono::system_clock::time_point& loanDate,
           const std::chrono::system_clock::time_point& dueDate)
    : loanId(loanId),
      userId(userId),
      itemId(itemId),
      loanDate(loanDate),
      dueDate(dueDate),
      returned(false) {}

// Getters
std::string Loan::getId() const { return loanId; }
std::string Loan::getUserId() const { return userId; }
std::string Loan::getItemId() const { return itemId; }
std::chrono::system_clock::time_point Loan::getLoanDate() const { return loanDate; }
std::chrono::system_clock::time_point Loan::getDueDate() const { return dueDate; }
bool Loan::isReturned() const { return returned; }

// Setters
void Loan::markReturned() { returned = true; }
void Loan::setDueDate(const std::chrono::system_clock::time_point& newDate) { dueDate = newDate; }

// Convert due date to "YYYY-MM-DD"
std::string Loan::getDueDateString() const {
    std::time_t tt = std::chrono::system_clock::to_time_t(dueDate);
    std::tm tm{};

#ifdef _WIN32
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

// Calculate days remaining until due date
int Loan::getDaysRemaining() const {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto diff = duration_cast<hours>(dueDate - now).count() / 24;
    return static_cast<int>(diff);
}

// Convert to string with catalogue lookup
std::string Loan::toString(const CatalogueList& catalogue) const {
    std::ostringstream oss;

    std::string itemTitle = "(Unknown Item)";
    auto itemPtr = catalogue.findById(itemId);
    if (itemPtr) {
        itemTitle = itemPtr->getTitle();
    }

    int daysRemaining = getDaysRemaining();

    std::time_t tt = std::chrono::system_clock::to_time_t(loanDate);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif

    oss << "=== Loan ===\n"
        << "Item Title: " << itemTitle << "\n"
        << "Loan Date: " << std::put_time(&tm, "%Y-%m-%d") << "\n"
        << "Due Date: " << getDueDateString() << "\n"
        << "Days Remaining: " << daysRemaining << "\n"
        << "Status: " << (returned ? "Returned"
                                  : (daysRemaining < 0 ? "Overdue" : "Active"))
        << "\n";

    return oss.str();
}

// Generate a unique loan ID
std::string Loan::generateLoanId() {
    static std::atomic<unsigned long long> counter{1};
    return "L" + std::to_string(counter.fetch_add(1));
}
