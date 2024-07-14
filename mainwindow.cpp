#include "mainwindow.h"

MainWindow::MainWindow(const QVariantMap &arguments , QWidget *parent)
    : QMainWindow(parent) , m_arguments(arguments) , angle(0.0)
{

    setFixedSize(1700,1020);

    QPalette palette ;
    palette.setColor(QPalette::Window,Qt::black);
    setPalette(palette);

    timer = new QTimer(this) ;
    connect(timer,&QTimer::timeout,this,&MainWindow::updateRadar);
    timer->start(1);

    targets.append(QPointF(width()/2 + 100,height()/2 + 200));
    targets.append(QPointF(width()/2 + 300,height()/2 - 200));
    targets.append(QPointF(width()/2 + 50 ,height()/2 -50));
    targets.append(QPointF(width()/2 + 130,height()/2 -130));

    mouseTimer = new QTimer(this);
    connect(mouseTimer,&QTimer::timeout,this,&MainWindow::updateMousePosition);
    mouseTimer->start(10);


    myStepRangeDial = new QDial(this);
    myStepRangeDial->setNotchesVisible(true);
    myStepRangeDial->setRange(50,500);
    myStepRangeDial->setSingleStep(50);
    myStepRangeDial->setGeometry(160,220,50,50);

    connect(myStepRangeDial,&QDial::valueChanged,this,&MainWindow::updateRangeStep);

    myAzimuthRangeDial = new QDial(this);
    myAzimuthRangeDial->setNotchesVisible(true);
    myAzimuthRangeDial->setRange(1,360);
    myAzimuthRangeDial->setSingleStep(10);
    myAzimuthRangeDial->setGeometry(100,220,50,50);

    connect(myAzimuthRangeDial,&QDial::valueChanged , this , &MainWindow::updateAzimuthStep);


    csvLineEdit = new QLineEdit(this);
    csvLineEdit->setGeometry(100,300,120,20);
    csvLineEdit->setPlaceholderText("path/to/csv");
    csvButton = new QPushButton(this);
    csvButton->setGeometry(100,330,80,20);
    csvButton->setText("Load CSV");
    connect(csvButton,&QPushButton::clicked,this,&MainWindow::csvSlot);

    csvDialogBtn = new QPushButton(this);
    csvDialogBtn->setGeometry(100 , 360 , 50 , 50);
    csvDialogBtn->setText("Dialog");
    connect(csvDialogBtn,&QPushButton::clicked,this,&MainWindow::csvDialogSlot);

}

MainWindow::~MainWindow()
{
    delete timer ;
    delete mouseTimer;
}

void MainWindow::readCSVtargets(const QString &filePath)
{

    QFile file(filePath);

    if(! file.open(QIODevice::Text | QIODevice::ReadOnly)){
        qCritical()<<"File is not OPEN\n";
    }

    QTextStream in (&file);

    while(! in.atEnd()){
        QString line = file.readLine();
        QStringList fields = line.split(",");

        if(fields.size()==2){
            bool rangeOk , azimuthOk ;
            double range = fields[0].toDouble(&rangeOk);
            double azimuth = fields[1].toDouble(&azimuthOk);

            if(rangeOk && azimuthOk){

                double radians = qDegreesToRadians(azimuth);
                double x = width()/2 + range * std::cos(radians);
                double y = height()/2 + range * std::sin(radians);

                targets.append(QPointF(x,y));
            }else {

                qCritical()<<"Invalid data in CSV file  :" << line;

            }
        }else {

            qCritical()<<"Invalid line in CSV file";
        }
    }

    file.close();
    update();

}


