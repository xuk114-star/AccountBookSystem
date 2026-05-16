#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QWidget>

namespace Ui { class BudgetPage; }

class BudgetPage : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetPage(int userId, QWidget *parent = nullptr);
    ~BudgetPage();

private slots:
    void onSetBudgetClicked();
    void refreshBudgetDisplay();

private:
    Ui::BudgetPage *ui;
    int m_userId;
};

#endif // BUDGETPAGE_H
