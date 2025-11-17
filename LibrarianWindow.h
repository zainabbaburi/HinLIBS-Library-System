#ifndef LIBRARIANWINDOW_H
#define LIBRARIANWINDOW_H

#include <QWidget>

namespace Ui {
class LibrarianWindow;
}

class LibrarianWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LibrarianWindow(QWidget *parent = nullptr);
    ~LibrarianWindow();

private:
    Ui::LibrarianWindow *ui;
};

#endif // LIBRARIANWINDOW_H
