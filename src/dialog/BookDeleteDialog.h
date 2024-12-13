#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>

class BookDeleteDialog : public QDialog {
Q_OBJECT

    /**
     * @brief BookDeleteDialog 删除单词本对话框
     * @param parent 父窗口
     */
public:
    explicit BookDeleteDialog(QWidget *parent = nullptr)
            : QDialog(parent) {
        setWindowTitle("删除单词本");

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QLabel *label = new QLabel("你确定要删除吗？");
        mainLayout->addWidget(label);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *okButton = new QPushButton(tr("确定"), this);
        QPushButton *cancelButton = new QPushButton(tr("取消"), this);
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);

        connect(okButton, &QPushButton::clicked, this, &BookDeleteDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &BookDeleteDialog::reject);
    }

private:
    QString textValue;
};
