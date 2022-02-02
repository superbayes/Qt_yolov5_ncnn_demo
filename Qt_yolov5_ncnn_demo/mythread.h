#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
class MyTHread : public QObject
{
    Q_OBJECT
public:
    explicit MyTHread(QObject *parent = nullptr);
public:
    bool flag_exitCamera = false;
    cv::Mat frame;
    cv::VideoCapture Capture;
public:
    void videoThread(std::string videoFileName);
signals:
    void signal_frame(cv::Mat& img);

};

#endif // MYTHREAD_H
