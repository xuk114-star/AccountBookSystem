QT       += core gui widgets charts sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Suppress __yield warning from Qt 6.11 + macOS 26 SDK compatibility
QMAKE_CXXFLAGS += -Wno-error=implicit-function-declaration

TARGET = AccountBookSystem
TEMPLATE = app

SOURCES += \
    main.cpp \
    database/databasemanager.cpp \
    database/userdao.cpp \
    database/billdao.cpp \
    database/categorydao.cpp \
    database/budgetdao.cpp \
    service/userservice.cpp \
    service/billservice.cpp \
    service/categoryservice.cpp \
    service/statisticsservice.cpp \
    service/budgetservice.cpp \
    ui/loginwindow.cpp \
    ui/mainwindow.cpp \
    ui/billform.cpp \
    ui/categorypage.cpp \
    ui/statisticspage.cpp \
    ui/budgetpage.cpp \
    ui/settingspage.cpp \
    theme/thememanager.cpp

HEADERS += \
    model/user.h \
    model/bill.h \
    model/category.h \
    model/budget.h \
    database/databasemanager.h \
    database/userdao.h \
    database/billdao.h \
    database/categorydao.h \
    database/budgetdao.h \
    service/userservice.h \
    service/billservice.h \
    service/categoryservice.h \
    service/statisticsservice.h \
    service/budgetservice.h \
    ui/loginwindow.h \
    ui/mainwindow.h \
    ui/billform.h \
    ui/categorypage.h \
    ui/statisticspage.h \
    ui/budgetpage.h \
    ui/settingspage.h \
    theme/thememanager.h

FORMS += \
    ui/loginwindow.ui \
    ui/mainwindow.ui \
    ui/billform.ui \
    ui/categorypage.ui \
    ui/statisticspage.ui \
    ui/budgetpage.ui \
    ui/settingspage.ui

RESOURCES += \
    resources/resources.qrc
