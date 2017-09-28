#include "mainwnd.h"
#include "ui_mainwnd.h"

MainWnd::MainWnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWnd)
{
    InitLog("log", QDir::current().path() + "/LOG/");
    ToLog("Program start");
    ToLog("Setup UI");
    ui->setupUi(this);

    ToLog("Connect SIGNALS-SLOTS");

    connect(ui->DatePeriodEnable, SIGNAL(stateChanged(int)), this, SLOT(EnableDatePeriod()));

    connect(ui->ProjectNameCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(SelectProject()));
    connect(ui->ParamCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(SelectParam()));


    connect(ui->BaseList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(CheckedList(QListWidgetItem*)));
    connect(ui->OperatorList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(CheckedList(QListWidgetItem*)));
    connect(ui->ParamList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(CheckedList(QListWidgetItem*)));

    connect(ui->SetBaseButton, SIGNAL(clicked(bool)), this, SLOT(ButtonCheckedParam()));
    connect(ui->SetOperatorButton, SIGNAL(clicked(bool)), this, SLOT(ButtonCheckedParam()));
    connect(ui->SetParamButton, SIGNAL(clicked(bool)), this, SLOT(ButtonCheckedParam()));
    connect(ui->DrawGraphic, SIGNAL(clicked(bool)), this, SLOT(DrawGraphic()));

    //
    ui->MainToolBar->addAction(QIcon(":/SAVE_ICO2"), "Зберегти", this, SLOT(SaveGraphic()));
    ui->MainToolBar->actions().at(0)->setDisabled(true);

    ui->MainToolBar->addAction(QIcon(":/SETTINGS_ICO"), "Налаштування", this, SLOT(OpenSettings()));
    ui->MainToolBar->addAction(QIcon(":/UPDATELIST_ICO"), "Список змін", this, SLOT(close()));

    QWidget* pToolBarSpacer = new QWidget();
    pToolBarSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->MainToolBar->addWidget(pToolBarSpacer);

    ui->MainToolBar->addAction(QIcon(":/CLOSE_ICO"), "Вихід", this, SLOT(close()));

    //ui->MainToolBar->setStyleSheet("QToolBar { background-color: rgb(205, 183, 158) }");
    //this->setStyleSheet("QMainWindow { background-color: rgb(205, 183, 158) }");
    //

    this->db = new DB;
}

void MainWnd::Start()
{
    if(this->updater.Update())
    {
        ToLog("Start update");
        QProcess::startDetached("eGraphics.exe");
        ToLog("End update");
        exit(0);
    }

    this->ParseProjects();

    ui->ProjectNameCombo->addItems(this->projects);

    QByteArray byteArr;
    QImage imageIco;
    for(int i = 1; i < ui->ProjectNameCombo->count(); i++)
    {
        byteArr = QByteArray::fromBase64(this->myXml.GetXmlProject()->at(i - 1).ico.toUtf8());
        imageIco = QImage::fromData(byteArr, "png");
        ui->ProjectNameCombo->setItemIcon(i, QPixmap::fromImage(imageIco));
    }
    ui->ProjectNameCombo->setEnabled(true);

    QThread *thr = new QThread();

    QMessageBox mbConnect;
    mbConnect.setText("Зачекайте. Йде з'єднання із сервером бази даних.");
    mbConnect.setWindowTitle("Інформація");
    mbConnect.setStandardButtons(QMessageBox::NoButton);

    db->moveToThread(thr);
    connect(thr, SIGNAL(started()), db, SLOT(StartConnect()));
    connect(db, SIGNAL(ConnectFinish()), thr, SLOT(quit()));
    connect(thr, SIGNAL(finished()), &mbConnect, SLOT(reject()));

    db->SetMode(0);

    thr->start();

    mbConnect.show();
    mbConnect.exec();

    if(!db->GetConnectResult())
    {
        QMessageBox mbErrorConnect;
        mbErrorConnect.setText("Проблеми з підключенням до серверу баз данних. Роботу программи завершено!");
        mbErrorConnect.setWindowTitle("Інформація");
        mbErrorConnect.show();
        mbErrorConnect.exec();

        delete thr;

        ToLog("Connecting to DataBase - FALSE");

        exit(0);
    }
    delete thr;

    ui->PeriodCalendarStart->setDate(QDate::currentDate());
    ui->PeriodCalendarEnd->setDate(QDate::currentDate());
}

