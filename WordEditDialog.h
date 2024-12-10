#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

class WordEditDialog : public QDialog {
    Q_OBJECT
public:
    explicit WordEditDialog(const QString &title, const QString &labelText, const QString &defaultValue, QWidget *parent = nullptr)
        : QDialog(parent), textValue(defaultValue) {
        setWindowTitle(title);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QLabel *label = new QLabel(labelText);
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
