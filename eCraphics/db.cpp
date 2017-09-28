#include "db.h"

DB::DB()
{
    //pDataBase = new QSqlDatabase;

    param = "DISTINCT Owner";
    table = "tAlfaBank";
}

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

QStringList DB::Query(const QString &selectParam, const QString &tableWhere)
{
   QSqlQuery *query = new QSqlQuery();

   query->exec("USE CallCenter SELECT " + selectParam + " FROM " + tableWhere);
   ToLog("Execute query: " + query->lastQuery());

   if(!resultList.empty())
       resultList.clear();

   while(query->next())
   {
       QString result = query->value(0).toString();
       resultList.append(result);
   }

   delete query;

   return resultList;
}

QStringList DB::CustomQuery(const QString &queryStr, int mode = 0)
{
    QSqlQuery *query = new QSqlQuery();
    query->exec(queryStr);
    ToLog("Execute query: " + query->lastQuery());

    if(query->lastError().text() != " ")
    {
        ToLog(query->lastError().text());
        return this->resultList;
    }

    if(!resultList.empty())
        resultList.clear();

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

void DB::StartQuery()
{
     queryResult = Query("DISTINCT Owner", "t" + table + param);
     moveToThread(QApplication::instance()->thread());
     emit QueryFinish();
}

void DB::StartQuery2()
{
     queryResult = Query("DISTINCT Result", "t" + table + param);
     moveToThread(QApplication::instance()->thread());
     emit QueryFinish();
}

void DB::StartCustomQuery()
{
    queryResult = CustomQuery(queryStr, mode);
    moveToThread(QApplication::instance()->thread());
    emit QueryFinish();
}

void DB::SetMode(int mode)
{
    this->mode = mode;
}
