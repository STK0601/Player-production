#ifndef PROFILE_PAGE_H
#define PROFILE_PAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class ProfilePage : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);
    void setLoggedIn(bool loggedIn, const QString &username = "");

signals:
    void goHome();
    void loginStatusChanged(bool loggedIn, const QString &username);

private slots:
    void onLoginClicked();
    void onLogoutClicked();

private:
    QLabel *titleLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginBtn;
    QPushButton *logoutBtn;
    QPushButton *homeBtn;
    bool isLoggedIn;
    QString currentUser;
};

#endif // PROFILE_PAGE_H
