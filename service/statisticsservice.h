#ifndef STATISTICSSERVICE_H
#define STATISTICSSERVICE_H

#include <QString>
#include <QList>
#include <QPair>
#include <QDate>

struct StatisticsSummary {
    double totalIncome = 0.0;
    double totalExpense = 0.0;
    double balance = 0.0;
};

struct CategoryExpenseItem {
    QString categoryName;
    double amount = 0.0;
    double percentage = 0.0;
};

struct MonthlyTrendItem {
    QString month;       // "yyyy-MM"
    double income = 0.0;
    double expense = 0.0;
};

class StatisticsService
{
public:
    StatisticsService() = default;

    // 总览统计（指定日期范围）
    StatisticsSummary getSummary(int userId, const QString &startDate, const QString &endDate);

    // 本月统计
    StatisticsSummary getCurrentMonthSummary(int userId);

    // 本年度收支统计
    StatisticsSummary getCurrentYearSummary(int userId);

    // 各分类支出占比（用于饼图）
    QList<CategoryExpenseItem> getCategoryExpenseBreakdown(int userId,
                                                           const QString &startDate,
                                                           const QString &endDate);

    // 月度收支趋势（用于柱状图/折线图）
    QList<MonthlyTrendItem> getMonthlyTrend(int userId, const QString &year);

    // 获取当前年月
    static QString currentMonth();  // "yyyy-MM"
    static QString currentYear();   // "yyyy"

private:
    QString currentMonthStart() const;
    QString currentMonthEnd() const;
    QString currentYearStart() const;
    QString currentYearEnd() const;
};

#endif // STATISTICSSERVICE_H
