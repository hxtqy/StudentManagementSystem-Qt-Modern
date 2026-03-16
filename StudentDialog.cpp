#include "StudentDialog.h"

// MySQL配置弹窗实现
MySQLConfigDialog::MySQLConfigDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("MySQL数据库配置");
    setFixedSize(350, 300);
    setModal(true); // 模态窗口，阻止操作主窗口

    // 创建控件
    lbHost = new QLabel("主机地址：");
    lbDbName = new QLabel("数据库名：");
    lbUser = new QLabel("用户名：");
    lbPwd = new QLabel("密码：");
    lbPort = new QLabel("端口：");

    leHost = new QLineEdit("localhost"); // 默认本地主机
    leDbName = new QLineEdit("studentmanagement"); // 默认数据库名
    leUser = new QLineEdit("root"); // 默认用户名
    lePwd = new QLineEdit();
    lePwd->setEchoMode(QLineEdit::Password); // 密码隐藏输入
    lePort = new QLineEdit("  "); // 默认MySQL端口

    btnOk = new QPushButton("确认");
    btnCancel = new QPushButton("取消");

    // 布局
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(lbHost, 0, 0, 1, 1);
    gridLayout->addWidget(leHost, 0, 1, 1, 2);
    gridLayout->addWidget(lbDbName, 1, 0, 1, 1);
    gridLayout->addWidget(leDbName, 1, 1, 1, 2);
    gridLayout->addWidget(lbUser, 2, 0, 1, 1);
    gridLayout->addWidget(leUser, 2, 1, 1, 2);
    gridLayout->addWidget(lbPwd, 3, 0, 1, 1);
    gridLayout->addWidget(lePwd, 3, 1, 1, 2);
    gridLayout->addWidget(lbPort, 4, 0, 1, 1);
    gridLayout->addWidget(lePort, 4, 1, 1, 2);
    gridLayout->setSpacing(15);
    gridLayout->setContentsMargins(20, 20, 20, 10);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);
    btnLayout->setSpacing(20);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(btnLayout);

    // 绑定信号槽
    connect(btnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

// 主窗口 StudentDialog 实现
StudentDialog::StudentDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("学生管理系统（支持MySQL持久化）");
    setFixedSize(750, 600); // 略微调大一点以便更好的展示表格
    initUI(); // 初始化界面
    loadStyleSheet(); // 加载美化样式
}

StudentDialog::~StudentDialog()
{
    // 析构时关闭数据库连接
    if (db.isOpen()) {
        db.close();
        qDebug() << "MySQL连接已关闭";
    }
}

