#include "library_page.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>

LibraryPage::LibraryPage(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: #f5f5f5;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 头部
    QWidget *header = new QWidget();
    header->setStyleSheet("background-color: #2c2c2c; height: 60px;");
    QHBoxLayout *headerLayout = new QHBoxLayout(header);

    homeBtn = new QPushButton("首页");
    homeBtn->setStyleSheet("color: white; background: none; font-size: 16px;");

    QLabel *title = new QLabel("我的视频库");
    title->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");

    headerLayout->addSpacing(20);
    headerLayout->addWidget(homeBtn);
    headerLayout->addStretch();
    headerLayout->addWidget(title);
    headerLayout->addStretch();
    headerLayout->addSpacing(20);

    // 内容区
    QLabel *content = new QLabel("本地视频内容将显示在这里");
    content->setStyleSheet("font-size: 16px; color: #666; margin-top: 50px;");
    content->setAlignment(Qt::AlignCenter);

    // 添加底部导航栏
    bottomNav = new BottomNavigationBar(this);

    mainLayout->addWidget(header);
    mainLayout->addWidget(content);
    mainLayout->addStretch();
    mainLayout->addWidget(bottomNav);


    connect(homeBtn, &QPushButton::clicked, this, &LibraryPage::goHome);
    connect(bottomNav, &BottomNavigationBar::homeClicked, this, &LibraryPage::goHome);
    connect(bottomNav, &BottomNavigationBar::searchClicked, [=]() {
        // 当前已经在库页面，可添加高亮逻辑
    });
    connect(bottomNav, &BottomNavigationBar::libraryClicked, [=]() {
        emit goHome();  // 临时返回首页，后续可改为直接跳转到库页面
    });
    connect(bottomNav, &BottomNavigationBar::profileClicked, [=]() {
        emit goHome();  // 临时返回首页，后续可改为直接跳转到个人中心
    });
}
