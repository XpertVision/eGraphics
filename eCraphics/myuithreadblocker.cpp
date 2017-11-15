#include "myuithreadblocker.h"

myUIThreadBlocker::myUIThreadBlocker(QObject *parent) : QObject(parent){}

myUIThreadBlockerConnect::myUIThreadBlockerConnect(DB* db, QString mbTitle, QString mbText, QObject *parent) : myUIThreadBlocker(parent)
{
    pDb = db;

    if(mb != nullptr)
        delete mb;

    mb = new QMessageBox;
    mb->setText(mbText);
    mb->setWindowTitle(mbTitle);
    mb->setStandardButtons(QMessageBox::NoButton);

    if(thr != nullptr)
        delete thr;

    thr = new QThread();

    pDb->moveToThread(thr);

    connect(thr, &QThread::started, pDb, &DB::StartConnect);
    connect(pDb, &DB::ConnectFinish, thr, &QThread::quit);
    connect(thr, &QThread::finished, mb, &QMessageBox::reject);
}

void myUIThreadBlockerConnect::Start()
{
    thr->start();

    mb->show();
    mb->exec();
}

myUIThreadBlockerConnect::~myUIThreadBlockerConnect()
{
    disconnect(thr, &QThread::started, pDb, &DB::StartConnect);
    disconnect(pDb, &DB::ConnectFinish, thr, &QThread::quit);
    disconnect(thr, &QThread::finished, mb, &QMessageBox::reject);

    delete mb;
    delete thr;
}

/*****************************************************************************************************************************************/

myUIThreadBlockerQuery::myUIThreadBlockerQuery(DB* db, QString mbTitle, QString mbText, QObject *parent) : myUIThreadBlocker(parent)
{
    pDb = db;

    if(mb != nullptr)
        delete mb;

    mb = new QMessageBox;
    mb->setText(mbText);
    mb->setWindowTitle(mbTitle);
    mb->setStandardButtons(QMessageBox::NoButton);

    if(thr != nullptr)
        delete thr;

    thr = new QThread();

    pDb->moveToThread(thr);

    connect(thr, &QThread::started, pDb, &DB::StartCustomQuery);
    connect(pDb, &DB::QueryFinish, thr, &QThread::quit);
    connect(thr, &QThread::finished, mb, &QMessageBox::reject);
}

void myUIThreadBlockerQuery::Start()
{
    thr->start();

    mb->show();
    mb->exec();
}

myUIThreadBlockerQuery::~myUIThreadBlockerQuery()
{
    disconnect(thr, &QThread::started, pDb, &DB::StartCustomQuery);
    disconnect(pDb, &DB::QueryFinish, thr, &QThread::quit);
    disconnect(thr, &QThread::finished, mb, &QMessageBox::reject);

    delete mb;
    delete thr;
}

/*****************************************************************************************************************************************/

myUIThreadBlockerMultQuery::myUIThreadBlockerMultQuery(DB* db, QString mbTitle, QString mbText, QStringList queryList, QObject *parent) : myUIThreadBlocker(parent)
{
    pDb = db;
    pDb->SetQueryList(queryList);

    if(mb != nullptr)
        delete mb;

    mb = new QMessageBox;
    mb->setText(mbText);
    mb->setWindowTitle(mbTitle);
    mb->setStandardButtons(QMessageBox::NoButton);

    if(thr != nullptr)
        delete thr;

    thr = new QThread;

    pDb->moveToThread(thr);

    connect(thr, &QThread::started, pDb, &DB::StartCustomMultQuery);
    connect(pDb, &DB::QueryFinish, thr, &QThread::quit);
    connect(thr, &QThread::finished, mb, &QMessageBox::reject);
}

void myUIThreadBlockerMultQuery::Start()
{
    thr->start();

    mb->show();
    mb->exec();
}

myUIThreadBlockerMultQuery::~myUIThreadBlockerMultQuery()
{
    disconnect(thr, &QThread::started, pDb, &DB::StartCustomMultQuery);
    disconnect(pDb, &DB::QueryFinish, thr, &QThread::quit);
    disconnect(thr, &QThread::finished, mb, &QMessageBox::reject);

    delete mb;
    delete thr;
}
