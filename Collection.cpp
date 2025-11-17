#include "Collection.h"
#include "User.h"
#include "CatalogueItem.h"
#include "Loan.h"

#include <algorithm>

// Constructors
UserList::UserList() = default;
CatalogueList::CatalogueList() = default;
LoanList::LoanList() = default;

// Helper: convert string to lowercase
static std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

// Find user by name (case-insensitive, substring match)
std::shared_ptr<User> UserList::findByName(const std::string& name) const {
    const std::string target = toLower(name);
    for (auto& u : items) {
        if (u && toLower(u->getName()).find(target) != std::string::npos) {
            return u;
        }
    }
    return nullptr;
}

// Find catalogue item by title (case-insensitive, substring match)
std::shared_ptr<CatalogueItem> CatalogueList::findByTitle(const std::string& title) const {
    const std::string target = toLower(title);
    for (auto& it : items) {
        if (it && toLower(it->getTitle()).find(target) != std::string::npos) {
            return it;
        }
    }
    return nullptr;
}

// Return all active (not returned) loans for a user
std::vector<std::shared_ptr<Loan>> LoanList::activeForUser(const std::string& userId) const {
    std::vector<std::shared_ptr<Loan>> result;

    for (const auto& loan : items) {
        if (!loan) continue;

        if (loan->getUserId() == userId && !loan->isReturned()) {
            result.push_back(loan);
        }
    }

    return result;
}
