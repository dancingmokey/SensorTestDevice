#include "MainWindow.h"
#include "ui_MainWindow.h"

/**
 * @brief MainWindow Custom Constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bIsPause(false),
    m_bIsAutoCatch(false),
    m_pCH1DataSrc(NULL),
    m_pCH2DataSrc(NULL),
    m_pCH1DataProc(NULL),
    m_pCH2DataProc(NULL),
    m_pStatusCheck(NULL)
{
    /** Setup UI of Main Window */
    ui->setupUi(this);

    /** ? */
    this->setAttribute(Qt::WA_AcceptTouchEvents);

    /** Create Curves */
    this->CreateCurves();

    /** Create Battery Monitor */
    this->CreateBatteryMonitor();

    /** Create StatusCheck */
    this->CreateStatusCheck();

    /** Create Data Source of Channel 1 and Channel 2 */
    this->CreateDataSource();

    /** Create Data Process of Channel 1 and Channel 2 */
    this->CreateDataProcess();

    /** Create Layout of Mainwindow */
    this->CreateLayout();

    /** Create Mainwindow Repaint Timer */
    g_dtLastUpDate = QTime::currentTime();
    this->CreateTimer();

    /** set Mouse Tracking */
    this->setMouseTracking(true);
}

/**
 * @brief MainWindow Custom Destructor
 */
MainWindow::~MainWindow()
{
    /** Exit from Thread and then Release Memory */
    m_pCH1DataSrc->setEnableRunning(false);
    Global::Safe_Delete(m_pCH1DataSrc);

    /** Exit from Thread and then Release Memory */
    m_pCH2DataSrc->setEnableRunning(false);
    Global::Safe_Delete(m_pCH2DataSrc);

    /** Exit from Thread and then Release Memory */
    m_pCH1DataProc->setEnableRunning(false);
    Global::Safe_Delete(m_pCH1DataProc);

    /** Exit from Thread and then Release Memory */
    m_pCH2DataProc->setEnableRunning(false);
    Global::Safe_Delete(m_pCH2DataProc);

    /** Delete MainWindow UI */
    delete ui;
}

/**
 * @brief CreateLayout
 */
void MainWindow::CreateLayout(void)
{
    /** Layout Curve 1 Widget */
    this->ui->Curve1Wgt->move(0, 0);
    this->ui->Curve1Wgt->resize(1280, 350);

    /** Layout Curve 2 Widget */
    this->ui->Curve2Wgt->move(0, 350);
    this->ui->Curve2Wgt->resize(1280, 350);

    /** Layout Control Widget */
    this->ui->ControlWgt->move(0, 700);
    this->ui->ControlWgt->resize(1280, 100);
}

/**
 * @brief CreateCurves
 */
void MainWindow::CreateCurves(void)
{
    /** Add Serial into Curve */
    this->ui->Curve1Wgt->UpdateCurve(Global::Serial_Add, Global::CH1_Serial_Name);
    this->ui->Curve2Wgt->UpdateCurve(Global::Serial_Add, Global::CH2_Serial_Name);
}

/**
 * @brief CreateBatteryMonitor
 */
void MainWindow::CreateBatteryMonitor(void)
{
    /** Create Battery Monitor */
    m_pBatteryMonitor = new BatteryMonitor(this);
    m_pBatteryMonitor->setEnableRunning(true);
    m_pBatteryMonitor->start();

    /** Create Connection */
    connect(m_pBatteryMonitor,
            SIGNAL(UpdateBatteryStatusSignal(int)),
            this,
            SLOT(UpdateBatteryStatusSlot(int)));
}

/**
 * @brief CreateStatusCheck
 */
void MainWindow::CreateStatusCheck(void)
{
    /** Create Status Check */
    m_pStatusCheck = new StatusCheck(this);
    m_pStatusCheck->setEnableRunning(true);
    m_pStatusCheck->start();

    /** Create Connection */
    connect(m_pStatusCheck,
            SIGNAL(UpdateCHStatusSignal(QString,unsigned char,QTime,int,double)),
            this,
            SLOT(UpdateCHStatusSlot(QString,unsigned char,QTime,int,double)));
    connect(m_pStatusCheck,
            SIGNAL(UpdateSensorStatusSignal(int)),
            this,
            SLOT(UpdateSensorStatusSlot(int)));
}

/**
 * @brief CreateDataProcess
 */
