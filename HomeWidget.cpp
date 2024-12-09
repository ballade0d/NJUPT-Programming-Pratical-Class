#include "HomeWidget.h"
#include "CalendarWindow.h"
#include <QtSql>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QCalendarWidget>

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);

    connect(ui.checkInButton, &QPushButton::clicked, this, &HomeWidget::handleCalendarButton);

}

void HomeWidget::handleCalendarButton(){
    CalendarWindow *calendarWindow = new CalendarWindow(this);
    // 设置窗口标题
    calendarWindow->setWindowTitle("每日打卡");
    // 显示窗口
    calendarWindow->exec();
}
