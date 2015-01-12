#include "DataProcess.h"

/**
 * @brief DataProcess Custom Destructor Function`
 * @param strProcName : QString : Data Processor Name
 * @param pCurve1 : Curve* : Curve 1
 * @param pCurve2 : Curve* : Curve 2
 * @param parent
 */
DataProcess::DataProcess(QString strProcName,
                         Curve* pCurve1,
                         Curve* pCurve2,
                         StatusCheck* pStatusCheck,
                         QObject *parent) :
    QThread(parent)
{
    /** Set Class Private Members Value */
    m_strProcName = strProcName;
    m_pCurve1 = pCurve1;
    m_pCurve2 = pCurve2;
    m_bIsFirst = true;
    m_nProcValueCnt = 0;
    m_nStatus = Global::Status_None;
    m_nCatchOffset = 0;
    m_dExtremumVal = 0.0f;
    m_dtCatch = QTime::currentTime();
    m_pStatusCheck = pStatusCheck;
    m_bIsPause = false;
    m_bIsCatched = false;
}

/**
 * @brief run
 * @desc Thread Work Function, Process Data from List
 */
void DataProcess::run(void)
{
    /** Main Loop of Work Thread */
#ifdef _DEBUG_OUTPUT
    int nLoopCount = 0;
#endif
    while (m_bEnableRunning == true)
    {
        /** Read Data From Global Storage List */
        QByteArray acDataBuf;

        /** Get Data List From Global Storage List */
        if (m_strProcName == Global::CH1_Serial_Name)
        {
            g_mutexCurve1Locker.lock();
            acDataBuf.append(g_acCurve1);
            g_acCurve1.clear();
            g_mutexCurve1Locker.unlock();
        }
        else if (m_strProcName == Global::CH2_Serial_Name)
        {
            g_mutexCurve2Locker.lock();
            acDataBuf.append(g_acCurve2);
            g_acCurve2.clear();
            g_mutexCurve2Locker.unlock();
        }

        /** The Pulse has been Catched, Don't Process any Data */
        if ((m_bIsCatched == true) &&
            (m_bIsPause == true))
        {
            if (m_nProcValueCnt >= m_nPausePosition)
            {
                continue;
            }
        }
        else if ((m_bIsCatched == false) &&
                 (m_bIsPause == true))
        {
            continue;
        }

        /** Check Data is Avaliable */
        int nDataBufSize = acDataBuf.size();
        if (nDataBufSize > 0)
        {
            /** Declare Varibles */
            int nValueCount = 0;
            double pValueList[nDataBufSize];

            /** Travesal Byte Array and Generate Double Serial Data */
            for (int i = 0; i < acDataBuf.size(); ++i)
            {
#ifdef _DEBUG_SIMULATE
                int nValue = (uint8)acDataBuf.at(i);
                pValueList[nValueCount++] = (double)nValue / 10.0f;
#else
                if (((uint8)acDataBuf.at(i)) != 0xAB)
                {
                    /** Calculate Value */
                    int nValue = ((uint8)acDataBuf.at(i));
                    double dValue = (((double)nValue) / 10.0f);
                    pValueList[nValueCount++] = dValue;

                    /** Update Channel Status */
                    dValue = getFilterValue(dValue);
                    UpdateChannelStatus(dValue);
                }
#endif
            }

            /** Update Curve */
            if (m_strProcName == Global::CH1_Serial_Name)
            {
                g_mutexCurve1Locker.lock();
                m_pCurve1->AddSerialData(m_strProcName, NULL, pValueList, nValueCount);
                m_nProcValueCnt += nValueCount;
                g_mutexCurve1Locker.unlock();
            }
            else if (m_strProcName == Global::CH2_Serial_Name)
            {
                g_mutexCurve2Locker.lock();
                m_pCurve2->AddSerialData(m_strProcName, NULL, pValueList, nValueCount);
                m_nProcValueCnt += nValueCount;
                g_mutexCurve2Locker.unlock();
            }

#ifndef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << " Loop Times "
                     << (++nLoopCount)
                     << " Data Process "
                     << m_strProcName
                     << " Process Data "
                     << nValueCount
                     << " Byte! All Data Count is "
                     << m_nProcValueCnt
                     << " Now Status is "
                     << Global::Status_ToString(m_nStatus);
#endif
        }
    }

    /** Debug Output */
    qDebug() << m_strProcName << " Data Process Thread End!!!!";
}

