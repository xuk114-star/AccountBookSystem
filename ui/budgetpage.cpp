#include "budgetpage.h"
#include "ui_budgetpage.h"
#include "../service/budgetservice.h"
#include <QMessageBox>

BudgetPage::BudgetPage(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::BudgetPage), m_userId(userId)
{
    ui->setupUi(this);

    connect(ui->setBudgetButton, &QPushButton::clicked,
            this, &BudgetPage::onSetBudgetClicked);
    connect(ui->refreshButton, &QPushButton::clicked,
            this, &BudgetPage::refreshBudgetDisplay);

    refreshBudgetDisplay();
}

BudgetPage::~BudgetPage() { delete ui; }

void BudgetPage::onSetBudgetClicked()
{
    double amount = ui->budgetSpinBox->value();
    if (amount <= 0) {
        QMessageBox::warning(this, "错误", "预算金额必须大于零");
        return;
    }

    BudgetService svc;
    if (svc.setMonthlyBudget(m_userId, amount)) {
        QMessageBox::information(this, "成功", "月度预算已更新");
        refreshBudgetDisplay();
    } else {
        QMessageBox::warning(this, "失败", "预算设置失败");
    }
}

void BudgetPage::refreshBudgetDisplay()
{
    BudgetService svc;
    BudgetInfo info = svc.getBudgetStatus(m_userId);

    // 预算额度
    ui->budgetAmountLabel->setText(
        QString("¥ %1").arg(info.monthlyBudget, 0, 'f', 2));

    // 本月已支出
    ui->currentSpendingLabel->setText(
        QString("¥ %1").arg(info.currentSpending, 0, 'f', 2));

    // 剩余额度
    ui->remainingLabel->setText(
        QString("¥ %1").arg(info.remaining, 0, 'f', 2));

    // 进度条
    int percent = static_cast<int>(info.progressPercent);
    ui->progressBar->setValue(percent > 100 ? 100 : percent);
    ui->progressPercentLabel->setText(
        QString("%1%").arg(info.progressPercent, 0, 'f', 1));

    // 使用服务层的统一状态消息
    QString statusText = BudgetService::statusMessage(info.status);
    QString progressColor;
    QString barColor;
    switch (info.status) {
    case BudgetStatus::Exceeded:
        progressColor = "color: red; font-weight: bold;";
        barColor = "QProgressBar::chunk { background-color: red; }";
        ui->statusLabel->setStyleSheet("color: red; font-weight: bold; font-size: 16px;");
        ui->budgetSpinBox->setValue(info.monthlyBudget);
        break;
    case BudgetStatus::Warning:
        progressColor = "color: orange; font-weight: bold;";
        barColor = "QProgressBar::chunk { background-color: orange; }";
        ui->statusLabel->setStyleSheet("color: orange; font-weight: bold; font-size: 16px;");
        break;
    default:
        progressColor = "color: green;";
        barColor = "QProgressBar::chunk { background-color: #4CAF50; }";
        ui->statusLabel->setStyleSheet("color: green; font-weight: bold; font-size: 16px;");
        break;
    }

    ui->statusLabel->setText(statusText);
    ui->progressPercentLabel->setStyleSheet(progressColor);
    ui->progressBar->setStyleSheet(barColor);

    // 剩余额度颜色
    if (info.remaining < 0)
        ui->remainingLabel->setStyleSheet("color: red; font-weight: bold;");
    else
        ui->remainingLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");
}
