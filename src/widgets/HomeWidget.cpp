#include "HomeWidget.h"
#include "../windows/CalendarWindow.h"
#include "../windows/EditWindow.h"
#include "../windows/LearnWindow.h"
#include "../windows/recite/SpellingWindow.h"
#include "../windows/recite/MultipleChoiceWindow.h"
#include "../dialog/BookAddDialog.h"
#include "../dialog/BookDeleteDialog.h"
#include "../windows/RecordBookWindow.h"
#include <QtSql>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QStackedLayout>

/**
 * @brief HomeWidget 主页窗口
 * @param parent 父窗口
 */
HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent) {
}

void HomeWidget::setup(int userId) {
    this->userId = userId;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建打卡日历按钮
    QPushButton *checkInButton = new QPushButton("打卡日历");
    connect(checkInButton, &QPushButton::clicked, this, &HomeWidget::handleCalendarButton);
    mainLayout->addWidget(checkInButton);

    listView = new QListView();
    refreshList();
    QHBoxLayout *listLayout = new QHBoxLayout();
    listLayout->addWidget(listView);

    QPushButton *addButton = new QPushButton("+");
    QPushButton *deleteButton = new QPushButton("-");
    addButton->setFixedSize(30, 30); // 设置按钮大小
    deleteButton->setFixedSize(30, 30); // 设置按钮大小
    QGridLayout *listButtonLayout = new QGridLayout();
    listButtonLayout->addWidget(addButton, 0, 0);
    listButtonLayout->addWidget(deleteButton, 1, 0);

    connect(addButton, &QPushButton::clicked, this, &HomeWidget::handleAddButton);
    connect(deleteButton, &QPushButton::clicked, this, &HomeWidget::handleDeleteButton);

    listLayout->addLayout(listButtonLayout);

    mainLayout->addLayout(listLayout);

    // 创建网格布局用于包含多个按钮
    QGridLayout *buttonLayout = new QGridLayout();
    mainLayout->addLayout(buttonLayout);

    // 学习按钮
    QPushButton *learnButton = new QPushButton("开始学习");
    // 设置按钮默认状态为关闭，在选择物品时开启
    learnButton->setEnabled(false);  // 默认禁用
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            [learnButton](const QItemSelection &selected, const QItemSelection &deselected) {
                Q_UNUSED(deselected);
                learnButton->setEnabled(!selected.indexes().isEmpty());
            });
    connect(learnButton, &QPushButton::clicked, this, &HomeWidget::handleLearnButton);
    buttonLayout->addWidget(learnButton, 0, 0);

    // 背诵按钮
    QPushButton *reciteButton = new QPushButton("开始背诵");
    // 设置按钮默认状态为关闭，在选择物品时开启
    reciteButton->setEnabled(false);  // 默认禁用
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            [reciteButton](const QItemSelection &selected, const QItemSelection &deselected) {
                Q_UNUSED(deselected);
                reciteButton->setEnabled(!selected.indexes().isEmpty());
            });
    connect(reciteButton, &QPushButton::clicked, this, &HomeWidget::handleReciteButton);
    buttonLayout->addWidget(reciteButton, 0, 1);

    // 错题按钮
    QPushButton *recordButton = new QPushButton("错题本");
    connect(recordButton, &QPushButton::clicked, this, &HomeWidget::handleRecordButton);
    buttonLayout->addWidget(recordButton, 1, 0);

    // 复习按钮
    QPushButton *reviewButton = new QPushButton("复习错题");
    connect(reviewButton, &QPushButton::clicked, this, &HomeWidget::handleReviewButton);
    buttonLayout->addWidget(reviewButton, 1, 1);

    // 编辑按钮
    QPushButton *editButton = new QPushButton("编辑");
    // 设置按钮默认状态为关闭，在选择物品时开启
    editButton->setEnabled(false);  // 默认禁用
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            [editButton](const QItemSelection &selected, const QItemSelection &deselected) {
                Q_UNUSED(deselected);
                editButton->setEnabled(!selected.indexes().isEmpty());
            });
    connect(editButton, &QPushButton::clicked, this, &HomeWidget::handleEditButton);
    mainLayout->addWidget(editButton);
    setStyleSheet(R"(
QPushButton {
    background-color: #0078D7;
    color: #ffffff;
    border-style: solid;
    border-width: 1px;
    border-radius: 5px;
    border-color: #0053a6;
    padding: 6px;
}

QPushButton:disabled {
    background-color: #cccccc;
    border-color: #aaaaaa;
    color: #666666;
}

QPushButton:hover {
    background-color: #0053a6;
}

QPushButton:pressed {
    background-color: #00396d;
}
)");
}

/**
 * @brief getSelectedBookId 获取选中的单词本Id
 * @return 单词本Id
 */
