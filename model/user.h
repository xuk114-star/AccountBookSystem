#ifndef USER_H
#define USER_H

#include <QString>

struct User {
    int id = -1;
    QString username;
    QString password;
};

#endif // USER_H
