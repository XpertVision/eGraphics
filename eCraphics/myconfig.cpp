#include "myconfig.h"

MyConfig* MyConfig::pMyConfigInstance = nullptr;



MyConfig::MyConfig(QObject *parent) : QObject(parent)
{
    xmlReader = new QXmlStreamReader();
    xmlWriter = new QXmlStreamWriter();
};

MyConfig::MyConfig(QString  configFileName, QString configFilePath, QObject *parent) : QObject(parent)
{
    this->confFileName = configFileName;
    this->confFilePath = configFilePath;

    xmlReader = new QXmlStreamReader();
    xmlWriter = new QXmlStreamWriter();
};

MyConfig* MyConfig::GetInstance()
{
    if(MyConfig::pMyConfigInstance == nullptr)
        MyConfig::pMyConfigInstance = new MyConfig();

    return MyConfig::pMyConfigInstance;
};

MyConfig* MyConfig::GetInstance(QString configFileName, QString configFilePath)
{
    if(MyConfig::pMyConfigInstance == nullptr)
        MyConfig::pMyConfigInstance = new MyConfig(configFileName, configFilePath);

    return MyConfig::pMyConfigInstance;
};

bool MyConfig::OpenConfig()
{
    QFile configFile;

    if((this->confFilePath + this->confFileName).isEmpty())
    {
        MyLogger::ToLog("Don't set config file name");
        return false;
    }
    else
        configFile.setFileName(this->confFilePath + this->confFileName);

    if(!QFile::exists(configFile.fileName()))
    {
        MyLogger::ToLog("Config file doesn't exist");
        return false;
    }

    if(!configFile.open(QFile::ReadOnly | QFile::Text))
    {
        MyLogger::ToLog("Config file for read didn't open");
        return false;
    }

    this->xmlReader->setDevice(&configFile);

    while(!this->xmlReader->atEnd())
    {
        xmlReader->readNext();

        if(xmlReader->isStartElement())
        {
            if(xmlReader->name() == "theme")
                this->confFromFile.themID = xmlReader->attributes().value("id").toInt();
            else if(xmlReader->name() == "singlchart")
                this->confFromFile.singlChartID = xmlReader->attributes().value("type").toInt();
            else if(xmlReader->name() == "multchart")
                this->confFromFile.multChartID = xmlReader->attributes().value("type").toInt();
            else if(xmlReader->name() == "language")
                this->confFromFile.langID = xmlReader->attributes().value("id").toInt();
            else if(xmlReader->name() == "windowsate")
            {
                this->confFromFile.enableFullScreen = xmlReader->attributes().value("enableFullScreen").toInt();

                this->confFromFile.enableWindowSize = xmlReader->attributes().value("enableSize").toInt();
                this->confFromFile.width = xmlReader->attributes().value("width").toInt();
                this->confFromFile.height = xmlReader->attributes().value("height").toInt();
            }
        }
    }

    if(this->xmlReader->hasError())
    {
        MyLogger::ToLog("Config parsing error: " + this->xmlReader->errorString());
        return false;
    }

    configFile.close();

    return true;
};

bool MyConfig::SaveConfig()
{
    QFile configFile;

    if((this->confFilePath + this->confFileName).isEmpty())
    {
        MyLogger::ToLog("Don't set config file name");
        return false;
    }
    else
        configFile.setFileName(this->confFilePath + this->confFileName);

    if(!QFile::exists(configFile.fileName()))
        MyLogger::ToLog("Config file doesn't exist. Create config file");

    if(!configFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        MyLogger::ToLog("Config file for write didn't open");
        return false;
    }

    this->xmlWriter->setDevice(&configFile);

    this->xmlWriter->setAutoFormatting(true);

    this->xmlWriter->writeStartDocument();
    this->xmlWriter->writeStartElement("Settings");


    this->xmlWriter->writeStartElement("theme");
    this->xmlWriter->writeAttribute("id", QString::number(this->confFromFile.themID));
    this->xmlWriter->writeEndElement();

    this->xmlWriter->writeStartElement("singlchart");
    this->xmlWriter->writeAttribute("type", QString::number(this->confFromFile.singlChartID));
    this->xmlWriter->writeEndElement();

    this->xmlWriter->writeStartElement("multchart");
    this->xmlWriter->writeAttribute("type", QString::number(this->confFromFile.multChartID));
    this->xmlWriter->writeEndElement();

    this->xmlWriter->writeStartElement("language");
    this->xmlWriter->writeAttribute("id", QString::number(this->confFromFile.langID));
    this->xmlWriter->writeEndElement();

    this->xmlWriter->writeStartElement("windowsate");
    this->xmlWriter->writeAttribute("enableFullScreen", QString::number(this->confFromFile.enableFullScreen));
    this->xmlWriter->writeAttribute("enableSize", QString::number(this->confFromFile.enableWindowSize));
    this->xmlWriter->writeAttribute("width", QString::number(this->confFromFile.width));
    this->xmlWriter->writeAttribute("height", QString::number(this->confFromFile.height));
    this->xmlWriter->writeEndElement();

    this->xmlWriter->writeEndElement();
    this->xmlWriter->writeEndDocument();

    configFile.close();

    return true;
};

ConfData MyConfig::GetConfig() const
{
    return this->confFromFile;
};

void MyConfig::SetConfig(ConfData& config)
{
    this->confFromFile = config;
};

MyConfig::~MyConfig()
{
    delete xmlReader;
    delete xmlWriter;
};
