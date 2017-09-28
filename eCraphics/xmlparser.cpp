#include "xmlparser.h"

MyXmlParser::MyXmlParser()
{
    this->xmlReader = new QXmlStreamReader;
    //this->projects = nullptr;
}

MyXmlParser::MyXmlParser(QString filename)
{
    this->xmlReader = new QXmlStreamReader;
    //this->projects = nullptr;

    this->xmlFileName = filename;
}

void MyXmlParser::SetXmlFileName(QString filename)
{
    this->xmlFileName = filename;
}

QXmlStreamReader* MyXmlParser::GetXmlReader()
{
    return this->xmlReader;
}

QVector<XmlProject>* MyXmlParser::GetXmlProject()
{
    return &this->projects;
}

void MyXmlParser::ParseXml()
{
    QFile fileXml;
    QString icoData;

    if(!this->xmlFileName.isEmpty())
        fileXml.setFileName(this->xmlFileName);
    else
    {
        QMessageBox::warning(nullptr, "Помилка", "Не вказаний шлях до файлу проектів", QMessageBox::Ok);
        ToLog("Don't set xml file name");
        return;
    }

    if(!QFile::exists(this->xmlFileName))
    {
        QMessageBox::warning(nullptr, "Помилка", "Не вдалося знайти файл проектів", QMessageBox::Ok);
        ToLog("XML file with projects doesn't exist");
        return;
    }

    if(!fileXml.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(nullptr, "Помилка", "Не вдалося відкрити файл проектів", QMessageBox::Ok);
        ToLog("XML file with projects didn't opened");
        return;
    }

    this->xmlReader->setDevice(&fileXml);

    while(!this->xmlReader->atEnd())
    {
        xmlReader->readNext();

        if(xmlReader->isStartElement())
        {
            if(xmlReader->name() == "icon")
            {
                icoData = xmlReader->attributes().value("data").toString();
            }
            if(xmlReader->name() == "project")
            {
                XmlProject *addProject = new XmlProject;
                addProject->caption = xmlReader->attributes().value("caption").toString();
                addProject->db = xmlReader->attributes().value("db").toString();
                addProject->type = xmlReader->attributes().value("type").toString();
                addProject->ico = icoData;

                this->projects.append(*addProject);
            }
        }
        if(xmlReader->isEndElement())
        {
            if(xmlReader->name() == "icon")
            {
                icoData.clear();
            }
        }
    }

    if(this->xmlReader->hasError())
        ToLog("XML parsing error: " + this->xmlReader->errorString());
}

MyXmlParser::~MyXmlParser()
{
    delete this->xmlReader;
}
