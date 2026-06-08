HinLIBS — Desktop Library Management System
A robust, multi-role desktop application built using C++ and the Qt framework to simulate enterprise library operations, account workflows, and real-time inventory control.

Project Evolution & Milestones
🔹 Milestone 1: In-Memory Patron Prototype (D1)
The initial stage focused on client-side business logic and essential user interactions without data persistence:

Architecture: In-memory data management models using standard collections.

Core Patron Workflows: Enabled browsing catalog systems, item checkouts (max 3 loan limits enforced), FIFO hold placement/cancellations, and active account tracking.

🔹 Milestone 2: SQLite Persistence & Multi-Role Admin UI (D2) — Current Build
The application was refactored to support enterprise-level admin operations and structural persistence:

Database Integration: Replaced in-memory data structures with a persistent SQLite relational database.

Advanced Architecture: Implemented the Factory Design Pattern via UserFactory and ItemFactory to cleanly instantiate polymorphic user types (Patron, Librarian, SysAdmin) and media subclasses.

Librarian Capabilities: Integrated comprehensive multi-attribute patron lookup, real-time inventory returns management, and data-tier permanent/temporary asset removal hooks.

Technical Stack
Language: C++ (OOP, Const correctness, Smart Pointers)

Framework: Qt Widgets (GUI development, Signal/Slot architecture)

Database: SQLite (Persistent relational data tier)

System Capabilities Matrix
User Role	Implemented Capabilities
Patron	Browse catalogue, check out items (14-day loan period), place/cancel FIFO holds, track active fine balances.
Librarian	Case-insensitive search filters for patrons/inventory, process dynamic returns, permanently or temporarily take items offline (maintaining active hold queues).
SysAdmin	Access system infrastructure windows and manage global security policies.
Build and Run Instructions
Clone this repository to your local directory.

Open the project profile configuration file (D2.pro) inside Qt Creator.

Select your desktop compiler kit, run qmake, and execute Build & Run.

Default Testing Credentials
The database comes pre-populated with a default test suite (20 catalogue items, 7 user profiles):

Patron Credentials: Card Number: 1001 | PIN: 1234 (Alice)

Admin Roles: Accessible via standard Librarian and SysAdmin portal routes.

 Team & Contributions
This system was developed as a collaborative 4-person academic project.

My Core Focus: Implemented backend database interaction components, integrated input sanitization routines, and engineered core business logic handlers (including the case-insensitive lookup systems and real-time filtration engines).

Team Members: Megan P., Shawn M., and Ryan A.