void MainWindow::paintEvent(QPaintEvent *e)
{
    painter.begin(this);


    int startRadius{25} , endRadius{475} ;
    QPointF scCenterPoint;
    scCenterPoint.setX(width()/2 + panOffset.x());
    scCenterPoint.setY(height()/2  + panOffset.y());

    startRadius *= zoomFactor ;
    endRadius *= zoomFactor ;

    QVariantMap myArguments = m_arguments;

    if(! myArguments .contains("rangeStep") || ! myArguments.contains("azimuthStep")){

        qCritical()<<"rangeStep and azimuthStep are required\n";

    }

    int rangeStep = myArguments["rangeStep"].toInt();
    double azimuthStep = myArguments["azimuthStep"].toDouble();
    double beamSpeed = myArguments["beamSpeed"].toDouble();




    painter.setPen(QPen(Qt::white,2));

    for(int i =  startRadius ; i <= endRadius ; i += rangeStep){

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
    double step = rangeStep * zoomFactor;
    QPointF point (scCenterPoint.x(),scCenterPoint.y()-25*zoomFactor);


    for(int i = 1 ; i <= circleNums ; ++i){

        QString text = QString :: number((rangeStep ) * 1000 /10000 * i*2)+ "km" ;
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

    painter.setPen(QPen(Qt::green,7));
    double rad = qDegreesToRadians(angle);
    QPointF endPoint(scCenterPoint.x() + endRadius * cos(rad) , scCenterPoint.y() + endRadius *sin(rad));
    painter.drawLine(scCenterPoint,endPoint);


    painter.setPen(QPen(Qt::yellow,3));
    painter.setBrush(QBrush(Qt::yellow));

    for(const QPointF target : targets)
    {
        painter.drawEllipse(target,10,10);
    }

    //Mouse Position


    QFont font("Times",30,10,true);
    painter.setFont(font);
    painter.setPen(QPen(Qt::white));
    painter.drawText(20,40,QString("X:%1 , Y:%2").arg(mousePos.x()).arg(mousePos.y()));



    painter.end();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //Why

    if(isPanning){
        QPoint delta = event->pos() - lastMousePoint ;
        panOffset += QPointF(delta.x(),delta.y());
        lastMousePoint = event->pos();
        update();
    }

    mousePos = event->pos();
    update() ;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0){

        //Zoom in
        zoomFactor *= 1.1 ;
    }else {

        //Zoom out
        zoomFactor/= 1.1 ;
    }
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){

        isPanning =true ;
        lastMousePoint = event->pos();

    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){

        isPanning = false ;
    }
}



void MainWindow::updateRadar()
{

    angle += 0.5;
    if(angle >= 360){
        angle = 0.0 ;
    }
    update();

}

void MainWindow::updateMousePosition()
{
    mousePos = QCursor::pos() - this->pos();
    QPoint center(width()/2,height()/2);
    mousePos -= center ;
    update();
}

void MainWindow::updateSetting()
{
    bool aZok , rAok ;
    double azimuthStep = azimuthStepEdit->text().toDouble(&aZok);
    if(&aZok){
        m_arguments["azimuthStep"]=azimuthStep;

    }
    int rangeStep = rangeStepEdit->text().toDouble(&rAok);

    if(&rAok){
        m_arguments["rangeStep"]=rangeStep;
    }

    update();
}

void MainWindow::updateRangeStep(int value)
{
    m_arguments["rangeStep"]=value;
    update();
}

void MainWindow::updateAzimuthStep(int value)
{
    m_arguments["azimuthStep"] = value ;
    update();
}

void MainWindow::updateBeamSpeedStep(int value)
{
    timer->start(value);
    update();
}

void MainWindow::updateRangeStepDial(int value)
{
    m_arguments["rangeStep"] = value ;
    update();
}

void MainWindow::csvSlot()
{

    QString filePath = csvLineEdit->text();
    if(! filePath.isEmpty()){

        readCSVtargets(filePath);

    }else {

        qCritical()<<"No File OR directory";
    }

}

void MainWindow::csvDialogSlot()
{

    QFileDialog dialog(this);

    dialog.setModal(true);

    dialog.setFileMode(QFileDialog::AnyFile);

    dialog.setNameFilter(tr("CSV Files(*.csv)"));

    if(dialog.exec() == QDialog::Accepted){

        QString fileName = dialog.selectedFiles().first();
        readCSVtargets(fileName);
    }

}











