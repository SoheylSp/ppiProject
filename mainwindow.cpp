#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    painter.begin(this);

    //Page's center coordinates

    double centerx = width()/2;
    double centery = height()/2;

    //Circles Center Point
    QPointF centerPoint(centerx,centery);

    QPen bluePen ;
    bluePen.setWidth(10);
    bluePen.setColor(Qt::blue);

    painter.setPen(bluePen);
    painter.drawEllipse(centerPoint,100,100);

    QPen redPen ;
    redPen.setWidth(10) ;
    redPen.setColor(Qt::red);

    painter.setPen(redPen);
    painter.drawEllipse(centerPoint,150,150);

    QPen greenPen ;
    greenPen.setWidth(10);
    greenPen.setColor(Qt::green);

    painter.setPen(greenPen);
    painter.drawEllipse(centerPoint,200,200);


    QPen yellowPen ;
    yellowPen.setWidth(10);
    yellowPen.setColor(Qt::yellow);

    painter.setPen(yellowPen);
    painter.drawEllipse(centerPoint,250,250);

    QPen blackPen ;
    blackPen.setColor(Qt::black) ;
    blackPen.setWidth(10);

    painter.setPen(blackPen);
    painter.drawEllipse(centerPoint,300,300);

    painter.end ();
}



