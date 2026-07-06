# 个人记账与消费分析系统

基于 **Qt 6.11 + SQLite** 开发的桌面端个人记账应用，支持账单管理、分类管理、消费统计分析（饼图/柱状图/折线图）、预算提醒、深浅主题切换。

## 功能模块

| 模块 | 功能 |
|------|------|
| 用户管理 | 注册、登录、退出登录 |
| 账单管理 | 添加/修改/删除账单，按类型/分类/关键词组合查询 |
| 分类管理 | 添加/修改/删除收支分类，内置 11 个默认分类 |
| 消费统计 | 收支概览、分类支出饼图、月度收支柱状图、收支趋势折线图 |
| 预算提醒 | 设置月度预算，进度条展示，超 80% 提醒，超 100% 警告 |
| 主题切换 | 浅色/深色主题，设置持久化保存 |

## 技术栈

| 项 | 内容 |
|----|------|
| 语言 | C++17 |
| 框架 | Qt 6.11 |
| 界面 | Qt Widgets (.ui) |
| 图表 | Qt Charts |
| 数据库 | SQLite (Qt SQL) |
| 构建 | qmake + make |

## 项目结构

```
AccountBookSystem/
├── main.cpp                    # 程序入口
├── AccountBookSystem.pro       # Qt 项目配置
├── model/                      # 数据模型
│   ├── user.h                  # 用户
│   ├── bill.h                  # 账单
│   ├── category.h              # 分类
│   └── budget.h                # 预算
├── database/                   # 数据库层
│   ├── databasemanager.h/.cpp  # SQLite 连接管理（单例）
│   ├── userdao.h/.cpp          # 用户数据操作
│   ├── billdao.h/.cpp          # 账单数据操作
│   ├── categorydao.h/.cpp      # 分类数据操作
│   └── budgetdao.h/.cpp        # 预算数据操作
├── service/                    # 业务逻辑层
│   ├── userservice.h/.cpp      # 登录注册
│   ├── billservice.h/.cpp      # 账单管理
│   ├── categoryservice.h/.cpp  # 分类管理
│   ├── statisticsservice.h/.cpp# 统计分析
│   └── budgetservice.h/.cpp    # 预算提醒
├── ui/                         # 界面层
│   ├── loginwindow.h/.cpp/.ui  # 登录窗口
│   ├── mainwindow.h/.cpp/.ui   # 主窗口
│   ├── billform.h/.cpp/.ui     # 账单管理页
│   ├── categorypage.h/.cpp/.ui # 分类管理页
│   ├── statisticspage.h/.cpp/.ui# 统计分析页
│   ├── budgetpage.h/.cpp/.ui   # 预算提醒页
│   └── settingspage.h/.cpp/.ui # 系统设置页
├── theme/                      # 主题系统
│   ├── thememanager.h/.cpp     # 主题管理器（单例）
│   ├── light.qss               # 浅色主题
│   └── dark.qss                # 深色主题
└── resources/
    └── resources.qrc           # Qt 资源文件
```

## 编译运行

### 环境要求

- Qt 6.x（需包含 Charts、SQL 模块）
- macOS / Windows / Linux
- C++17 编译器

### 构建

```bash
# 生成 Makefile
qmake AccountBookSystem.pro

# 编译
make

# 运行
open AccountBookSystem.app   # macOS
```

或在 Qt Creator 中打开 `AccountBookSystem.pro` 直接构建。

## 数据库 ER 图

```
users               categories          bills               budgets
──────              ──────────          ─────               ───────
id (PK)       ──┬── id (PK)             id (PK)             id (PK)
username         ├── user_id (FK) ────── user_id (FK)        user_id (FK, UNIQUE)
password         │   name               type                 monthly_budget
                 │   type               category_id (FK) ────┘
                 └────────────────────── amount
                                         date
                                         note
```

默认分类（user_id=0）全局共享，用户自定义分类与默认分类合并显示。
## 程序展示录屏

链接:https://disk.pku.edu.cn/link/AA532F115AECA046BF91C5AF7FCBAE4F8B
文件名：38_1780757439.mp4
有效期限：永久有效
提取码：Df2D
