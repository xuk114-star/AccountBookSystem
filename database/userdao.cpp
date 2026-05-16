#include "userdao.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool UserDao::insertUser(const User &user)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(user.username);
    query.addBindValue(user.password);
    if (!query.exec()) {
        qWarning() << "insertUser failed:" << query.lastError().text();
        return false;
    }
    return true;
}

User UserDao::getUserByUsername(const QString &username)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, username, password FROM users WHERE username = ?");
    query.addBindValue(username);
    if (query.exec() && query.next()) {
        User u;
        u.id = query.value(0).toInt();
        u.username = query.value(1).toString();
        u.password = query.value(2).toString();
        return u;
    }
    return User();
}

User UserDao::getUserById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, username, password FROM users WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        User u;
        u.id = query.value(0).toInt();
        u.username = query.value(1).toString();
        u.password = query.value(2).toString();
        return u;
    }
    return User();
}

bool UserDao::validateLogin(const QString &username, const QString &password)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);
    return query.exec() && query.next();
}
