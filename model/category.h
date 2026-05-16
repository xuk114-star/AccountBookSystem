#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

struct Category {
    int id = -1;
    int userId = -1;
    QString name;
    QString type; // "income" or "expense"
};

#endif // CATEGORY_H
