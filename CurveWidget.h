#ifndef CURVEWIDGET_H
#define CURVEWIDGET_H

#include <QWidget>
#include <QLineF>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
#include <QTime>
#include <QDebug>
#include <QDragMoveEvent>

#include "Curve.h"

class ZoomParams
{
public:
    ZoomParams(double dMaxValue, double dMinValue, int nZoomValue)
    {
        m_dMaxValue = dMaxValue;
        m_dMinValue = dMinValue;
        m_nZoomValue = nZoomValue;
    }

public:
    double m_dMaxValue;
    double m_dMinValue;
    int m_nZoomValue;
};

/**
 * @brief The CurveWidget class
 */
class CurveWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief CurveWidget
     * @param parent : Parent Widget
     */
    explicit CurveWidget(QWidget *parent = 0);

private:
    /**
     * @brief paintEvent
     * @param QPaintEvent* : Resize Event
     */
    void paintEvent(QPaintEvent* e);

    /**
     * @brief resizeEvent
     * @param QResizeEvent* : Resize Event
     */
    void resizeEvent(QResizeEvent* e);

private:
    /**
     * @brief Draw Axes
     */
    void DrawAxes(void);

    /**
     * @brief Draw Ticks
     */
    void DrawTicks(void);

    /**
     * @brief Draw Child Ticks
     */
    void DrawChildTicks(void);

    /**
     * @brief Draw Grids
     */
    void DrawGrids(void);

    /**
     * @brief Draw Serials
     */
    void DrawSerials(void);

    /**
     * @brief Draw Labels
     */
    void DrawLabels(void);

    /**
     * @brief DrawMousePosLine
     */
    void DrawWidgetName(void);

    /**
     * @brief DrawMousePosLine
     */
    void DrawMousePosLine(void);

public:
    /**
     * @brief UpdateCurve
     * @param nSerialOpt : uint8 : Serial Operate Type
     * @param strSerialName : QString : Serial Name
     */
    void UpdateCurve(uint8 nSerialOpt, QString strSerialName);

    /**
     * @brief UpdateCurve
     * @param pBoundRect : QRectF* : Bound Rectangle
     */
    void UpdateCurve(QRectF* pBoundRect);

    /**
     * @brief UpdateCurve
     * @param nAxisType : uint8 : Axis is Horizontal or Vertical
     * @param dMaxValue : double : Maximum Value of Axis
     * @param dMinValue : double : Minimum Value of Axis
     * @param dTickValue : double : single Tick Value of Axis
     */
    void UpdateCurve(uint8 nAxisType,
                     double dMaxValue,
                     double dMinValue,
                     double dTickValue);

    /**
     * @brief UpdateCurve
     * @param strSerialName : QString : Serial Name
     * @param dXValueList : double* : Array of Horizontal Values
     * @param dYValueList : double* : Array of Vertical Values
     * @param nCount : int : Count of Array
     */
    void UpdateCurve(QString strSerialName,
                     double* dXValueList,
                     double* dYValueList,
                     int nCount);

    /**
     * @brief ZoomInOperation
     */
    void ZoomInOperation(void);

    /**
     * @brief ZoomOutOperation
     */
    void ZoomOutOperation(void);

    /**
     * @brief ResetZoomParams
     */
    void ResetZoomParams(void);

public:
    /**
     * @brief getCurve
     * @return Curve* : Curve Pointer
     */
    Curve* getCurve(void) const;

    /**
     * @brief getMousPos
     */
    QPoint getMousePos(void);

    /**
     * @brief setMousPos
     * @param ptMousePos
     */
    void setMousePos(QPoint ptMousePos);

    /**
     * @brief setIsDrawMousePos
     * @param bIsDrawMousePos
     */
    void setIsDrawMousePos(bool bIsDrawMousePos);

    /**
     * @brief setZoomParams
     * @param bIsAutoCatch
     * @param dMaxValue
     * @param dMinValue
     */
    void setZoomParams(bool bIsAutoCatch, double dMaxValue, double dMinValue);

private:
    /**
     * @brief m_pCurve
     */
    Curve* m_pCurve;

    /**
     * @brief m_ptMousePos
     */
    QPoint m_ptMousePos;

    /**
     * @brief m_dMousePosVal
     */
    double m_dMousePosVal;

    /**
     * @brief m_bIsDrawMousePos
     */
    bool m_bIsDrawMousePos;

    /**
     * @brief m_nZoomValue
     */
    int m_nZoomValue;

    /**
     * @brief m_ltZoomParams
     */
    QList<ZoomParams*> m_ltZoomParams;
};

#endif // CURVEWIDGET_H
