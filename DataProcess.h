#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <QThread>
#include <QDebug>
#include <QTime>
#include "Global.h"
#include "Curve.h"

/**
 * @brief The DataProcess class
 */
class DataProcess : public QThread
{
    Q_OBJECT
public:
    /**
     * @brief DataProcess Custom Destructor Function`
     * @param strProcName : QString : Data Processor Name
     * @param pCurve1 : Curve* : Curve 1
     * @param pCurve2 : Curve* : Curve 2
     * @param parent
     */
    explicit DataProcess(QString strProcName, Curve* pCurve1, Curve* pCurve2, QObject *parent = 0);

public:
    /**
     * @brief run
     * @desc Thread Work Function, Process Data from List
     */
    virtual void run(void);

public:
    /**
     * @brief getProcName
     * @return QString
     */
    QString getProcName(void) const;

    /**
     * @brief getEnableRunning
     * @return bool
     */
    bool getEnableRunning(void) const;

    /**
     * @brief setProcName
     * @param strProcName : QString : Data Processor Name
     */
    void setProcName(const QString &strProcName);

    /**
     * @brief setEnableRunning
     * @param bEnableRunning : bool : Work is Enable Running or Not
     */
    void setEnableRunning(bool bEnableRunning);

signals:
    /**
     * @brief DataProcUpdateSignal
     */
    void DataProcUpdateSignal(QString);

    /**
     * @brief DataProcErrorSignal
     */
    void DataProcErrorSignal(QString);

public slots:

private:
    /**
     * @brief m_pCurve1 : Curve* : Curve1
     */
    Curve* m_pCurve1;

    /**
     * @brief m_pCurve1 : Curve* : Curve2
     */
    Curve* m_pCurve2;

    /**
     * @brief m_bEnableRunning : bool : Work is Enable Running or Not
     */
    bool m_bEnableRunning;

    /**
     * @brief m_strProcName : QString : Data Processor Name
     */
    QString m_strProcName;
};

#endif // DATAPROCESS_H
