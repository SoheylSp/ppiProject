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

    //Add the input for the azimuthStep and the rangeStep

    azimuthStepEdit = new QLineEdit(this);
    azimuthStepEdit->setPlaceholderText("Enter Azimuth Step");
    rangeStepEdit = new QLineEdit(this);
    rangeStepEdit->setPlaceholderText("Enter Range Step");

    QPushButton *updateButton = new QPushButton("Update",this);
    connect(updateButton,&QPushButton::clicked,this,&MainWindow::updateSetting);

    azimuthStepEdit->setGeometry(20,100,200,30);
    rangeStepEdit->setGeometry(20,130,200,30);
    updateButton->setGeometry(20,160,200,30);

    // myRangeStepSlider = new QSlider(this);
    //myRangeStepSlider->setRange(1,32);
    //myRangeStepSlider->setValue(m_arguments["rangeStep"].toInt());
    //connect(myRangeStepSlider,&QSlider::valueChanged,this,&MainWindow::updateRangeStep);

    //myRangeStepSlider->setGeometry(40,220,10,100);

    //myAzimuthStepSlider = new QSlider(this);
    //myAzimuthStepSlider->setRange(1,72);
    //myAzimuthStepSlider->setValue(m_arguments["azimuthStep"].toDouble());
    //connect(myAzimuthStepSlider,&QSlider::valueChanged,this,&MainWindow::updateAzimuthStep);
    //myAzimuthStepSlider->setGeometry(80 , 220 , 10 , 100);

    //  myBeamSpeed = new QSlider(this);
    // myBeamSpeed->setRange(1,10000);
    // connect(myBeamSpeed,&QSlider::valueChanged,this,&MainWindow::updateBeamSpeedStep);
    // myBeamSpeed->setGeometry(120,220,10,100);

    myStepRangeDial = new QDial(this);
    myStepRangeDial->setNotchesVisible(true);
    myStepRangeDial->setRange(1,500);
    myStepRangeDial->setSingleStep(10);
    myStepRangeDial->setGeometry(160,220,50,50);

    connect(myStepRangeDial,&QDial::valueChanged,this,&MainWindow::updateRangeStep);

    myAzimuthRangeDial = new QDial(this);
    myAzimuthRangeDial->setNotchesVisible(true);
    myAzimuthRangeDial->setRange(1,90);
    myAzimuthRangeDial->setGeometry(100,220,50,50);

    connect(myAzimuthRangeDial,&QDial::valueChanged , this , &MainWindow::updateAzimuthStep);

    loadCSVAction = new QAction("Load CSV",this);
    menuBar()->addAction(loadCSVAction);
    connect(loadCSVAction,&QAction::triggered,this,&MainWindow::loadCSVFile);
    QFont myFont("Times",20);

    loadCSVAction->setFont(myFont);




}

MainWindow::~MainWindow()
{
    delete timer ;
    delete mouseTimer;
}

void MainWindow::loadTargetsFromFile(const QString &filePath)
{
    QList<QPair<double,double>> targets = readTargetFromCsv(filePath);

    //Process the targets

    for(const auto &target : targets){

        double range = target.first;
        double azimuth = target.second;

    }
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

void MainWindow::loadCSVFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("Open CSV File"),"",tr("CSV Files (*.csv);;All Files (*)"));

    if(! filePath.isEmpty()){
        loadTargetsFromFile(filePath);
    }
}

QList<QPair<double, double> > MainWindow::readTargetFromCsv(const QString &filePath)
{
    QList <QPair<double,double>> targets;
    QFile file(filePath);

    if(! file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical()<<"Cannot open file : " << filePath;
        return targets ;
    }

    QTextStream in(&file) ;

    while(! in.atEnd()){
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if(fields.size() ==2){
            bool ok1,ok2;
            double range = fields[0].toDouble(&ok1);
            double azimuth = fields[1].toDouble(&ok2);

            if(ok1 && ok2){
                targets.append(qMakePair(range,azimuth));
            }else {

                qCritical()<<"Invalid data in csv file"<<line<<"\n";
            }
        }else {

            qCritical()<<"Invalid line in CSV file"<<line<<"\n";
        }
    }

    file.close();
    return targets;
}






