#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <ui_LoginWidget.h>

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginSuccessful();  // 发送登录成功的信号

private slots:
    void handleLoginButton();  // 处理登录按钮的点击事件
    void handleRegisterButton(); // 处理注册按钮的点击事件

private:
    Ui::LoginWidget ui;
};

#endif // LOGINWIDGET_H
