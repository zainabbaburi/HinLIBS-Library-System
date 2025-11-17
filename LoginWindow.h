#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "Collection.h"
#include "UserFactory.h"
#include "Identification.h"
#include "User.h"
#include "ui_LoginWindow.h"
#include "MainWindow.h"
#include "SysAdminWindow.h"
#include "LibrarianWindow.h"
#include "LoanManager.h"

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <memory>
#include <QScreen>

namespace Ui { class LoginWindow; }

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(UserList& usersRef,
                             CatalogueList& catalogueRef,
                             LoanList& loansRef,
                             LoanManager& loanManagerRef,
                             FIFO& holdQueueRef,
                             QWidget* parent = nullptr);
    ~LoginWindow();

private:
    Ui::LoginWindow *ui;

    UserList&      users;
    CatalogueList& catalogue;
    LoanList&      loans;
    LoanManager& loanManager;
    FIFO& holdQueue;

    std::shared_ptr<User> currentUserPtr{nullptr};
    int currentAccess{-1};
    QString currentUsername;

    void setLoginFieldsVisible(bool visible);

private slots:
        void on_startButton_clicked();
        void on_loginButton_clicked();

};
#endif // LOGINWINDOW_H