void MainWindow::CreateDataProcess(void)
{
    /** Create Data Process of Channel 1 */
    m_pCH1DataProc = new DataProcess(Global::CH1_Serial_Name,
                                     ui->Curve1Wgt->getCurve(),
                                     ui->Curve2Wgt->getCurve(),
                                     m_pStatusCheck);
    m_pCH1DataProc->setEnableRunning(true);
    m_pCH1DataProc->start();

    /** Create Connection */
    connect(m_pCH1DataProc,
            SIGNAL(DataProcPauseSignal()),
            this,
            SLOT(DataProcPauseSlot()));

    /** Create Data Process of Channel 2 */
    m_pCH2DataProc = new DataProcess(Global::CH2_Serial_Name,
                                     ui->Curve1Wgt->getCurve(),
                                     ui->Curve2Wgt->getCurve(),
                                     m_pStatusCheck);
    m_pCH2DataProc->setEnableRunning(true);
    m_pCH2DataProc->start();

    /** Create Connection */
    connect(m_pCH2DataProc,
            SIGNAL(DataProcPauseSignal()),
            this,
            SLOT(DataProcPauseSlot()));
}

/**
 * @brief CreateDataSource
 */
void MainWindow::CreateDataSource(void)
{
    /** Create Data Source of Channel 1 */
    m_pCH1DataSrc = new DataSource(Global::CH1_Serial_Name);
#ifdef _DEBUG_SIMULATE
    m_pCH1DataSrc->setEnableRunning(true);
    m_pCH1DataSrc->start();
#else
    if (m_pCH1DataSrc->InitSerialPort(Global::CH1_SerialPort_Name) == true)
    {
        m_pCH1DataSrc->setEnableRunning(true);
        m_pCH1DataSrc->start();
    }
    else
    {
        /** Show Error Message */
        QMessageBox::information(this, "Error!", "Channel 1 Serial Port Open Failed!");
    }
#endif

    /** Create Data Source of Channel 2 */
    m_pCH2DataSrc = new DataSource(Global::CH2_Serial_Name);
#ifdef _DEBUG_SIMULATE
    m_pCH2DataSrc->setEnableRunning(true);
    m_pCH2DataSrc->start();
#else
    if (m_pCH2DataSrc->InitSerialPort(Global::CH2_SerialPort_Name) == true)
    {
        m_pCH2DataSrc->setEnableRunning(true);
        m_pCH2DataSrc->start();
    }
    else
    {
        /** Show Error Message */
        QMessageBox::information(this, "Error!", "Channel 2 Serial Port Open Failed!");
    }
#endif
}

/**
 * @brief CreateTimer
 */
void MainWindow::CreateTimer(void)
{
    m_pUpdateTimer = new QTimer();
    connect(m_pUpdateTimer,
            SIGNAL(timeout()),
            this,
            SLOT(UpdateSlot()));
    m_pUpdateTimer->start(100);
}

void MainWindow::UpdateInfoWidget(void)
{
    if (m_bIsPause == true)
    {
        int nCH1Offset = this->m_pCH1DataProc->getPausePosition() -
                this->ui->Curve1Wgt->getCurve()->getXAxis()->getMinValue();
        int nCH2Offset = this->m_pCH2DataProc->getPausePosition() -
                this->ui->Curve2Wgt->getCurve()->getXAxis()->getMinValue();


        this->ui->TestInfoWgt->m_strCH1Status = QString("捕获峰值");




        this->ui->TestInfoWgt->m_strCH2Status = QString("捕获峰值");

        this->ui->TestInfoWgt->m_strCH1Status = QString("%1").arg(nCH1Offset);
        this->ui->TestInfoWgt->m_strCH2Status = QString("%1").arg(nCH2Offset);
        this->ui->TestInfoWgt->repaint();
//        this->ui->TestInfoWgt->m_strCH1Value = QString("%1V").arg(QString::number(m_pStatusCheck->m_dCH1ExtremumVal, 'f', 2));
//        this->ui->TestInfoWgt->m_crCH1Other = Qt::red;
//        this->ui->TestInfoWgt->m_strCH1Order = nMSDiff > 0 ? QString("+%1ms").arg(nMSDiff) : QString("%1ms").arg(nMSDiff);
//        this->ui->TestInfoWgt->m_strCH2Status = QString("捕获峰值");
//        this->ui->TestInfoWgt->m_strCH2Value = QString("%1V").arg(QString::number(m_pStatusCheck->m_dCH2ExtremumVal, 'f', 2));
//        this->ui->TestInfoWgt->m_crCH2Other = Qt::blue;
//        this->ui->TestInfoWgt->m_strCH2Order = nMSDiff > 0 ? QString("%1ms").arg(nMSDiff * -1) : QString("+%1ms").arg(nMSDiff * -1);


        this->ui->Curve1Wgt->getCurve()->getXAxis()->m_nCatchVal = m_pStatusCheck->m_nCH1CatchOffset;
        this->ui->Curve2Wgt->getCurve()->getXAxis()->m_nCatchVal = m_pStatusCheck->m_nCH2CatchOffset;
    }
    else
    {
        this->ui->TestInfoWgt->m_strCH1Status = QString("%1").arg(0);
        this->ui->TestInfoWgt->m_strCH2Status = QString("%1").arg(0);
        this->ui->TestInfoWgt->repaint();
    }
}

