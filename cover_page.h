// cover_page.h
#ifndef COVER_PAGE_H
#define COVER_PAGE_H

#include <QWidget>
#include <QPushButton>

class CoverPage : public QWidget {
    Q_OBJECT

public:
    CoverPage(QWidget *parent = nullptr);

signals:
    void enterHome();

private:
    QPushButton *startBtn;
};

#endif // COVER_PAGE_H
