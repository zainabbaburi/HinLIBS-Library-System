#include "FIFO.h"
#include <algorithm>
#include <stdexcept>

// Add a patron to the hold queue for an item (if not already in it)
void FIFO::enqueue(const std::string& patronId, const std::string& itemId) {
    auto& itemQueue = holds[itemId];
    if (std::find(itemQueue.begin(), itemQueue.end(), patronId) == itemQueue.end()) {
        itemQueue.push_back(patronId);
    }
}

// Return all holds as a list of (patronId, itemId) records
std::list<HoldRecord> FIFO::getQueue() const {
    std::list<HoldRecord> allHolds;
    for (const auto& pair : holds) {
        const std::string& itemId = pair.first;
        for (const auto& patronId : pair.second) {
            allHolds.emplace_back(patronId, itemId);
        }
    }
    return allHolds;
}

// Get a patron's 1-based position in the queue for a given item
int FIFO::getPosition(const std::string& patronId, const std::string& itemId) const {
    auto it = holds.find(itemId);
    if (it == holds.end()) return 0;

    int pos = 1;
    for (const auto& id : it->second) {
        if (id == patronId) return pos;
        ++pos;
    }
    return 0;
}

// Remove a patron from the queue for a given item
void FIFO::remove(const std::string& patronId, const std::string& itemId) {
    auto it = holds.find(itemId);
    if (it == holds.end()) return;

    it->second.remove(patronId);
    if (it->second.empty()) {
        holds.erase(it);
    }
}

// True if there are no holds at all
bool FIFO::isEmpty() const {
    return holds.empty();
}

// Return the first hold in the whole structure (any item)
// Throws if there are no holds
HoldRecord FIFO::peek() const {
    if (holds.empty()) throw std::out_of_range("Hold queue is empty");
    auto it = holds.begin();
    return HoldRecord(it->second.front(), it->first);
}

// True if this item has at least one hold
bool FIFO::hasHold(const std::string& itemId) const {
    auto it = holds.find(itemId);
    return (it != holds.end() && !it->second.empty());
}

// Get the patron at the front of the queue for an item
// Throws if there are no holds for that item
std::string FIFO::peekNextPatron(const std::string& itemId) const {
    auto it = holds.find(itemId);
    if (it == holds.end() || it->second.empty()) {
        throw std::out_of_range("No active holds found for this item ID.");
    }
    return it->second.front();
}

// Remove and return the patron at the front of the queue for an item
// Throws if there are no holds for that item
std::string FIFO::dequeue(const std::string& itemId) {
    auto it = holds.find(itemId);
    if (it == holds.end() || it->second.empty()) {
        throw std::out_of_range("No active holds found for this item ID.");
    }

    std::string patronId = it->second.front();
    it->second.pop_front();

    if (it->second.empty()) {
        holds.erase(it);
    }

    return patronId;
}
