#ifndef BOTTOM_NAVIGATION_BAR_H
#define BOTTOM_NAVIGATION_BAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QToolButton>

class BottomNavigationBar : public QWidget {
    Q_OBJECT
public:
    explicit BottomNavigationBar(QWidget *parent = nullptr);
    QToolButton* createNavButton(const QString &iconPath, const QString &text);

    // 根据屏幕尺寸决定是否显示
    void updateVisibility();
    void resizeEvent(QResizeEvent *event) override;

signals:
    void homeClicked();
    void searchClicked();
    void libraryClicked();
    void profileClicked();

private:
    QToolButton *homeBtn;
    QToolButton *searchBtn;
    QToolButton *libraryBtn;
    QToolButton *profileBtn;
};

#endif // BOTTOM_NAVIGATION_BAR_H
