#include "SettlementWindow.h"

#include <QSqlDatabase>
#include <QPieSeries>
#include <QChart>
#include <QHBoxLayout>
#include <QChartView>
#include <QListView>
#include <QStringListModel>

SettlementWindow::SettlementWindow(QWidget *parent, QList<QString> correctAnswers, QList<QString> wrongAnswers) {
    QHBoxLayout *layout = new QHBoxLayout(this);

    QListView *listView = new QListView();
    QStringListModel *model = new QStringListModel();
    QStringList correctAnswersList;
    for (const QString &answer: correctAnswers) {
        correctAnswersList.append(answer);
    }
    QStringList wrongAnswersList;
    for (const QString &answer: wrongAnswers) {
        wrongAnswersList.append(answer);
    }
    model->setStringList(correctAnswersList + wrongAnswersList);
    listView->setModel(model);
    CustomDelegate *delegate = new CustomDelegate(correctAnswersList, wrongAnswersList);
    listView->setItemDelegate(delegate); // 设置代理
    layout->addWidget(listView);

    QPieSeries *series = new QPieSeries();
    // 计算正确率
    QString correctRate = QString::number(
            (double) correctAnswers.size() / (correctAnswers.size() + wrongAnswers.size()) * 100, 'f', 2) + "%";
    series->append("正确", correctAnswers.size());
    series->append("错误", wrongAnswers.size());
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("正确率：" + correctRate);
    chart->legend()->show();

    // 创建图表视图
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chartView);
}