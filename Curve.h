#ifndef CURVE_H
#define CURVE_H

#include <QRectF>
#include <QLineF>
#include <QVector>
#include <QList>
#include <QMap>
#include <QDebug>
#include <QThread>

#include "Axis.h"
#include "Serial.h"
#include "Global.h"

/**
 * @brief The Curve class
 */
class Curve/* : public QThread*/
{
public:
    /**
     * @brief Curve Class Constructor Function
     * @param pBoundRect : QRectF* : Bound Rectangle of Curve Area
     */
    Curve(QRectF* pBoundRect);

    /**
     * @brief Curve class Destructor Function
      */
    ~Curve(void);

public:
    /**
     * @brief CreateAxis
     */
    void CreateAxis(void);

    /**
     * @brief UpdateCurve
     * @param pBoundRect : QRectF* : Bound Rectangle of Curve Area
     */
    void UpdateCurve(QRectF* pBoundRect);

    /**
     * @brief UpdateCurve
     * @param nAxisType : uint8 : Axis Type(Horizontal or Vertical)
     * @param dMaxValue : double : Maximum Value of Axis
     * @param dMinValue : double : Minimum Value of Axis
     * @param dTickValue : double : Single Tick Value of Axis
     */
    void UpdateCurve(uint8 nAxisType, double dMaxValue, double dMinValue, double dTickValue);

    /**
     * @brief AddSerial
     * @param strSerialName : QString : Serial Name
     */
    void AddSerial(QString strSerialName);

    /**
     * @brief RemoveSerial
     * @param strSerialName : QString : Serial Name
     */
    void RemoveSerial(QString strSerialName);

    /**
     * @brief AddSerialData
     * @param strSerialName : QString : Serial Name
     * @param pXValueList : double* : Array of Horizontal Values
     * @param pYValueList : double* : Array of Vertical Values
     * @param nCount : int : Count of Array
     */
    void AddSerialData(QString strSerialName, double* pXValueList, double* pYValueList, int nCount);

    /**
     * @brief CalcAxisLine
     * @param nAxisType : uint8 : Axis Type(Horizontal or Vertical)
     * @return QLineF* : Axis Line
     */
    QLineF* CalcAxisLine(uint8 nAxisType);

    /**
     * @brief getSerialByName
     * @param strSerialName : QString : Serial Name
     * @return Serial* : Target Serial or NULL
     */
    Serial* getSerialByName(QString strSerialName);

    /**
     * @brief getSerialDataByName
     * @param strSerialName : QString : Serial Name
     * @return QList<SerialData*> : Serial Data Found by Name
     */
    QList<SerialData*> getSerialDataByName(QString strSerialName);

public:
    /**
     * @brief getBoundRect
     * @return QRectF* : Bound Rectangle of Curve Area
     */
    QRectF* getBoundRect(void) const;

    /**
     * @brief getXAxis
     * @return Axis* : X Axis Pointer
     */
    Axis* getXAxis(void) const;

    /**
     * @brief getYAxis
     * @return Axis* : Y Axis Pointer
     */
    Axis* getYAxis(void) const;

    /**
     * @brief getSerialsMap
     * @return QMap<QString, Serial*> : Serial Map
     */
    QMap<QString, Serial*> getSerialsMap(void) const;

    /**
     * @brief setBoundRect
     * @param pBoundRect : QRectF* : Bound Rectangle of Curve Area
     */
    void setBoundRect(QRectF* pBoundRect);

    /**
     * @brief setXAxis
     * @param pXAxis : Axis* : X Axis Pointer
     */
    void setXAxis(Axis* pXAxis);

    /**
     * @brief setYAxis
     * @param pYAxis : Axis* : Y Axis Pointer
     */
    void setYAxis(Axis* pYAxis);

private:
    /**
     * @brief m_pBoundRect : QRectF* : Bound Rectangle of Curve
     */
    QRectF* m_pBoundRect;

    /**
     * @brief m_pXAxis : Axis* : Horizontal Axis
     */
    Axis* m_pXAxis;

    /**
     * @brief m_pYAxis : Axis* : Vertical Axis
     */
    Axis* m_pYAxis;

    /**
     * @brief m_mpSerials : QMap<QString, Serial*> : Serials Map with Serial Name
     */
    QMap<QString, Serial*> m_mpSerials;

};

#endif // CURVE_H
