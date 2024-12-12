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

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent) {
}

void HomeWidget::setup(int userId) {
    this->userId = userId;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建打卡日历按钮
    QPushButton *checkInButton = new QPushButton("打卡日历", this);
    connect(checkInButton, &QPushButton::clicked, this, &HomeWidget::handleCalendarButton);
    mainLayout->addWidget(checkInButton);

    listView = new QListView(this);
    refreshList();
    QHBoxLayout *listLayout = new QHBoxLayout();
    listLayout->addWidget(listView);

    QPushButton *addButton = new QPushButton("+");
    QPushButton *deleteButton = new QPushButton("-");
    addButton->setFixedSize(30, 30); // 设置按钮大小
    deleteButton->setFixedSize(30, 30); // 设置按钮大小
    QVBoxLayout *listButtonLayout = new QVBoxLayout();
    listButtonLayout->addWidget(addButton);
    listButtonLayout->addWidget(deleteButton);

    QString listButtonStyle = R"(
QPushButton {
    background-color: #f0f0f0;
    border: 1px solid #d0d0d0;
    border-radius: 15px;
    font-size: 20px;
    color: #666666;
}
QPushButton:hover {
    background-color: #e0e0e0;
}
QPushButton:pressed {
    background-color: #d0d0d0;
}
)";
    addButton->setStyleSheet(listButtonStyle);
    deleteButton->setStyleSheet(listButtonStyle);

    connect(addButton, &QPushButton::clicked, this, &HomeWidget::handleAddButton);
    connect(deleteButton, &QPushButton::clicked, this, &HomeWidget::handleDeleteButton);

    listLayout->addWidget(addButton);
    listLayout->addLayout(listButtonLayout);

    mainLayout->addLayout(listLayout);

    // 创建网格布局用于包含多个按钮
    QGridLayout *buttonLayout = new QGridLayout();
    mainLayout->addLayout(buttonLayout);

    // 学习按钮
    QPushButton *learnButton = new QPushButton("开始学习", this);
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
    QPushButton *reciteButton = new QPushButton("开始背诵", this);
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
    QPushButton *recordButton = new QPushButton("错题本", this);
    connect(recordButton, &QPushButton::clicked, this, &HomeWidget::handleRecordButton);
    buttonLayout->addWidget(recordButton, 1, 0);

    // 复习按钮
    QPushButton *reviewButton = new QPushButton("复习错题", this);
    connect(reviewButton, &QPushButton::clicked, this, &HomeWidget::handleReviewButton);
    buttonLayout->addWidget(reviewButton, 1, 1);

    // 编辑按钮
    QPushButton *editButton = new QPushButton("编辑", this);
    // 设置按钮默认状态为关闭，在选择物品时开启
    editButton->setEnabled(false);  // 默认禁用
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            [editButton](const QItemSelection &selected, const QItemSelection &deselected) {
                Q_UNUSED(deselected);
                editButton->setEnabled(!selected.indexes().isEmpty());
            });
    connect(editButton, &QPushButton::clicked, this, &HomeWidget::handleEditButton);
    mainLayout->addWidget(editButton);
}

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

void HomeWidget::handleCalendarButton() {
    CalendarWindow *calendarWindow = new CalendarWindow(this, userId);
    // 设置窗口标题
    calendarWindow->setWindowTitle("每日打卡");
    // 显示窗口
    calendarWindow->exec();
}

void HomeWidget::handleEditButton() {
    int bookId = getSelectedBookId();
    if (bookId == -1) {
        return;
    }
    EditWindow *editWindow = new EditWindow(this, bookId);
    editWindow->setWindowTitle("编辑单词本");
    editWindow->exec();
}

void HomeWidget::handleLearnButton() {
    int bookId = getSelectedBookId();
    if (bookId == -1) {
        return;
    }
    LearnWindow *learnWindow = new LearnWindow(nullptr, bookId);
    learnWindow->setWindowTitle("单词背诵");
    learnWindow->show();
}

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
