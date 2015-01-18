#include "Global.h"

const QString Global::CH1_Serial_Name = QString("Channel 1");
const QString Global::CH2_Serial_Name = QString("Channel 2");
const QString Global::CH1_SerialPort_Name = QString("/dev/ttyUSB0");
const QString Global::CH2_SerialPort_Name = QString("/dev/ttyUSB1");


const QColor Global::CH1_Serial_Color = Qt::red;
const QColor Global::CH2_Serial_Color = Qt::blue;

QMutex g_mutexCurve1Locker;
QList<double> g_ltCurve1Data;
QByteArray g_acCurve1;
QMutex g_mutexCurve2Locker;
QList<double> g_ltCurve2Data;
QByteArray g_acCurve2;

QList<QString> g_ltXLabelTexts;
QTime g_dtLastUpDate;

Global::Global()
{
}

/**
 * @brief Safe_Delete
 * @param pPointer : void* : Pointer need to be Deleted
 */
void Global::Safe_Delete(void* pPointer)
{
    if (pPointer != NULL)
    {
        /** Release Memory Space and Set Pointer to NULL */
        delete pPointer;
        pPointer = NULL;
    }
}

QString Global::Status_ToString(uint8 nStatus)
{
    if (nStatus == Global::CH_Status_High)
    {
        return QString("High");
    }
    else if (nStatus == Global::CH_Status_Low)
    {
        return QString("Low");
    }
    else if (nStatus == Global::CH_Status_None)
    {
        return QString("None");
    }
    else
    {
        return QString("Unknown");
    }
}
