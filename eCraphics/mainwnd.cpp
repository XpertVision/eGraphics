#include "mainwnd.h"
#include "ui_mainwnd.h"

MainWnd::MainWnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWnd)
{
    MyLogger::InitLog("log", QDir::current().path() + "/LOG/");
    MyLogger::ToLog("Program start");
    MyLogger::ToLog("Setup UI");

    ui->setupUi(this);

    MyConfig::InitConfig("config.xml", QDir::current().path() + "/");
    MyConfig::OpenConfigure();

    if(MyConfig::GetConfigure().enableFullScreen == 1)
        this->setWindowState(Qt::WindowMaximized);
    else if(MyConfig::GetConfigure().enableWindowSize)
        this->resize(MyConfig::GetConfigure().width, MyConfig::GetConfigure().height);

    connect(ui->DatePeriodEnable, &QCheckBox::stateChanged, this, &MainWnd::EnableDatePeriod);

    connect(ui->ProjectNameCombo, &QComboBox::currentTextChanged, this, &MainWnd::SelectProject);
    connect(ui->ParamCombo, &QComboBox::currentTextChanged, this, &MainWnd::SelectParam);

    //MyListWidgets
    connect(ui->BaseList, &MyListWidget::itemDoubleClicked, ui->BaseList, &MyListWidget::SetMyCheckState);
    connect(ui->BaseList, &MyListWidget::itemChanged, ui->BaseList, &MyListWidget::CheckedList);
    connect(ui->BaseList, &MyListWidget::ActivateButton, ui->SetBaseButton, &QPushButton::setEnabled);

    connect(ui->OperatorList, &MyListWidget::itemDoubleClicked, ui->OperatorList, &MyListWidget::SetMyCheckState);
    connect(ui->OperatorList, &MyListWidget::itemChanged, ui->OperatorList, &MyListWidget::CheckedList);
    connect(ui->OperatorList, &MyListWidget::ActivateButton, ui->SetOperatorButton, &QPushButton::setEnabled);

    connect(ui->ParamList, &MyListWidget::itemDoubleClicked, ui->ParamList, &MyListWidget::SetMyCheckState);
    connect(ui->ParamList, &MyListWidget::itemChanged, ui->ParamList, &MyListWidget::CheckedList);
    connect(ui->ParamList, &MyListWidget::ActivateButton, ui->SetParamButton, &QPushButton::setEnabled);
    //MyListWidgets

    connect(ui->SetBaseButton, &QPushButton::clicked, this, &MainWnd::ButtonCheckedParam);
    connect(ui->SetOperatorButton, &QPushButton::clicked, this, &MainWnd::ButtonCheckedParam);
    connect(ui->SetParamButton, &QPushButton::clicked, this, &MainWnd::ButtonCheckedParam);
    connect(ui->DrawGraphic, &QPushButton::clicked, this, &MainWnd::DrawGraphic);

    //
    ui->MainToolBar->addAction(QIcon(":/LOGON_ICO"), "Авторизуватися", this, &MainWnd::OpenAuthorization);

    ui->MainToolBar->addAction(QIcon(":/SAVE_ICO"), "Зберегти", this, &MainWnd::SaveGraphic);
    ui->MainToolBar->actions().at(1)->setDisabled(true);

    ui->MainToolBar->addAction(QIcon(":/SETTINGS_ICO"), "Налаштування", this, &MainWnd::OpenSettings);
    ui->MainToolBar->addAction(QIcon(":/UPDATELIST_ICO"), "Список змін", this, &MainWnd::OpenChangeList);

    QWidget* pToolBarSpacer = new QWidget();
    pToolBarSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->MainToolBar->addWidget(pToolBarSpacer);

    ui->MainToolBar->addAction(QIcon(":/CLOSE_ICO"), "Вихід", this, &MainWnd::close);

    this->db = new DB;
}

