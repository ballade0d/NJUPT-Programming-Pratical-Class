#include "HomeWidget.h"
#include "../windows/CalendarWindow.h"
#include "../windows/EditWindow.h"
#include "../windows/LearnWindow.h"
#include "../windows/SpellingWindow.h"
#include <QtSql>
#include <QCryptographicHash>
#include <QMessageBox>

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);\

    // 创建打卡日历按钮
    QPushButton *checkInButton = new QPushButton("打卡日历", this);
    connect(checkInButton, &QPushButton::clicked, this, &HomeWidget::handleCalendarButton);
    mainLayout->addWidget(checkInButton);

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT (name) FROM book", db);

    listView = new QListView(this);
    listView->setModel(model);
    mainLayout->addWidget(listView);

    // 创建网格布局用于包含多个按钮
    QGridLayout *buttonLayout = new QGridLayout();
    mainLayout->addLayout(buttonLayout);

    // 学习按钮
    QPushButton *learnButton = new QPushButton("开始学习", this);
    // 设置按钮默认状态为关闭，在选择物品时开启
    learnButton->setEnabled(false);  // 默认禁用
    QObject::connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [learnButton](const QItemSelection &selected, const QItemSelection &deselected) {
                         Q_UNUSED(deselected);
                         learnButton->setEnabled(!selected.indexes().isEmpty());
                     });
    connect(learnButton, &QPushButton::clicked, this, &HomeWidget::handleLearnButton);
    buttonLayout->addWidget(learnButton, 0, 0);

    // 背诵按钮
    QPushButton *reciteButton = new QPushButton("开始背诵", this);
    // 设置按钮默认状态为关闭，在选择物品时开启
    reciteButton->setEnabled(false);  // 默认禁用
    QObject::connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [reciteButton](const QItemSelection &selected, const QItemSelection &deselected) {
                         Q_UNUSED(deselected);
                         reciteButton->setEnabled(!selected.indexes().isEmpty());
                     });
    connect(reciteButton, &QPushButton::clicked, this, &HomeWidget::handleReciteButton);
    buttonLayout->addWidget(reciteButton, 0, 1);

    // 错题按钮
    QPushButton *recordButton = new QPushButton("错题本", this);
    buttonLayout->addWidget(recordButton, 1, 0);

    // 复习按钮
    QPushButton *reviewButton = new QPushButton("复习错题", this);
    buttonLayout->addWidget(reviewButton, 1, 1);

    // 编辑按钮
    QPushButton *editButton = new QPushButton("编辑", this);
    // 设置按钮默认状态为关闭，在选择物品时开启
    editButton->setEnabled(false);  // 默认禁用
    QObject::connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [editButton](const QItemSelection &selected, const QItemSelection &deselected) {
                         Q_UNUSED(deselected);
                         editButton->setEnabled(!selected.indexes().isEmpty());
                     });
    connect(editButton, &QPushButton::clicked, this, &HomeWidget::handleEditButton);
    mainLayout->addWidget(editButton);
}

void HomeWidget::handleCalendarButton() {
    CalendarWindow *calendarWindow = new CalendarWindow(this);
    // 设置窗口标题
    calendarWindow->setWindowTitle("每日打卡");
    // 显示窗口
    calendarWindow->exec();
}

void HomeWidget::handleEditButton() {
    QModelIndexList indexes = listView->selectionModel()->selectedIndexes();
    if (indexes.isEmpty()) {
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

void HomeWidget::handleLearnButton() {
    QModelIndexList indexes = listView->selectionModel()->selectedIndexes();
    if (indexes.isEmpty()) {
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

void HomeWidget::handleReciteButton() {
    QModelIndexList indexes = listView->selectionModel()->selectedIndexes();
    if (indexes.isEmpty()) {
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

        SpellingWindow *reciteWindow = new SpellingWindow(nullptr, bookId.toInt());
        reciteWindow->setWindowTitle("单词背诵");
        reciteWindow->show();
    }
}
