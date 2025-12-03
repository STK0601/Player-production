#include "bottom_navigation_bar.h"
#include <QScreen>
#include <QGuiApplication>
#include <QResizeEvent>

BottomNavigationBar::BottomNavigationBar(QWidget *parent) : QWidget(parent) {
    // 设置导航栏样式（避免 font-size）
    setStyleSheet("background-color: #2c2c2c; border-top: 1px solid #444;");
    setMinimumHeight(60);

    // 创建布局
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 5, 0, 5);
    layout->setSpacing(0);

    // 创建导航按钮
    homeBtn = createNavButton(":/images/home.jpg", "首页");
    searchBtn = createNavButton(":/images/guanzhu.jpg", "关注");
    libraryBtn = createNavButton(":/images/KU.jpg", "库");
    profileBtn = createNavButton(":/images/my.jpg", "我的");

    // 添加按钮到布局
    layout->addWidget(homeBtn, 1);
    layout->addWidget(searchBtn, 1);
    layout->addWidget(libraryBtn, 1);
    layout->addWidget(profileBtn, 1);

    // 连接信号
    connect(homeBtn, &QToolButton::clicked, this, &BottomNavigationBar::homeClicked);
    connect(searchBtn, &QToolButton::clicked, this, &BottomNavigationBar::searchClicked);
    connect(libraryBtn, &QToolButton::clicked, this, &BottomNavigationBar::libraryClicked);
    connect(profileBtn, &QToolButton::clicked, this, &BottomNavigationBar::profileClicked);

    // 初始更新可见性（启动时立即判断）
    updateVisibility();
}

QToolButton* BottomNavigationBar::createNavButton(const QString &iconPath, const QString &text) {
    QToolButton *btn = new QToolButton(this);
    btn->setText(text);
    btn->setStyleSheet(R"(
        QToolButton {
            color: #ffffff;
            background: none;
            border: none;
            padding: 6px;
        }
        QToolButton:hover {
            color: #4a90e2;
        }
        QToolButton:pressed {
            color: #5b9ef5;
        }
    )");
    // 设置图标（若有图标资源）
    if (!iconPath.isEmpty()) {
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(24, 24));
    }
    // 设置按钮布局为上下结构（图标在上，文字在下）
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    return btn;
}

void BottomNavigationBar::updateVisibility() {
    // 优先使用父窗体宽度判断（若存在父窗体）
    bool isMobile = true;
    QWidget *p = parentWidget();
    if (p) {
        isMobile = (p->width() < 600);
    } else {
        // 兜底使用主屏幕宽度
        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
            isMobile = (screen->geometry().width() < 600);
        }
    }

    setVisible(isMobile);
}

void BottomNavigationBar::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateVisibility();
}
