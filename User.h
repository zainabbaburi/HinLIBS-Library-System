#ifndef USER_H
#define USER_H

#pragma once
#include "Loan.h"
#include "Collection.h"

#include <string>
#include <memory>
#include <vector>
#include <algorithm>

// Base class: User
class User {
private:
    std::string id;
    std::string name;
    std::string username;
    std::string password;
    int accessLevel; // 1 = Patron, 2 = Librarian, 3 = SysAdmin

public:
    User(const std::string& id,
         const std::string& name,
         const std::string& username,
         const std::string& password,
         int accessLevel);
    virtual ~User() = default;

    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getUsername() const;
    std::string getPassword() const;
    int getAccessLevel() const;

    // Setters
    void setName(const std::string& newName);
    void setUsername(const std::string& newUsername);
    void setPassword(const std::string& newPassword);
    void setAccessLevel(int newLevel);
};

// Patron class
class Patron : public User {
private:
    std::string phoneNumber;
    std::string email;
    int numLoans;
    int numHolds;

public:
    Patron(const std::string& id,
           const std::string& name,
           const std::string& username,
           const std::string& password,
           const std::string& phoneNumber,
           const std::string& email);

    // Getters
    std::string getPhoneNumber() const;
    std::string getCardNumber() const;
    std::string getEmail() const;
    std::string getId() const;
    int getNumLoans() const;
    int getNumHolds() const { return numHolds; }

    // Setters
    void setPhoneNumber(const std::string& newPhoneNumber);
    void setEmail(const std::string& newEmail);

    void incrementHoldCount();
    void decrementHoldCount();
    void incrementLoanCount();
    void decrementLoanCount();
};

// Librarian class
class Librarian : public User {
public:
    Librarian(const std::string& id,
              const std::string& name,
              const std::string& username,
              const std::string& password);
};

// SysAdmin class
class SysAdmin : public User {
public:
    SysAdmin(const std::string& id,
             const std::string& name,
             const std::string& username,
             const std::string& password);
};

#endif // USER_H
