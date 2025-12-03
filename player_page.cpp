// player_page.cpp
#include "player_page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QKeyEvent>

PlayerPage::PlayerPage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 头部标题栏（与首页一致）
    header = new QWidget();
    header->setStyleSheet("background-color: #2c2c2c; height: 60px;");
    QHBoxLayout *headerLayout = new QHBoxLayout(header);

    homeBtn = new QPushButton("首页");
    homeBtn->setStyleSheet("color: white; background: none; font-size: 16px;");

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
    loginBtn->setStyleSheet("color: white; background: none; font-size: 16px;");

    headerLayout->addSpacing(20);
    headerLayout->addWidget(homeBtn);
    headerLayout->addSpacing(30);
    headerLayout->addWidget(searchBar);
    headerLayout->addSpacing(30);
    headerLayout->addWidget(loginBtn);
    headerLayout->addStretch();

    // 视频播放区
    videoWidget = new QVideoWidget();
    videoWidget->setStyleSheet("background-color: black;");

    // 播放器控制栏
    player = new ThePlayer();
    controlBar = new ControlBar(player, videoWidget);
    player->setVideoOutput(videoWidget);

    // 评论区
    commentSection = new QWidget();
    commentSection->setStyleSheet("background-color: white; border-top: 1px solid #eee;");
    QVBoxLayout *commentLayout = new QVBoxLayout(commentSection);
    commentLayout->setContentsMargins(15, 10, 15, 15);
    commentLayout->setSpacing(10);

    QLabel *commentTitle = new QLabel("评论区");
    commentTitle->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");

    // 添加评论数量显示
    QLabel *commentCount = new QLabel("0 条评论");
    commentCount->setStyleSheet("color: #666; font-size: 12px;");

    // 评论列表美化
    commentList = new QListWidget();
    commentList->setStyleSheet(R"(
    QListWidget {
        border: 1px solid #eee;
        border-radius: 8px;
        background-color: #f9f9f9;
        padding: 5px;
    }
    QListWidget::item {
        background-color: white;
        border-radius: 5px;
        padding: 10px;
        margin-bottom: 8px;
        border: 1px solid #f0f0f0;
    }
)");

    // 评论输入区
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(10);

    commentInput = new QTextEdit();
    commentInput->setStyleSheet(R"(
    QTextEdit {
        border: 1px solid #ccc;
        border-radius: 20px;
        padding: 10px 15px;
        font-size: 14px;
    }
)");
    commentInput->setMinimumHeight(40);
    commentInput->setMaximumHeight(40);
    commentInput->setPlaceholderText("写下你的评论...");

    sendCommentBtn = new QPushButton("发送");
    sendCommentBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #4a90e2;
        color: white;
        padding: 0 20px;
        border-radius: 20px;
        font-size: 14px;
    }
    QPushButton:hover {
        background-color: #5b9ef5;
    }
    QPushButton:pressed {
        background-color: #3a80d2;
    }
    QPushButton:disabled {
        background-color: #ccc;
    }
)");
    sendCommentBtn->setFixedHeight(40);
    // 初始禁用发送按钮
    sendCommentBtn->setDisabled(true);

    inputLayout->addWidget(commentInput);
    inputLayout->addWidget(sendCommentBtn);

    // 组装评论区
    commentLayout->addWidget(commentTitle);
    commentLayout->addWidget(commentCount);
    commentLayout->addWidget(commentList);
    commentLayout->addLayout(inputLayout);

    // 评论输入框信号连接
    connect(commentInput, &QTextEdit::textChanged, [=]() {
        // 根据输入内容是否为空启用/禁用发送按钮
        sendCommentBtn->setDisabled(commentInput->toPlainText().trimmed().isEmpty());
    });

    connect(sendCommentBtn, &QPushButton::clicked, this, [=]() {
        QString comment = commentInput->toPlainText().trimmed();
        if (!comment.isEmpty()) {
            // 添加评论到列表
            QListWidgetItem *item = new QListWidgetItem(comment);
            commentList->addItem(item);
            // 滚动到最新评论
            commentList->scrollToBottom();
            // 清空输入框
            commentInput->clear();
            // 更新评论数量
            commentCount->setText(QString("%1 条评论").arg(commentList->count()));
        }
    });

    // 支持按Enter发送评论
    commentInput->installEventFilter(this);

    // 组装主布局
    mainLayout->addWidget(header);
    mainLayout->addWidget(videoWidget, 1);
    mainLayout->addWidget(controlBar);
    mainLayout->addWidget(commentSection, 0);

    // 连接信号
    connect(homeBtn, &QPushButton::clicked, this, &PlayerPage::goHome);
    connect(sendCommentBtn, &QPushButton::clicked, this, [=]() {
        QString comment = commentInput->toPlainText();
        if (!comment.isEmpty()) {
            commentList->addItem(new QListWidgetItem(comment));
            commentInput->clear();
        }
    });
    connect(loginBtn, &QPushButton::clicked, this, &PlayerPage::loginClicked);
}

void PlayerPage::loadVideo(const QString &path) {
    player->setMedia(QUrl::fromLocalFile(path));
    player->play();
}

// player_page.cpp 中实现
void PlayerPage::adjustForScreenSize(bool isMobile) {
    // 调整控制栏元素大小
    if (controlBar) {
        controlBar->setControlSizes(isMobile);
    }

    // 调整评论区（修改部分）
    if (commentSection) {
        commentSection->setStyleSheet("background-color: white; border-top: 1px solid #eee;");

        if (isMobile) {
            // 手机端评论区保持显示，移除评论按钮相关代码
            commentSection->setMaximumHeight(200);
            commentSection->setVisible(true); // 确保评论区可见
        } else {
            // 桌面端评论区保持原有逻辑
            commentSection->setMaximumHeight(400);
            commentSection->setVisible(true);
        }

        // 清除可能存在的评论按钮
        QPushButton *toggleCommentBtn = findChild<QPushButton*>("toggleCommentBtn");
        if (toggleCommentBtn) {
            delete toggleCommentBtn; // 彻底删除按钮
        }
    }


    // 调整头部
    header->setStyleSheet(isMobile ? "background-color: #2c2c2c; height: 50px;"
                                   : "background-color: #2c2c2c; height: 60px;");
    homeBtn->setStyleSheet(isMobile ? "color: white; background: none; font-size: 14px;"
                                    : "color: white; background: none; font-size: 16px;");
    searchBar->setStyleSheet(isMobile ? R"(
        QLineEdit {
            background-color: #444;
            color: white;
            padding: 6px 10px;
            border-radius: 12px;
            width: 120px;
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
}

void PlayerPage::updateBottomNavVisibility() {
    // 暂时为空实现，可根据需求补充逻辑
}

bool PlayerPage::eventFilter(QObject *obj, QEvent *event) {
    // 检查事件是否来自评论输入框且是键盘按下事件
    if (obj == commentInput && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        // 检查是否按下了 Enter 键（不含 Shift，避免换行）
        if (keyEvent->key() == Qt::Key_Return && !keyEvent->modifiers() & Qt::ShiftModifier) {
            // 触发发送按钮点击
            sendCommentBtn->click();
            return true; // 拦截事件，不进行默认处理（避免换行）
        }
    }
    // 其他事件交给默认处理
    return QWidget::eventFilter(obj, event);
}

void PlayerPage::updateLoginButton(const QString &text) {
    loginBtn->setText(text);
}


