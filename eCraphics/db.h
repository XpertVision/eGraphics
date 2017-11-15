#ifndef DB_H
#define DB_H

#include "headers.h"

class DB : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase dataBase;
    QStringList resultList;

    QStringList queryList;

    bool connectResult;

    int mode;

    QStringList CustomQuery(const QString&, int);
public:
    DB();

    QSqlDatabase* GetSqlDB();

    bool Connect();
    bool GetConnectResult();
    void SetMode(int);

    void SetQueryList(QStringList);

    QString param, table, queryStr;//!!!!!
    QStringList queryResult;//!!!!!

    QString error;//!!!!!!

    ~DB();
public slots:
    void StartConnect();
    void StartCustomQuery();
    void StartCustomMultQuery();
signals:
    void ConnectFinish();
    void QueryFinish();
};

#endif // DB_H