/**
 * @brief DataProcess::UpdateChannelStatus
 * @param dCurrValue
 */
void DataProcess::UpdateChannelStatus(double dCurrValue)
{
    /** in First Update Case */
    if (m_bIsFirst == true)
    {
        m_bIsFirst = false;
        m_dLastValue = dCurrValue;
    }

    /** Get Current Time */
    QTime dtCurr = QTime::currentTime();

    /** Current Status is None */
    if (m_nStatus == Global::Status_None)
    {
        /** Change Status to Catch if Value Changed */
        if (dCurrValue != m_dLastValue)
        {
            /** Set Values */
            m_nStatus = Global::Status_Catch;
            m_dtCatch = dtCurr;
            m_nCatchOffset = m_nProcValueCnt;
            m_dExtremumVal = dCurrValue;
            m_nPausePosition = m_nProcValueCnt + 1000;

            /** Update Channel Status */
            m_pStatusCheck->UpdateChannelData(m_strProcName,
                                              m_nStatus,
                                              m_dtCatch,
                                              m_nProcValueCnt,
                                              m_dExtremumVal);

            qDebug() << m_strProcName
                     << " Catch Pluse at Point "
                     << m_nProcValueCnt
                     << " Stop at Point"
                     << m_nPausePosition;
        }
        /** Keep Status to None if Value not Changed */
        else
        {
            m_nStatus = Global::Status_None;
        }
    }
    /** Current Status is None */
    else if (m_nStatus == Global::Status_Catch)
    {
        /** When Value hasnot been Changed */
        if (dCurrValue == m_dLastValue)
        {
            /** Change Status to None Only when Catch Status keep Time loger than 300ms */
            if (m_dtCatch.msecsTo(dtCurr) > Global::Catch_Keep_Time)
            {
#ifndef _DEBUG_OUTPUT
                qDebug() << m_strProcName
                         << " Status Changed from Catch to None "
                         << "Catch Time = "
                         << m_dtCatch.toString("hh:mm:ss.zzz ")
                         << "Current Time = "
                         << dtCurr.toString("hh:mm:ss.zzz ");
#endif
                m_nStatus = Global::Status_None;

                /** Update Channel Status */
                m_pStatusCheck->UpdateChannelData(m_strProcName,
                                                  m_nStatus,
                                                  m_dtCatch,
                                                  m_nProcValueCnt,
                                                  m_dExtremumVal);
            }
        }
        /** When Value has been Changed, Keep Status to Catch*/
        else
        {
            m_nStatus = Global::Status_Catch;
        }
    }
    /** Unknown Status, Change it to None */
    else
    {
        m_nStatus = Global::Status_None;
    }

    /** Reset Last Value */
    m_dLastValue = dCurrValue;
}


double DataProcess::getFilterValue(double dCurrValue)
{
    if (dCurrValue > Global::Filter_Threshold_Up)
    {
        dCurrValue = Global::Filter_Threshold_Up;
    }
    else if (dCurrValue < Global::Filter_Threshold_Down)
    {
        dCurrValue = Global::Filter_Threshold_Down;
    }

    return dCurrValue;
}


/**
 * @brief getProcName
 * @return QString
 */
QString DataProcess::getProcName() const
{
    return m_strProcName;
}

/**
 * @brief getEnableRunning
 * @return bool
 */
bool DataProcess::getEnableRunning() const
{
    return m_bEnableRunning;
}

int DataProcess::getPausePosition()
{
    return m_nPausePosition;
}

/**
 * @brief setProcName
 * @param strProcName : QString : Data Processor Name
 */
void DataProcess::setProcName(const QString &strProcName)
{
    m_strProcName = strProcName;
}

/**
 * @brief setEnableRunning
 * @param bEnableRunning : bool : Work is Enable Running or Not
 */
void DataProcess::setEnableRunning(bool bEnableRunning)
{
    m_bEnableRunning = bEnableRunning;
}

/**
 * @brief DataProcess::setIsCatched
 * @param bIsCatched
 */
void DataProcess::setIsCatched(bool bIsCatched)
{
    m_bIsCatched = bIsCatched;
}

/**
 * @brief DataProcess::setPause
 * @param bIsPause
 */
void DataProcess::setPause(bool bIsPause)
{
    m_bIsPause = bIsPause;
}

void DataProcess::setPauseCondition(bool bIsPause, int nCatchOffset)
{
    m_bIsPause = bIsPause;
    m_nPausePosition = nCatchOffset;
}

