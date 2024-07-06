#include "mainwindow.h"
#include <QVariantMap>
#include <QApplication>
#include <utils.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList myArgumnts = a.arguments() ;
    QVariantMap argumentsMap = Utils::readArguments(myArgumnts);


    MainWindow w(argumentsMap,nullptr);

    w.show();
    return a.exec();
}
