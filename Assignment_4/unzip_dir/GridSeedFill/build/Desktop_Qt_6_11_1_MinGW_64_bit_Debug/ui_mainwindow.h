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
    QLabel *gridLabel;
    QSpinBox *spinBox;
    QPushButton *pushButton_2;
    QPushButton *draw_line;
    QPushButton *clear;
    QLabel *circleSectionLabel;
    QLabel *circleRadiusLabel;
    QSpinBox *circleRadiusBox;
    QPushButton *drawCirclePolar;
    QPushButton *drawCircleBresenham;
    QLabel *ellipseSectionLabel;
    QLabel *ellipseRxLabel;
    QSpinBox *ellipseRxBox;
    QLabel *ellipseRyLabel;
    QSpinBox *ellipseRyBox;
    QPushButton *drawEllipsePolar;
    QPushButton *drawEllipseBresenham;
    QPushButton *boundaryColorButton;
    QPushButton *fillColorButton;
    QLabel *algoLabel;
    QComboBox *algoCombo;
    QCheckBox *fillModeCheckBox;
    QCheckBox *animateCheckBox;
    QLabel *center_label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(830, 790);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame = new my_label(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(15, 15, 520, 520));
        mouse_movement = new QLabel(centralwidget);
        mouse_movement->setObjectName("mouse_movement");
        mouse_movement->setGeometry(QRect(555, 15, 230, 20));
        gridLabel = new QLabel(centralwidget);
        gridLabel->setObjectName("gridLabel");
        gridLabel->setGeometry(QRect(555, 50, 91, 16));
        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(555, 70, 91, 22));
        spinBox->setMinimum(4);
        spinBox->setMaximum(100);
        spinBox->setSingleStep(2);
        spinBox->setValue(20);
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(655, 70, 130, 22));
        draw_line = new QPushButton(centralwidget);
        draw_line->setObjectName("draw_line");
        draw_line->setGeometry(QRect(555, 105, 230, 28));
        clear = new QPushButton(centralwidget);
        clear->setObjectName("clear");
        clear->setGeometry(QRect(555, 140, 230, 28));
        circleSectionLabel = new QLabel(centralwidget);
        circleSectionLabel->setObjectName("circleSectionLabel");
        circleSectionLabel->setGeometry(QRect(555, 180, 230, 16));
        circleRadiusLabel = new QLabel(centralwidget);
        circleRadiusLabel->setObjectName("circleRadiusLabel");
        circleRadiusLabel->setGeometry(QRect(555, 202, 75, 20));
        circleRadiusBox = new QSpinBox(centralwidget);
        circleRadiusBox->setObjectName("circleRadiusBox");
        circleRadiusBox->setGeometry(QRect(635, 200, 150, 22));
        circleRadiusBox->setMinimum(1);
        circleRadiusBox->setMaximum(500);
        circleRadiusBox->setValue(8);
        drawCirclePolar = new QPushButton(centralwidget);
        drawCirclePolar->setObjectName("drawCirclePolar");
        drawCirclePolar->setGeometry(QRect(555, 228, 230, 26));
        drawCircleBresenham = new QPushButton(centralwidget);
        drawCircleBresenham->setObjectName("drawCircleBresenham");
        drawCircleBresenham->setGeometry(QRect(555, 258, 230, 26));
        ellipseSectionLabel = new QLabel(centralwidget);
        ellipseSectionLabel->setObjectName("ellipseSectionLabel");
        ellipseSectionLabel->setGeometry(QRect(555, 296, 230, 16));
        ellipseRxLabel = new QLabel(centralwidget);
        ellipseRxLabel->setObjectName("ellipseRxLabel");
        ellipseRxLabel->setGeometry(QRect(555, 318, 75, 20));
        ellipseRxBox = new QSpinBox(centralwidget);
        ellipseRxBox->setObjectName("ellipseRxBox");
        ellipseRxBox->setGeometry(QRect(635, 316, 150, 22));
        ellipseRxBox->setMinimum(1);
        ellipseRxBox->setMaximum(500);
        ellipseRxBox->setValue(12);
        ellipseRyLabel = new QLabel(centralwidget);
        ellipseRyLabel->setObjectName("ellipseRyLabel");
        ellipseRyLabel->setGeometry(QRect(555, 346, 75, 20));
        ellipseRyBox = new QSpinBox(centralwidget);
        ellipseRyBox->setObjectName("ellipseRyBox");
        ellipseRyBox->setGeometry(QRect(635, 344, 150, 22));
        ellipseRyBox->setMinimum(1);
        ellipseRyBox->setMaximum(500);
        ellipseRyBox->setValue(7);
        drawEllipsePolar = new QPushButton(centralwidget);
        drawEllipsePolar->setObjectName("drawEllipsePolar");
        drawEllipsePolar->setGeometry(QRect(555, 374, 230, 26));
        drawEllipseBresenham = new QPushButton(centralwidget);
        drawEllipseBresenham->setObjectName("drawEllipseBresenham");
        drawEllipseBresenham->setGeometry(QRect(555, 404, 230, 26));
        boundaryColorButton = new QPushButton(centralwidget);
        boundaryColorButton->setObjectName("boundaryColorButton");
        boundaryColorButton->setGeometry(QRect(555, 442, 230, 26));
        fillColorButton = new QPushButton(centralwidget);
        fillColorButton->setObjectName("fillColorButton");
        fillColorButton->setGeometry(QRect(555, 472, 230, 26));
        algoLabel = new QLabel(centralwidget);
        algoLabel->setObjectName("algoLabel");
        algoLabel->setGeometry(QRect(555, 506, 151, 16));
        algoCombo = new QComboBox(centralwidget);
        algoCombo->setObjectName("algoCombo");
        algoCombo->setGeometry(QRect(555, 526, 230, 24));
        fillModeCheckBox = new QCheckBox(centralwidget);
        fillModeCheckBox->setObjectName("fillModeCheckBox");
        fillModeCheckBox->setGeometry(QRect(555, 558, 230, 20));
        animateCheckBox = new QCheckBox(centralwidget);
        animateCheckBox->setObjectName("animateCheckBox");
        animateCheckBox->setGeometry(QRect(555, 582, 230, 20));
        center_label = new QLabel(centralwidget);
        center_label->setObjectName("center_label");
        center_label->setGeometry(QRect(555, 610, 250, 160));
        center_label->setWordWrap(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 830, 17));
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
        frame->setText(QString());
        mouse_movement->setText(QCoreApplication::translate("MainWindow", "X : 0, Y : 0", nullptr));
        gridLabel->setText(QCoreApplication::translate("MainWindow", "Grid size:", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Reset Grid (10px)", nullptr));
        draw_line->setText(QCoreApplication::translate("MainWindow", "Draw Line (between the 2 picked points)", nullptr));
        clear->setText(QCoreApplication::translate("MainWindow", "Clear Canvas", nullptr));
        circleSectionLabel->setText(QCoreApplication::translate("MainWindow", "Circle (click grid to pick centre)", nullptr));
        circleRadiusLabel->setText(QCoreApplication::translate("MainWindow", "Radius:", nullptr));
        drawCirclePolar->setText(QCoreApplication::translate("MainWindow", "Draw Circle - Polar", nullptr));
        drawCircleBresenham->setText(QCoreApplication::translate("MainWindow", "Draw Circle - Bresenham (Midpoint)", nullptr));
        ellipseSectionLabel->setText(QCoreApplication::translate("MainWindow", "Ellipse (click grid to pick centre)", nullptr));
        ellipseRxLabel->setText(QCoreApplication::translate("MainWindow", "rx:", nullptr));
        ellipseRyLabel->setText(QCoreApplication::translate("MainWindow", "ry:", nullptr));
        drawEllipsePolar->setText(QCoreApplication::translate("MainWindow", "Draw Ellipse - Polar", nullptr));
        drawEllipseBresenham->setText(QCoreApplication::translate("MainWindow", "Draw Ellipse - Bresenham (Midpoint)", nullptr));
        boundaryColorButton->setText(QCoreApplication::translate("MainWindow", "Boundary Colour...", nullptr));
        fillColorButton->setText(QCoreApplication::translate("MainWindow", "Fill Colour...", nullptr));
        algoLabel->setText(QCoreApplication::translate("MainWindow", "Fill algorithm:", nullptr));
        fillModeCheckBox->setText(QCoreApplication::translate("MainWindow", "Fill mode (click = seed point)", nullptr));
        animateCheckBox->setText(QCoreApplication::translate("MainWindow", "Animate fill", nullptr));
        center_label->setText(QCoreApplication::translate("MainWindow", "1) Lines: click 2 points, press Draw Line. Repeat to close the shape.\n"
"2) Circle/Ellipse: click 1 point (centre), set radius or rx/ry, press Polar or Bresenham.\n"
"3) Pick a fill colour + algorithm, tick \"Fill mode\", click inside the shape.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
