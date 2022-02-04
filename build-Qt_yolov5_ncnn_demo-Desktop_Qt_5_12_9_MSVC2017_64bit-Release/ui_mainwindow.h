/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pb_opencam;
    QLabel *ogl_cam;
    QPushButton *pb_close;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1006, 835);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pb_opencam = new QPushButton(centralwidget);
        pb_opencam->setObjectName(QString::fromUtf8("pb_opencam"));
        pb_opencam->setGeometry(QRect(10, 740, 101, 31));
        ogl_cam = new QLabel(centralwidget);
        ogl_cam->setObjectName(QString::fromUtf8("ogl_cam"));
        ogl_cam->setGeometry(QRect(10, 10, 960, 720));
        ogl_cam->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        pb_close = new QPushButton(centralwidget);
        pb_close->setObjectName(QString::fromUtf8("pb_close"));
        pb_close->setGeometry(QRect(120, 740, 101, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1006, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pb_opencam->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\350\247\206\351\242\221", nullptr));
        ogl_cam->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        pb_close->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
