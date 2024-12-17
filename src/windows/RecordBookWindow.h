#ifndef RECORDBOOKWINDOW_H
#define RECORDBOOKWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QListView>
#include "../entity/Word.h"

class RecordBookWindow : public QWidget {
Q_OBJECT

public:
    explicit RecordBookWindow(QWidget *parent, int userId);

private slots:

    void updateWordDisplay();

private:
    int userId;
    QListView *listView;
    QList<Word*> record;
    int currentIndex = -1;
    QLabel *wordLabel;
};

#endif // RECORDBOOKWINDOW_H