#include "MultipleChoiceWindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>

MultipleChoiceWindow::MultipleChoiceWindow(QWidget *parent, int bookId) : QWidget(parent) {
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query(db);
    query.prepare("SELECT word, data from book_word WHERE book_id = :book_id");
    query.bindValue(":book_id", bookId);
    query.exec();

    while (query.next()){
        QString word = query.value(0).toString();
        QString data = query.value(1).toString();
        words.append(qMakePair(word, data));
    }
}

void MultipleChoiceWindow::updateWordDisplay(){

}
bool MultipleChoiceWindow::check(){

}
