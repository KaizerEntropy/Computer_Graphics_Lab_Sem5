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
#include <QtWidgets/QCheckBox>
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
    QLabel *label_rx;
    QSpinBox *rxBox;
    QLabel *label_ry;
    QSpinBox *ryBox;
    QCheckBox *animate_checkBox;
    QPushButton *draw_polar;
    QPushButton *draw_midpoint_ellipse;
    QPushButton *draw_cartesian;
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
        label_rx = new QLabel(centralwidget);
        label_rx->setObjectName("label_rx");
        label_rx->setGeometry(QRect(540, 220, 30, 30));
        rxBox = new QSpinBox(centralwidget);
        rxBox->setObjectName("rxBox");
        rxBox->setGeometry(QRect(570, 220, 80, 30));
        label_ry = new QLabel(centralwidget);
        label_ry->setObjectName("label_ry");
        label_ry->setGeometry(QRect(670, 220, 30, 30));
        ryBox = new QSpinBox(centralwidget);
        ryBox->setObjectName("ryBox");
        ryBox->setGeometry(QRect(700, 220, 80, 30));
        animate_checkBox = new QCheckBox(centralwidget);
        animate_checkBox->setObjectName("animate_checkBox");
        animate_checkBox->setGeometry(QRect(540, 270, 240, 30));
        draw_polar = new QPushButton(centralwidget);
        draw_polar->setObjectName("draw_polar");
        draw_polar->setGeometry(QRect(540, 320, 240, 40));
        draw_midpoint_ellipse = new QPushButton(centralwidget);
        draw_midpoint_ellipse->setObjectName("draw_midpoint_ellipse");
        draw_midpoint_ellipse->setGeometry(QRect(540, 370, 240, 40));
        draw_cartesian = new QPushButton(centralwidget);
        draw_cartesian->setObjectName("draw_cartesian");
        draw_cartesian->setGeometry(QRect(540, 420, 240, 40));
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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Ellipse Drawing Algorithms", nullptr));
        frame->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        mouse_movement->setText(QCoreApplication::translate("MainWindow", "Mouse Pos", nullptr));
        mouse_pressed->setText(QString());
        mouse_pressed_2->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Draw Grid", nullptr));
        clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        spinBox->setPrefix(QCoreApplication::translate("MainWindow", "Grid Size: ", nullptr));
        center_label->setText(QCoreApplication::translate("MainWindow", "Center: Not Selected", nullptr));
        time_label->setText(QCoreApplication::translate("MainWindow", "Stats will appear here...", nullptr));
        label_rx->setText(QCoreApplication::translate("MainWindow", "Rx:", nullptr));
        label_ry->setText(QCoreApplication::translate("MainWindow", "Ry:", nullptr));
        animate_checkBox->setText(QCoreApplication::translate("MainWindow", "Animate 4-Point Symmetry", nullptr));
        draw_polar->setText(QCoreApplication::translate("MainWindow", "Draw Polar Ellipse", nullptr));
        draw_midpoint_ellipse->setText(QCoreApplication::translate("MainWindow", "Draw Midpoint Ellipse", nullptr));
        draw_cartesian->setText(QCoreApplication::translate("MainWindow", "Draw Cartesian Ellipse", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
