#ifndef SETTINGS_H
#define SETTINGS_H

#include "headers.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;

    void AddThemePic(QGraphicsView*, QString);

    void SetLang();
    void SetTheme();
    void SetCharts();
    void SetSizeState();
    void SetSettingsFromConfig();

    void GetFromLang();
    void GetFromTheme();
    void GetFromCharts();
    void GetSizeState();
    void GetSettingsToConfig();
public slots:
};

#endif // SETTINGS_H
