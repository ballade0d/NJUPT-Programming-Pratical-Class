#ifndef RECORDBOOKWINDOW_H
#define RECORDBOOKWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QListView>

class RecordBookWindow : public QWidget {
Q_OBJECT

public:
    explicit RecordBookWindow(QWidget *parent = nullptr, int userId = -1);

private slots:

    void updateWordDisplay();

private:
    int userId;
    QListView *listView;
    QList<QPair<QString, QVariant>> record;
    int currentIndex = -1;
    QLabel *wordLabel;
};

#endif // RECORDBOOKWINDOW_H