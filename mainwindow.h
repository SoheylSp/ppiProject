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
#include <QMouseEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QDial>
#include <QWheelEvent>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QPalette>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QVariantMap& arguments ,QWidget *parent = nullptr);
    ~MainWindow();
    void loadTargetsFromFile(const QString &filePath);

protected:

    void paintEvent(QPaintEvent *e)override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override ;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override ;

public slots:

    void updateRadar() ;
    void updateMousePosition();
    void updateSetting();

    void updateRangeStep(int value);
    void updateAzimuthStep(int value);
    void updateBeamSpeedStep(int value);

    void updateRangeStepDial(int value);
    void loadCSVFile();


private:
    QPainter painter ;
    QVariantMap m_arguments ;
    QTimer *timer;
    double angle ;
    QVector<QPointF> targets ;
    QPoint mousePos ;
    QTimer *mouseTimer ;
    QLineEdit *azimuthStepEdit;
    QLineEdit *rangeStepEdit;
    //QSlider *myRangeStepSlider;
    //QSlider *myAzimuthStepSlider;
    //QSlider *myBeamSpeed;

    QDial *myStepRangeDial ;
    QDial *myAzimuthRangeDial;
    double zoomFactor = 1.0 ;

    QPointF panOffset ;
    QPoint lastMousePoint;
    bool isPanning = false ;

    QList<QPair<double,double>> readTargetFromCsv(const QString &filePath);

    QAction *loadCSVAction ;





 
};
#endif // MAINWINDOW_H
