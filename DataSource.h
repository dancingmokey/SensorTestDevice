#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QThread>
#include <QString>
#include <QApplication>
#include "SerialPort/qextserialbase.h"
#include "SerialPort/posix_qextserialport.h"

#include "Global.h"

/**
 * @brief The DataSource class
 */
class DataSource : public QThread
{
    Q_OBJECT
public:
    /**
     * @brief DataSource Custom Constructor Function
     * @param strSrcName : QString : Data Source Name
     */
    explicit DataSource(QString strSrcName);

    /**
     * @brief DataSource Custom Destructor Function
     */
    ~DataSource(void);

public:
    /**
     * @brief InitSerialPort
     * @param strPortName : QString : Serial Port Name
     * @return bool
     */
    bool InitSerialPort(QString strPortName);

    /**
     * @brief DestroySerialPort
     */
    void DestroySerialPort(void);

public:
    /**
     * @brief run
     * @desc  Thread Work Function, Read Data from Serial Port and then Storage it into List
     */
    virtual void run();

public:
    /**
     * @brief getSourceName
     * @return QString
     */
    QString getSourceName(void) const;

    /**
     * @brief getEnableRunning
     * @return bool
     */
    bool getEnableRunning(void) const;

    /**
     * @brief setSourceName
     * @param strSrcName : QString : Data Source Name
     */
    void setSourceName(const QString &strSrcName);

    /**
     * @brief setEnableRunning
     * @param bEnableRunning : bool : Work is Enable Running or Not
     */
    void setEnableRunning(bool bEnableRunning);

public slots:

signals:
    /**
     * @brief DataSrcErrorSignal
     */
    void DataSrcErrorSignal(QString);

private:
    /**
     * @brief m_pSerialPort : Posix_QextSerialPort* : Serial Port Object
     */
    Posix_QextSerialPort* m_pSerialPort;

    /**
     * @brief m_strSrcName : QString : Data Source Name
     */
    QString m_strSrcName;

    /**
     * @brief m_bEnableRunning : bool : Work is Enable Running or Not
     */
    bool m_bEnableRunning;
};

#endif // DATASOURCE_H
