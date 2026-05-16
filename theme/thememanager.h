#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QString>

class ThemeManager
{
public:
    static ThemeManager& instance();

    void applyTheme(const QString &themeName); // "light" or "dark"
    QString currentTheme() const;

private:
    ThemeManager();
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    QString m_currentTheme;
};

#endif // THEMEMANAGER_H
