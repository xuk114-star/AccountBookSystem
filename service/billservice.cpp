#include "billservice.h"
#include "../database/billdao.h"

bool BillService::addBill(const Bill &bill) { BillDao dao; return dao.insertBill(bill); }
bool BillService::updateBill(const Bill &bill) { BillDao dao; return dao.updateBill(bill); }
bool BillService::deleteBill(int id) { BillDao dao; return dao.deleteBill(id); }
Bill BillService::getBill(int id) { BillDao dao; return dao.getBillById(id); }
QList<Bill> BillService::getUserBills(int userId) { BillDao dao; return dao.getBillsByUser(userId); }

QList<Bill> BillService::queryBills(int userId, const QString &startDate,
                                     const QString &endDate, const QString &type,
                                     int categoryId, double minAmount,
                                     double maxAmount, const QString &keyword)
{
    BillDao dao;
    return dao.queryBills(userId, startDate, endDate, type,
                          categoryId, minAmount, maxAmount, keyword);
}
