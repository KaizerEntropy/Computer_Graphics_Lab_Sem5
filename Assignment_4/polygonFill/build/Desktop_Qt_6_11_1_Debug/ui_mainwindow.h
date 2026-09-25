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
#include <QtWidgets/QComboBox>
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
    QLabel *mouse_pressed_2;
    QPushButton *pushButton_2;
    QPushButton *clear;
    QSpinBox *spinBox;
    QLabel *center_label;
    QLabel *time_label;
    QPushButton *set_vertex;
    QPushButton *close_polygon;
    QPushButton *boundary_fill;
    QPushButton *flood_fill;
    QPushButton *scanline_fill;
    QComboBox *color_box;
    QLabel *label_color;
    QPushButton *set_seed;
    QComboBox *connectivity_box;
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
        frame->setGeometry(QRect(20, 10, 500, 500));
        mouse_movement = new QLabel(centralwidget);
        mouse_movement->setObjectName("mouse_movement");
        mouse_movement->setGeometry(QRect(20, 520, 200, 20));
        mouse_pressed = new QLabel(centralwidget);
        mouse_pressed->setObjectName("mouse_pressed");
        mouse_pressed->setGeometry(QRect(240, 520, 200, 20));
        mouse_pressed_2 = new QLabel(centralwidget);
        mouse_pressed_2->setObjectName("mouse_pressed_2");
        mouse_pressed_2->setGeometry(QRect(460, 520, 200, 20));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(540, 10, 110, 35));
        clear = new QPushButton(centralwidget);
        clear->setObjectName("clear");
        clear->setGeometry(QRect(670, 10, 110, 35));
        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(540, 60, 240, 30));
        spinBox->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::UpDownArrows);
        spinBox->setMaximum(100);
        spinBox->setSingleStep(10);
        center_label = new QLabel(centralwidget);
        center_label->setObjectName("center_label");
        center_label->setGeometry(QRect(540, 100, 240, 25));
        time_label = new QLabel(centralwidget);
        time_label->setObjectName("time_label");
        time_label->setGeometry(QRect(540, 130, 240, 80));
        time_label->setWordWrap(true);
        set_vertex = new QPushButton(centralwidget);
        set_vertex->setObjectName("set_vertex");
        set_vertex->setGeometry(QRect(540, 220, 115, 35));
        close_polygon = new QPushButton(centralwidget);
        close_polygon->setObjectName("close_polygon");
        close_polygon->setGeometry(QRect(665, 220, 115, 35));
        boundary_fill = new QPushButton(centralwidget);
        boundary_fill->setObjectName("boundary_fill");
        boundary_fill->setGeometry(QRect(540, 270, 115, 40));
        flood_fill = new QPushButton(centralwidget);
        flood_fill->setObjectName("flood_fill");
        flood_fill->setGeometry(QRect(540, 320, 240, 40));
        scanline_fill = new QPushButton(centralwidget);
        scanline_fill->setObjectName("scanline_fill");
        scanline_fill->setGeometry(QRect(540, 370, 240, 40));
        color_box = new QComboBox(centralwidget);
        color_box->addItem(QString());
        color_box->addItem(QString());
        color_box->addItem(QString());
        color_box->addItem(QString());
        color_box->addItem(QString());
        color_box->setObjectName("color_box");
        color_box->setGeometry(QRect(540, 420, 115, 30));
        label_color = new QLabel(centralwidget);
        label_color->setObjectName("label_color");
        label_color->setGeometry(QRect(665, 420, 115, 30));
        set_seed = new QPushButton(centralwidget);
        set_seed->setObjectName("set_seed");
        set_seed->setGeometry(QRect(665, 270, 115, 40));
        connectivity_box = new QComboBox(centralwidget);
        connectivity_box->addItem(QString());
        connectivity_box->addItem(QString());
        connectivity_box->setObjectName("connectivity_box");
        connectivity_box->setGeometry(QRect(790, 270, 80, 40));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Polygon Fill Algorithms", nullptr));
        frame->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        mouse_movement->setText(QCoreApplication::translate("MainWindow", "Mouse Pos", nullptr));
        mouse_pressed->setText(QString());
        mouse_pressed_2->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Draw Grid", nullptr));
        clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        spinBox->setPrefix(QCoreApplication::translate("MainWindow", "Grid Size: ", nullptr));
        center_label->setText(QCoreApplication::translate("MainWindow", "Center: Not Selected", nullptr));
        time_label->setText(QCoreApplication::translate("MainWindow", "Stats will appear here...", nullptr));
        set_vertex->setText(QCoreApplication::translate("MainWindow", "Set Vertex", nullptr));
        close_polygon->setText(QCoreApplication::translate("MainWindow", "Close Polygon", nullptr));
        boundary_fill->setText(QCoreApplication::translate("MainWindow", "Boundary Fill", nullptr));
        flood_fill->setText(QCoreApplication::translate("MainWindow", "Flood Fill", nullptr));
        scanline_fill->setText(QCoreApplication::translate("MainWindow", "Scanline Fill", nullptr));
        color_box->setItemText(0, QCoreApplication::translate("MainWindow", "Yellow", nullptr));
        color_box->setItemText(1, QCoreApplication::translate("MainWindow", "Red", nullptr));
        color_box->setItemText(2, QCoreApplication::translate("MainWindow", "Green", nullptr));
        color_box->setItemText(3, QCoreApplication::translate("MainWindow", "Blue", nullptr));
        color_box->setItemText(4, QCoreApplication::translate("MainWindow", "White", nullptr));

        label_color->setText(QCoreApplication::translate("MainWindow", "Fill Color", nullptr));
        set_seed->setText(QCoreApplication::translate("MainWindow", "Set Seed", nullptr));
        connectivity_box->setItemText(0, QCoreApplication::translate("MainWindow", "4-Way", nullptr));
        connectivity_box->setItemText(1, QCoreApplication::translate("MainWindow", "8-Way", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
