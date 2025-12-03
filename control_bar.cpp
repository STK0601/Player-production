#include "control_bar.h"
#include "the_player.h"
#include <QHBoxLayout>
#include <QStyle>
#include <QScreen>
#include <QGuiApplication>
#include <QKeyEvent>

ControlBar::ControlBar(ThePlayer* player, QWidget *parent)
    : QWidget(parent), player(player) {
    setStyleSheet("background-color: #2c2c2c;");
    setMinimumHeight(40);

    // 初始化控件
    playPauseBtn = new QPushButton(this);
    playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playPauseBtn->setFixedSize(24, 24);
    playPauseBtn->setStyleSheet(R"(
        QPushButton {
            border-radius: 12px;
            background-color: #3a3a3a;
            color: white;
        }
        QPushButton:hover {
            background-color: #4a4a4a;
        }
        QPushButton:pressed {
            background-color: #5a5a5a;
        }
    )");

    progressBar = new QSlider(Qt::Horizontal, this);
    progressBar->setRange(0, 1000);
    progressBar->setStyleSheet(R"(
        QSlider::groove:horizontal {
            background: #555555;
            height: 6px;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #4a90e2;
            width: 16px;
            height: 16px;
            margin: -5px 0;
            border-radius: 8px;
        }
        QSlider::handle:horizontal:hover {
            background: #5b9ef5;
            transform: scale(1.1);
        }
    )");

    // 在ControlBar构造函数的末尾添加（布局和信号连接之后）
    // 安装事件过滤器以捕获键盘事件
    this->setFocusPolicy(Qt::StrongFocus);  // 允许控件接收焦点
    //this->grabKeyboard();  // 捕获键盘事件 这里导致输入栏除了中文，其他都没法输入就很难绷，找你妈半天

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(player->volume());
    volumeSlider->setFixedWidth(45);
    volumeSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            background: #555555;
            height: 4px;
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            background: #ffffff;
            width: 12px;
            height: 12px;
            margin: -4px 0;
            border-radius: 6px;
        }
    )");

    fullscreenBtn = new QPushButton("全屏", this);
    fullscreenBtn->setFixedSize(50, 36);
    fullscreenBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #3a3a3a;
            color: white;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #4a4a4a;
        }
        QPushButton:pressed {
            background-color: #5a5a5a;
        }
    )");


    speedCombo = new QComboBox(this);
    speedCombo->addItems({"0.5x", "1.0x", "1.5x", "2.0x"});
    speedCombo->setCurrentText("1.0x");
    speedCombo->setFixedWidth(45);
    speedCombo->setStyleSheet(R"(
        QComboBox {
            background-color: #3a3a3a;
            color: white;
            border-radius: 4px;
            padding: 5px;
            font-size: 13px;
        }
        QComboBox::drop-down {
            border-left: 1px solid #555;
        }
        QComboBox::down-arrow {
            image: url(:/icons/down_arrow.png);
            width: 8px;
            height: 8px;
        }
        QComboBox QAbstractItemView {
            background-color: #3a3a3a;
            color: white;
            border: 1px solid #555;
        }
        QComboBox QAbstractItemView::item:hover {
            background-color: #4a90e2;
        }
    )");

    timeLabel = new QLabel("00:00/00:00", this);
    timeLabel->setStyleSheet("color: white; font-size: 10px;");
    timeLabel->setMinimumWidth(20);
    timeLabel->setAlignment(Qt::AlignCenter);

    // 布局
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(playPauseBtn);
    layout->addSpacing(10);
    layout->addWidget(progressBar, 1);  // 让进度条占满剩余空间
    layout->addSpacing(10);
    layout->addWidget(volumeSlider);
    layout->addSpacing(10);
    layout->addWidget(speedCombo);
    layout->addSpacing(10);
    layout->addWidget(timeLabel);
    layout->addSpacing(10);
    layout->addWidget(fullscreenBtn);
    layout->setContentsMargins(15, 8, 15, 8);
    layout->setSpacing(5);

    // 连接信号槽
    connect(playPauseBtn, &QPushButton::clicked, this, &ControlBar::onPlayPauseClicked);
    connect(progressBar, &QSlider::sliderMoved, this, &ControlBar::onProgressChanged);
    connect(volumeSlider, &QSlider::valueChanged, this, &ControlBar::onVolumeChanged);
    connect(fullscreenBtn, &QPushButton::clicked, this, &ControlBar::onFullscreenClicked);
    connect(speedCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ControlBar::onSpeedChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &ControlBar::updateProgress);
    connect(player, &QMediaPlayer::durationChanged, this, &ControlBar::updateDuration);

}

