#include "CatalogueItem.h"
#include <sstream>
#include <algorithm>

// Helper: convert string to uppercase
static std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Helper: convert bool to "Yes"/"No"
static std::string boolToYesNo(bool b) { return b ? "Yes" : "No"; }

// CatalogueItem

CatalogueItem::CatalogueItem(const std::string& id, const std::string& title, int year)
    : id(id), title(title), year(year), available(true) {}

std::string CatalogueItem::getId() const { return id; }
std::string CatalogueItem::getTitle() const { return title; }
int CatalogueItem::getYear() const { return year; }

// Returns true if item is available and not reserved
bool CatalogueItem::isAvailable() const {
    return available && reservedForPatronId.empty();
}

void CatalogueItem::setReservedFor(const std::string& patronId) { reservedForPatronId = patronId; }
std::string CatalogueItem::getReservedFor() const { return reservedForPatronId; }

void CatalogueItem::setTitle(const std::string& newTitle) { title = newTitle; }
void CatalogueItem::setYear(int newYear) { year = newYear; }
void CatalogueItem::setAvailable(bool status) { available = status; }

// Basic HTML representation of an item
std::string CatalogueItem::toString() const {
    std::ostringstream out;
    out << "<h2>" << toUpper(getTitle()) << "</h2>\n"
        << "<hr>\n"
        << "<p>"
        << "<b>Type:</b> Catalogue Item<br>"
        << "<b>Year:</b> " << getYear() << "<br>"
        << "<b>Available:</b> " << boolToYesNo(isAvailable()) << "<br>"
        << "<b>ID:</b> " << getId() << "<br>"
        << "</p>\n";
    return out.str();
}

// Book

Book::Book(const std::string& id, const std::string& title, int year, const std::string& author)
    : CatalogueItem(id, title, year), author(author) {}

std::string Book::getAuthor() const { return author; }
void Book::setAuthor(const std::string& newAuthor) { author = newAuthor; }

std::string Book::toString() const {
    std::ostringstream out;
    out << "<h2>" << toUpper(getTitle()) << "</h2>\n"
        << "<hr>\n"
        << "<p>"
        << "<b>Type:</b> Book<br>"
        << "<b>Author:</b> " << author << "<br>"
        << "<b>Year:</b> " << getYear() << "<br>"
        << "<b>Available:</b> " << boolToYesNo(isAvailable()) << "<br>"
        << "<b>ID:</b> " << getId() << "<br>"
        << "</p>\n";
    return out.str();
}

// FictionBook
FictionBook::FictionBook(const std::string& id, const std::string& title, int year, const std::string& author)
    : Book(id, title, year, author) {}

std::string FictionBook::toString() const {
    std::ostringstream out;
    out << "<h2>" << toUpper(getTitle()) << "</h2>\n"
        << "<hr>\n"
        << "<p>"
        << "<b>Type:</b> Fiction Book<br>"
        << "<b>Author:</b> " << getAuthor() << "<br>"
        << "<b>Year:</b> " << getYear() << "<br>"
        << "<b>Available:</b> " << boolToYesNo(isAvailable()) << "<br>"
        << "<b>ID:</b> " << getId() << "<br>"
        << "</p>\n";
    return out.str();
}

// NonFictionBook

NonFictionBook::NonFictionBook(const std::string& id, const std::string& title, int year,
                               const std::string& author, const std::string& dewey)
    : Book(id, title, year, author), dewey(dewey) {}

std::string NonFictionBook::getDewey() const { return dewey; }
void NonFictionBook::setDewey(const std::string& newDewey) { dewey = newDewey; }

std::string NonFictionBook::toString() const {
    std::ostringstream out;
    out << "<h2>" << toUpper(getTitle()) << "</h2>\n"
        << "<hr>\n"
        << "<p>"
        << "<b>Type:</b> Non-Fiction Book<br>"
        << "<b>Author:</b> " << getAuthor() << "<br>"
        << "<b>Dewey Code:</b> " << dewey << "<br>"
        << "<b>Year:</b> " << getYear() << "<br>"
        << "<b>Available:</b> " << boolToYesNo(isAvailable()) << "<br>"
        << "<b>ID:</b> " << getId() << "<br>"
        << "</p>\n";
    return out.str();
}

