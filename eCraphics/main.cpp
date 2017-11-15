#include "mainwnd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setWindowIcon(QIcon(":/MAIN_ICO"));

    MainWnd window;

    window.show();
    window.Start();

    return application.exec();
}
