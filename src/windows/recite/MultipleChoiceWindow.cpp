#include "MultipleChoiceWindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QRadioButton>
#include <QButtonGroup>
#include <QOverload>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>

QStringList predefinedWrongAnswers = {"测试", "2", "3", "4", "5"};

MultipleChoiceWindow::MultipleChoiceWindow(QWidget *parent, int bookId) : QWidget(parent) {
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

    QGridLayout *buttonLayout = new QGridLayout();

    // 创建单选按钮
    QRadioButton *option1 = new QRadioButton("选项 1");
    QRadioButton *option2 = new QRadioButton("选项 2");
    QRadioButton *option3 = new QRadioButton("选项 3");
    QRadioButton *option4 = new QRadioButton("选项 4");

    buttonLayout->addWidget(option1, 0, 0);
    buttonLayout->addWidget(option2, 1, 0);
    buttonLayout->addWidget(option3, 2, 0);
    buttonLayout->addWidget(option4, 3, 0);
    vLayout->addLayout(buttonLayout);

    buttonGroup = new QButtonGroup();
    buttonGroup->addButton(option1, 0);
    buttonGroup->addButton(option2, 1);
    buttonGroup->addButton(option3, 2);
    buttonGroup->addButton(option4, 3);

    confirmButton = new QPushButton("确认");
    vLayout->addWidget(confirmButton);
    confirmButton->setEnabled(false);

    QObject::connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
                     [&](QAbstractButton *button) {
                         confirmButton->setEnabled(true);
                     });

    connect(confirmButton, &QPushButton::clicked, this, &MultipleChoiceWindow::handleButtonClick);

    if (!words.isEmpty()) {
        updateWordDisplay();
    }
}


void MultipleChoiceWindow::updateWordDisplay() {
    if (currentIndex >= 0 && currentIndex < words.size()) {
        QString word = words[currentIndex].first;
        wordLabel->setText(word);
        QString jsonData = words[currentIndex].second;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject obj = doc.object();
        QString answer = obj.find("释义")->toString();

        QList<QAbstractButton *> buttons = buttonGroup->buttons();

        // 随机生成正确答案的位置
        correctIndex = QRandomGenerator::global()->bounded(buttons.size());
        buttons[correctIndex]->setText(answer);

        // 随机生成错误答案
        QList<int> indices;
        for (int i = 0; i < words.size(); ++i) {
            if (i != currentIndex) {
                indices.append(i);
            }
        }
        QList<QString> wrongAnswers;
        while (wrongAnswers.size() < 3 && !indices.isEmpty()) {
            int randomIndex = QRandomGenerator::global()->bounded(indices.size());
            int selectedIndex = indices[randomIndex];
            indices.removeAt(randomIndex);  // 移除已选择的索引，防止重复选择

            // 解析JSON获取释义
            QString waJsonData = words[selectedIndex].second;
            QJsonDocument waDoc = QJsonDocument::fromJson(waJsonData.toUtf8());
            QJsonObject waObj = waDoc.object();
            QString wrongAnswer = waObj.value("释义").toString();
            wrongAnswers.append(wrongAnswer);
        }
        // 如果从words中获取的错误答案不足，从预定义列表中补充
        while (wrongAnswers.size() < 3) {
            QString &randomWrongAnswer = predefinedWrongAnswers[QRandomGenerator::global()->bounded(
                    predefinedWrongAnswers.size())];
            if (!wrongAnswers.contains(randomWrongAnswer)) {
                wrongAnswers.append(randomWrongAnswer);
            }
        }

        int wrongAnswerIndex = 0;
        for (int i = 0; i < buttons.size(); ++i) {
            if (i == correctIndex) {
                continue;
            }
            if (wrongAnswerIndex < wrongAnswers.size()) {
                buttons[i]->setText(wrongAnswers[wrongAnswerIndex++]);
            }
        }
    }
}

void MultipleChoiceWindow::handleButtonClick() {
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    for (int i = 0; i < buttons.size(); ++i) {
        if (!buttons[i]->isChecked()) {
            continue;
        }
        QPixmap pixmap;
        if (i == correctIndex) {
            pixmap = QPixmap(":/correct.svg");
        } else {
            pixmap = QPixmap(":/incorrect.svg");
        }
        pixmap = pixmap.scaled(20, 20, Qt::KeepAspectRatio);
        iconLabel->setPixmap(pixmap);
        confirmButton->setEnabled(false);

        // 等待1500ms
        QTimer::singleShot(1500, this, [this]() {
            iconLabel->clear();  // 清除图标
            if (currentIndex < words.size() - 1) {
                currentIndex++;
                updateWordDisplay();
                confirmButton->setEnabled(true);
            } else {

            }
        });
        break;
    }
}
