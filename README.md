# Student Management System (Modern Qt Edition) 🎓

这是一个基于 **Qt (C++)** 开发的现代化学生管理系统，支持本地二进制/TXT 存储以及 **MySQL/MariaDB** 数据库同步。项目在原有基础上进行了深度 UI 美化，采用了扁平化、现代化的视觉风格。

## ✨ 特性

- **现代化 UI**：全面应用 QSS 样式表，支持圆角交互、动态悬停效果及专业配色。
- **多后端同步**：
  - 支持 **MySQL/MariaDB** 远程/本地数据库同步。
  - 支持 TXT 文档便捷导入。
  - 支持二进制文件 (`.dat`) 高效备份与还原。
- **完整 CRUD**：提供学生信息的添加、查询、删除及成绩等级自动计算 (A/B/C/D)。
- **统计概览**：一键生成及格率、优良率及分数段分布统计。

## 📸 界面预览
*(美化后的界面采用现代蓝配色，优化了控件间距与比例)*

## 🛠️ 环境要求

- **Qt 6.x** (建议使用 Qt 6.5+)
- **CMake 3.16+**
- **MySQL Connector/C++** (用于数据库连接)
- **MySQL/MariaDB** 数据库服务

## 🚀 快速开始

### 1. 克隆项目
```bash
git clone https://github.com/your-username/some-practices.git
cd some-practices/StudentManagementSystem-Modern
```

### 2. 数据库配置
在运行程序后，点击 **"同步到 MySQL"**，在弹出的配置窗口中输入：
- **Host**: 数据库地址 (如 `localhost`)
- **Database**: 数据库名
- **User/Password**: 你的数据库凭据
- **Port**: 默认 `3306`

> [!TIP]
> 程序会自动为你创建 `students` 数据表，无需手动执行 SQL。

### 3. 构建与运行
1. 使用 **Qt Creator** 打开 `CMakeLists.txt`。
2. 配置项目并点击 **"运行"**。
3. 确保 `style.qss` 与可执行文件处于同一目录下，以加载美化样式。

## 📁 目录结构

- `StudentDialog.h/cpp`: 主逻辑与 UI 初始化。
- `style.qss`: 核心 UI 样式定义。
- `main.cpp`: 程序入口。
- `CMakeLists.txt`: 构建配置文件。

## 🤝 贡献
欢迎提交 Issue 或 Pull Request 来完善这个项目。

---
*Powered by Qt & C++*
