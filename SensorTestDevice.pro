#-------------------------------------------------
#
# Project created by QtCreator 2014-12-12T09:16:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SensorTestDevice
TEMPLATE = app

DEFINES += _TTY_POSIX_
#DEFINES += _DEBUG_SIMULATE
#DEFINES += _DEBUG_OUTPUT


SOURCES += main.cpp\
        MainWindow.cpp \
    Axis.cpp \
    Serial.cpp \
    Global.cpp \
    CurveWidget.cpp \
    Curve.cpp \
    InfoWidget.cpp \
    DataSource.cpp \
    SerialPort/posix_qextserialport.cpp \
    SerialPort/qextserialbase.cpp \
    DataProcess.cpp \
    BatteryMonitor.cpp

HEADERS  += MainWindow.h \
    Serial.h \
    Global.h \
    CurveWidget.h \
    Curve.h \
    Axis.h \
    InfoWidget.h \
    DataSource.h \
    SerialPort/posix_qextserialport.h \
    SerialPort/qextserialbase.h \
    DataProcess.h \
    BatteryMonitor.h

FORMS    += MainWindow.ui

SUBDIRS += \
    SerialPort/qextserialport.pro

RESOURCES += \
    Images.qrc
