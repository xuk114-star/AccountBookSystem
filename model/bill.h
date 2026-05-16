#ifndef BILL_H
#define BILL_H

#include <QString>

struct Bill {
    int id = -1;
    int userId = -1;
    QString type;       // "income" or "expense"
    int categoryId = -1;
    QString categoryName;
    double amount = 0.0;
    QString date;
    QString note;
};

#endif // BILL_H
