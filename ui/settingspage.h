#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

namespace Ui { class SettingsPage; }

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

private slots:
    void onThemeChanged(int index);

private:
    Ui::SettingsPage *ui;
};

#endif // SETTINGSPAGE_H
