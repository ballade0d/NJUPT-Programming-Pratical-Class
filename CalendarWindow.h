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
    void loadCheckIns();
    bool checkIn();
    bool isTodayCheckedIn();

private:
    QCalendarWidget *calendar;
};

#endif // CALENDARWINDOW_H
