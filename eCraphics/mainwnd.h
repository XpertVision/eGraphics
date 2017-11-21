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
    DB *db = nullptr;

    MyChartBase *pChart = nullptr;

    MyXmlParser myXml;

    MyUpdater updater;

    QString project;
    QString dbTable;
    QString projectType;
    QString dbCryteria;
    QString dbCryteriaCast;
    QString dbCryteriaType;
    QVector<CryteriaData> cryteria;

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

    QVector<XmlProject>* parsedXmlProject;

private slots:
    void EnableDatePeriod();
    void SelectProject();
    void SelectOperator();
    void SelectParam();
    void ButtonCheckedParam();
    void DrawGraphic();
    void SaveGraphic();

    void OpenSettings();
    void OpenChangeList();
    void OpenAuthorization();

    void SetDisabledBaseBlock(bool);
};

#endif // MAINWND_H
