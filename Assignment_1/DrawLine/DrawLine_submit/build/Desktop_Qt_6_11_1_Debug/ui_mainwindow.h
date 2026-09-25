/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
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
#include <my_label.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    my_label *frame;
    QLabel *instructions;
    QLabel *mouse_movement_caption;
    QLabel *mouse_movement;
    QLabel *mouse_pressed_caption;
    QLabel *mouse_pressed;
    QPushButton *draw_dda;
    QPushButton *draw_bresenham;
    QPushButton *clear;
    QLabel *time_caption;
    QLabel *dda_time_label;
    QLabel *bresenham_time_label;
    QLabel *legend_label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(840, 620);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame = new my_label(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(20, 20, 500, 500));
        instructions = new QLabel(centralwidget);
        instructions->setObjectName("instructions");
        instructions->setGeometry(QRect(20, 525, 500, 60));
        instructions->setWordWrap(true);
        mouse_movement_caption = new QLabel(centralwidget);
        mouse_movement_caption->setObjectName("mouse_movement_caption");
        mouse_movement_caption->setGeometry(QRect(650, 15, 170, 16));
        mouse_movement = new QLabel(centralwidget);
        mouse_movement->setObjectName("mouse_movement");
        mouse_movement->setGeometry(QRect(650, 33, 170, 25));
        mouse_pressed_caption = new QLabel(centralwidget);
        mouse_pressed_caption->setObjectName("mouse_pressed_caption");
        mouse_pressed_caption->setGeometry(QRect(650, 68, 170, 16));
        mouse_pressed = new QLabel(centralwidget);
        mouse_pressed->setObjectName("mouse_pressed");
        mouse_pressed->setGeometry(QRect(650, 86, 170, 25));
        draw_dda = new QPushButton(centralwidget);
        draw_dda->setObjectName("draw_dda");
        draw_dda->setGeometry(QRect(650, 130, 170, 50));
        draw_bresenham = new QPushButton(centralwidget);
        draw_bresenham->setObjectName("draw_bresenham");
        draw_bresenham->setGeometry(QRect(650, 195, 170, 50));
        clear = new QPushButton(centralwidget);
        clear->setObjectName("clear");
        clear->setGeometry(QRect(650, 260, 170, 40));
        time_caption = new QLabel(centralwidget);
        time_caption->setObjectName("time_caption");
        time_caption->setGeometry(QRect(650, 315, 170, 16));
        dda_time_label = new QLabel(centralwidget);
        dda_time_label->setObjectName("dda_time_label");
        dda_time_label->setGeometry(QRect(650, 333, 170, 25));
        bresenham_time_label = new QLabel(centralwidget);
        bresenham_time_label->setObjectName("bresenham_time_label");
        bresenham_time_label->setGeometry(QRect(650, 358, 170, 25));
        legend_label = new QLabel(centralwidget);
        legend_label->setObjectName("legend_label");
        legend_label->setGeometry(QRect(650, 400, 170, 110));
        legend_label->setWordWrap(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 840, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Assignment 1 - Line Drawing (DDA & Bresenham)", nullptr));
        frame->setText(QString());
        instructions->setText(QCoreApplication::translate("MainWindow", "Click two points on the canvas to set the endpoints, then pick an algorithm on the right.", nullptr));
        mouse_movement_caption->setText(QCoreApplication::translate("MainWindow", "Cursor position:", nullptr));
        mouse_movement->setText(QCoreApplication::translate("MainWindow", "X : -, Y : -", nullptr));
        mouse_pressed_caption->setText(QCoreApplication::translate("MainWindow", "Last clicked point:", nullptr));
        mouse_pressed->setText(QCoreApplication::translate("MainWindow", "X : -, Y : -", nullptr));
        draw_dda->setText(QCoreApplication::translate("MainWindow", "Draw: DDA", nullptr));
        draw_bresenham->setText(QCoreApplication::translate("MainWindow", "Draw: Bresenham", nullptr));
        clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        time_caption->setText(QCoreApplication::translate("MainWindow", "Avg. time (ms):", nullptr));
        dda_time_label->setText(QCoreApplication::translate("MainWindow", "DDA: -", nullptr));
        bresenham_time_label->setText(QCoreApplication::translate("MainWindow", "Bresenham: -", nullptr));
        legend_label->setText(QCoreApplication::translate("MainWindow", "Legend:\n"
"Red = clicked point\n"
"Green = DDA line\n"
"Blue = Bresenham line", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
