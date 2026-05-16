#include "budgetservice.h"
#include "../database/budgetdao.h"
#include "../database/billdao.h"
#include <QDate>

static QString currentMonthStart()
{
    QDate now = QDate::currentDate();
    return QDate(now.year(), now.month(), 1).toString("yyyy-MM-dd");
}

static QString currentMonthEnd()
{
    QDate now = QDate::currentDate();
    return QDate(now.year(), now.month(), now.daysInMonth()).toString("yyyy-MM-dd");
}

double BudgetService::getMonthlyBudget(int userId)
{
    BudgetDao dao;
    Budget b = dao.getBudgetByUser(userId);
    return b.monthlyBudget;
}

bool BudgetService::setMonthlyBudget(int userId, double amount)
{
    BudgetDao dao;
    return dao.setBudget(userId, amount);
}

BudgetInfo BudgetService::getBudgetStatus(int userId)
{
    BudgetInfo info;
    info.monthlyBudget = getMonthlyBudget(userId);

    BillDao billDao;
    info.currentSpending = billDao.getTotalExpense(userId, currentMonthStart(), currentMonthEnd());
    info.remaining = info.monthlyBudget - info.currentSpending;

    if (info.monthlyBudget > 0)
        info.progressPercent = (info.currentSpending / info.monthlyBudget) * 100.0;
    else if (info.currentSpending > 0)
        info.progressPercent = 100.0;

    // 直接内联判断，避免二次查询
    if (info.monthlyBudget <= 0.0)
        info.status = BudgetStatus::Normal;
    else if (info.progressPercent >= 100.0)
        info.status = BudgetStatus::Exceeded;
    else if (info.progressPercent >= 80.0)
        info.status = BudgetStatus::Warning;
    else
        info.status = BudgetStatus::Normal;

    return info;
}

BudgetStatus BudgetService::checkBudget(int userId)
{
    double budget = getMonthlyBudget(userId);
    if (budget <= 0.0)
        return BudgetStatus::Normal;

    BillDao billDao;
    double spending = billDao.getTotalExpense(userId, currentMonthStart(), currentMonthEnd());
    double percent = (spending / budget) * 100.0;

    if (percent >= 100.0)
        return BudgetStatus::Exceeded;
    if (percent >= 80.0)
        return BudgetStatus::Warning;
    return BudgetStatus::Normal;
}

QString BudgetService::statusMessage(BudgetStatus status)
{
    switch (status) {
    case BudgetStatus::Exceeded: return "⚠ 警告：预算已超支！请控制消费！";
    case BudgetStatus::Warning:  return "⚡ 提醒：预算使用已超过 80%，请注意！";
    case BudgetStatus::Normal:   return "✓ 预算状态正常";
    }
    return "";
}