int HomeWidget::getSelectedBookId() {
    QModelIndexList indexes = listView->selectionModel()->selectedIndexes();
    if (indexes.isEmpty()) {
        return -1;
    }
    // 获取列表中选中的物品
    QModelIndex index = indexes.first();
    QString book = index.data(Qt::DisplayRole).toString();

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT id from book WHERE user_id = :user_id AND name = :name");
    query.bindValue(":user_id", userId);
    query.bindValue(":name", book);
    query.exec();

    if (query.next()) {
        // 从数据库中获取单词本Id
        return query.value(0).toInt();
    }
    return -1;
}

/**
 * @brief handleCalendarButton 处理打卡日历按钮
 */
void HomeWidget::handleCalendarButton() {
    CalendarWindow *calendarWindow = new CalendarWindow(this, userId);
    // 设置窗口标题
    calendarWindow->setWindowTitle("每日打卡");
    // 显示窗口
    calendarWindow->exec();
}

/**
 * @brief handleEditButton 处理编辑按钮
 */
void HomeWidget::handleEditButton() {
    int bookId = getSelectedBookId();
    if (bookId == -1) {
        return;
    }
    EditWindow *editWindow = new EditWindow(this, bookId);
    editWindow->setWindowTitle("编辑单词本");
    editWindow->exec();
}

/**
 * @brief handleLearnButton 处理学习按钮
 */
void HomeWidget::handleLearnButton() {
    int bookId = getSelectedBookId();
    if (bookId == -1) {
        return;
    }
    LearnWindow *learnWindow = new LearnWindow(nullptr, bookId);
    learnWindow->setWindowTitle("单词背诵");
    learnWindow->show();
}

/**
 * @brief handleReciteButton 处理背诵按钮
 */
void HomeWidget::handleReciteButton() {
    int bookId = getSelectedBookId();
    if (bookId == -1) {
        return;
    }
    QMessageBox msgBox;
    msgBox.setWindowTitle("请选择背诵方式");
    msgBox.setText("请选择背诵方式");
    QPushButton *button1 = msgBox.addButton("选择题背诵", QMessageBox::ActionRole);
    QPushButton *button2 = msgBox.addButton("拼写单词背诵", QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == button1) {
        MultipleChoiceWindow *window = new MultipleChoiceWindow(nullptr, userId, bookId);
        window->setWindowTitle("单词背诵");
        window->show();
    } else if (msgBox.clickedButton() == button2) {
        SpellingWindow *window = new SpellingWindow(nullptr, userId, bookId);
        window->setWindowTitle("单词背诵");
        window->show();
    }
}

/**
 * @brief handleAddButton 处理新增按钮
 */
void HomeWidget::handleAddButton() {
    BookAddDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString newBook = dialog.getBook();

        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("INSERT INTO book(user_id, name) VALUES (:user_id, :name)");
        query.bindValue(":user_id", userId);
        query.bindValue(":name", newBook);
        query.exec();

        refreshList();
    }
}

/**
 * @brief handleDeleteButton 处理删除按钮
 */
void HomeWidget::handleDeleteButton() {
    BookDeleteDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int bookId = getSelectedBookId();
        if (bookId == -1) {
            return;
        }

        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(db);
        query.prepare("DELETE FROM book WHERE id = :id");
        query.bindValue(":id", bookId);
        query.exec();

        listView->selectionModel()->clearSelection();
        refreshList();
    }
}

/**
 * @brief handleRecordButton 处理错题本按钮
 */
void HomeWidget::handleRecordButton() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM record WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);
    query.exec();
    if (!query.next()) {
        QMessageBox::information(this, "提示", "错题本为空");
        return;
    }

    RecordBookWindow *recordBookWindow = new RecordBookWindow(nullptr, userId);
    recordBookWindow->setWindowTitle("错题本");
    recordBookWindow->setBaseSize(480, 320);
    recordBookWindow->show();
}

/**
 * @brief handleReviewButton 处理复习按钮
 */
void HomeWidget::handleReviewButton() {
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM record WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);
    query.exec();
    if (!query.next()) {
        QMessageBox::information(this, "提示", "错题本为空");
        return;
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("请选择背诵方式");
    msgBox.setText("请选择背诵方式");
    QPushButton *button1 = msgBox.addButton("选择题背诵", QMessageBox::ActionRole);
    QPushButton *button2 = msgBox.addButton("拼写单词背诵", QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == button1) {
        MultipleChoiceWindow *window = new MultipleChoiceWindow(nullptr, userId, -1);
        window->setWindowTitle("单词背诵");
        window->show();
    } else if (msgBox.clickedButton() == button2) {
        SpellingWindow *window = new SpellingWindow(nullptr, userId, -1);
        window->setWindowTitle("单词背诵");
        window->show();
    }
}

/**
 * @brief refreshList 刷新单词本列表
 */
void HomeWidget::refreshList() {
    QSqlDatabase db = QSqlDatabase::database();
    // 查询数据库
    QSqlQuery query(db);
    query.prepare("SELECT (name) FROM book WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);
    query.exec();

    if (listView->model() == nullptr) {
        QSqlQueryModel *model = new QSqlQueryModel(this);
        listView->setModel(model);
    }

    dynamic_cast<QSqlQueryModel *>(listView->model())->setQuery(query);
}
