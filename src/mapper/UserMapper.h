//
// Created by Dylan Deng on 2024/12/17.
//

#ifndef DEMO_USERMAPPER_H
#define DEMO_USERMAPPER_H

#include <QString>
#include <QSqlQuery>
#include "../entity/User.h"

class UserMapper {
public:
    static User *getUserByName(const QString &username) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("SELECT id, password FROM user_password WHERE username = :username");
        query.bindValue(":username", username);
        query.exec();

        if (query.next()) {
            int id = query.value("id").toInt();
            QString password = query.value("password").toString();
            return new User(id, username, password);
        }

        return nullptr;
    }

    static void createUser(const QString &username, const QString &password) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("INSERT INTO user_password(username, password) VALUES (:username, :password)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);

        query.exec();
    }

    static bool exists(const QString &username) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) FROM user_password WHERE username = :username");
        query.bindValue(":username", username);
        query.exec();
        return query.next() && query.value(0).toInt() > 0;
    }
};

#endif //DEMO_USERMAPPER_H
