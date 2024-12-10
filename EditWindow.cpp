#include "EditWindow.h"
#include "WordAddDialog.h"
#include "WordEditDialog.h"
#include <QTextCharFormat>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtSql>

EditWindow::EditWindow(QWidget *parent, QVariant *bookId) : QDialog(parent) {
    setModal(true);

    QSqlDatabase db = QSqlDatabase::database();

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建按钮
    QPushButton *editButton = new QPushButton("编辑单词", this);
    QPushButton *addButton = new QPushButton("新增单词", this);
    QPushButton *deleteButton = new QPushButton("删除单词", this);

    this->wordList = new QListView(this);
    this->bookId = bookId->toInt();

    refreshList();

    layout->addWidget(wordList);
    layout->addWidget(editButton);
    layout->addWidget(addButton);
    layout->addWidget(deleteButton);

    // 设置编辑按钮默认状态为关闭，在选择物品时开启
    editButton->setEnabled(false);  // 默认禁用
    QObject::connect(wordList->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [editButton](const QItemSelection &selected, const QItemSelection &deselected) {
                         editButton->setEnabled(!selected.indexes().isEmpty());
                     });

    // 设置删除按钮默认状态为关闭，在选择物品时开启
    deleteButton->setEnabled(false);  // 默认禁用
    QObject::connect(wordList->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [deleteButton](const QItemSelection &selected, const QItemSelection &deselected) {
                         deleteButton->setEnabled(!selected.indexes().isEmpty());
                     });

    connect(editButton, &QPushButton::clicked, this, &EditWindow::handleEditButton);
    connect(addButton, &QPushButton::clicked, this, &EditWindow::handleAddButton);
    connect(deleteButton, &QPushButton::clicked, this, &EditWindow::handleDeleteButton);
}

void EditWindow::handleEditButton(){
    QModelIndexList indexes = wordList->selectionModel()->selectedIndexes();
    if(indexes.isEmpty()){
        return;
    }
    // 获取列表中选中的物品
    QModelIndex index = indexes.first();
    QString word = index.data(Qt::DisplayRole).toString();

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT data from book_word WHERE word = :word");
    query.bindValue(":word", word);
    query.exec();

    if (query.next()) {
        // 从数据库中获取单词信息
        QString data = query.value(0).toString();
        // 美化json格式
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
        QString formattedJson = doc.toJson(QJsonDocument::Indented);

        bool ok;
        WordEditDialog dialog(tr("单词编辑"), tr("请编辑:"), formattedJson, this);
        QString newText;
        if (dialog.exec() == QDialog::Accepted) {
            newText = dialog.getText();
            // 解析用户编辑后的 JSON
            QJsonDocument doc = QJsonDocument::fromJson(newText.toUtf8());
            // 将 JSON 转换为紧凑格式
            QString unbeautifiedJson = doc.toJson(QJsonDocument::Compact);
            // 更新数据到数据库
            QSqlQuery updateQuery(db);
            updateQuery.prepare("UPDATE book_word SET data = :data WHERE word = :word AND book_id = :book_id");
            updateQuery.bindValue(":book_id", bookId);
            updateQuery.bindValue(":data", unbeautifiedJson);
            updateQuery.bindValue(":word", word);
            updateQuery.exec();
        }
    }
}

void EditWindow::handleAddButton(){
    QSqlDatabase db = QSqlDatabase::database();

    bool ok;
    WordAddDialog dialog(tr("新增单词"), tr("请编辑:"), this);
    QString newWord;
    QString newText;
    if (dialog.exec() == QDialog::Accepted) {
        newWord = dialog.getWord();
        newText = dialog.getText();
        // 解析用户编辑后的 JSON
        QJsonDocument doc = QJsonDocument::fromJson(newText.toUtf8());
        // 将 JSON 转换为紧凑格式
        QString unbeautifiedJson = doc.toJson(QJsonDocument::Compact);
        // 更新数据到数据库
        QSqlQuery query(db);
        query.prepare("INSERT INTO book_word(book_id, word, data) VALUES (:book_id, :word, :data)");
        query.bindValue(":book_id", bookId);
        query.bindValue(":word", newWord);
        query.bindValue(":data", unbeautifiedJson);
        query.exec();
    }

    refreshList();
}

void EditWindow::handleDeleteButton(){
    QModelIndexList indexes = wordList->selectionModel()->selectedIndexes();
    if(indexes.isEmpty()){
        return;
    }
    // 获取列表中选中的物品
    QModelIndex index = indexes.first();
    QString word = index.data(Qt::DisplayRole).toString();

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("DELETE FROM book_word WHERE word = :word AND book_id = :book_id");
    query.bindValue(":book_id", bookId);
    query.bindValue(":word", word);
    query.exec();

    wordList->selectionModel()->clearSelection();
    refreshList();
}

void EditWindow::refreshList(){
    QSqlDatabase db = QSqlDatabase::database();
    // 查询数据库
    QSqlQuery query(db);
    query.prepare("SELECT word FROM book_word WHERE book_id = :book_id");
    query.bindValue(":book_id", bookId);
    query.exec();

    if (wordList->model() == nullptr) {
        QSqlQueryModel *model = new QSqlQueryModel(this);
        wordList->setModel(model);
    }

    static_cast<QSqlQueryModel*>(wordList->model())->setQuery(query);
}
