#include "budgetdao.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Budget BudgetDao::getBudgetByUser(int userId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, user_id, monthly_budget FROM budgets WHERE user_id = ?");
    query.addBindValue(userId);
    if (query.exec() && query.next()) {
        Budget b;
        b.id = query.value(0).toInt();
        b.userId = query.value(1).toInt();
        b.monthlyBudget = query.value(2).toDouble();
        return b;
    }
    return Budget();
}

bool BudgetDao::setBudget(int userId, double monthlyBudget)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO budgets (user_id, monthly_budget) VALUES (?, ?) "
                  "ON CONFLICT(user_id) DO UPDATE SET monthly_budget = excluded.monthly_budget");
    query.addBindValue(userId);
    query.addBindValue(monthlyBudget);
    if (!query.exec()) {
        qWarning() << "setBudget failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BudgetDao::updateBudget(const Budget &budget)
{
    return setBudget(budget.userId, budget.monthlyBudget);
}
