#ifndef CATEGORYSERVICE_H
#define CATEGORYSERVICE_H

#include "../model/category.h"
#include <QList>
#include <QString>

class CategoryService
{
public:
    CategoryService() = default;

    bool addCategory(const Category &cat);
    bool updateCategory(const Category &cat);
    bool deleteCategory(int id);
    QList<Category> getUserCategories(int userId);
    QList<Category> getCategoriesByType(int userId, const QString &type);
};

#endif // CATEGORYSERVICE_H
