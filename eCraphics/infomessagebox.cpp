#include "infomessagebox.h"

InfoMessageBox::InfoMessageBox()
{
    //mb.setText("dfdfdfdd");
    //mb.setWhatsThis("win");
    //mb.setDefaultButton(nullptr);
}

void InfoMessageBox::begin()
{
    mb = new QMessageBox;
    mb->setText("55555");
    mb->setWhatsThis("55hhh");
    mb->show();
    qDebug() << "mb thr start";
    mb->exec();
    //mb.show();
    //mb.exec();
}

void InfoMessageBox::end()
{
    mb->close();
    delete mb;
    //mb.close();
}
