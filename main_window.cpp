#include "main_window.h"
#include "bottom_navigation_bar.h"
#include "settings_dialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(388, 667);           // 初始窗口大小
    setMinimumSize(388, 667);   // 最小窗口大小
    setWindowTitle("播放器");

    stackedWidget = new QStackedWidget(this);

    // 初始化各页面
    coverPage   = new CoverPage(this);
    homePage    = new HomePage(this);
    playerPage  = new PlayerPage(this);
    searchPage  = new SearchPage(this);
    libraryPage = new LibraryPage(this);
    profilePage = new ProfilePage(this);

    stackedWidget->addWidget(coverPage);
    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(playerPage);
    stackedWidget->addWidget(searchPage);
    stackedWidget->addWidget(libraryPage);
    stackedWidget->addWidget(profilePage);

    setCentralWidget(stackedWidget);

    // ----------------------- 页面切换信号 -----------------------
    connect(coverPage, &CoverPage::enterHome, this, [=]() {
        navigateTo(PageHome);
    });

    connect(homePage, &HomePage::videoSelected, this, [=](const QString &path) {
        navigateTo(PagePlayer);
        playerPage->loadVideo(path);
    });

    connect(homePage, &HomePage::goHome, this, [=]() {
        navigateTo(PageHome);
    });

    connect(playerPage, &PlayerPage::goHome, this, [=]() {
        navigateTo(PageHome);
    });

    // ----------------------- 底部导航栏 -----------------------
    connect(homePage->bottomNav, &BottomNavigationBar::searchClicked, this, [=]() {
        navigateTo(PageSearch);
    });

    connect(homePage->bottomNav, &BottomNavigationBar::libraryClicked, this, [=]() {
        navigateTo(PageLibrary);
    });

    connect(homePage->bottomNav, &BottomNavigationBar::profileClicked, this, [=]() {
        navigateTo(PageProfile);
    });

    // ----------------------- 页面返回首页 -----------------------
    connect(searchPage, &SearchPage::goHome, this, [=]() { navigateTo(PageHome); });
    connect(libraryPage, &LibraryPage::goHome, this, [=]() { navigateTo(PageHome); });
    connect(profilePage, &ProfilePage::goHome, this, [=]() { navigateTo(PageHome); });

    // ----------------------- 登录状态变化同步 -----------------------
    connect(profilePage, &ProfilePage::loginStatusChanged,
            this, [=](bool loggedIn, const QString &username) {
                QString name = loggedIn ? username : "登录";
                homePage->updateLoginButton(name);
                playerPage->updateLoginButton(name);
            });

    // 首页 / 播放页点击登录跳转
    connect(homePage, &HomePage::loginClicked, this, [=]() { navigateTo(PageProfile); });
    connect(playerPage, &PlayerPage::loginClicked, this, [=]() { navigateTo(PageProfile); });

    //设置
    connect(homePage, &HomePage::showSettingsDialog, this, &MainWindow::onShowSettings);
    connect(homePage, &HomePage::fontSizeChanged, this, &MainWindow::applyFontSize);
    connect(homePage, &HomePage::languageChanged, this, &MainWindow::applyLanguage);
}

// ----------------------- 页面切换 -----------------------
void MainWindow::navigateTo(PageType page, const QString &videoPath) {
    stackedWidget->setCurrentIndex(page);
}

// ----------------------- 窗口缩放触发响应式 -----------------------
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    bool isMobile = width() < 600;

    if (homePage)
        homePage->adjustForScreenSize(isMobile);
    if (playerPage)
        playerPage->adjustForScreenSize(isMobile);
}

// ----------------------- 旋转或其他事件处理 -----------------------
void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::OrientationChange) {
        adjustForScreenSize();
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::adjustForScreenSize() {
    bool isMobile = width() < 600;

    if (homePage)
        homePage->adjustForScreenSize(isMobile);
    if (playerPage)
        playerPage->adjustForScreenSize(isMobile);
}

void MainWindow::onShowSettings() {
    SettingsDialog dialog(currentFontSize, isChinese, this);
    if (dialog.exec() == QDialog::Accepted) {
        int newSize = dialog.getFontSize();
        bool newLang = dialog.isChinese();

        if (newSize != currentFontSize) {
            currentFontSize = newSize;
            homePage->fontSizeChanged(newSize);
            playerPage->fontSizeChanged(newSize);
        }

        if (newLang != isChinese) {
            isChinese = newLang;
            homePage->languageChanged(newLang);
            playerPage->languageChanged(newLang);
        }
    }
}

void MainWindow::applyFontSize(int size) {
    // 应用字体大小到整个应用
    QFont font = QApplication::font();
    font.setPointSize(size);
    QApplication::setFont(font);
}

void MainWindow::applyLanguage(bool isChinese) {
    // 这里可以实现实际的语言切换逻辑
    // 实际项目中可能需要使用Qt的翻译框架
    if (isChinese) {
        homePage->setWindowTitle("首页");
        playerPage->setWindowTitle("播放器");
    } else {
        homePage->setWindowTitle("Home");
        playerPage->setWindowTitle("Player");
    }
}