// Magazine

Magazine::Magazine(const std::string& id, const std::string& title, int year,
                   int issueNumber, const std::string& pubDateISO)
    : CatalogueItem(id, title, year), issueNumber(issueNumber), pubDateISO(pubDateISO) {}

int Magazine::getIssueNumber() const { return issueNumber; }
std::string Magazine::getPubDate() const { return pubDateISO; }
void Magazine::setIssueNumber(int issue) { issueNumber = issue; }
void Magazine::setPubDate(const std::string& newDate) { pubDateISO = newDate; }

std::string Magazine::toString() const {
    std::ostringstream out;
    out << "<h2>" << toUpper(getTitle()) << "</h2>\n"
        << "<hr>\n"
        << "<p>"
        << "<b>Type:</b> Magazine<br>"
        << "<b>Issue #:</b> " << issueNumber << "<br>"
        << "<b>Publication Date:</b> " << pubDateISO << "<br>"
        << "<b>Year:</b> " << getYear() << "<br>"
        << "<b>Available:</b> " << boolToYesNo(isAvailable()) << "<br>"
        << "<b>ID:</b> " << getId() << "<br>"
        << "</p>\n";
    return out.str();
}

// Movie
Movie::Movie(const std::string& id, const std::string& title, int year,
             const std::string& genre, const std::string& rating)
    : CatalogueItem(id, title, year), genre(genre), rating(rating) {}

std::string Movie::getGenre() const { return genre; }
std::string Movie::getRating() const { return rating; }
void Movie::setGenre(const std::string& newGenre) { genre = newGenre; }
void Movie::setRating(const std::string& newRating) { rating = newRating; }

std::string Movie::toString() const {
    std::ostringstream out;
    out << "<h2>" << toUpper(getTitle()) << "</h2>\n"
        << "<hr>\n"
        << "<p>"
        << "<b>Type:</b> Movie<br>"
        << "<b>Genre:</b> " << genre << "<br>"
        << "<b>Rating:</b> " << rating << "<br>"
        << "<b>Year:</b> " << getYear() << "<br>"
        << "<b>Available:</b> " << boolToYesNo(isAvailable()) << "<br>"
        << "<b>ID:</b> " << getId() << "<br>"
        << "</p>\n";
    return out.str();
}

// VideoGame

VideoGame::VideoGame(const std::string& id, const std::string& title, int year,
                     const std::string& platform, const std::string& genre, const std::string& rating)
    : CatalogueItem(id, title, year), platform(platform), genre(genre), rating(rating) {}

std::string VideoGame::getPlatform() const { return platform; }
std::string VideoGame::getGenre() const { return genre; }
std::string VideoGame::getRating() const { return rating; }

void VideoGame::setPlatform(const std::string& newPlatform) { platform = newPlatform; }
void VideoGame::setGenre(const std::string& newGenre) { genre = newGenre; }
void VideoGame::setRating(const std::string& newRating) { rating = newRating; }

std::string VideoGame::toString() const {
    std::ostringstream out;
    out << "<h2>" << toUpper(getTitle()) << "</h2>\n"
        << "<hr>\n"
        << "<p>"
        << "<b>Type:</b> Video Game<br>"
        << "<b>Platform:</b> " << platform << "<br>"
        << "<b>Genre:</b> " << genre << "<br>"
        << "<b>Rating:</b> " << rating << "<br>"
        << "<b>Year:</b> " << getYear() << "<br>"
        << "<b>Available:</b> " << boolToYesNo(isAvailable()) << "<br>"
        << "<b>ID:</b> " << getId() << "<br>"
        << "</p>\n";
    return out.str();
}
