#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include "../entity/Word.h"
#include <QDialog>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QListView>

class EditWindow : public QDialog {
Q_OBJECT

public:
    explicit EditWindow(QWidget *parent, int bookId);

private slots:

    Word* getSelectedWord();

    void handleAddButton();

    void handleEditButton();

    void handleDeleteButton();

    void refreshList();

private:
    QListView *wordList;
    int bookId;
};

#endif // EDITWINDOW_H
