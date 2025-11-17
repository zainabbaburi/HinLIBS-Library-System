#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "HomeWindow.h"
#include "BrowseWindow.h"
#include "AccountWindow.h"
#include "DetailedItemWindow.h"
#include "FIFO.h"

#include <QApplication>
#include <QRect>
#include <QScreen>
#include <QPushButton>

MainWindow::MainWindow(CatalogueList& catalogueRef,
                       Patron& currentUser,
                       LoanList& loansRef,
                       FIFO& holdQueueRef,
                       LoanManager& loanManagerRef,
                       QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , catalogue(catalogueRef)
    , user(currentUser)
    , loans(loansRef)
    , holdQueue(holdQueueRef)
    , loanManager(loanManagerRef)
    , homeWindow(nullptr)
    , browseWindow(nullptr)
    , accountWindow(nullptr)
    , detailedItemWindow(nullptr)
{
    ui->setupUi(this);

    // Resize and center window
    this->resize(1000, 700);
    const QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
    this->move(screenGeometry.center() - this->rect().center());

    // Connect navigation bar buttons
    connect(ui->homeButton, &QPushButton::clicked, this, &MainWindow::onHomeButtonClicked);
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseButtonClicked);
    connect(ui->accountButton, &QPushButton::clicked, this, &MainWindow::onAccountButtonClicked);
    connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutButtonClicked);

    // Create and add page widgets to the stacked widget
    homeWindow   = new HomeWindow(catalogue, user, holdQueue, this);
    browseWindow = new BrowseWindow(catalogue, user, loans, holdQueue, loanManager, this);

    // Connects details page
    connect(browseWindow, &BrowseWindow::detailsRequested,
            this, &MainWindow::showItemDetails);

       accountWindow = new AccountWindow(catalogue, user, loans, holdQueue, loanManager, this);

     ui->stackedWidget->addWidget(homeWindow);
     ui->stackedWidget->addWidget(browseWindow);
     ui->stackedWidget->addWidget(accountWindow);
     ui->stackedWidget->setCurrentWidget(homeWindow);
    // Show home page by default
    ui->stackedWidget->setCurrentWidget(homeWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (detailedItemWindow) {
        delete detailedItemWindow;
    }
}

void MainWindow::onHomeButtonClicked()
{
    ui->stackedWidget->setCurrentWidget(homeWindow);
}

void MainWindow::onBrowseButtonClicked()
{
    ui->stackedWidget->setCurrentWidget(browseWindow);
    browseWindow->refreshResults();
}

void MainWindow::onAccountButtonClicked()
{
    // Refresh account info before showing
    accountWindow->refreshAccountInfo();
    ui->stackedWidget->setCurrentWidget(accountWindow);
}

void MainWindow::onLogoutButtonClicked()
{
    // Emit logout signal so LoginWindow can show itself again
    emit logoutRequested();
    this->close();
}

void MainWindow::showItemDetails(CatalogueItem& item)
{
    // If a details page already exists, remove it so we can bind a new item&
    if (detailedItemWindow) {
        ui->stackedWidget->removeWidget(detailedItemWindow);
        detailedItemWindow->deleteLater();
        detailedItemWindow = nullptr;
    }

    // Create a fresh details window for this item
    detailedItemWindow = new DetailedItemWindow(item, user, loans, holdQueue, loanManager, this);

    // When "Close" is clicked in the details window, go back to Browse
    connect(detailedItemWindow, &DetailedItemWindow::backRequested,
            this, &MainWindow::onBrowseButtonClicked);
    connect(detailedItemWindow, &DetailedItemWindow::accountDataChanged, 
        accountWindow, &AccountWindow::updateLoansList);
    detailedItemWindow->show();

    // Add to stacked widget and switch to it
    ui->stackedWidget->addWidget(detailedItemWindow);
    ui->stackedWidget->setCurrentWidget(detailedItemWindow);
}
