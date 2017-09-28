#ifndef MAINWND_H
#define MAINWND_H

#include "headers.h"

namespace Ui {
class MainWnd;
}

class MainWnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWnd(QWidget *parent = 0);

    void Start();
    void SetList(QStringList &, QListWidget*);

    ~MainWnd();

private:
    void resizeEvent(QResizeEvent* event);

    void ParseProjects();
    void SetProjectParams();

    Ui::MainWnd *ui;

    //VARIABLES
    //
    DB *db = nullptr;
    //MyChart *pChart = nullptr;
    MyChartBase *pChart = nullptr;

    MyXmlParser myXml;

    MyUpdater updater;
    //
    QString project;
    QString dbTable;
    QString projectType;

    QStringList projects;
    QStringList bases;
    QStringList operators;
    QStringList params;
    QDateTime beginDate;
    QDateTime endDate;
    bool withPeriod;
    //
    QStringList baseList;
    QStringList operatorList;
    //
    QStringList queryResult;
    //
    int dayDiff;
    int sceneLen;
    //QVector<float> minMaxRange;
    QVector<XmlProject>* parsedXmlProject;

private slots:
    void EnableDatePeriod();
    void SelectProject();
    void SelectOperator();
    void SelectParam();
    void CheckedList(QListWidgetItem*);
    void ButtonCheckedParam();
    void DrawGraphic();
    void SaveGraphic();

    void OpenSettings();
};

#endif // MAINWND_H
