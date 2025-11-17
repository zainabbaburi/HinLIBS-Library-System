#ifndef COLLECTION_H
#define COLLECTION_H

#pragma once
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <QDebug>

// Collection template
template <typename T>
class ListCollection {
protected:
    std::vector<T> items;

public:
    void add(T item) { items.push_back(item); }

    void remove(int index) {
        if (index < 0 || index >= (int)items.size())
            throw std::out_of_range("Invalid index");
        items.erase(items.begin() + index);
    }

    T get(int index) const {
        if (index < 0 || index >= (int)items.size())
            throw std::out_of_range("Invalid index");
        return items[index];
    }

    size_t size() const { return items.size(); }
    void clear() { items.clear(); }

    T findById(const std::string& id) const {
        for (const auto& item : items) {
            if (item && item->getId() == id) {
                return item;
            }
        }
        return nullptr;
    }

    bool removeById(const std::string& id) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i] && items[i]->getId() == id) {
                items.erase(items.begin() + i);
                return true;
            }
        }
        return false;
    }
};

// Forward declarations
class User;
class CatalogueItem;
class Loan;

// UserList
class UserList : public ListCollection<std::shared_ptr<User>> {
public:
    UserList();
    std::shared_ptr<User> findByName(const std::string& name) const;
};

// CatalogueList
class CatalogueList : public ListCollection<std::shared_ptr<CatalogueItem>> {
public:
    CatalogueList();
    std::shared_ptr<CatalogueItem> findByTitle(const std::string& title) const;
};

// LoanList
class LoanList : public ListCollection<std::shared_ptr<Loan>> {
public:
    LoanList();
    std::vector<std::shared_ptr<Loan>> activeForUser(const std::string& userId) const;

private:
    std::vector<std::shared_ptr<Loan>> loans_;
    size_t seq_ = 1;
};

#endif // COLLECTION_H
