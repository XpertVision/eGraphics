#ifndef SETTINGS_H
#define SETTINGS_H

#include "headers.h"
#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    void AddThemePic(QGraphicsView*, QString);
    ~Settings();

private:
    Ui::Settings *ui;
    void PointTheme();
public slots:
    void ChangeTheme();
};

#endif // SETTINGS_H
