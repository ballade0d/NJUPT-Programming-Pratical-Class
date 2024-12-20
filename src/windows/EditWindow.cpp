#include "EditWindow.h"
#include "../dialog/WordAddDialog.h"
#include "../dialog/WordEditDialog.h"
#include "../mapper/WordMapper.h"
#include <QSqlDatabase>
#include <QPushButton>
#include <QMessageBox>
#include <QJsonDocument>
#include <QtSql>

/**
 * @brief EditWindow 编辑单词本窗口
 * @param parent 父窗口
 * @param bookId 书籍 ID
 */
EditWindow::EditWindow(QWidget *parent, int bookId) : QDialog(parent) {
    setModal(true);

    QSqlDatabase db = QSqlDatabase::database();

    QVBoxLayout * layout = new QVBoxLayout(this);

    // 创建按钮
    QPushButton *editButton = new QPushButton("编辑单词", this);
    QPushButton *addButton = new QPushButton("新增单词", this);
    QPushButton *deleteButton = new QPushButton("删除单词", this);

    this->wordList = new QListView(this);
    this->bookId = bookId;

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

Word *EditWindow::getSelectedWord() {
    QModelIndexList indexes = wordList->selectionModel()->selectedIndexes();
    if (indexes.isEmpty()) {
        return nullptr;
    }
    // 获取列表中选中的物品
    QModelIndex index = indexes.first();
    QString spelling = index.data(Qt::DisplayRole).toString();

    return WordMapper::getWord(spelling, bookId);
}

/**
 * @brief handleEditButton 编辑按钮点击事件
 */
void EditWindow::handleEditButton() {
    Word *word = getSelectedWord();
    if (word == nullptr) {
        return;
    }

    // 从数据库中获取当前单词信息
    QString data = word->getData();
    // 美化json格式
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    QString formattedJson = doc.toJson(QJsonDocument::Indented);

    WordEditDialog dialog(formattedJson, this);
    QString newText;
    if (dialog.exec() == QDialog::Accepted) {
        newText = dialog.getText();
        // 解析用户编辑后的 JSON
        QJsonParseError parseError;
        QJsonDocument newDoc = QJsonDocument::fromJson(newText.toUtf8() , &parseError);
        // 将 JSON 转换为紧凑格式
        QString unbeautifiedJson = newDoc.toJson(QJsonDocument::Compact);
        // 检查 JSON 格式是否正确
        if(parseError.error != QJsonParseError::NoError) {
            QMessageBox::warning(this, "错误", "JSON 格式错误");
            return;
        }
        // 更新数据到数据库
        word->setData(unbeautifiedJson);
        WordMapper::updateWord(word);
    }
}

/**
 * @brief handleAddButton 处理新增单词按钮
 */
void EditWindow::handleAddButton() {
    QSqlDatabase db = QSqlDatabase::database();

    WordAddDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString newWord = dialog.getWord();
        QString newText = dialog.getText();
        // 解析用户编辑后的 JSON
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(newText.toUtf8(), &parseError);
        // 将 JSON 转换为紧凑格式
        QString unbeautifiedJson = doc.toJson(QJsonDocument::Compact);
        // 检查 JSON 格式是否正确
        if(parseError.error != QJsonParseError::NoError) {
            QMessageBox::warning(this, "错误", "JSON 格式错误");
            return;
        }
        // 更新数据到数据库
        WordMapper::createWord(bookId, newWord, unbeautifiedJson);
    }

    refreshList();
}

/**
 * @brief handleDeleteButton 处理删除单词按钮
 */
void EditWindow::handleDeleteButton() {
    Word *word = getSelectedWord();
    if (word == nullptr) {
        return;
    }
    WordMapper::deleteWord(word->getId());
    wordList->selectionModel()->clearSelection();
    refreshList();
}

/**
 * @brief refreshList 刷新列表
 */
void EditWindow::refreshList() {
    QList<Word *> words = WordMapper::getWords(bookId);
    QList<QString> spellings;
    for (Word *word: words) {
        spellings.append(word->getWord());
    }
    // 如果列表模型为空，则创建一个新的列表模型。否则只更新列表模型
    if (wordList->model() == nullptr) {
        QStringListModel *model = new QStringListModel(this);
        wordList->setModel(model);
    }
    dynamic_cast<QStringListModel *>(wordList->model())->setStringList(spellings);
}