/**
 * @brief on_ZoomInCtrlBtn_clicked
 */
void MainWindow::on_ZoomInCtrlBtn_clicked(void)
{
    if (m_bIsPause == true)
    {
        /** Set Widget Zoom Value of Curve 1 & 2 */
        this->ui->Curve1Wgt->ZoomInOperation();
        this->ui->Curve2Wgt->ZoomInOperation();
    }
}

/**
 * @brief on_ZoomOutCtrlBtn_clicked
 */
void MainWindow::on_ZoomOutCtrlBtn_clicked(void)
{
    if (m_bIsPause == true)
    {
        /** Set Widget Zoom Value of Curve 1 & 2 */
        this->ui->Curve1Wgt->ZoomOutOperation();
        this->ui->Curve2Wgt->ZoomOutOperation();
    }
}

/**
 * @brief on_PauseCtrlBtn_clicked
 */
void MainWindow::on_PauseCtrlBtn_clicked(void)
{
    /** Stop Update */
    if (m_bIsPause == false)
    {
        /** Reset Pause Flag */
        m_bIsPause = true;

        /** Pause Data Process Thread */
        m_pCH1DataProc->setPause(true);
        m_pCH2DataProc->setPause(true);

        /** Allow Curve Widget Draw Mouse Position Line */
        this->ui->Curve1Wgt->setIsDrawMousePos(true);
        this->ui->Curve2Wgt->setIsDrawMousePos(true);

        /** Update Test Information Widget */
        UpdateInfoWidget();

        /** Change Button Bounder-Image */
        this->ui->PauseCtrlBtn->setStyleSheet(QString("border-image: url(:/Images/Images/ContinueBtn.png);"));
    }
    else
    {
        /** Reset Pause Flag */
        m_bIsPause = false;

        /** Restart Data Process Thread */
        m_pCH1DataProc->setPause(false);
        m_pCH1DataProc->setIsCatched(false);
        m_pCH2DataProc->setPause(false);
        m_pCH2DataProc->setIsCatched(false);

        /** Forbidden Curve Widget Draw Mouse Position Line */
        this->ui->Curve1Wgt->setIsDrawMousePos(false);
        this->ui->Curve2Wgt->setIsDrawMousePos(false);

        /** Update Test Information Widget */
        UpdateInfoWidget();

        /** Change Button Bounder-Image */
        this->ui->PauseCtrlBtn->setStyleSheet(QString("border-image: url(:/Images/Images/PauseBtn.png);"));
    }
}

/**
 * @brief on_CatchCtrlBtn_clicked
 */
void MainWindow::on_CatchCtrlBtn_clicked(void)
{
    /** Set Catch Mode Manually */
    if (m_bIsAutoCatch == false)
    {
        /** Reset Auto Catch Flag */
        m_bIsAutoCatch = true;

        /** Change Button Bounder-Image */
        this->ui->CatchCtrlBtn->setStyleSheet(QString("border-image: url(:/Images/Images/AutoCatchCloseBtn.png);"));
    }
    else
    {
        /** Reset Pause Flag */
        m_bIsAutoCatch = false;

        /** Change Button Bounder-Image */
        this->ui->CatchCtrlBtn->setStyleSheet(QString("border-image: url(:/Images/Images/AutoCatchOpenBtn.png);"));
    }
}

/**
 * @brief UpdateSlot
 */
void MainWindow::UpdateSlot(void)
{
#ifdef _DEBUG_OUTPUT
    /** Get Start Time */
    QTime vStartTime = QTime::currentTime();
#endif

    /** Generate Global X Axis Labels */
    g_ltXLabelTexts.clear();
    int nTickCount = (Global::Axis_Hor_MaxVal -
                      Global::Axis_Hor_MinVal) /
                      Global::Axis_Hor_TickVal + 1;
    for (int i = 0; i < nTickCount; i++)
    {
        double dValue = (double)i;
        g_ltXLabelTexts.push_back(QString::number(dValue, 'f', 1));
    }

    /** Repaint Curve 1 */
    g_mutexCurve1Locker.lock();
    this->ui->Curve1Wgt->repaint();
    g_mutexCurve1Locker.unlock();

    /** Repaint Curve 2 */
    g_mutexCurve2Locker.lock();
    this->ui->Curve2Wgt->repaint();
    g_mutexCurve2Locker.unlock();

    /** Repaint Test Info Widget */
    this->ui->TestInfoWgt->repaint();

#ifdef _DEBUG_OUTPUT
    /** Get End Time & Repaint Used Time */
    QTime vEndTime = QTime::currentTime();
    int nMSecond = vStartTime.msecsTo(vEndTime);

    /** Output Paint Used Time */
    qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz")
             << " Received Update Timer timeout() Signal then Widget Update Used Time "
             << nMSecond
             << " ms";
#endif
}

