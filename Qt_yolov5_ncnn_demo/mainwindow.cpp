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
    thread_camera = new QThread();
    MyCamera = new MyTHread();
    MyCamera->moveToThread(thread_camera);
    detector = new  YoloV5(
                "./model/yolov5s-opt-fp16.param",
                "./model/yolov5s-opt-fp16.bin",true);
    //=================================================================================================
#if 1
    connect(ui->pb_opencam,&QPushButton::clicked,this,[&](){
        //打开视频文件
        QString fileName = QFileDialog::getOpenFileName(
                    this, tr("open video file"),
                    "./", tr("video files (*.mp4 *.avi *.webm);All files (*.*)"));
        qDebug()<<fileName;
        //==============================================================
        if(!thread_camera->isRunning()&& !fileName.isEmpty())
        {
            qDebug()<<"打开camera";
            thread_camera->start();
            emit signal_openCAM(fileName.toStdString());
        }else
        {

            qDebug()<<"camera is running!!!";
        }
    });
    connect(this,&MainWindow::signal_openCAM,MyCamera,&MyTHread::videoThread);
    connect(MyCamera,&MyTHread::signal_frame,[&](cv::Mat& irImg_){
        this->frame = irImg_;//.clone();
        detector->detect(frame, 0.7, 0.5);
        detector->draw(frame);
        MyUtils::Mat2QImage(this->frame,this->frame2QImage);
        //qDebug()<<QString("frame width:%1;height:%2").arg(frame.cols).arg(frame.rows);
        this->update();
    });
#endif
    //=================================================================================================
    //关闭线程
    connect(this,&MainWindow::destroyed,[&](){
        flag_closeOpencvVideoCaptureThread=true;
        if(thread_camera->isRunning())
        {
            MyCamera->flag_exitCamera=true;
            thread_camera->quit();
            thread_camera->wait();
        }
        qDebug()<<"关闭线程====";
    });
}
MainWindow::~MainWindow()
{
    delete ui;
    if (MyCamera!=nullptr)delete MyCamera;
    if (thread_camera!=nullptr)delete thread_camera;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(ui->ogl_cam);
    painter.drawPixmap(0,0,ui->ogl_cam->width(),ui->ogl_cam->height(),QPixmap::fromImage(this->frame2QImage));
    painter.end();
}

