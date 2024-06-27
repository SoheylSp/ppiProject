#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QPainter>
#include "utils.h"
#include <QGeoCoordinate>
#include <QLine>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QStringList& arguments ,QWidget *parent = nullptr);
    ~MainWindow();

protected:

    void paintEvent(QPaintEvent *e);

private:
    QPainter painter ;
    QStringList m_arguments ;

};
#endif // MAINWINDOW_H
