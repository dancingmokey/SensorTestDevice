#include "Axis.h"

/**
 * @brief Axis class Constructor Function
 * @param pAxisLine : QLineF* : Line of Axis
 * @param dMaxValue : double : Maximum Value of Axis
 * @param dMinValue : double : Minimum Value of Axis
 * @param dTickValue : double : Every Single Tick Value of Axis
 */
Axis::Axis(QLineF* pAxisLine,
           double dMaxValue,
           double dMinValue,
           double dTickValue)
{
    /** Set Private Members Value */
    this->m_pAxisLine = pAxisLine;
    this->m_dMaxValue = dMaxValue;
    this->m_dMinValue = dMinValue;
    this->m_dTickValue = dTickValue;

    /** Create Axis Scale System */
    this->ResetAxisScale();

    /** Create Axis Layout */
    this->ResetAxisLayout();
}

/**
  * @brief Axis class Destructor Function
  */
Axis::~Axis(void)
{
    /** Release Memories */
    Global::Safe_Delete(m_pAxisLine);

    /** Release All List and Clear them */
    this->ClearElementsList(m_ltTickLines);
    this->ClearElementsList(m_ltGridLines);
    this->ClearElementsList(m_ltLabelRects);
    m_ltLabelTexts.clear();
}

/**
 * @brief ResetAxisScale
 */
void Axis::ResetAxisScale(void)
{
    /** */
    double dMaxMinDiff = m_dMaxValue - m_dMinValue;

    /** Calculate Scale Value */
    this->m_dScaleValue = dMaxMinDiff / m_pAxisLine->length();

    /** Calculate Count of Tick on Axis */
    int nTickCount = dMaxMinDiff / m_dTickValue + 1;

    /** Clear Old Tick Values and then Generate New */
    this->m_ltTickValues.clear();
    for (int i = 0; i < nTickCount; ++i)
    {
        this->m_ltTickValues.push_back(
                    this->m_dMinValue + this->m_dTickValue * i + 1);
    }
}

/**
 * @brief CreateAxisLayout
 */
void Axis::ResetAxisLayout(void)
{
    /** Release All List and Clear them */
    this->ClearElementsList(m_ltTickLines);
    this->ClearElementsList(m_ltGridLines);
    this->ClearElementsList(m_ltLabelRects);
    m_ltLabelTexts.clear();

    /** Declare Origin Point Coordinate */
    double dStartX = this->m_pAxisLine->p1().x();
    double dStartY = this->m_pAxisLine->p1().y();

    /** in Horizontal Axis Case */
    if (this->m_pAxisLine->p1().y() == this->m_pAxisLine->p2().y())
    {
        /** Vertical Coordinate of Elements */
        double dYPos = dStartY - Global::Tick_Value;

        /** Travalsal Tick Values List, then Generate Tick Lines/Grid Lines/Label Rectangles */
        for (QList<double>::iterator it = m_ltTickValues.begin();
             it != m_ltTickValues.end();
             it++)
        {
            /** Get New Horizontal Position on Axis Based on it Value */
            double dXPos = dStartX + this->GetPositionByValue(*it);

            /** Storage Tick Line into List */
            m_ltTickLines.push_back(new QLineF(dXPos, dStartY, dXPos, dYPos));

            /** Storage Grid Line into List */
            m_ltGridLines.push_back(new QLineF(dXPos, dStartY, dXPos, 0));

            /** Storage Label Rectangle into List */
            m_ltLabelRects.push_back(
                        new QRectF(dXPos - Global::Label_Width / 2,
                                   dStartY,
                                   Global::Label_Width,
                                   Global::Label_Height));

            /** Storage Label Text into List */
            m_ltLabelTexts.push_back(QString("%1").arg(*it));
        }
    }
    /** in Vertical Axis Case */
    else if (this->m_pAxisLine->p1().x() == this->m_pAxisLine->p2().x())
    {
        /** Horizontal Coordinate of Elements */
        double dXPos = dStartX + Global::Tick_Value;

        /** Travalsal Tick Values List, then Generate Tick Lines/Grid Lines/Label Rectangles */
        for (QList<double>::iterator it = m_ltTickValues.begin();
             it != m_ltTickValues.end();
             it++)
        {
            /** Get New Vertical Position on Axis Based on it Value */
            double dYPos = dStartY - this->GetPositionByValue(*it);

            /** Storage Tick Line into List */
            m_ltTickLines.push_back(new QLineF(dStartX, dYPos, dXPos, dYPos));

            /** Storage Grid Line into List */
            m_ltGridLines.push_back(new QLineF(dStartX, dYPos, 1280, dYPos));

            /** Storage Label Rectangle into List */
            m_ltLabelRects.push_back(
                        new QRectF(dStartX - Global::Label_Width / 2,
                                   dYPos - Global::Label_Height / 2,
                                   Global::Label_Width / 2,
                                   Global::Label_Height));

            /** Storage Label Text into List */
            m_ltLabelTexts.push_back(QString("%1").arg(*it));
        }
    }
}

/**
 * @brief UpdateAxisLayout
 * @param pAxisLine
 */
