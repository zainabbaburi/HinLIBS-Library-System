#pragma once
#include <string>
#include <list>
#include <map>
#include <utility>

using HoldRecord = std::pair<std::string, std::string>; // <patronId, itemId>

class FIFO {
public:
    void enqueue(const std::string& patronId, const std::string& itemId);
    int getPosition(const std::string& patronId, const std::string& itemId) const;
    void remove(const std::string& patronId, const std::string& itemId);
    bool isEmpty() const;
    HoldRecord peek() const;
    std::list<HoldRecord> getQueue() const; // NEW
    bool hasHold(const std::string& itemId) const; 
    std::string peekNextPatron(const std::string& itemId) const;
    std::string dequeue(const std::string& itemId);

private:
    std::map<std::string, std::list<std::string>> holds;
};

