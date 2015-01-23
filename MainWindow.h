#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QEvent>
#include <QMovie>

#ifdef _DEBUG_SCREENSHOT
    #include <QPixmap>
    #include <QApplication>
    #include <QDesktopWidget>
#endif

#include "DataSource.h"
#include "DataProcess.h"
#include "BatteryMonitor.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief MainWindow Custom Constructor
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief MainWindow Custom Destructor
     */
    ~MainWindow();

public:
    /**
     * @brief CreateLayout
     */
    void CreateLayout(void);

    /**
     * @brief CreateCurves
     */
    void CreateCurves(void);

    /**
     * @brief CreateBatteryMonitor
     */
    void CreateBatteryMonitor(void);

    /**
     * @brief CreateStatusCheck
     */
    void CreateStatusCheck(void);

    /**
     * @brief CreateDataProcess
     */
    void CreateDataProcess(void);

    /**
     * @brief CreateDataSource
     */
    void CreateDataSource(void);

    /**
     * @brief CreateTimer
     */
    void CreateTimer(void);

    /**
     * @brief CtrlBattryAlarm
     * @param bAlarmStart
     * @param strResourceName
     */
    void CtrlBattryAlarm(bool bAlarmStart, QString strResourceName);

private slots:
    /**
     * @brief on_ZoomInCtrlBtn_clicked
     */
    void on_ZoomInCtrlBtn_clicked(void);

    /**
     * @brief on_ZoomOutCtrlBtn_clicked
     */
    void on_ZoomOutCtrlBtn_clicked(void);

    /**
     * @brief on_PauseCtrlBtn_clicked
     */
    void on_PauseCtrlBtn_clicked(void);

    /**
     * @brief on_CatchCtrlBtn_clicked
     */
    void on_CatchCtrlBtn_clicked(void);

    /**
     * @brief UpdateSlot
     */
    void UpdateSlot(void);

    /**
     * @brief UpdateCHDirectSlot
     */
    void UpdateCHDirectSlot(void);

    /**
     * @brief DataProcPauseSlot
     * @param strProcName
     */
    void DataProcPauseSlot(QString strProcName, double dNewMaxVal, double dNewMinVal);

    /**
     * @brief UpdateBatteryStatusSlot
     * @param nBattryCap
     */
    void UpdateBatteryStatusSlot(int nBatteryCap);

protected:
    /**
     * @brief mouseMoveEvent
     * @param e
     */
    void mouseMoveEvent(QMouseEvent *e);

#ifdef _DEBUG_SCREENSHOT

    /**
     * @brief mouseDoubleClickEvent
     * @param e
     */
    void mouseDoubleClickEvent(QMouseEvent *e);
#endif

private:
    /**
     * @brief m_bIsPause : bool : Is Pause Display ot Not
     */
    bool m_bIsPause;

    /**
     * @brief m_bIsAutoCatch : bool : Is Auto Catch ot Manually
     */
    bool m_bIsAutoCatch;
    bool m_bIsCatched;

    /**
     * @brief m_pBatteryMonitor
     */
    BatteryMonitor* m_pBatteryMonitor;

    /**
     * @brief m_pCH1DataSrc : DataSource* : Channel 1 Data Source
     */
    DataSource* m_pCH1DataSrc;

    /**
     * @brief m_pCH2DataSrc : DataSource* : Channel 2 Data Source
     */
    DataSource* m_pCH2DataSrc;

    /**
     * @brief m_pCH1DataProc : DataProcess* : Channel 1 Data Process
     */
    DataProcess* m_pCH1DataProc;

    /**
     * @brief m_pCH2DataProc : DataProcess* : Channel 2 Data Process
     */
    DataProcess* m_pCH2DataProc;

    /**
     * @brief m_pUpdateTimer : QTimer* : Timer of Update UI
     */
    QTimer* m_pUpdateTimer;

    /**
     * @brief m_ptMousePos
     */
    QPoint m_ptMousePos;

    QMovie* m_pBatteryAlarmMv;
    bool m_bIsBatteryLow;

private:
    /**
     * @brief ui
     */
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
