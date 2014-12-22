#include "Curve.h"

/**
 * @brief Curve Class Constructor Function
 * @param pBoundRect : QRectF* : Bound Rectangle of Curve Area
 */
Curve::Curve(QRectF* pBoundRect)
{
    /** Set Bound Rectangle with pBoundRect */
    m_pBoundRect = pBoundRect;

    /** Reset X&Y Axis */
    this->CreateAxis();
}

/**
 * @brief Curve class Destructor Function
  */
Curve::~Curve(void)
{
    /** Release Serials Memories */
    for (QMap<QString, Serial*>::iterator it = m_mpSerials.begin();
         it != m_mpSerials.end();
         it++)
    {
        Serial* pSerial = it.value();
        Global::Safe_Delete(pSerial);
    }

    /** Release X Axis Memories */
    Global::Safe_Delete(m_pXAxis);

    /** Release Y Axis Memories */
    Global::Safe_Delete(m_pYAxis);

    /** Release Bound Rectangle Memories */
    Global::Safe_Delete(m_pBoundRect);
}

/**
 * @brief CreateAxis
 */
void Curve::CreateAxis(void)
{
    /** Create Horizontal Axis */
    m_pXAxis = new Axis(this->CalcAxisLine(Global::Axis_Horizontal),
                        2000,
                        0,
                        200);

    /** Create Vertical Axis */
    m_pYAxis = new Axis(this->CalcAxisLine(Global::Axis_Vertical),
                        25.5,
                        0,
                        5);
}

/**
 * @brief UpdateCurve
 * @param pBoundRect : QRectF* : Bound Rectangle of Curve Area
 */
void Curve::UpdateCurve(QRectF* pBoundRect)
{
    /** Set Bound Rectangle of Curve Area */
    this->setBoundRect(pBoundRect);

    /** Update Horizontal Axis Line */
    this->m_pXAxis->UpdateAxisLayout(
                this->CalcAxisLine(Global::Axis_Horizontal));

    /** Updaye Vertical Axis Line */
    this->m_pYAxis->UpdateAxisLayout(
                this->CalcAxisLine(Global::Axis_Vertical));
}

/**
 * @brief UpdateCurve
 * @param nAxisType : uint8 : Axis Type(Horizontal or Vertical)
 * @param dMaxValue : double : Maximum Value of Axis
 * @param dMinValue : double : Minimum Value of Axis
 * @param dTickValue : double : Single Tick Value of Axis
 */
void Curve::UpdateCurve(uint8 nAxisType, double dMaxValue, double dMinValue, double dTickValue)
{
    /** in Horizontal Axis Case */
    if (nAxisType == Global::Axis_Horizontal)
    {
        this->m_pXAxis->UpdateAxisScale(dMaxValue, dMinValue, dTickValue);
    }
    /** in Vertical Axis and Other Case */
    else
    {
        this->m_pYAxis->UpdateAxisScale(dMaxValue, dMinValue, dTickValue);
    }
}

/**
 * @brief AddSerial
 * @param strSerialName : QString : Serial Name
 */
void Curve::AddSerial(QString strSerialName)
{
    /** Find Serial in Map by Serial Name */
    QMap<QString, Serial*>::iterator it = m_mpSerials.find(strSerialName);

    /** Serial is not Exist in Map, Create a new Serial */
    if (it == m_mpSerials.end())
    {
        Serial* pSerial = new Serial();
        pSerial->setSerialDataCnt(m_pXAxis->getMinValue());
        m_mpSerials.insert(strSerialName, pSerial);
    }
}

/**
 * @brief RemoveSerial
 * @param strSerialName : QString : Serial Name
 */
void Curve::RemoveSerial(QString strSerialName)
{
    /** Find Serial in Map by Serial Name */
    QMap<QString, Serial*>::iterator it = m_mpSerials.find(strSerialName);

    /** Serial Exist in Map, Delete it */
    if (it != m_mpSerials.end())
    {
        /** Release Memory */
        Serial* pSerial = it.value();
        Global::Safe_Delete(pSerial);

        /** Erase from Map */
        m_mpSerials.erase(it);
    }
}

/**
 * @brief AddSerialData
 * @param strSerialName : QString : Serial Name
 * @param pXValueList : double* : Array of Horizontal Values
 * @param pYValueList : double* : Array of Vertical Values
 * @param nCount : int : Count of Array
 */
