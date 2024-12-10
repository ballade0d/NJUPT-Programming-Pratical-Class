#include "MainWindow.h"

#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    // 创建堆栈窗口
    stackedWidget = new QStackedWidget(this);

    // 创建登录和主界面的实例
    loginWidget = new LoginWidget();
    homeWidget = new HomeWidget();

    stackedWidget->addWidget(loginWidget); // index=0
    stackedWidget->addWidget(homeWidget); // index=1

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(stackedWidget);
    setLayout(layout);

    // 监听登陆成功，切换到主界面
    connect(loginWidget, &LoginWidget::loginSuccessful, this, [this]() {
        stackedWidget->setCurrentIndex(1);
    });
}

MainWindow::~MainWindow() {
}
