#include "changelist.h"
#include "ui_changelist.h"

ChangeList::ChangeList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeList)
{
    ui->setupUi(this);

    setWindowFlags (windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(ui->Close, &QPushButton::clicked, this, &ChangeList::close);
}

ChangeList::~ChangeList()
{
    delete ui;
}
