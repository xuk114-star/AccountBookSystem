#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "../service/userservice.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setWindowTitle("个人记账系统 - 登录");
    setFixedSize(400, 350);

    connect(ui->loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked,
            this, &LoginWindow::onRegisterClicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::onLoginClicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();

    UserService svc;
    QString error;
    int userId = svc.login(username, password, error);
    if (userId > 0) {
        emit loginSuccess(userId);
    } else {
        QMessageBox::warning(this, "登录失败", error);
    }
}

void LoginWindow::onRegisterClicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();

    UserService svc;
    QString error;
    if (svc.registerUser(username, password, error)) {
        QMessageBox::information(this, "注册成功", "账号注册成功，请登录");
    } else {
        QMessageBox::warning(this, "注册失败", error);
    }
}
