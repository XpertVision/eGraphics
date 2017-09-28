#include "mainwnd.h"
#include <QApplication>

int chartTheme(0);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/MAIN_ICO"));

    MainWnd w;

    w.show();
    w.Start();

    return a.exec();
}
