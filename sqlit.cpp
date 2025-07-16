#include "sqlit.h"

/* 静态成员定义 */
sqlit *sqlit::m_pInstance = nullptr;
QMutex sqlit::m_mutex;
sqlit::Garbo sqlit::m_garbo;

sqlit::sqlit(QObject *parent) : QObject(parent) {}

sqlit::~sqlit()
{
    if (m_db.isOpen())
        m_db.close();
}

/* 初始化数据库连接 */
bool sqlit::initDB(const QString &dbPath)
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
        m_db = QSqlDatabase::database("qt_sql_default_connection");
    else
        m_db = QSqlDatabase::addDatabase("QSQLITE");

    m_db.setDatabaseName(dbPath);

    if (!m_db.open())
    {
        qWarning() << "Open DB failed:" << m_db.lastError().text();
        return false;
    }
    return createTable();
}

/* 创建两张表 */
bool sqlit::createTable()
{
    QSqlQuery query;
    /* 管理员表：id 主键，pwd 密码 */
    if (!query.exec("CREATE TABLE IF NOT EXISTS admin("
                    "id   TEXT PRIMARY KEY,"
                    "pwd  TEXT NOT NULL)"))
    {
        qWarning() << "Create admin table failed:" << query.lastError().text();
        return false;
    }

    /* 打卡记录表：自增主键，id 外键，date/time/valid 字段 */
    if (!query.exec("CREATE TABLE IF NOT EXISTS record("
                    "rec_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "id     TEXT,"
                    "date   TEXT,"
                    "time   TEXT,"
                    "valid  TEXT,"
                    "FOREIGN KEY(id) REFERENCES admin(id))"))
    {
        qWarning() << "Create record table failed:" << query.lastError().text();
        return false;
    }
    insertAdmin("1", "2");
    insertRecord("17", "2001-01-01", "09:00:01", "yes");
    return true;
}

/* 插入/更新管理员密码 */
bool sqlit::insertAdmin(const QString &id, const QString &pwd)
{
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO admin(id, pwd) VALUES(?, ?)");
    query.addBindValue(id);
    query.addBindValue(pwd);
    if (!query.exec())
    {
        qWarning() << "Insert admin failed:" << query.lastError().text();
        return false;
    }
    return true;
}

/* 根据 id 查询管理员密码 */
QString sqlit::getAdminPassword(const QString &id) const
{
    QSqlQuery query;
    query.prepare("SELECT pwd FROM admin WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next())
        return query.value(0).toString();
    return QString();
}

/* 插入一条记录 */
bool sqlit::insertRecord(const QString &id,const QString &date,const QString &time,const QString &valid)
{
    QSqlQuery query;
    query.prepare("INSERT INTO record(id, date, time, valid) VALUES(?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(date);
    query.addBindValue(time);
    query.addBindValue(valid);
    if (!query.exec())
    {
        qWarning() << "Insert record failed:" << query.lastError().text();
        return false;
    }
    return true;
}

/* 根据 id 查询所有打卡记录，返回 QList<QString> 每行格式：date|time|valid */
QList<QString> sqlit::getRecord(const QString &id) const
{
    QList<QString> list;
    QSqlQuery query;
    query.prepare("SELECT date, time, valid FROM record WHERE id = ? ORDER BY rec_id DESC");
    query.addBindValue(id);
    if (query.exec())
    {
        while (query.next())
        {
            list.append(QString("%1|%2|%3")
                        .arg(query.value(0).toString())
                        .arg(query.value(1).toString())
                        .arg(query.value(2).toString()));
        }
    }
    return list;
}

void sqlit::updateAdminPassword(const QString &id, const QString &newPwd)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE admin SET pwd=? WHERE id=?");
    query.addBindValue(newPwd);
    query.addBindValue(id);
    query.exec();
}
