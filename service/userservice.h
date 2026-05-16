#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QString>

class UserService
{
public:
    UserService() = default;

    bool registerUser(const QString &username, const QString &password, QString &errorMsg);
    int login(const QString &username, const QString &password, QString &errorMsg);
};

#endif // USERSERVICE_H
