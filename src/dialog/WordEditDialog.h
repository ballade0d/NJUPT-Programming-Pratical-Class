#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

class WordEditDialog : public QDialog {
Q_OBJECT

    /**
     * @brief WordEditDialog 编辑单词对话框
     * @param parent 父窗口
     */
public:
    explicit WordEditDialog(const QString &defaultValue,
                            QWidget *parent = nullptr)
            : QDialog(parent), textValue(defaultValue) {
        setWindowTitle("单词编辑");

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QLabel *label = new QLabel("请编辑：");
        mainLayout->addWidget(label);

        textEdit = new QTextEdit(this);
        textEdit->setText(defaultValue);
        mainLayout->addWidget(textEdit);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *okButton = new QPushButton(tr("完成"), this);
        QPushButton *cancelButton = new QPushButton(tr("取消"), this);
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);

        connect(okButton, &QPushButton::clicked, this, &WordEditDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &WordEditDialog::reject);
    }

    QString getText() const {
        return textEdit->toPlainText();
    }

private:
    QTextEdit *textEdit;
    QString textValue;
};
