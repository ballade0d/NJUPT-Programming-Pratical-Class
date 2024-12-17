#ifndef LEARNWINDOW_H
#define LEARNWINDOW_H

#include <QWidget>
#include <QLabel>
#include "../entity/Word.h"

class LearnWindow : public QWidget {
Q_OBJECT

public:
    explicit LearnWindow(QWidget *parent, int bookId);

private slots:

    void showPreviousWord();

    void showNextWord();

    void updateWordDisplay();


private:
    QList<Word*> words;
    QLabel *wordLabel;
    int currentIndex = 0;
};

#endif // LEARNWINDOW_H
