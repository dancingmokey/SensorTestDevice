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
    explicit DataProcess(QString strProcName,
                         Curve* pCurve1,
                         Curve* pCurve2,
                         QObject *parent = 0);

public:
    /**
     * @brief run
     * @desc Thread Work Function, Process Data from List
     */
    virtual void run(void);

public:
    /**
     * @brief CheckStatus
     * @param dValue
     * @param dLastValue
     */
    void UpdateChannelStatus(double dCurrValue);

    /**
     * @brief DataProcess::UpdateChannelDirect
     * @param nLastCHStatus
     */
    void UpdateChannelDirect(uint8 nLastCHStatus);

    /**
     * @brief getFilterValue
     * @param dCurrValue
     * @return
     */
    double getFilterValue(double dCurrValue);


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
     * @brief getChannelStatus
     * @return
     */
    uint8 getChannelStatus(void);

    /**
     * @brief getChannelDirect
     * @return
     */
    uint8 getChannelDirect(void);

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

    /**
     * @brief setPausePosition
     * @param bIsPause
     * @param nDelayValue
     */
    void setPausePosition(bool bIsPause, int nDelayValue);

signals:
    /**
     * @brief DataProcPauseSignal
     */
    void DataProcPauseSignal(QString, double, double);

    /**
     * @brief UpdateCHDirectSignal
     */
    void UpdateCHDirectSignal(void);

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

    /**
     * @brief m_nProcValueCnt
     */
    int m_nProcValueCnt;

    /**
     * @brief m_nChannelStatus
     */
    uint8 m_nChannelStatus;

    /**
     * @brief m_dLastValue
     */
    double m_dLastValue;

    /**
     * @brief m_nChannelDirect
     */
    uint8 m_nChannelDirect;

    /**
     * @brief m_dtDirectChange
     */
    QTime m_dtDirectChange;

    /**
     * @brief m_bIsPause
     */
    bool m_bIsPause;

    /**
     * @brief m_nPausePosition
     */
    int m_nPausePosition;

    /**
     * @brief m_bIsSendPauseSig
     */
    bool m_bIsSendPauseSig;
};

#endif // DATAPROCESS_H
