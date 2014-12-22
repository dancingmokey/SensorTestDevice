#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <QString>
#include <QList>
#include <QMutex>
#include <QColor>

typedef unsigned char uint8;

extern QMutex g_mutexCurve1Locker;
extern QList<double> g_ltCurve1Data;
extern QByteArray g_acCurve1;
extern QMutex g_mutexCurve2Locker;
extern QList<double> g_ltCurve2Data;
extern QByteArray g_acCurve2;

class Global
{
public:
    Global();
public:
    /**
     * @brief Safe_Delete
     * @param pPointer : void* : Pointer need to be Deleted
     */
    static void Safe_Delete(void* pPointer);

public:
    static const int Tick_Value = 10;
    static const int Axis_Margin = 30;
    static const int Label_Width = 40;
    static const int Label_Height = 40;
    static const uint8 Axis_Horizontal = 0x01;
    static const uint8 Axis_Vertical = 0x02;


    static const uint8 Serial_Add = 0x01;
    static const uint8 Serial_del = 0x02;


    static const QString CH1_Serial_Name;
    static const QString CH2_Serial_Name;


    static const QColor CH1_Serial_Color;
    static const QColor CH2_Serial_Color;

    static const QString CH1_SerialPort_Name;
    static const QString CH2_SerialPort_Name;
};

#endif // GLOBAL_H
