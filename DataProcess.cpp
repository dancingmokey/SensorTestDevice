#include "DataProcess.h"

/**
 * @brief DataProcess Custom Destructor Function`
 * @param strProcName : QString : Data Processor Name
 * @param pCurve1 : Curve* : Curve 1
 * @param pCurve2 : Curve* : Curve 2
 * @param parent
 */
DataProcess::DataProcess(QString strProcName, Curve* pCurve1, Curve* pCurve2, QObject *parent) :
    QThread(parent)
{
    /** Set Class Private Members Value */
    m_strProcName = strProcName;
    m_pCurve1 = pCurve1;
    m_pCurve2 = pCurve2;
}

/**
 * @brief run
 * @desc Thread Work Function, Process Data from List
 */
void DataProcess::run(void)
{
    /** Main Loop of Work Thread */
    int nLoopCount = 0;
    while (m_bEnableRunning == true)
    {
        /** Read Data From Global Storage List */
        QByteArray acDataBuf;
        g_mutexCurve1Locker.lock();
        acDataBuf.append(g_acCurve1);
        g_acCurve1.clear();
        g_mutexCurve1Locker.unlock();

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
                    int nValue = ((uint8)acDataBuf.at(i));
                    pValueList[nValueCount++] = (((double)nValue) / 10.0f);
                }
#endif
            }

            /** Update Curve */
            g_mutexCurve1Locker.lock();
            m_pCurve1->AddSerialData(m_strProcName, NULL, pValueList, nValueCount);
            g_mutexCurve1Locker.unlock();

            g_mutexCurve2Locker.lock();
            m_pCurve2->AddSerialData(m_strProcName, NULL, pValueList, nValueCount);
            g_mutexCurve2Locker.unlock();

            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << " Loop Times "
                     << (++nLoopCount)
                     << " Data Process "
                     << m_strProcName
                     << " Process Data "
                     << nValueCount
                     << " Byte!";
        }
    }

    /** Debug Output */
    qDebug() << m_strProcName << " Data Process Thread End!!!!";
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
