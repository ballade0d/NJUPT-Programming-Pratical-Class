#ifndef SETTLEMENTWINDOW_H
#define SETTLEMENTWINDOW_H

#include <QWidget>
#include <QStyledItemDelegate>

class SettlementWindow : public QWidget {
    Q_OBJECT

public:
    explicit SettlementWindow(QWidget *parent,
                              QList<QString> correctAnswers,
                              QList<QString> wrongAnswers);

private slots:

private:
    QList<QString> correctAnswers;
    QList<QString> wrongAnswers;
};

class CustomDelegate : public QStyledItemDelegate {
    QStringList correctAnswers;
    QStringList wrongAnswers;

public:
    CustomDelegate(const QStringList &correctAnswers, const QStringList &wrongAnswers, QObject *parent = nullptr)
            : QStyledItemDelegate(parent), correctAnswers(correctAnswers), wrongAnswers(wrongAnswers) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem optionCopy = option;
        initStyleOption(&optionCopy, index);

        if (correctAnswers.contains(index.data().toString())) {
            optionCopy.palette.setColor(QPalette::Text, Qt::green); // 正确答案绿色
        } else if (wrongAnswers.contains(index.data().toString())) {
            optionCopy.palette.setColor(QPalette::Text, Qt::red); // 错误答案红色
        }

        QStyledItemDelegate::paint(painter, optionCopy, index);
    }
};

#endif //SETTLEMENTWINDOW_H
