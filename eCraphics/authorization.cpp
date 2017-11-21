#include "authorization.h"
#include "ui_authorization.h"

authorization::authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::authorization)
{
    ui->setupUi(this);

    setWindowFlags (windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

authorization::~authorization()
{
    delete ui;
}
