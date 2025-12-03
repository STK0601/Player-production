#include "home_page.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDir>
#include <QResizeEvent>

HomePage::HomePage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 头部标题栏
    header = new QWidget();
    header->setStyleSheet("background-color: #2c2c2c; height: 60px;");
    QHBoxLayout *headerLayout = new QHBoxLayout(header);

    homeBtn = new QPushButton("首页");
    homeBtn->setStyleSheet("color: white; background: none;"); // 去除 font-size

    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("搜索视频...");
    searchBar->setStyleSheet(R"(
        QLineEdit {
            background-color: #444;
            color: white;
            padding: 8px 15px;
            border-radius: 15px;
            width: 400px;
        }
    )");

    loginBtn = new QPushButton("登录");
    loginBtn->setStyleSheet("color: white; background: none;");
    settingsBtn = new QPushButton("设置");
    settingsBtn->setStyleSheet("color: white; background: none; font-size: 16px;");

    headerLayout->addSpacing(20);
    headerLayout->addWidget(homeBtn);
    headerLayout->addSpacing(20);
    headerLayout->addWidget(searchBar);
    headerLayout->addSpacing(20);
    headerLayout->addWidget(loginBtn);
    headerLayout->addSpacing(20);
    headerLayout->addWidget(settingsBtn);
    headerLayout->addStretch();

    // 创建滚动区域
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 滚动区域内容容器
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setContentsMargins(0, 0, 0, 0);
    scrollLayout->setSpacing(0);

    // 视频网格区域
    videoGrid = new QWidget();
    videoGrid->setStyleSheet("background-color: #f5f5f5;");
    gridLayout = new QGridLayout(videoGrid);
    gridLayout->setContentsMargins(20, 20, 20, 20);
    gridLayout->setSpacing(20);

    // 底部栏
    footer = new QWidget();
    footer->setStyleSheet("background-color: #2c2c2c; height: 100px;");
    QVBoxLayout *footerLayout = new QVBoxLayout(footer);

    QLabel *contactLabel = new QLabel("联系方式 | 加盟合作: ");
    contactLabel->setStyleSheet("color: #aaa; margin-top: 20px;"); // 去 font-size
    contactLabel->setAlignment(Qt::AlignCenter);

    QLabel *copyrightLabel = new QLabel("@白金播放器");
    copyrightLabel->setStyleSheet("color: #888; margin-top: 10px;"); // 去 font-size
    copyrightLabel->setAlignment(Qt::AlignCenter);

    footerLayout->addWidget(contactLabel);
    footerLayout->addWidget(copyrightLabel);

    // 底部导航栏（移动端）
    bottomNav = new BottomNavigationBar(this);

    scrollLayout->addWidget(videoGrid, 1);
    scrollLayout->addWidget(footer); // 尾栏放在滚动区域内
    scrollArea->setWidget(scrollContent);

    // 组装主布局
    mainLayout->addWidget(header);
    mainLayout->addWidget(scrollArea, 1);
    mainLayout->addWidget(bottomNav);

    // 连接底部导航信号
    connect(bottomNav, &BottomNavigationBar::homeClicked, this, &HomePage::goHome);
    connect(bottomNav, &BottomNavigationBar::searchClicked, this, [=]() {
        // 处理搜索逻辑
        searchBar->setFocus();
    });
    connect(bottomNav, &BottomNavigationBar::libraryClicked, this, [=]() {
        // 处理库逻辑
    });
    connect(bottomNav, &BottomNavigationBar::profileClicked, this, [=]() {
        // 处理个人中心逻辑
    });

    // 连接信号
    connect(homeBtn, &QPushButton::clicked, this, &HomePage::goHome);
    connect(loginBtn, &QPushButton::clicked, this, &HomePage::loginClicked);
    connect(settingsBtn, &QPushButton::clicked, this, &HomePage::showSettingsDialog);
    // 初始 videoWidgets 为空
    videoWidgets.clear();
}

