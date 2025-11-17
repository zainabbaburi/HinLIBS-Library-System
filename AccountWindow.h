#ifndef ACCOUNTWINDOW_H
#define ACCOUNTWINDOW_H

#include <QWidget>
#include "Collection.h"
#include "User.h"
#include "LoanManager.h"

class QListWidgetItem;
class FIFO;

namespace Ui {
class AccountWindow;
}

class AccountWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AccountWindow(CatalogueList& catalogueRef,
                           Patron& currentUser,
                           LoanList& loansRef,
                           FIFO& holdQueueRef,
                           LoanManager& loanManager,
                           QWidget *parent = nullptr);

    ~AccountWindow();

    void refreshAccountInfo();
    void updateLoansList();
    void updateHoldsList();
signals:
	void accountDataChanged();

private slots:
    void onReturnButtonClicked();
    void onCancelHoldButtonClicked();

private:
    void loadCheckedOutItems();
    void loadHolds();

    Ui::AccountWindow *ui;
    CatalogueList& catalogue;
    Patron& user;
    LoanList& loans;
    FIFO& holdQueue;
    LoanManager& loanManager;
};

#endif // ACCOUNTWINDOW_H
