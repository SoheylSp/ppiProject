#include "mainwindow.h"

MainWindow::MainWindow(const QStringList &arguments , QWidget *parent)
    : QMainWindow(parent) , m_arguments(arguments)
{
    
    setFixedSize(1200,1020);
    
    QPalette palette ;
    palette.setColor(QPalette::Window,Qt::black);
    setPalette(palette);

    setMouseTracking(true);

    // Label
   m_mousePoisitionLabel = new QLabel(this);
   m_mousePoisitionLabel->setGeometry(width() - 150 , 10 , 140 , 20) ;
   m_mousePoisitionLabel->setStyleSheet("color:white;background-color:black;");
   m_mousePoisitionLabel->setText("Mouse:(0, 0)");

}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    painter.begin(this);

    QVariantMap myArguments = Utils::readArguments(m_arguments);

    double centerX=width()/2;
    double centerY = height()/2;
    QPointF screenCenterPoint ;
    screenCenterPoint.setX(centerX);
    screenCenterPoint.setY(centerY);

    double startRadius {0} , endRadius {0} , rangeStep {0} ;
    int azimuthStep{0};

    if(!myArguments.contains("radar")){

        qCritical()<<"what do you want to do?";
    }else {

        if(! myArguments.contains("startRadius") || ! myArguments.contains("endRadius") || !myArguments.contains("rangeStep")){
            qCritical()<<"a startRadius , an endRadius & a rangeStep needed";

        }else{

            startRadius = myArguments["startRadius"].toDouble();
            endRadius = myArguments["endRadius"].toDouble();
            rangeStep = myArguments["rangeStep"].toDouble();
        }
    }

    for(double i = startRadius ; i <= endRadius ; i+= rangeStep  ){

        QPen pen ;
        pen.setColor(Qt::white);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawEllipse(screenCenterPoint,i,i);

    }

    if(!myArguments.contains("azimuthStep")){
        qCritical()<<"azimuthStep is essential";
    }else {

        azimuthStep = myArguments["azimuthStep"].toDouble();
    }

    double numLines = 360 / azimuthStep ;

    if(360 % azimuthStep != 0){
        qCritical()<<"azimuth step is not valid";

    }else{

    for(int i = 0 ; i < numLines ; ++i){

        double angle = azimuthStep * i;
        double radians = qDegreesToRadians(angle - 90);

        double startX = centerX + startRadius * std::cos(radians);
        double startY = centerY + startRadius * std::sin(radians);

        double endX = centerX + endRadius * std::cos(radians);
        double endY = centerY + endRadius * std::sin(radians);

        painter.drawLine(QPointF(startX,startY),QPointF(endX,endY));

        QFont font ;
        font.setPointSizeF(10);
        painter.setFont(font);
        QPen pen ;
        pen.setColor(Qt::white);
        painter.setPen(pen);
        QString text = QString::number(angle) + "^" ;
        QRectF textRect = painter.boundingRect(QRectF(),Qt::AlignCenter,text);

        double offset = 15 ;
        double textEndX = endX + offset * std::cos(radians);
        double textEndY = endY + offset * std :: sin(radians);

        textRect . moveCenter(QPointF(textEndX,textEndY));
        painter.drawText(textRect,Qt::AlignCenter,text);


    }
    }

    QFont font ;
    font.setPointSizeF(10);
    painter.setFont(font);
    QPen pen ;
    pen.setColor(Qt::white);
    painter.setPen(pen);

    double step{rangeStep};
    QPointF point ;
    point.setX(centerX );
    point.setY(centerY);

    int circleNums = (endRadius - startRadius) / rangeStep ;

    for(int i=0  ; i <=circleNums + 1 ; ++i){

        QString text = QString::number(rangeStep*0.01*i)+"m";
        QRectF textRect = painter.boundingRect(QRectF(),Qt::AlignCenter,text);
        textRect.moveCenter(point);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::black));
        painter.drawRect(textRect);

        painter.setPen(pen);
        painter.drawText(textRect,Qt::AlignCenter,text);

        point.setY(point.y()-step);

    }




    painter.end();
}

void MainWindow::mousMoveEvent(QMouseEvent *event)
{
    double centerX = width()/2 ;
    double centerY = height()/2;

    double mouseX = event->pos().x();
    double mouseY = event->pos().y();

    double relativeX = mouseX - centerX ;
    double relativeY = centerY - mouseY ;

    m_mousePoisitionLabel->setText(QString("Mouse : (%1,%2)").arg(relativeX).arg(relativeY));

}



