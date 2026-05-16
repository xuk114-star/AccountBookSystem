#ifndef BUDGETSERVICE_H
#define BUDGETSERVICE_H

#include <QString>

enum class BudgetStatus {
    Normal,    // 支出 < 80%
    Warning,   // 支出 >= 80% 且 < 100%
    Exceeded   // 支出 >= 100%
};

struct BudgetInfo {
    double monthlyBudget = 0.0;
    double currentSpending = 0.0;
    double remaining = 0.0;
    double progressPercent = 0.0; // 0.0 ~ 100+
    BudgetStatus status = BudgetStatus::Normal;
};

class BudgetService
{
public:
    BudgetService() = default;

    // 获取用户的月度预算
    double getMonthlyBudget(int userId);

    // 设置月度预算
    bool setMonthlyBudget(int userId, double amount);

    // 获取预算执行情况（本月）
    BudgetInfo getBudgetStatus(int userId);

    // 判断预算状态并返回提示文字
    BudgetStatus checkBudget(int userId);
    static QString statusMessage(BudgetStatus status);
};

#endif // BUDGETSERVICE_H
