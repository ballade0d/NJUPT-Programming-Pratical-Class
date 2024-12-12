#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QListView>

class HomeWidget : public QWidget {
Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);

    void setup(int userId);

signals:

    void loginSuccessful();  // 发送登录成功的信号

private slots:

    int getSelectedBookId();

    void handleCalendarButton(); // 处理打卡按钮
    void handleEditButton(); // 处理编辑按钮
    void handleLearnButton();

    void handleReciteButton();

    void handleAddButton();

    void handleDeleteButton();

    void handleRecordButton();

    void handleReviewButton();

    void refreshList();

private:
    int userId;
    QListView *listView;
};

#endif // HOMEWIDGET_H
