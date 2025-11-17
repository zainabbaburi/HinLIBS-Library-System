# HinLIBS - Deliverable 1 (D1)

This repository contains the HinLIBS Deliverable 1 implementation. This is a Qt-based in-memory prototype demonstrating Patron features for borrowing, holding, and returning items.

## What’s included
- Qt C++ project with a GUI built using Qt Widgets
- In-memory data only — no database 
- Default startup data (20 catalogue items, 7 users - 5 patrons + librarian + sysadmin)
- Core Patron features implemented: browse, borrow, return, place hold, cancel hold, view account

## Startup default data
You can use either the full name or the username (card number) on the initial entry screen to look yourself up. After clicking Start, the login form is populated with the user credentials. Click Login to enter the main UI.

Patrons (card number / PIN):
- Alice — username/card number: `1001`, PIN: `1234`
- Brandon — username/card number: `1002`, PIN: `5678`
- Chloe — username/card number: `1003`, PIN: `9101`
- Darius — username/card number: `1004`, PIN: `1314`
- Eve — username/card number: `1005`, PIN: `1516`


## Build Instructions
1. Extract the ZIP.
2. Open `D1.pro` in Qt Creator:
	- `File -> Open File or Project -> select D1.pro`

## How to use
1. Run the application
2. On the startup screen, enter a `name`, `username`, or `card number` then press **Start**.
3. For simplicity, the login fields will be filled with the stored username/PIN; press **Login**. You can also test invalid logins by changing these fields.
4. The **MainWindow** opens with options including Browse, Account, and Home.

Patron actions to test:
- Browse Catalogue: View all items, search by title, and open item details
- Borrow Item: From the Detailed Item view, click **Check Out** (limits enforced: max 3 active loans; loan period = 14 days)
- Return Item: From Account -> Checked Out items, click **Return** to process returns
- Place Hold: If an item is checked out, click **Place Hold** to add to the FIFO hold queue
- Cancel Hold: From Account -> Holds, cancel a previously placed hold
- View Account: Shows loans (title, due date, days remaining) and holds (title, queue position)
- Logout: Return to the login screen