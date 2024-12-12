#include "SpellingWindow.h"
#include "SettlementWindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QTimer>
#include <QSqlError>

SpellingWindow::SpellingWindow(QWidget *parent, int userId, int bookId) : QWidget(parent) {
    this->bookId = bookId;
    this->userId = userId;
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query(db);
    if (bookId != -1) {
        // 背诵单词本
        query.prepare("SELECT id, word, data from book_word WHERE book_id = :book_id");
        query.bindValue(":book_id", bookId);
    } else {
        // 背诵错题本
        query.prepare(
                "SELECT bw.id, bw.word, bw.data from record r JOIN book_word bw ON r.word_id = bw.id WHERE r.user_id = :user_id");
        query.bindValue(":user_id", userId);
    }
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        QString word = query.value(1).toString();
        QString data = query.value(2).toString();
        words.append(std::make_tuple(id, word, data));
    }

    // 创建一个垂直布局
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    QHBoxLayout *wordAndIcon = new QHBoxLayout();
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

void SpellingWindow::handleNextButton() {
    QPixmap pixmap;
    if (check()) {
        pixmap = QPixmap(":/correct.svg");
        correctAnswers.append(std::get<1>(words[currentIndex]));

        if (bookId == -1) {
            // 从错题本中删除
            QSqlDatabase db = QSqlDatabase::database();
            QSqlQuery query(db);
            query.prepare("DELETE FROM record WHERE user_id = :user_id AND word_id = :word_id");
            query.bindValue(":user_id", userId);
            query.bindValue(":word_id", std::get<0>(words[currentIndex]));
            query.exec();
        }
    } else {
        pixmap = QPixmap(":/incorrect.svg");
        wrongAnswers.append(std::get<1>(words[currentIndex]));

        // 已在错题本的不再添加
        if (bookId != -1) {
            // 添加到错题本
            QSqlDatabase db = QSqlDatabase::database();
            QSqlQuery query(db);
            query.prepare("INSERT INTO record(user_id, book_id, word_id) VALUES (:user_id, :book_id, :word_id)");
            query.bindValue(":user_id", userId);
            query.bindValue(":book_id", bookId);
            query.bindValue(":word_id", std::get<0>(words[currentIndex]));
            query.exec();
        }
    }
    pixmap = pixmap.scaled(20, 20, Qt::KeepAspectRatio);
    iconLabel->setPixmap(pixmap);
    nextButton->setEnabled(false);

    // 等待1500ms
    QTimer::singleShot(1500, this, [this]() {
        iconLabel->clear();  // 清除图标
        if (currentIndex < words.size() - 1) {
            currentIndex++;
            updateWordDisplay();
            nextButton->setEnabled(true);
        } else {
            this->close();
            SettlementWindow *settlementWindow = new SettlementWindow(nullptr, correctAnswers, wrongAnswers);
            settlementWindow->setWindowTitle("结果");
            settlementWindow->setBaseSize(640, 320);
            settlementWindow->show();
        }
    });
}

bool SpellingWindow::check() {
    QString result;
    // 遍历所有的 QLineEdit，获取其中的文本
    for (int i = 0; i < spellingLayout->count(); ++i) {
        QLayoutItem *item = spellingLayout->itemAt(i);
        QWidget *widget = item->widget();
        if (widget) {
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(widget);
            if (lineEdit) {
                result += lineEdit->text();
            }
        }
    }

    return result == std::get<1>(words[currentIndex]);
}

void SpellingWindow::updateWordDisplay() {
    if (currentIndex >= 0 && currentIndex < words.size()) {
        QLayoutItem *item;
        // 删除所有格子控件
        while ((item = spellingLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        QString word = std::get<1>(words[currentIndex]);
        QString jsonData = std::get<2>(words[currentIndex]);
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
