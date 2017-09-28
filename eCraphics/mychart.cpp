#include "mychart.h"

MyChartClassic::MyChartClassic()
{
    pChart = new QtCharts::QChart();

    pAxisX = new QtCharts::QDateTimeAxis;

    pAxisY = new QtCharts::QValueAxis;

    pChartWidget = new QtCharts::QChartView(pChart);
}

void MyChartClassic::SetDateAxis(int count)
{
    pAxisX->setTickCount(pSeries[0].count());
    pAxisX->setFormat("yyyy/MM/dd");
    pAxisX->setTitleText("Дата");
    pChart->addAxis(pAxisX, Qt::AlignBottom);

    for(int i = 0; i < count; i++)
        pSeries[i].attachAxis(pAxisX);
}

void MyChartClassic::SetValueAxis(int count, float minRange, float maxRange)
{
    pAxisY->setTitleText("Кількість дзвінків");
    pAxisY->setLabelFormat("%i");

    if(maxRange <= 29)
        pAxisY->setTickCount(maxRange);
    else
        pAxisY->setTickCount(29);

    pAxisY->setMin(minRange);
    pAxisY->setMax(maxRange);

    pChart->addAxis(pAxisY, Qt::AlignLeft);

    for(int i = 0; i < count; i++)
        pSeries[i].attachAxis(pAxisY);
}

bool MyChartClassic::SetChart(const QVector<QPair<QDateTime, float>*> *pDataVector, const QStringList *pOperatorsName)
{
    SetChartView();

    pSeries = new QtCharts::QLineSeries[pOperatorsName->count()];

    QVector<float> minMaxRange;

    for(int i = 0; i < pOperatorsName->count(); i++)
    {
        for(int j = 0; j < pDataVector[i].count(); j++)
        {
            QDateTime xDate = pDataVector[i].at(j)->first;
            float yValue = pDataVector[i].at(j)->second;
            minMaxRange += pDataVector[i].at(j)->second;

            pSeries[i].append(xDate.toMSecsSinceEpoch(), yValue);
        }

        pSeries[i].setName(pOperatorsName->at(i));
    }

    for(int i = 0; i < pOperatorsName->count(); i++)
        pChart->addSeries(&pSeries[i]);

    /****************************/

    qSort(minMaxRange.begin(), minMaxRange.end());

    /****************************/


    SetValueAxis(pOperatorsName->count(), minMaxRange.first(), minMaxRange.last());
    SetDateAxis(pOperatorsName->count());

    return true;
}

void MyChartClassic::SetChartView()
{
    pChartWidget->setRenderHint(QPainter::Antialiasing);
    pChart->setAnimationOptions(QChart::AllAnimations);
    pChart->setTheme(static_cast<QChart::ChartTheme>(chartTheme)); //!!!!!!!! enable choose
}

QtCharts::QChartView* MyChartClassic::GetChartWidget()
{
    return pChartWidget;
}

MyChartClassic::~MyChartClassic()
{
    delete this->pAxisY;

    delete this->pAxisX;

    //delete this->pChartWidget;

    //delete this->pChart;
}

/***************************************************************************************/

MyChartBar::MyChartBar()//!!!
{
    pChart = new QtCharts::QChart();

    pSeries = new QtCharts::QBarSeries;

    pCategoryAxis = new QtCharts::QBarCategoryAxis;

    pChartWidget = new QtCharts::QChartView(pChart);
}

bool MyChartBar::SetChart(const QVector<QPair<QDateTime, float>*> *pDataVector, const QStringList *pOperatorsName)
{
    SetChartView();

    QVector<QtCharts::QBarSet*> vectorOperators;

    for(int i = 0; i < pOperatorsName->count(); i++)
    {
        vectorOperators += new QtCharts::QBarSet(pOperatorsName->at(i));

        for(int j = 0; j < pDataVector[i].count(); j++)
        {
            vectorOperators.at(i)->append(pDataVector[i].at(j)->second);

            if(i == 0)
                date << pDataVector[i].at(j)->first.toString("yyyy/MM/dd");
        }

        pSeries->append(vectorOperators.at(i));
    }

    pChart->addSeries(pSeries);

    pCategoryAxis->append(date);

    pChart->createDefaultAxes();
    pChart->setAxisX(pCategoryAxis, pSeries);

    return true;
}

void MyChartBar::SetChartView()
{
    pChartWidget->setRenderHint(QPainter::Antialiasing);
    pChart->setAnimationOptions(QChart::AllAnimations); //!!!!!!!!! enable choose
    pChart->setTheme(static_cast<QChart::ChartTheme>(chartTheme)); //!!!!!!!! enable choose
}

QtCharts::QChartView* MyChartBar::GetChartWidget()
{
    return pChartWidget;
}

MyChartBar::~MyChartBar()
{
    //delete this->pAxisY;

    //delete this->pAxisX;

    //delete this->pChartWidget;

    //delete this->pChart;
}

/***************************************************************************************/

/*MyChart::MyChart()
{
    pChart = new QtCharts::QChart();

    pAxisX = new QtCharts::QDateTimeAxis;

    pAxisY = new QtCharts::QValueAxis;

    pChartWidget = new QtCharts::QChartView(pChart);
}

void MyChart::SetDateAxis(int count)
{
    pAxisX->setTickCount(pSeries[0].count());
    pAxisX->setFormat("yy/MM/dd");
    pAxisX->setTitleText("Дата");
    pChart->addAxis(pAxisX, Qt::AlignBottom);

    for(int i = 0; i < count; i++)
        pSeries[i].attachAxis(pAxisX);
}

void MyChart::SetValueAxis(int count, float minRange, float maxRange)
{
    pAxisY->setTitleText("Кількість дзвінків");
    pAxisY->setLabelFormat("%i");

    if(maxRange <= 29)
        pAxisY->setTickCount(maxRange);
    else
        pAxisY->setTickCount(29);

    pAxisY->setMin(minRange);
    pAxisY->setMax(maxRange);

    pChart->addAxis(pAxisY, Qt::AlignLeft);

    for(int i = 0; i < count; i++)
        pSeries[i].attachAxis(pAxisY);
}

bool MyChart::SetChart(const QVector<QPair<QDateTime, float>*> *pDataVector, const QStringList *pOperatorsName, float minRange, float maxRange)
{
    SetChartView();

    pSeries = new QtCharts::QLineSeries[pOperatorsName->count()];

    for(int i = 0; i < pOperatorsName->count(); i++)
    {
        for(int j = 0; j < pDataVector[i].count(); j++)
        {
            QDateTime xDate = pDataVector[i].at(j)->first;
            float yValue = pDataVector[i].at(j)->second;

            pSeries[i].append(xDate.toMSecsSinceEpoch(), yValue);
        }

        pSeries[i].setName(pOperatorsName->at(i));
    }

    for(int i = 0; i < pOperatorsName->count(); i++)
        pChart->addSeries(&pSeries[i]);

    SetValueAxis(pOperatorsName->count(), minRange, maxRange);
    SetDateAxis(pOperatorsName->count());

    return true;
}

void MyChart::SetChartView()
{
    pChartWidget->setRenderHint(QPainter::Antialiasing);
}

QtCharts::QChartView* MyChart::GetChartWidget()
{
    return pChartWidget;
}

MyChart::~MyChart()
{
    delete this->pAxisY;

    delete this->pAxisX;

    //delete this->pChartWidget;

    //delete this->pChart;
}*/
