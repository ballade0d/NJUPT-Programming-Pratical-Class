#include "HomeWidget.h"
#include "CalendarWindow.h"
#include "EditWindow.h"
#include "LearnWindow.h"
#include <QtSql>
#include <QCryptographicHash>
#include <QMessageBox>

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT (name) FROM book", db);
    ui.listView->setModel(model);

    // 打卡按钮
    connect(ui.checkInButton, &QPushButton::clicked, this, &HomeWidget::handleCalendarButton);

    // 编辑按钮
    QPushButton *editButton = ui.editButton;
    // 设置编辑按钮默认状态为关闭，在选择物品时开启
    editButton->setEnabled(false);  // 默认禁用
    QObject::connect(ui.listView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [editButton](const QItemSelection &selected, const QItemSelection &deselected) {
                         Q_UNUSED(deselected);
                         editButton->setEnabled(!selected.indexes().isEmpty());
                     });
    connect(editButton, &QPushButton::clicked, this, &HomeWidget::handleEditButton);

    // 学习按钮
    QPushButton *learnButton = ui.learnButton;
    // 设置编辑按钮默认状态为关闭，在选择物品时开启
    learnButton->setEnabled(false);  // 默认禁用
    QObject::connect(ui.listView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [learnButton](const QItemSelection &selected, const QItemSelection &deselected) {
                         Q_UNUSED(deselected);
                         learnButton->setEnabled(!selected.indexes().isEmpty());
                     });
    connect(learnButton, &QPushButton::clicked, this, &HomeWidget::handleLearnButton);

}

void HomeWidget::handleCalendarButton(){
    CalendarWindow *calendarWindow = new CalendarWindow(this);
    // 设置窗口标题
    calendarWindow->setWindowTitle("每日打卡");
    // 显示窗口
    calendarWindow->exec();
}

void HomeWidget::handleEditButton(){
    QModelIndexList indexes = ui.listView->selectionModel()->selectedIndexes();
    if(indexes.isEmpty()){
        return;
    }
    // 获取列表中选中的物品
    QModelIndex index = indexes.first();
    QString book = index.data(Qt::DisplayRole).toString();

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT id from book WHERE name = :name");
    query.bindValue(":name", book);
    query.exec();

    if (query.next()) {
        // 从数据库中获取单词本Id
        QVariant bookId = query.value(0).toInt();

        EditWindow *editWindow = new EditWindow(this, bookId.toInt());
        editWindow->setWindowTitle("编辑单词本");
        editWindow->exec();
    }
}

void HomeWidget::handleLearnButton(){
    QModelIndexList indexes = ui.listView->selectionModel()->selectedIndexes();
    if(indexes.isEmpty()){
        return;
    }
    // 获取列表中选中的物品
    QModelIndex index = indexes.first();
    QString book = index.data(Qt::DisplayRole).toString();

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT id from book WHERE name = :name");
    query.bindValue(":name", book);
    query.exec();

    if (query.next()) {
        // 从数据库中获取单词本Id
        QVariant bookId = query.value(0).toInt();

        LearnWindow *learnWindow = new LearnWindow(nullptr, bookId.toInt());
        learnWindow->setWindowTitle("单词背诵");
        learnWindow->show();
    }
}
