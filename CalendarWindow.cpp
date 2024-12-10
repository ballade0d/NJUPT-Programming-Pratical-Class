#include "CalendarWindow.h"
#include <QTextCharFormat>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPushButton>
#include <QMessageBox>

CalendarWindow::CalendarWindow(QWidget *parent) : QDialog(parent) {
    setModal(true);
    calendar = new QCalendarWidget(this);
    calendar->setGridVisible(true);

    loadCheckIns();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(calendar);

    QPushButton *checkInButton = new QPushButton("今日打卡", this);
    if (isTodayCheckedIn()) {
        checkInButton->setText("今日已打卡");
        checkInButton->setEnabled(false); // 禁用按钮
    }
    layout->addWidget(checkInButton);

    connect(checkInButton, &QPushButton::clicked, this, &CalendarWindow::handleCheckInButton);
}

void CalendarWindow::loadCheckIns() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    query.exec("SELECT date FROM check_in ORDER BY date DESC");

    QTextCharFormat format;
    format.setBackground(Qt::green);  // 已打卡日期的显示格式

    while (query.next()) {
        QDate date = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
        calendar->setDateTextFormat(date, format);
    }
}

bool CalendarWindow::checkIn() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QDateTime currentTime = QDateTime::currentDateTime();

    query.prepare("INSERT INTO check_in (date) VALUES (:date)");
    query.bindValue(":date", currentTime.toString("yyyy-MM-dd"));
    query.exec();

    return true;
}

bool CalendarWindow::isTodayCheckedIn() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QDate today = QDate::currentDate();

    query.prepare("SELECT COUNT(*) FROM check_in WHERE date(date) = :today");
    query.bindValue(":today", today.toString("yyyy-MM-dd"));
    query.exec();

    if (query.next() && query.value(0).toInt() > 0) {
        return true;
    }
    return false;
}

void CalendarWindow::handleCheckInButton() {
    if (checkIn()) {
        QPushButton *button = qobject_cast<QPushButton *>(sender());
        button->setText("今日已打卡");
        button->setEnabled(false); // 禁用按钮

        // 更新日历视图以显示新的签到日期
        QTextCharFormat format;
        format.setBackground(Qt::green);
        calendar->setDateTextFormat(QDateTime::currentDateTime().date(), format);
        QMessageBox::information(this, "每日打卡", "已成功打卡！");
    }
}
