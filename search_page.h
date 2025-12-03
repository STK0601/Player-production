#ifndef SEARCH_PAGE_H
#define SEARCH_PAGE_H

#include <QWidget>
#include <QPushButton>
#include "bottom_navigation_bar.h"

class SearchPage : public QWidget {
    Q_OBJECT

public:
    explicit SearchPage(QWidget *parent = nullptr);
    BottomNavigationBar *bottomNav;

signals:
    void goHome();

private:
    QPushButton *homeBtn;
};

#endif // SEARCH_PAGE_H
