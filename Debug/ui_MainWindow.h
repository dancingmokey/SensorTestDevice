/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "CurveWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    CurveWidget *Curve2Wgt;
    CurveWidget *Curve1Wgt;
    QWidget *ControlWgt;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *SplitCtrlBtn;
    QPushButton *ZoomInCtrlBtn;
    QPushButton *ZoomOutCtrlBtn;
    QPushButton *PauseCtrlBtn;
    QPushButton *CatchCtrlBtn;
    QPushButton *ParamCtrlBtn;
    QWidget *TestInfoWgt;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 800);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1280, 800));
        MainWindow->setMaximumSize(QSize(1280, 800));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMinimumSize(QSize(1280, 800));
        centralWidget->setMaximumSize(QSize(1280, 800));
        Curve2Wgt = new CurveWidget(centralWidget);
        Curve2Wgt->setObjectName(QString::fromUtf8("Curve2Wgt"));
        Curve2Wgt->setGeometry(QRect(0, 350, 1280, 350));
        sizePolicy1.setHeightForWidth(Curve2Wgt->sizePolicy().hasHeightForWidth());
        Curve2Wgt->setSizePolicy(sizePolicy1);
        Curve2Wgt->setMinimumSize(QSize(1280, 0));
        Curve2Wgt->setMaximumSize(QSize(1280, 350));
        QPalette palette;
        QBrush brush(QColor(12, 255, 60, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        Curve2Wgt->setPalette(palette);
        Curve2Wgt->setStyleSheet(QString::fromUtf8("background-color: rgb(12, 255, 60);\n"
"border-color: rgb(255, 255, 255);"));
        Curve1Wgt = new CurveWidget(centralWidget);
        Curve1Wgt->setObjectName(QString::fromUtf8("Curve1Wgt"));
        Curve1Wgt->setGeometry(QRect(0, 0, 1280, 350));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Curve1Wgt->sizePolicy().hasHeightForWidth());
        Curve1Wgt->setSizePolicy(sizePolicy2);
        Curve1Wgt->setMinimumSize(QSize(1280, 350));
        Curve1Wgt->setMaximumSize(QSize(1280, 700));
        Curve1Wgt->setStyleSheet(QString::fromUtf8("background-color: rgb(11, 11, 255);\n"
"border-color: rgb(255, 255, 255);"));
        ControlWgt = new QWidget(centralWidget);
        ControlWgt->setObjectName(QString::fromUtf8("ControlWgt"));
        ControlWgt->setEnabled(true);
        ControlWgt->setGeometry(QRect(0, 700, 1280, 100));
        sizePolicy.setHeightForWidth(ControlWgt->sizePolicy().hasHeightForWidth());
        ControlWgt->setSizePolicy(sizePolicy);
        ControlWgt->setMinimumSize(QSize(1280, 100));
        ControlWgt->setMaximumSize(QSize(1280, 100));
        ControlWgt->setStyleSheet(QString::fromUtf8("background-color: rgb(248, 255, 161);"));
        horizontalLayout_2 = new QHBoxLayout(ControlWgt);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        SplitCtrlBtn = new QPushButton(ControlWgt);
        SplitCtrlBtn->setObjectName(QString::fromUtf8("SplitCtrlBtn"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(SplitCtrlBtn->sizePolicy().hasHeightForWidth());
        SplitCtrlBtn->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(SplitCtrlBtn);

        ZoomInCtrlBtn = new QPushButton(ControlWgt);
        ZoomInCtrlBtn->setObjectName(QString::fromUtf8("ZoomInCtrlBtn"));
        sizePolicy3.setHeightForWidth(ZoomInCtrlBtn->sizePolicy().hasHeightForWidth());
        ZoomInCtrlBtn->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(ZoomInCtrlBtn);

        ZoomOutCtrlBtn = new QPushButton(ControlWgt);
        ZoomOutCtrlBtn->setObjectName(QString::fromUtf8("ZoomOutCtrlBtn"));
        sizePolicy3.setHeightForWidth(ZoomOutCtrlBtn->sizePolicy().hasHeightForWidth());
        ZoomOutCtrlBtn->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(ZoomOutCtrlBtn);

        PauseCtrlBtn = new QPushButton(ControlWgt);
        PauseCtrlBtn->setObjectName(QString::fromUtf8("PauseCtrlBtn"));
        sizePolicy3.setHeightForWidth(PauseCtrlBtn->sizePolicy().hasHeightForWidth());
        PauseCtrlBtn->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(PauseCtrlBtn);

        CatchCtrlBtn = new QPushButton(ControlWgt);
        CatchCtrlBtn->setObjectName(QString::fromUtf8("CatchCtrlBtn"));
        sizePolicy3.setHeightForWidth(CatchCtrlBtn->sizePolicy().hasHeightForWidth());
        CatchCtrlBtn->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(CatchCtrlBtn);

        ParamCtrlBtn = new QPushButton(ControlWgt);
        ParamCtrlBtn->setObjectName(QString::fromUtf8("ParamCtrlBtn"));
        sizePolicy3.setHeightForWidth(ParamCtrlBtn->sizePolicy().hasHeightForWidth());
        ParamCtrlBtn->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(ParamCtrlBtn);

        TestInfoWgt = new QWidget(ControlWgt);
        TestInfoWgt->setObjectName(QString::fromUtf8("TestInfoWgt"));
        TestInfoWgt->setStyleSheet(QString::fromUtf8("background-color: rgb(151, 224, 255);"));

        horizontalLayout->addWidget(TestInfoWgt);


        horizontalLayout_2->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        SplitCtrlBtn->setText(QApplication::translate("MainWindow", "Split/Merge", 0, QApplication::UnicodeUTF8));
        ZoomInCtrlBtn->setText(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        ZoomOutCtrlBtn->setText(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        PauseCtrlBtn->setText(QApplication::translate("MainWindow", "Pause/Continue", 0, QApplication::UnicodeUTF8));
        CatchCtrlBtn->setText(QApplication::translate("MainWindow", "Catch", 0, QApplication::UnicodeUTF8));
        ParamCtrlBtn->setText(QApplication::translate("MainWindow", "Param", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
