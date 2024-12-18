#include "CalendarWindow.h"
#include "../mapper/CheckInMapper.h"
#include <QTextCharFormat>
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

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(calendar);

    QPushButton *checkInButton = new QPushButton("今日打卡", this);
    if (isTodayCheckedIn()) {
        checkInButton->setText("今日已打卡");
        checkInButton->setEnabled(false); // 禁用按钮

        // 如果今日已打卡，将当前日期的背景色设置为绿色
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
    QTextCharFormat format;
    format.setBackground(Qt::green);  // 已打卡日期的显示格式
    for (QDate date: CheckInMapper::getAllCheckIns(userId)) {
        calendar->setDateTextFormat(date, format);
    }
}

/**
 * @brief checkIn 打卡
 */
void CalendarWindow::checkIn() {
    QDateTime currentTime = QDateTime::currentDateTime();
    CheckInMapper::checkIn(userId, currentTime.toString("yyyy-MM-dd"));
}

/**
 * @brief isTodayCheckedIn 判断今日是否已打卡
 * @return
 */
bool CalendarWindow::isTodayCheckedIn() {
    QDate today = QDate::currentDate();
    return CheckInMapper::exists(userId, today.toString("yyyy-MM-dd"));
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
