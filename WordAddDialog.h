#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>

#define DEFAULT_VALUE "{\n" \
"    \"释义\": \"\",\n" \
    "    \"英式英标\": \"\",\n" \
    "    \"美式英标\": \"\",\n" \
    "    \"词频\": ,\n" \
    "    \"难度\": ,\n" \
    "    \"认识率\": \n" \
    "}\n"

class WordAddDialog : public QDialog {
    Q_OBJECT
public:
    explicit WordAddDialog(const QString &title, const QString &labelText, QWidget *parent = nullptr)
        : QDialog(parent) {
        setWindowTitle(title);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QLabel *label = new QLabel(labelText);
        mainLayout->addWidget(label);

        lineEdit = new QLineEdit(this);
        mainLayout->addWidget(lineEdit);

        textEdit = new QTextEdit(this);
        textEdit->setText(DEFAULT_VALUE);
        mainLayout->addWidget(textEdit);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *okButton = new QPushButton(tr("完成"), this);
        QPushButton *cancelButton = new QPushButton(tr("取消"), this);
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);

        connect(okButton, &QPushButton::clicked, this, &WordAddDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &WordAddDialog::reject);
    }

    QString getWord() const{
        return lineEdit->text();
    }

    QString getText() const {
        return textEdit->toPlainText();
    }

private:
    QLineEdit *lineEdit;
    QTextEdit *textEdit;
    QString textValue;
};
