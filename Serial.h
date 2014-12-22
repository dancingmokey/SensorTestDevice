#ifndef SERIAL_H
#define SERIAL_H

/**
 * @brief The SerialData class
 */
class SerialData
{
public:
    /**
     * @brief SerialData
     * @param dX : double : Horizontal Value of Serial Data
     * @param dY : double : Vertical Value of Serial Data
     */
    SerialData(double dX, double dY)
    {
        m_dX = dX;
        m_dY = dY;
    }
public:
    /**
     * @brief m_dX : double : Horizontal Value of Serial Data
     */
    double m_dX;

    /**
     * @brief m_dY : double : Vertical Value of Serial Data
     */
    double m_dY;
};


#include <QList>
#include <QPointF>
#include "Axis.h"

/**
 * @brief The Serial class
 */
class Serial
{
public:
    /**
     * @brief Serial Class Default Constructor Function
     */
    Serial(void);

public:
    /**
     * @brief AddSerialData
     * @param pXValueList : double* : Array of Horizontal Values
     * @param pYValueList : double* : Array of Vertical Values
     * @param nCount : int : Count of Array
     */
    void AddSerialData(double* pXValueList, double* pYValueList, int nCount);

    /**
     * @brief RemoveSerialData
     * @param nCount : int : Remove First nCount Elements from List of Serial Data
     */
    void RemoveSerialData(int nCount);

    /**
     * @brief ResizeData
     * @param nMaxSize : int : Max Size Limitation
     */
    void ResizeData(int nMaxSize);

public:
    /**
     * @brief GetSerialData
     * @return QList<SerialData*> : List of Serial Data
     */
    QList<SerialData*> GetSerialData(void);

    /**
     * @brief getSerialDataCnt
     * @return int
     */
    int getSerialDataCnt() const;

    /**
     * @brief setSerialDataCnt
     * @param nSerialDataCnt : int : Data Count
     */
    void setSerialDataCnt(int nSerialDataCnt);

private:
    /**
     * @brief m_ltSerialData : QList<SerialData*> : List of Serial Data
     */
    QList<SerialData*> m_ltSerialData;

    /**
     * @brief m_nSerialDataCnt : int : Data Count
     */
    int m_nSerialDataCnt;
};

#endif // SERIAL_H
