#ifndef IDENTIFICATION_H
#define IDENTIFICATION_H

#pragma once
#include <string>
#include <memory>
#include "Collection.h"
#include "User.h"

class Identification {
private:
    std::string userInputLower;   
    UserList* users;

    static std::string toLower(const std::string& s);

public:
    Identification(const std::string& userInput, UserList& users);

    std::shared_ptr<User> findUser() const;
};

#endif //IDENTIFICATION_H
