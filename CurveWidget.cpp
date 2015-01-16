#include "CurveWidget.h"

/**
 * @brief CurveWidget::CurveWidget
 * @param parent
 */
CurveWidget::CurveWidget(QWidget *parent) :
    QWidget(parent)
{
    m_pCurve = new Curve(new QRectF(this->rect().left(),
                                    this->rect().top(),
                                    this->rect().width(),
                                    this->rect().height()));
    m_nZoomValue = 1000;
}

/**
 * @brief resizeEvent
 * @param QResizeEvent* : Resize Event
 */
void CurveWidget::resizeEvent(QResizeEvent *e)
{
    m_pCurve->UpdateCurve(new QRectF(this->rect().left(),
                                     this->rect().top(),
                                     this->rect().width(),
                                     this->rect().height()));
}

/**
 * @brief paintEvent
 * @param QPaintEvent* : Resize Event
 */
void CurveWidget::paintEvent(QPaintEvent *e)
{
    /** Draw Ticks of X&Y Axis */
    this->DrawGrids();

    /** Draw Ticks of X&Y Axis */
    this->DrawTicks();

    /** Draw Child Ticks of X&Y Axis */
    this->DrawChildTicks();

    /** Draw X&Y Axis */
    this->DrawAxes();

    /** Draw Labels of X&Y Axis */
    this->DrawLabels();

    /** Draw Serials */
    this->DrawSerials();

    /** */
    if (m_bIsDrawMousePos == true)
    {
        this->DrawMousePosLine();
    }
}

/**
 * @brief Draw Axes
 */
void CurveWidget::DrawAxes(void)
{
    /** Declare Painter and then Set Pen */
    QPainter vPainter(this);
    vPainter.setPen(QPen(Qt::green, 3, Qt::SolidLine, Qt::FlatCap));

    /** Draw X Axis */
    QLineF* pXAxisLine = m_pCurve->getXAxis()->getAxisLine();
    vPainter.drawLine(*pXAxisLine);

    /** Draw Y Axis */
    QLineF* pYAxisLine = m_pCurve->getYAxis()->getAxisLine();
    vPainter.drawLine(*pYAxisLine);
}

/**
 * @brief Draw Ticks
 */
void CurveWidget::DrawTicks(void)
{
    /** Declare Painter and then Set Pen */
    QPainter vPainter(this);
    vPainter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::FlatCap));

    /** Draw X Axis Ticks */
    QList<QLineF*> ltXAxisTicks = m_pCurve->getXAxis()->getTickLines();
    for (QList<QLineF*>::iterator it =  ltXAxisTicks.begin();
         it != ltXAxisTicks.end();
         it++)
    {
        /** Draw Line */
        QLineF* pLine = *it;
        vPainter.drawLine(*pLine);
    }

    /** Draw Y Axis Ticks */
    QList<QLineF*> ltYAxisTicks = m_pCurve->getYAxis()->getTickLines();
    for (QList<QLineF*>::iterator it =  ltYAxisTicks.begin();
         it != ltYAxisTicks.end();
         it++)
    {
        /** Draw Line */
        QLineF* pLine = *it;
        vPainter.drawLine(*pLine);
    }
}

/**
 * @brief Draw Child Ticks
 */
void CurveWidget::DrawChildTicks(void)
{
    /** Declare Painter and then Set Pen */
    QPainter vPainter(this);
    vPainter.setPen(QPen(Qt::gray, 1, Qt::SolidLine, Qt::FlatCap));

    /** Draw X Axis Ticks */
    QList<QLineF*> ltChildXAxisTicks = m_pCurve->getXAxis()->getChildTickLines();
    for (QList<QLineF*>::iterator it =  ltChildXAxisTicks.begin();
         it != ltChildXAxisTicks.end();
         it++)
    {
        /** Draw Line */
        QLineF* pLine = *it;
        vPainter.drawLine(*pLine);
    }

    /** Draw Y Axis Ticks */
    QList<QLineF*> ltChildYAxisTicks = m_pCurve->getYAxis()->getChildTickLines();
    for (QList<QLineF*>::iterator it =  ltChildYAxisTicks.begin();
         it != ltChildYAxisTicks.end();
         it++)
    {
        /** Draw Line */
        QLineF* pLine = *it;
        vPainter.drawLine(*pLine);
    }
}

/**
 * @brief Draw Grids
 */
