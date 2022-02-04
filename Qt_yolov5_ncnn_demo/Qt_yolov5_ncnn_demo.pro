QT       += core gui
QT      += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    YoloV5.cpp \
    main.cpp \
    mainwindow.cpp \
    myutils.cpp

HEADERS += \
    YoloV5.h \
    mainwindow.h \
    myutils.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Qt_yolov5_ncnn_demo_zh_CN.ts

INCLUDEPATH+= D:\ProgramData\opencv_3416\vc15\include
INCLUDEPATH+= D:\ProgramData\ncnn-20211122-windows-vs2017-shared\x64\include\ncnn
INCLUDEPATH+= D:\VulkanSDK\1.2.198.1\Include
CONFIG(debug, debug|release) {
    LIBS+=-LD:\ProgramData\opencv_3416\vc15\lib -lopencv_world3416d
} else{
    LIBS+=-LD:\ProgramData\opencv_3416\vc15\lib -lopencv_world3416
    LIBS+=-LD:\ProgramData\ncnn-20211122-windows-vs2017-shared\x64\lib -lncnn
    LIBS+=-LD:\VulkanSDK\1.2.198.1\Lib -lvulkan-1
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
