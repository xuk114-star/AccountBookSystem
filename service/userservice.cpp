#include "userservice.h"
#include "../database/userdao.h"

bool UserService::registerUser(const QString &username, const QString &password, QString &errorMsg)
{
    if (username.trimmed().isEmpty()) {
        errorMsg = "用户名不能为空";
        return false;
    }
    if (password.length() < 4) {
        errorMsg = "密码长度不能少于4位";
        return false;
    }

    UserDao dao;
    User existing = dao.getUserByUsername(username.trimmed());
    if (existing.id > 0) {
        errorMsg = "用户名已存在";
        return false;
    }

    User user;
    user.username = username.trimmed();
    user.password = password;
    return dao.insertUser(user);
}

int UserService::login(const QString &username, const QString &password, QString &errorMsg)
{
    UserDao dao;
    if (dao.validateLogin(username, password)) {
        User u = dao.getUserByUsername(username);
        return u.id;
    }
    errorMsg = "用户名或密码错误";
    return -1;
}
