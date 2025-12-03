// cover_page.cpp
#include "cover_page.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QStyle>

CoverPage::CoverPage(QWidget *parent) : QWidget(parent) {
    // 设置背景
    setStyleSheet("background-color: #f0f0f0;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 标题
    QLabel *titleLabel = new QLabel("白金 视频播放器");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333; margin: 30px 0;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // 封面图片
    QLabel *coverImage = new QLabel();
    coverImage->setPixmap(QPixmap(":/images/background.jpg").scaled(640, 360, Qt::KeepAspectRatio));
    coverImage->setAlignment(Qt::AlignCenter);

    // 开始按钮
    startBtn = new QPushButton("开始使用");
    startBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #4a90e2;
            color: white;
            font-size: 18px;
            padding: 15px 45px;
            border-radius: 35px;
            margin-top: 30px;
            font-weight: bold;
            text-align: center;
        }
        QPushButton:hover {
            background-color: #5b9ef5;
        }
    )");
    startBtn->setFixedSize(280, 80);


    layout->addStretch();
    layout->addWidget(coverImage);
    layout->addWidget(titleLabel);

    layout->addWidget(startBtn, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(startBtn, &QPushButton::clicked, this, &CoverPage::enterHome);
}
