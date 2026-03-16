#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include <QHeaderView>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>


// 学生数据结构
struct Student {
    int id;
    QString name;
    float score;
    // 计算等级
    QChar getGrade() const {
        if (score >= 90) return 'A';
        else if (score >= 80) return 'B';
        else if (score >= 60) return 'C';
        else return 'D';
    }
    bool operator==(const Student&other)const{
        return this->id==other.id;
    }
};

// MySQL配置（子对话框，用于输入用户名、密码等）
class MySQLConfigDialog : public QDialog {
    Q_OBJECT
public:
    explicit MySQLConfigDialog(QWidget *parent = nullptr);
    // 获取用户输入的配置信息
    QString getHost() const { return leHost->text().trimmed(); }
    QString getDbName() const { return leDbName->text().trimmed(); }
    QString getUser() const { return leUser->text().trimmed(); }
    QString getPwd() const { return lePwd->text().trimmed(); }
    int getPort() const { return lePort->text().toInt(); }

private:
    QLabel *lbHost;    // 主机地址标签
    QLabel *lbDbName;  // 数据库名标签
    QLabel *lbUser;    // 用户名标签
    QLabel *lbPwd;     // 密码标签
    QLabel *lbPort;    // 端口标签
    QLineEdit *leHost; // 主机地址输入框
    QLineEdit *leDbName;// 数据库名输入框
    QLineEdit *leUser; // 用户名输入框
    QLineEdit *lePwd;  // 密码输入框（隐藏输入）
    QLineEdit *lePort; // 端口输入框
    QPushButton *btnOk; // 确认按钮
    QPushButton *btnCancel; // 取消按钮
};

class StudentDialog : public QDialog
{
    Q_OBJECT

public:
    StudentDialog(QWidget *parent = nullptr);
    ~StudentDialog() override;

private slots:
    void onAddStudentClicked();  // 添加学生按钮的槽函数
    void onQueryStudentClicked(); // 查询学生按钮的槽函数
    void onDeleteStudentClicked(); // 删除学生按钮的槽函数

    void onStatClicked();         // 统计信息的槽函数
    void onLoadTxtClicked();      // 从TXT加载的槽函数
    void onSaveBinaryClicked();   // 保存为二进制的槽函数
    void onLoadBinaryClicked();   // 从二进制加载的槽函数
    void onSaveToMySQLClicked();  // 保存到MySQL
    void onLoadFromMySQLClicked();// 从MySQL加载

private:
    // 原有界面控件声明（不变）
    QLabel *lbId;       // 学号标签
    QLabel *lbName;     // 姓名标签
    QLabel *lbScore;    // 成绩标签
    QLineEdit *leId;    // 学号编辑框
    QLineEdit *leName;  // 姓名编辑框
    QLineEdit *leScore; // 成绩编辑框
    QPushButton *btnAdd; // 添加按钮
    QPushButton *btnQuery; // 查询按钮
    QPushButton *btnDelete; // 删除按钮

    QPushButton *btnStat; // 统计按钮
    QPushButton *btnLoadTxt; // 从TXT加载按钮
    QPushButton *btnSaveBinary; // 保存为二进制按钮
    QPushButton *btnLoadBinary; // 从二进制加载按钮
    // 新增MySQL相关按钮
    QPushButton *btnSaveToMySQL; // 保存到MySQL按钮
    QPushButton *btnLoadFromMySQL; // 从MySQL加载按钮
    QTableWidget *table; // 学生列表控件

    std::vector<Student> students; // 存储所有学生数据
    QSqlDatabase db; // MySQL数据库连接对象

    void initUI();
    void refreshTable();
    void calculateStatistics(int &total, float &passRate, float &excellentRate, std::vector<int> &scoreRanges);
    void loadFromTxt(const QString &fileName);
    void saveToBinary(const QString &fileName);
    void loadFromBinary(const QString &fileName);
    bool initMySQLConnection(const QString &host, const QString &dbName, const QString &user, const QString &pwd, int port); // MySQL连接
    void createStudentTable(); // 创建学生表
    void loadStyleSheet();
    void showMySQLConfigDialog(QString &host, QString &dbName, QString &user, QString &pwd, int &port);
};

#endif // STUDENTDIALOG_H
