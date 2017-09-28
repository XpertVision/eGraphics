#include "mylogger.h"

MyLogger* MyLogger::pMyLoggerInstance = nullptr;
QString MyLogger::logFileName;
QString MyLogger::logFilePath;

MyLogger::MyLogger(QObject *parent) : QObject(parent)
{
    this->loggerStream = nullptr;
}

MyLogger::MyLogger(QString logFileName, QString logFilePath)
{
    SetLogFileName(logFileName);
    SetLogFilePath(logFilePath);

    this->loggerStream = nullptr;

    this->OpenFileForLog();
}

void MyLogger::SetLogFileName(QString logFileName)
{
    MyLogger::logFileName = logFileName + "_" + QDateTime::currentDateTime().toLocalTime().toString("yyyy_MM_dd") + ".txt";
}

void MyLogger::SetLogFilePath(QString logFilePath)
{
    MyLogger::logFilePath = logFilePath;
}

void MyLogger::AddToLog(QString strToLog)
{
    if(this->loggerStream != nullptr)
    {
        (*this->loggerStream) << QDateTime::currentDateTime().toLocalTime().toString("yyyy/MM/dd HH:mm:ss") << ": "<< strToLog << "\n\n";

        this->CloseLogFile();
        this->OpenFileForLog();
    }
}

MyLogger* MyLogger::GetInstance()
{
    if(pMyLoggerInstance == nullptr)
    {
        pMyLoggerInstance = new MyLogger(MyLogger::logFileName, MyLogger::logFilePath);
    }

    return pMyLoggerInstance;
}

MyLogger* MyLogger::GetInstance(QString logFileName, QString logFilePath)
{
    if(pMyLoggerInstance == nullptr)
    {
        pMyLoggerInstance = new MyLogger(logFileName, logFilePath);
    }

    return pMyLoggerInstance;
}

void MyLogger::CloseLogFile()
{
    delete this->loggerStream;
    this->logger.close();
    this->openLogFile = false;
}

void MyLogger::OpenFileForLog()
{
    QDir dir(MyLogger::logFilePath);
    if(!dir.exists())
    {
        dir.mkpath(MyLogger::logFilePath);
    }


    this->logger.setFileName(MyLogger::logFilePath + MyLogger::logFileName);
    this->logger.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    this->loggerStream = new QTextStream(&logger);
    this->openLogFile = true;
}

MyLogger::~MyLogger()
{
    if(this->openLogFile)
        this->CloseLogFile();
}
