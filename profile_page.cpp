#include "profile_page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ProfilePage::ProfilePage(QWidget *parent) : QWidget(parent), isLoggedIn(false) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 头部返回按钮
    QHBoxLayout *headerLayout = new QHBoxLayout();
    homeBtn = new QPushButton("首页");
    homeBtn->setStyleSheet("color: #4a90e2; background: none; font-size: 16px;");
    headerLayout->addWidget(homeBtn);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // 标题
    titleLabel = new QLabel("个人中心");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 30px 0;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // 登录表单
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("请输入用户名");
    usernameEdit->setStyleSheet(R"(
        QLineEdit {
            padding: 10px;
            border: 1px solid #ccc;
            border-radius: 5px;
            margin-bottom: 15px;
        }
    )");

    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet(usernameEdit->styleSheet());

    loginBtn = new QPushButton("登录");
    loginBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #4a90e2;
            color: white;
            padding: 10px;
            border-radius: 5px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #5b9ef5;
        }
    )");

    logoutBtn = new QPushButton("退出登录");
    logoutBtn->setStyleSheet(loginBtn->styleSheet());
    logoutBtn->setVisible(false);

    mainLayout->addWidget(usernameEdit);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addWidget(loginBtn);
    mainLayout->addWidget(logoutBtn);
    mainLayout->addStretch();

    // 连接信号
    connect(homeBtn, &QPushButton::clicked, this, &ProfilePage::goHome);
    connect(loginBtn, &QPushButton::clicked, this, &ProfilePage::onLoginClicked);
    connect(logoutBtn, &QPushButton::clicked, this, &ProfilePage::onLogoutClicked);
}

void ProfilePage::setLoggedIn(bool loggedIn, const QString &username) {
    isLoggedIn = loggedIn;
    currentUser = username;

    if (loggedIn) {
        usernameEdit->setVisible(false);
        passwordEdit->setVisible(false);
        loginBtn->setVisible(false);
        logoutBtn->setVisible(true);
        titleLabel->setText(QString("欢迎回来，%1").arg(username));
    } else {
        usernameEdit->setVisible(true);
        passwordEdit->setVisible(true);
        loginBtn->setVisible(true);
        logoutBtn->setVisible(false);
        titleLabel->setText("个人中心");
    }
}

void ProfilePage::onLoginClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入用户名和密码");
        return;
    }


    emit loginStatusChanged(true, username);
    setLoggedIn(true, username);

    usernameEdit->clear();
    passwordEdit->clear();
}

void ProfilePage::onLogoutClicked() {
    emit loginStatusChanged(false, "");
    setLoggedIn(false);
}
