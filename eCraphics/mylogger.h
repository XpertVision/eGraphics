#ifndef MYLOGGER_H
#define MYLOGGER_H

#include "headers.h"
#include <QObject>

class MyLogger : public QObject
{
private:
    Q_OBJECT

    static MyLogger* pMyLoggerInstance;

    explicit MyLogger(QObject *parent = nullptr);
    MyLogger(QString logFileName, QString logFilePath);
    MyLogger(const MyLogger&) = delete;
    MyLogger& operator = (MyLogger&) = delete;

    QFile logger;
    QTextStream* loggerStream;
    bool openLogFile = false;

    void OpenFileForLog();
    void CloseLogFile();

    void SetLogFileName(QString logFileName);
    void SetLogFilePath(QString logFilePath);

    static QString logFileName;
    static QString logFilePath;

public:
    void AddToLog(QString strToLog);

    static MyLogger* GetInstance();
    static MyLogger* GetInstance(QString logFileName, QString logFilePath);

    ~MyLogger();
signals:
public slots:
};

#define ToLog(a) MyLogger::GetInstance()->AddToLog(a)
#define InitLog(a, b) MyLogger::GetInstance(a, b)
#define CloseLog() MyLogger::GetInstance()->~MyLogger()

#endif // MYLOGGER_H