/**
 * @brief DataProcPauseSlot
 * @param strProcName
 */
void MainWindow::DataProcPauseSlot(void)
{
    if ((m_pCH1DataProc->getPauseStatus() == true) &&
        (m_pCH2DataProc->getPauseStatus() == true))
    {
        /** Simulate Pause Button Click Signal */
        emit this->ui->PauseCtrlBtn->click();
    }
}

/**
 * @brief MainWindow::UpdateCHStatusSlot
 * @param strCHName
 * @param nCHStatus
 * @param dtCHCatch
 * @param nCHCatchOffset
 * @param nCHExtremumVal
 */
void MainWindow::UpdateCHStatusSlot(QString strCHName,
                                    uint8 nCHStatus,
                                    QTime dtCHCatch,
                                    int nCHCatchOffset,
                                    double dCHExtremumVal)
{
    /** */
    if (m_bIsPause == true)
    {
        return;
    }

#ifdef _DEBUG_OUTPUT
    /** Debug Output */
    qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz")
             << " Received Update Channel StatusTimer Signal Channel Name is "
             << strCHName
             << " Status is "
             << Global::Status_ToString(nCHStatus)
             << " Catch Time is "
             << dtCHCatch.toString("hh:mm:ss.zzz")
             << " Offset is "
             << nCHCatchOffset
             << " Value is "
             << dCHExtremumVal
             << " v";
#endif
}

/**
 * @brief MainWindow::UpdateSensorStatusSlot
 * @param nMSDiff
 */
void MainWindow::UpdateSensorStatusSlot(int nMSDiff)
{
    /** Current Mode is Auto Catch */
    if (m_bIsAutoCatch == true)
    {
        /** Now the Process is Runninng */
        if (m_bIsPause == false)
        {
            /** Set Data Process Catched Flag */
            m_pCH1DataProc->setIsCatched(true);
            m_pCH2DataProc->setIsCatched(true);

            /** Pause Data Process Thread */
            m_pCH1DataProc->setPause(true);
            m_pCH2DataProc->setPause(true);
        }
    }
}

void MainWindow::UpdateBatteryStatusSlot(int nBatteryCap)
{
    /** Battery Capcity Value isnot Avaliable */
    if (nBatteryCap < 0)
    {
        if (nBatteryCap == -4)
        {
            qDebug() << "Open Device /dev/adc Failed!";
        }
        else if (nBatteryCap == -3)
        {
            qDebug() << "Set Device /dev/adc Channel Failed!";
        }
        else if (nBatteryCap == -2)
        {
            qDebug() << "Tranform Battery Capacity Value Failed!";
        }
        else if (nBatteryCap == -1)
        {
            qDebug() << "Read Battery Capacity Value Failed!";
        }
        else
        {
            qDebug() << "Read Battery Capacity Value Failed! Unknown Error!";
        }
    }
    /** Battery Capacity Value is Avaliable */
    else
    {
        /** Battery Capacity Full */
        if (nBatteryCap > 12)
        {
            /** Set Battery Capacity Image Full */
            this->ui->BatteryStatusLabel->setStyleSheet(
                        QString("border-image: url(:/Images/Images/Battery_Full.png);"));
        }
        /** Battery Capacity Medium */
        else if (nBatteryCap > 11)
        {
            /** Set Battery Capacity Image High */
            this->ui->BatteryStatusLabel->setStyleSheet(
                        QString("border-image: url(:/Images/Images/Battery_Medium.png);"));
        }
        /** Battery Capacity Low */
        else
        {
            /** Set Battery Capacity Image Low */
            this->ui->BatteryStatusLabel->setStyleSheet(
                        QString("border-image: url(:/Images/Images/Battery_Low.png);"));
        }
    }

#ifdef _DEBUG_OUTPUT
            /** Debug Output */
            qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz")
                     << " Battery Monitor Send Value  "
                     << nBatteryCap;
#endif
}

/**
 * @brief MainWindow::mouseMoveEvent
 * @param e
 */
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    /** Only if Device is Paused */
    if (m_bIsPause == true)
    {
        /** Set Mouse Position to CurveWidget 1 & 2 */
        this->ui->Curve1Wgt->setMousePos(e->pos());
        this->ui->Curve2Wgt->setMousePos(e->pos());

#ifdef _DEBUG_OUTPUT
        /** Debug Output */
        qDebug() << "Detect Mouse Double Click Event, Position("
                 << e->pos().x()
                 << ","
                 << e->pos().y()
                 << ")";
#endif
    }
}








