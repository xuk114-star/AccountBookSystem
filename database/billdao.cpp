#include "billdao.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool BillDao::insertBill(const Bill &bill)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO bills (user_id, type, category_id, amount, date, note) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(bill.userId);
    query.addBindValue(bill.type);
    query.addBindValue(bill.categoryId);
    query.addBindValue(bill.amount);
    query.addBindValue(bill.date);
    query.addBindValue(bill.note);
    if (!query.exec()) {
        qWarning() << "insertBill failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BillDao::updateBill(const Bill &bill)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE bills SET type = ?, category_id = ?, amount = ?, "
                  "date = ?, note = ? WHERE id = ?");
    query.addBindValue(bill.type);
    query.addBindValue(bill.categoryId);
    query.addBindValue(bill.amount);
    query.addBindValue(bill.date);
    query.addBindValue(bill.note);
    query.addBindValue(bill.id);
    return query.exec();
}

bool BillDao::deleteBill(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM bills WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

Bill BillDao::getBillById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT b.id, b.user_id, b.type, b.category_id, c.name, "
                  "b.amount, b.date, b.note "
                  "FROM bills b LEFT JOIN categories c ON b.category_id = c.id "
                  "WHERE b.id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        Bill b;
        b.id = query.value(0).toInt();
        b.userId = query.value(1).toInt();
        b.type = query.value(2).toString();
        b.categoryId = query.value(3).toInt();
        b.categoryName = query.value(4).toString();
        b.amount = query.value(5).toDouble();
        b.date = query.value(6).toString();
        b.note = query.value(7).toString();
        return b;
    }
    return Bill();
}

QList<Bill> BillDao::getBillsByUser(int userId)
{
    QList<Bill> list;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT b.id, b.user_id, b.type, b.category_id, c.name, "
                  "b.amount, b.date, b.note "
                  "FROM bills b LEFT JOIN categories c ON b.category_id = c.id "
                  "WHERE b.user_id = ? ORDER BY b.date DESC, b.id DESC");
    query.addBindValue(userId);
    if (query.exec()) {
        while (query.next()) {
            Bill b;
            b.id = query.value(0).toInt();
            b.userId = query.value(1).toInt();
            b.type = query.value(2).toString();
            b.categoryId = query.value(3).toInt();
            b.categoryName = query.value(4).toString();
            b.amount = query.value(5).toDouble();
            b.date = query.value(6).toString();
            b.note = query.value(7).toString();
            list.append(b);
        }
    }
    return list;
}

QList<Bill> BillDao::queryBills(int userId, const QString &startDate,
                                 const QString &endDate, const QString &type,
                                 int categoryId, double minAmount,
                                 double maxAmount, const QString &keyword)
{
    QList<Bill> list;
    QString sql = "SELECT b.id, b.user_id, b.type, b.category_id, c.name, "
                  "b.amount, b.date, b.note "
                  "FROM bills b LEFT JOIN categories c ON b.category_id = c.id "
                  "WHERE b.user_id = ?";
    QVariantList bindings;
    bindings << userId;

    if (!startDate.isEmpty()) {
        sql += " AND b.date >= ?";
        bindings << startDate;
    }
    if (!endDate.isEmpty()) {
        sql += " AND b.date <= ?";
        bindings << endDate;
    }
    if (!type.isEmpty()) {
        sql += " AND b.type = ?";
        bindings << type;
    }
    if (categoryId > 0) {
        sql += " AND b.category_id = ?";
        bindings << categoryId;
    }
    if (minAmount >= 0) {
        sql += " AND b.amount >= ?";
        bindings << minAmount;
    }
    if (maxAmount > 0) {
        sql += " AND b.amount <= ?";
        bindings << maxAmount;
    }
    if (!keyword.isEmpty()) {
        sql += " AND b.note LIKE ?";
        bindings << ("%" + keyword + "%");
    }

    sql += " ORDER BY b.date DESC, b.id DESC";

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(sql);
    for (const auto &v : bindings)
        query.addBindValue(v);

    if (query.exec()) {
        while (query.next()) {
            Bill b;
            b.id = query.value(0).toInt();
            b.userId = query.value(1).toInt();
            b.type = query.value(2).toString();
            b.categoryId = query.value(3).toInt();
            b.categoryName = query.value(4).toString();
            b.amount = query.value(5).toDouble();
            b.date = query.value(6).toString();
            b.note = query.value(7).toString();
            list.append(b);
        }
    }
    return list;
}

// ==================== 成员三：统计专用查询 ====================

double BillDao::getTotalIncome(int userId, const QString &startDate, const QString &endDate)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT COALESCE(SUM(amount), 0) FROM bills "
                  "WHERE user_id = ? AND type = 'income' AND date >= ? AND date <= ?");
    query.addBindValue(userId);
    query.addBindValue(startDate);
    query.addBindValue(endDate);
    if (query.exec() && query.next())
        return query.value(0).toDouble();
    return 0.0;
}

double BillDao::getTotalExpense(int userId, const QString &startDate, const QString &endDate)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT COALESCE(SUM(amount), 0) FROM bills "
                  "WHERE user_id = ? AND type = 'expense' AND date >= ? AND date <= ?");
    query.addBindValue(userId);
    query.addBindValue(startDate);
    query.addBindValue(endDate);
    if (query.exec() && query.next())
        return query.value(0).toDouble();
    return 0.0;
}

QList<QPair<QString, double>> BillDao::getExpenseByCategory(int userId,
                                                              const QString &startDate,
                                                              const QString &endDate)
{
    QList<QPair<QString, double>> result;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT c.name, COALESCE(SUM(b.amount), 0) "
                  "FROM bills b "
                  "JOIN categories c ON b.category_id = c.id "
                  "WHERE b.user_id = ? AND b.type = 'expense' "
                  "AND b.date >= ? AND b.date <= ? "
                  "GROUP BY c.name ORDER BY SUM(b.amount) DESC");
    query.addBindValue(userId);
    query.addBindValue(startDate);
    query.addBindValue(endDate);
    if (query.exec()) {
        while (query.next())
            result.append({query.value(0).toString(), query.value(1).toDouble()});
    }
    return result;
}

QList<QPair<QString, double>> BillDao::getMonthlyExpense(int userId, const QString &year)
{
    QList<QPair<QString, double>> result;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT substr(date, 1, 7) AS month, COALESCE(SUM(amount), 0) "
                  "FROM bills WHERE user_id = ? AND type = 'expense' AND date LIKE ? "
                  "GROUP BY month ORDER BY month");
    query.addBindValue(userId);
    query.addBindValue(year + "%");
    if (query.exec()) {
        while (query.next())
            result.append({query.value(0).toString(), query.value(1).toDouble()});
    }
    return result;
}

QList<QPair<QString, double>> BillDao::getMonthlyIncome(int userId, const QString &year)
{
    QList<QPair<QString, double>> result;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT substr(date, 1, 7) AS month, COALESCE(SUM(amount), 0) "
                  "FROM bills WHERE user_id = ? AND type = 'income' AND date LIKE ? "
                  "GROUP BY month ORDER BY month");
    query.addBindValue(userId);
    query.addBindValue(year + "%");
    if (query.exec()) {
        while (query.next())
            result.append({query.value(0).toString(), query.value(1).toDouble()});
    }
    return result;
}
