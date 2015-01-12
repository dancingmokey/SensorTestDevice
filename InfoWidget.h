#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QPainter>

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = 0);


public:
    void CalcBoundRect();

private:
    void paintEvent(QPaintEvent* e);

signals:

public slots:


public:
    QRectF m_rtCH1Title;
    QRectF m_rtCH1Status;
    QRectF m_rtCH1Value;
    QRectF m_rtCH1Order;
    QFont m_ftCH1Title;
    QColor m_crCH1Title;
    QFont m_ftCH1Other;
    QColor m_crCH1Other;
    QString m_strCH1Title;
    QString m_strCH1Status;
    QString m_strCH1Value;
    QString m_strCH1Order;

    QRectF m_rtCH2Title;
    QRectF m_rtCH2Status;
    QRectF m_rtCH2Value;
    QRectF m_rtCH2Order;
    QFont m_ftCH2Title;
    QColor m_crCH2Title;
    QFont m_ftCH2Other;
    QColor m_crCH2Other;
    QString m_strCH2Title;
    QString m_strCH2Status;
    QString m_strCH2Value;
    QString m_strCH2Order;

};

#endif // INFOWIDGET_H
