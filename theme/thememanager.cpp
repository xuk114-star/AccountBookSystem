#include "thememanager.h"
#include <QApplication>
#include <QFile>
#include <QSettings>

ThemeManager& ThemeManager::instance()
{
    static ThemeManager inst;
    return inst;
}

ThemeManager::ThemeManager()
    : m_currentTheme("light")
{
    QSettings settings("AccountBook", "AccountBookSystem");
    m_currentTheme = settings.value("theme", "light").toString();
}

void ThemeManager::applyTheme(const QString &themeName)
{
    m_currentTheme = themeName;

    QFile file(QString(":/%1.qss").arg(themeName));
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        qApp->setStyleSheet(QString::fromUtf8(file.readAll()));
    }

    QSettings settings("AccountBook", "AccountBookSystem");
    settings.setValue("theme", themeName);
}

QString ThemeManager::currentTheme() const
{
    return m_currentTheme;
}
