#include "billform.h"
#include "ui_billform.h"
#include "../service/billservice.h"
#include "../service/categoryservice.h"
#include <QMessageBox>
#include <QTableWidget>

BillForm::BillForm(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::BillForm), m_userId(userId)
{
    ui->setupUi(this);

    connect(ui->typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BillForm::onTypeChanged);
    connect(ui->filterTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BillForm::onFilterTypeChanged);
    connect(ui->addButton, &QPushButton::clicked, this, &BillForm::onAddClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &BillForm::onDeleteClicked);
    connect(ui->queryButton, &QPushButton::clicked, this, &BillForm::onQueryClicked);
    connect(ui->billTable, &QTableWidget::cellClicked, this, &BillForm::onTableRowSelected);

    ui->typeCombo->addItems({"支出", "收入"});
    ui->filterTypeCombo->addItems({"全部类型", "支出", "收入"});
    loadCategories();
    loadFilterCategories();
    refreshTable();
}

BillForm::~BillForm() { delete ui; }

void BillForm::onTypeChanged(int) { loadCategories(); }

void BillForm::onFilterTypeChanged(int) { loadFilterCategories(); }

void BillForm::loadCategories()
{
    ui->categoryCombo->clear();
    CategoryService svc;
    QString type = ui->typeCombo->currentIndex() == 0 ? "expense" : "income";
    auto cats = svc.getCategoriesByType(m_userId, type);
    for (const auto &c : cats)
        ui->categoryCombo->addItem(c.name, c.id);
}

void BillForm::loadFilterCategories()
{
    ui->filterCategoryCombo->clear();
    ui->filterCategoryCombo->addItem("全部分类", -1);

    CategoryService svc;
    int filterIdx = ui->filterTypeCombo->currentIndex();
    if (filterIdx == 0) {
        auto cats = svc.getUserCategories(m_userId);
        for (const auto &c : cats)
            ui->filterCategoryCombo->addItem(
                QString("[%1] %2").arg(c.type == "expense" ? "支出" : "收入", c.name), c.id);
    } else {
        QString type = filterIdx == 1 ? "expense" : "income";
        auto cats = svc.getCategoriesByType(m_userId, type);
        for (const auto &c : cats)
            ui->filterCategoryCombo->addItem(c.name, c.id);
    }
}

void BillForm::onAddClicked()
{
    double amount = ui->amountSpin->value();
    if (amount <= 0) {
        QMessageBox::warning(this, "输入错误", "金额必须大于零");
        return;
    }
    if (ui->categoryCombo->count() == 0) {
        QMessageBox::warning(this, "输入错误", "请先在分类管理中创建分类");
        return;
    }

    Bill bill;
    bill.userId = m_userId;
    bill.type = ui->typeCombo->currentIndex() == 0 ? "expense" : "income";
    bill.categoryId = ui->categoryCombo->currentData().toInt();
    bill.amount = amount;
    bill.date = ui->dateEdit->date().toString("yyyy-MM-dd");
    bill.note = ui->noteEdit->text().trimmed();

    BillService svc;
    if (m_editingBillId > 0) {
        bill.id = m_editingBillId;
        svc.updateBill(bill);
        m_editingBillId = -1;
        ui->addButton->setText("添加账单");
    } else {
        svc.addBill(bill);
    }
    clearForm();
    refreshTable();
}

void BillForm::onDeleteClicked()
{
    int row = ui->billTable->currentRow();
    if (row < 0) return;
    int id = ui->billTable->item(row, 0)->data(Qt::UserRole).toInt();
    if (QMessageBox::question(this, "确认", "确定删除该账单吗？") == QMessageBox::Yes) {
        BillService svc;
        svc.deleteBill(id);
        refreshTable();
    }
}

void BillForm::onQueryClicked()
{
    refreshTable();
}

void BillForm::onTableRowSelected()
{
    int row = ui->billTable->currentRow();
    if (row < 0) return;
    int id = ui->billTable->item(row, 0)->data(Qt::UserRole).toInt();
    BillService svc;
    Bill b = svc.getBill(id);
    ui->typeCombo->setCurrentIndex(b.type == "expense" ? 0 : 1);
    loadCategories();
    int idx = ui->categoryCombo->findData(b.categoryId);
    if (idx >= 0) ui->categoryCombo->setCurrentIndex(idx);
    ui->amountSpin->setValue(b.amount);
    ui->dateEdit->setDate(QDate::fromString(b.date, "yyyy-MM-dd"));
    ui->noteEdit->setText(b.note);
    m_editingBillId = b.id;
    ui->addButton->setText("更新账单");
}

void BillForm::refreshTable()
{
    BillService svc;
    int catId = ui->filterCategoryCombo->currentData().toInt();
    auto bills = svc.queryBills(m_userId, "", "", "", catId, -1, -1, ui->filterKeyword->text().trimmed());

    auto *t = ui->billTable;
    t->setRowCount(bills.size());
    t->setColumnCount(6);
    t->setHorizontalHeaderLabels({"ID", "类型", "分类", "金额", "日期", "备注"});

    for (int i = 0; i < bills.size(); ++i) {
        const auto &b = bills[i];
        auto *idItem = new QTableWidgetItem(QString::number(b.id));
        idItem->setData(Qt::UserRole, b.id);
        t->setItem(i, 0, idItem);
        t->setItem(i, 1, new QTableWidgetItem(b.type == "expense" ? "支出" : "收入"));
        t->setItem(i, 2, new QTableWidgetItem(b.categoryName));
        t->setItem(i, 3, new QTableWidgetItem(QString::number(b.amount, 'f', 2)));
        t->setItem(i, 4, new QTableWidgetItem(b.date));
        t->setItem(i, 5, new QTableWidgetItem(b.note));
    }
}

void BillForm::clearForm()
{
    ui->typeCombo->setCurrentIndex(0);
    ui->amountSpin->setValue(0);
    ui->noteEdit->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    m_editingBillId = -1;
    ui->addButton->setText("添加账单");
    loadCategories();
}
