#include "categoryservice.h"
#include "../database/categorydao.h"

bool CategoryService::addCategory(const Category &cat) { CategoryDao dao; return dao.insertCategory(cat); }
bool CategoryService::updateCategory(const Category &cat) { CategoryDao dao; return dao.updateCategory(cat); }
bool CategoryService::deleteCategory(int id) { CategoryDao dao; return dao.deleteCategory(id); }
QList<Category> CategoryService::getUserCategories(int userId) { CategoryDao dao; return dao.getCategoriesByUser(userId); }
QList<Category> CategoryService::getCategoriesByType(int userId, const QString &type) { CategoryDao dao; return dao.getCategoriesByType(userId, type); }
