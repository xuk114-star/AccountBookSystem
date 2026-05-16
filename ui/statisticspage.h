#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QWidget>
#include <QDate>
#include <QtCharts/QChartView>

namespace Ui { class StatisticsPage; }

class StatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPage(int userId, QWidget *parent = nullptr);
    ~StatisticsPage();

private slots:
    void onRefreshClicked();
    void refreshCharts();

private:
    void updateSummaryLabels();
    void drawPieChart();
    void drawBarChart();
    void drawLineChart();

    Ui::StatisticsPage *ui;
    int m_userId;
    QChartView *m_chartView = nullptr;
};

#endif // STATISTICSPAGE_H