void MainWnd::Start()
{
    if(this->updater.Update())
    {
        MyLogger::ToLog("Start update");
        QProcess::startDetached("eGraphics.exe");
        MyLogger::ToLog("End update");

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

    db->SetMode(0);

    myUIThreadBlockerConnect uiBlockConnect(db, "Інформація", "Зачекайте. Йде з'єднання із сервером бази даних.");
    uiBlockConnect.Start();

    if(!db->GetConnectResult())
    {
        QMessageBox mbErrorConnect;
        mbErrorConnect.setText("Проблеми з підключенням до серверу баз данних. Роботу программи завершено!");
        mbErrorConnect.setWindowTitle("Інформація");
        mbErrorConnect.show();
        mbErrorConnect.exec();

        MyLogger::ToLog("Connecting to DataBase - FALSE");

        exit(0);
    }

    ui->PeriodCalendarStart->setDate(QDate::currentDate());
    ui->PeriodCalendarEnd->setDate(QDate::currentDate());
}

void MainWnd::SetList(QStringList &itemList, QListWidget* ListWidget)
{
    ListWidget->clear();

    ListWidget->blockSignals(true);
    for(int i = 0; i < itemList.size(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem(itemList[i], ListWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
    ListWidget->blockSignals(false);
}

MainWnd::~MainWnd()
{
    ConfData data = MyConfig::GetConfigure();

    if(data.enableFullScreen == 0 && data.enableWindowSize == 1)
    {
        data.width = this->width();
        data.height = this->height();

        MyConfig::SetConfigure(data);
        MyConfig::SaveConfigure();
    }

    if(db != nullptr)
        delete db;

    delete ui;

    MyLogger::ToLog("Program end");
    MyLogger::CloseLog();
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
    ui->ParamCombo->clear();
    ui->ParamCombo->setDisabled(true);

    ui->OperatorList->clear();
    ui->OperatorList->SetDisabledWithReset(true);

    ui->ParamList->clear();
    ui->ParamList->SetDisabledWithReset(true);

    SetDisabledBaseBlock(false);

    ui->SetOperatorButton->setDisabled(true);
    ui->SetParamButton->setDisabled(true);
    ui->DrawGraphic->setDisabled(true);

    ui->DatePeriodEnable->setDisabled(true);
    ui->PeriodCalendarStart->setDisabled(true);
    ui->PeriodCalendarEnd->setDisabled(true);

    project = ui->ProjectNameCombo->currentText();
    if(project == "")
    {
        ui->BaseList->clear();
        ui->BaseList->SetDisabledWithReset(true);
    }
    else
    {
        this->SetProjectParams();

        if(this->projectType == "outbound")
        {
            ui->BaseList->clear();

            db->SetMode(0);

            db->queryStr = "SELECT RecordBase FROM t_RecordBases WHERE TableName = '" + this->dbTable + "' ORDER BY ImportDate DESC";

            myUIThreadBlockerQuery uiBlockQuery(db, "Інформація", "Зачекайте. Йде обробка запиту.");
            uiBlockQuery.Start();

            if(db->queryResult.count() == 0)
            {
                ui->BaseList->SetDisabledWithReset(true);
                ui->BaseList->ShowListEmpty(":/EMPTY_ANIM");
                ui->SetBaseButton->setDisabled(true);

                return;
            }

            queryResult = db->queryResult;
            queryResult.insert(0, "Виділити усі");

            ui->BaseList->SetEnabledWithReset(true);
            SetList(queryResult, ui->BaseList);
        }
        else if(this->projectType == "inbound")
        {
            SetDisabledBaseBlock(true);

            SelectOperator();
        }
    }
}

void MainWnd::SelectOperator()
{
    ui->OperatorList->SetDisabledWithReset(true);

    db->SetMode(0);
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

        db->queryStr = "SELECT DISTINCT Owner FROM " + db->table + " WHERE RecordBase in(" + whereList + ") and Owner is not null ORDER BY Owner ASC";
    }
    else
    {
        db->queryStr = "SELECT DISTINCT Owner FROM " + db->table + " WHERE Owner is not null ORDER BY Owner ASC";
    }

    myUIThreadBlockerQuery uiBlockQuery(db, "Інформація", "Зачекайте. Йде обробка запиту.");
    uiBlockQuery.Start();

    if(db->queryResult.count() == 0)
    {
        ui->OperatorList->ShowListEmpty(":/EMPTY_ANIM");
        return;
    }

    ui->OperatorList->setEnabled(true);
    queryResult = db->queryResult;
    queryResult.insert(0, "Виділити усі");
    queryResult.insert(1, "УСІ");

    SetList(queryResult, ui->OperatorList);
}

void MainWnd::SelectParam()
{
    if(ui->ParamCombo->currentText() != "")
    {
        for(int i = 0; i < this->cryteria.count(); i++)
        {
            if(this->cryteria.at(i).caption == ui->ParamCombo->currentText())//.first
            {
                dbCryteria = this->cryteria.at(i).field;//.second
                dbCryteriaType = this->cryteria.at(i).type;
                break;
            }
        }

        db->SetMode(0);
        db->table = this->dbTable;

        if(this->dbCryteriaType == "date")
        {
            this->dbCryteriaCast = "CONVERT(nvarchar(10), " + this->dbCryteria + ", 103)";//CONVERT(nvarchar(10), CallDate, 103)
        }
        else
            this->dbCryteriaCast = this->dbCryteria;


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

            db->queryStr = "SELECT DISTINCT " + dbCryteriaCast + " FROM " + db->table + " WHERE Result is not null and RecordBase in (" + whereList + ") ORDER BY " + dbCryteriaCast + " ASC";
        }
        else
        {
            db->queryStr = "SELECT DISTINCT " + dbCryteriaCast + " FROM " + db->table + " WHERE Result is not null ORDER BY " + dbCryteriaCast + " ASC";
        }

        myUIThreadBlockerQuery uiBlockQuery(db, "Інформація", "Зачекайте. Йде обробка запиту.");
        uiBlockQuery.Start();

        if(db->queryResult.count() == 0)
        {
            ui->ParamList->SetDisabledWithReset(true);
            ui->ParamList->ShowListEmpty(":/EMPTY_ANIM");
            ui->SetParamButton->setDisabled(true);
            return;
        }

        queryResult = db->queryResult;
        queryResult.insert(0, "Виділити усі");

        ui->ParamList->SetEnabledWithReset(true);
        SetList(queryResult, ui->ParamList);
    }
    else
    {
        ui->ParamList->clear();
        ui->ParamList->setDisabled(true);
        ui->SetParamButton->setDisabled(true);
    }

    ui->PeriodCalendarStart->setDisabled(true);
    ui->PeriodCalendarEnd->setDisabled(true);
    ui->DatePeriodEnable->setChecked(false);
    ui->DatePeriodEnable->setDisabled(true);
    ui->DrawGraphic->setDisabled(true);
}

void MainWnd::ButtonCheckedParam()
{
    QPushButton *buttonSender = (QPushButton*)sender();

    if(buttonSender->accessibleName() == "SetBaseButton")
    {
        bases.clear();

        if(ui->BaseList->item(0)->checkState() == Qt::Checked)
        {
            for(int i = 1; i < ui->BaseList->count(); i++)
                bases.append(ui->BaseList->item(i)->text());
        }
        else
        {
            for(int i = 1; i < ui->BaseList->count(); i++)
            {
                if(ui->BaseList->item(i)->checkState() == Qt::Checked)
                    bases.append(ui->BaseList->item(i)->text());
            }
        }

        ui->SetOperatorButton->setDisabled(true);
        ui->SetParamButton->setDisabled(true);
        ui->DrawGraphic->setDisabled(true);

        ui->ParamCombo->setDisabled(true);

        ui->ParamList->clear();
        ui->ParamList->setDisabled(true);

        ui->PeriodCalendarStart->setDisabled(true);
        ui->DatePeriodEnable->setChecked(false);
        ui->DatePeriodEnable->setDisabled(true);

        SelectOperator();
    }
    else if(buttonSender->accessibleName() == "SetOperatorButton")
    {
        operators.clear();

        if(ui->OperatorList->item(0)->checkState() == Qt::Checked)
        {
            for(int i = 1; i < ui->OperatorList->count(); i++)
                operators.append(ui->OperatorList->item(i)->text());
        }
        else
        {
            for(int i = 1; i < ui->OperatorList->count(); i++)
            {
                if(ui->OperatorList->item(i)->checkState() == Qt::Checked)
                    operators.append(ui->OperatorList->item(i)->text());
            }
        }

        QStringList list;

        list.append("");

        for(int i = 0; i < this->cryteria.count(); i++)
            list.append(this->cryteria.at(i).caption);//.first


        ui->ParamCombo->clear();
        ui->ParamCombo->addItems(list);
        ui->ParamCombo->setEnabled(true);

        ui->ParamList->clear();
        ui->ParamList->setDisabled(true);

        ui->SetParamButton->setDisabled(true);
        ui->DrawGraphic->setDisabled(true);

        ui->PeriodCalendarStart->setDisabled(true);
        ui->DatePeriodEnable->setChecked(false);
        ui->DatePeriodEnable->setDisabled(true);
    }
    else if(buttonSender->accessibleName() == "SetParamButton")
    {
        params.clear();

        if(ui->ParamList->item(0)->checkState() == Qt::Checked)
        {
            for(int i = 1; i < ui->ParamList->count(); i++)
                params.append(ui->ParamList->item(i)->text());
        }
        else
        {
            for(int i = 1; i < ui->ParamList->count(); i++)
            {
                if(ui->ParamList->item(i)->checkState() == Qt::Checked)
                    params.append(ui->ParamList->item(i)->text());
            }
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

    QStringList allQuery;
    QString queryTmp;

    for(int i = 0; i < this->operators.count(); i++)
    {
        db->SetMode(1);

        db->table = project;
        queryTmp = "set nocount on DECLARE @STEPDATE datetime ";
        queryTmp += "DECLARE @ENDDATE datetime ";
        queryTmp += "DECLARE @TMPTable table(DATE nvarchar(30), COUNT int)";
        queryTmp += "SET @STEPDATE = '" + this->beginDate.toString("yyyy/MM/dd");
        queryTmp += "' SET @ENDDATE = '" + this->endDate.toString("yyyy/MM/dd");
        queryTmp += "' WHILE(@STEPDATE) <= @ENDDATE BEGIN ";
        queryTmp += "INSERT INTO @TMPTable SELECT CONVERT(nvarchar(30),  @STEPDATE, 104) AS DATE, COUNT(*) AS COUNT FROM " + this->dbTable;
        queryTmp += " WHERE CallDate >= @STEPDATE AND CallDate < @STEPDATE + 1 ";

        if(this->operators[i] == "УСІ")
            queryTmp += "AND Owner is not null ";
        else
            queryTmp += "AND Owner = '" + this->operators[i] + "' ";

        if(this->projectType != "inbound")
        {
            queryTmp += "AND RecordBase in (";

            for(int j = 0; j < bases.count(); j++)
            {
                if(j != 0)
                    queryTmp += ",";

                queryTmp += "'" + this->bases.at(j) + "'";
            }

            queryTmp += ") ";
        }

        if(this->dbCryteriaType == "date")
            queryTmp += "AND CONVERT(nvarchar(10), " + dbCryteria + ", 103) in (";
        else
            queryTmp += "AND " + dbCryteria + " in (";

        for(int k = 0; k < params.count(); k++)
        {
            if(k != 0)
                queryTmp += ",";

            queryTmp += "'" + this->params.at(k) + "'";
        }

        queryTmp += ") ";

        queryTmp += "SET @STEPDATE = @STEPDATE + 1 END ";
        queryTmp += "SELECT * FROM @TMPTable";

        allQuery += queryTmp;
        queryTmp.clear();
    }

    myUIThreadBlockerMultQuery uiBlockQuery(db, "Інформація", "Зачекайте. Йде обробка запиту.", allQuery);
    uiBlockQuery.Start();

    this->queryResult.clear();
    this->queryResult = db->queryResult;

    for(int i = 0; i < this->operators.count(); i++)
    {
        if(this->dayDiff > 1)
            pair[i] = new QPair<QDateTime, float>[this->dayDiff];
        else if(this->dayDiff == 1)
            pair[i] = new QPair<QDateTime, float>[this->dayDiff + 1];
    }

    int tmp(0);
    //QPair<QDateTime, float> pair;
    for(int i = 0; i < this->operators.count(); i++)
    {
        if(this->dayDiff > 1)
        {
            for (int j = 0; j < this->dayDiff; j++)
            {
                pair[i][j].first = QDateTime::fromString(queryResult[tmp], "dd.MM.yyyy");
                //pair.first = QDateTime::fromString(queryResult[tmp], "dd.MM.yyyy");
                tmp++;
                pair[i][j].second = queryResult[tmp].toFloat();
                //pair.second = queryResult[tmp].toFloat();
                tmp++;
                //vec[i].push_back(&pair);
                vec[i].push_back(&pair[i][j]);
            }
        }
        else if(this->dayDiff == 1)
        {
            for (int j = 0; j < this->dayDiff; j++)
            {
                pair[i][j].first = QDateTime::fromString(queryResult[tmp], "dd.MM.yyyy");
                pair[i][j + 1].first = QDateTime::fromString(queryResult[tmp], "dd.MM.yyyy");
                //pair.first =
                tmp++;
                pair[i][j].second = queryResult[tmp].toFloat();
                pair[i][j + 1].second = queryResult[tmp].toFloat();
                tmp++;

                vec[i].push_back(&pair[i][j]);
                vec[i].push_back(&pair[i][j + 1]);
            }
        }
    }

    if(pChart != nullptr)
        delete pChart;

    if(this->dayDiff == 1)
        pChart = new MyChartBar;
    else if (this->dayDiff > 1)
    {
        int chartType = MyConfig::GetConfigure().multChartID;

        if( chartType == 0)
            pChart = new MyChartClassic;
        else if(chartType == 1)
            pChart = new MyChartBar;
    }


    pChart->SetChart(vec, &this->operators);

    QGraphicsScene *pScene = new QGraphicsScene(ui->graphicsView);
    pScene->addWidget(pChart->GetChartWidget());


    int sceneLenTmp = this->sceneLen = pChart->GetChartLen(this->dayDiff);

    if(this->sceneLen < ui->graphicsView->geometry().width())
        sceneLenTmp = ui->graphicsView->geometry().width() - 2;

    pScene->setSceneRect(0, 0, sceneLenTmp, ui->graphicsView->geometry().height() - 2);
    ui->graphicsView->setScene(pScene);
    pChart->GetChartWidget()->setGeometry(0,0, sceneLenTmp, ui->graphicsView->sceneRect().height());

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

   if(ui->graphicsView->scene() != nullptr)
   {
       if(this->sceneLen < ui->graphicsView->geometry().width() - 2)
           ui->graphicsView->scene()->setSceneRect(0, 0, ui->graphicsView->geometry().width() - 2, ui->graphicsView->geometry().height() - 2);
       else
           ui->graphicsView->scene()->setSceneRect(0, 0, this->sceneLen, ui->graphicsView->geometry().height() - 2);

       pChart->GetChartWidget()->setGeometry(0, 0, ui->graphicsView->scene()->width(), ui->graphicsView->scene()->height());
   }
}

void MainWnd::SaveGraphic()
{
    QString fileName = QFileDialog::getSaveFileName(this, QString::fromUtf8("Зберегти файл"), QDir::rootPath(), "Зображення (*.png)");

    if(!fileName.isEmpty())
    {
        QImage image(ui->graphicsView->scene()->width(), ui->graphicsView->scene()->height(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&image);

        pChart->GetChartWidget()->render(&painter);

        image.save(fileName);
    }
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

            this->cryteria.clear();
            this->cryteria.append(CryteriaData{"Результат", "Result", ""});
            this->cryteria.append(parsedXmlProject->at(i).cryteria);
            break;
        }
    }
}

void MainWnd::SetDisabledBaseBlock(bool flag)
{
    ui->BaseIcon->setHidden(flag);
    ui->BaseTitle->setHidden(flag);

    ui->BaseList->setHidden(flag);

    ui->SetBaseButton->setHidden(flag);
}

void MainWnd::OpenSettings()
{
    Settings* pSettingsWnd = new Settings(this);

    pSettingsWnd->show();
    pSettingsWnd->exec();

    delete pSettingsWnd;
}


void MainWnd::OpenAuthorization()
{
    authorization* pSettingsWnd = new authorization(this);

    pSettingsWnd->show();
    pSettingsWnd->exec();

    delete pSettingsWnd;
}

void MainWnd::OpenChangeList()
{
    static ChangeList* pChangeListWnd = nullptr;

    if (pChangeListWnd == nullptr)
        pChangeListWnd = new ChangeList();
    else
    {
        pChangeListWnd->activateWindow();
        return;
    }

    pChangeListWnd->show();
    pChangeListWnd->exec();

    delete pChangeListWnd;
    pChangeListWnd = nullptr;
}
