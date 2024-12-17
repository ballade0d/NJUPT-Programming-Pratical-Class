//
// Created by Dylan Deng on 2024/12/17.
//

#ifndef DEMO_USER_H
#define DEMO_USER_H

#include <QString>

class User {
private:
    int id;
    QString username;
    QString password;

public:
    User(int id, const QString &username, const QString &password) {
        this->id = id;
        this->username = username;
        this->password = password;
    }

    int getId() {
        return id;
    }

    const QString &getUsername() {
        return username;
    }

    const QString &getPassword() {
        return password;
    }
};

#endif //DEMO_USER_H
