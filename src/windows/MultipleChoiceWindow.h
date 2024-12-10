#ifndef MULTIPLECHOICEWINDOW_H
#define MULTIPLECHOICEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QLineEdit>
#include <QKeyEvent>
#include <QPushButton>

class MultipleChoiceWindow : public QWidget {
Q_OBJECT

public:
    explicit MultipleChoiceWindow(QWidget *parent = nullptr, int bookId = -1);

private slots:

    void updateWordDisplay();

    bool check();

private:
    QList<QPair<QString, QString>> words;
    QLabel *wordLabel;
    int currentIndex = 0;
    QHBoxLayout *choiceLayout;
    QLabel *iconLabel;
};

#endif // MULTIPLECHOICEWINDOW_H
