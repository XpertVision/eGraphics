#include "update.h"

AppVersion MyUpdater::GetAppVersion(QString appPath)
{
    AppVersion appVer;
    MyLogger::ToLog("Program on server path: " + appPath);
    if(!QFile::exists(appPath))
    {
        MyLogger::ToLog("Don't exist program on update server");

        appVer.first = -1;

        return appVer;
    }

    DWORD  verHandle = 0;
    UINT   size      = 0;
    LPVOID lpBuffer  = NULL;
    LPCWSTR szVersionFile;

    szVersionFile = appPath.toStdWString().c_str();
    //DWORD  verSize   = GetFileVersionInfoSize(szVersionFile, &verHandle);GetFileV
    DWORD verSize = GetFileVersionInfoSizeW(szVersionFile, &verHandle);

    qDebug() << appPath.toStdString().c_str() << ":::" << verSize;


    if (verSize)
    {
        qDebug() << "~1";
        LPWSTR verData = new wchar_t[verSize];

        if (GetFileVersionInfoW(szVersionFile, verHandle, verSize, verData)/*GetFileVersionInfo(szVersionFile, verHandle, verSize, verData)*/)
        {
            qDebug() << "~2";
            if (VerQueryValueW(verData, L"\\", /*(VOID FAR* FAR*)*/&lpBuffer, &size))
            {
                qDebug() << "~3";
                if (size)
                {
                    qDebug() << "~4";
                    VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
                    if (verInfo->dwSignature == 0xfeef04bd)
                    {
                        appVer.first = ((verInfo->dwFileVersionMS >> 16) & 0xffff);
                        qDebug() << appVer.first;
                        appVer.second = ((verInfo->dwFileVersionMS >>  0) & 0xffff);
                        qDebug() << appVer.second;
                        appVer.third = ((verInfo->dwFileVersionLS >> 16) & 0xffff);
                        qDebug() << appVer.third;
                        appVer.fourth = ((verInfo->dwFileVersionLS >>  0) & 0xffff);
                        qDebug() << appVer.fourth;
                    }
                }
            }
        }
        delete[] verData;
    }

    qDebug() << "LAST ERROR: "<< GetLastError();

    qDebug() << QApplication::applicationVersion();

    return appVer;
}

bool MyUpdater::NeedUpdate()
{
    if(this->thisApp.first != this->remoteApp.first)
    {
        if(this->thisApp.first > this->remoteApp.first)
            this->rollBack = true;
        else
            this->rollBack = false;

        return true;
    }
    else if(this->thisApp.second != this->remoteApp.second)
    {
        if(this->thisApp.second > this->remoteApp.second)
            this->rollBack = true;
        else
            this->rollBack = false;

        return true;
    }
    else if(this->thisApp.third != this->remoteApp.third)
    {
        if(this->thisApp.third > this->remoteApp.third)
            this->rollBack = true;
        else
            this->rollBack = false;

        return true;
    }
    else if(this->thisApp.fourth != this->remoteApp.fourth)
    {
        if(this->thisApp.fourth > this->remoteApp.fourth)
            this->rollBack = true;
        else
            this->rollBack = false;

        return true;
    }
    else
    {
        return false;
    }
}

QString MyUpdater::GetAppVersionToString(AppVersion version)
{
    QString stringAppVersion;
    stringAppVersion += QString::number(version.first);
    stringAppVersion += ".";
    stringAppVersion += QString::number(version.second);
    stringAppVersion += ".";
    stringAppVersion += QString::number(version.third);
    stringAppVersion += ".";
    stringAppVersion += QString::number(version.fourth);

    return stringAppVersion;
}

bool MyUpdater::Update()
{
    this->remoteApp = this->GetAppVersion(this->remoteAppPath);
    if(this->remoteApp.first == -1)
        return false;

    this->thisApp = this->GetAppVersion(this->thisAppPath);

    if(this->NeedUpdate())
    {
        QMessageBox mbUpdate;

        if(this->rollBack)
            mbUpdate.setText(QString("Доступна старіша версія программи: ") + this->GetAppVersionToString(this->remoteApp) + ". Оновіться до неї, якщо вас попереджали про це. Оновити?");
        else
            mbUpdate.setText(QString("Знайдена нова версія программи: ") + this->GetAppVersionToString(this->remoteApp) + ". Оновити?");

        mbUpdate.setWindowTitle("Оновлення");

        mbUpdate.setStandardButtons(QMessageBox::Button::Yes | QMessageBox::Button::No);
        mbUpdate.setButtonText(QMessageBox::Button::Yes, "Так");
        mbUpdate.setButtonText(QMessageBox::Button::No, "Ні");

        mbUpdate.show();
        int updateButtonClickId = mbUpdate.exec();

        if(updateButtonClickId == QMessageBox::Button::Yes)
        {
            QFile fileNew(remoteAppPath);
            if(fileNew.exists())
            {
                QFile file("eGraphics.exe");
                file.rename("eGraphics.exe.bak");

                fileNew.copy("eGraphics.exe");

                return true;
            }
        }
        else if(updateButtonClickId == QMessageBox::Button::No)
            MyLogger::ToLog("Update to version: " + this->GetAppVersionToString(this->remoteApp) + " decline by user");
    }
    return false;
}
