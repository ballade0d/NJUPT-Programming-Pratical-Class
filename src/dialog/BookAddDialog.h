#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>

class BookAddDialog : public QDialog {
Q_OBJECT

    /**
     * @brief BookAddDialog 新增单词本对话框
     * @param parent 父窗口
     */
public:
    explicit BookAddDialog(QWidget *parent = nullptr)
            : QDialog(parent) {
        setWindowTitle("新增单词本");

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QLabel *label = new QLabel("请编辑：");
        mainLayout->addWidget(label);

        lineEdit = new QLineEdit(this);
        lineEdit->setPlaceholderText("请输入单词本名称");
        mainLayout->addWidget(lineEdit);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *okButton = new QPushButton(tr("完成"), this);
        QPushButton *cancelButton = new QPushButton(tr("取消"), this);
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);

        connect(okButton, &QPushButton::clicked, this, &BookAddDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &BookAddDialog::reject);
    }

    QString getBook() const {
        return lineEdit->text();
    }

private:
    QLineEdit *lineEdit;
    QString textValue;
};
