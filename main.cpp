#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include "database/databasemanager.h"
#include "theme/thememanager.h"
#include "ui/loginwindow.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("AccountBookSystem");

    // 数据库路径：放在用户数据目录下（不受应用重装影响）
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);
    QString dbPath = dataDir + "/accountbook.db";

    if (!DatabaseManager::instance().initialize(dbPath)) {
        qCritical("Failed to initialize database.");
        return 1;
    }

    // 加载已保存的主题
    ThemeManager::instance().applyTheme(ThemeManager::instance().currentTheme());

    auto *loginWindow = new LoginWindow();
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();

    QObject::connect(loginWindow, &LoginWindow::loginSuccess, [&](int userId) {
        auto *mainWindow = new MainWindow(userId);
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);

        // 主窗口关闭后重新显示登录窗口
        QObject::connect(mainWindow, &MainWindow::loggedOut, [=]() {
            auto *newLogin = new LoginWindow();
            newLogin->setAttribute(Qt::WA_DeleteOnClose);
            newLogin->show();
            QObject::connect(newLogin, &LoginWindow::loginSuccess, [&](int uid) {
                auto *mw = new MainWindow(uid);
                mw->setAttribute(Qt::WA_DeleteOnClose);
                mw->show();
            });
        });

        mainWindow->show();
        loginWindow->close();
    });

    return app.exec();
}
