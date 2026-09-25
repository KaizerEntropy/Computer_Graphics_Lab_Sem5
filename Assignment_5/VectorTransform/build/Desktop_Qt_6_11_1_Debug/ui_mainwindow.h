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
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "canvas.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainHLayout;
    QScrollArea *leftControlScroll;
    QWidget *leftControlWidget;
    QVBoxLayout *leftControlVLayout;
    QGroupBox *groupSelection;
    QVBoxLayout *selectionLayout;
    QLabel *lblScopeInfo;
    QHBoxLayout *selectDropdownLayout;
    QComboBox *comboSelectShape;
    QPushButton *btnDeselectAll;
    QLabel *lblSelectHint;
    QGroupBox *groupQuickCommand;
    QHBoxLayout *quickCmdLayout;
    QLineEdit *txtQuickCommand;
    QPushButton *btnRunQuickCommand;
    QGroupBox *groupTransforms;
    QVBoxLayout *transformsVLayout;
    QTabWidget *tabTransforms;
    QWidget *tabTranslate;
    QVBoxLayout *transLayout;
    QFormLayout *transForm;
    QLabel *lblTx;
    QSpinBox *spinTx;
    QLabel *lblTy;
    QSpinBox *spinTy;
    QPushButton *btnApplyTranslate;
    QGridLayout *nudgeLayout;
    QPushButton *btnNudgeUp;
    QPushButton *btnNudgeLeft;
    QPushButton *btnNudgeReset;
    QPushButton *btnNudgeRight;
    QPushButton *btnNudgeDown;
    QWidget *tabRotate;
    QVBoxLayout *rotLayout;
    QHBoxLayout *rotAngleLayout;
    QLabel *lblRotAngle;
    QDoubleSpinBox *spinRotAngle;
    QGroupBox *groupRotPivot;
    QVBoxLayout *rotPivotVLayout;
    QRadioButton *radioRotOrigin;
    QRadioButton *radioRotCentroid;
    QRadioButton *radioRotCustom;
    QHBoxLayout *rotCustomCoordLayout;
    QLabel *lblRotPx;
    QSpinBox *spinRotPx;
    QLabel *lblRotPy;
    QSpinBox *spinRotPy;
    QPushButton *btnPickPivot;
    QPushButton *btnApplyRotate;
    QHBoxLayout *quickRotLayout;
    QPushButton *btnQuickRot45;
    QPushButton *btnQuickRotNeg45;
    QPushButton *btnQuickRot90;
    QPushButton *btnQuickRot180;
    QWidget *tabScale;
    QVBoxLayout *scaleLayout;
    QFormLayout *scaleForm;
    QLabel *lblSx;
    QDoubleSpinBox *spinScaleX;
    QLabel *lblSy;
    QDoubleSpinBox *spinScaleY;
    QCheckBox *chkUniformScale;
    QGroupBox *groupScaleFixed;
    QVBoxLayout *scaleFixedVLayout;
    QRadioButton *radioScaleOrigin;
    QRadioButton *radioScaleCentroid;
    QRadioButton *radioScaleCustom;
    QHBoxLayout *scaleCustomCoordLayout;
    QLabel *lblScalePx;
    QSpinBox *spinScalePx;
    QLabel *lblScalePy;
    QSpinBox *spinScalePy;
    QPushButton *btnApplyScale;
    QHBoxLayout *quickScaleLayout;
    QPushButton *btnQuickScale2;
    QPushButton *btnQuickScaleHalf;
    QPushButton *btnQuickScale15;
    QPushButton *btnQuickScaleFlip;
    QWidget *tabShear;
    QVBoxLayout *shearLayout;
    QFormLayout *shearForm;
    QLabel *lblShx;
    QDoubleSpinBox *spinShearX;
    QLabel *lblShy;
    QDoubleSpinBox *spinShearY;
    QLabel *lblYref;
    QSpinBox *spinShearYRef;
    QLabel *lblXref;
    QSpinBox *spinShearXRef;
    QPushButton *btnApplyShear;
    QWidget *tabReflect;
    QVBoxLayout *reflectLayout;
    QLabel *lblStandardRefTitle;
    QGridLayout *stdReflectGrid;
    QPushButton *btnReflectX;
    QPushButton *btnReflectY;
    QPushButton *btnReflectOrigin;
    QPushButton *btnReflectLineYeqX;
    QPushButton *btnReflectLineYeqNegX;
    QGroupBox *groupArbitraryLine;
    QVBoxLayout *arbLineVLayout;
    QFormLayout *arbLineForm;
    QLabel *lblSlope;
    QDoubleSpinBox *spinReflectSlope;
    QLabel *lblIntercept;
    QDoubleSpinBox *spinReflectIntercept;
    QHBoxLayout *arbLineToolsLayout;
    QPushButton *btnPickReflectLine;
    QPushButton *btnApplyReflectArbitrary;
    QWidget *tabCustomMatrix;
    QVBoxLayout *customMatLayout;
    QLabel *lblCustomMatInfo;
    QGridLayout *customMatrixGrid;
    QDoubleSpinBox *spinM00;
    QDoubleSpinBox *spinM01;
    QDoubleSpinBox *spinM02;
    QDoubleSpinBox *spinM10;
    QDoubleSpinBox *spinM11;
    QDoubleSpinBox *spinM12;
    QDoubleSpinBox *spinM20;
    QDoubleSpinBox *spinM21;
    QDoubleSpinBox *spinM22;
    QPushButton *btnApplyCustomMatrix;
    QGroupBox *groupFills;
    QVBoxLayout *fillsLayout;
    QComboBox *comboFillAlgorithm;
    QHBoxLayout *fillActionLayout;
    QPushButton *btnExecuteFill;
    QPushButton *btnClearFill;
    QGroupBox *groupShapes;
    QVBoxLayout *shapesVLayout;
    QGridLayout *shapeButtonsGrid;
    QPushButton *btnDrawPolygon;
    QPushButton *btnDrawCircle;
    QPushButton *btnClosePolygon;
    QPushButton *btnUndoShape;
    QLabel *lblPresetsHeader;
    QGridLayout *presetsGrid;
    QPushButton *btnPresetTriangle;
    QPushButton *btnPresetConvex;
    QPushButton *btnPresetHouse;
    QPushButton *btnPresetStar;
    QPushButton *btnPresetConcave;
    QPushButton *btnPresetNested;
    QPushButton *btnPresetMultiScene;
    QVBoxLayout *canvasAndMatrixVLayout;
    Canvas *canvasWidget;
    QHBoxLayout *bottomDashboardLayout;
    QGroupBox *groupMatrixViewer;
    QVBoxLayout *matrixBoxVLayout;
    QLabel *lblMatrixFormula;
    QTextEdit *txtMatrixDisplay;
    QHBoxLayout *matrixActionsLayout;
    QLabel *lblMatrixDet;
    QSpacerItem *spacerItem;
    QPushButton *btnUndoTransform;
    QPushButton *btnResetTransform;
    QGroupBox *groupHistory;
    QVBoxLayout *historyVLayout;
    QTableWidget *tableHistory;
    QGroupBox *groupView;
    QGridLayout *viewGridLayout;
    QCheckBox *chkShowGhost;
    QCheckBox *chkShowPivot;
    QCheckBox *chkShowReflectLine;
    QCheckBox *chkShowLabels;
    QCheckBox *chkShowGrid;
    QCheckBox *chkShowAxes;
    QLabel *lblGridSize;
    QSpinBox *spinGridSize;
    QPushButton *btnClearAll;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1450, 940);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainHLayout = new QHBoxLayout(centralwidget);
        mainHLayout->setSpacing(8);
        mainHLayout->setObjectName("mainHLayout");
        mainHLayout->setContentsMargins(8, 8, 8, 8);
        leftControlScroll = new QScrollArea(centralwidget);
        leftControlScroll->setObjectName("leftControlScroll");
        leftControlScroll->setMinimumSize(QSize(440, 0));
        leftControlScroll->setMaximumSize(QSize(460, 16777215));
        leftControlScroll->setWidgetResizable(true);
        leftControlWidget = new QWidget();
        leftControlWidget->setObjectName("leftControlWidget");
        leftControlWidget->setGeometry(QRect(0, 0, 442, 1200));
        leftControlVLayout = new QVBoxLayout(leftControlWidget);
        leftControlVLayout->setSpacing(8);
        leftControlVLayout->setObjectName("leftControlVLayout");
        leftControlVLayout->setContentsMargins(6, 6, 6, 6);
        groupSelection = new QGroupBox(leftControlWidget);
        groupSelection->setObjectName("groupSelection");
        selectionLayout = new QVBoxLayout(groupSelection);
        selectionLayout->setSpacing(6);
        selectionLayout->setObjectName("selectionLayout");
        lblScopeInfo = new QLabel(groupSelection);
        lblScopeInfo->setObjectName("lblScopeInfo");

        selectionLayout->addWidget(lblScopeInfo);

        selectDropdownLayout = new QHBoxLayout();
        selectDropdownLayout->setObjectName("selectDropdownLayout");
        comboSelectShape = new QComboBox(groupSelection);
        comboSelectShape->addItem(QString());
        comboSelectShape->setObjectName("comboSelectShape");

        selectDropdownLayout->addWidget(comboSelectShape);

        btnDeselectAll = new QPushButton(groupSelection);
        btnDeselectAll->setObjectName("btnDeselectAll");

        selectDropdownLayout->addWidget(btnDeselectAll);


        selectionLayout->addLayout(selectDropdownLayout);

        lblSelectHint = new QLabel(groupSelection);
        lblSelectHint->setObjectName("lblSelectHint");
        lblSelectHint->setWordWrap(true);

        selectionLayout->addWidget(lblSelectHint);


        leftControlVLayout->addWidget(groupSelection);

        groupQuickCommand = new QGroupBox(leftControlWidget);
        groupQuickCommand->setObjectName("groupQuickCommand");
        quickCmdLayout = new QHBoxLayout(groupQuickCommand);
        quickCmdLayout->setSpacing(6);
        quickCmdLayout->setObjectName("quickCmdLayout");
        txtQuickCommand = new QLineEdit(groupQuickCommand);
        txtQuickCommand->setObjectName("txtQuickCommand");

        quickCmdLayout->addWidget(txtQuickCommand);

        btnRunQuickCommand = new QPushButton(groupQuickCommand);
        btnRunQuickCommand->setObjectName("btnRunQuickCommand");

        quickCmdLayout->addWidget(btnRunQuickCommand);


        leftControlVLayout->addWidget(groupQuickCommand);

        groupTransforms = new QGroupBox(leftControlWidget);
        groupTransforms->setObjectName("groupTransforms");
        transformsVLayout = new QVBoxLayout(groupTransforms);
        transformsVLayout->setSpacing(6);
        transformsVLayout->setObjectName("transformsVLayout");
        tabTransforms = new QTabWidget(groupTransforms);
        tabTransforms->setObjectName("tabTransforms");
        tabTranslate = new QWidget();
        tabTranslate->setObjectName("tabTranslate");
        transLayout = new QVBoxLayout(tabTranslate);
        transLayout->setSpacing(6);
        transLayout->setObjectName("transLayout");
        transForm = new QFormLayout();
        transForm->setObjectName("transForm");
        lblTx = new QLabel(tabTranslate);
        lblTx->setObjectName("lblTx");

        transForm->setWidget(0, QFormLayout::ItemRole::LabelRole, lblTx);

        spinTx = new QSpinBox(tabTranslate);
        spinTx->setObjectName("spinTx");
        spinTx->setMinimum(-1000);
        spinTx->setMaximum(1000);
        spinTx->setValue(5);

        transForm->setWidget(0, QFormLayout::ItemRole::FieldRole, spinTx);

        lblTy = new QLabel(tabTranslate);
        lblTy->setObjectName("lblTy");

        transForm->setWidget(1, QFormLayout::ItemRole::LabelRole, lblTy);

        spinTy = new QSpinBox(tabTranslate);
        spinTy->setObjectName("spinTy");
        spinTy->setMinimum(-1000);
        spinTy->setMaximum(1000);
        spinTy->setValue(5);

        transForm->setWidget(1, QFormLayout::ItemRole::FieldRole, spinTy);


        transLayout->addLayout(transForm);

        btnApplyTranslate = new QPushButton(tabTranslate);
        btnApplyTranslate->setObjectName("btnApplyTranslate");

        transLayout->addWidget(btnApplyTranslate);

        nudgeLayout = new QGridLayout();
        nudgeLayout->setObjectName("nudgeLayout");
        btnNudgeUp = new QPushButton(tabTranslate);
        btnNudgeUp->setObjectName("btnNudgeUp");

        nudgeLayout->addWidget(btnNudgeUp, 0, 1, 1, 1);

        btnNudgeLeft = new QPushButton(tabTranslate);
        btnNudgeLeft->setObjectName("btnNudgeLeft");

        nudgeLayout->addWidget(btnNudgeLeft, 1, 0, 1, 1);

        btnNudgeReset = new QPushButton(tabTranslate);
        btnNudgeReset->setObjectName("btnNudgeReset");

        nudgeLayout->addWidget(btnNudgeReset, 1, 1, 1, 1);

        btnNudgeRight = new QPushButton(tabTranslate);
        btnNudgeRight->setObjectName("btnNudgeRight");

        nudgeLayout->addWidget(btnNudgeRight, 1, 2, 1, 1);

        btnNudgeDown = new QPushButton(tabTranslate);
        btnNudgeDown->setObjectName("btnNudgeDown");

        nudgeLayout->addWidget(btnNudgeDown, 2, 1, 1, 1);


        transLayout->addLayout(nudgeLayout);

        tabTransforms->addTab(tabTranslate, QString());
        tabRotate = new QWidget();
        tabRotate->setObjectName("tabRotate");
        rotLayout = new QVBoxLayout(tabRotate);
        rotLayout->setSpacing(6);
        rotLayout->setObjectName("rotLayout");
        rotAngleLayout = new QHBoxLayout();
        rotAngleLayout->setObjectName("rotAngleLayout");
        lblRotAngle = new QLabel(tabRotate);
        lblRotAngle->setObjectName("lblRotAngle");

        rotAngleLayout->addWidget(lblRotAngle);

        spinRotAngle = new QDoubleSpinBox(tabRotate);
        spinRotAngle->setObjectName("spinRotAngle");
        spinRotAngle->setDecimals(2);
        spinRotAngle->setMinimum(-3600.000000000000000);
        spinRotAngle->setMaximum(3600.000000000000000);
        spinRotAngle->setValue(45.000000000000000);
        spinRotAngle->setSingleStep(1.000000000000000);

        rotAngleLayout->addWidget(spinRotAngle);


        rotLayout->addLayout(rotAngleLayout);

        groupRotPivot = new QGroupBox(tabRotate);
        groupRotPivot->setObjectName("groupRotPivot");
        rotPivotVLayout = new QVBoxLayout(groupRotPivot);
        rotPivotVLayout->setObjectName("rotPivotVLayout");
        radioRotOrigin = new QRadioButton(groupRotPivot);
        radioRotOrigin->setObjectName("radioRotOrigin");
        radioRotOrigin->setChecked(true);

        rotPivotVLayout->addWidget(radioRotOrigin);

        radioRotCentroid = new QRadioButton(groupRotPivot);
        radioRotCentroid->setObjectName("radioRotCentroid");

        rotPivotVLayout->addWidget(radioRotCentroid);

        radioRotCustom = new QRadioButton(groupRotPivot);
        radioRotCustom->setObjectName("radioRotCustom");

        rotPivotVLayout->addWidget(radioRotCustom);

        rotCustomCoordLayout = new QHBoxLayout();
        rotCustomCoordLayout->setObjectName("rotCustomCoordLayout");
        lblRotPx = new QLabel(groupRotPivot);
        lblRotPx->setObjectName("lblRotPx");

        rotCustomCoordLayout->addWidget(lblRotPx);

        spinRotPx = new QSpinBox(groupRotPivot);
        spinRotPx->setObjectName("spinRotPx");
        spinRotPx->setMinimum(-1000);
        spinRotPx->setMaximum(1000);
        spinRotPx->setValue(0);

        rotCustomCoordLayout->addWidget(spinRotPx);

        lblRotPy = new QLabel(groupRotPivot);
        lblRotPy->setObjectName("lblRotPy");

        rotCustomCoordLayout->addWidget(lblRotPy);

        spinRotPy = new QSpinBox(groupRotPivot);
        spinRotPy->setObjectName("spinRotPy");
        spinRotPy->setMinimum(-1000);
        spinRotPy->setMaximum(1000);
        spinRotPy->setValue(0);

        rotCustomCoordLayout->addWidget(spinRotPy);

        btnPickPivot = new QPushButton(groupRotPivot);
        btnPickPivot->setObjectName("btnPickPivot");

        rotCustomCoordLayout->addWidget(btnPickPivot);


        rotPivotVLayout->addLayout(rotCustomCoordLayout);


        rotLayout->addWidget(groupRotPivot);

        btnApplyRotate = new QPushButton(tabRotate);
        btnApplyRotate->setObjectName("btnApplyRotate");

        rotLayout->addWidget(btnApplyRotate);

        quickRotLayout = new QHBoxLayout();
        quickRotLayout->setObjectName("quickRotLayout");
        btnQuickRot45 = new QPushButton(tabRotate);
        btnQuickRot45->setObjectName("btnQuickRot45");

        quickRotLayout->addWidget(btnQuickRot45);

        btnQuickRotNeg45 = new QPushButton(tabRotate);
        btnQuickRotNeg45->setObjectName("btnQuickRotNeg45");

        quickRotLayout->addWidget(btnQuickRotNeg45);

        btnQuickRot90 = new QPushButton(tabRotate);
        btnQuickRot90->setObjectName("btnQuickRot90");

        quickRotLayout->addWidget(btnQuickRot90);

        btnQuickRot180 = new QPushButton(tabRotate);
        btnQuickRot180->setObjectName("btnQuickRot180");

        quickRotLayout->addWidget(btnQuickRot180);


        rotLayout->addLayout(quickRotLayout);

        tabTransforms->addTab(tabRotate, QString());
        tabScale = new QWidget();
        tabScale->setObjectName("tabScale");
        scaleLayout = new QVBoxLayout(tabScale);
        scaleLayout->setSpacing(6);
        scaleLayout->setObjectName("scaleLayout");
        scaleForm = new QFormLayout();
        scaleForm->setObjectName("scaleForm");
        lblSx = new QLabel(tabScale);
        lblSx->setObjectName("lblSx");

        scaleForm->setWidget(0, QFormLayout::ItemRole::LabelRole, lblSx);

        spinScaleX = new QDoubleSpinBox(tabScale);
        spinScaleX->setObjectName("spinScaleX");
        spinScaleX->setDecimals(3);
        spinScaleX->setMinimum(-100.000000000000000);
        spinScaleX->setMaximum(100.000000000000000);
        spinScaleX->setValue(1.500000000000000);
        spinScaleX->setSingleStep(0.100000000000000);

        scaleForm->setWidget(0, QFormLayout::ItemRole::FieldRole, spinScaleX);

        lblSy = new QLabel(tabScale);
        lblSy->setObjectName("lblSy");

        scaleForm->setWidget(1, QFormLayout::ItemRole::LabelRole, lblSy);

        spinScaleY = new QDoubleSpinBox(tabScale);
        spinScaleY->setObjectName("spinScaleY");
        spinScaleY->setDecimals(3);
        spinScaleY->setMinimum(-100.000000000000000);
        spinScaleY->setMaximum(100.000000000000000);
        spinScaleY->setValue(1.500000000000000);
        spinScaleY->setSingleStep(0.100000000000000);

        scaleForm->setWidget(1, QFormLayout::ItemRole::FieldRole, spinScaleY);


        scaleLayout->addLayout(scaleForm);

        chkUniformScale = new QCheckBox(tabScale);
        chkUniformScale->setObjectName("chkUniformScale");
        chkUniformScale->setChecked(false);

        scaleLayout->addWidget(chkUniformScale);

        groupScaleFixed = new QGroupBox(tabScale);
        groupScaleFixed->setObjectName("groupScaleFixed");
        scaleFixedVLayout = new QVBoxLayout(groupScaleFixed);
        scaleFixedVLayout->setObjectName("scaleFixedVLayout");
        radioScaleOrigin = new QRadioButton(groupScaleFixed);
        radioScaleOrigin->setObjectName("radioScaleOrigin");

        scaleFixedVLayout->addWidget(radioScaleOrigin);

        radioScaleCentroid = new QRadioButton(groupScaleFixed);
        radioScaleCentroid->setObjectName("radioScaleCentroid");
        radioScaleCentroid->setChecked(true);

        scaleFixedVLayout->addWidget(radioScaleCentroid);

        radioScaleCustom = new QRadioButton(groupScaleFixed);
        radioScaleCustom->setObjectName("radioScaleCustom");

        scaleFixedVLayout->addWidget(radioScaleCustom);

        scaleCustomCoordLayout = new QHBoxLayout();
        scaleCustomCoordLayout->setObjectName("scaleCustomCoordLayout");
        lblScalePx = new QLabel(groupScaleFixed);
        lblScalePx->setObjectName("lblScalePx");

        scaleCustomCoordLayout->addWidget(lblScalePx);

        spinScalePx = new QSpinBox(groupScaleFixed);
        spinScalePx->setObjectName("spinScalePx");
        spinScalePx->setMinimum(-1000);
        spinScalePx->setMaximum(1000);
        spinScalePx->setValue(0);

        scaleCustomCoordLayout->addWidget(spinScalePx);

        lblScalePy = new QLabel(groupScaleFixed);
        lblScalePy->setObjectName("lblScalePy");

        scaleCustomCoordLayout->addWidget(lblScalePy);

        spinScalePy = new QSpinBox(groupScaleFixed);
        spinScalePy->setObjectName("spinScalePy");
        spinScalePy->setMinimum(-1000);
        spinScalePy->setMaximum(1000);
        spinScalePy->setValue(0);

        scaleCustomCoordLayout->addWidget(spinScalePy);


        scaleFixedVLayout->addLayout(scaleCustomCoordLayout);


        scaleLayout->addWidget(groupScaleFixed);

        btnApplyScale = new QPushButton(tabScale);
        btnApplyScale->setObjectName("btnApplyScale");

        scaleLayout->addWidget(btnApplyScale);

        quickScaleLayout = new QHBoxLayout();
        quickScaleLayout->setObjectName("quickScaleLayout");
        btnQuickScale2 = new QPushButton(tabScale);
        btnQuickScale2->setObjectName("btnQuickScale2");

        quickScaleLayout->addWidget(btnQuickScale2);

        btnQuickScaleHalf = new QPushButton(tabScale);
        btnQuickScaleHalf->setObjectName("btnQuickScaleHalf");

        quickScaleLayout->addWidget(btnQuickScaleHalf);

        btnQuickScale15 = new QPushButton(tabScale);
        btnQuickScale15->setObjectName("btnQuickScale15");

        quickScaleLayout->addWidget(btnQuickScale15);

        btnQuickScaleFlip = new QPushButton(tabScale);
        btnQuickScaleFlip->setObjectName("btnQuickScaleFlip");

        quickScaleLayout->addWidget(btnQuickScaleFlip);


        scaleLayout->addLayout(quickScaleLayout);

        tabTransforms->addTab(tabScale, QString());
        tabShear = new QWidget();
        tabShear->setObjectName("tabShear");
        shearLayout = new QVBoxLayout(tabShear);
        shearLayout->setSpacing(6);
        shearLayout->setObjectName("shearLayout");
        shearForm = new QFormLayout();
        shearForm->setObjectName("shearForm");
        lblShx = new QLabel(tabShear);
        lblShx->setObjectName("lblShx");

        shearForm->setWidget(0, QFormLayout::ItemRole::LabelRole, lblShx);

        spinShearX = new QDoubleSpinBox(tabShear);
        spinShearX->setObjectName("spinShearX");
        spinShearX->setDecimals(3);
        spinShearX->setMinimum(-100.000000000000000);
        spinShearX->setMaximum(100.000000000000000);
        spinShearX->setValue(0.500000000000000);
        spinShearX->setSingleStep(0.100000000000000);

        shearForm->setWidget(0, QFormLayout::ItemRole::FieldRole, spinShearX);

        lblShy = new QLabel(tabShear);
        lblShy->setObjectName("lblShy");

        shearForm->setWidget(1, QFormLayout::ItemRole::LabelRole, lblShy);

        spinShearY = new QDoubleSpinBox(tabShear);
        spinShearY->setObjectName("spinShearY");
        spinShearY->setDecimals(3);
        spinShearY->setMinimum(-100.000000000000000);
        spinShearY->setMaximum(100.000000000000000);
        spinShearY->setValue(0.000000000000000);
        spinShearY->setSingleStep(0.100000000000000);

        shearForm->setWidget(1, QFormLayout::ItemRole::FieldRole, spinShearY);

        lblYref = new QLabel(tabShear);
        lblYref->setObjectName("lblYref");

        shearForm->setWidget(2, QFormLayout::ItemRole::LabelRole, lblYref);

        spinShearYRef = new QSpinBox(tabShear);
        spinShearYRef->setObjectName("spinShearYRef");
        spinShearYRef->setMinimum(-1000);
        spinShearYRef->setMaximum(1000);
        spinShearYRef->setValue(0);

        shearForm->setWidget(2, QFormLayout::ItemRole::FieldRole, spinShearYRef);

        lblXref = new QLabel(tabShear);
        lblXref->setObjectName("lblXref");

        shearForm->setWidget(3, QFormLayout::ItemRole::LabelRole, lblXref);

        spinShearXRef = new QSpinBox(tabShear);
        spinShearXRef->setObjectName("spinShearXRef");
        spinShearXRef->setMinimum(-1000);
        spinShearXRef->setMaximum(1000);
        spinShearXRef->setValue(0);

        shearForm->setWidget(3, QFormLayout::ItemRole::FieldRole, spinShearXRef);


        shearLayout->addLayout(shearForm);

        btnApplyShear = new QPushButton(tabShear);
        btnApplyShear->setObjectName("btnApplyShear");

        shearLayout->addWidget(btnApplyShear);

        tabTransforms->addTab(tabShear, QString());
        tabReflect = new QWidget();
        tabReflect->setObjectName("tabReflect");
        reflectLayout = new QVBoxLayout(tabReflect);
        reflectLayout->setSpacing(6);
        reflectLayout->setObjectName("reflectLayout");
        lblStandardRefTitle = new QLabel(tabReflect);
        lblStandardRefTitle->setObjectName("lblStandardRefTitle");

        reflectLayout->addWidget(lblStandardRefTitle);

        stdReflectGrid = new QGridLayout();
        stdReflectGrid->setObjectName("stdReflectGrid");
        btnReflectX = new QPushButton(tabReflect);
        btnReflectX->setObjectName("btnReflectX");

        stdReflectGrid->addWidget(btnReflectX, 0, 0, 1, 1);

        btnReflectY = new QPushButton(tabReflect);
        btnReflectY->setObjectName("btnReflectY");

        stdReflectGrid->addWidget(btnReflectY, 0, 1, 1, 1);

        btnReflectOrigin = new QPushButton(tabReflect);
        btnReflectOrigin->setObjectName("btnReflectOrigin");

        stdReflectGrid->addWidget(btnReflectOrigin, 1, 0, 1, 1);

        btnReflectLineYeqX = new QPushButton(tabReflect);
        btnReflectLineYeqX->setObjectName("btnReflectLineYeqX");

        stdReflectGrid->addWidget(btnReflectLineYeqX, 1, 1, 1, 1);

        btnReflectLineYeqNegX = new QPushButton(tabReflect);
        btnReflectLineYeqNegX->setObjectName("btnReflectLineYeqNegX");

        stdReflectGrid->addWidget(btnReflectLineYeqNegX, 2, 0, 1, 2);


        reflectLayout->addLayout(stdReflectGrid);

        groupArbitraryLine = new QGroupBox(tabReflect);
        groupArbitraryLine->setObjectName("groupArbitraryLine");
        arbLineVLayout = new QVBoxLayout(groupArbitraryLine);
        arbLineVLayout->setObjectName("arbLineVLayout");
        arbLineForm = new QFormLayout();
        arbLineForm->setObjectName("arbLineForm");
        lblSlope = new QLabel(groupArbitraryLine);
        lblSlope->setObjectName("lblSlope");

        arbLineForm->setWidget(0, QFormLayout::ItemRole::LabelRole, lblSlope);

        spinReflectSlope = new QDoubleSpinBox(groupArbitraryLine);
        spinReflectSlope->setObjectName("spinReflectSlope");
        spinReflectSlope->setDecimals(3);
        spinReflectSlope->setMinimum(-1000.000000000000000);
        spinReflectSlope->setMaximum(1000.000000000000000);
        spinReflectSlope->setValue(1.000000000000000);
        spinReflectSlope->setSingleStep(0.500000000000000);

        arbLineForm->setWidget(0, QFormLayout::ItemRole::FieldRole, spinReflectSlope);

        lblIntercept = new QLabel(groupArbitraryLine);
        lblIntercept->setObjectName("lblIntercept");

        arbLineForm->setWidget(1, QFormLayout::ItemRole::LabelRole, lblIntercept);

        spinReflectIntercept = new QDoubleSpinBox(groupArbitraryLine);
        spinReflectIntercept->setObjectName("spinReflectIntercept");
        spinReflectIntercept->setDecimals(2);
        spinReflectIntercept->setMinimum(-1000.000000000000000);
        spinReflectIntercept->setMaximum(1000.000000000000000);
        spinReflectIntercept->setValue(0.000000000000000);
        spinReflectIntercept->setSingleStep(1.000000000000000);

        arbLineForm->setWidget(1, QFormLayout::ItemRole::FieldRole, spinReflectIntercept);


        arbLineVLayout->addLayout(arbLineForm);

        arbLineToolsLayout = new QHBoxLayout();
        arbLineToolsLayout->setObjectName("arbLineToolsLayout");
        btnPickReflectLine = new QPushButton(groupArbitraryLine);
        btnPickReflectLine->setObjectName("btnPickReflectLine");

        arbLineToolsLayout->addWidget(btnPickReflectLine);

        btnApplyReflectArbitrary = new QPushButton(groupArbitraryLine);
        btnApplyReflectArbitrary->setObjectName("btnApplyReflectArbitrary");

        arbLineToolsLayout->addWidget(btnApplyReflectArbitrary);


        arbLineVLayout->addLayout(arbLineToolsLayout);


        reflectLayout->addWidget(groupArbitraryLine);

        tabTransforms->addTab(tabReflect, QString());
        tabCustomMatrix = new QWidget();
        tabCustomMatrix->setObjectName("tabCustomMatrix");
        customMatLayout = new QVBoxLayout(tabCustomMatrix);
        customMatLayout->setSpacing(6);
        customMatLayout->setObjectName("customMatLayout");
        lblCustomMatInfo = new QLabel(tabCustomMatrix);
        lblCustomMatInfo->setObjectName("lblCustomMatInfo");

        customMatLayout->addWidget(lblCustomMatInfo);

        customMatrixGrid = new QGridLayout();
        customMatrixGrid->setObjectName("customMatrixGrid");
        spinM00 = new QDoubleSpinBox(tabCustomMatrix);
        spinM00->setObjectName("spinM00");
        spinM00->setDecimals(3);
        spinM00->setMinimum(-1000.000000000000000);
        spinM00->setMaximum(1000.000000000000000);
        spinM00->setValue(1.000000000000000);

        customMatrixGrid->addWidget(spinM00, 0, 0, 1, 1);

        spinM01 = new QDoubleSpinBox(tabCustomMatrix);
        spinM01->setObjectName("spinM01");
        spinM01->setDecimals(3);
        spinM01->setMinimum(-1000.000000000000000);
        spinM01->setMaximum(1000.000000000000000);
        spinM01->setValue(0.000000000000000);

        customMatrixGrid->addWidget(spinM01, 0, 1, 1, 1);

        spinM02 = new QDoubleSpinBox(tabCustomMatrix);
        spinM02->setObjectName("spinM02");
        spinM02->setDecimals(3);
        spinM02->setMinimum(-1000.000000000000000);
        spinM02->setMaximum(1000.000000000000000);
        spinM02->setValue(0.000000000000000);

        customMatrixGrid->addWidget(spinM02, 0, 2, 1, 1);

        spinM10 = new QDoubleSpinBox(tabCustomMatrix);
        spinM10->setObjectName("spinM10");
        spinM10->setDecimals(3);
        spinM10->setMinimum(-1000.000000000000000);
        spinM10->setMaximum(1000.000000000000000);
        spinM10->setValue(0.000000000000000);

        customMatrixGrid->addWidget(spinM10, 1, 0, 1, 1);

        spinM11 = new QDoubleSpinBox(tabCustomMatrix);
        spinM11->setObjectName("spinM11");
        spinM11->setDecimals(3);
        spinM11->setMinimum(-1000.000000000000000);
        spinM11->setMaximum(1000.000000000000000);
        spinM11->setValue(1.000000000000000);

        customMatrixGrid->addWidget(spinM11, 1, 1, 1, 1);

        spinM12 = new QDoubleSpinBox(tabCustomMatrix);
        spinM12->setObjectName("spinM12");
        spinM12->setDecimals(3);
        spinM12->setMinimum(-1000.000000000000000);
        spinM12->setMaximum(1000.000000000000000);
        spinM12->setValue(0.000000000000000);

        customMatrixGrid->addWidget(spinM12, 1, 2, 1, 1);

        spinM20 = new QDoubleSpinBox(tabCustomMatrix);
        spinM20->setObjectName("spinM20");
        spinM20->setDecimals(3);
        spinM20->setMinimum(-1000.000000000000000);
        spinM20->setMaximum(1000.000000000000000);
        spinM20->setValue(0.000000000000000);

        customMatrixGrid->addWidget(spinM20, 2, 0, 1, 1);

        spinM21 = new QDoubleSpinBox(tabCustomMatrix);
        spinM21->setObjectName("spinM21");
        spinM21->setDecimals(3);
        spinM21->setMinimum(-1000.000000000000000);
        spinM21->setMaximum(1000.000000000000000);
        spinM21->setValue(0.000000000000000);

        customMatrixGrid->addWidget(spinM21, 2, 1, 1, 1);

        spinM22 = new QDoubleSpinBox(tabCustomMatrix);
        spinM22->setObjectName("spinM22");
        spinM22->setDecimals(3);
        spinM22->setMinimum(-1000.000000000000000);
        spinM22->setMaximum(1000.000000000000000);
        spinM22->setValue(1.000000000000000);

        customMatrixGrid->addWidget(spinM22, 2, 2, 1, 1);


        customMatLayout->addLayout(customMatrixGrid);

        btnApplyCustomMatrix = new QPushButton(tabCustomMatrix);
        btnApplyCustomMatrix->setObjectName("btnApplyCustomMatrix");

        customMatLayout->addWidget(btnApplyCustomMatrix);

        tabTransforms->addTab(tabCustomMatrix, QString());

        transformsVLayout->addWidget(tabTransforms);


        leftControlVLayout->addWidget(groupTransforms);

        groupFills = new QGroupBox(leftControlWidget);
        groupFills->setObjectName("groupFills");
        fillsLayout = new QVBoxLayout(groupFills);
        fillsLayout->setSpacing(6);
        fillsLayout->setObjectName("fillsLayout");
        comboFillAlgorithm = new QComboBox(groupFills);
        comboFillAlgorithm->addItem(QString());
        comboFillAlgorithm->addItem(QString());
        comboFillAlgorithm->addItem(QString());
        comboFillAlgorithm->setObjectName("comboFillAlgorithm");

        fillsLayout->addWidget(comboFillAlgorithm);

        fillActionLayout = new QHBoxLayout();
        fillActionLayout->setObjectName("fillActionLayout");
        btnExecuteFill = new QPushButton(groupFills);
        btnExecuteFill->setObjectName("btnExecuteFill");

        fillActionLayout->addWidget(btnExecuteFill);

        btnClearFill = new QPushButton(groupFills);
        btnClearFill->setObjectName("btnClearFill");

        fillActionLayout->addWidget(btnClearFill);


        fillsLayout->addLayout(fillActionLayout);


        leftControlVLayout->addWidget(groupFills);

        groupShapes = new QGroupBox(leftControlWidget);
        groupShapes->setObjectName("groupShapes");
        shapesVLayout = new QVBoxLayout(groupShapes);
        shapesVLayout->setSpacing(6);
        shapesVLayout->setObjectName("shapesVLayout");
        shapeButtonsGrid = new QGridLayout();
        shapeButtonsGrid->setObjectName("shapeButtonsGrid");
        btnDrawPolygon = new QPushButton(groupShapes);
        btnDrawPolygon->setObjectName("btnDrawPolygon");

        shapeButtonsGrid->addWidget(btnDrawPolygon, 0, 0, 1, 1);

        btnDrawCircle = new QPushButton(groupShapes);
        btnDrawCircle->setObjectName("btnDrawCircle");

        shapeButtonsGrid->addWidget(btnDrawCircle, 0, 1, 1, 1);

        btnClosePolygon = new QPushButton(groupShapes);
        btnClosePolygon->setObjectName("btnClosePolygon");

        shapeButtonsGrid->addWidget(btnClosePolygon, 1, 0, 1, 1);

        btnUndoShape = new QPushButton(groupShapes);
        btnUndoShape->setObjectName("btnUndoShape");

        shapeButtonsGrid->addWidget(btnUndoShape, 1, 1, 1, 1);


        shapesVLayout->addLayout(shapeButtonsGrid);

        lblPresetsHeader = new QLabel(groupShapes);
        lblPresetsHeader->setObjectName("lblPresetsHeader");

        shapesVLayout->addWidget(lblPresetsHeader);

        presetsGrid = new QGridLayout();
        presetsGrid->setObjectName("presetsGrid");
        btnPresetTriangle = new QPushButton(groupShapes);
        btnPresetTriangle->setObjectName("btnPresetTriangle");

        presetsGrid->addWidget(btnPresetTriangle, 0, 0, 1, 1);

        btnPresetConvex = new QPushButton(groupShapes);
        btnPresetConvex->setObjectName("btnPresetConvex");

        presetsGrid->addWidget(btnPresetConvex, 0, 1, 1, 1);

        btnPresetHouse = new QPushButton(groupShapes);
        btnPresetHouse->setObjectName("btnPresetHouse");

        presetsGrid->addWidget(btnPresetHouse, 1, 0, 1, 1);

        btnPresetStar = new QPushButton(groupShapes);
        btnPresetStar->setObjectName("btnPresetStar");

        presetsGrid->addWidget(btnPresetStar, 1, 1, 1, 1);

        btnPresetConcave = new QPushButton(groupShapes);
        btnPresetConcave->setObjectName("btnPresetConcave");

        presetsGrid->addWidget(btnPresetConcave, 2, 0, 1, 1);

        btnPresetNested = new QPushButton(groupShapes);
        btnPresetNested->setObjectName("btnPresetNested");

        presetsGrid->addWidget(btnPresetNested, 2, 1, 1, 1);

        btnPresetMultiScene = new QPushButton(groupShapes);
        btnPresetMultiScene->setObjectName("btnPresetMultiScene");

        presetsGrid->addWidget(btnPresetMultiScene, 3, 0, 1, 2);


        shapesVLayout->addLayout(presetsGrid);


        leftControlVLayout->addWidget(groupShapes);

        leftControlScroll->setWidget(leftControlWidget);

        mainHLayout->addWidget(leftControlScroll);

        canvasAndMatrixVLayout = new QVBoxLayout();
        canvasAndMatrixVLayout->setSpacing(6);
        canvasAndMatrixVLayout->setObjectName("canvasAndMatrixVLayout");
        canvasWidget = new Canvas(centralwidget);
        canvasWidget->setObjectName("canvasWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(canvasWidget->sizePolicy().hasHeightForWidth());
        canvasWidget->setSizePolicy(sizePolicy);

        canvasAndMatrixVLayout->addWidget(canvasWidget);

        bottomDashboardLayout = new QHBoxLayout();
        bottomDashboardLayout->setSpacing(8);
        bottomDashboardLayout->setObjectName("bottomDashboardLayout");
        groupMatrixViewer = new QGroupBox(centralwidget);
        groupMatrixViewer->setObjectName("groupMatrixViewer");
        matrixBoxVLayout = new QVBoxLayout(groupMatrixViewer);
        matrixBoxVLayout->setSpacing(4);
        matrixBoxVLayout->setObjectName("matrixBoxVLayout");
        lblMatrixFormula = new QLabel(groupMatrixViewer);
        lblMatrixFormula->setObjectName("lblMatrixFormula");

        matrixBoxVLayout->addWidget(lblMatrixFormula);

        txtMatrixDisplay = new QTextEdit(groupMatrixViewer);
        txtMatrixDisplay->setObjectName("txtMatrixDisplay");
        txtMatrixDisplay->setReadOnly(true);
        txtMatrixDisplay->setMaximumHeight(110);

        matrixBoxVLayout->addWidget(txtMatrixDisplay);

        matrixActionsLayout = new QHBoxLayout();
        matrixActionsLayout->setObjectName("matrixActionsLayout");
        lblMatrixDet = new QLabel(groupMatrixViewer);
        lblMatrixDet->setObjectName("lblMatrixDet");

        matrixActionsLayout->addWidget(lblMatrixDet);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        matrixActionsLayout->addItem(spacerItem);

        btnUndoTransform = new QPushButton(groupMatrixViewer);
        btnUndoTransform->setObjectName("btnUndoTransform");

        matrixActionsLayout->addWidget(btnUndoTransform);

        btnResetTransform = new QPushButton(groupMatrixViewer);
        btnResetTransform->setObjectName("btnResetTransform");

        matrixActionsLayout->addWidget(btnResetTransform);


        matrixBoxVLayout->addLayout(matrixActionsLayout);


        bottomDashboardLayout->addWidget(groupMatrixViewer);

        groupHistory = new QGroupBox(centralwidget);
        groupHistory->setObjectName("groupHistory");
        historyVLayout = new QVBoxLayout(groupHistory);
        historyVLayout->setSpacing(4);
        historyVLayout->setObjectName("historyVLayout");
        tableHistory = new QTableWidget(groupHistory);
        if (tableHistory->columnCount() < 3)
            tableHistory->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableHistory->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableHistory->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableHistory->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableHistory->setObjectName("tableHistory");
        tableHistory->setMaximumHeight(110);
        tableHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableHistory->setAlternatingRowColors(true);

        historyVLayout->addWidget(tableHistory);


        bottomDashboardLayout->addWidget(groupHistory);

        groupView = new QGroupBox(centralwidget);
        groupView->setObjectName("groupView");
        viewGridLayout = new QGridLayout(groupView);
        viewGridLayout->setSpacing(4);
        viewGridLayout->setObjectName("viewGridLayout");
        chkShowGhost = new QCheckBox(groupView);
        chkShowGhost->setObjectName("chkShowGhost");
        chkShowGhost->setChecked(true);

        viewGridLayout->addWidget(chkShowGhost, 0, 0, 1, 1);

        chkShowPivot = new QCheckBox(groupView);
        chkShowPivot->setObjectName("chkShowPivot");
        chkShowPivot->setChecked(true);

        viewGridLayout->addWidget(chkShowPivot, 0, 1, 1, 1);

        chkShowReflectLine = new QCheckBox(groupView);
        chkShowReflectLine->setObjectName("chkShowReflectLine");
        chkShowReflectLine->setChecked(true);

        viewGridLayout->addWidget(chkShowReflectLine, 1, 0, 1, 1);

        chkShowLabels = new QCheckBox(groupView);
        chkShowLabels->setObjectName("chkShowLabels");
        chkShowLabels->setChecked(true);

        viewGridLayout->addWidget(chkShowLabels, 1, 1, 1, 1);

        chkShowGrid = new QCheckBox(groupView);
        chkShowGrid->setObjectName("chkShowGrid");
        chkShowGrid->setChecked(true);

        viewGridLayout->addWidget(chkShowGrid, 2, 0, 1, 1);

        chkShowAxes = new QCheckBox(groupView);
        chkShowAxes->setObjectName("chkShowAxes");
        chkShowAxes->setChecked(true);

        viewGridLayout->addWidget(chkShowAxes, 2, 1, 1, 1);

        lblGridSize = new QLabel(groupView);
        lblGridSize->setObjectName("lblGridSize");

        viewGridLayout->addWidget(lblGridSize, 3, 0, 1, 1);

        spinGridSize = new QSpinBox(groupView);
        spinGridSize->setObjectName("spinGridSize");
        spinGridSize->setMinimum(6);
        spinGridSize->setMaximum(50);
        spinGridSize->setValue(14);

        viewGridLayout->addWidget(spinGridSize, 3, 1, 1, 1);

        btnClearAll = new QPushButton(groupView);
        btnClearAll->setObjectName("btnClearAll");

        viewGridLayout->addWidget(btnClearAll, 4, 0, 1, 2);


        bottomDashboardLayout->addWidget(groupView);


        canvasAndMatrixVLayout->addLayout(bottomDashboardLayout);


        mainHLayout->addLayout(canvasAndMatrixVLayout);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabTransforms->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "2D Vector Transformations Studio (Homogeneous Coordinates 3x3)", nullptr));
        groupSelection->setTitle(QCoreApplication::translate("MainWindow", "1. Shape Selection & Scope", nullptr));
        lblScopeInfo->setText(QCoreApplication::translate("MainWindow", "\360\237\214\220 Current Scope: Entire Space / All Shapes", nullptr));
        lblScopeInfo->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #e3b341; font-weight: bold; font-size: 12px;", nullptr));
        comboSelectShape->setItemText(0, QCoreApplication::translate("MainWindow", "\360\237\214\220 Entire Space (All Shapes & Grid)", nullptr));

        btnDeselectAll->setText(QCoreApplication::translate("MainWindow", "Select All", nullptr));
        lblSelectHint->setText(QCoreApplication::translate("MainWindow", "\360\237\222\241 Right-click any shape on canvas to select it. Right-click empty grid to deselect.", nullptr));
        lblSelectHint->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #8b949e; font-size: 11px;", nullptr));
        groupQuickCommand->setTitle(QCoreApplication::translate("MainWindow", "\342\232\241 Quick Transform Command / Argument Input", nullptr));
        txtQuickCommand->setPlaceholderText(QCoreApplication::translate("MainWindow", "e.g. R 45, T 10 -5, S 2 1.5, Sh 0.5 0, RefLine 1 2", nullptr));
        txtQuickCommand->setStyleSheet(QCoreApplication::translate("MainWindow", "font-family: monospace; font-size: 12px; color: #58a6ff;", nullptr));
        btnRunQuickCommand->setText(QCoreApplication::translate("MainWindow", "Run \342\206\265", nullptr));
        btnRunQuickCommand->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #238636; color: #ffffff; font-weight: bold;", nullptr));
        groupTransforms->setTitle(QCoreApplication::translate("MainWindow", "2. 2D Vector Transformations (3x3 Matrix)", nullptr));
        lblTx->setText(QCoreApplication::translate("MainWindow", "Translate X (tx):", nullptr));
        lblTy->setText(QCoreApplication::translate("MainWindow", "Translate Y (ty):", nullptr));
        btnApplyTranslate->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Apply Translation T(tx, ty) [Enter]", nullptr));
        btnApplyTranslate->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #1f6feb; color: #ffffff; font-weight: bold; padding: 6px;", nullptr));
        btnNudgeUp->setText(QCoreApplication::translate("MainWindow", "\342\226\262 +Y (Up)", nullptr));
        btnNudgeLeft->setText(QCoreApplication::translate("MainWindow", "\342\227\200 -X (Left)", nullptr));
        btnNudgeReset->setText(QCoreApplication::translate("MainWindow", "(0,0)", nullptr));
        btnNudgeRight->setText(QCoreApplication::translate("MainWindow", "+X (Right) \342\226\266", nullptr));
        btnNudgeDown->setText(QCoreApplication::translate("MainWindow", "\342\226\274 -Y (Down)", nullptr));
        tabTransforms->setTabText(tabTransforms->indexOf(tabTranslate), QCoreApplication::translate("MainWindow", "\342\206\224 Translate", nullptr));
        lblRotAngle->setText(QCoreApplication::translate("MainWindow", "Angle \316\270 (degrees):", nullptr));
        groupRotPivot->setTitle(QCoreApplication::translate("MainWindow", "Rotation Center (Pivot Point):", nullptr));
        radioRotOrigin->setText(QCoreApplication::translate("MainWindow", "Origin (0, 0)", nullptr));
        radioRotCentroid->setText(QCoreApplication::translate("MainWindow", "Shape Centroid", nullptr));
        radioRotCustom->setText(QCoreApplication::translate("MainWindow", "Arbitrary Point (xr, yr)", nullptr));
        lblRotPx->setText(QCoreApplication::translate("MainWindow", "xr:", nullptr));
        lblRotPy->setText(QCoreApplication::translate("MainWindow", "yr:", nullptr));
        btnPickPivot->setText(QCoreApplication::translate("MainWindow", "\360\237\216\257 Pick", nullptr));
        btnApplyRotate->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Apply Rotation R(\316\270) [Enter]", nullptr));
        btnApplyRotate->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #1f6feb; color: #ffffff; font-weight: bold; padding: 6px;", nullptr));
        btnQuickRot45->setText(QCoreApplication::translate("MainWindow", "+45\302\260", nullptr));
        btnQuickRotNeg45->setText(QCoreApplication::translate("MainWindow", "-45\302\260", nullptr));
        btnQuickRot90->setText(QCoreApplication::translate("MainWindow", "+90\302\260", nullptr));
        btnQuickRot180->setText(QCoreApplication::translate("MainWindow", "180\302\260", nullptr));
        tabTransforms->setTabText(tabTransforms->indexOf(tabRotate), QCoreApplication::translate("MainWindow", "\360\237\224\204 Rotate", nullptr));
        lblSx->setText(QCoreApplication::translate("MainWindow", "Scale X (sx):", nullptr));
        lblSy->setText(QCoreApplication::translate("MainWindow", "Scale Y (sy):", nullptr));
        chkUniformScale->setText(QCoreApplication::translate("MainWindow", "Lock Uniform Scaling (sx = sy)", nullptr));
        groupScaleFixed->setTitle(QCoreApplication::translate("MainWindow", "Fixed Reference Point:", nullptr));
        radioScaleOrigin->setText(QCoreApplication::translate("MainWindow", "Origin (0, 0)", nullptr));
        radioScaleCentroid->setText(QCoreApplication::translate("MainWindow", "Shape Centroid (Local Scale)", nullptr));
        radioScaleCustom->setText(QCoreApplication::translate("MainWindow", "Arbitrary Point (xf, yf)", nullptr));
        lblScalePx->setText(QCoreApplication::translate("MainWindow", "xf:", nullptr));
        lblScalePy->setText(QCoreApplication::translate("MainWindow", "yf:", nullptr));
        btnApplyScale->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Apply Scaling S(sx, sy) [Enter]", nullptr));
        btnApplyScale->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #1f6feb; color: #ffffff; font-weight: bold; padding: 6px;", nullptr));
        btnQuickScale2->setText(QCoreApplication::translate("MainWindow", "2.0x", nullptr));
        btnQuickScaleHalf->setText(QCoreApplication::translate("MainWindow", "0.5x", nullptr));
        btnQuickScale15->setText(QCoreApplication::translate("MainWindow", "1.5x", nullptr));
        btnQuickScaleFlip->setText(QCoreApplication::translate("MainWindow", "-1.0x", nullptr));
        tabTransforms->setTabText(tabTransforms->indexOf(tabScale), QCoreApplication::translate("MainWindow", "\360\237\224\215 Scale", nullptr));
        lblShx->setText(QCoreApplication::translate("MainWindow", "Shear X (shx):", nullptr));
        lblShy->setText(QCoreApplication::translate("MainWindow", "Shear Y (shy):", nullptr));
        lblYref->setText(QCoreApplication::translate("MainWindow", "Ref Line yref:", nullptr));
        lblXref->setText(QCoreApplication::translate("MainWindow", "Ref Line xref:", nullptr));
        btnApplyShear->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Apply Shearing Sh(shx, shy) [Enter]", nullptr));
        btnApplyShear->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #1f6feb; color: #ffffff; font-weight: bold; padding: 6px;", nullptr));
        tabTransforms->setTabText(tabTransforms->indexOf(tabShear), QCoreApplication::translate("MainWindow", "\360\237\223\220 Shear", nullptr));
        lblStandardRefTitle->setText(QCoreApplication::translate("MainWindow", "Standard Axis Reflections:", nullptr));
        btnReflectX->setText(QCoreApplication::translate("MainWindow", "X-Axis (y=0)", nullptr));
        btnReflectY->setText(QCoreApplication::translate("MainWindow", "Y-Axis (x=0)", nullptr));
        btnReflectOrigin->setText(QCoreApplication::translate("MainWindow", "Origin (0,0)", nullptr));
        btnReflectLineYeqX->setText(QCoreApplication::translate("MainWindow", "Line y = x", nullptr));
        btnReflectLineYeqNegX->setText(QCoreApplication::translate("MainWindow", "Line y = -x", nullptr));
        groupArbitraryLine->setTitle(QCoreApplication::translate("MainWindow", "Reflection wrt Arbitrary Line (y = mx + c):", nullptr));
        lblSlope->setText(QCoreApplication::translate("MainWindow", "Slope (m):", nullptr));
        lblIntercept->setText(QCoreApplication::translate("MainWindow", "Intercept (c):", nullptr));
        btnPickReflectLine->setText(QCoreApplication::translate("MainWindow", "\360\237\223\220 2 Points", nullptr));
        btnApplyReflectArbitrary->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Reflect Across Line", nullptr));
        btnApplyReflectArbitrary->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #238636; color: #ffffff; font-weight: bold;", nullptr));
        tabTransforms->setTabText(tabTransforms->indexOf(tabReflect), QCoreApplication::translate("MainWindow", "\360\237\252\236 Reflect", nullptr));
        lblCustomMatInfo->setText(QCoreApplication::translate("MainWindow", "Directly enter 3x3 Matrix Coefficients:", nullptr));
        lblCustomMatInfo->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #58a6ff; font-weight: bold;", nullptr));
        btnApplyCustomMatrix->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Apply Custom 3x3 Matrix", nullptr));
        btnApplyCustomMatrix->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #238636; color: #ffffff; font-weight: bold; padding: 6px;", nullptr));
        tabTransforms->setTabText(tabTransforms->indexOf(tabCustomMatrix), QCoreApplication::translate("MainWindow", "\360\237\224\262 Custom 3x3", nullptr));
        groupFills->setTitle(QCoreApplication::translate("MainWindow", "3. Integrated Fill Algorithms", nullptr));
        comboFillAlgorithm->setItemText(0, QCoreApplication::translate("MainWindow", "\342\234\250 Scanline Polygon Fill (AET)", nullptr));
        comboFillAlgorithm->setItemText(1, QCoreApplication::translate("MainWindow", "\360\237\216\250 8-Way Boundary Fill (BFS Wavefront)", nullptr));
        comboFillAlgorithm->setItemText(2, QCoreApplication::translate("MainWindow", "\360\237\216\250 8-Way Boundary Fill (DFS Stack)", nullptr));

        btnExecuteFill->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Execute Fill", nullptr));
        btnExecuteFill->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #1f6feb; color: #ffffff; font-weight: bold; padding: 6px;", nullptr));
        btnClearFill->setText(QCoreApplication::translate("MainWindow", "Clear Fill", nullptr));
        groupShapes->setTitle(QCoreApplication::translate("MainWindow", "4. Shape Tools & Presets", nullptr));
        btnDrawPolygon->setText(QCoreApplication::translate("MainWindow", "\342\234\217 Draw Poly", nullptr));
        btnDrawCircle->setText(QCoreApplication::translate("MainWindow", "\342\232\252 Draw Circle", nullptr));
        btnClosePolygon->setText(QCoreApplication::translate("MainWindow", "\342\234\224 Close Poly", nullptr));
        btnClosePolygon->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #238636; color: #ffffff;", nullptr));
        btnUndoShape->setText(QCoreApplication::translate("MainWindow", "\342\206\266 Undo Shape", nullptr));
        lblPresetsHeader->setText(QCoreApplication::translate("MainWindow", "Load Geometric Presets:", nullptr));
        btnPresetTriangle->setText(QCoreApplication::translate("MainWindow", "Triangle (3-gon)", nullptr));
        btnPresetConvex->setText(QCoreApplication::translate("MainWindow", "Convex (5-gon)", nullptr));
        btnPresetHouse->setText(QCoreApplication::translate("MainWindow", "House / Pentagon", nullptr));
        btnPresetStar->setText(QCoreApplication::translate("MainWindow", "5-Point Star", nullptr));
        btnPresetConcave->setText(QCoreApplication::translate("MainWindow", "Concave Arrow", nullptr));
        btnPresetNested->setText(QCoreApplication::translate("MainWindow", "\360\237\215\251 Nested Donut", nullptr));
        btnPresetMultiScene->setText(QCoreApplication::translate("MainWindow", "\360\237\216\255 Multi-Scene (2 Polys + Circle)", nullptr));
        btnPresetMultiScene->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #21262d; border: 1px solid #2ea043; color: #2ea043; font-weight: bold;", nullptr));
        groupMatrixViewer->setTitle(QCoreApplication::translate("MainWindow", "\360\237\223\220 Active 3x3 Homogeneous Transformation Matrix [M]", nullptr));
        lblMatrixFormula->setText(QCoreApplication::translate("MainWindow", "M_composite = I (Identity)", nullptr));
        lblMatrixFormula->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #e3b341; font-weight: bold; font-family: monospace; font-size: 12px;", nullptr));
        txtMatrixDisplay->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #0d1117; border: 1px solid #30363d; font-family: monospace; font-size: 12px;", nullptr));
        lblMatrixDet->setText(QCoreApplication::translate("MainWindow", "Det(M): 1.000", nullptr));
        lblMatrixDet->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #8b949e; font-size: 11px;", nullptr));
        btnUndoTransform->setText(QCoreApplication::translate("MainWindow", "\342\206\266 Undo Step", nullptr));
        btnResetTransform->setText(QCoreApplication::translate("MainWindow", "\360\237\224\204 Reset Matrix (I)", nullptr));
        btnResetTransform->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #ff7b72;", nullptr));
        groupHistory->setTitle(QCoreApplication::translate("MainWindow", "\360\237\223\213 Transformation Pipeline History", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableHistory->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Step #", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableHistory->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Transformation", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableHistory->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Parameters / Formula", nullptr));
        groupView->setTitle(QCoreApplication::translate("MainWindow", "\360\237\221\201 Display & Overlays", nullptr));
        chkShowGhost->setText(QCoreApplication::translate("MainWindow", "Show Ghost (Pre-Transform)", nullptr));
        chkShowPivot->setText(QCoreApplication::translate("MainWindow", "Show Pivot Point", nullptr));
        chkShowReflectLine->setText(QCoreApplication::translate("MainWindow", "Show Reflection Line", nullptr));
        chkShowLabels->setText(QCoreApplication::translate("MainWindow", "Show Vertex Labels", nullptr));
        chkShowGrid->setText(QCoreApplication::translate("MainWindow", "Show Grid", nullptr));
        chkShowAxes->setText(QCoreApplication::translate("MainWindow", "Show Axes", nullptr));
        lblGridSize->setText(QCoreApplication::translate("MainWindow", "Grid Cell Size:", nullptr));
        btnClearAll->setText(QCoreApplication::translate("MainWindow", "\342\234\226 Clear All (Shapes & Fills)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