void CurveWidget::DrawGrids(void)
{
    /** Declare Painter and then Set Pen */
    QPainter vPainter(this);
    vPainter.setPen(QPen(Qt::gray, 1, Qt::DotLine, Qt::FlatCap));

    /** Draw X Axis Grids */
    QList<QLineF*> ltXAxisGrids = m_pCurve->getXAxis()->getGridLines();
    for (QList<QLineF*>::iterator it =  ltXAxisGrids.begin();
         it != ltXAxisGrids.end();
         it++)
    {
        /** Draw Line */
        QLineF* pLine = *it;
        vPainter.drawLine(*pLine);
    }

    /** Draw X Axis Grids */
    QList<QLineF*> ltYAxisGrids = m_pCurve->getYAxis()->getGridLines();
    for (QList<QLineF*>::iterator it =  ltYAxisGrids.begin();
         it != ltYAxisGrids.end();
         it++)
    {
        /** Draw Line */
        QLineF* pLine = *it;
        vPainter.drawLine(*pLine);
    }
}

/**
 * @brief Draw Labels
 */
void CurveWidget::DrawLabels(void)
{
    /** Declare Painter and then Set Pen & Font */
    QPainter vPainter(this);
    vPainter.setFont(QFont(QString(""), 10, 2, false));
    vPainter.setPen(QPen(Qt::gray, 1, Qt::DotLine, Qt::FlatCap));

    /** Draw X Axis Grids */
    QList<QRectF*> ltXLabelRects = m_pCurve->getXAxis()->getLabelRects();
    QList<QString> ltXLabelTexts = m_pCurve->getXAxis()->getLabelTexts();
    for (int i = 0; i < ltXLabelRects.size(); ++i)
    {
        /** Draw Labels */
        QRectF* pRect = ltXLabelRects.at(i);

//        /** Draw Global Label Text */
//        if(i < g_ltXLabelTexts.size())
//        {
//            QString strText = g_ltXLabelTexts.at(i);
//            vPainter.drawText(*pRect, Qt::AlignHCenter | Qt::AlignVCenter, strText);
//        }

        /** Draw Original Label Text */
        QString strText = ltXLabelTexts.at(i);
        vPainter.drawText(*pRect, Qt::AlignHCenter | Qt::AlignVCenter, strText);
    }

    /** Draw Y Axis Labels */
    QList<QRectF*> ltYLabelRects = m_pCurve->getYAxis()->getLabelRects();
    QList<QString> ltYLabelTexts = m_pCurve->getYAxis()->getLabelTexts();
    for (int i = 0; i < ltYLabelRects.size(); ++i)
    {
        /** Draw Label */
        QRectF* pRect = ltYLabelRects.at(i);
        QString strText = ltYLabelTexts.at(i);
        vPainter.drawText(*pRect, Qt::AlignRight | Qt::AlignVCenter, strText);
    }
}

/**
 * @brief Draw Serials
 */
void CurveWidget::DrawSerials(void)
{
    /** Travesal Every Serial in Curve */
    QMap<QString, Serial*> mpSerial = m_pCurve->getSerialsMap();
    for (QMap<QString, Serial*>::iterator it = mpSerial.begin();
         it != mpSerial.end();
         it++)
    {
        /**Get Serial Name & Color */
        QString strSerialName = it.key();
        QColor crSerial = ((strSerialName == Global::CH1_Serial_Name) ? Global::CH1_Serial_Color : Global::CH2_Serial_Color);

        /** Declare Painter and then Set Pen with Diffrent Style for Every Single Serials */
        QPainter vPainter(this);
        vPainter.setPen(QPen(crSerial, 3, Qt::SolidLine, Qt::FlatCap));

        /** Get Variabls about Horizontal Axis */
        Axis* pXAxis = m_pCurve->getXAxis();
        QLineF* pXAxisLine = pXAxis->getAxisLine();
        double dXAxisStartPos = pXAxisLine->p1().x();

        /** Get Variabls about Vertical Axis */
        Axis* pYAxis = m_pCurve->getYAxis();
        QLineF* pYAxisLine = pYAxis->getAxisLine();
        double dYAxisStartPos = pYAxisLine->p1().y();

        /** Travelsal Serial Data and Draw Serial */
        QList<SerialData*> ltSerialDatas = m_pCurve->getSerialDataByName(strSerialName);
        for (int i = 0; i < ltSerialDatas.size() - 1; ++i)
        {
            /** Get Current and Next Data */
            SerialData* pCurrData = ltSerialDatas.at(i);
            SerialData* pNextData = ltSerialDatas.at(i + 1);

            /** Calculate Start Point */
            QPointF vStartPoint(dXAxisStartPos +
                                pXAxis->GetPositionByValue(pCurrData->m_dX),
                                dYAxisStartPos -
                                pYAxis->GetPositionByValue(pCurrData->m_dY));

            /** Calculate End Point */
            QPointF vEndPoint(dXAxisStartPos +
                              pXAxis->GetPositionByValue(pNextData->m_dX),
                              dYAxisStartPos -
                              pYAxis->GetPositionByValue(pNextData->m_dY));

            /** Only Draw the Points in Axis Bound */
            if (vStartPoint.x() >= dXAxisStartPos)
            {
                /** Draw Line */
                 vPainter.drawLine(vStartPoint, vEndPoint);
            }
        }
    }
}

