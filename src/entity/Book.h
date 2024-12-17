#ifndef DEMO_BOOK_H
#define DEMO_BOOK_H


#include <QString>

class Book {
private:
    int id;
    int userId;
    QString name;

public:
    Book(int id, int userId, const QString &name) {
        this->id = id;
        this->userId = userId;
        this->name = name;
    }

    int getId() {
        return id;
    }

    int getUserId() {
        return userId;
    }

    const QString &getName() {
        return name;
    }
};


#endif //DEMO_BOOK_H
