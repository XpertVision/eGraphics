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

    QVector<CryteriaData> cryteriaVector;

    XmlProject *addProject = nullptr;

    if(!this->xmlFileName.isEmpty())
        fileXml.setFileName(this->xmlFileName);
    else
    {
        QMessageBox::warning(nullptr, "Помилка", "Не вказаний шлях до файлу проектів", QMessageBox::Ok);
        MyLogger::ToLog("Don't set xml file name");
        return;
    }

    if(!QFile::exists(this->xmlFileName))
    {
        QMessageBox::warning(nullptr, "Помилка", "Не вдалося знайти файл проектів", QMessageBox::Ok);
        MyLogger::ToLog("XML file with projects doesn't exist");
        return;
    }

    if(!fileXml.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(nullptr, "Помилка", "Не вдалося відкрити файл проектів", QMessageBox::Ok);
        MyLogger::ToLog("XML file with projects didn't open");
        return;
    }

    this->xmlReader->setDevice(&fileXml);

    while(!this->xmlReader->atEnd())
    {
        if(this->xmlReader->isStartElement())
        {
            if(this->xmlReader->name().toString() == "projectGroup")
                this->xmlReader->readNext();
            else if(this->xmlReader->name().toString() == "icon")
            {
                icoData = this->xmlReader->attributes().value("data").toString();
                this->xmlReader->readNext();
            }
            else if(this->xmlReader->name().toString() == "project")
            {
                addProject = new XmlProject;
                addProject->caption = this->xmlReader->attributes().value("caption").toString();
                addProject->db = this->xmlReader->attributes().value("db").toString();
                addProject->type = this->xmlReader->attributes().value("type").toString();
                addProject->ico = icoData;

                this->xmlReader->readNext();
            }
            else if(this->xmlReader->name().toString() == "list")
            {
                if(this->xmlReader->attributes().value("name").toString() == "cryteria")
                {
                    do
                    {
                        this->xmlReader->readNext();
                    }
                    while(!this->xmlReader->isStartElement());

                    bool isStartElem = this->xmlReader->isStartElement();

                    while(isStartElem)
                    {
                        CryteriaData cryteria;

                        cryteria.caption = this->xmlReader->attributes().value("caption").toString();
                        cryteria.field   = this->xmlReader->attributes().value("field").toString();
                        cryteria.type    = this->xmlReader->attributes().value("type").toString();

                        cryteriaVector.append(cryteria);

                        do
                        {
                            this->xmlReader->readNext();
                        }
                        while(!this->xmlReader->isStartElement() && (this->xmlReader->name().isEmpty() || this->xmlReader->name().toString() == "listItem"));

                        if(this->xmlReader->name().toString() == "listItem")
                            isStartElem = true;
                        else
                            isStartElem = false;
                    }
                }
            }
            else
                this->xmlReader->readNext();
        }
        else if(this->xmlReader->isEndElement())
        {
            if(this->xmlReader->name().toString() == "icon")
                icoData.clear();
            else if(this->xmlReader->name().toString() == "project")
            {
                this->projects.append(*addProject);
                addProject = nullptr;
            }
            else if(this->xmlReader->name().toString() == "list")
            {
                addProject->cryteria = cryteriaVector;
                cryteriaVector.clear();
            }

            this->xmlReader->readNext();
        }
        else
        {
            this->xmlReader->readNext();
        }
    }

    if(this->xmlReader->hasError())
        MyLogger::ToLog("XML parsing error: " + this->xmlReader->errorString());

    fileXml.close();
}

MyXmlParser::~MyXmlParser()
{
    delete this->xmlReader;
}
