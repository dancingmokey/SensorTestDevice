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
                         QObject *parent) :
    QThread(parent)
{
    /** Set Class Private Members Value */
    m_strProcName = strProcName;
    m_pCurve1 = pCurve1;
    m_pCurve2 = pCurve2;

    /** Initial Channel Direct & Status */
    m_nChannelDirect = Global::CH_Direct_None;
    m_nChannelStatus = Global::CH_Status_None;
    m_bIsPause = false;
    m_nProcValueCnt = 0;
    m_nPausePosition = 0;
    m_bIsSendPauseSig = false;
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
            acDataBuf.append(g_acCurve2);
            g_acCurve2.clear();
            g_mutexCurve1Locker.unlock();
        }
        else if (m_strProcName == Global::CH2_Serial_Name)
        {
            g_mutexCurve2Locker.lock();
            acDataBuf.append(g_acCurve1);
            g_acCurve1.clear();
            g_mutexCurve2Locker.unlock();
        }

        /** The Pulse has been Paused, Don't Process any Data */
        m_mutexWRLock.lock();
        bool bIsPause = m_bIsPause;
        m_mutexWRLock.unlock();

        /** */
        if (bIsPause == true)
        {
            /** Pause Data Process */
            if (m_nProcValueCnt >= m_nPausePosition)
            {
                /** Is Send Pause Signal Already ? */
                if (m_bIsSendPauseSig == false)
                {
                    double dNewMaxVal = 0.0f;
                    double dNewMinVal = 0.0f;

                    if (m_strProcName == Global::CH1_Serial_Name)
                    {
                        g_mutexCurve1Locker.lock();
                        dNewMaxVal = m_pCurve1->getXAxis()->getMaxValue();
                        dNewMinVal = m_pCurve1->getXAxis()->getMinValue();
                        g_mutexCurve1Locker.unlock();
                    }
                    else if (m_strProcName == Global::CH2_Serial_Name)
                    {
                        g_mutexCurve2Locker.lock();
                        dNewMaxVal = m_pCurve2->getXAxis()->getMaxValue();
                        dNewMinVal = m_pCurve2->getXAxis()->getMinValue();
                        g_mutexCurve2Locker.unlock();
                    }

                    emit DataProcPauseSignal(m_strProcName, dNewMaxVal, dNewMinVal);
                    m_bIsSendPauseSig = true;
                }

                continue;
            }
            /** Hadnot Reached Pause Position, Keep Processing Data */
            else
            {
                m_bIsSendPauseSig = false;
            }
        }
        /** Device isnot Paused, Keep Processing Data */
        else
        {
            m_bIsSendPauseSig = false;
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
                double dValue = (double)nValue / 10.0f;
                pValueList[nValueCount++] = dValue;
                m_nProcValueCnt++;

                /** Make Value Through Filter */
                dValue = getFilterValue(dValue);

                /** Update Channel Status */
                UpdateChannelStatus(dValue);
#else
                if (((uint8)acDataBuf.at(i)) != 0xAB)
                {
                    /** Calculate Value */
                    int nValue = ((uint8)acDataBuf.at(i));
                    double dValue = (((double)nValue) / 10.0f);
                    pValueList[nValueCount++] = dValue;
                    m_nProcValueCnt++;

                    /** Make Value Through Filter */
                    dValue = getFilterValue(dValue);

                    /** Update Channel Status */
                    UpdateChannelStatus(dValue);
                }
#endif
            }

            /** Update Curve */
            if (m_strProcName == Global::CH1_Serial_Name)
            {
                g_mutexCurve1Locker.lock();
                m_pCurve1->AddSerialData(m_strProcName, NULL, pValueList, nValueCount);
                g_mutexCurve1Locker.unlock();
            }
            else if (m_strProcName == Global::CH2_Serial_Name)
            {
                g_mutexCurve2Locker.lock();
                m_pCurve2->AddSerialData(m_strProcName, NULL, pValueList, nValueCount);
                g_mutexCurve2Locker.unlock();
            }

