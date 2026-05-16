#include "categorypage.h"
#include "ui_categorypage.h"
#include "../service/categoryservice.h"
#include <QMessageBox>
#include <QListWidget>

CategoryPage::CategoryPage(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::CategoryPage), m_userId(userId)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &CategoryPage::onAddClicked);
    connect(ui->updateButton, &QPushButton::clicked, this, &CategoryPage::onUpdateClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &CategoryPage::onDeleteClicked);
    connect(ui->categoryList, &QListWidget::currentItemChanged,
            this, &CategoryPage::onListSelectionChanged);
    ui->typeCombo->addItems({"支出", "收入"});
    refreshList();
}

CategoryPage::~CategoryPage() { delete ui; }

void CategoryPage::onListSelectionChanged()
{
    auto *item = ui->categoryList->currentItem();
    if (!item) return;
    int id = item->data(Qt::UserRole).toInt();
    CategoryService svc;
    auto cats = svc.getUserCategories(m_userId);
    for (const auto &c : cats) {
        if (c.id == id) {
            m_editingCategoryId = c.id;
            ui->typeCombo->setCurrentIndex(c.type == "expense" ? 0 : 1);
            ui->nameEdit->setText(c.name);
            return;
        }
    }
}

void CategoryPage::onAddClicked()
{
    QString name = ui->nameEdit->text().trimmed();
    if (name.isEmpty()) return;

    Category cat;
    cat.userId = m_userId;
    cat.name = name;
    cat.type = ui->typeCombo->currentIndex() == 0 ? "expense" : "income";

    CategoryService svc;
    svc.addCategory(cat);
    ui->nameEdit->clear();
    m_editingCategoryId = -1;
    ui->addButton->setText("添加");
    refreshList();
}

void CategoryPage::onUpdateClicked()
{
    if (m_editingCategoryId <= 0) {
        QMessageBox::information(this, "提示", "请先在列表中选择一个分类");
        return;
    }
    QString name = ui->nameEdit->text().trimmed();
    if (name.isEmpty()) return;

    Category cat;
    cat.id = m_editingCategoryId;
    cat.userId = m_userId;
    cat.name = name;
    cat.type = ui->typeCombo->currentIndex() == 0 ? "expense" : "income";

    CategoryService svc;
    svc.updateCategory(cat);
    ui->nameEdit->clear();
    m_editingCategoryId = -1;
    ui->addButton->setText("添加");
    refreshList();
}

void CategoryPage::onDeleteClicked()
{
    auto *item = ui->categoryList->currentItem();
    if (!item) return;
    int id = item->data(Qt::UserRole).toInt();
    if (QMessageBox::question(this, "确认", "确定删除该分类吗？") == QMessageBox::Yes) {
        CategoryService svc;
        svc.deleteCategory(id);
        m_editingCategoryId = -1;
        ui->nameEdit->clear();
        refreshList();
    }
}

void CategoryPage::refreshList()
{
    CategoryService svc;
    auto cats = svc.getUserCategories(m_userId);
    ui->categoryList->clear();
    for (const auto &c : cats) {
        QString label = QString("[%1] %2").arg(c.type == "expense" ? "支出" : "收入", c.name);
        auto *item = new QListWidgetItem(label);
        item->setData(Qt::UserRole, c.id);
        ui->categoryList->addItem(item);
    }
}
