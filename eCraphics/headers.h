#ifndef HEADERS_H
#define HEADERS_H

#include <QMainWindow>

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

//---------------------------------------------

#include <Windows.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "Version.lib")

//---------------------------------------------
extern int chartTheme;

#include "db.h"
#include "mychart.h"
#include "xmlparser.h"
#include "update.h"
#include "mylogger.h"
#include "settings.h"

#endif // HEADERS_H
