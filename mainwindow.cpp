#include "mainwindow.h"

MainWindow::MainWindow(const QStringList &arguments , QWidget *parent)
    : QMainWindow(parent) , m_arguments(arguments) , angle(0.0)
{

    setFixedSize(1200,1020);

    QPalette palette ;
    palette.setColor(QPalette::Window,Qt::black);
    setPalette(palette);

    timer = new QTimer(this) ;
    connect(timer,&QTimer::timeout,this,&MainWindow::updateRadar);
    timer->start(2);

    targets.append(QPointF(700,500));
    targets.append(QPointF(300,800));
    targets.append(QPointF(1000,400));
    targets.append(QPointF(1000,600));

}

MainWindow::~MainWindow()
{
    delete timer ;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    painter.begin(this);


    int startRadius{25} , endRadius{475} ;
    QPointF scCenterPoint;
    scCenterPoint.setX(width()/2);
    scCenterPoint.setY(height()/2);

    QVariantMap myArguments = Utils::readArguments(m_arguments);

    if(! myArguments .contains("rangeStep") || ! myArguments.contains("azimuthStep")){

        qCritical()<<"rangeStep and azimuthStep are required\n";

    }

    int rangeStep = myArguments["rangeStep"].toInt();
    double azimuthStep = myArguments["azimuthStep"].toDouble();



    painter.setPen(QPen(Qt::white,2));

    for(int i =  startRadius ; i <= endRadius ; i +=rangeStep){

        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawEllipse(scCenterPoint,i,i);


    }

    double numLines = 360 / azimuthStep ;

    for(int i = 0 ; i < numLines ; i++){

        double angle = azimuthStep * i ;
        double radians = qDegreesToRadians(angle - 90);

        double startX = scCenterPoint.x() + startRadius * std::cos(radians);
        double startY = scCenterPoint.y() + startRadius * std::sin(radians);

        double endX = scCenterPoint.x() + endRadius  * std::cos(radians);
        double endY = scCenterPoint.y()  + endRadius * std::sin(radians) ;

        painter.drawLine(QPointF(startX,startY),QPointF(endX,endY));

        QFont font ;
        font.setPointSize(10);
        painter.setFont(font);


        QPen pen(Qt::white,2);
        QString text=QString::number(angle )+"^";
        QRectF textRect = painter.boundingRect(QRectF(),Qt::AlignCenter,text);

        double offset = 15 ;
        double textEndX = endX + offset * std::cos(radians);
        double textEndY = endY + offset * std::sin(radians);

        textRect.moveCenter(QPointF(textEndX,textEndY));
        painter.drawText(textRect,Qt::AlignCenter,text);


    }

    int circleNums = (endRadius - startRadius) / rangeStep ;
    double step = rangeStep ;
    QPointF point;
    point.setX(scCenterPoint.x());
    point.setY(scCenterPoint.y() - 25);

    for(int i = 1/2 ; i <= circleNums ; i+=1){

        QString text = QString :: number((rangeStep ) * 0.01 * i)+ "km" ;
        QRectF textRect = painter.boundingRect(QRectF(),Qt::AlignCenter,text);
        textRect.moveCenter(point);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::black));
        painter.drawRect(textRect);

        painter.setPen(QPen(Qt::white,2));
        painter.drawText(textRect,Qt::AlignCenter,text);
        point.setY(point.y()-step);

    }

    //Radar rotate beam

    painter.setPen(QPen(Qt::cyan,3));
    double rad = qDegreesToRadians(angle);
    QPointF endPoint(scCenterPoint.x() + endRadius * cos(rad) , scCenterPoint.y() + endRadius *sin(rad));
    painter.drawLine(scCenterPoint,endPoint);


    painter.setPen(QPen(Qt::yellow,3));
    painter.setBrush(QBrush(Qt::yellow));

    for(const QPointF target : targets)
    {
        painter.drawEllipse(target,10,10);
    }

    painter.end();
}

void MainWindow::updateRadar()
{

 angle += 1.0 ;
 if(angle >= 360){
     angle = 0.0 ;
 }
 update();

}






