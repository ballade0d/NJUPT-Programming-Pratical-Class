#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <ui_HomeWidget.h>

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);

signals:
    void loginSuccessful();  // 发送登录成功的信号

private slots:
    void handleCalendarButton(); // 处理打卡按钮
    void handleEditButton(); // 处理编辑按钮

private:
    Ui::HomeWidget ui;
};

#endif // HOMEWIDGET_H