#ifdef _DEBUG_OUTPUT
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
    /** Save Current Status */
    uint8 nLastCHStatus = m_nChannelStatus;

    /** Current Status is None */
    if (m_nChannelStatus == Global::CH_Status_None)
    {
        /** Change Status to High */
        if (dCurrValue >= Global::Filter_Threshold_Up)
        {
            /** Set Values */
            m_nChannelStatus = Global::CH_Status_High;

#ifdef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << m_strProcName
                     << " Status Changed from None to High";
#endif
        }
        /** Keep Status to None if Value not Changed */
        else if (dCurrValue <= Global::Filter_Threshold_Down)
        {
            /** Set Values */
            m_nChannelStatus = Global::CH_Status_Low;

#ifdef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << m_strProcName
                     << " Status Changed from None to Low";
#endif
        }
    }
    /** Current Status is High */
    else if (m_nChannelStatus == Global::CH_Status_High)
    {
        /** Change Status to High */
        if (dCurrValue <= Global::Filter_Threshold_Down)
        {
            /** Set Values */
            m_nChannelStatus = Global::CH_Status_Low;

#ifdef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << m_strProcName
                     << " Status Changed from High to Low";
#endif
        }
        /** When Value has been Changed, Keep Status to Catch*/
        else
        {
            m_nChannelStatus = Global::CH_Status_High;
        }
    }
    /** Current Status is High */
    else if (m_nChannelStatus == Global::CH_Status_Low)
    {
        /** Change Status to High */
        if (dCurrValue >= Global::Filter_Threshold_Up)
        {
            /** Set Values */
            m_nChannelStatus = Global::CH_Status_High;

#ifdef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << m_strProcName
                     << " Status Changed from Low to High";
#endif
        }
        /** Keep Status to None if Value not Changed */
        /** When Value has been Changed, Keep Status to Catch*/
        else
        {
            m_nChannelStatus = Global::CH_Status_Low;
        }
    }
    /** Unknown Status, Change it to None */
    else
    {
#ifdef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << m_strProcName
                     << " Status Changed from "
                     << Global::Status_ToString(m_nChannelStatus)
                     << " to None";
#endif

        /** Default Process */
        m_nChannelStatus = Global::CH_Status_None;
    }

    /** Update Channel Direct */
    this->UpdateChannelDirect(nLastCHStatus);
}

/**
 * @brief DataProcess::UpdateChannelDirect
 * @param nLastCHStatus
 */
