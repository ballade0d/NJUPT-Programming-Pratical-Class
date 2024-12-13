#include "MainWindow.h"

#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 创建中心窗口部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建堆栈窗口
    stackedWidget = new QStackedWidget(this);
    // 设置背景图片
    stackedWidget->setStyleSheet(R"(
QStackedWidget {
    background-image: url(:/background.png);
})");

    // 创建登录和主界面的实例
    loginWidget = new LoginWidget();
    loginWidget->setObjectName("LoginWidget");
    homeWidget = new HomeWidget();
    homeWidget->setObjectName("HomeWidget");

    stackedWidget->addWidget(loginWidget); // index=0
    stackedWidget->addWidget(homeWidget); // index=1

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(stackedWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    centralWidget->setLayout(layout);

    // 监听登陆成功，切换到主界面
    connect(loginWidget, &LoginWidget::loginSuccessful, this, [this](int userId) {
        homeWidget->setup(userId);     // 初始化 homeWidget
        stackedWidget->setCurrentIndex(1); // 切换到主界面
        stackedWidget->setStyleSheet(R"(
QStackedWidget {
    background-image: none;
})");
    });
}

MainWindow::~MainWindow() {
}
