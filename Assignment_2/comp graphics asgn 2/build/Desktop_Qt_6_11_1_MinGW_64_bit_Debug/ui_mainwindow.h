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
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <my_label.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    my_label *frame;
    QLabel *mouse_movement;
    QLabel *mouse_pressed;
    QPushButton *draw_line;
    QPushButton *clear;
    QPushButton *pushButton_2;
    QSpinBox *spinBox;
    QPushButton *draw_dda;
    QPushButton *draw_bresenham;
    QLabel *mouse_pressed_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame = new my_label(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(20, 9, 500, 541));
        mouse_movement = new QLabel(centralwidget);
        mouse_movement->setObjectName("mouse_movement");
        mouse_movement->setGeometry(QRect(680, 490, 111, 41));
        mouse_pressed = new QLabel(centralwidget);
        mouse_pressed->setObjectName("mouse_pressed");
        mouse_pressed->setGeometry(QRect(690, 450, 111, 20));
        draw_line = new QPushButton(centralwidget);
        draw_line->setObjectName("draw_line");
        draw_line->setGeometry(QRect(730, 0, 71, 16));
        clear = new QPushButton(centralwidget);
        clear->setObjectName("clear");
        clear->setGeometry(QRect(670, 100, 81, 21));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(670, 50, 81, 31));
        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(670, 150, 91, 21));
        spinBox->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::UpDownArrows);
        spinBox->setMaximum(100);
        spinBox->setSingleStep(10);
        draw_dda = new QPushButton(centralwidget);
        draw_dda->setObjectName("draw_dda");
        draw_dda->setGeometry(QRect(670, 190, 111, 16));
        draw_bresenham = new QPushButton(centralwidget);
        draw_bresenham->setObjectName("draw_bresenham");
        draw_bresenham->setGeometry(QRect(670, 220, 111, 21));
        mouse_pressed_2 = new QLabel(centralwidget);
        mouse_pressed_2->setObjectName("mouse_pressed_2");
        mouse_pressed_2->setGeometry(QRect(640, 550, 141, 41));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        frame->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        mouse_movement->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        mouse_pressed->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        draw_line->setText(QCoreApplication::translate("MainWindow", "Draw Line", nullptr));
        clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Draw Grid", nullptr));
        draw_dda->setText(QCoreApplication::translate("MainWindow", "Draw DDA", nullptr));
        draw_bresenham->setText(QCoreApplication::translate("MainWindow", "Draw Bresenham", nullptr));
        mouse_pressed_2->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
