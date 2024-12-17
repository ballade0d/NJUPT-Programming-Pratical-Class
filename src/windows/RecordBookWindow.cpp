#include "RecordBookWindow.h"
#include "../mapper/BookMapper.h"
#include "../mapper/RecordMapper.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListView>
#include <QSqlError>
#include <QStringListModel>

/**
 * @brief RecordBookWindow 错题本窗口
 * @param parent 父窗口
 * @param userId 用户 ID
 */
RecordBookWindow::RecordBookWindow(QWidget *parent, int userId) : QWidget(parent) {
    this->record = RecordMapper::getWords(userId);
    QStringList list;
    for (Word *word: record) {
        list.append(word->getWord());
    }

    QHBoxLayout * hLayout = new QHBoxLayout(this);
    listView = new QListView();
    listView->setModel(new QStringListModel(list));

    // 创建一个垂直布局
    QVBoxLayout * vLayout = new QVBoxLayout();

    // 创建显示错题的标签
    wordLabel = new QLabel();
    wordLabel->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(wordLabel);

    hLayout->addWidget(listView);
    hLayout->addLayout(vLayout);

    // 选择列表中的某个单词时，更新显示的单词内容
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            [this](const QItemSelection &selected, const QItemSelection &deselected) {
                Q_UNUSED(deselected);
                if (selected.isEmpty()) {
                    return;
                }
                QModelIndex index = selected.indexes().first();
                currentIndex = index.row();
                updateWordDisplay();
            });

    if (!record.isEmpty()) {
        updateWordDisplay();
    }
}

/**
 * @brief updateWordDisplay 更新显示的单词信息
 */
void RecordBookWindow::updateWordDisplay() {
    if (currentIndex >= 0 && currentIndex < record.size()) {
        QSqlDatabase db = QSqlDatabase::database();

        Book *book = BookMapper::getBookById(record[currentIndex]->getBookId());
        Word *word = record[currentIndex];

        QString spelling = word->getWord();
        QString jsonData = word->getData();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject obj = doc.object();
        // 加粗错题单词
        QString htmlContent = "<strong style='font-size: 24px;'>" + spelling + "</strong><br/><br/><br/>";
        // 显示书名
        htmlContent += "<strong>来源:</strong> " + book->getName() + "<br/>";
        htmlContent += "<br/>";
        // 遍历 JSON 对象中的所有键值对
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            htmlContent += "<strong>" + it.key() + ":</strong> " + it.value().toString() + "<br/>";
        }

        wordLabel->setText("<html><head><style type='text/css'>"
                           "body {"
                           "  margin: 0;"
                           "  padding: 0;"
                           "  text-align: left;"
                           "}"
                           "</style></head>"
                           "<body><p>" + htmlContent + "</p></body></html>");
    }
}
