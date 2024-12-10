#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include "LoginWidget.h"
#include "HomeWidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stackedWidget;
    LoginWidget *loginWidget;
    HomeWidget *homeWidget;
};
#endif // MAINWINDOW_H
