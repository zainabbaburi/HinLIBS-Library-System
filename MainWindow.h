#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "User.h"
#include "Collection.h"
#include "FIFO.h"
#include "LoanManager.h"

class HomeWindow;
class BrowseWindow;
class AccountWindow;
class DetailedItemWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(CatalogueList& catalogueRef,
                            Patron& currentUser,
                            LoanList& loansRef,
                            FIFO& holdQueueRef,
                            LoanManager& loanManagerRef,
                            QWidget* parent = nullptr);
    ~MainWindow();


signals:
    void logoutRequested();

private slots:
    void onHomeButtonClicked();
    void onBrowseButtonClicked();
    void onAccountButtonClicked();
    void onLogoutButtonClicked();
    void showItemDetails(CatalogueItem& item);

private:
    Ui::MainWindow* ui;
    CatalogueList& catalogue;
    Patron& user;
    LoanList& loans;
    FIFO& holdQueue;
    LoanManager& loanManager;
    
    // Page widgets
    HomeWindow* homeWindow;
    BrowseWindow* browseWindow;
    AccountWindow* accountWindow;
    DetailedItemWindow* detailedItemWindow;
};

#endif // MAINWINDOW_H
