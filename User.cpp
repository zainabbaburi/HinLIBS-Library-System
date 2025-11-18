#include "User.h"
#include <algorithm>

// User
User::User(const std::string& id,
           const std::string& name,
           const std::string& username,
           const std::string& password,
           int accessLevel)
    : id(id), name(name), username(username), password(password), accessLevel(accessLevel) {}

// Getters
std::string User::getId() const        { return id; }
std::string User::getName() const      { return name; }
std::string User::getUsername() const  { return username; }
std::string User::getPassword() const  { return password; }
int         User::getAccessLevel() const { return accessLevel; }
std::string Patron::getId() const { return User::getId(); }

// Setters
void User::setName(const std::string& newName)        { name = newName; }
void User::setUsername(const std::string& newUsername){ username = newUsername; }
void User::setPassword(const std::string& newPassword){ password = newPassword; }
void User::setAccessLevel(int newLevel)               { accessLevel = newLevel; }

// Patron
Patron::Patron(const std::string& id,
               const std::string& name,
               const std::string& username,
               const std::string& password,
               const std::string& phoneNumber,
               const std::string& email)
    : User(id, name, username, password, 1)
    , phoneNumber(phoneNumber)
    , email(email)
    , numLoans(0)
    , numHolds(0)
    , amountOwed(0.0)
{
}

// Gettters
std::string Patron::getPhoneNumber() const { return phoneNumber; }
std::string Patron::getEmail() const       { return email; }
int         Patron::getNumLoans() const    { return numLoans; }
std::string Patron::getCardNumber() const  { return getUsername(); }

// Setters
void Patron::setPhoneNumber(const std::string& newPhoneNumber) { phoneNumber = newPhoneNumber; }
void Patron::setEmail(const std::string& newEmail)             { email = newEmail; }

void Patron::decrementLoanCount() {
    if (numLoans > 0) {
        numLoans--;
    }
}

void Patron::incrementLoanCount() {
    numLoans++;
}
void Patron::decrementHoldCount() {
    if (numHolds > 0) {
        numHolds--;
    }
}

void Patron::incrementHoldCount() {
    numHolds++;
}

// Librarian
Librarian::Librarian(const std::string& id,
                     const std::string& name,
                     const std::string& username,
                     const std::string& password)
    : User(id, name, username, password, 2) {}

// SysAdmin
SysAdmin::SysAdmin(const std::string& id,
                   const std::string& name,
                   const std::string& username,
                   const std::string& password)
    : User(id, name, username, password, 3) {}
