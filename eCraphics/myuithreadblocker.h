#ifndef MYUITHREADBLOCKER_H
#define MYUITHREADBLOCKER_H

#include "headers.h"

class DB;

class myUIThreadBlocker : public QObject
{
    Q_OBJECT
protected:
    QThread* thr = nullptr;
    QMessageBox* mb = nullptr;
    DB* pDb = nullptr;
public:
    explicit myUIThreadBlocker(QObject *parent = nullptr);

    virtual void Start() = 0;
};

/**********************************************************************************************************************************/

class myUIThreadBlockerConnect : public myUIThreadBlocker
{
    Q_OBJECT
public:
    myUIThreadBlockerConnect(DB* db, QString  mbTitle, QString mbText, QObject *parent = nullptr);

    void Start();

    ~myUIThreadBlockerConnect();
};

/**********************************************************************************************************************************/

class myUIThreadBlockerQuery : public myUIThreadBlocker
{
    Q_OBJECT
public:
    myUIThreadBlockerQuery(DB* db, QString  mbTitle, QString mbText, QObject *parent = nullptr);

    void Start();

    ~myUIThreadBlockerQuery();
};

/**********************************************************************************************************************************/
class myUIThreadBlockerMultQuery : public myUIThreadBlocker
{
    Q_OBJECT
private:
    QStringList allQuery;
public:
    myUIThreadBlockerMultQuery(DB* db, QString  mbTitle, QString mbText, QStringList queryList, QObject *parent = nullptr);

    void SetParams(DB* db, QString  mbTitle, QString mbText, QStringList queryList);
    void Start();

    ~myUIThreadBlockerMultQuery();

public slots:
};

#endif // MYUITHREADBLOCKER_H
