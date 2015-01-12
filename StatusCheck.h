#ifndef STATUSCHECK_H
#define STATUSCHECK_H

#include <QThread>
#include <QList>
#include <QMutex>
#include <QDebug>

#include "Global.h"

class StatusCheck : public QThread
{
    Q_OBJECT
public:
    explicit StatusCheck(QObject *parent = 0);

public:
    /**
     * @brief run
     * @desc Thread Work Function, Process Data from List
     */
    virtual void run(void);

public:
    /**
     * @brief UpdateChannelData
     * @param nCHType
     * @param nCHStatus
     * @param dtCHCatch
     * @param nCHCatchOffset
     * @param nCHExtremumVal
     */
    void UpdateChannelData(QString strCHName,
                           unsigned char nCHStatus,
                           QTime dtCHCatch,
                           int nCHCatchOffset,
                           double nCHExtremumVal);

public:
    /**
     * @brief getEnableRunning
     * @return bool
     */
    bool getEnableRunning(void) const;
    /**
     * @brief setEnableRunning
     * @param bEnableRunning : bool : Work is Enable Running or Not
     */
    void setEnableRunning(bool bEnableRunning);

signals:
    void UpdateCHStatusSignal(QString, unsigned char, QTime, int, double);
    void UpdateSensorStatusSignal(int);

public slots:

private:
    /**
     * @brief m_bEnableRunning : bool : Work is Enable Running or Not
     */
    bool m_bEnableRunning;

public:
    /**
     * @brief m_nCH1Status
     */
    uint8 m_nCH1Status;

    /**
     * @brief m_dtCH1Catch
     */
    QTime m_dtCH1Catch;

    /**
     * @brief m_nCH1CatchOffset
     */
    int m_nCH1CatchOffset;

    /**
     * @brief m_dCH1ExtremumVal
     */
    double m_dCH1ExtremumVal;

    /**
     * @brief m_mutexCH1Locker
     */
    QMutex m_mutexCH1Locker;

    /**
     * @brief m_nCH2Status
     */
    uint8 m_nCH2Status;

    /**
     * @brief m_dtCH2Catch
     */
    QTime m_dtCH2Catch;

    /**
     * @brief m_nCH2CatchOffset
     */
    int m_nCH2CatchOffset;

    /**
     * @brief m_dCH2ExtremumVal
     */
    double m_dCH2ExtremumVal;

    /**
     * @brief m_mutexCH2Locker
     */
    QMutex m_mutexCH2Locker;

};

#endif // STATUSCHECK_H