void DataProcess::UpdateChannelDirect(uint8 nLastCHStatus)
{
    /** */
    if ((nLastCHStatus == Global::CH_Status_None) ||
        (m_nChannelStatus == Global::CH_Status_None))
    {
        m_nChannelDirect = Global::CH_Direct_None;
        return;
    }

    /** Get Current Time */
    QTime dtCurr = QTime::currentTime();

    /** */
    if (nLastCHStatus != m_nChannelStatus)
    {
        if (nLastCHStatus == Global::CH_Status_High)
        {
            if ((m_nChannelDirect == Global::CH_Direct_None) ||
                (m_nChannelDirect == Global::CH_Direct_Pos) ||
                (m_nChannelDirect == Global::CH_Direct_Neg))
            {
                m_nChannelDirect = Global::CH_Direct_HL;
                m_dtDirectChange = dtCurr;

#ifdef _DEBUG_OUTPUT
                /** Debug Output */
                qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                         << m_strProcName
                         << " Direct Changed from None to High-Low at Point "
                         << m_nProcValueCnt;
#endif
            }
            else if (m_nChannelDirect == Global::CH_Direct_LH)
            {
                m_nChannelDirect = Global::CH_Direct_Neg;
                m_dtDirectChange = dtCurr;

                emit UpdateCHDirectSignal();

#ifdef _DEBUG_OUTPUT
                /** Debug Output */
                qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                         << m_strProcName
                         << " Direct Changed from Low-High to Negtive at Point "
                         << m_nProcValueCnt;
#endif
            }
        }
        else if (nLastCHStatus == Global::CH_Status_Low)
        {
            if ((m_nChannelDirect == Global::CH_Direct_None) ||
                (m_nChannelDirect == Global::CH_Direct_Pos) ||
                (m_nChannelDirect == Global::CH_Direct_Neg))
            {
                m_nChannelDirect = Global::CH_Direct_LH;
                m_dtDirectChange = dtCurr;

#ifdef _DEBUG_OUTPUT
                /** Debug Output */
                qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                         << m_strProcName
                         << " Direct Changed from None to Low-High at Point "
                         << m_nProcValueCnt;
#endif
            }
            else if (m_nChannelDirect == Global::CH_Direct_HL)
            {
                m_nChannelDirect = Global::CH_Direct_Pos;
                m_dtDirectChange = dtCurr;

                emit UpdateCHDirectSignal();

#ifdef _DEBUG_OUTPUT
                /** Debug Output */
                qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                         << m_strProcName
                         << " Direct Changed from High-Low to Positive at Point "
                         << m_nProcValueCnt;
#endif
            }
        }
    }

    /** */
    if (m_dtDirectChange.msecsTo(dtCurr) >= Global::Status_Keep_Time)
    {
        if (m_nChannelDirect == Global::CH_Direct_LH)
        {
            m_nChannelDirect = Global::CH_Direct_Pos;
            m_dtDirectChange = dtCurr;

            emit UpdateCHDirectSignal();

#ifdef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << m_strProcName
                     << " Direct Changed from Low-High to Positive Cause Keep Timeout at Point "
                     << m_nProcValueCnt;
#endif
        }
        else if (m_nChannelDirect == Global::CH_Direct_HL)
        {
            m_nChannelDirect = Global::CH_Direct_Neg;
            m_dtDirectChange = dtCurr;

            emit UpdateCHDirectSignal();

#ifdef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << m_strProcName
                     << " Direct Changed from High-Low to Negative Cause Keep Timeout at Point "
                     << m_nProcValueCnt;
#endif
        }
        else
        {
             if (m_dtDirectChange.msecsTo(dtCurr) >= (Global::Status_Keep_Time * 10))
             {
                 m_nChannelDirect = Global::CH_Direct_None;
                 m_dtDirectChange = dtCurr;

#ifdef _DEBUG_OUTPUT
                 /** Debug Output */
                 qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                          << m_strProcName
                          << " Direct Changed to None Cause Keep Timeout";
#endif
             }
        }
    }
}

/**
 * @brief DataProcess::getFilterValue
 * @param dCurrValue
 * @return
 */
double DataProcess::getFilterValue(double dCurrValue)
{
    /** Value More than Up Threshold */
    if (dCurrValue > Global::Filter_Threshold_Up)
    {
        dCurrValue = Global::Filter_Threshold_Up;
    }
    /** Value Less than Down Threshold */
    else if (dCurrValue < Global::Filter_Threshold_Down)
    {
        dCurrValue = Global::Filter_Threshold_Down;
    }

    /** Return Value Through Filter */
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

/**
 * @brief getChannelStatus
 * @return
 */
uint8 DataProcess::getChannelStatus(void)
{
    return m_nChannelStatus;
}

/**
 * @brief getChannelDirect
 * @return
 */
uint8 DataProcess::getChannelDirect(void)
{
    return m_nChannelDirect;
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
 * @brief setPausePosition
 * @param bIsPause
 * @param nDelayValue
 */
void DataProcess::setPausePosition(bool bIsPause, int nDelayValue)
{
    /** */
    m_mutexWRLock.lock();
    m_bIsPause = bIsPause;
    m_nPausePosition = m_nProcValueCnt + nDelayValue;
    m_mutexWRLock.unlock();
}

