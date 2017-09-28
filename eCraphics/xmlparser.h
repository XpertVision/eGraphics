#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "headers.h"

struct XmlProject
{
    QString caption;
    QString db;
    QString type;
    QString ico;
};

class MyXmlParser : public QObject
{
    Q_OBJECT
private:
    QXmlStreamReader *xmlReader;
    QVector<XmlProject> projects;
    QString xmlFileName;
public:
    MyXmlParser();
    MyXmlParser(QString);
    ~MyXmlParser();

    void SetXmlFileName(QString);

    QXmlStreamReader* GetXmlReader();
    QVector<XmlProject>* GetXmlProject();

    void ParseXml();
};

#endif // XMLPARSER_H
