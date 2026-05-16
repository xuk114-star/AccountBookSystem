#ifndef CATEGORYPAGE_H
#define CATEGORYPAGE_H

#include <QWidget>

namespace Ui { class CategoryPage; }

class CategoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit CategoryPage(int userId, QWidget *parent = nullptr);
    ~CategoryPage();

private slots:
    void onAddClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onListSelectionChanged();
    void refreshList();

private:
    Ui::CategoryPage *ui;
    int m_userId;
    int m_editingCategoryId = -1;
};

#endif // CATEGORYPAGE_H
