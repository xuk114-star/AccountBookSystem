#ifndef BILLFORM_H
#define BILLFORM_H

#include <QWidget>
#include "../model/bill.h"

namespace Ui { class BillForm; }

class BillForm : public QWidget
{
    Q_OBJECT

public:
    explicit BillForm(int userId, QWidget *parent = nullptr);
    ~BillForm();

private slots:
    void onTypeChanged(int index);
    void onAddClicked();
    void onDeleteClicked();
    void onQueryClicked();
    void onTableRowSelected();
    void onFilterTypeChanged(int index);
    void refreshTable();

private:
    void loadCategories();
    void loadFilterCategories();
    void clearForm();

    Ui::BillForm *ui;
    int m_userId;
    int m_editingBillId = -1;
};

#endif // BILLFORM_H
