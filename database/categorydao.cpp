#include "categorydao.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool CategoryDao::insertCategory(const Category &cat)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO categories (user_id, name, type) VALUES (?, ?, ?)");
    query.addBindValue(cat.userId);
    query.addBindValue(cat.name);
    query.addBindValue(cat.type);
    if (!query.exec()) {
        qWarning() << "insertCategory failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CategoryDao::updateCategory(const Category &cat)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE categories SET name = ?, type = ? WHERE id = ?");
    query.addBindValue(cat.name);
    query.addBindValue(cat.type);
    query.addBindValue(cat.id);
    return query.exec();
}

bool CategoryDao::deleteCategory(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM categories WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

QList<Category> CategoryDao::getCategoriesByUser(int userId)
{
    QList<Category> list;
    QSqlQuery query(DatabaseManager::instance().database());
    // Include default categories (user_id = 0) and user's own categories
    query.prepare("SELECT id, user_id, name, type FROM categories "
                  "WHERE user_id = 0 OR user_id = ? ORDER BY type, id");
    query.addBindValue(userId);
    if (query.exec()) {
        while (query.next()) {
            Category c;
            c.id = query.value(0).toInt();
            c.userId = query.value(1).toInt();
            c.name = query.value(2).toString();
            c.type = query.value(3).toString();
            list.append(c);
        }
    }
    return list;
}

QList<Category> CategoryDao::getCategoriesByType(int userId, const QString &type)
{
    QList<Category> list;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, user_id, name, type FROM categories "
                  "WHERE (user_id = 0 OR user_id = ?) AND type = ? ORDER BY id");
    query.addBindValue(userId);
    query.addBindValue(type);
    if (query.exec()) {
        while (query.next()) {
            Category c;
            c.id = query.value(0).toInt();
            c.userId = query.value(1).toInt();
            c.name = query.value(2).toString();
            c.type = query.value(3).toString();
            list.append(c);
        }
    }
    return list;
}

Category CategoryDao::getCategoryById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, user_id, name, type FROM categories WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        Category c;
        c.id = query.value(0).toInt();
        c.userId = query.value(1).toInt();
        c.name = query.value(2).toString();
        c.type = query.value(3).toString();
        return c;
    }
    return Category();
}

QList<Category> CategoryDao::getDefaultCategories()
{
    QList<Category> list;
    QSqlQuery query(DatabaseManager::instance().database());
    if (!query.exec("SELECT id, user_id, name, type FROM categories WHERE user_id = 0"))
        return list;
    while (query.next()) {
        Category c;
        c.id = query.value(0).toInt();
        c.userId = query.value(1).toInt();
        c.name = query.value(2).toString();
        c.type = query.value(3).toString();
        list.append(c);
    }
    return list;
}
