#ifndef MULTIPLECHOICEWINDOW_H
#define MULTIPLECHOICEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QLineEdit>
#include <QKeyEvent>
#include <QPushButton>
#include "../../entity/Word.h"

class MultipleChoiceWindow : public QWidget {
Q_OBJECT

public:
    explicit MultipleChoiceWindow(QWidget *parent, int userId, int bookId);

private slots:

    void updateWordDisplay();

    void handleButtonClick();

private:
    int bookId;
    int userId;
    QList<Word*> words;
    QLabel *wordLabel;
    int currentIndex = 0;
    QButtonGroup *buttonGroup;
    QPushButton *confirmButton;
    int correctIndex;
    QLabel *iconLabel;
    QList<QString> correctAnswers;
    QList<QString> wrongAnswers;
};

#endif // MULTIPLECHOICEWINDOW_H
