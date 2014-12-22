#include "Serial.h"

/**
 * @brief Serial Class Default Constructor Function
 */
Serial::Serial(void) :
    m_nSerialDataCnt(0)
{
    /** Do Nothing */
}

/**
 * @brief AddSerialData
 * @param pXValueList : double* : Array of Horizontal Values
 * @param pYValueList : double* : Array of Vertical Values
 * @param nCount : int : Count of Array
 */
void Serial::AddSerialData(double* pXValueList, double* pYValueList, int nCount)
{
    for (int i = 0; i < nCount; ++i)
    {
        /**  */
        double dXValue = (double)(m_nSerialDataCnt++);

        /** Allocate Memory Space for Pointer to Serial Data */
        SerialData* pNewSerialData = new SerialData(dXValue, pYValueList[i]);

        /** Storage into List of Serial Data */
        m_ltSerialData.push_back(pNewSerialData);
    }
}

/**
 * @brief RemoveSerialData
 * @param nCount : int : Remove First nCount Elements from List of Serial Data
 */
void Serial::RemoveSerialData(int nCount)
{
    /** Traversal First nCount Elements in Serial Data List  */
    for (QList<SerialData*>::iterator it = m_ltSerialData.begin();
         it != m_ltSerialData.end();
         it++)
    {
        /** First nCount Elements has been Removed, break */
        if ((nCount--) == 0)
        {
            break;
        }

        /** Safe Delete SerialData Pointer */
        SerialData* pData = *it;
        Global::Safe_Delete(pData);

        /** Remove Element from List of Serial Data */
        m_ltSerialData.erase(it);
    }
}

/**
 * @brief ResizeData
 * @param nMaxSize : int : Max Size Limitation
 */
void Serial::ResizeData(int nMaxSize)
{
    /** Calculate Diff between Serial Data Size and Limited Max Size */
    int nDiff = m_ltSerialData.size() - nMaxSize;

    /** Serial Data Size more than Limited, Remove them */
    if (nDiff > 0)
    {
        RemoveSerialData(nDiff);
    }
}

/**
 * @brief Serial::GetSerialData
 * @return QList<SerialData*> : List of Serial Data
 */
QList<SerialData*> Serial::GetSerialData(void)
{
    return m_ltSerialData;
}

/**
 * @brief nSerialDataCnt
 * @return int
 */
int Serial::getSerialDataCnt() const
{
    return m_nSerialDataCnt;
}

/**
 * @brief setSerialDataCnt
 * @param nSerialDataCnt : int : Data Count
 */
void Serial::setSerialDataCnt(int nSerialDataCnt)
{
    m_nSerialDataCnt = nSerialDataCnt;
}