void Curve::AddSerialData(QString strSerialName, double* pXValueList, double* pYValueList, int nCount)
{
    /** Find target Serial */
    QMap<QString, Serial*>::iterator it = m_mpSerials.find(strSerialName);

    /** in Serial Exist Case */
    if (it != m_mpSerials.end())
    {
        /** Add Data to Serial */
        Serial* pSerial = it.value();
        pSerial->AddSerialData(pXValueList, pYValueList, nCount);

        /** Declare Variables about Size of List and Axis Value Range */
        double dXAxisMaxVal = m_pXAxis->getMaxValue();
        double dXAxisMinVal = m_pXAxis->getMinValue();
        int nMaxPointPerPage = (int)(dXAxisMaxVal - dXAxisMinVal);
        int nDataSize = pSerial->GetSerialData().size();
        int nDiff = nDataSize - nMaxPointPerPage;

        /** in Data List Overflowed Case */
        if (nDataSize > nMaxPointPerPage)
        {
            /** Update Horizaontal Axis */
            double dNewMaxValue = nDiff + dXAxisMaxVal;
            double dNewMinValue = nDiff + dXAxisMinVal;
            m_pXAxis->UpdateAxisScale(dNewMaxValue,
                                      dNewMinValue,
                                      m_pXAxis->getTickValue());

            /** Resize Data in Serial */
            pSerial->ResizeData(nMaxPointPerPage);
        }
    }
}

/**
 * @brief CalcAxisLine
 * @param nAxisType : uint8 : Axis Type(Horizontal or Vertical)
 * @return QLineF* : Axis Line
 */
QLineF* Curve::CalcAxisLine(uint8 nAxisType)
{
    /** in Horizontal Axis Case */
    if (nAxisType == Global::Axis_Horizontal)
    {
        return (new QLineF(m_pBoundRect->left() + Global::Axis_Margin,
                            m_pBoundRect->bottom() - Global::Axis_Margin,
                            m_pBoundRect->right(),
                            m_pBoundRect->bottom() - Global::Axis_Margin));
    }
    /** in Vertical Axis and Other Case */
    else
    {
        return (new QLineF(m_pBoundRect->left() + Global::Axis_Margin,
                            m_pBoundRect->bottom() - Global::Axis_Margin,
                            m_pBoundRect->left() + Global::Axis_Margin,
                            m_pBoundRect->top()));
    }

}

/**
 * @brief getSerialByName
 * @param strSerialName : QString : Serial Name
 * @return Serial* : Target Serial or NULL
 */
Serial* Curve::getSerialByName(QString strSerialName)
{
    /** Travesal Map */
    QMap<QString, Serial*>::iterator it = m_mpSerials.find(strSerialName);

    /** in Serial Exist in Map case, Return Serial Pointer */
    if (it != m_mpSerials.end())
    {
        return it.value();
    }
    /** in Serial Exist in Map case, Return NULL */
    else
    {
        return NULL;
    }
}

/**
 * @brief getSerialDataByName
 * @param strSerialName : QString : Serial Name
 * @return QList<SerialData*> : Serial Data Found by Name
 */
QList<SerialData*> Curve::getSerialDataByName(QString strSerialName)
{
    /** Declare Variable used to Return Result */
    QList<SerialData*> ltSerialData;

    /** Find Serial Pointer */
    QMap<QString, Serial*>::iterator it = m_mpSerials.find(strSerialName);
    if (it != m_mpSerials.end())
    {
        /** Get Serial by Serial name */
        Serial* pSerial =it.value();

        /** Serial is Exist and Valid */
        if (pSerial != NULL)
        {
            ltSerialData = pSerial->GetSerialData();
        }
    }

    return ltSerialData;
}

/**
 * @brief getBoundRect
 * @return QRectF* : Bound Rectangle of Curve Area
 */
QRectF* Curve::getBoundRect(void) const
{
    return m_pBoundRect;
}

/**
 * @brief getXAxis
 * @return Axis* : X Axis Pointer
 */
Axis* Curve::getXAxis(void) const
{
    return m_pXAxis;
}

/**
 * @brief getYAxis
 * @return Axis* : Y Axis Pointer
 */
Axis* Curve::getYAxis(void) const
{
    return m_pYAxis;
}

/**
 * @brief getSerialsMap
 * @return QMap<QString, Serial*> : Serial Map
 */
QMap<QString, Serial*> Curve::getSerialsMap(void) const
{
    return m_mpSerials;
}

/**
 * @brief setBoundRect
 * @param pBoundRect : QRectF* : Bound Rectangle of Curve Area
 */
void Curve::setBoundRect(QRectF *pBoundRect)
{
    /** Release Memory */
    Global::Safe_Delete(m_pBoundRect);

    /** Set New Value of Bound Rectangle */
    m_pBoundRect = pBoundRect;
}

/**
 * @brief setXAxis
 * @param pXAxis : Axis* : X Axis Pointer
 */
void Curve::setXAxis(Axis* pXAxis)
{
    /** Release Memory */
    Global::Safe_Delete(m_pXAxis);

    /** Set New Value of X Axis */
    m_pXAxis = pXAxis;
}

/**
 * @brief setYAxis
 * @param pYAxis : Axis* : Y Axis Pointer
 */
void Curve::setYAxis(Axis* pYAxis)
{
    /** Release Memory */
    Global::Safe_Delete(m_pYAxis);

    /** Set New Value of Y Axis */
    m_pYAxis = pYAxis;
}
