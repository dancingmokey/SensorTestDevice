#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "DataSource.h"
#include "DataProcess.h"
#include "StatusCheck.h"

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


    void UpdateInfoWidget(void);

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
     * @brief UpdateCHStatusSlot
     * @param strCHName
     * @param nCHStatus
     * @param dtCHCatch
     * @param nCHCatchOffset
     * @param nCHExtremumVal
     */
    void UpdateCHStatusSlot(QString strCHName,
                            unsigned char nCHStatus,
                            QTime dtCHCatch,
                            int nCHCatchOffset,
                            double nCHExtremumVal);

    /**
     * @brief UpdateSensorStatusSlot
     * @param nMSDiff
     */
    void UpdateSensorStatusSlot(int nMSDiff);

private:
    /**
     * @brief m_bIsPause : bool : Is Pause Display ot Not
     */
    bool m_bIsPause;

    /**
     * @brief m_bIsAutoCatch : bool : Is Auto Catch ot Manually
     */
    bool m_bIsAutoCatch;

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

    StatusCheck* m_pStatusCheck;

    /**
     * @brief m_pUpdateTimer : QTimer* : Timer of Update UI
     */
    QTimer* m_pUpdateTimer;

private:
    /**
     * @brief ui
     */
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H