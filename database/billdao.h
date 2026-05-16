#ifndef BILLDAO_H
#define BILLDAO_H

#include "../model/bill.h"
#include <QList>
#include <QString>

class BillDao
{
public:
    BillDao() = default;

    bool insertBill(const Bill &bill);
    bool updateBill(const Bill &bill);
    bool deleteBill(int id);
    Bill getBillById(int id);
    QList<Bill> getBillsByUser(int userId);
    QList<Bill> queryBills(int userId, const QString &startDate,
                           const QString &endDate, const QString &type,
                           int categoryId, double minAmount,
                           double maxAmount, const QString &keyword);

    // 成员三统计专用查询
    double getTotalIncome(int userId, const QString &startDate, const QString &endDate);
    double getTotalExpense(int userId, const QString &startDate, const QString &endDate);
    QList<QPair<QString, double>> getExpenseByCategory(int userId, const QString &startDate,
                                                        const QString &endDate);
    QList<QPair<QString, double>> getMonthlyExpense(int userId, const QString &year);
    QList<QPair<QString, double>> getMonthlyIncome(int userId, const QString &year);
};

#endif // BILLDAO_H
