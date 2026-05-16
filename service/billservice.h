#ifndef BILLSERVICE_H
#define BILLSERVICE_H

#include "../model/bill.h"
#include <QList>
#include <QString>

class BillService
{
public:
    BillService() = default;

    bool addBill(const Bill &bill);
    bool updateBill(const Bill &bill);
    bool deleteBill(int id);
    Bill getBill(int id);
    QList<Bill> getUserBills(int userId);
    QList<Bill> queryBills(int userId, const QString &startDate,
                           const QString &endDate, const QString &type,
                           int categoryId, double minAmount,
                           double maxAmount, const QString &keyword);
};

#endif // BILLSERVICE_H
