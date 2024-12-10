#include "HomeWidget.h"
#include "CalendarWindow.h"
#include "EditWindow.h"
#include <QtSql>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QCalendarWidget>

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT (name) FROM book", db);
    ui.listView->setModel(model);

    QPushButton *editButton = ui.editButton;

    // 设置编辑按钮默认状态为关闭，在选择物品时开启
    editButton->setEnabled(false);  // 默认禁用
    QObject::connect(ui.listView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [editButton](const QItemSelection &selected, const QItemSelection &deselected) {
                         Q_UNUSED(deselected);
                         editButton->setEnabled(!selected.indexes().isEmpty());
                     });

    connect(ui.checkInButton, &QPushButton::clicked, this, &HomeWidget::handleCalendarButton);
    connect(editButton, &QPushButton::clicked, this, &HomeWidget::handleEditButton);

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

        EditWindow *editWindow = new EditWindow(this, &bookId);
        editWindow->setWindowTitle("编辑单词本");
        editWindow->exec();
    }
}
