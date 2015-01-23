#include "BatteryMonitor.h"

#define ADC_SET_CHANNEL         0xc000fa01
#define ADC_SET_ADCTSC          0xc000fa02


BatteryMonitor::BatteryMonitor(QObject *parent) :
    QThread(parent)
{
}


void BatteryMonitor::run()
{
    int nFileDesc = -1;

    while (m_bEnableRunning == true)
    {
        /** Open Device */
        nFileDesc = open("/dev/adc", 0);
        if (nFileDesc < 0)
        {
            /** Send Update Signale, Status = -1; */
            emit UpdateBatteryStatusSignal(Global::ADC_Open_Failed);
            continue;
        }

        /** Set Device AD Channels, Default : 0 */
        if (ioctl(nFileDesc, ADC_SET_CHANNEL, 0) < 0)
        {
            /** Set Failed, Close Device */
            close(nFileDesc);

            /** Send Update Signal */
            emit UpdateBatteryStatusSignal(Global::ADC_Set_Failed);
            continue;
        }

        /** Read Device */
        char pDataBuf[30];
        int nLength = read(nFileDesc, pDataBuf, sizeof(pDataBuf) - 1);

        /** In Read Successed Case */
        if (nLength > 0)
        {
            /** Calc Battery Captility */
            pDataBuf[nLength] = '\0';

            /** Calc Battery Captility */
            int nBatteryCap = Global::ADC_Trans_Failed;
            sscanf(pDataBuf, "%d", &nBatteryCap);

            /** Send Update Signal */
            emit UpdateBatteryStatusSignal(nBatteryCap);
        }
        /** In Read Failed Case */
        else
        {
            /** Read Failed, Close Device */
            close(nFileDesc);

            /** Send Update Signal */
            emit UpdateBatteryStatusSignal(Global::ADC_Read_Failed);
        }

        /** We Check Battery Every 10s */
        sleep(10);
    }

    /** Read Failed, Close Device */
    close(nFileDesc);

    /** Debug Output */
    qDebug() << "Battery Monitor Thread Exited!";
}

/**
 * @brief getEnableRunning
 * @return bool
 */
bool BatteryMonitor::getEnableRunning() const
{
    return m_bEnableRunning;
}

/**
 * @brief setEnableRunning
 * @param bEnableRunning : bool : Work is Enable Running or Not
 */
void BatteryMonitor::setEnableRunning(bool bEnableRunning)
{
    m_bEnableRunning = bEnableRunning;
}
