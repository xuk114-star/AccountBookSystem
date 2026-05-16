#ifndef CATEGORYDAO_H
#define CATEGORYDAO_H

#include "../model/category.h"
#include <QList>
#include <QString>

class CategoryDao
{
public:
    CategoryDao() = default;

    bool insertCategory(const Category &cat);
    bool updateCategory(const Category &cat);
    bool deleteCategory(int id);
    QList<Category> getCategoriesByUser(int userId);
    QList<Category> getCategoriesByType(int userId, const QString &type);
    Category getCategoryById(int id);
    QList<Category> getDefaultCategories();
};

#endif // CATEGORYDAO_H
