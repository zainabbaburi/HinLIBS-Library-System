#include "LibrarianWindow.h"
#include "ui_LibrarianWindow.h"

LibrarianWindow::LibrarianWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibrarianWindow)
{
    ui->setupUi(this);
}

LibrarianWindow::~LibrarianWindow()
{
    delete ui;
}
