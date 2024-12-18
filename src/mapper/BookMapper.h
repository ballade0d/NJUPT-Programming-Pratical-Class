//
// Created by Dylan Deng on 2024/12/17.
//

#ifndef DEMO_BOOKMAPPER_H
#define DEMO_BOOKMAPPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include "../entity/Book.h"


class BookMapper {
public:
    static int createBook(int userId, QString name) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("INSERT INTO book(user_id, name) VALUES (:user_id, :name)");
        query.bindValue(":user_id", userId);
        query.bindValue(":name", name);
        query.exec();

        return query.lastInsertId().toInt();
    }

    static void deleteBook(int id) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("DELETE FROM book WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();
    }

    static Book *getBookById(int id) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("SELECT * FROM book WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();
        if (query.next()) {
            return new Book(query.value("id").toInt(), query.value("user_id").toInt(), query.value("name").toString());
        }
        return nullptr;
    }

    static Book *getBookByName(int userId, QString name) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("SELECT * FROM book WHERE user_id = :user_id AND name = :name");
        query.bindValue(":user_id", userId);
        query.bindValue(":name", name);
        query.exec();
        if (query.next()) {
            return new Book(query.value("id").toInt(), query.value("user_id").toInt(), query.value("name").toString());
        }
        return nullptr;
    }

    static QList<Book *> getAllBooks(int userId) {
        QSqlDatabase db = QSqlDatabase::database();
        QList<Book *> books;
        QSqlQuery query(db);
        query.prepare("SELECT * FROM book WHERE user_id = :user_id");
        query.bindValue(":user_id", userId);
        query.exec();
        while (query.next()) {
            books.append(new Book(query.value("id").toInt(), query.value("user_id").toInt(),
                                  query.value("name").toString()));
        }
        return books;
    }
};


#endif //DEMO_BOOKMAPPER_H
