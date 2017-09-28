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

    //AddThemePic(ui->graphicsView_9, "");
    //AddThemePic(ui->graphicsView_10, "");
    //AddThemePic(ui->graphicsView_11, "");

    setWindowFlags (windowFlags() & ~Qt::WindowContextHelpButtonHint);


    //CONNECT BLOCK
    connect(ui->EXIT, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->SAVE, SIGNAL(clicked(bool)), this, SLOT(ChangeTheme()));
    //

    this->PointTheme();
}

void Settings::AddThemePic(QGraphicsView* view, QString picAlias)
{
    QGraphicsScene* pScene = new QGraphicsScene(view);

    view->setScene(pScene);

    view->scene()->addPixmap(QPixmap::fromImage(QImage(picAlias)).scaled(view->sceneRect().width(), view->sceneRect().height()));
}

void Settings::ChangeTheme()
{
    if(ui->radioButton->isChecked()) //ChartThemeLight
        chartTheme = 0;
    else if(ui->radioButton_2->isChecked()) //ChartThemeQt
        chartTheme = 7;
    else if(ui->radioButton_3->isChecked()) //ChartThemeBlueIcy
        chartTheme = 6;
    else if(ui->radioButton_4->isChecked()) //ChartThemeBlueNcs
        chartTheme = 4;
    else if(ui->radioButton_5->isChecked()) //ChartThemeBlueCerulean
        chartTheme = 1;
    else if(ui->radioButton_6->isChecked()) //ChartThemeBrownSand
        chartTheme = 3;
    else if(ui->radioButton_7->isChecked()) //ChartThemeDark
        chartTheme = 2;
    else if(ui->radioButton_8->isChecked()) //ChartThemeHighContrast
        chartTheme = 5;

    this->close();
}

void Settings::PointTheme()
{
    if(chartTheme == 0)
        ui->radioButton->setChecked(true);
    else if(chartTheme == 1)
        ui->radioButton_5->setChecked(true);
    else if(chartTheme == 2)
        ui->radioButton_7->setChecked(true);
    else if(chartTheme == 3)
        ui->radioButton_6->setChecked(true);
    else if(chartTheme == 4)
        ui->radioButton_4->setChecked(true);
    else if(chartTheme == 5)
        ui->radioButton_8->setChecked(true);
    else if(chartTheme == 6)
        ui->radioButton_3->setChecked(true);
    else if(chartTheme == 7)
        ui->radioButton_2->setChecked(true);
}

Settings::~Settings()
{
    delete ui;
}
