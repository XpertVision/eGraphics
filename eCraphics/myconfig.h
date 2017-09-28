#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <QObject>

class MyConfig : public QObject
{
    Q_OBJECT
public:
    explicit MyConfig(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MYCONFIG_H