#include "SpellingWindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QTimer>

SpellingWindow::SpellingWindow(QWidget *parent, int bookId) : QWidget(parent) {
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
    } else {
        pixmap = QPixmap(":/incorrect.svg");
        wordLabel->setText(words[currentIndex].first);
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
            //
        }
    });
}

bool SpellingWindow::check() {
    QString result;
    for (int i = 0; i < spellingLayout->count(); ++i) {
        QLayoutItem *item = spellingLayout->itemAt(i);
        QWidget *widget = item->widget();  // 尝试获取 QLayoutItem 中的 QWidget
        if (widget) {
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(widget);
            if (lineEdit) {
                result += lineEdit->text();
            }
        }
    }

    return result == words[currentIndex].first;
}

void SpellingWindow::updateWordDisplay() {
    if (currentIndex >= 0 && currentIndex < words.size()) {
        QLayoutItem *item;
        // 删除所有格子控件
        while ((item = spellingLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        QString word = words[currentIndex].first;
        QString jsonData = words[currentIndex].second;
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
