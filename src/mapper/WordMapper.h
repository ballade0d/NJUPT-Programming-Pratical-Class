//
// Created by Dylan Deng on 2024/12/17.
//

#ifndef DEMO_WORDMAPPER_H
#define DEMO_WORDMAPPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include "../entity/Word.h"

class WordMapper {
public:
    static void createWord(int bookId, QString word, QString data) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("INSERT INTO book_word(book_id, word, data) VALUES (:book_id, :word, :data)");
        query.bindValue(":book_id", bookId);
        query.bindValue(":word", word);
        query.bindValue(":data", data);
        query.exec();
    }

    static void deleteWord(int id) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("DELETE FROM book_word WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();
    }

    static void updateWord(Word *word) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("UPDATE book_word SET word = :word, data = :data WHERE id = :id");
        query.bindValue(":id", word->getId());
        query.bindValue(":word", word->getWord());
        query.bindValue(":data", word->getData());
        query.exec();
    }

    static QList<Word *> getWords(int bookId) {
        QSqlDatabase db = QSqlDatabase::database();
        QList<Word *> words;
        QSqlQuery query(db);
        query.prepare("SELECT id, book_id, word, data from book_word WHERE book_id = :book_id");
        query.bindValue(":book_id", bookId);
        query.exec();

        while (query.next()) {
            int id = query.value("id").toInt();
            QString word = query.value("word").toString();
            QString data = query.value("data").toString();

            words.append(new Word(id, bookId, word, data));
        }

        return words;
    }

    static Word *getWord(const QString &spelling, int bookId) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("SELECT id, book_id, word, data from book_word WHERE word = :word AND book_id = :book_id");
        query.bindValue(":word", spelling);
        query.bindValue(":book_id", bookId);
        query.exec();

        if (query.next()) {
            int id = query.value("id").toInt();
            QString data = query.value("data").toString();

            return new Word(id, bookId, spelling, data);
        }

        return nullptr;
    }
};


#endif //DEMO_WORDMAPPER_H