/**
 * @brief Draw Mouse Position Line
 */
void CurveWidget::DrawMousePosLine(void)
{
    /** */
    if (m_ptMousePos.x() < this->getCurve()->getYAxis()->getAxisLine()->p1().x())
    {
        return;
    }

    /** Declare Painter and then Set Pen */
    QPainter vPainter(this);
    vPainter.setPen(QPen(Qt::cyan, 3, Qt::DotLine, Qt::FlatCap));

    /** */
    QLine lnVertical = QLine(QPoint(m_ptMousePos.x(), this->rect().top()),
                             QPoint(m_ptMousePos.x(), this->getCurve()->getXAxis()->getAxisLine()->p1().y()));

    /** */
    vPainter.drawLine(lnVertical);
}

/**
 * @brief UpdateCurve
 * @param strSerialName : QString : Serial Name
 */
void CurveWidget::UpdateCurve(uint8 nSerialOpt, QString strSerialName)
{
    if (nSerialOpt == Global::Serial_Add)
    {
        this->m_pCurve->AddSerial(strSerialName);
    }
    else if (nSerialOpt == Global::Serial_del)
    {
        this->m_pCurve->RemoveSerial(strSerialName);
    }
}

/**
 * @brief UpdateCurve
 * @param pBoundRect : QRectF* : Bound Rectangle
 */
void CurveWidget::UpdateCurve(QRectF* pBoundRect)
{
    m_pCurve->UpdateCurve(pBoundRect);
}

/**
 * @brief UpdateCurve
 * @param nAxisType : uint8 : Axis is Horizontal or Vertical
 * @param dMaxValue : double : Maximum Value of Axis
 * @param dMinValue : double : Minimum Value of Axis
 * @param dTickValue : double : single Tick Value of Axis
 */
void CurveWidget::UpdateCurve(uint8 nAxisType,
                              double dMaxValue,
                              double dMinValue,
                              double dTickValue)
{
    this->m_pCurve->UpdateCurve(nAxisType, dMaxValue, dMinValue, dTickValue);
}

/**
 * @brief UpdateCurve
 * @param strSerialName : QString : Serial Name
 * @param dXValueList : double* : Array of Horizontal Values
 * @param dYValueList : double* : Array of Vertical Values
 * @param nCount : int : Count of Array
 */
void CurveWidget::UpdateCurve(QString strSerialName,
                              double* dXValueList,
                              double* dYValueList,
                              int nCount)
{
    this->m_pCurve->AddSerialData(strSerialName, dXValueList, dYValueList, nCount);
}

/**
 * @brief CurveWidget::ZoomInOperation
 */
