#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "cover_page.h"
#include "home_page.h"
#include "player_page.h"
#include "search_page.h"
#include "library_page.h"
#include "profile_page.h"

// 枚举改名，避免和类名冲突
enum PageType {
    PageCover,
    PageHome,
    PagePlayer,
    PageSearch,
    PageLibrary,
    PageProfile
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void navigateTo(PageType page, const QString &videoPath = "");

    class HomePage *homePage;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
    void onShowSettings();
    void applyFontSize(int size);
    void applyLanguage(bool isChinese);


private:
    void adjustForScreenSize();

    QStackedWidget *stackedWidget;
    class CoverPage *coverPage;
    class PlayerPage *playerPage;
    class SearchPage *searchPage;
    class LibraryPage *libraryPage;
    class ProfilePage *profilePage;
    int currentFontSize = 12;
    bool isChinese = true;
};


#endif // MAIN_WINDOW_H
