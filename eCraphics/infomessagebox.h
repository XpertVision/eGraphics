#ifndef INFOMESSAGEBOX_H
#define INFOMESSAGEBOX_H

#include "headers.h"

class InfoMessageBox : public QObject
{
    Q_OBJECT
public:
    explicit InfoMessageBox();

signals:

public slots:
    void begin();
    void end();
private:
    QMessageBox *mb;
};

#endif // INFOMESSAGEBOX_H
