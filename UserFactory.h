#ifndef USERFACTORY_H
#define USERFACTORY_H

#pragma once
#include <memory>
#include <string>
#include "User.h"

// Patron, Librarian, SysAdmin
// NOTE: Patron's username and password is their cardnumber/pin
class UserFactory {
public:
    std::shared_ptr<Patron> createPatron(const std::string& id,
                                         const std::string& name,
                                         const std::string& username,
                                         const std::string& password,
                                         const std::string& phone,
                                         const std::string& email) {
        return std::make_shared<Patron>(id, name, username, password, phone, email);
    }

    std::shared_ptr<Librarian> createLibrarian(const std::string& id,
        const std::string& name,
        const std::string& username,
        const std::string& password) {
        return std::make_shared<Librarian>(id, name, username, password);
    }

    std::shared_ptr<SysAdmin> createSysAdmin(const std::string& id,
        const std::string& name,
        const std::string& username,
        const std::string& password) {
        return std::make_shared<SysAdmin>(id, name, username, password);
    }
};

#endif  //USERFACTORY_H
