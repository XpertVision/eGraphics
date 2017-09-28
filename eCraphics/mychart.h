#ifndef MYCHART_H
#define MYCHART_H

#include "headers.h"

struct OperatorsData
{
    QString operatorName;
    int value;
};

class MyChartBase : public QObject
{
private:
    Q_OBJECT

protected:
    virtual void SetChartView() = 0;
    //virtual void SetDateAxis(int) = 0;
    //virtual void SetValueAxis(int, float, float) = 0;
public:
    MyChartBase(){}

    virtual bool SetChart(const QVector<QPair<QDateTime, float>*>*, const QStringList*) = 0;

    virtual QtCharts::QChartView* GetChartWidget() = 0;

    virtual ~MyChartBase(){}
};

class MyChartClassic : public MyChartBase
{
private:
    QtCharts::QChart *pChart = nullptr;
    QtCharts::QDateTimeAxis *pAxisX = nullptr;
    QtCharts::QValueAxis *pAxisY = nullptr;
    QtCharts::QChartView *pChartWidget = nullptr;

    QtCharts::QLineSeries *pSeries = nullptr;

    virtual void SetChartView();
    virtual void SetDateAxis(int);
    virtual void SetValueAxis(int, float, float);
public:

    virtual bool SetChart(const QVector<QPair<QDateTime, float>*>*, const QStringList*);

    virtual QtCharts::QChartView* GetChartWidget();

    MyChartClassic();
    virtual ~MyChartClassic();
};

class MyChartBar : public MyChartBase
{
private:
    QtCharts::QChart *pChart = nullptr;
    QStringList date;
    QtCharts::QBarSeries *pSeries = nullptr;
    //QtCharts::QBarSet* pOperators = nullptr;
    QtCharts::QBarCategoryAxis* pCategoryAxis = nullptr;
    QtCharts::QChartView *pChartWidget = nullptr;

    //QtCharts::QDateTimeAxis *pAxisX = nullptr;
    //QtCharts::QValueAxis *pAxisY = nullptr;


    //QtCharts::QLineSeries *pSeries = nullptr;
    virtual void SetChartView();
    //virtual void SetDateAxis(int);
    //virtual void SetValueAxis(int, float, float);
public:
    virtual bool SetChart(const QVector<QPair<QDateTime, float>*>*, const QStringList*);

    virtual QtCharts::QChartView* GetChartWidget();

    MyChartBar();
    virtual ~MyChartBar();
};

/***************************************************************************************************************/


/*class MyChart : public QObject
{
    Q_OBJECT
private:
    QtCharts::QChart *pChart = nullptr;
    QtCharts::QDateTimeAxis *pAxisX = nullptr;
    QtCharts::QValueAxis *pAxisY = nullptr;
    QtCharts::QChartView *pChartWidget = nullptr;

    QtCharts::QLineSeries *pSeries = nullptr;

    void SetChartView();
public:
    MyChart();

    void SetDateAxis(int);
    void SetValueAxis(int, float, float);

    bool SetChart(const QVector<QPair<QDateTime, float>*>*, const QStringList*, float, float);

    QtCharts::QChartView* GetChartWidget();

    ~MyChart();
};*/



#endif // MYCHART_H
