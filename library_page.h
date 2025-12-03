#ifndef LIBRARY_PAGE_H
#define LIBRARY_PAGE_H

#include <QWidget>
#include <QPushButton>
#include "bottom_navigation_bar.h"

class LibraryPage : public QWidget {
    Q_OBJECT

public:
    explicit LibraryPage(QWidget *parent = nullptr);
    BottomNavigationBar *bottomNav;

signals:
    void goHome();

private:
    QPushButton *homeBtn;
};

#endif // LIBRARY_PAGE_H
