#include "mythread.h"
#include <QMetaType>
#include <QDebug>
MyTHread::MyTHread(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::string>("std::string&");
}

void MyTHread::videoThread(std::string videoFileName)
{
    //打开相机
//    cv::VideoCapture Capture(0);
    Capture.open(videoFileName);
    if(!Capture.isOpened()) return;
    //Capture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);//宽度
    //Capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);//高度
    //Capture.set(cv::CAP_PROP_FPS, 30);//帧数
    //Capture.set(cv::CAP_PROP_BRIGHTNESS, 100);//亮度 100
    //Capture.set(cv::CAP_PROP_CONTRAST,40);//对比度 40
    //Capture.set(cv::CAP_PROP_SATURATION, 50);//饱和度 50
    //Capture.set(cv::CAP_PROP_HUE, 50);//色调 50
    //Capture.set(cv::CAP_PROP_AUTOFOCUS, 0);//开启自动对焦功能(AutoFocus)
    //Capture.set(cv::CAP_PROP_SETTINGS, 1);//开启参数自调节模式
    //Capture.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
    //Capture.set(cv::CAP_PROP_EXPOSURE, -6);//曝光 50
    //循环
    while(1){

        Capture>>this->frame;
        if(flag_exitCamera==true) break;
        if(frame.empty()) break;
        cv::resize(frame,frame,cv::Size(0,0),0.5,0.5);
        emit signal_frame(frame);
        QThread::msleep(1000/50);
    }

}
