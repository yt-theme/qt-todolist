#include "sqlite_db.h"

Sqlite_db::Sqlite_db()
{
    init();
}

Sqlite_db::~Sqlite_db()
{

}

void Sqlite_db::init () {
    // 创建 ~/.local/share/todoList 目录
    sqlite_db_dir_path = QString("%1/%2").arg(homePath).arg(DB_PATH);
    QDir sqlite_dir(sqlite_db_dir_path);
    if (!sqlite_dir.exists()) {
        auto mkdir_ok = sqlite_dir.mkdir(sqlite_db_dir_path);
        if (!mkdir_ok) {
            throw(mkdir_ok);
        }
    }


}

bool Sqlite_db::connection (const QString dbFileName) {
    // 数据库准备
    if (QSqlDatabase::contains(dbFileName)) {
        db = QSqlDatabase::database(dbFileName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", dbFileName);
        QString sqlite_db_path = QString("%1/%2.db").arg(sqlite_db_dir_path).arg(dbFileName);
        db.setDatabaseName(sqlite_db_path);
    }

    auto ok = db.open();
    if (ok) {
        qDebug() << "database open ok" << ok;
        return true;
    } else {
        qDebug() << "database open err" << ok;
        return false;
    }
}

void Sqlite_db::close () {
    db.close();
}

bool Sqlite_db::exec (const QString sql_str) {
    QSqlQuery qr = QSqlQuery(db);
    bool isOk = qr.exec(sql_str);

    if (isOk) {
        qDebug() << "Sqlite Exec Ok =>" << isOk << qr.lastError();
    } else {
        qDebug() << "Sqlite Exec Err =>" << isOk;
    }

    return isOk;
}

bool Sqlite_db::insert (const QString sql_str) {
    bool isOk = exec(sql_str);

    return isOk;
}

bool Sqlite_db::update (const QString sql_str) {
    bool isOk = exec(sql_str);

    return isOk;
}

QSqlQuery Sqlite_db::query (const QString sql_str) {
    QSqlQuery qr = QSqlQuery(db);
    bool isOk = qr.exec(sql_str);

    if (isOk) {
        qDebug() << "Sqlite Query Ok =>" << isOk;

    } else {
        qDebug() << "Sqlite Query Err =>" << isOk << qr.lastError();

    }

    return qr;
}

bool Sqlite_db::delete_ (const QString sql_str) {
    bool isOk = exec(sql_str);

    return isOk;
}

bool Sqlite_db::transaction (const QStringList sql_list) {
    QSqlQuery qr = QSqlQuery(db);

    db.transaction();

    for (auto ite: sql_list) {
        if (!qr.exec(ite)) {
            qDebug() << "Sqlite Transaction Err =>" << qr.lastError();

            return false;
        }
    }

    return db.commit();
}

// ---------------------- 判断 --------------------
bool Sqlite_db::table_isExist (const QString name) {
    QSqlQuery qr = QSqlQuery(db);
    bool isExist = qr.exec( QString("SELECT COUNT(*) FROM sqlite_master WHERE type='table' and name='%1'").arg(name) );

    return isExist;
}
