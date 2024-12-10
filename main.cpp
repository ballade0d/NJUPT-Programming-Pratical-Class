#include "MainWindow.h"

#include <QApplication>
#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db"); // 数据库名

    // 尝试连接数据库
    if (!db.open()) {
        qDebug() << "无法连接数据库:" << db.lastError().text();
        return -1;
    } else {
        qDebug() << "数据库连接成功!";
    }

    // 创建表
    {
        // 用户名、密码表
        QSqlQuery query(db);
        bool queryResult = query.exec(
            "CREATE TABLE IF NOT EXISTS user_password ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "username TEXT NOT NULL, "
            "password TEXT NOT NULL)"
            );
        if (queryResult) {
            qDebug() << "表 'user_password' 创建成功或已存在。";
        } else {
            qDebug() << "创建表 'user_password' 失败：" << query.lastError().text();
            return -1;
        }
    }
    {
        // 打卡日期表
        QSqlQuery query(db);
        bool queryResult = query.exec(
            "CREATE TABLE IF NOT EXISTS check_in ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "date DATETIME NOT NULL)"
            );
        if (queryResult) {
            qDebug() << "表 'check_in' 创建成功或已存在。";
        } else {
            qDebug() << "创建表 'check_in' 失败：" << query.lastError().text();
            return -1;
        }
    }
    {
        // 单词本表
        QSqlQuery query(db);
        bool queryResult = query.exec(
            "CREATE TABLE IF NOT EXISTS book ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL)"
            );
        if (queryResult) {
            qDebug() << "表 'book' 创建成功或已存在。";
        } else {
            qDebug() << "创建表 'book' 失败：" << query.lastError().text();
            return -1;
        }
    }
    {
        // 单词表
        QSqlQuery query(db);
        bool queryResult = query.exec(
            "CREATE TABLE IF NOT EXISTS book_word ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "book_id INTEGER NOT NULL, "
            "word TEXT NOT NULL, "
            "data TEXT)"
            );
        if (queryResult) {
            qDebug() << "表 'book_word' 创建成功或已存在。";
        } else {
            qDebug() << "创建表 'book_word' 失败：" << query.lastError().text();
            return -1;
        }
    }
    {
        // 错题表
        QSqlQuery query(db);
        bool queryResult = query.exec(
            "CREATE TABLE IF NOT EXISTS record ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "book_id INTEGER NOT NULL, "
            "word_id INTEGER NOT NULL)"
            );
        if (queryResult) {
            qDebug() << "表 'record' 创建成功或已存在。";
        } else {
            qDebug() << "创建表 'record' 失败：" << query.lastError().text();
            return -1;
        }
    }

    MainWindow w;
    w.setFixedSize(480, 640);
    w.show();
    return a.exec();
}