void CurveWidget::ZoomInOperation(void)
{
    /** Check Zoom Value */
    m_nZoomValue -= 200;
    if (m_nZoomValue <= 200)
    {
        return;
    }

    /** Calculate New Maximum & Minimum Values */
    Axis* pCurveXAxis = this->getCurve()->getXAxis();
    double dNewMaxValue = m_dMousePosVal + m_nZoomValue;
    dNewMaxValue = (dNewMaxValue > pCurveXAxis->getMaxValue()) ? pCurveXAxis->getMaxValue() : dNewMaxValue;
    double dNewMinValue = m_dMousePosVal - m_nZoomValue;
    dNewMinValue = (dNewMinValue < pCurveXAxis->getMinValue()) ? pCurveXAxis->getMinValue() : dNewMinValue;

    /** Update Stack */
    m_ltMaxZoomVals.push_back(dNewMaxValue);
    m_ltMinZoomVals.push_back(dNewMinValue);

#ifdef _DEBUG_OUTPUT
    /** Debug Output */
    qDebug() << "Zoom In Operation Update Axis Scale, Now Zoom Value is "
             << m_nZoomValue
             << " Mouse Position Value is "
             << m_dMousePosVal
             << " Max Value Changed from "
             << pCurveXAxis->getMaxValue()
             << " to "
             << dNewMaxValue
             << " Min Value Changed from "
             << pCurveXAxis->getMinValue()
             << " to "
             << dNewMinValue;
#endif

    /** Set X Axis Manimum & Minimum Value and then Update Scale */
    pCurveXAxis->UpdateAxisScale(dNewMaxValue, dNewMinValue, Global::Axis_Hor_TickVal);
    m_ptMousePos.setX(pCurveXAxis->GetValueByPosition(m_dMousePosVal));

    /** Repaint Curve Widget */
    this->repaint();
}

/**
 * @brief CurveWidget::ZoomOutOperation
 */
void CurveWidget::ZoomOutOperation(void)
{
    /** Check Zoom Value */
    m_nZoomValue += 200;
    if (m_nZoomValue >= 1000)
    {
        m_nZoomValue = 1000;
    }

    /** Recover to the Last Change */
    double dNewMaxValue = m_ltMaxZoomVals.last();
    double dNewMinValue = m_ltMinZoomVals.last();

    /** Remain thr Original Max&Min Values */
    if ((m_ltMaxZoomVals.size() > 1) &&
        (m_ltMinZoomVals.size() > 1))
    {
        m_ltMaxZoomVals.pop_back();
        m_ltMinZoomVals.pop_back();
    }

#ifdef _DEBUG_OUTPUT
    /** Debug Output */
    qDebug() << "Zoom Out Operation Update Axis Scale, Max Value Changed from "
             << m_pCurve->getXAxis()->getMaxValue()
             << " to "
             << dNewMaxValue
             << " Min Value Changed from "
             << m_pCurve->getXAxis()->getMinValue()
             << " to "
             << dNewMinValue;
#endif

    /** Set X Axis Manimum & Minimum Value and then Update Scale */
    Axis* pCurveXAxis = m_pCurve->getXAxis();
    pCurveXAxis->UpdateAxisScale(dNewMaxValue, dNewMinValue, Global::Axis_Hor_TickVal);
    m_ptMousePos.setX(pCurveXAxis->GetValueByPosition(m_dMousePosVal));

    /** Repaint Curve Widget */
    this->repaint();
}

/**
 * @brief getCurve
 * @return Curve* : Curve Pointer
 */
Curve* CurveWidget::getCurve() const
{
    return m_pCurve;
}

/**
 * @brief getMousPos
 */
QPoint CurveWidget::getMousePos(void)
{
    return m_ptMousePos;
}

/**
 * @brief setMousPos
 * @param ptMousePos
 */
void CurveWidget::setMousePos(QPoint ptMousePos)
{
    /** Set Mouse Position */
    m_ptMousePos = ptMousePos;

    /** Get Variables */
    Axis* pCurveXAxis = this->getCurve()->getXAxis();
    int nMousePosX = m_ptMousePos.x() - pCurveXAxis->getAxisLine()->p1().x();
    m_dMousePosVal = pCurveXAxis->GetValueByPosition((double)nMousePosX);
}

/**
 * @brief setIsDrawMousePos
 * @param bIsDrawMousePos
 */
void CurveWidget::setIsDrawMousePos(bool bIsDrawMousePos)
{
    m_bIsDrawMousePos = bIsDrawMousePos;

    if (m_bIsDrawMousePos == true)
    {
        Axis* pXAxis = m_pCurve->getXAxis();
        m_ltMaxZoomVals.push_back(pXAxis->getMaxValue());
        m_ltMinZoomVals.push_back(pXAxis->getMinValue());
    }
    else
    {
        /** Set X Axis Manimum & Minimum Value and then Update Scale */
        m_pCurve->getXAxis()->UpdateAxisScale(m_ltMaxZoomVals.first(), m_ltMinZoomVals.first(), Global::Axis_Hor_TickVal);
        m_ltMaxZoomVals.clear();
        m_ltMinZoomVals.clear();
        m_nZoomValue = 1000;
    }
}














