#include "db.h"

DB::DB(){}

bool DB::Connect()
{
    dataBase = QSqlDatabase::addDatabase("QODBC3");
    //dataBase.setDatabaseName("Driver={SQL SERVER NATIVE CLIENT 11.0};Server=SQLSRV;Database=CallCenter;Uid=eGraphics;Pwd=EG!23456;");
    dataBase.setDatabaseName("Driver={SQL SERVER NATIVE CLIENT 11.0};Server=DISNEY\\SQLEXPRESS;Database=CallCenter;Uid=eGraphics;Pwd=EG!234567;");

    return dataBase.open();
}

QSqlDatabase* DB::GetSqlDB()
{
    return &dataBase;
}

void DB::SetQueryList(QStringList list)
{
    this->queryList.clear();
    this->queryList = list;
}

QStringList DB::CustomQuery(const QString &queryStr, int mode = 0)
{
    QSqlQuery *query = new QSqlQuery();
    query->exec(queryStr);
    MyLogger::ToLog("Execute query: " + query->lastQuery());

    if(!resultList.empty())
        resultList.clear();

    if(query->lastError().text() != " ")
    {
        MyLogger::ToLog(query->lastError().text());
        return this->resultList;
    }

    if(mode == 0)
    {
        while(query->next())
        {
            QString result = query->value(0).toString();
            resultList.append(result);
        }
    }
    else if(mode ==1)
    {
        while(query->next())
        {
            QString result = query->value(0).toString();
            resultList.append(result);
            result = query->value(1).toString();
            resultList.append(result);
        }
    }

    delete query;

    return resultList;
}

bool DB::GetConnectResult()
{
    return connectResult;
}

DB::~DB()
{
    if(dataBase.isOpen())
        dataBase.close();
}

void DB::StartConnect()
{
    connectResult = Connect();
    moveToThread(QApplication::instance()->thread());
    emit ConnectFinish();
}

void DB::StartCustomQuery()
{
    queryResult = CustomQuery(queryStr, mode);
    moveToThread(QApplication::instance()->thread());
    emit QueryFinish();
}

void DB::StartCustomMultQuery()
{
    queryResult.clear();
    for(int i = 0; i < this->queryList.count(); i++)
        queryResult += CustomQuery(queryList.at(i), mode);

    moveToThread(QApplication::instance()->thread());
    emit QueryFinish();
}

void DB::SetMode(int mode)
{
    this->mode = mode;
}
