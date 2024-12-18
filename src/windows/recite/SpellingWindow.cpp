#include "SpellingWindow.h"
#include "SettlementWindow.h"
#include "../../mapper/WordMapper.h"
#include "../../mapper/RecordMapper.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QTimer>

/**
 * @brief SpellingWindow 拼写窗口
 * @param parent 父窗口
 * @param userId 用户ID
 * @param bookId 单词本ID；若为-1，则表示背诵错题本
 */
SpellingWindow::SpellingWindow(QWidget *parent, int userId, int bookId) : QWidget(parent) {
    this->bookId = bookId;
    this->userId = userId;

    if (bookId != -1) {
        // 背诵单词本
        words = WordMapper::getWords(bookId);
    } else {
        // 背诵错题本
        words = RecordMapper::getWords(userId);
    }

    // 创建一个垂直布局
    QVBoxLayout * vLayout = new QVBoxLayout(this);

    QHBoxLayout * wordAndIcon = new QHBoxLayout();
    // 创建显示单词的标签
    wordLabel = new QLabel();
    wordLabel->setAlignment(Qt::AlignCenter);
    iconLabel = new QLabel();
    wordAndIcon->addWidget(wordLabel);
    wordAndIcon->addWidget(iconLabel);

    vLayout->addLayout(wordAndIcon);

    spellingLayout = new QHBoxLayout();
    vLayout->addLayout(spellingLayout);

    nextButton = new QPushButton("下一个");
    vLayout->addWidget(nextButton);

    connect(nextButton, &QPushButton::clicked, this, &SpellingWindow::handleNextButton);

    if (!words.isEmpty()) {
        updateWordDisplay();
    }
}

/**
 * @brief 处理下一个按钮点击事件
 */
void SpellingWindow::handleNextButton() {
    QPixmap pixmap;
    if (check()) {
        pixmap = QPixmap(":/correct.svg");
        correctAnswers.append(words[currentIndex]->getWord());

        if (bookId == -1) {
            // 从错题本中删除
            RecordMapper::removeWord(userId, words[currentIndex]);
        }
    } else {
        pixmap = QPixmap(":/incorrect.svg");
        wrongAnswers.append(words[currentIndex]->getWord());

        if (bookId != -1) {
            // 已在错题本的不再添加
            if (!RecordMapper::exists(userId, words[currentIndex])) {
                // 添加到错题本
                RecordMapper::addWord(userId, words[currentIndex]);
            }
        }
    }
    pixmap = pixmap.scaled(20, 20, Qt::KeepAspectRatio);
    iconLabel->setPixmap(pixmap);
    nextButton->setEnabled(false);

    // 等待1500ms
    QTimer::singleShot(1500, this, [this]() {
        // 清除图标
        iconLabel->clear();
        if (currentIndex < words.size() - 1) {
            // 显示下一个单词
            currentIndex++;
            updateWordDisplay();
            nextButton->setEnabled(true);
        } else {
            // 显示结果
            this->close();
            SettlementWindow *settlementWindow = new SettlementWindow(nullptr, correctAnswers, wrongAnswers);
            settlementWindow->setWindowTitle("结果");
            settlementWindow->setBaseSize(640, 320);
            settlementWindow->show();
        }
    });
}

/**
 * @brief 检查拼写是否正确
 */
bool SpellingWindow::check() {
    QString result;
    // 遍历所有的 QLineEdit，获取其中的文本
    for (int i = 0; i < spellingLayout->count(); ++i) {
        QLayoutItem *item = spellingLayout->itemAt(i);
        QWidget * widget = item->widget();
        if (widget) {
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(widget);
            if (lineEdit) {
                result += lineEdit->text();
            }
        }
    }

    return result == words[currentIndex]->getWord();
}

/**
 * @brief 更新单词显示
 */
void SpellingWindow::updateWordDisplay() {
    if (currentIndex >= 0 && currentIndex < words.size()) {
        QLayoutItem *item;
        // 删除所有格子控件
        while ((item = spellingLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        QString word = words[currentIndex]->getWord();
        QString jsonData = words[currentIndex]->getData();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject obj = doc.object();
        wordLabel->setText(obj.find("释义")->toString());

        std::vector<QLineEdit *> lineEdits(word.length());
        for (int i = 0; i < word.length(); i++) {
            lineEdits[i] = new QLineEdit();
            lineEdits[i]->setMaxLength(1);  // 设置最大长度为1
            lineEdits[i]->setFixedWidth(30);  // 设置固定宽度
            spellingLayout->addWidget(lineEdits[i]);

            if (i > 0) {
                // 将前一个 QLineEdit 的 textEdited 信号连接 setFocus 到下一个 lineEdit
                QObject::connect(lineEdits[i - 1], &QLineEdit::textEdited, [lineEdits, i](const QString &text) {
                    if (!text.isEmpty()) {
                        lineEdits[i]->setFocus();
                    }
                });

                // 监听退格键，切换到前一个编辑框
                BackspaceFilter *backspaceFilter = new BackspaceFilter();
                lineEdits[i]->installEventFilter(backspaceFilter);
                QObject::connect(backspaceFilter, &BackspaceFilter::requestPreviousFocus, [lineEdits, i]() {
                    lineEdits[i]->clear();
                    lineEdits[i - 1]->setFocus();
                });
            }
        }
    }
}
