#ifndef SQLIT_H
#define SQLIT_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QList>
#include <QDebug>

class sqlit : public QObject
{
    Q_OBJECT
public:
    static sqlit *getInstance()
    {
        if (m_pInstance == nullptr)
        {
            QMutexLocker locker(&m_mutex);
            if (m_pInstance == nullptr)
                m_pInstance = new sqlit();
        }
        return m_pInstance;
    }
    /* 用于自动析构单例的嵌套类 */
    class Garbo
    {
    public:
        ~Garbo()
        {
            if (m_pInstance)
            {
                delete m_pInstance;
                m_pInstance = nullptr;
                qDebug() << "[sqlit] singleton destroyed";
            }
        }
    };
    static Garbo m_garbo;
    QSqlDatabase m_db;

    explicit sqlit(QObject *parent = nullptr);
    ~sqlit();
    /* 初始化数据库连接 */
    bool initDB(const QString &dbPath);
    /* 创建管理员表和记录表 */
    bool createTable();
    /* 管理员表操作 */
    bool insertAdmin(const QString &id, const QString &pwd);
    QString getAdminPassword(const QString &id) const;

    /* 记录表操作 */
    bool insertRecord(const QString &id,
                      const QString &date,
                      const QString &time,
                      const QString &valid);
    QList<QString> getRecord(const QString &id) const;

    void updateAdminPassword(const QString &id, const QString &newPwd);

private:

    static sqlit *m_pInstance;
    static QMutex m_mutex;

};

#endif // SQLIT_H
