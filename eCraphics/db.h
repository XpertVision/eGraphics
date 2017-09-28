#ifndef DB_H
#define DB_H

#include "headers.h"

class DB : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase dataBase;
    QStringList resultList;

    bool connectResult;

    int mode;
    //QStringList queryResult;
public:
    DB();

    bool Connect();
    QStringList Query(const QString&, const QString&);
    bool GetConnectResult();
    QStringList CustomQuery(const QString&, int);

    QSqlDatabase* GetSqlDB();

    ~DB();

    QString param, table, queryStr;
    QStringList queryResult;

    QString error;

    void SetMode(int);
private slots:
    void StartConnect();
    void StartQuery();
    void StartQuery2();
    void StartCustomQuery();
signals:
    void ConnectFinish();
    void QueryFinish();
};

#endif // DB_H
