#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <vector>
#include "the_button.h"
#include "bottom_navigation_bar.h"
#include <QScrollArea>

class HomePage : public QWidget {
    Q_OBJECT

public:
    HomePage(QWidget *parent = nullptr);
    void loadVideos(const std::vector<TheButtonInfo> &videos);
    void adjustForScreenSize(bool isMobile);
    void updateBottomNavVisibility();
    void updateLoginButton(const QString &text);
    BottomNavigationBar *bottomNav;

signals:
    void videoSelected(const QString &path);
    void goHome();
    void loginClicked();
    void showSettingsDialog();
    void fontSizeChanged(int size);
    void languageChanged(bool isChinese);


protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void rearrangeGrid(); // 根据当前宽度重排 gridLayout 的列数

    QWidget *header;
    QLineEdit *searchBar;
    QPushButton *homeBtn;
    QPushButton *loginBtn;
    QWidget *videoGrid;
    QGridLayout *gridLayout;
    QWidget *footer;
    QScrollArea *scrollArea;
    QPushButton *settingsBtn;
    // 缓存当前缩略图按钮（便于重排）
    std::vector<QWidget*> videoWidgets;
};

#endif // HOME_PAGE_H
