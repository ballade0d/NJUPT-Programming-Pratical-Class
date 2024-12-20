#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QMainWindow>
#include "../widgets/LoginWidget.h"
#include "../widgets/HomeWidget.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent);

    ~MainWindow();

private:
    QStackedWidget *stackedWidget;
    LoginWidget *loginWidget;
    HomeWidget *homeWidget;
};

#endif // MAINWINDOW_H
