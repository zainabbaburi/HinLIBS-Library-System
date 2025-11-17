#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <string>
#include <QMessageBox>
#include <QTimer>
#include "Collection.h"
#include "User.h"
#include "FIFO.h"

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindow(CatalogueList& catalogueRef,
                        Patron& currentUser,
                        FIFO& holdQueueRef,
                        QWidget* parent = nullptr);

    ~HomeWindow();

    void checkReadyHolds();

private:
    Ui::HomeWindow* ui;

    CatalogueList& catalogue;
    Patron&        user;
    FIFO&          holdQueue;
};

#endif // HOMEWINDOW_H