// 初始化界面
void StudentDialog::initUI()
{
    // 1. 创建所有控件
    lbId = new QLabel("学号：");
    lbName = new QLabel("姓名：");
    lbScore = new QLabel("成绩：");
    leId = new QLineEdit();
    leName = new QLineEdit();
    leScore = new QLineEdit();
    btnAdd = new QPushButton("添加学生");
    btnQuery = new QPushButton("快速查询");
    btnDelete = new QPushButton("删除学生记录");
    btnStat = new QPushButton("统计概览");
    btnLoadTxt = new QPushButton("导入 TXT");
    btnSaveBinary = new QPushButton("备份为二进制");
    btnLoadBinary = new QPushButton("还原自二进制");
    btnSaveToMySQL = new QPushButton("同步到 MySQL");
    btnLoadFromMySQL = new QPushButton("从 MySQL 同步");

    // 为 QSS 匹配设置对象名
    btnAdd->setObjectName("btnAdd");
    btnDelete->setObjectName("btnDelete");

    table = new QTableWidget();

    // 2. 设置列表控件
    table->setColumnCount(4);
    QStringList headers;
    headers << "ID" << "姓名" << "成绩" << "等级";
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);

    // 3. 布局管理
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(lbId);
    leftLayout->addWidget(leId);
    leftLayout->addWidget(lbName);
    leftLayout->addWidget(leName);
    leftLayout->addWidget(lbScore);
    leftLayout->addWidget(leScore);
    leftLayout->setSpacing(15);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(btnAdd);
    rightLayout->addWidget(btnQuery);
    rightLayout->addWidget(btnDelete);

    rightLayout->addWidget(btnStat);
    rightLayout->addWidget(btnLoadTxt);
    rightLayout->addWidget(btnSaveBinary);
    rightLayout->addWidget(btnLoadBinary);

    rightLayout->addWidget(btnSaveToMySQL);
    rightLayout->addWidget(btnLoadFromMySQL);
    rightLayout->setSpacing(10);
    rightLayout->setAlignment(Qt::AlignTop);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addLayout(leftLayout);
    inputLayout->addLayout(rightLayout);
    inputLayout->setContentsMargins(20, 20, 20, 10);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(table);
    mainLayout->setContentsMargins(20, 20, 20, 20); // 增大边距，视觉更呼吸
    mainLayout->setSpacing(20);

    // 4. 信号槽绑定
    connect(btnAdd, &QPushButton::clicked, this, &StudentDialog::onAddStudentClicked);
    connect(btnQuery, &QPushButton::clicked, this, &StudentDialog::onQueryStudentClicked);
    connect(btnDelete, &QPushButton::clicked, this, &StudentDialog::onDeleteStudentClicked);
    connect(btnStat, &QPushButton::clicked, this, &StudentDialog::onStatClicked);
    connect(btnLoadTxt, &QPushButton::clicked, this, &StudentDialog::onLoadTxtClicked);
    connect(btnSaveBinary, &QPushButton::clicked, this, &StudentDialog::onSaveBinaryClicked);
    connect(btnLoadBinary, &QPushButton::clicked, this, &StudentDialog::onLoadBinaryClicked);
    connect(btnSaveToMySQL, &QPushButton::clicked, this, &StudentDialog::onSaveToMySQLClicked);
    connect(btnLoadFromMySQL, &QPushButton::clicked, this, &StudentDialog::onLoadFromMySQLClicked);
}

// 显示MySQL配置弹窗
void StudentDialog::showMySQLConfigDialog(QString &host, QString &dbName, QString &user, QString &pwd, int &port)
{
    MySQLConfigDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        host = dialog.getHost();
        dbName = dialog.getDbName();
        user = dialog.getUser();
        pwd = dialog.getPwd();
        port = dialog.getPort();

        // 校验配置
        if (host.isEmpty() || dbName.isEmpty() || user.isEmpty() || port <= 0) {
            QMessageBox::warning(this, "配置错误", "主机地址、数据库名、用户名、端口不能为空！");
            host.clear(); // 标记配置无效
        }
    }
}

// 初始化MySQL连接
bool StudentDialog::initMySQLConnection(const QString &host, const QString &dbName, const QString &user, const QString &pwd, int port)
{
    // 关闭已有连接
    if (db.isOpen()) {
        db.close();
    }

    // 初始化连接
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(pwd);
    db.setPort(port);

    // 尝试连接
    if (db.open()) {
        qDebug() << "MySQL连接成功！";
        createStudentTable(); // 连接成功后创建表
        return true;
    } else {
        qDebug() << "MySQL连接失败：" << db.lastError().text();
        QMessageBox::warning(this, "MySQL连接失败",
                             "连接失败原因：\n" + db.lastError().text() +
                                 "\n\n请检查：\n1. MySQL服务是否启动\n2. 配置信息是否正确\n3. 数据库是否存在");
        return false;
    }
}

// 创建学生表（如果不存在则创建）
void StudentDialog::createStudentTable()
{
    QSqlQuery query;
    QString createSql = R"(
        CREATE TABLE IF NOT EXISTS students (
            id INT PRIMARY KEY,
            name VARCHAR(50) NOT NULL,
            score FLOAT NOT NULL,
            grade CHAR(1) NOT NULL
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
    )";

    if (!query.exec(createSql)) {
        qDebug() << "创建学生表失败：" << query.lastError().text();
        QMessageBox::warning(this, "表创建失败", "创建学生表失败：\n" + query.lastError().text());
    }
}

