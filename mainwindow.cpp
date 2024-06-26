#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    
    setFixedSize(1200,1020);
    
    QPalette palette ;
    palette.setColor(QPalette::Window,Qt::black);
    setPalette(palette);
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    
    painter.begin(this);
    
    double centerX = width()/2;
    double centerY = height()/2;
    
    QPointF centerPoint;
    centerPoint.setX(centerX);
    centerPoint.setY(centerY);
    
    for(int radius = 0 ; radius <= 500 ; radius += 50){
        
        QPen pen;
        pen.setColor(Qt::white);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawEllipse(centerPoint,radius,radius);
        
    }
    
    //Line
    
    double startRadius = 50 ;
    double endRadius = 500 ;
    int numLines = 36 ;
    double angleStep = 360 / numLines ;
    
    
    for(int i = 0 ; i < numLines ; ++i){
        
        double angle = i * angleStep ;
        double radians = qDegreesToRadians(angle);
        
        double startX = centerX + startRadius * std::cos(radians);
        double startY = centerY + startRadius * std::sin(radians);
        
        double endX = centerX + endRadius * std::cos(radians);
        double endY = centerY + endRadius * std::sin(radians);
        
        painter.drawLine(QPointF(startX,startY),QPointF(endX,endY));
        
    }
    
    double step{50};
    QPen pen ;
    pen.setColor(Qt::white);
    pen.setWidth(2);

    QFont font;
    font.setPointSizeF(10);
    painter.setFont(font);
    QPointF point_1;
    point_1.setX(centerPoint.x() - 10);
    point_1.setY(centerPoint.y());
    painter.drawText(point_1,"0.0m");

    QPointF point_2;
    point_2.setX(point_1.x() + 15);
    point_2.setY(point_1.y() - step);

    QRectF textRect = painter.boundingRect(QRectF(), Qt::AlignCenter, "0.5m");
    textRect.moveCenter(point_2);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect);

    painter.setPen(pen);
    painter.drawText(textRect, Qt::AlignCenter, "0.5m");

    QPointF point_3 ;
    point_3.setX(point_2.x());
    point_3.setY(point_2.y()-step);

    QRectF textRect_3 = painter.boundingRect(QRectF(),Qt::AlignCenter,"1.0m");
    textRect_3.moveCenter(point_3);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect_3);

    painter.setPen(pen);
    painter.drawText(textRect_3,Qt::AlignCenter,"1.0m");

    QPointF point_4 ;
    point_4.setX(point_3.x());
    point_4.setY(point_3.y()-step);

    QRectF textRect_4 = painter.boundingRect(QRectF(),Qt::AlignCenter,"1.5m");
    textRect_4.moveCenter(point_4);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect_4);

    painter.setPen(pen);
    painter.drawText(textRect_4,Qt::AlignCenter,"1.5m");


    QPointF point_5 ;
    point_5.setX(point_4.x());
    point_5.setY(point_4.y()-step);

    QRectF textRect_5 = painter.boundingRect(QRectF(),Qt::AlignCenter,"2.0m");
    textRect_5.moveCenter(point_5);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect_5);

    painter.setPen(pen);
    painter.drawText(textRect_5,Qt::AlignCenter,"2.0m");


    QPointF point_6 ;
    point_6.setX(point_5.x());
    point_6.setY(point_5.y()-step);

    QRectF textRect_6 = painter.boundingRect(QRectF(),Qt::AlignCenter,"2.5m");
    textRect_6.moveCenter(point_6);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect_6);

    painter.setPen(pen);
    painter.drawText(textRect_6,Qt::AlignCenter,"2.5m");


    QPointF point_7 ;
    point_7.setX(point_6.x());
    point_7.setY(point_6.y()-step);

    QRectF textRect_7 = painter.boundingRect(QRectF(),Qt::AlignCenter,"3.0m");
    textRect_7.moveCenter(point_7);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect_7);

    painter.setPen(pen);
    painter.drawText(textRect_7,Qt::AlignCenter,"3.0m");


    QPointF point_8 ;
    point_8.setX(point_7.x());
    point_8.setY(point_7.y()-step);

    QRectF textRect_8 = painter.boundingRect(QRectF(),Qt::AlignCenter,"3.5m");
    textRect_8.moveCenter(point_8);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect_8);

    painter.setPen(pen);
    painter.drawText(textRect_8,Qt::AlignCenter,"3.5m");


    QPointF point_9 ;
    point_9.setX(point_8.x());
    point_9.setY(point_8.y()-step);

    QRectF textRect_9 = painter.boundingRect(QRectF(),Qt::AlignCenter,"4.0m");
    textRect_9.moveCenter(point_9);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect_9);

    painter.setPen(pen);
    painter.drawText(textRect_9,Qt::AlignCenter,"4.0m");


    QPointF point_10 ;
    point_10.setX(point_9.x());
    point_10.setY(point_9.y()-step);

    QRectF textRect_10 = painter.boundingRect(QRectF(),Qt::AlignCenter,"4.5m");
    textRect_10.moveCenter(point_10);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect_10);

    painter.setPen(pen);
    painter.drawText(textRect_10,Qt::AlignCenter,"4.5m");


    QPointF point_11 ;
    point_11.setX(point_10.x());
    point_11.setY(point_10.y()-step);

    QRectF textRect_11 = painter.boundingRect(QRectF(),Qt::AlignCenter,"5.0m");
    textRect_11.moveCenter(point_11);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(textRect_11);

    painter.setPen(pen);
    painter.drawText(textRect_11,Qt::AlignCenter,"5.0m");
    
}