void MainWnd::SetList(QStringList &itemList, QListWidget* ListWidget)
{
    ListWidget->clear();

    for(int i = 0; i < itemList.size(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem(itemList[i], ListWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

MainWnd::~MainWnd()
{
    if(db != nullptr)
        delete db;

    delete ui;

    ToLog("Program end");
    CloseLog();
}

void MainWnd::EnableDatePeriod()
{
    if(ui->DatePeriodEnable->isChecked())
        ui->PeriodCalendarEnd->setEnabled(true);
    else
        ui->PeriodCalendarEnd->setEnabled(false);
}

void MainWnd::SelectProject()
{
    project = ui->ProjectNameCombo->currentText();
    if(project == "")
    {
        ui->BaseList->clear();
        ui->BaseList->setEnabled(false);

        ui->SetBaseButton->setDisabled(true);
        ui->SetOperatorButton->setDisabled(true);
        ui->SetParamButton->setDisabled(true);

        ui->DatePeriodEnable->setDisabled(true);
        ui->DrawGraphic->setDisabled(true);
        ui->MainToolBar->actions().at(0)->setDisabled(true);
        ui->PeriodCalendarStart->setDisabled(true);
        ui->PeriodCalendarEnd->setDisabled(true);
    }
    else
    {
        this->SetProjectParams();

        if(this->projectType != "inbound")
        {
            ui->BaseList->setEnabled(true);
            ui->BaseList->clear();

            QThread *thr = new QThread();

            QMessageBox mbQuery;
            mbQuery.setText("Зачекайте. Йде обробка запиту.");
            mbQuery.setWindowTitle("Інформація");
            mbQuery.setStandardButtons(QMessageBox::NoButton);

            db->SetMode(0);

            db->moveToThread(thr);

            db->queryStr = "SELECT RecordBase FROM t_RecordBases WHERE TableName = '" + this->dbTable + "' ORDER BY ImportDate DESC";

            connect(thr, SIGNAL(started()), db, SLOT(StartCustomQuery()));
            connect(db, SIGNAL(QueryFinish()), thr, SLOT(quit()));
            connect(thr, SIGNAL(finished()), &mbQuery, SLOT(reject()));

            thr->start();

            mbQuery.show();
            mbQuery.exec();

            delete thr;

            queryResult = db->queryResult;
            queryResult.insert(0, "Виділити усі");

            SetList(queryResult, ui->BaseList);

            ui->OperatorList->setDisabled(true);
        }
        else
        {
            ui->BaseList->setEnabled(false);
            ui->BaseList->clear();
            ui->SetBaseButton->setDisabled(true);
            emit ui->SetBaseButton->clicked(true);
        }
    }

    ui->ParamCombo->setDisabled(true);
    ui->ParamList->setDisabled(true);
    ui->DatePeriodEnable->setChecked(false);
    ui->DatePeriodEnable->setDisabled(true);
    ui->PeriodCalendarStart->setDisabled(true);
    ui->PeriodCalendarEnd->setDisabled(true);
    ui->DrawGraphic->setDisabled(true);
    ui->MainToolBar->actions().at(0)->setDisabled(true);
}

void MainWnd::SelectOperator()
{
    ui->OperatorList->setEnabled(true);
    ui->OperatorList->clear();

    QThread *thr = new QThread();

    QMessageBox mbQuery;
    mbQuery.setText("Зачекайте. Йде обробка запиту.");
    mbQuery.setWindowTitle("Інформація");
    mbQuery.setStandardButtons(QMessageBox::NoButton);

    db->SetMode(0);

    db->moveToThread(thr);

    db->table = this->dbTable;

    connect(thr, SIGNAL(started()), db, SLOT(StartCustomQuery()));
    connect(db, SIGNAL(QueryFinish()), thr, SLOT(quit()));
    connect(thr, SIGNAL(finished()), &mbQuery, SLOT(reject()));

    if(this->projectType != "inbound")
    {
        QString whereList;
        whereList.clear();

        for(int i = 0; i < bases.size(); i++)
        {
            if(i == 0)
                whereList += "'";
            else
                whereList += ",'";
            whereList += bases.value(i);
            whereList += "'";
        }

        db->queryStr = "SELECT DISTINCT Owner FROM " + db->table + " WHERE RecordBase in(" + whereList + ") and Owner is not null ORDER BY Owner ASC";
    }
    else
    {
        db->queryStr = "SELECT DISTINCT Owner FROM " + db->table + " WHERE Owner is not null ORDER BY Owner ASC";
    }

    thr->start();

    mbQuery.show();
    mbQuery.exec();

    delete thr;

    queryResult = db->queryResult;
    queryResult.insert(0, "Виділити усі");
    queryResult.insert(1, "УСІ");

    SetList(queryResult, ui->OperatorList);
}

void MainWnd::SelectParam()
{
    if(ui->ParamCombo->currentText() != "")
    {
        QMessageBox mbQuery;
        mbQuery.setText("Зачекайте. Йде обробка запиту.");
        mbQuery.setWindowTitle("Інформація");
        mbQuery.setStandardButtons(QMessageBox::NoButton);

        db->SetMode(0);

        QThread *thr = new QThread();

        db->moveToThread(thr);


        db->table = this->dbTable;

        if(this->projectType != "inbound")
        {
            QString whereList;
            whereList.clear();

            for(int i = 0; i < bases.size(); i++)
            {
                if(i == 0)
                    whereList += "'";
                else
                    whereList += ",'";
                whereList += bases.value(i);
                whereList += "'";
            }

            db->queryStr = "SELECT DISTINCT Result FROM " + db->table + " WHERE Result is not null and RecordBase in (" + whereList + ") ORDER BY Result ASC";
        }
        else
        {
            db->queryStr = "SELECT DISTINCT Result FROM " + db->table + " WHERE Result is not null ORDER BY Result ASC";
        }

        connect(thr, SIGNAL(started()), db, SLOT(StartCustomQuery()));
        connect(db, SIGNAL(QueryFinish()), thr, SLOT(quit()));
        connect(thr, SIGNAL(finished()), &mbQuery, SLOT(reject()));

        thr->start();

        mbQuery.show();
        mbQuery.exec();

        delete thr;

        queryResult = db->queryResult;
        queryResult.insert(0, "Виділити усі");

        SetList(queryResult, ui->ParamList);
        ui->ParamList->setEnabled(true);

        ui->PeriodCalendarStart->setDisabled(true);
        ui->PeriodCalendarEnd->setDisabled(true);
        ui->DatePeriodEnable->setDisabled(true);
        ui->DrawGraphic->setDisabled(true);
        ui->MainToolBar->actions().at(0)->setDisabled(true);
    }
    else
    {
        ui->ParamList->setDisabled(true);
        ui->SetParamButton->setDisabled(true);

        ui->PeriodCalendarStart->setDisabled(true);
        ui->PeriodCalendarEnd->setDisabled(true);
        ui->DatePeriodEnable->setChecked(false);
        ui->DatePeriodEnable->setDisabled(true);
        ui->DrawGraphic->setDisabled(true);
        ui->MainToolBar->actions().at(0)->setDisabled(true);
    }
}

void MainWnd::CheckedList(QListWidgetItem* Item)
{
    static bool flagAll(false);
    bool flagActivateButton(false);

    if(Item->listWidget()->item(0)->checkState() == Qt::Checked && flagAll == false)
    {
        for(int i = 1; i < Item->listWidget()->count(); i++)
        {
            Item->listWidget()->item(i)->setCheckState(Qt::Checked);
        }
        flagAll = true;

        flagActivateButton = true;
    }
    else if(Item->listWidget()->item(0)->checkState() == Qt::Unchecked && flagAll == true)
    {
        for(int i = 1; i < Item->listWidget()->count(); i++)
        {
            Item->listWidget()->item(i)->setCheckState(Qt::Unchecked);
        }
        flagAll = false;
    }
    else
    {
        bool isAllCheck(true);
        bool breakCheck(false), breakUncheck(false);
        for(int i = 1; i < Item->listWidget()->count(); i++)
        {
            if(Item->listWidget()->item(i)->checkState() == Qt::Unchecked)
            {
                isAllCheck = false;;
                breakUncheck = true;
            }
            else
            {
                flagActivateButton = true;
                breakCheck = true;
            }

            if(breakCheck == true && breakUncheck == true)
                break;
        }
        if(isAllCheck == true)
        {
            Item->listWidget()->item(0)->setCheckState(Qt::Checked);
            flagAll = true;

            flagActivateButton = true;
        }
        else
        {
            Item->listWidget()->blockSignals(true);
            Item->listWidget()->item(0)->setCheckState(Qt::Unchecked);
            Item->listWidget()->blockSignals(false);
            flagAll = false;
        }
    }

    if(Item->listWidget()->accessibleName() == "BaseList")
    {
        if(flagActivateButton)
            ui->SetBaseButton->setEnabled(true);
        else
            ui->SetBaseButton->setEnabled(false);
    }
    else if(Item->listWidget()->accessibleName() == "OperatorList")
    {
        if(flagActivateButton)
            ui->SetOperatorButton->setEnabled(true);
        else
            ui->SetOperatorButton->setEnabled(false);
    }
    else if(Item->listWidget()->accessibleName() == "ParamList")
    {
        if(flagActivateButton)
            ui->SetParamButton->setEnabled(true);
        else
            ui->SetParamButton->setEnabled(false);
    }
}

void MainWnd::ButtonCheckedParam()
{
    QPushButton *buttonSender = (QPushButton*)sender();

    if(buttonSender->accessibleName() == "SetBaseButton")
    {
        bases.clear();
        for(int i = 0; i < ui->BaseList->count(); i++)
        {
            if(ui->BaseList->item(i)->checkState() == Qt::Checked)
                bases.insert(i, ui->BaseList->item(i)->text());
        }

        ui->SetOperatorButton->setDisabled(true);
        ui->ParamCombo->setDisabled(true);
        ui->ParamList->setDisabled(true);
        ui->SetParamButton->setDisabled(true);
        ui->PeriodCalendarStart->setDisabled(true);
        ui->DatePeriodEnable->setChecked(false);
        ui->DatePeriodEnable->setDisabled(true);
        ui->DrawGraphic->setDisabled(true);
        ui->MainToolBar->actions().at(0)->setDisabled(true);

        SelectOperator();
    }
    else if(buttonSender->accessibleName() == "SetOperatorButton")
    {
        operators.clear();
        for(int i = 0; i < ui->OperatorList->count(); i++)
        {
            if(ui->OperatorList->item(i)->checkState() == Qt::Checked)
            {
                if(ui->OperatorList->item(i)->text() != "Виділити усі")
                    operators.insert(i, ui->OperatorList->item(i)->text());
            }
        }
        QStringList list;

        list.append("");
        list.append("Результат");
        ui->ParamCombo->clear();
        ui->ParamCombo->addItems(list);
        ui->ParamCombo->setEnabled(true);


        ui->ParamList->setDisabled(true);
        ui->SetParamButton->setDisabled(true);
        ui->PeriodCalendarStart->setDisabled(true);
        ui->DatePeriodEnable->setChecked(false);
        ui->DatePeriodEnable->setDisabled(true);
        ui->DrawGraphic->setDisabled(true);
        ui->MainToolBar->actions().at(0)->setDisabled(true);
    }
    else if(buttonSender->accessibleName() == "SetParamButton")
    {
        params.clear();
        for(int i = 0; i < ui->ParamList->count(); i++)
        {
            if(ui->ParamList->item(i)->checkState() == Qt::Checked)
                params.insert(i, ui->ParamList->item(i)->text());
        }

        ui->PeriodCalendarStart->setEnabled(true);
        ui->DatePeriodEnable->setEnabled(true);

        ui->DrawGraphic->setEnabled(true);
    }
}

void MainWnd::DrawGraphic()
{
    this->withPeriod = ui->DatePeriodEnable->isChecked();
    this->beginDate = ui->PeriodCalendarStart->dateTime();
    if(this->withPeriod)
    {
        this->dayDiff = ui->PeriodCalendarStart->date().daysTo(ui->PeriodCalendarEnd->date());
        if(this->dayDiff <= 0)
        {
            this->dayDiff = 1;
            this->endDate = this->beginDate;
        }
        else
        {
            this->endDate = ui->PeriodCalendarEnd->dateTime();
            dayDiff++;
        }

    }
    else
    {
        this->endDate = ui->PeriodCalendarStart->dateTime();
        dayDiff = 1;
    }

    QVector<QPair<QDateTime, float>*> *vec = new QVector<QPair<QDateTime, float>*>[this->operators.count()];
    QPair<QDateTime, float> **pair = new QPair<QDateTime, float>*[this->operators.count()];

    QMessageBox mbQuery;
    mbQuery.setText("Зачекайте. Йде обробка запиту.");
    mbQuery.setWindowTitle("Інформація");
    mbQuery.setStandardButtons(QMessageBox::NoButton);

    QThread *thr = new QThread();

    db->moveToThread(thr);

    this->queryResult.clear();

    for(int i = 0; i < this->operators.count(); i++)
    {
        db->SetMode(1);

        db->table = project;
        db->queryStr = "set nocount on DECLARE @STEPDATE datetime ";
        db->queryStr += "DECLARE @ENDDATE datetime ";
        db->queryStr += "DECLARE @TMPTable table(DATE nvarchar(30), COUNT int)";
        db->queryStr += "SET @STEPDATE = '";
        db->queryStr += this->beginDate.toString("yyyy/MM/dd");
        db->queryStr += "' SET @ENDDATE = '";
        db->queryStr += this->endDate.toString("yyyy/MM/dd");
        db->queryStr += "' WHILE(@STEPDATE) <= @ENDDATE BEGIN ";
        db->queryStr += "INSERT INTO @TMPTable SELECT CONVERT(nvarchar(30),  @STEPDATE, 104) AS DATE, COUNT(*) AS COUNT FROM ";
        db->queryStr += this->dbTable;
        db->queryStr += " WHERE CallDate BETWEEN @STEPDATE AND @STEPDATE + 1 ";

        if(this->operators[i] == "УСІ")
        {
            db->queryStr += "AND Owner is not null ";
        }
        else
        {
            db->queryStr += "AND Owner = '";
            db->queryStr += this->operators[i];
            db->queryStr += "' ";
        }

        if(this->projectType != "inbound")
        {
            db->queryStr += "AND RecordBase in (";

            for(int i = 0; i < bases.count(); i++)
            {
                if(i != 0)
                    db->queryStr += ",";

                db->queryStr += "'";
                db->queryStr += this->bases.at(i);
                db->queryStr += "'";
            }

            db->queryStr += ") ";
        }
        db->queryStr += "AND Result in (";

        for(int i = 0; i < params.count(); i++)
        {
            if(i != 0)
                db->queryStr += ",";

            db->queryStr += "'";
            db->queryStr += this->params.at(i);
            db->queryStr += "'";
        }

        db->queryStr += ") ";

        db->queryStr += "SET @STEPDATE = @STEPDATE + 1 END ";
        db->queryStr += "SELECT * FROM @TMPTable";

        connect(thr, SIGNAL(started()), db, SLOT(StartCustomQuery()));
        connect(db, SIGNAL(QueryFinish()), thr, SLOT(quit()));
        connect(thr, SIGNAL(finished()), &mbQuery, SLOT(reject()));

        thr->start();

        mbQuery.show();
        mbQuery.exec();

        queryResult += db->queryResult;
    }


    for(int i = 0; i < this->operators.count(); i++)
    {
        if(this->dayDiff > 1)
            pair[i] = new QPair<QDateTime, float>[this->dayDiff];
        else if(this->dayDiff == 1)
            pair[i] = new QPair<QDateTime, float>[this->dayDiff + 1];
    }

    int tmp(0);

    for(int i = 0; i < this->operators.count(); i++)
    {
        if(this->dayDiff > 1)
        {
            for (int j = 0; j < this->dayDiff; j++)
            {
                pair[i][j].first = QDateTime::fromString(queryResult[tmp], "dd.MM.yyyy");

                tmp++;
                pair[i][j].second = queryResult[tmp].toFloat();
                tmp++;

                vec[i].push_back(&pair[i][j]);
            }
        }
        else if(this->dayDiff == 1)
        {
            for (int j = 0; j < this->dayDiff; j++)
            {
                pair[i][j].first = QDateTime::fromString(queryResult[tmp], "dd.MM.yyyy");
                pair[i][j + 1].first = QDateTime::fromString(queryResult[tmp], "dd.MM.yyyy");

                tmp++;
                pair[i][j].second = queryResult[tmp].toFloat();
                pair[i][j + 1].second = queryResult[tmp].toFloat();
                tmp++;

                vec[i].push_back(&pair[i][j]);
                vec[i].push_back(&pair[i][j + 1]);
            }
        }
    }

    delete thr;

    if(pChart != nullptr)
        delete pChart;

    if(this->dayDiff == 1)
        pChart = new MyChartBar;
    else if (this->dayDiff > 1)
        pChart = new MyChartClassic;


    pChart->SetChart(vec, &this->operators);

    QGraphicsScene *pScene = new QGraphicsScene(ui->graphicsView);
    pScene->addWidget(pChart->GetChartWidget());

    /*this->sceneLen = 70 * this->dayDiff;
    if(this->sceneLen < 1800)
        this->sceneLen = 700;

    pScene->setSceneRect(0, 0, sceneLen, ui->graphicsView->geometry().height() - 2);
    ui->graphicsView->setScene(pScene);
    pChart->GetChartWidget()->setGeometry(0,0, sceneLen, ui->graphicsView->sceneRect().height() - 2);*/

    this->sceneLen = 70 * this->dayDiff;
    if(this->sceneLen < ui->graphicsView->geometry().width())
        this->sceneLen = ui->graphicsView->geometry().width();

    pScene->setSceneRect(0, 0, sceneLen, ui->graphicsView->geometry().height() - 2);
    ui->graphicsView->setScene(pScene);
    pChart->GetChartWidget()->setGeometry(0,0, sceneLen, ui->graphicsView->sceneRect().height() - 2);

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui->MainToolBar->actions().at(0)->setEnabled(true);


    for(int i=0; i < this->operators.count(); i++)
    {
        delete [] pair[i];
    }

    delete pair;

    delete [] vec;
}

void MainWnd::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   /*if(ui->graphicsView->scene() != nullptr)
   {
       ui->graphicsView->scene()->setSceneRect(0, 0, this->sceneLen, ui->graphicsView->geometry().height() - 2);

       if(pChart->GetChartWidget()->width() < 1800)
           pChart->GetChartWidget()->setGeometry(0, 0, 700, ui->graphicsView->height());
       else
       pChart->GetChartWidget()->setGeometry(0, 0, ui->graphicsView->scene()->width(), ui->graphicsView->height());
   }*/

   if(ui->graphicsView->scene() != nullptr)
   {
        ui->graphicsView->scene()->setSceneRect(0, 0, this->sceneLen, ui->graphicsView->geometry().height() - 2);

        if(pChart->GetChartWidget()->width() < ui->graphicsView->geometry().width())
            pChart->GetChartWidget()->setGeometry(0, 0, ui->graphicsView->geometry().width(), ui->graphicsView->height());
        else
            pChart->GetChartWidget()->setGeometry(0, 0, this->sceneLen, ui->graphicsView->height());
    }
}

void MainWnd::SaveGraphic()
{
    QImage image(ui->graphicsView->scene()->width(), ui->graphicsView->scene()->height(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    ui->graphicsView->scene()->render(&painter);

    QString fileName = QFileDialog::getSaveFileName(this, QString::fromUtf8("Зберегти файл"), QDir::currentPath(), "Зображення (*.png)");

    image.save(fileName);
}

void MainWnd::ParseProjects()
{    
    //this->myXml.SetXmlFileName("//10.1.56.88/Programs/eCall/Applications/eGraphics/Projects/Projects.xml");
    this->myXml.SetXmlFileName("C:/Users/mbezu/Desktop/eg/Projects.xml");
    this->myXml.ParseXml();

    this->parsedXmlProject = this->myXml.GetXmlProject();

    this->projects.append("");
    for(int i = 0; i < this->parsedXmlProject->count(); i++)
    {
        this->projects.append(this->parsedXmlProject->at(i).caption);
    }
}

void MainWnd::SetProjectParams()
{
    for(int i = 0; i < this->parsedXmlProject->count(); i++)
    {
        if(this->parsedXmlProject->at(i).caption == this->project)
        {
            this->dbTable = parsedXmlProject->at(i).db;
            this->projectType = parsedXmlProject->at(i).type;
            break;
        }
    }
}

void MainWnd::OpenSettings()
{
    Settings* pSettingsWnd = new Settings();

    pSettingsWnd->show();
    pSettingsWnd->exec();

    delete pSettingsWnd;
}
