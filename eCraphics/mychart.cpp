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

    if(pSeries != nullptr)
        delete[] pSeries;

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
    pChart->setTheme(static_cast<QChart::ChartTheme>(MyConfig::GetConfigure().themID)); //!!!!!!!! enable choose
}

QtCharts::QChartView* MyChartClassic::GetChartWidget()
{
    return pChartWidget;
}

int MyChartClassic::GetChartLen(int daysCount)
{
    return this->chartStep * daysCount;
}

MyChartClassic::~MyChartClassic()
{
    delete[] this->pSeries;
    delete this->pAxisY;
    delete this->pAxisX;
    //delete this->pChart;//pChartWidget delet this memory too;
    delete this->pChartWidget;
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

    this->operatorsCount = pOperatorsName->count();

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
    pCategoryAxis->setTitleText("Дата");

    pChart->createDefaultAxes();
    pChart->setAxisX(pCategoryAxis, pSeries);
    pChart->axisY()->setTitleText("Кількість дзвінків");

    return true;
}

void MyChartBar::SetChartView()
{
    pChartWidget->setRenderHint(QPainter::Antialiasing);
    pChart->setAnimationOptions(QChart::AllAnimations); //!!!!!!!!! enable choose
    pChart->setTheme(static_cast<QChart::ChartTheme>(MyConfig::GetConfigure().themID)); //!!!!!!!! enable choose
}

QtCharts::QChartView* MyChartBar::GetChartWidget()
{
    return pChartWidget;
}

int MyChartBar::GetChartLen(int daysCount)
{
    int chartBlock = this->chartStep * this->operatorsCount;

    if(chartBlock < 100)
        return 100 * daysCount;
    else
        return chartBlock * daysCount;
}

MyChartBar::~MyChartBar()
{
    delete pCategoryAxis;
    delete pSeries;
    //delete pChart;//pChartWidget delet this memory too;
    delete pChartWidget;
}

/***************************************************************************************/
