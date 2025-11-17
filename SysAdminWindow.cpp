#include "SysAdminWindow.h"
#include "ui_SysAdminWindow.h"

SysAdminWindow::SysAdminWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysAdminWindow)
{
    ui->setupUi(this);
}

SysAdminWindow::~SysAdminWindow()
{
    delete ui;
}