// 保存到MySQL（弹出配置弹窗）
void StudentDialog::onSaveToMySQLClicked()
{
    if (students.empty()) {
        QMessageBox::warning(this, "无数据", "本地没有学生数据可保存！");
        return;
    }

    // 1. 弹出配置弹窗，获取用户输入
    QString host, dbName, user, pwd;
    int port;
    showMySQLConfigDialog(host, dbName, user, pwd, port);
    if (host.isEmpty()) { // 检验配置格式
        return;
    }

    // 2. 连接MySQL
    if (!initMySQLConnection(host, dbName, user, pwd, port)) {
        return;
    }

    // 3. 确认是否覆盖数据库数据
    int ret = QMessageBox::question(this, "确认保存",
                                    "是否将本地所有学生数据保存到MySQL？\n（会覆盖数据库中已有的学生记录）",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    // 4. 批量刷新数据（先清空表）
    QSqlQuery query;
    query.exec("TRUNCATE TABLE students"); // 清空表（保留表结构）
    bool allSuccess = true;

    for (const auto &stu : students) {
        query.prepare("INSERT INTO students (id, name, score, grade) "
                      "VALUES (:id, :name, :score, :grade)");
        query.bindValue(":id", stu.id);
        query.bindValue(":name", stu.name);
        query.bindValue(":score", stu.score);
        query.bindValue(":grade", stu.getGrade());

        if (!query.exec()) {
            qDebug() << "插入数据失败（ID：" << stu.id << "）：" << query.lastError().text();
            allSuccess = false;
        }
    }

    // 5. 结果提示
    if (allSuccess) {
        QMessageBox::information(this, "保存成功", "所有本地数据已成功保存到MySQL！");
    } else {
        QMessageBox::warning(this, "保存不完全", "部分数据保存失败，请查看调试信息！");
    }
}

// 从MySQL加载（弹出配置弹窗）
void StudentDialog::onLoadFromMySQLClicked()
{
    // 1. 弹出配置弹窗，获取用户输入
    QString host, dbName, user, pwd;
    int port;
    showMySQLConfigDialog(host, dbName, user, pwd, port);
    if (host.isEmpty()) { // 配置无效
        return;
    }

    // 2. 连接MySQL
    if (!initMySQLConnection(host, dbName, user, pwd, port)) {
        return;
    }

    // 3. 确认是否覆盖本地数据
    int ret = QMessageBox::question(this, "确认加载",
                                    "是否从MySQL加载数据？\n（会覆盖本地已有的学生记录）",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    // 4. 查询数据库数据
    QSqlQuery query("SELECT id, name, score, grade FROM students");
    students.clear(); // 清空本地数据
    bool hasData = false;

    while (query.next()) {
        hasData = true;
        Student stu;
        stu.id = query.value("id").toInt();
        stu.name = query.value("name").toString();
        stu.score = query.value("score").toFloat();
        students.push_back(stu);
    }

    // 5. 刷新列表+提示
    refreshTable();
    if (hasData) {
        QMessageBox::information(this, "加载成功", "已从MySQL加载所有学生数据！");
    } else {
        QMessageBox::information(this, "无数据", "MySQL数据库中没有学生记录！");
    }
}

// 新增学生（同步到MySQL）
void StudentDialog::onAddStudentClicked()
{
    // 原有逻辑不变（输入校验、本地数据保存）
    QString strId = leId->text().trimmed();
    QString strName = leName->text().trimmed();
    QString strScore = leScore->text().trimmed();

    if (strId.isEmpty() || strName.isEmpty() || strScore.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整学生信息！");
        return;
    }

    int id = strId.toInt();
    float score = strScore.toFloat();
    if (id <= 0) {
        QMessageBox::warning(this, "警告", "学号必须是正整数！");
        return;
    }
    if (score < 0 || score > 100) {
        QMessageBox::warning(this, "警告", "成绩必须在0-100之间！");
        return;
    }
    Student tempstu;
    tempstu.id=id;

    for (const auto &stu : students) {
        if (stu == tempstu) {
            QMessageBox::warning(this, "警告", "学号已存在！");
            return;
        }
    }

    Student stu;
    stu.id = id;
    stu.name = strName;
    stu.score = score;
    students.push_back(stu);

    // 新增：同步到MySQL（如果连接正常）
    if (db.isOpen()) {
        QSqlQuery query;
        query.prepare("INSERT INTO students (id, name, score, grade) "
                      "VALUES (:id, :name, :score, :grade)");
        query.bindValue(":id", id);
        query.bindValue(":name", strName);
        query.bindValue(":score", score);
        query.bindValue(":grade", stu.getGrade());

        if (!query.exec()) {
            qDebug() << "MySQL插入失败：" << query.lastError().text();
            QMessageBox::warning(this, "MySQL同步失败", "本地添加成功，但同步到MySQL失败：\n" + query.lastError().text());
        }
    }

    refreshTable();
    leId->clear();
    leName->clear();
    leScore->clear();
    QMessageBox::information(this, "成功", "添加学生成功！");
}

// 删除学生（同步到MySQL）
void StudentDialog::onDeleteStudentClicked()
{
    // 1. 输入校验（
    QString strId = leId->text().trimmed();

    // 空值校验
    if (strId.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入要删除的学号！");
        return;
    }

    //格式检验
    int id = strId.toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "警告", "学号必须是正整数！");
        return;
    }

    // 2. 校验学号是否存在
    Student tempStu;
    tempStu.id = id;
    bool isExist = false;
    auto targetIt = students.end();

    // 遍历本地列表查找
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->id == tempStu.id) {
            isExist = true;
            targetIt = it;
            break;
        }
    }

    // 学号不存在提示
    if (!isExist) {
        QMessageBox::warning(this, "警告", "该学号不存在，无法删除！");
        return;
    }

    // 3. 确认删除
    int ret = QMessageBox::question(this, "确认",
                                    QString("确定要删除学号为%1的学生吗？\n（本地和MySQL中记录将同步删除）").arg(id),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    // 4. 本地数据删除
    students.erase(targetIt);

    // 5. 同步到MySQL
    if (db.isOpen()) {
        QSqlQuery query;
        query.prepare("DELETE FROM students WHERE id = :id"); // 类比添加的INSERT语句
        query.bindValue(":id", id);

        if (!query.exec()) {
            qDebug() << "MySQL删除失败：" << query.lastError().text();
            QMessageBox::warning(this, "MySQL同步失败", "本地删除成功，但同步到MySQL失败：\n" + query.lastError().text());
        }
    }

    // 刷新界面
    refreshTable();
    leId->clear(); // 只清空学号输入框
    QMessageBox::information(this, "成功", "删除学生成功！");
}
//刷新表格
void StudentDialog::refreshTable()
{
    table->setRowCount(0);
    for (const auto &stu : students) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::number(stu.id)));
        table->setItem(row, 1, new QTableWidgetItem(stu.name));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(stu.score, 'f', 1)));
        table->setItem(row, 3, new QTableWidgetItem(stu.getGrade()));
        qDebug() << "刷新表格成功" ;
    }
}
//根据学号进行学生信息查询
void StudentDialog::onQueryStudentClicked()
{
    QString strId = leId->text().trimmed();
    if (strId.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入要查询的学号！");
        return;
    }

    int queryId = strId.toInt();
    for (int i = 0; i < table->rowCount(); ++i) {
        int rowId = table->item(i, 0)->text().toInt();
        if (rowId == queryId) {
            table->selectRow(i);

            // 提取学生全部信息
            QString name = table->item(i, 1)->text();
            float score = table->item(i, 2)->text().toFloat();
            QString grade = table->item(i, 3)->text();

            // 展示详细信息
            QString info = QString("找到该学生！\n姓名：%1\n成绩：%2\n等级：%3")
                               .arg(name)
                               .arg(score)
                               .arg(grade);
            QMessageBox::information(this, "成功", info);

            return;
        }
    }

    QMessageBox::information(this, "提示", "未找到学号对应的学生！");
}