void Axis::UpdateAxisLayout(QLineF* pAxisLine)
{
    /** Set Axis Line */
    this->setAxisLine(pAxisLine);

    /** Reset Axis Scale System */
    this->ResetAxisScale();

    /** Reset Axis Layout */
    this->ResetAxisLayout();
}

/**
 * @brief UpdateAxisScale
 * @param dMaxValue : double : Maximum Value of Axis
 * @param dMinValue : double : Minimum Value of Axis
 * @param dTickValue : double : Every Single Tick Value of Axis
 */
void Axis::UpdateAxisScale(double dMaxValue, double dMinValue, double dTickValue)
{
    /** Set Axis Values */
    this->setMaxValue(dMaxValue);
    this->setMinValue(dMinValue);
    this->setTickValue(dTickValue);

    /** Reset Axis Scale System */
    this->ResetAxisScale();

    /** Reset Axis Layout */
    this->ResetAxisLayout();
}

/**
 * @brief GetPositionByValue
 * @param dValue : double : Horizontal or Vertical Data Value
 * @return double : Horizontal or Vertical Coordinate on Axis
 */
double Axis::GetPositionByValue(double dValue)
{
    /** in Scale Value Equals 0 Case, Return 0.0f */
    if (this->m_dScaleValue == 0.0f)
    {
        return 0.0f;
    }
    /** in Other Case, Return Offset Coordinate from Start Point */
    else
    {
        return ((dValue - m_dMinValue) / m_dScaleValue);
    }
}

/**
 * @brief getAxisLine
 * @return QLineF* : Line of Axis
 */
QLineF* Axis::getAxisLine(void) const
{
    return m_pAxisLine;
}

/**
 * @brief getTickLines
 * @return QList<QLineF*> : List of Tick Lines
 */
QList<QLineF*> Axis::getTickLines(void) const
{
    return m_ltTickLines;
}

/**
 * @brief getGridLines
 * @return QList<QLineF*> : List of Grid Lines
 */
QList<QLineF*> Axis::getGridLines(void) const
{
    return m_ltGridLines;
}

/**
 * @brief getLabelRects
 * @return QList<QRectF*> : Label Rectangle of Ticks
 */
QList<QRectF *> Axis::getLabelRects(void) const
{
    return m_ltLabelRects;
}

/**
 * @brief getLabelTexts
 * @return QList<QString> : Label Text of Ticks
 */
QList<QString> Axis::getLabelTexts(void) const
{
    return m_ltLabelTexts;
}

/**
 * @brief getTickValues
 * @return QList<double> : Tick Value List of Axis
 */
QList<double> Axis::getTickValues(void) const
{
    return m_ltTickValues;
}

/**
 * @brief getMaxValue
 * @return double : Maximum Value of Axis
 */
double Axis::getMaxValue(void) const
{
    return m_dMaxValue;
}

/**
 * @brief getMinValue
 * @return double : Minimum Value of Axis
 */
double Axis::getMinValue(void) const
{
    return m_dMinValue;
}

/**
 * @brief getTickValue
 * @return double : Every Single Tick Value of Axis
 */
double Axis::getTickValue(void) const
{
    return m_dTickValue;
}

/**
 * @brief getScaleValue
 * @return double : Scale Value of Axis
 */
double Axis::getScaleValue(void) const
{
    return m_dScaleValue;
}

/**
 * @brief setAxisLine
 * @param pAxisLine : QLineF* : Line of Axis
 */
void Axis::setAxisLine(QLineF* pAxisLine)
{
    /** Release Memory */
    Global::Safe_Delete(m_pAxisLine);

    /** Set New Value of Axis Line */
    m_pAxisLine = pAxisLine;
}

/**
 * @brief setMaxValue
 * @param dMaxValue : double : Maximum Value of Axis
 */
void Axis::setMaxValue(double dMaxValue)
{
    m_dMaxValue = dMaxValue;
}

/**
 * @brief setMinValue
 * @param dMinValue : double : Minimum Value of Axis
 */
void Axis::setMinValue(double dMinValue)
{
    m_dMinValue = dMinValue;
}

/**
 * @brief setTickValue
 * @param dTickValue : double : Every Single Tick Value of Axis
 */
void Axis::setTickValue(double dTickValue)
{
    m_dTickValue = dTickValue;
}

/**
 * @brief setScaleValue
 * @param dScaleValue : double : Scale Value of Axis
 */
void Axis::setScaleValue(double dScaleValue)
{
    m_dScaleValue = dScaleValue;
}

/**
 * @brief ClearElementsList
 * @param ltElements : QList<Template*> : Template Type Elements List
 */
template<typename T>
void Axis::ClearElementsList(QList<T*> &ltElements)
{
    /** Travelsal Elements List */
    for (typename QList<T*>::iterator it = ltElements.begin();
         it != ltElements.end();
         it++)
    {
        /** Safe Delete Element */
        T* pPointer = *it;
        Global::Safe_Delete(pPointer);

        /** Erase Element from Elements List */
        ltElements.erase(it);
    }

    /** */
    ltElements.clear();
}

