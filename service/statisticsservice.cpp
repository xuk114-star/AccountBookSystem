#include "statisticsservice.h"
#include "../database/billdao.h"
#include <QDate>
#include <cmath>

QString StatisticsService::currentMonth()
{
    return QDate::currentDate().toString("yyyy-MM");
}

QString StatisticsService::currentYear()
{
    return QDate::currentDate().toString("yyyy");
}

QString StatisticsService::currentMonthStart() const
{
    QDate now = QDate::currentDate();
    return QDate(now.year(), now.month(), 1).toString("yyyy-MM-dd");
}

QString StatisticsService::currentMonthEnd() const
{
    QDate now = QDate::currentDate();
    int days = now.daysInMonth();
    return QDate(now.year(), now.month(), days).toString("yyyy-MM-dd");
}

QString StatisticsService::currentYearStart() const
{
    return QString("%1-01-01").arg(QDate::currentDate().year());
}

QString StatisticsService::currentYearEnd() const
{
    return QString("%1-12-31").arg(QDate::currentDate().year());
}

StatisticsSummary StatisticsService::getSummary(int userId,
                                                  const QString &startDate,
                                                  const QString &endDate)
{
    BillDao dao;
    StatisticsSummary s;
    s.totalIncome = dao.getTotalIncome(userId, startDate, endDate);
    s.totalExpense = dao.getTotalExpense(userId, startDate, endDate);
    s.balance = s.totalIncome - s.totalExpense;
    return s;
}

StatisticsSummary StatisticsService::getCurrentMonthSummary(int userId)
{
    return getSummary(userId, currentMonthStart(), currentMonthEnd());
}

StatisticsSummary StatisticsService::getCurrentYearSummary(int userId)
{
    return getSummary(userId, currentYearStart(), currentYearEnd());
}

QList<CategoryExpenseItem> StatisticsService::getCategoryExpenseBreakdown(
    int userId, const QString &startDate, const QString &endDate)
{
    BillDao dao;
    auto raw = dao.getExpenseByCategory(userId, startDate, endDate);

    double total = 0.0;
    for (const auto &p : raw)
        total += p.second;

    QList<CategoryExpenseItem> result;
    for (const auto &p : raw) {
        if (p.second <= 0.0) continue;
        CategoryExpenseItem item;
        item.categoryName = p.first;
        item.amount = p.second;
        item.percentage = total > 0 ? (p.second / total) * 100.0 : 0.0;
        result.append(item);
    }
    return result;
}

QList<MonthlyTrendItem> StatisticsService::getMonthlyTrend(int userId, const QString &year)
{
    BillDao dao;
    auto expenses = dao.getMonthlyExpense(userId, year);
    auto incomes = dao.getMonthlyIncome(userId, year);

    // 合并收入支出到同一个月份条目
    QMap<QString, MonthlyTrendItem> map;

    for (const auto &p : incomes) {
        MonthlyTrendItem item;
        item.month = p.first;
        item.income = p.second;
        item.expense = 0.0;
        map[p.first] = item;
    }
    for (const auto &p : expenses) {
        if (map.contains(p.first)) {
            map[p.first].expense = p.second;
        } else {
            MonthlyTrendItem item;
            item.month = p.first;
            item.income = 0.0;
            item.expense = p.second;
            map[p.first] = item;
        }
    }

    // 补全缺失的月份（1-12月）
    QList<MonthlyTrendItem> result;
    for (int m = 1; m <= 12; ++m) {
        QString key = QString("%1-%2").arg(year).arg(m, 2, 10, QChar('0'));
        if (map.contains(key)) {
            result.append(map[key]);
        } else {
            MonthlyTrendItem item;
            item.month = key;
            item.income = 0.0;
            item.expense = 0.0;
            result.append(item);
        }
    }
    return result;
}
