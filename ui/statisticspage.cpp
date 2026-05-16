#include "statisticspage.h"
#include "ui_statisticspage.h"
#include "../service/statisticsservice.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QLabel>

StatisticsPage::StatisticsPage(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::StatisticsPage), m_userId(userId)
{
    ui->setupUi(this);

    // 设置默认日期范围：本月
    QDate now = QDate::currentDate();
    ui->startDateEdit->setDate(QDate(now.year(), now.month(), 1));
    ui->endDateEdit->setDate(QDate(now.year(), now.month(), now.daysInMonth()));

    // 图表类型选择
    ui->chartTypeCombo->addItems({"饼图 - 分类支出占比", "柱状图 - 月度收支对比", "折线图 - 收支趋势"});

    connect(ui->refreshButton, &QPushButton::clicked, this, &StatisticsPage::onRefreshClicked);
    connect(ui->chartTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StatisticsPage::refreshCharts);

    // 初始加载
    updateSummaryLabels();
    refreshCharts();
}

StatisticsPage::~StatisticsPage() { delete ui; }

void StatisticsPage::onRefreshClicked()
{
    updateSummaryLabels();
    refreshCharts();
}

void StatisticsPage::updateSummaryLabels()
{
    StatisticsService svc;
    QString start = ui->startDateEdit->date().toString("yyyy-MM-dd");
    QString end = ui->endDateEdit->date().toString("yyyy-MM-dd");

    auto summary = svc.getSummary(m_userId, start, end);
    auto monthSum = svc.getCurrentMonthSummary(m_userId);
    auto yearSum = svc.getCurrentYearSummary(m_userId);

    ui->totalIncomeLabel->setText(QString::number(summary.totalIncome, 'f', 2));
    ui->totalExpenseLabel->setText(QString::number(summary.totalExpense, 'f', 2));
    ui->balanceLabel->setText(QString::number(summary.balance, 'f', 2));

    ui->monthIncomeLabel->setText(QString::number(monthSum.totalIncome, 'f', 2));
    ui->monthExpenseLabel->setText(QString::number(monthSum.totalExpense, 'f', 2));

    ui->yearIncomeLabel->setText(QString::number(yearSum.totalIncome, 'f', 2));
    ui->yearExpenseLabel->setText(QString::number(yearSum.totalExpense, 'f', 2));

    // 余额颜色
    if (summary.balance < 0)
        ui->balanceLabel->setStyleSheet("color: red; font-weight: bold; font-size: 16px;");
    else
        ui->balanceLabel->setStyleSheet("color: green; font-weight: bold; font-size: 16px;");
}

void StatisticsPage::refreshCharts()
{
    int chartType = ui->chartTypeCombo->currentIndex();
    switch (chartType) {
    case 0: drawPieChart(); break;
    case 1: drawBarChart(); break;
    case 2: drawLineChart(); break;
    }
}

void StatisticsPage::drawPieChart()
{
    StatisticsService svc;
    QString start = ui->startDateEdit->date().toString("yyyy-MM-dd");
    QString end = ui->endDateEdit->date().toString("yyyy-MM-dd");

    auto items = svc.getCategoryExpenseBreakdown(m_userId, start, end);

    auto *series = new QPieSeries();

    if (items.isEmpty()) {
        series->append("暂无数据", 1);
    } else {
        for (const auto &item : items) {
            auto *slice = series->append(
                QString("%1 ¥%2 (%3%)")
                    .arg(item.categoryName)
                    .arg(item.amount, 0, 'f', 2)
                    .arg(item.percentage, 0, 'f', 1),
                item.amount);
            slice->setExplodeDistanceFactor(0.08);
        }
    }

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("支出分类占比");
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 替换旧的 chart view
    if (m_chartView) {
        ui->chartLayout->removeWidget(m_chartView);
        delete m_chartView;
    }
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartLayout->addWidget(m_chartView);
}

void StatisticsPage::drawBarChart()
{
    StatisticsService svc;
    QString year = QString::number(ui->startDateEdit->date().year());
    auto trends = svc.getMonthlyTrend(m_userId, year);

    auto *incomeSet = new QBarSet("收入");
    auto *expenseSet = new QBarSet("支出");
    QStringList categories;

    for (const auto &t : trends) {
        *incomeSet << t.income;
        *expenseSet << t.expense;
        categories << t.month.mid(5); // 取 "MM" 部分
    }

    auto *series = new QBarSeries();
    series->append(incomeSet);
    series->append(expenseSet);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString("%1 年月度收支对比").arg(year));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    auto *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto *axisY = new QValueAxis();
    axisY->setTitleText("金额（元）");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    if (m_chartView) {
        ui->chartLayout->removeWidget(m_chartView);
        delete m_chartView;
    }
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartLayout->addWidget(m_chartView);
}

void StatisticsPage::drawLineChart()
{
    StatisticsService svc;
    QString year = QString::number(ui->startDateEdit->date().year());
    auto trends = svc.getMonthlyTrend(m_userId, year);

    auto *incomeSeries = new QLineSeries();
    incomeSeries->setName("收入");
    auto *expenseSeries = new QLineSeries();
    expenseSeries->setName("支出");

    QStringList categories;
    double maxVal = 0;
    for (int i = 0; i < trends.size(); ++i) {
        const auto &t = trends[i];
        incomeSeries->append(i, t.income);
        expenseSeries->append(i, t.expense);
        categories << t.month.mid(5);
        maxVal = std::max(maxVal, std::max(t.income, t.expense));
    }

    auto *chart = new QChart();
    chart->addSeries(incomeSeries);
    chart->addSeries(expenseSeries);
    chart->setTitle(QString("%1 年收支趋势").arg(year));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    auto *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    incomeSeries->attachAxis(axisX);
    expenseSeries->attachAxis(axisX);

    auto *axisY = new QValueAxis();
    axisY->setTitleText("金额（元）");
    axisY->setRange(0, maxVal * 1.1);
    chart->addAxis(axisY, Qt::AlignLeft);
    incomeSeries->attachAxis(axisY);
    expenseSeries->attachAxis(axisY);

    if (m_chartView) {
        ui->chartLayout->removeWidget(m_chartView);
        delete m_chartView;
    }
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartLayout->addWidget(m_chartView);
}