//: 添加视频缩略图（第一次加载）
void HomePage::loadVideos(const std::vector<TheButtonInfo> &videos) {
    // 如果已有旧的 widgets，先清理父关系（但不 delete widget 对象本身，若你希望删除可做 delete）
    for (QWidget *w : videoWidgets) {
        if (w) {
            gridLayout->removeWidget(w);
            w->setParent(nullptr);
        }
    }
    videoWidgets.clear();

    // 创建新的缩略图 widgets 并缓存
    for (const auto &info : videos) {
        TheButton *btn = new TheButton(videoGrid);
        btn->init(const_cast<TheButtonInfo*>(&info));
        connect(btn, &TheButton::jumpTo, this, [=](TheButtonInfo *info) {
            emit videoSelected(info->url->toLocalFile());
        });
        // 给按钮一个合理的最小尺寸（响应式时会调整）
        btn->setMinimumSize(140, 100);
        btn->setMaximumSize(400, 400);
        videoWidgets.push_back(btn);
    }

    // 根据当前宽度重排
    rearrangeGrid();
}

// 重排 gridLayout 中的 widget（根据 videoGrid 宽度决定列数）
void HomePage::rearrangeGrid() {
    // 计算合适列数（阈值可调）
    int w = videoGrid->width();
    int cols = 1;
    if (w < 480) cols = 1;
    else if (w < 900) cols = 2;
    else cols = 3;

    // 清空 layout 的 item（注意：不 delete widget）
    while (gridLayout->count() > 0) {
        QLayoutItem *item = gridLayout->takeAt(0);
        if (item) {
            QWidget *wgt = item->widget();
            if (wgt) {
                // detach temporarily
                wgt->setParent(nullptr);
            }
            delete item; // 删除 layout item
        }
    }

    // 重新把 widgets 加回 layout，按 cols 排布
    int row = 0, col = 0;
    for (QWidget *w : videoWidgets) {
        if (!w) continue;
        gridLayout->addWidget(w, row, col);
        w->setParent(videoGrid); // 重新设置父控件
        // 设置在不同列数下的大小策略
        if (cols == 1) {
            w->setMinimumSize( videoGrid->width() - 60, 140 );
        } else if (cols == 2) {
            w->setMinimumSize( (videoGrid->width() - 60) / 2, 120 );
        } else {
            w->setMinimumSize( (videoGrid->width() - 80) / 3, 110 );
        }

        col++;
        if (col >= cols) {
            col = 0;
            row++;
        }
    }

    // 更新 spacing/margins 以保证视觉间距合理
    gridLayout->setSpacing(cols == 1 ? 10 : 20);
    gridLayout->setContentsMargins(cols == 1 ? 10 : 20, cols == 1 ? 10 : 20,
                                   cols == 1 ? 10 : 20, cols == 1 ? 10 : 20);

    // Ensure layout updates
    videoGrid->updateGeometry();
    videoGrid->update();
}

// 根据外部传入的 isMobile 标志调整样式/尺寸（保留，供 MainWindow 调用）
void HomePage::adjustForScreenSize(bool isMobile) {
    // 调整网格布局列数（是一个提示，真正的排列在 resizeEvent 或 rearrangeGrid 中处理）
    gridLayout->setSpacing(isMobile ? 10 : 20);
    gridLayout->setContentsMargins(isMobile ? 10 : 20, isMobile ? 10 : 20,
                                   isMobile ? 10 : 20, isMobile ? 10 : 20);

    // 调整搜索栏宽度
    searchBar->setStyleSheet(isMobile ? R"(
        QLineEdit {
            background-color: #444;
            color: white;
            padding: 6px 10px;
            border-radius: 12px;
            width: 180px;
        }
    )" : R"(
        QLineEdit {
            background-color: #444;
            color: white;
            padding: 8px 15px;
            border-radius: 15px;
            width: 400px;
        }
    )");

    // 对已创建的 videoWidgets 调整大小策略
    for (QWidget *widget : videoWidgets) {
        if (!widget) continue;
        if (isMobile) {
            widget->setMinimumSize(140, 100);
            widget->setMaximumSize(400, 400);
        } else {
            widget->setMinimumSize(220, 140);
            widget->setMaximumSize(800, 800);
        }
    }

    // 重新排列
    rearrangeGrid();
}

// 当窗口尺寸发生变化时，重排 grid（动态响应）
void HomePage::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    rearrangeGrid();

    // 更新底部导航可见性（如果需要）
    if (bottomNav) bottomNav->updateVisibility();
}

void HomePage::updateBottomNavVisibility() {
    if (bottomNav) bottomNav->updateVisibility();
}

void HomePage::updateLoginButton(const QString &text) {
    loginBtn->setText(text);
}