//进行所有学生的信息统计
void StudentDialog::onStatClicked()
{
    int total = 0;
    float passRate = 0.0f;
    float excellentRate = 0.0f;
    std::vector<int> scoreRanges(5, 0);
    calculateStatistics(total, passRate, excellentRate, scoreRanges);

    QString statText = QString("总人数：%1\n")
                           .arg(total) +
                       QString("及格率：%1%\n")
                           .arg(passRate, 0, 'f', 2) +
                       QString("优良率：%1%\n")
                           .arg(excellentRate, 0, 'f', 2) +
                       QString("分数段分布：\n") +
                       QString("0-59：%1人\n")
                           .arg(scoreRanges[0]) +
                       QString("60-69：%1人\n")
                           .arg(scoreRanges[1]) +
                       QString("70-79：%1人\n")
                           .arg(scoreRanges[2]) +
                       QString("80-89：%1人\n")
                           .arg(scoreRanges[3]) +
                       QString("90-100：%1人")
                           .arg(scoreRanges[4]);

    QMessageBox::information(this, "统计信息", statText);
}
//等级计算与划分
void StudentDialog::calculateStatistics(int &total, float &passRate, float &excellentRate, std::vector<int> &scoreRanges)
{
    total = students.size();
    if (total == 0) {
        passRate = 0.0f;
        excellentRate = 0.0f;
        std::fill(scoreRanges.begin(), scoreRanges.end(), 0);
        return;
    }

    int passCount = 0;
    int excellentCount = 0;
    std::fill(scoreRanges.begin(), scoreRanges.end(), 0);

    for (const auto &stu : students) {
        if (stu.score >= 60) {
            passCount++;
            if (stu.score >= 80) {
                excellentCount++;
            }
        }

        if (stu.score < 60) {
            scoreRanges[0]++;
        } else if (stu.score < 70) {
            scoreRanges[1]++;
        } else if (stu.score < 80) {
            scoreRanges[2]++;
        } else if (stu.score < 90) {
            scoreRanges[3]++;
        } else {
            scoreRanges[4]++;
        }
    }

    passRate = (float)passCount / total * 100;
    excellentRate = (float)excellentCount / total * 100;
}
//导入txt文件
void StudentDialog::onLoadTxtClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择TXT文件", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    loadFromTxt(fileName);
    refreshTable();
    QMessageBox::information(this, "成功", "从TXT加载数据成功！");
}

