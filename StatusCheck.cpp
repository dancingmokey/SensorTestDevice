#include "StatusCheck.h"

StatusCheck::StatusCheck(QObject *parent) :
    QThread(parent)
{
    /** Initialize Class Members */
    m_nCH1Status = Global::Status_None;
    m_dtCH1Catch = QTime::currentTime();
    m_nCH1CatchOffset = 0;
    m_dCH1ExtremumVal = 0.0f;
    m_nCH2Status = Global::Status_None;
    m_dtCH2Catch = QTime::currentTime();
    m_nCH2CatchOffset = 0;
    m_dCH2ExtremumVal = 0.0f;
}


/**
 * @brief run
 * @desc  Thread Work Function, Read Data from Serial Port and then Storage it into List
 */
void StatusCheck::run(void)
{
    /** Declare Variables */
    uint8 nLastCH1Status = Global::Status_None;
    uint8 nLastCH2Status = Global::Status_None;

    /** Loop of Work Thread */
    while (m_bEnableRunning == true)
    {
        /** Check Channel 1 Status */
        if (m_nCH1Status != nLastCH1Status)
        {
            /** Send Update Channel1 Status Info Signal */
            emit UpdateCHStatusSignal(Global::CH1_Serial_Name,
                                   (unsigned char)m_nCH1Status,
                                   m_dtCH1Catch,
                                   m_nCH1CatchOffset,
                                   m_dCH1ExtremumVal);

            /** Reset Last Status */
            nLastCH1Status = m_nCH1Status;

#ifndef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz")
                     << " Now Channel 1 Status is "
                     << Global::Status_ToString(m_nCH1Status)
                     << " Catch Time is  "
                     << m_dtCH1Catch.toString("hh:mm:ss.zzz")
                     << " Catch Offset is "
                     << m_nCH1CatchOffset
                     << " Extremum Value is "
                     << m_dCH1ExtremumVal;
#endif
        }

        /** Check Channel 2 Data */
        if (m_nCH2Status != nLastCH2Status)
        {
            /** Send Update Channel2 Status Info Signal */
            emit UpdateCHStatusSignal(Global::CH2_Serial_Name,
                                   (unsigned char)m_nCH2Status,
                                   m_dtCH2Catch,
                                   m_nCH2CatchOffset,
                                   m_dCH2ExtremumVal);


            /** Reset Last Status */
            nLastCH2Status = m_nCH2Status;

#ifndef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz")
                     << " Now Channel 2 Status is "
                     << Global::Status_ToString(m_nCH2Status)
                     << " Catch Time is  "
                     << m_dtCH2Catch.toString("hh:mm:ss.zzz")
                     << " Catch Offset is "
                     << m_nCH2CatchOffset
                     << " Extremum Value is "
                     << m_dCH2ExtremumVal;
#endif
        }

        /** Check Sensor Status */
        if ((m_nCH1Status == Global::Status_Catch) ||
            (m_nCH2Status == Global::Status_Catch))
        {
            int nTimeDiff = m_dtCH1Catch.msecsTo(m_dtCH2Catch);
            if (abs(nTimeDiff) < Global::Catch_Keep_Time)
            {
                emit UpdateSensorStatusSignal(nTimeDiff);
            }
        }

#ifndef _DEBUG_OUTPUT
        /** Debug Output */
        qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                 << " Now Channel 1 Status is "
                 << Global::Status_ToString(m_nCH1Status)
                 << " Channel 2 Status is "
                 << Global::Status_ToString(m_nCH2Status);
#endif

        /** Sleep Thread 10ms*/
        msleep(10);
    }

    /** Debug Output */
    qDebug() << "Status Check Thread End!!!!";
}

/**
 * @brief StatusCheck::UpdateChannelData
 * @param nCHType
 * @param nCHStatus
 * @param dtCHCatch
 * @param nCHCatchOffset
 * @param nCHExtremumVal
 */
void StatusCheck::UpdateChannelData(QString strCHName,
                                    uint8 nCHStatus,
                                    QTime dtCHCatch,
                                    int nCHCatchOffset,
                                    double nCHExtremumVal)
{
    if (strCHName == Global::CH1_Serial_Name)
    {
        m_mutexCH1Locker.lock();
        m_nCH1Status = nCHStatus;
        m_dtCH1Catch = dtCHCatch;
        m_nCH1CatchOffset = nCHCatchOffset;
        m_dCH1ExtremumVal = nCHExtremumVal;
        m_mutexCH1Locker.unlock();
    }
    else if (strCHName == Global::CH2_Serial_Name)
    {
        m_mutexCH2Locker.lock();
        m_nCH2Status = nCHStatus;
        m_dtCH2Catch = dtCHCatch;
        m_nCH2CatchOffset = nCHCatchOffset;
        m_dCH2ExtremumVal = nCHExtremumVal;
        m_mutexCH2Locker.unlock();
    }
}

/**
 * @brief getEnableRunning
 * @return bool
 */
bool StatusCheck::getEnableRunning() const
{
    return m_bEnableRunning;
}

/**
 * @brief setEnableRunning
 * @param bEnableRunning : bool : Work is Enable Running or Not
 */
void StatusCheck::setEnableRunning(bool bEnableRunning)
{
    m_bEnableRunning = bEnableRunning;
}
