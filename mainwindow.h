#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QPainter>
#include "utils.h"
#include <QGeoCoordinate>
#include <QLine>
#include <QLabel>
#include <cmath>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QStringList& arguments ,QWidget *parent = nullptr);
    ~MainWindow();

protected:

    void paintEvent(QPaintEvent *e)override;

public slots:

    void updateRadar() ;
    

private:
    QPainter painter ;
    QStringList m_arguments ;
    QTimer *timer;
    double angle ;
    QVector<QPointF> targets ;

 
};
#endif // MAINWINDOW_H
