#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <QString>
#include <QList>
#include <QMutex>
#include <QColor>
#include <QTime>
typedef unsigned char uint8;

extern QMutex g_mutexCurve1Locker;
extern QList<double> g_ltCurve1Data;
extern QByteArray g_acCurve1;
extern QMutex g_mutexCurve2Locker;
extern QList<double> g_ltCurve2Data;
extern QByteArray g_acCurve2;
extern QList<QString> g_ltXLabelTexts;
extern QTime g_dtLastUpDate;



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
    static const int Axis_Margin = 50;
    static const int Label_Width = 100;
    static const int Label_Height = 40;

    static const uint8 Axis_Horizontal = 0x01;
    static const uint8 Axis_Vertical = 0x02;
    static const int Axis_Hor_MaxVal = 2000;
    static const int Axis_Hor_MinVal = 0;
    static const int Axis_Hor_TickVal = 200;
    static const int Axis_Ver_MaxVal = 13;
    static const int Axis_Ver_MinVal = -1;
    static const int Axis_Ver_TickVal = 2;

    static const uint8 Serial_Add = 0x01;
    static const uint8 Serial_del = 0x02;

    static const uint8 CH1_Data= 0x01;
    static const uint8 CH2_Data = 0x02;

    static const int CHStatus_Reset = 100;

    static const QString CH1_Serial_Name;
    static const QString CH2_Serial_Name;

    static const QColor CH1_Serial_Color;
    static const QColor CH2_Serial_Color;

    static const QString CH1_SerialPort_Name;
    static const QString CH2_SerialPort_Name;


public:
    /**
     * @brief Status_ToString
     * @param nStatus
     * @return
     */
    static QString Status_ToString(uint8 nStatus);

public:
    /** Filter Threshold Value Definitions */
    static const double Filter_Threshold_Up = 8.0f;
    static const double Filter_Threshold_Down = 4.0f;

    /** Channel Status Definitions */
    static const uint8 CH_Status_None = 0x01;
    static const uint8 CH_Status_High = 0x02;
    static const uint8 CH_Status_Low = 0x03;

    /** Channel Direct Definitions */
    static const uint8 CH_Direct_None = 0x04;
    static const uint8 CH_Direct_HL = 0x05;
    static const uint8 CH_Direct_LH = 0x06;
    static const uint8 CH_Direct_Pos = 0x07;
    static const uint8 CH_Direct_Neg = 0x08;

    /** Channel Status Keep Time Length */
    static const int Status_Keep_Time = 300;
};

#endif // GLOBAL_H
