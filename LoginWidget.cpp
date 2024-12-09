#include "LoginWidget.h"
#include <QtSql>
#include <QCryptographicHash>
#include <QMessageBox>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.loginButton, &QPushButton::clicked, this, &LoginWidget::handleLoginButton);
    connect(ui.registerButton, &QPushButton::clicked, this, &LoginWidget::handleRegisterButton);
}

// 将用户输入的密码进行加密
QString encryptPassword(const QString &password){
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString hashedPassword = QString::fromUtf8(hash.toHex());
    return hashedPassword;
}

bool checkPassword(const QString &username, const QString &password) {
    QSqlDatabase db = QSqlDatabase::database();  // 获取数据库连接
    QSqlQuery query(db);

    // 使用预处理语句防止SQL注入
    query.prepare("SELECT password FROM user_password WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();

    if (query.next()) {
        // 从数据库中获取加密的密码
        QString storedPassword = query.value(0).toString();

        // 与数据库中的加密密码进行比较
        if (encryptPassword(password) == storedPassword) {
            return true;
        }
    }
    return false;
}

void LoginWidget::handleLoginButton() {
    // 从输入框中获取账号密码
    const QString username = ui.usernameLineEdit->text();
    const QString password = ui.passwordLineEdit->text();

    if (checkPassword(username, password)){
        QMessageBox::information(nullptr, "信息", "登陆成功！");
        emit loginSuccessful();
    }else{
        QMessageBox::critical(nullptr, "错误", "用户名或密码错误！");
    }
}

bool checkIfUserExists(const QString &username, QSqlDatabase &db) {
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM user_password WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();

    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}

void LoginWidget::handleRegisterButton() {
    // 从输入框中获取账号密码
    const QString username = ui.usernameLineEdit->text();
    const QString password = ui.passwordLineEdit->text();

    QSqlDatabase db = QSqlDatabase::database();
    if (checkIfUserExists(username, db)){
        QMessageBox::critical(nullptr, "错误", "用户名已存在，请登陆！");
    } else {
        QSqlQuery query(db);
        query.prepare("INSERT INTO user_password(username, password) VALUES (:username, :password)");
        query.bindValue(":username", username);
        query.bindValue(":password", encryptPassword(password));

        if(query.exec()){
            QMessageBox::information(nullptr, "信息", "注册成功！");
            emit loginSuccessful();
        }
    }
}
