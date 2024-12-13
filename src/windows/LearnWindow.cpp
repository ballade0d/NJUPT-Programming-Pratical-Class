#include "LearnWindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>

/**
 * @brief LearnWindow 学习单词窗口
 * @param parent 父窗口
 * @param bookId 书籍 ID
 */
LearnWindow::LearnWindow(QWidget *parent, int bookId) : QWidget(parent) {
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query(db);
    query.prepare("SELECT word, data from book_word WHERE book_id = :book_id");
    query.bindValue(":book_id", bookId);
    query.exec();

    while (query.next()) {
        QString word = query.value(0).toString();
        QString data = query.value(1).toString();

        words.append(qMakePair(word, data));
    }

    // 创建一个垂直布局
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    // 创建显示单词的标签
    wordLabel = new QLabel(this);
    wordLabel->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(wordLabel);

    QPushButton *previousButton = new QPushButton("上一个", this);
    QPushButton *nextButton = new QPushButton("下一个", this);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(previousButton);
    hLayout->addWidget(nextButton);
    vLayout->addLayout(hLayout);

    connect(previousButton, &QPushButton::clicked, this, &LearnWindow::showPreviousWord);
    connect(nextButton, &QPushButton::clicked, this, &LearnWindow::showNextWord);

    if (!words.isEmpty()) {
        updateWordDisplay();
    }
}

/**
 * @brief 显示上一个单词
 */
void LearnWindow::showPreviousWord() {
    if (currentIndex > 0) {
        currentIndex--;
        updateWordDisplay();
    }
}

/**
 * @brief 显示下一个单词
 */
void LearnWindow::showNextWord() {
    if (currentIndex < words.size() - 1) {
        currentIndex++;
        updateWordDisplay();
    }
}

/**
 * @brief 更新单词信息展示
 */
void LearnWindow::updateWordDisplay() {
    if (currentIndex >= 0 && currentIndex < words.size()) {
        QString word = words[currentIndex].first;

        QString jsonData = words[currentIndex].second;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject obj = doc.object();
        // 加粗单词
        QString htmlContent = "<strong style='font-size: 24px;'>" + word + "</strong><br/><br/><br/>";
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
