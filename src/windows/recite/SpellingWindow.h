#ifndef SPELLINGWINDOW_H
#define SPELLINGWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QLineEdit>
#include <QKeyEvent>
#include <QPushButton>

class SpellingWindow : public QWidget {
Q_OBJECT

public:
    explicit SpellingWindow(QWidget *parent = nullptr, int bookId = -1);

private slots:

    void handleNextButton();

    void updateWordDisplay();

    bool check();

private:
    QList<QPair<QString, QString>> words;
    QLabel *wordLabel;
    int currentIndex = 0;
    QHBoxLayout *spellingLayout;
    QLabel *iconLabel;
    QPushButton *nextButton;
    QList<QString> correctAnswers;
    QList<QString> wrongAnswers;
};

// 退格监听
class BackspaceFilter : public QObject {
Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Backspace) {
                QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj);
                if (lineEdit) {
                    // 发出请求切换焦点的信号
                    emit requestPreviousFocus();
                    return true;
                }
            }
        }
        return QObject::eventFilter(obj, event);
    }

signals:

    void requestPreviousFocus();
};

#endif // SPELLINGWINDOW_H
