#include "LoginWidget.h"
#include "../entity/User.h"
#include "../mapper/UserMapper.h"
#include <QCryptographicHash>
#include <QMessageBox>

/**
 * @brief LoginWidget 登陆窗口
 * @param parent 父窗口
 */
LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent) {
    // 创建用户名输入框
    usernameLineEdit = new QLineEdit(this);
    // 居中对齐, 并留出足够空间
    usernameLineEdit->setGeometry((480 - 200) / 2, 200, 200, 32);
    usernameLineEdit->setPlaceholderText("用户名");

    // 创建密码输入框
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    // 紧接用户名输入框
    passwordLineEdit->setGeometry((480 - 200) / 2, 200 + 40, 200, 32);
    passwordLineEdit->setPlaceholderText("密码");

    // 创建登陆按钮
    QPushButton * loginButton = new QPushButton(this);
    // 位于密码输入框下方
    loginButton->setGeometry((480 - 100) / 2, 200 + 40 + 40, 100, 32);
    loginButton->setText("登陆");
    loginButton->setObjectName("loginButton");

    // 创建注册按钮
    QPushButton * registerButton = new QPushButton(this);
    // 位于登录按钮下方
    registerButton->setGeometry((480 - 150) / 2, 200 + 40 + 40 + 40, 150, 32);
    registerButton->setText("新用户注册");
    registerButton->setObjectName("registerButton");

    QString styleSheet = R"(
/* 输入框样式 */
QLineEdit {
    color: black;
    background-color: #ffffff;
    border: 1px solid #ccc;
    border-radius: 5px;
    padding: 5px;
    font-size: 14px;
}

QLineEdit::placeholder {
    color: gray; /* 占位符文本颜色 */
}

QLineEdit:focus {
    border: 1px solid #0078d7;
    background-color: #f0f8ff;
}

/* 按钮样式 */
QPushButton {
    background-color: #0078d7;
    color: #ffffff;
    border: none;
    border-radius: 5px;
    padding: 5px 10px;
    font-size: 14px;
}

QPushButton:hover {
    background-color: #005bb5;
}

QPushButton:pressed {
    background-color: #003f8c;
    padding-left: 6px;
    padding-top: 6px;
}

/* 登录按钮图标 */
QPushButton#loginButton {
    background-color: #28a745;
}

QPushButton#loginButton:hover {
    background-color: #218838;
}

QPushButton#loginButton:pressed {
    background-color: #1e7e34;
}

/* 新用户注册按钮 */
QPushButton#registerButton {
    background-color: #6c757d;
}

QPushButton#registerButton:hover {
    background-color: #5a6268;
}

QPushButton#registerButton:pressed {
    background-color: #4e555b;
}
        )";
    setStyleSheet(styleSheet);

    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::handleLoginButton);
    connect(registerButton, &QPushButton::clicked, this, &LoginWidget::handleRegisterButton);
}

// 将用户输入的密码进行加密
QString encryptPassword(const QString &password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString hashedPassword = QString::fromUtf8(hash.toHex());
    return hashedPassword;
}

int checkPassword(const QString &username, const QString &password) {
    User *user = UserMapper::getUserByName(username);
    if (user == nullptr) {
        return -1;
    }
    if (encryptPassword(password) == user->getPassword()) {
        return user->getId();
    }
    return -1;
}

void LoginWidget::handleLoginButton() {
    // 从输入框中获取账号密码
    const QString username = usernameLineEdit->text();
    const QString password = passwordLineEdit->text();

    int id = checkPassword(username, password);

    if (id != -1) {
        QMessageBox::information(nullptr, "信息", "登陆成功！");
        emit loginSuccessful(id);
    } else {
        QMessageBox::critical(nullptr, "错误", "用户名或密码错误！");
    }
}

void LoginWidget::handleRegisterButton() {
    // 从输入框中获取账号密码
    const QString username = usernameLineEdit->text();
    const QString password = passwordLineEdit->text();

    if (username.length() < 3 || password.length() < 6) {
        QMessageBox::critical(nullptr, "错误", "密码长度不能小于6位！");
        return;
    }

    if (UserMapper::exists(username)) {
        QMessageBox::critical(nullptr, "错误", "用户名已存在，请登陆！");
    } else {
        UserMapper::createUser(username, encryptPassword(password));
        QMessageBox::information(nullptr, "信息", "注册成功！请登录");
    }
}
