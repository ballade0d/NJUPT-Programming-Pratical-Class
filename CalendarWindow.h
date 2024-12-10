#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <QDialog>
#include <QCalendarWidget>
#include <QVBoxLayout>

class CalendarWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CalendarWindow(QWidget *parent = nullptr);

private slots:
    void handleCheckInButton(); // 处理打卡按钮
    void loadCheckIns(); // 加载已打卡日期
    bool checkIn(); // 打卡
    bool isTodayCheckedIn(); // 判断今日是否打卡

private:
    QCalendarWidget *calendar;
};

#endif // CALENDARWINDOW_H
