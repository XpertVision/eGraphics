#ifndef MYLOGGER_H
#define MYLOGGER_H

#include "headers.h"

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

    static MyLogger* GetInstance(QString logFileName, QString logFilePath);
    void AddToLog(QString strToLog);
public:
    static MyLogger* GetInstance();

    static bool InitLog(QString logFileName, QString logFilePath)
    {
        if(MyLogger::GetInstance(logFileName, logFilePath) != nullptr)
            return true;
        else
            return false;
    }

    static void ToLog(QString strToLog){ MyLogger::GetInstance()->AddToLog(strToLog); }

    static void CloseLog(){ MyLogger::GetInstance()->~MyLogger(); }

    ~MyLogger();
signals:
public slots:
};

#endif // MYLOGGER_H
