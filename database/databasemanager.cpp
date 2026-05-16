#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen())
        m_db.close();
}

bool DatabaseManager::initialize(const QString &dbPath)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qCritical() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }

    if (!createTables())
        return false;

    insertDefaultCategories();
    return true;
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(m_db);

    const QStringList statements = {
        "CREATE TABLE IF NOT EXISTS users ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT UNIQUE NOT NULL,"
        "  password TEXT NOT NULL"
        ")",

        "CREATE TABLE IF NOT EXISTS categories ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  user_id INTEGER NOT NULL,"
        "  name TEXT NOT NULL,"
        "  type TEXT NOT NULL CHECK(type IN ('income','expense')),"
        "  FOREIGN KEY (user_id) REFERENCES users(id)"
        ")",

        "CREATE TABLE IF NOT EXISTS bills ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  user_id INTEGER NOT NULL,"
        "  type TEXT NOT NULL CHECK(type IN ('income','expense')),"
        "  category_id INTEGER NOT NULL,"
        "  amount REAL NOT NULL,"
        "  date TEXT NOT NULL,"
        "  note TEXT,"
        "  FOREIGN KEY (user_id) REFERENCES users(id),"
        "  FOREIGN KEY (category_id) REFERENCES categories(id)"
        ")",

        "CREATE TABLE IF NOT EXISTS budgets ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  user_id INTEGER UNIQUE NOT NULL,"
        "  monthly_budget REAL NOT NULL DEFAULT 0.0,"
        "  FOREIGN KEY (user_id) REFERENCES users(id)"
        ")"
    };

    for (const auto &sql : statements) {
        if (!query.exec(sql)) {
            qCritical() << "Failed to create table:" << query.lastError().text();
            return false;
        }
    }
    return true;
}

void DatabaseManager::insertDefaultCategories()
{
    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) FROM categories WHERE user_id = 0");
    if (query.next() && query.value(0).toInt() > 0)
        return;

    struct Cat { QString name; QString type; };
    const QList<Cat> defaults = {
        {"餐饮", "expense"}, {"交通", "expense"}, {"购物", "expense"},
        {"娱乐", "expense"}, {"学习", "expense"}, {"医疗", "expense"},
        {"其他", "expense"},
        {"工资", "income"},  {"兼职", "income"},  {"奖金", "income"},
        {"其他", "income"}
    };

    query.prepare("INSERT INTO categories (user_id, name, type) VALUES (0, ?, ?)");
    for (const auto &c : defaults) {
        query.addBindValue(c.name);
        query.addBindValue(c.type);
        query.exec();
    }
}
