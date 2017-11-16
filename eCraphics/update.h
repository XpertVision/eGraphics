#ifndef UPDATE_H
#define UPDATE_H

#include "headers.h"

struct AppVersion
{
    int first  = 0;
    int second = 0;
    int third  = 0;
    int fourth = 0;
};

class MyUpdater
{
private:
    AppVersion thisApp;
    AppVersion remoteApp;

    bool rollBack;

    QString thisAppPath = "eGraphics.exe";
    QString remoteAppPath = "//10.1.56.88/Programs/eCall/Applications/eGraphics/eGraphics/eGraphics.exe";
    //QString remoteAppPath = "E:/eGraphics.exe";

    AppVersion GetAppVersion(QString);
    bool NeedUpdate();
    QString GetAppVersionToString(AppVersion);
public:
    bool Update();
};

#endif // UPDATE_H
