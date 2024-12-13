#include "MultipleChoiceWindow.h"
#include "SettlementWindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QRadioButton>
#include <QButtonGroup>
#include <QOverload>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>

// 预定义错误答案
QStringList predefinedWrongAnswers = {"测试", "2", "3", "4", "5"};

/**
 * @brief MultipleChoiceWindow 选择题窗口
 * @param parent 父窗口
 * @param userId 用户ID
 * @param bookId 单词本ID；若为-1，则表示背诵错题本
 */
MultipleChoiceWindow::MultipleChoiceWindow(QWidget *parent, int userId, int bookId) : QWidget(parent) {
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

/**
 * @brief 更新单词显示
 */
void MultipleChoiceWindow::updateWordDisplay() {
    if (currentIndex >= 0 && currentIndex < words.size()) {
        QString word = std::get<1>(words[currentIndex]);
        wordLabel->setText(word);
        QString jsonData = std::get<2>(words[currentIndex]);
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
            QString waJsonData = std::get<2>(words[selectedIndex]);
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

/**
 * @brief 处理按钮点击事件
 */
void MultipleChoiceWindow::handleButtonClick() {
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    for (int i = 0; i < buttons.size(); ++i) {
        if (!buttons[i]->isChecked()) {
            continue;
        }
        QPixmap pixmap;
        if (i == correctIndex) {
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

            if (bookId != -1) {
                QSqlDatabase db = QSqlDatabase::database();
                QSqlQuery query(db);
                // 已在错题本的不再添加
                query.prepare("SELECT * FROM record WHERE user_id = :user_id AND word_id = :word_id");
                query.bindValue(":user_id", userId);
                query.bindValue(":word_id", std::get<0>(words[currentIndex]));
                query.exec();
                if (!query.next()) {
                    // 添加到错题本
                    query.prepare(
                            "INSERT INTO record(user_id, book_id, word_id) VALUES (:user_id, :book_id, :word_id)");
                    query.bindValue(":user_id", userId);
                    query.bindValue(":book_id", bookId);
                    query.bindValue(":word_id", std::get<0>(words[currentIndex]));
                    query.exec();
                }
            }
        }
        pixmap = pixmap.scaled(20, 20, Qt::KeepAspectRatio);
        iconLabel->setPixmap(pixmap);
        confirmButton->setEnabled(false);

        // 等待1500ms
        QTimer::singleShot(1500, this, [this]() {
            // 清除图标
            iconLabel->clear();
            if (currentIndex < words.size() - 1) {
                // 显示下一个单词
                currentIndex++;
                updateWordDisplay();
                confirmButton->setEnabled(true);
            } else {
                // 显示结果
                this->close();
                SettlementWindow *settlementWindow = new SettlementWindow(nullptr, correctAnswers, wrongAnswers);
                settlementWindow->setWindowTitle("结果");
                settlementWindow->setBaseSize(640, 320);
                settlementWindow->show();
            }
        });
        break;
    }
}
