// player_page.h
#ifndef PLAYER_PAGE_H
#define PLAYER_PAGE_H

#include <QWidget>
#include <QVideoWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include "the_player.h"
#include "control_bar.h"

class PlayerPage : public QWidget {
    Q_OBJECT

public:
    PlayerPage(QWidget *parent = nullptr);
    void loadVideo(const QString &path);
    void adjustForScreenSize(bool isMobile);
    void updateBottomNavVisibility();
    bool eventFilter(QObject *obj, QEvent *event) override;
    void updateLoginButton(const QString &text);
signals:
    void goHome();
    void loginClicked();
    void fontSizeChanged(int size);
    void languageChanged(bool isChinese);

private:
    QWidget *header;
    QPushButton *homeBtn;
    QLineEdit *searchBar;
    QPushButton *loginBtn;
    QVideoWidget *videoWidget;
    ThePlayer *player;
    ControlBar *controlBar;
    QWidget *commentSection;
    QListWidget *commentList;
    QTextEdit *commentInput;
    QPushButton *sendCommentBtn;
};

#endif // PLAYER_PAGE_H
