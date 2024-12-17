#include "CalendarWindow.h"
#include <QTextCharFormat>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPushButton>
#include <QMessageBox>

/**
 * @brief CalendarWindow 打卡日历窗口
 * @param parent 父窗口
 * @param userId 用户ID
 */
CalendarWindow::CalendarWindow(QWidget *parent, int userId) : QDialog(parent) {
    setModal(true);
    this->userId = userId;

    calendar = new QCalendarWidget(this);
    calendar->setGridVisible(true);
    calendar->setSelectionMode(QCalendarWidget::NoSelection);

    loadCheckIns();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(calendar);

    QPushButton *checkInButton = new QPushButton("今日打卡", this);
    if (isTodayCheckedIn()) {
        checkInButton->setText("今日已打卡");
        checkInButton->setEnabled(false); // 禁用按钮

        calendar->setStyleSheet(R"(
QCalendarWidget QAbstractItemView {
    selection-background-color: green;
}
        )");
    }
    layout->addWidget(checkInButton);

    connect(checkInButton, &QPushButton::clicked, this, &CalendarWindow::handleCheckInButton);
}

/**
 * @brief loadCheckIns 加载用户的打卡记录
 */
void CalendarWindow::loadCheckIns() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    query.exec("SELECT date FROM check_in WHERE user_id = " + QString::number(userId));

    QTextCharFormat format;
    format.setBackground(Qt::green);  // 已打卡日期的显示格式

    while (query.next()) {
        QDate date = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
        calendar->setDateTextFormat(date, format);
    }
}

/**
 * @brief checkIn 打卡
 */
void CalendarWindow::checkIn() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QDateTime currentTime = QDateTime::currentDateTime();

    query.prepare("INSERT INTO check_in (user_id, date) VALUES (:user_id, :date)");
    query.bindValue(":user_id", userId);
    query.bindValue(":date", currentTime.toString("yyyy-MM-dd"));
    query.exec();
}

/**
 * @brief isTodayCheckedIn 判断今日是否已打卡
 * @return
 */
bool CalendarWindow::isTodayCheckedIn() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QDate today = QDate::currentDate();

    query.prepare("SELECT COUNT(*) FROM check_in WHERE date(date) = :today AND user_id = :user_id");
    query.bindValue(":user_id", userId);
    query.bindValue(":today", today.toString("yyyy-MM-dd"));
    query.exec();

    if (query.next() && query.value(0).toInt() > 0) {
        return true;
    }
    return false;
}

/**
 * @brief handleCheckInButton 处理打卡按钮点击事件
 */
void CalendarWindow::handleCheckInButton() {
    checkIn();
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    button->setText("今日已打卡");
    button->setEnabled(false); // 禁用按钮

    calendar->setStyleSheet(R"(
QCalendarWidget QAbstractItemView {
    selection-background-color: green;
}
        )");

    // 更新日历视图以显示新的签到日期
    QTextCharFormat format;
    format.setBackground(Qt::green);
    calendar->setDateTextFormat(QDateTime::currentDateTime().date(), format);
    QMessageBox::information(this, "每日打卡", "已成功打卡！");
}
