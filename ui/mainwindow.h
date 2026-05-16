#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }
class BillForm;
class CategoryPage;
class StatisticsPage;
class BudgetPage;
class SettingsPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int userId, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void loggedOut();

private slots:
    void onLogoutClicked();

private:
    Ui::MainWindow *ui;
    int m_userId;
    BillForm *m_billPage;
    CategoryPage *m_categoryPage;
    StatisticsPage *m_statisticsPage;
    BudgetPage *m_budgetPage;
    SettingsPage *m_settingsPage;
};

#endif // MAINWINDOW_H
