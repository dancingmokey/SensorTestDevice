#include "DataSource.h"

/**
 * @brief DataSource Custom Constructor Function
 * @param strSrcName : QString : Data Source Name
 */
DataSource::DataSource(QString strSrcName) :
    m_pSerialPort(NULL),
    m_strSrcName(QString("Unknown"))
{
    /** */
    m_strSrcName = strSrcName;
}

/**
 * @brief DataSource Custom Destrcuctor Function
 */
DataSource::~DataSource(void)
{
    m_bEnableRunning = false;

    qDebug() << "Destructor Function!!!!";
}

/**
 * @brief InitSerialPort
 * @param strPortName : QString : Serial Port Name
 * @return bool
 */
bool DataSource::InitSerialPort(QString strPortName)
{
    /** Set Port Name */
    m_pSerialPort = new Posix_QextSerialPort();
    m_pSerialPort->setPortName(strPortName);

    /** Check Serial Port is Open or Not */
    bool bIsOpened = m_pSerialPort->open(QIODevice::ReadWrite);
    if (bIsOpened == true)
    {
        /** Set Other Attributes of Serial Port */
        m_pSerialPort->setBaudRate(BAUD115200);
        m_pSerialPort->setFlowControl(FLOW_OFF);
        m_pSerialPort->setDataBits(DATA_8);
        m_pSerialPort->setStopBits(STOP_1);
        m_pSerialPort->setParity(PAR_NONE);
        m_pSerialPort->setQueryMode(QextSerialBase::Polling);
        m_pSerialPort->setTimeout(5);
    }
    else
    {
        /** Default Operate, Close Serial Port */
        m_pSerialPort->close();
    }

    /** Return Serial Port is Open or Not */
    return bIsOpened;
}

/**
 * @brief DestroySerialPort
 */
void DataSource::DestroySerialPort(void)
{
    /** Close Serial Port */
    if (m_pSerialPort->isOpen() == true)
    {
        m_pSerialPort->close();
    }

    /** Release Serial Port Memory */
    Global::Safe_Delete(m_pSerialPort);
}

/**
 * @brief run
 * @desc  Thread Work Function, Read Data from Serial Port and then Storage it into List
 */
void DataSource::run(void)
{
#ifdef _DEBUG_SIMULATE
    /** Generate Ranfom Numbers Seed*/
    QTime vCurrTime = QTime::currentTime();
    qsrand(vCurrTime.msec() + vCurrTime.second() * 1000);
#endif

#ifdef _DEBUG_OUTPUT
    /** Debug Output Variable */
    int nLoopCount = 0;
#endif

    /** Loop of Work Thread */
    while (m_bEnableRunning == true)
    {
        /** Declare Data Buffer */
        QByteArray acRecvBuf;

#ifdef _DEBUG_SIMULATE
        /** Generate Random Numbers */
        int nValue = qrand() % 130 - 10;    /** Random Number Range : -10 ~ 120 */
        for (int i = 0; i < 16; ++i)
        {
            acRecvBuf.append(nValue);
        }

#else
        /** Serial Port is Closed, Exit Thread */
        if (m_pSerialPort->isOpen() == false)
        {
            break;
        }

        /** Read Data From Serial Port */
        int nAvaliableCnt = m_pSerialPort->bytesAvailable();
        if (nAvaliableCnt > 0)
        {
            /** Read Data */
            acRecvBuf = m_pSerialPort->readAll();
        }
#endif
        if (acRecvBuf.size() > 0)
        {
            /** Storage Data into Global List */
            if (m_strSrcName == Global::CH1_Serial_Name)
            {
                g_mutexCurve1Locker.lock();
                g_acCurve1.append(acRecvBuf);
                g_mutexCurve1Locker.unlock();
            }
            else if (m_strSrcName == Global::CH2_Serial_Name)
            {
                g_mutexCurve2Locker.lock();
                g_acCurve2.append(acRecvBuf);
                g_mutexCurve2Locker.unlock();
            }

#ifdef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz ")
                     << " Loop Times "
                     << (++nLoopCount)
                     << " Data Source "
                     << m_strSrcName
                     << " Received Data "
                     << acRecvBuf.size()
                     << " Byte!";
#endif
        }

        /** Sleep Thread 10ms*/
        msleep(10);
    }

    /** Destroy Serial Port */
#ifndef _DEBUG_SIMULATE
    DestroySerialPort();
#endif

    /** Debug Output */
    qDebug() << m_strSrcName << " Data Source Thread End!!!!";
}

/**
 * @brief getSourceName
 * @return QString
 */
QString DataSource::getSourceName() const
{
    return m_strSrcName;
}

/**
 * @brief getEnableRunning
 * @return bool
 */
bool DataSource::getEnableRunning() const
{
    return m_bEnableRunning;
}

/**
 * @brief setSourceName
 * @param strSrcName : QString : Data Source Name
 */
void DataSource::setSourceName(const QString &strSrcName)
{
    m_strSrcName = strSrcName;
}

/**
 * @brief setEnableRunning
 * @param bEnableRunning : bool : Work is Enable Running or Not
 */
void DataSource::setEnableRunning(bool bEnableRunning)
{
    m_bEnableRunning = bEnableRunning;
}

