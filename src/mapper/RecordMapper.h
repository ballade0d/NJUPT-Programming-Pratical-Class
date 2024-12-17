//
// Created by Dylan Deng on 2024/12/17.
//

#ifndef DEMO_RECORDMAPPER_H
#define DEMO_RECORDMAPPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include "../entity/Word.h"

class RecordMapper{
public:
    static QList<Word*> getWords(int userId){
        QSqlDatabase db = QSqlDatabase::database();
        QList<Word*> words;
        QSqlQuery query(db);
        query.prepare("SELECT bw.id, bw.book_id, bw.word, bw.data from record r JOIN book_word bw ON r.word_id = bw.id WHERE r.user_id = :user_id");
        query.bindValue(":user_id", userId);
        query.exec();

        while (query.next()) {
            int id = query.value("id").toInt();
            int bookId = query.value("book_id").toInt();
            QString word = query.value("word").toString();
            QString data = query.value("data").toString();

            words.append(new Word(id, bookId, word, data));
        }

        return words;
    }

    static void addWord(int userId, Word *word){
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("INSERT INTO record(user_id, book_id, word_id) VALUES (:user_id, :book_id, :word_id)");
        query.bindValue(":user_id", userId);
        query.bindValue(":book_id", word->getBookId());
        query.bindValue(":word_id", word->getId());
        query.exec();
    }

    static void removeWord(int userId, Word *word){
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("DELETE FROM record WHERE user_id = :user_id AND word_id = :word_id");
        query.bindValue(":user_id", userId);
        query.bindValue(":word_id", word->getId());
        query.exec();
    }

    static bool exists(int userId, Word *word){
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) FROM record WHERE user_id = :user_id AND word_id = :word_id");
        query.bindValue(":user_id", userId);
        query.bindValue(":word_id", word->getId());
        query.exec();
        return query.next() && query.value(0).toInt() > 0;
    }
};

#endif //DEMO_RECORDMAPPER_H
