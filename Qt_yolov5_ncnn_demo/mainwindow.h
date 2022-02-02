#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "mythread.h"
#include <QImage>
#include <QDebug>
#include "opencv2/opencv.hpp"
#include "YoloV5.h"
#include <QPainter>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QThread *thread_camera;
    MyTHread* MyCamera;
    cv::Mat frame;
    cv::Mat frame_3D;
    QImage frame2QImage;
    QImage frame2QImage_3D;
    //NanoDetPLUS1* detector;
    YoloV5* detector;
    cv::VideoCapture Capture;
    bool flag_closeOpencvVideoCaptureThread=false;
public:
    void paintEvent(QPaintEvent *e);

signals:
    void signal_openIR(int deviceid);
    void signal_openCAM(std::string videoFileName);

private slots:


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