void StudentDialog::loadFromTxt(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件！");
        return;
    }

    students.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(';');
        if (parts.size() != 3) continue;

        Student stu;
        stu.id = parts[0].toInt();
        stu.name = parts[1];
        stu.score = parts[2].toFloat();
        students.push_back(stu);
    }
    file.close();
}
//保存为二进制文件
void StudentDialog::onSaveBinaryClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存为二进制文件", "", "Binary Files (*.dat)");
    if (fileName.isEmpty()) return;

    saveToBinary(fileName);
    QMessageBox::information(this, "成功", "保存为二进制文件成功！");
}

void StudentDialog::saveToBinary(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "错误", "无法创建文件！");
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);




    // 写入数据
    out << (qint32)students.size();
    for (const auto &stu : students) {
        out << (qint32)stu.id;
        out << stu.name;
        out << (float)stu.score;
    }
    file.close();

}
//导入二进制文件
void StudentDialog::onLoadBinaryClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择二进制文件", "", "Binary Files (*.dat)");
    if (fileName.isEmpty()) return;

    loadFromBinary(fileName);
    refreshTable();
    QMessageBox::information(this, "成功", "从二进制文件加载数据成功！");
}

void StudentDialog::loadFromBinary(const QString &fileName)
{
    QFile file(fileName);
    if (!file.exists()) { // 先检查文件是否存在
        QMessageBox::warning(this, "错误", "文件不存在！路径：" + fileName);
        return;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "错误", "无法打开文件！原因：" + file.errorString());
        return;
    }

    students.clear();
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_10);

    qint32 count;
    in >> count;


    for (int i = 0; i < count; ++i) {
        Student stu;
        qint32 id;
        in >> id;
        stu.id = (int)id;
        in >> stu.name;
        float score;
        in >> score;
        stu.score = score;
        students.push_back(stu);

    }
    file.close();

    refreshTable();

}

// 加载美化所需的各种 QSS 样式表
void StudentDialog::loadStyleSheet()
{
    QFile file("style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        this->setStyleSheet(qss);
        file.close();
    }
}
