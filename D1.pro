QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AccountWindow.cpp \
    BrowseWindow.cpp \
    CatalogueItem.cpp \
    Collection.cpp \
    DetailedItemWindow.cpp \
    FIFO.cpp \
    HomeWindow.cpp \
    Identification.cpp \
    LibrarianWindow.cpp \
    Loan.cpp \
    LoanManager.cpp \
    LoginWindow.cpp \
    MainWindow.cpp \
    SysAdminWindow.cpp \
    User.cpp \
    main.cpp

HEADERS += \
    AccountWindow.h \
    BrowseWindow.h \
    CatalogueItem.h \
    Collection.h \
    DetailedItemWindow.h \
    FIFO.h \
    HomeWindow.h \
    Identification.h \
    ItemFactory.h \
    LibrarianWindow.h \
    Loan.h \
    LoanManager.h \
    LoanUiHelpers.h \
    LoginWindow.h \
    MainWindow.h \
    SysAdminWindow.h \
    User.h \
    UserFactory.h

FORMS += \
    AccountWindow.ui \
    BrowseWindow.ui \
    DetailedItemWindow.ui \
    HomeWindow.ui \
    LibrarianWindow.ui \
    LoginWindow.ui \
    MainWindow.ui \
    SysAdminWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
