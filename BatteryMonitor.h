#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>

#include <QThread>
#include <QDebug>

#include "Global.h"

class BatteryMonitor : public QThread
{
    Q_OBJECT
public:
    explicit BatteryMonitor(QObject *parent = 0);



public:
    /**
     * @brief run
     * @desc  Thread Work Function
     */
    virtual void run();


public:
    /**
     * @brief setEnableRunning
     * @param bEnableRunning : bool : Work is Enable Running or Not
     */
    void setEnableRunning(bool bEnableRunning);

    /**
     * @brief getEnableRunning
     * @return bool
     */
    bool getEnableRunning(void) const;

signals:
    /**
     * @brief UpdateBatteryStatus
     * @param nStatus
     */
    void UpdateBatteryStatusSignal(int nStatus);

public slots:


private:

    /**
     * @brief m_bEnableRunning : bool : Work is Enable Running or Not
     */
    bool m_bEnableRunning;

};

#endif // BATTERYMONITOR_H
