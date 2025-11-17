// window_login.cpp
#include "LoginWindow.h"

LoginWindow::LoginWindow(UserList& usersRef,
                         CatalogueList& catalogueRef,
                         LoanList& loansRef,
                         LoanManager& loanManagerRef,
                         FIFO& holdQueueRef,
                         QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
    , users(usersRef)
    , catalogue(catalogueRef)
    , loans(loansRef)
    , loanManager(loanManagerRef)
    , holdQueue(holdQueueRef)
{
    ui->setupUi(this);

    // Center window
    QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
    this->move(screenGeometry.center() - this->rect().center());

    // Hide login fields
    setLoginFieldsVisible(false);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

// Toggle login field visibility
void LoginWindow::setLoginFieldsVisible(bool visible)
{
    ui->loginButton->setVisible(visible);
    ui->usernameText->setVisible(visible);
    ui->passwordText->setVisible(visible);
    ui->username->setVisible(visible);
    ui->password->setVisible(visible);
}

void LoginWindow::on_startButton_clicked()
{
    // Reset login UI
    setLoginFieldsVisible(false);

    const QString typed = ui->loginLineEdit->text().trimmed();
    Identification ident(typed.toStdString(), users);
    currentUserPtr = ident.findUser();

    if (!currentUserPtr) {
        statusBar()->setStyleSheet("color: red;");
        statusBar()->showMessage("User not found. Please try again.", 3000);
        ui->loginLineEdit->clear();
        ui->loginLineEdit->setFocus();
        currentAccess = -1;
        currentUsername.clear();
        return;
    }

    currentAccess   = currentUserPtr->getAccessLevel();
    currentUsername = typed;

    // Show login fields
    setLoginFieldsVisible(true);

    // Fills userrname and password automatically (as it was not needed for D1)
    if (currentAccess == 1) {
        ui->usernameText->setText("Card Number:");
        ui->passwordText->setText("PIN:");
        ui->username->setText(QString::fromStdString(currentUserPtr->getUsername()));
        ui->password->setText(QString::fromStdString(currentUserPtr->getPassword()));
    } else {
        ui->usernameText->setText("Username:");
        ui->passwordText->setText("Password:");
        ui->username->setText(QString::fromStdString(currentUserPtr->getUsername()));
        ui->password->setText(QString::fromStdString(currentUserPtr->getPassword()));
    }
}

void LoginWindow::on_loginButton_clicked()
{
    const QString inputUsername = ui->username->text().trimmed();
    const QString inputPassword = ui->password->text();

    const QString storedUser = QString::fromStdString(currentUserPtr->getUsername());
    const QString storedPass = QString::fromStdString(currentUserPtr->getPassword());

    // Check login credentials
    if ((inputUsername == storedUser) && (inputPassword == storedPass))
    {
        if (currentAccess == 1) {
            auto patronPtr = std::static_pointer_cast<Patron>(currentUserPtr);
            auto* mainWindow = new MainWindow(catalogue, *patronPtr, loans, holdQueue, loanManager, this);
            connect(mainWindow, &MainWindow::logoutRequested, this, &LoginWindow::show);
            mainWindow->show();
            this->hide();
        }
        else if (currentAccess == 2) {
            auto* librarianWin = new LibrarianWindow();
            librarianWin->show();
            this->hide();
        }
        else if (currentAccess == 3) {
            auto* adminWin = new SysAdminWindow();
            adminWin->show();
            this->hide();
        }

        // Hide and clear fields
        setLoginFieldsVisible(false);
        ui->password->clear();
        ui->loginLineEdit->clear();
    }
    else {
        // Wrong credentials
        statusBar()->setStyleSheet("color: red;");
        statusBar()->showMessage("Invalid credentials. Please try again.", 3000);
        ui->password->clear();
        ui->password->setFocus();
    }
}
