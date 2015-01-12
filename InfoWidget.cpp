#include "InfoWidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent)
{
    /** CH1 Other Parameters */
    m_ftCH1Title = QFont(QString("wenquanyi"), 15, 2, false);
    m_crCH1Title = Qt::red;
    m_ftCH1Other = QFont(QString("wenquanyi"), 10, 2, false);
    m_crCH1Other = Qt::white;
    m_strCH1Title = QString("通道1");
    m_strCH1Status = QString("无峰值");
    m_strCH1Value = QString("");
    m_strCH1Order = QString("");

    /** CH2 Other Parameters */
    m_ftCH2Title = QFont(QString("wenquanyi"), 15, 2, false);
    m_crCH2Title = Qt::blue;
    m_ftCH2Other = QFont(QString("wenquanyi"), 10, 2, false);
    m_crCH2Other = Qt::white;
    m_strCH2Title = QString("通道2");
    m_strCH2Status = QString("无峰值");
    m_strCH2Value = QString("");
    m_strCH2Order = QString("");
}

void InfoWidget::CalcBoundRect()
{
    /** Calculate Parameters */
    QRectF rtWidget = this->rect();

    /** CH1 Bound Rectangle Parameters */
    m_rtCH1Title = QRectF(rtWidget.left(),
                          rtWidget.top(),
                          rtWidget.width() / 4,
                          rtWidget.height() / 2);
    m_rtCH1Status = QRectF(rtWidget.left() + rtWidget.width() / 4,
                          rtWidget.top(),
                          rtWidget.width() / 4,
                          rtWidget.height() / 2);
    m_rtCH1Value = QRectF(rtWidget.left() + rtWidget.width() / 2,
                          rtWidget.top(),
                          rtWidget.width() / 4,
                          rtWidget.height() / 2);
    m_rtCH1Order = QRectF(rtWidget.right() - rtWidget.width() / 4,
                          rtWidget.top(),
                          rtWidget.width() / 4,
                          rtWidget.height() / 2);

    /** CH2 Bound Rectangle Parameters */
    m_rtCH2Title = QRectF(rtWidget.left(),
                          rtWidget.top() + rtWidget.height() / 2,
                          rtWidget.width() / 4,
                          rtWidget.height() / 2);
    m_rtCH2Status = QRectF(rtWidget.left() + rtWidget.width() / 4,
                          rtWidget.top() + rtWidget.height() / 2,
                          rtWidget.width() / 4,
                          rtWidget.height() / 2);
    m_rtCH2Value = QRectF(rtWidget.left() + rtWidget.width() / 2,
                          rtWidget.top() + rtWidget.height() / 2,
                          rtWidget.width() / 4,
                          rtWidget.height() / 2);
    m_rtCH2Order = QRectF(rtWidget.right() - rtWidget.width() / 4,
                          rtWidget.top() + rtWidget.height() / 2,
                          rtWidget.width() / 4,
                          rtWidget.height() / 2);
}

void InfoWidget::paintEvent(QPaintEvent *e)
{
    /** */
    this->CalcBoundRect();

    /** Declare Painter and then Set Pen & Font */
    QPainter vPainter(this);

    /** Draw Channel 1 Title */
    vPainter.setFont(m_ftCH1Title);
    vPainter.setPen(QPen(m_crCH1Title, 2, Qt::SolidLine, Qt::FlatCap));
    vPainter.drawText(m_rtCH1Title,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      m_strCH1Title);

    /** Draw Channel 1 Other */
    vPainter.setFont(m_ftCH1Other);
    vPainter.setPen(QPen(m_crCH1Other, 2, Qt::SolidLine, Qt::FlatCap));
    vPainter.drawText(m_rtCH1Status,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      m_strCH1Status);
    vPainter.drawText(m_rtCH1Value,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      m_strCH1Value);
    vPainter.drawText(m_rtCH1Order,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      m_strCH1Order);

    /** Draw Channel 2 Title */
    vPainter.setFont(m_ftCH2Title);
    vPainter.setPen(QPen(m_crCH2Title, 2, Qt::SolidLine, Qt::FlatCap));
    vPainter.drawText(m_rtCH2Title,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      m_strCH2Title);

    /** Draw Channel 2 Other */
    vPainter.setFont(m_ftCH2Other);
    vPainter.setPen(QPen(m_crCH2Other, 2, Qt::SolidLine, Qt::FlatCap));
    vPainter.drawText(m_rtCH2Status,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      m_strCH2Status);
    vPainter.drawText(m_rtCH2Value,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      m_strCH2Value);
    vPainter.drawText(m_rtCH2Order,
                      Qt::AlignHCenter | Qt::AlignVCenter,
                      m_strCH2Order);
}
