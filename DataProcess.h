#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <QThread>
#include <QDebug>
#include <QTime>
#include "Global.h"
#include "Curve.h"
#include "StatusCheck.h"

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
                         StatusCheck* pStatusCheck,
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
     * @brief getPausePosition
     * @return
     */
    int getPausePosition();

    /**
     * @brief getIsCatched
     * @return
     */
    bool getIsCatched(void);

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
     * @brief setIsCatched
     * @param bIsCatched
     */
    void setIsCatched(bool bIsCatched);

    /**
     * @brief setPause
     * @param bIsPause
     */
    void setPause(bool bIsPause);

signals:
    /**
     * @brief DataProcUpdateSignal
     */
    void DataProcUpdateSignal(QString);

    /**
     * @brief DataProcPauseSignal
     */
    void DataProcPauseSignal(QString, double, double);

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
     * @brief m_bIsFirst
     */
    bool m_bIsFirst;

    /**
     * @brief m_strProcName : QString : Data Processor Name
     */
    QString m_strProcName;

    /**
     * @brief m_nProcValueCnt
     */
    int m_nProcValueCnt;

    /**
     * @brief m_nStatus
     */
    uint8 m_nStatus;

    /**
     * @brief m_dtCatchTime
     */
    QTime m_dtCatch;

    /**
     * @brief m_dLastValue
     */
    double m_dLastValue;

    /**
     * @brief m_dExtremumVal
     */
    double m_dExtremumVal;

    /**
     * @brief m_nCatchOffset
     */
    int m_nCatchOffset;

    /**
     * @brief m_pStatusCheck
     */
    StatusCheck* m_pStatusCheck;

    /**
     * @brief m_bIsCatched
     */
    bool m_bIsCatched;
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
