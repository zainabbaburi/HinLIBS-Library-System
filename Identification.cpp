#include "Identification.h"
#include <algorithm>
#include <cctype>
#include <memory>

// Convert a string to lowercase
std::string Identification::toLower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char ch){ return std::tolower(ch); });
    return result;
}

// Constructor: store lowercase input and reference to user list
Identification::Identification(const std::string& userInput, UserList& usersRef)
    : userInputLower(toLower(userInput))
    , users(&usersRef)
{}

// Search for a matching user by name or username (case-insensitive)
std::shared_ptr<User> Identification::findUser() const {
    if (!users) return nullptr;

    size_t n = users->size();
    for (size_t i = 0; i < n; ++i) {
        auto user = users->get(static_cast<int>(i));
        if (!user) continue;

        const std::string& input = userInputLower;
        std::string nameLower = toLower(user->getName());
        std::string usernameLower = toLower(user->getUsername());

        if (nameLower == input || usernameLower == input) {
            return user;
        }
    }
    return nullptr;
}
