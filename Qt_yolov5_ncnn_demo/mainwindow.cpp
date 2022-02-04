#include "mainwindow.h"
#include "myutils.h"
#include "ui_mainwindow.h"
#include <omp.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    detector = new  YoloV5(
                "./model/yolov5s-opt-fp16.param",
                "./model/yolov5s-opt-fp16.bin",true);
    m_timer = new QTimer();
    m_timer->setInterval(100);
    //=================================================================================================
#if 1
    connect(ui->pb_opencam,&QPushButton::clicked,this,[&](){
        //打开视频文件
        QString fileName = QFileDialog::getOpenFileName(
                    this, tr("open video file"),
                    "./", tr("video files (*.mp4 *.avi *.webm);All files (*.*)"));
        qDebug()<<fileName;
        if(!fileName.isEmpty())
        {
            qDebug()<<"打开camera";
            this->Capture.open(fileName.toStdString());
            if(!m_timer->isActive())m_timer->start();
        }
    });

    connect(m_timer,&QTimer::timeout,[&](){
        if (!Capture.isOpened())
        {
            if(m_timer->isActive())m_timer->stop();
            return;
        }

        Capture >> this->frame;

        if (frame.empty())
        {
            if(m_timer->isActive())m_timer->stop();
            return;
        }
        //AI MODEL can insert here
        detector->detect(frame, 0.7, 0.5);
        detector->draw(frame);
        //========================
        MyUtils::Mat2QImage(this->frame,this->frame2QImage);
        ui->ogl_cam->setScaledContents(true);
        ui->ogl_cam->setPixmap(QPixmap::fromImage(this->frame2QImage));

    });
#endif
    //=================================================================================================
    //关闭线程
    connect(this,&MainWindow::destroyed,[&](){
        flag_closeOpencvVideoCaptureThread=true;
        if(m_timer->isActive())m_timer->stop();
        qDebug()<<"关闭线程====";
    });
}
MainWindow::~MainWindow()
{
    delete ui;
    if(m_timer->isActive()) m_timer->stop();
    if(m_timer!=nullptr) delete m_timer;
    this->Capture.release();
}




void MainWindow::on_pb_close_clicked()
{
    if(m_timer->isActive()) m_timer->stop();
}
