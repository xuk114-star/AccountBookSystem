#include "settingspage.h"
#include "ui_settingspage.h"
#include "../theme/thememanager.h"

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::SettingsPage)
{
    ui->setupUi(this);
    ui->themeCombo->addItems({"浅色主题", "深色主题"});
    connect(ui->themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsPage::onThemeChanged);

    // 加载已保存的主题
    QString saved = ThemeManager::instance().currentTheme();
    ui->themeCombo->setCurrentIndex(saved == "dark" ? 1 : 0);
}

SettingsPage::~SettingsPage() { delete ui; }

void SettingsPage::onThemeChanged(int index)
{
    QString theme = (index == 1) ? "dark" : "light";
    ThemeManager::instance().applyTheme(theme);
}
