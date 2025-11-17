#ifndef DETAILEDITEMWINDOW_H
#define DETAILEDITEMWINDOW_H

#pragma once
#include <QWidget>
#include <QMessageBox>

#include "CatalogueItem.h"
#include "Collection.h"
#include "Loan.h"
#include "User.h"
#include "FIFO.h"
#include "LoanManager.h"
#include "LoanUiHelpers.h"


namespace Ui { class DetailedItemWindow; }

class DetailedItemWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DetailedItemWindow(CatalogueItem& item, Patron& user, LoanList& loans, FIFO& holdQueueRef, LoanManager& loanManagerRef, QWidget *parent = nullptr);
    ~DetailedItemWindow();

private:
    Ui::DetailedItemWindow *ui;
    CatalogueItem& currentItem;
    Patron& currentUser;
    LoanList& loans;
    FIFO& holdQueue;
    LoanManager& loanManager;

    void refreshDisplay();
    void updateButtons();

signals:
    void backRequested();
    void accountDataChanged();


private slots:
    void on_checkoutButton_clicked();
    void on_placeHoldButton_clicked();
    void on_closeButton_clicked();
};

#endif // DETAILEDITEMWINDOW_H
