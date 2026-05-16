#ifndef USERDAO_H
#define USERDAO_H

#include "../model/user.h"
#include <QString>

class UserDao
{
public:
    UserDao() = default;

    bool insertUser(const User &user);
    User getUserByUsername(const QString &username);
    User getUserById(int id);
    bool validateLogin(const QString &username, const QString &password);
};

#endif // USERDAO_H
