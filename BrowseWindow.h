#ifndef BROWSEWINDOW_H
#define BROWSEWINDOW_H

#include <QWidget>
#include "Collection.h"
#include "User.h"
#include "FIFO.h"
#include "LoanManager.h"
#include "LoanUiHelpers.h"

class QListWidgetItem;
class DetailedItemWindow;
class FIFO;

namespace Ui {
class BrowseWindow;
}

class BrowseWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BrowseWindow(CatalogueList& catalogueRef,
                              Patron& currentUser,
                              LoanList& loansRef,
                              FIFO& holdQueue,
                              LoanManager& loanManagerRef,
                              QWidget *parent = nullptr);
    ~BrowseWindow();

public slots:
    void refreshResults();

signals:
    void detailsRequested(CatalogueItem& item);

private slots:
    void onSearchTextChanged(const QString& text);
    void onDetailsButtonClicked();
    void onCheckoutButtonClicked();
    void onPlaceHoldButtonClicked();
   
private:
    void performSearch(const QString& query);
    
    Ui::BrowseWindow *ui;
    CatalogueList& catalogue;
    LoanList& loans;
    Patron& user;
    FIFO& holdQueue;
    LoanManager& loanManager;
    DetailedItemWindow* detailedItemWindow;
};

#endif // BROWSEWINDOW_H
