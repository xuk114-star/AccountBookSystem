#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "billform.h"
#include "categorypage.h"
#include "statisticspage.h"
#include "budgetpage.h"
#include "settingspage.h"
#include "../database/userdao.h"
#include <QTabWidget>
#include <QMenuBar>

MainWindow::MainWindow(int userId, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_userId(userId)
{
    ui->setupUi(this);

    // 获取用户名并显示在标题栏
    UserDao dao;
    User u = dao.getUserById(m_userId);
    QString username = u.username.isEmpty() ? QString::number(m_userId) : u.username;
    setWindowTitle(QString("个人记账与消费分析系统 - %1").arg(username));

    // 登出菜单
    QMenu *fileMenu = menuBar()->addMenu("系统");
    QAction *logoutAction = fileMenu->addAction("退出登录");
    connect(logoutAction, &QAction::triggered, this, &MainWindow::onLogoutClicked);

    auto *tabs = ui->tabWidget;

    m_billPage = new BillForm(m_userId);
    m_categoryPage = new CategoryPage(m_userId);
    m_statisticsPage = new StatisticsPage(m_userId);
    m_budgetPage = new BudgetPage(m_userId);
    m_settingsPage = new SettingsPage();

    tabs->addTab(m_billPage, "账单管理");
    tabs->addTab(m_categoryPage, "分类管理");
    tabs->addTab(m_statisticsPage, "消费统计");
    tabs->addTab(m_budgetPage, "预算提醒");
    tabs->addTab(m_settingsPage, "系统设置");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLogoutClicked()
{
    close();
    emit loggedOut();
}
