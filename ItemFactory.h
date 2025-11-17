#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#pragma once
#include <memory>
#include <string>
#include "CatalogueItem.h"

class ItemFactory {
public:
    std::shared_ptr<FictionBook> createFictionBook(const std::string& id,
        const std::string& title,
        int year,
        const std::string& author) {
        return std::make_shared<FictionBook>(id, title, year, author);
    }

    std::shared_ptr<NonFictionBook> createNonFictionBook(const std::string& id,
        const std::string& title,
        int year,
        const std::string& author,
        const std::string& dewey) {
        return std::make_shared<NonFictionBook>(id, title, year, author, dewey);
    }

    std::shared_ptr<Magazine> createMagazine(const std::string& id,
        const std::string& title,
        int year,
        int issueNumber,
        const std::string& pubDateISO) {
        return std::make_shared<Magazine>(id, title, year, issueNumber, pubDateISO);
    }

    std::shared_ptr<Movie> createMovie(const std::string& id,
        const std::string& title,
        int year,
        const std::string& genre,
        const std::string& rating) {
        return std::make_shared<Movie>(id, title, year, genre, rating);
    }

    std::shared_ptr<VideoGame> createVideoGame(const std::string& id,
        const std::string& title,
        int year,
        const std::string& platform,
        const std::string& genre,
        const std::string& rating) {
        return std::make_shared<VideoGame>(id, title, year, platform, genre, rating);
    }
};

#endif //ITEMFACTORY_H
