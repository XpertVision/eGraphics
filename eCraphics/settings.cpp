#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    AddThemePic(ui->graphicsView, ":/THEME_LIGHT_IMG");
    AddThemePic(ui->graphicsView_2, ":/THEME_QT_IMG");
    AddThemePic(ui->graphicsView_3, ":/THEME_BLUEICY_IMG");
    AddThemePic(ui->graphicsView_4, ":/THEME_BLUENCS_IMG");
    AddThemePic(ui->graphicsView_5, ":/THEME_BLUECERULEAN_IMG");
    AddThemePic(ui->graphicsView_6, ":/THEME_BROWNSAND_IMG");
    AddThemePic(ui->graphicsView_7, ":/THEME_DARK_IMG");
    AddThemePic(ui->graphicsView_8, ":/THEME_HIGHTCONTRAST_IMG");

    AddThemePic(ui->graphicsView_9, ":/TYPE_CLASSIC_IMG");
    AddThemePic(ui->graphicsView_10, ":/THEME_LIGHT_IMG");
    AddThemePic(ui->graphicsView_11, ":/THEME_LIGHT_IMG");

    setWindowFlags (windowFlags() & ~Qt::WindowContextHelpButtonHint);


    //CONNECT BLOCK
    connect(ui->EXIT, &QPushButton::clicked, this, &Settings::close);
    connect(ui->SAVE, &QPushButton::clicked, this, &Settings::GetSettingsToConfig);

    connect(ui->MaximizeWindow, &QCheckBox::clicked, ui->LastWindowSize, &QCheckBox::setDisabled);
    //
    QButtonGroup* pMultChartGroup = new QButtonGroup(this);
    QButtonGroup* pSinglChartGroup = new QButtonGroup(this);

    pMultChartGroup->addButton(ui->radioMultType_0);
    pMultChartGroup->addButton(ui->radioMultType_1);

    pSinglChartGroup->addButton(ui->radioSinglType_0);


    this->SetSettingsFromConfig();
}

void Settings::AddThemePic(QGraphicsView* view, QString picAlias)
{
    QGraphicsScene* pScene = new QGraphicsScene(view);

    view->setScene(pScene);

    view->scene()->addPixmap(QPixmap::fromImage(QImage(picAlias)).scaled(view->sceneRect().width(), view->sceneRect().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void Settings::SetTheme()
{
    int chartTheme = MyConfig::GetConfigure().themID;

    if(chartTheme == 0)
        ui->radioTheme_0->setChecked(true);
    else if(chartTheme == 1)
        ui->radioTheme_5->setChecked(true);
    else if(chartTheme == 2)
        ui->radioTheme_7->setChecked(true);
    else if(chartTheme == 3)
        ui->radioTheme_6->setChecked(true);
    else if(chartTheme == 4)
        ui->radioTheme_4->setChecked(true);
    else if(chartTheme == 5)
        ui->radioTheme_8->setChecked(true);
    else if(chartTheme == 6)
        ui->radioTheme_3->setChecked(true);
    else if(chartTheme == 7)
        ui->radioTheme_2->setChecked(true);
}

void Settings::SetLang()
{

}

void Settings::SetCharts()
{
    int singlChartType = MyConfig::GetConfigure().singlChartID;
    int multChartType = MyConfig::GetConfigure().multChartID;

    if(singlChartType == 0)
        ui->radioSinglType_0->setChecked(true);

    switch(multChartType)
    {
    case 0: ui->radioMultType_0->setChecked(true);
        break;
    case 1: ui->radioMultType_1->setChecked(true);
        break;
    default: break;
    }
}

void Settings::SetSizeState()
{
    ConfData data = MyConfig::GetConfigure();

    if(data.enableWindowSize == 1)
        ui->LastWindowSize->setChecked(true);

    if(data.enableFullScreen == 1)
    {
        ui->MaximizeWindow->setChecked(true);
        ui->LastWindowSize->setDisabled(true);
    }
}

void Settings::SetSettingsFromConfig()
{
    this->SetSizeState();
    this->SetLang();
    this->SetCharts();
    this->SetTheme();
}

void Settings::GetFromLang()
{

}

void Settings::GetFromTheme()
{
    ConfData data = MyConfig::GetConfigure();
    if(ui->radioTheme_0->isChecked()) //ChartThemeLight
        data.themID = 0;
    else if(ui->radioTheme_2->isChecked()) //ChartThemeQt
        data.themID = 7;
    else if(ui->radioTheme_3->isChecked()) //ChartThemeBlueIcy
        data.themID = 6;
    else if(ui->radioTheme_4->isChecked()) //ChartThemeBlueNcs
        data.themID = 4;
    else if(ui->radioTheme_5->isChecked()) //ChartThemeBlueCerulean
        data.themID = 1;
    else if(ui->radioTheme_6->isChecked()) //ChartThemeBrownSand
        data.themID = 3;
    else if(ui->radioTheme_7->isChecked()) //ChartThemeDark
        data.themID = 2;
    else if(ui->radioTheme_8->isChecked()) //ChartThemeHighContrast
        data.themID = 5;

    MyConfig::SetConfigure(data);
    MyConfig::SaveConfigure();
}

void Settings::GetFromCharts()
{
    ConfData data = MyConfig::GetConfigure();

    if(ui->radioMultType_0->isChecked())
        data.multChartID = 0;
    else if(ui->radioMultType_1->isChecked())
        data.multChartID = 1;

    if(ui->radioSinglType_0->isChecked())
        data.singlChartID = 0;

    MyConfig::SetConfigure(data);
    MyConfig::SaveConfigure();
}

void Settings::GetSizeState()
{
    ConfData data = MyConfig::GetConfigure();

    if(ui->MaximizeWindow->isChecked())
    {
        data.enableFullScreen = 1;

        if(ui->LastWindowSize->isChecked())
            data.enableWindowSize = 1;
        else
            data.enableWindowSize = 0;
    }
    else if(ui->LastWindowSize->isChecked())
    {
        data.enableFullScreen = 0;
        data.enableWindowSize = 1;
    }
    else
    {
        data.enableFullScreen = 0;
        data.enableWindowSize = 0;
    }

    MyConfig::SetConfigure(data);
    MyConfig::SaveConfigure();
}

void Settings::GetSettingsToConfig()
{
    this->GetSizeState();
    this->GetFromLang();
    this->GetFromCharts();
    this->GetFromTheme();

    this->close();
}

Settings::~Settings()
{
    delete ui->graphicsView->scene();
    delete ui->graphicsView_2->scene();
    delete ui->graphicsView_3->scene();
    delete ui->graphicsView_4->scene();
    delete ui->graphicsView_5->scene();
    delete ui->graphicsView_6->scene();
    delete ui->graphicsView_7->scene();
    delete ui->graphicsView_8->scene();
    //delete ui->graphicsView_9->scene();
    delete ui->graphicsView_10->scene();
    delete ui->graphicsView_11->scene();

    delete ui;
}
