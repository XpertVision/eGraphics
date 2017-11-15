#ifndef MYCONFIG_H
#define MYCONFIG_H

#include "headers.h"

struct ConfData
{
    int themID           = 0;
    int singlChartID     = 0;
    int multChartID      = 0;
    int langID           = 0;

    int enableFullScreen = 0;

    int enableWindowSize = 0;
    int width            = 0;
    int height           = 0;
};

class MyConfig : public QObject
{
    Q_OBJECT
private:
    static MyConfig* pMyConfigInstance;//

    QXmlStreamWriter* xmlWriter;
    QXmlStreamReader* xmlReader;

    QString confFileName;
    QString confFilePath;

    ConfData confFromFile;

    explicit MyConfig(QObject *parent = nullptr);//
    explicit MyConfig(QString  confFileName, QString confFilePath, QObject *parent = nullptr);//
    MyConfig(const MyConfig&) = delete;//
    MyConfig& operator = (MyConfig&) = delete;//

    static MyConfig* GetInstance(QString configFileName, QString configFilePath);

    ConfData GetConfig() const;
    void SetConfig(ConfData&);

    bool OpenConfig();
    bool SaveConfig();
public:
    static MyConfig* GetInstance();

    static bool InitConfig(QString configFileName, QString configFilePath)
    {
        if(MyConfig::GetInstance(configFileName, configFilePath) != nullptr)
            return true;
        else
            return false;
    }

    static bool SaveConfigure(){ return MyConfig::GetInstance()->SaveConfig(); }

    static bool OpenConfigure(){ return MyConfig::GetInstance()->OpenConfig(); }

    static ConfData GetConfigure(){ return MyConfig::GetInstance()->GetConfig(); }

    static void SetConfigure(ConfData& data){ MyConfig::GetInstance()->SetConfig(data); }


    ~MyConfig();//
signals:

public slots:
};

#endif // MYCONFIG_H
