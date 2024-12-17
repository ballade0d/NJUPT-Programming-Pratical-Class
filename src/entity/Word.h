//
// Created by Dylan Deng on 2024/12/17.
//

#ifndef DEMO_WORD_H
#define DEMO_WORD_H

#include <QString>

class Word {
private:
    int id;
    int bookId;
    QString word;
    QString data;

public:
    Word(int id, int bookId, const QString &word, const QString &data) {
        this->id = id;
        this->bookId = bookId;
        this->word = word;
        this->data = data;
    }

    int getId() {
        return id;
    }

    int getBookId() {
        return bookId;
    }

    const QString &getWord() {
        return word;
    }

    const QString &getData() {
        return data;
    }

    void setData(const QString &data) {
        this->data = data;
    }
};

#endif //DEMO_WORD_H