void ControlBar::onPlayPauseClicked() {
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    } else {
        player->play();
        playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void ControlBar::onProgressChanged(int value) {
    qint64 duration = player->duration();
    if (duration > 0) {
        player->setPosition((value * duration) / 1000);
    }
}

void ControlBar::onVolumeChanged(int value) {
    player->setVolume(value);
    // 音量图标反馈（可选，需要添加图标资源）
    // if (value == 0) volumeBtn->setIcon(muteIcon);
    // else if (value < 50) volumeBtn->setIcon(lowIcon);
    // else volumeBtn->setIcon(highIcon);
}

void ControlBar::onFullscreenClicked() {
    // 找到最顶层窗口
    QWidget* topLevelWidget = window();
    if (!topLevelWidget) return;

    if (topLevelWidget->isFullScreen()) {
        topLevelWidget->showNormal();
        fullscreenBtn->setText("全屏");
        // 恢复控制栏显示
        this->show();
    } else {
        topLevelWidget->showFullScreen();
        fullscreenBtn->setText("退出全屏");
    }
}


void ControlBar::onSpeedChanged(int index) {
    double speed = 1.0;
    switch (index) {
    case 0: speed = 0.5; break;
    case 1: speed = 1.0; break;
    case 2: speed = 1.5; break;
    case 3: speed = 2.0; break;
    }
    player->setPlaybackRate(speed);
}

void ControlBar::updateProgress(qint64 position) {
    qint64 duration = player->duration();
    if (duration > 0) {
        progressBar->blockSignals(true);
        progressBar->setValue((position * 1000) / duration);
        progressBar->blockSignals(false);
    }
    int posSec = position / 1000;
    int durSec = duration / 1000;
    timeLabel->setText(QString("%1:%2/%3:%4")
                           .arg(posSec / 60, 2, 10, QChar('0'))
                           .arg(posSec % 60, 2, 10, QChar('0'))
                           .arg(durSec / 60, 2, 10, QChar('0'))
                           .arg(durSec % 60, 2, 10, QChar('0')));
}

void ControlBar::updateDuration(qint64 duration) {
    updateProgress(player->position());
}

// 处理键盘按键事件
void ControlBar::keyPressEvent(QKeyEvent *event) {
    // 检测ESC键按下，且当前处于全屏状态
    if (event->key() == Qt::Key_Escape) {
        QWidget* videoWidget = parentWidget();
        if (videoWidget && videoWidget->isFullScreen()) {
            videoWidget->showNormal();  // 退出全屏
            fullscreenBtn->setText("全屏");  // 更新按钮文本
            event->accept();  // 标记事件已处理
            return;
        }
    }
    // 其他按键交给父类处理
    QWidget::keyPressEvent(event);
}

// control_bar.cpp 中实现
void ControlBar::setControlSizes(bool isMobile) {
    playPauseBtn->setFixedSize(isMobile ? 40 : 48, isMobile ? 40 : 48);
    fullscreenBtn->setFixedSize(isMobile ? 60 : 70, isMobile ? 30 : 36);
    speedCombo->setFixedWidth(isMobile ? 60 : 80);
    volumeSlider->setFixedWidth(isMobile ? 60 : 80);
    timeLabel->setMinimumWidth(isMobile ? 60 : 80);
}
