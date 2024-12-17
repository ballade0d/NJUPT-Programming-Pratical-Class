//
// Created by Dylan Deng on 2024/12/17.
//

#ifndef DEMO_CHECKINMAPPER_H
#define DEMO_CHECKINMAPPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>

class CheckInMapper {
public:
    static QList<QDate> getAllCheckIns(int userId) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);

        query.prepare("SELECT date FROM check_in WHERE user_id = :user_id");
        query.bindValue(":user_id", userId);
        query.exec();

        QList<QDate> checkIns;
        while (query.next()) {
            QDate date = QDate::fromString(query.value("date").toString(), "yyyy-MM-dd");
            checkIns.append(date);
        }

        return checkIns;
    }

    static void checkIn(int userId, const QString &date) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);

        query.prepare("INSERT INTO check_in (user_id, date) VALUES (:user_id, :date)");
        query.bindValue(":user_id", userId);
        query.bindValue(":date", date);
        query.exec();
    }

    static bool exists(int userId, const QString &date) {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) FROM check_in WHERE user_id = :user_id AND date = :date");
        query.bindValue(":user_id", userId);
        query.bindValue(":date", date);
        query.exec();
        return query.next() && query.value(0).toInt() > 0;
    }
};

#endif //DEMO_CHECKINMAPPER_H
