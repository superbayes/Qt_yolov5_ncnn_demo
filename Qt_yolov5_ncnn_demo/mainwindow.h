#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QThread>
//#include "mythread.h"
#include <QImage>
#include <QDebug>
#include <QTimer>
//#include "opencv2/opencv.hpp"
#include "YoloV5.h"
//#include <QPainter>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    cv::Mat frame;
    QImage frame2QImage;
    YoloV5* detector;
    cv::VideoCapture Capture;
    bool flag_closeOpencvVideoCaptureThread=false;
    QTimer* m_timer;
private slots:
    void on_pb_close_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
