#include "LoginWindow.h"
#include "UserFactory.h"
#include "ItemFactory.h"
#include "Collection.h"
#include "LoanManager.h"
#include "FIFO.h"
#include "User.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Factories
    UserFactory userFactory;
    ItemFactory itemFactory;

    // Lists
    UserList users;
    LoanList loans;
    CatalogueList catalogue;
    FIFO holdQueue;

    // Managers
    LoanManager loanManager(loans, catalogue, holdQueue, users);
   
    // 5 Patrons (ID, Name, Cardnumber(username), Password, Phonenumber, Email)
    users.add(userFactory.createPatron("p1", "Alice",   "1001", "1234", "613-555-0101", "alice@example.com"));
    users.add(userFactory.createPatron("p2", "Brandon", "1002", "5678", "613-555-0102", "brandon@example.com"));
    users.add(userFactory.createPatron("p3", "Chloe",   "1003", "9101", "613-555-0103", "chloe@example.com"));
    users.add(userFactory.createPatron("p4", "Darius",  "1004", "1314", "417-555-0104", "darius@example.com"));
    users.add(userFactory.createPatron("p5", "Eve",     "1005", "1516", "705-555-0105", "eve@example.com"));

    // 1 Librarian (ID, Name, Username, Password)
    users.add(userFactory.createLibrarian("l1", "Lydia", "lib_lydia", "libpass"));

    // 1 SysAdmin (ID, Name, Username, Password)
    users.add(userFactory.createSysAdmin("s1", "Sam", "samroot", "rootpass"));


    // Add 20 Catalogue Items (5 Fiction, 5 Non-Fiction, 3 Magazines, 3 Movies, 4 Video Games)

    // --- Fiction Books (5) ---
    
    catalogue.add(itemFactory.createFictionBook("F1", "The Lost Horizon", 2018, "James Ford"));
    catalogue.add(itemFactory.createFictionBook("F2", "Midnight Rain", 2019, "Sarah Lin"));
    catalogue.add(itemFactory.createFictionBook("F3", "Glass Kingdom", 2020, "Arthur Wells"));
    catalogue.add(itemFactory.createFictionBook("F4", "River of Stars", 2022, "Emily North"));
    catalogue.add(itemFactory.createFictionBook("F5", "Songs of Earth", 2023, "Kai Shimizu"));

    // --- Non-Fiction Books (5) ---
    catalogue.add(itemFactory.createNonFictionBook("NF1", "Quantum Reality", 2021, "Dr. Harper", "530.12"));
    catalogue.add(itemFactory.createNonFictionBook("NF2", "History of Civilizations", 2017, "M. Clarkson", "909"));
    catalogue.add(itemFactory.createNonFictionBook("NF3", "Wildlife of the North", 2015, "Nora Feldman", "590.72"));
    catalogue.add(itemFactory.createNonFictionBook("NF4", "The Art of Baking", 2020, "Ava Chen", "641.815"));
    catalogue.add(itemFactory.createNonFictionBook("NF5", "Understanding Economics", 2018, "Dr. Peterson", "330"));

    // --- Magazines (3) ---
    catalogue.add(itemFactory.createMagazine("M1", "National Geographic", 2024, 112, "2024-05-15"));
    catalogue.add(itemFactory.createMagazine("M2", "Science Today", 2023, 87,  "2023-11-01"));
    catalogue.add(itemFactory.createMagazine("M3", "Tech World", 2025, 32,  "2025-01-20"));

    // --- Movies (3) ---
    catalogue.add(itemFactory.createMovie("MV1", "Inception", 2010, "Sci-Fi", "PG-13"));
    catalogue.add(itemFactory.createMovie("MV2", "Spirited Away", 2001, "Fantasy", "PG"));
    catalogue.add(itemFactory.createMovie("MV3", "Interstellar", 2014, "Sci-Fi", "PG-13"));

    // --- Video Games (4) ---
    catalogue.add(itemFactory.createVideoGame("VG1", "The Legend of Zelda", 2017, "Switch", "Adventure", "E"));
    catalogue.add(itemFactory.createVideoGame("VG2", "Elden Ring", 2022, "PS5",    "RPG",      "M"));
    catalogue.add(itemFactory.createVideoGame("VG3", "Stardew Valley", 2016, "PC",     "Simulation","E"));
    catalogue.add(itemFactory.createVideoGame("VG4", "Horizon Zero Dawn", 2017, "PS4",    "Action",   "T"));

    // Start App
    LoginWindow w(users, catalogue, loans, loanManager, holdQueue);
    w.show();

    return a.exec();
}

