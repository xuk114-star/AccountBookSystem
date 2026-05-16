#ifndef BUDGETDAO_H
#define BUDGETDAO_H

#include "../model/budget.h"

class BudgetDao
{
public:
    BudgetDao() = default;

    Budget getBudgetByUser(int userId);
    bool setBudget(int userId, double monthlyBudget);
    bool updateBudget(const Budget &budget);
};

#endif // BUDGETDAO_H
