#ifndef SQLITE_DB_H
#define SQLITE_DB_H
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>
#include <QDir>
#include <QtDebug>

#include <define.h>

namespace Db {
    class Sqlite_db;
}

class Sqlite_db
{
public:
    Sqlite_db();
    ~Sqlite_db();

    QString                 homePath = QDir::homePath();

    QSqlDatabase            db;

    bool                    connection(const QString);              // 创建连接 并打开数据库
    void                    close();                                // 断开连接
    bool                    exec(const QString);                    // 执行语句
    bool                    insert(const QString);                  // 数据库操作
    bool                    update(const QString);                  // 数据库操作
    QSqlQuery               query(const QString);                   // 数据库操作
    bool                    delete_(const QString);                 // 数据库操作

    bool                    transaction(const QStringList);         // 事务

    // ------------------- 判断 ------------------
    bool                    table_isExist(const QString);           // 判断表是否存在

private:
    QString                 sqlite_db_dir_path;                     // 数据库目录
    void                    init();                                 // 初始化操作
};

#endif // SQLITE_DB_H
