#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = 0);


private:
    void paintEvent(QPaintEvent* e);

signals:

public slots:


public:
    QString m_strTitle;
    QString m_strOrder;
    QString m_strAmplitute;

};

#endif // INFOWIDGET_H
