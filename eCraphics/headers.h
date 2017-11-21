#ifndef HEADERS_H
#define HEADERS_H

#include <QObject>
#include <QMainWindow>
#include <QListWidget>
#include <QtCharts/QtCharts>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSet>

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <QtSql/QSqlDriver>
#include <QtSql/QtSql>
#include <QtPlugin>
#include <odbcinst.h>

#include <QtSql/QSqlDriverPlugin>

#include <QPalette>

#include <QDate>

#include <QtXml>

#include <QMovie>

//---------------------------------------------

#include <memory>
#include <utility>
#include <exception>

//---------------------------------------------

#include <Windows.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Mincore.lib")

//---------------------------------------------

#include "db.h"
#include "mychart.h"
#include "xmlparser.h"
#include "update.h"
#include "mylogger.h"
#include "settings.h"
#include "myconfig.h"
#include "changelist.h"
#include "mylistwidget.h"
#include "myuithreadblocker.h"
#include "authorization.h"

#endif // HEADERS_H
