#include "mainwindow.h"
#include <QVariantMap>
#include <QApplication>
#include <utils.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList myArgumnts = a.arguments();

    MainWindow w(myArgumnts,nullptr);



    w.show();
    return a.exec();
}
