#ifndef AXIS_H
#define AXIS_H

#include <QLineF>
#include <QRectF>
#include <QList>
#include <qmath.h>
#include <QDebug>
#include "Global.h"

/**
 * @brief The Axis class
 */
class Axis
{
public:
    /**
     * @brief Axis class Constructor Function
     * @param pAxisLine : QLineF* : Line of Axis
     * @param dMaxValue : double : Maximum Value of Axis
     * @param dMinValue : double : Minimum Value of Axis
     * @param dTickValue : double : Every Single Tick Value of Axis
     */
    Axis(QLineF* pAxisLine,
         double dMaxValue,
         double dMinValue,
         double dTickValue);

    /**
      * @brief Axis class Destructor Function
      */
    ~Axis(void);

public:
    /**
     * @brief ResetAxisScale
     */
    void ResetAxisScale(void);

    /**
     * @brief CreateAxisLayout
     */
    void ResetAxisLayout(void);

    /**
     * @brief UpdateAxisLayout
     * @param pAxisLine
     */
    void UpdateAxisLayout(QLineF* pAxisLine);

    /**
     * @brief UpdateAxisScale
     * @param dMaxValue : double : Maximum Value of Axis
     * @param dMinValue : double : Minimum Value of Axis
     * @param dTickValue : double : Every Single Tick Value of Axis
     */
    void UpdateAxisScale(double dMaxValue, double dMinValue, double dTickValue);

    /**
     * @brief GetPositionByValue
     * @param dValue : double : Horizontal or Vertical Data Value
     * @return double : Horizontal or Vertical Coordinate on Axis
     */
    double GetPositionByValue(double dValue);

public:
    /**
     * @brief getAxisLine
     * @return QLineF* : Line of Axis
     */
    QLineF* getAxisLine(void) const;

    /**
     * @brief getTickLines
     * @return QList<QLineF*> : List of Tick Lines
     */
    QList<QLineF*> getTickLines(void) const;

    /**
     * @brief getGridLines
     * @return QList<QLineF*> : List of Grid Lines
     */
    QList<QLineF *> getGridLines(void) const;

    /**
     * @brief getLabelRects
     * @return QList<QRectF*> : Label Rectangle of Ticks
     */
    QList<QRectF*> getLabelRects(void) const;

    /**
     * @brief getLabelTexts
     * @return QList<QString> : Label Text of Ticks
     */
    QList<QString> getLabelTexts(void) const;

    /**
     * @brief getTickValues
     * @return QList<double> : Tick Value List of Axis
     */
    QList<double> getTickValues(void) const;

    /**
     * @brief getMaxValue
     * @return double : Maximum Value of Axis
     */
    double getMaxValue(void) const;

    /**
     * @brief getMinValue
     * @return double : Minimum Value of Axis
     */
    double getMinValue(void) const;

    /**
     * @brief getTickValue
     * @return double : Every Single Tick Value of Axis
     */
    double getTickValue(void) const;

    /**
     * @brief getScaleValue
     * @return double : Scale Value of Axis
     */
    double getScaleValue(void) const;

    /**
     * @brief setAxisLine
     * @param pAxisLine : QLineF* : Line of Axis
     */
    void setAxisLine(QLineF* pAxisLine);

    /**
     * @brief setMaxValue
     * @param dMaxValue : double : Maximum Value of Axis
     */
    void setMaxValue(double dMaxValue);

    /**
     * @brief setMinValue
     * @param dMinValue : double : Minimum Value of Axis
     */
    void setMinValue(double dMinValue);

    /**
     * @brief setTickValue
     * @param dTickValue : double : Every Single Tick Value of Axis
     */
    void setTickValue(double dTickValue);

    /**
     * @brief setScaleValue
     * @param dScaleValue : double : Scale Value of Axis
     */
    void setScaleValue(double dScaleValue);

private:
    /**
     * @brief ClearElementsList
     * @param ltElements : QList<Template*> : Template Type Elements List
     */
    template<typename T>
    void ClearElementsList(QList<T*> &ltElements);

private:
    /**
     * @brief m_pAxisLine : QLineF* : Line of Axis
     */
    QLineF* m_pAxisLine;

    /**
     * @brief m_ltTickLines : QList<QLineF*> : List of Tick Lines
     */
    QList<QLineF*> m_ltTickLines;

    /**
     * @brief m_ltGridLines : QList<QLineF*> : List of Grid Lines
     */
    QList<QLineF*> m_ltGridLines;

    /**
     * @brief m_ltLabelRects : QList<QRectF*> : Label Rectangle of Ticks
     */
    QList<QRectF*> m_ltLabelRects;

    /**
     * @brief m_ltLabelTexts
     */
    QList<QString> m_ltLabelTexts;

    /**
     * @brief m_dMaxValue : double : Maximum Value of Axis
     */
    double m_dMaxValue;

    /**
     * @brief m_dMinValue : double : Minimum Value of Axis
     */
    double m_dMinValue;

    /**
     * @brief m_dTickValue : double : Every Single Tick Value of Axis
     */
    double m_dTickValue;

    /**
     * @brief m_dScaleValue : double : Scale Value of Axis
     */
    double m_dScaleValue;

    /**
     * @brief m_ltTickValues : QList<double> : Tick Value List of Axis
     */
    QList<double> m_ltTickValues;


public:
    int m_nCatchVal;
};

#endif // AXIS_H
