#ifndef CATALOGUEITEM_H
#define CATALOGUEITEM_H

#include <string>
#include <iostream>

// Base class for all catalogue items
class CatalogueItem {
private:
    std::string id;
    std::string title;
    int year;
    bool available;
    std::string reservedForPatronId;

public:
    CatalogueItem(const std::string& id, const std::string& title, int year);
    virtual ~CatalogueItem() = default;

    // Basic info
    std::string getId() const;
    std::string getTitle() const;
    int getYear() const;

    // Availability
    bool isAvailable() const;
    void setReservedFor(const std::string& patronId);
    std::string getReservedFor() const;

    // Setters
    void setTitle(const std::string& newTitle);
    void setYear(int newYear);
    void setAvailable(bool status);

    // Convert item details to a string
    virtual std::string toString() const;
};

// Book (used for both Fiction and Nonfiction)
class Book : public CatalogueItem {
private:
    std::string author;

public:
    Book(const std::string& id, const std::string& title, int year, const std::string& author);
    std::string getAuthor() const;
    void setAuthor(const std::string& newAuthor);
    std::string toString() const override;
};

// Fiction book subclass
class FictionBook : public Book {
public:
    FictionBook(const std::string& id, const std::string& title, int year, const std::string& author);
    std::string toString() const override;
};

// Non-fiction book subclass
class NonFictionBook : public Book {
private:
    std::string dewey;

public:
    NonFictionBook(const std::string& id, const std::string& title, int year,
                   const std::string& author, const std::string& dewey);
    std::string getDewey() const;
    void setDewey(const std::string& newDewey);
    std::string toString() const override;
};

// Magazine class
class Magazine : public CatalogueItem {
private:
    int issueNumber;
    std::string pubDateISO;

public:
    Magazine(const std::string& id, const std::string& title, int year,
             int issueNumber, const std::string& pubDateISO);
    int getIssueNumber() const;
    std::string getPubDate() const;
    void setIssueNumber(int issue);
    void setPubDate(const std::string& newDate);
    std::string toString() const override;
};

// Movie class
class Movie : public CatalogueItem {
private:
    std::string genre;
    std::string rating;

public:
    Movie(const std::string& id, const std::string& title, int year,
          const std::string& genre, const std::string& rating);
    std::string getGenre() const;
    std::string getRating() const;
    void setGenre(const std::string& newGenre);
    void setRating(const std::string& newRating);
    std::string toString() const override;
};

// Video game class
class VideoGame : public CatalogueItem {
private:
    std::string platform;
    std::string genre;
    std::string rating;

public:
    VideoGame(const std::string& id, const std::string& title, int year,
              const std::string& platform, const std::string& genre, const std::string& rating);
    std::string getPlatform() const;
    std::string getGenre() const;
    std::string getRating() const;
    void setPlatform(const std::string& newPlatform);
    void setGenre(const std::string& newGenre);
    void setRating(const std::string& newRating);
    std::string toString() const override;
};

#endif // CATALOGUEITEM_H
