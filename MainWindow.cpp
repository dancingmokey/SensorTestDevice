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
    m_bIsCatched(false),
    m_pCH1DataSrc(NULL),
    m_pCH2DataSrc(NULL),
    m_pCH1DataProc(NULL),
    m_pCH2DataProc(NULL)
{
    /** Setup UI of Main Window */
    ui->setupUi(this);

    /** */
    m_bIsPause = false;
    m_bIsAutoCatch = false;
    m_bIsCatched = false;

    /** Set Battery Capacity Alarm */
    m_bIsBatteryLow = false;
    m_pBatteryAlarmMv = new QMovie(":/Images/Images/Battery_Alarm.gif");

    /** ? */
    this->setAttribute(Qt::WA_AcceptTouchEvents);

    /** Create Curves */
    this->CreateCurves();

    /** Create Battery Monitor */
    this->CreateBatteryMonitor();

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
 * @brief CreateDataProcess
 */
void MainWindow::CreateDataProcess(void)
{
    /** Create Data Process of Channel 1 */
    m_pCH1DataProc = new DataProcess(Global::CH1_Serial_Name,
                                     ui->Curve1Wgt->getCurve(),
                                     ui->Curve2Wgt->getCurve());
    m_pCH1DataProc->setEnableRunning(true);
    m_pCH1DataProc->start();

    /** Create Connection */
    connect(m_pCH1DataProc,
            SIGNAL(UpdateCHDirectSignal()),
            this,
            SLOT(UpdateCHDirectSlot()));
    connect(m_pCH1DataProc,
            SIGNAL(DataProcPauseSignal(QString, double, double)),
            this,
            SLOT(DataProcPauseSlot(QString, double, double)));

    /** Create Data Process of Channel 2 */
    m_pCH2DataProc = new DataProcess(Global::CH2_Serial_Name,
                                     ui->Curve1Wgt->getCurve(),
                                     ui->Curve2Wgt->getCurve());
    m_pCH2DataProc->setEnableRunning(true);
    m_pCH2DataProc->start();

    /** Create Connection */
    connect(m_pCH2DataProc,
            SIGNAL(UpdateCHDirectSignal()),
            this,
            SLOT(UpdateCHDirectSlot()));
    connect(m_pCH2DataProc,
            SIGNAL(DataProcPauseSignal(QString, double, double)),
            this,
            SLOT(DataProcPauseSlot(QString, double, double)));
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

        /** Allow Curve Widget Draw Mouse Position Line when Auto Catch Mode is close */
        if ((m_bIsAutoCatch == false) ||
            ((m_bIsAutoCatch == true) &&
             ((m_bIsCatched == false))))
        {
            /** Pause Data Process Thread */
            m_pCH1DataProc->setPausePosition(true, 0);
            m_pCH2DataProc->setPausePosition(true, 0);

            Axis* pCurve1XAxis = this->ui->Curve1Wgt->getCurve()->getXAxis();
            this->ui->Curve1Wgt->setIsDrawMousePos(true);
            this->ui->Curve1Wgt->setMousePos(QPoint(665, 0));
            this->ui->Curve1Wgt->setZoomParams(true,
                                               pCurve1XAxis->getMaxValue(),
                                               pCurve1XAxis->getMinValue());
            Axis* pCurve2XAxis = this->ui->Curve2Wgt->getCurve()->getXAxis();
            this->ui->Curve2Wgt->setIsDrawMousePos(true);
            this->ui->Curve2Wgt->setMousePos(QPoint(665, 0));
            this->ui->Curve2Wgt->setZoomParams(true,
                                               pCurve2XAxis->getMaxValue(),
                                               pCurve2XAxis->getMinValue());
        }
        else
        {
            /** Pause Data Process Thread */
            m_pCH1DataProc->setPausePosition(true, 700);
            m_pCH2DataProc->setPausePosition(true, 700);
        }

        /** Change Button Bounder-Image */
        this->ui->PauseCtrlBtn->setStyleSheet(
                    QString("border-image: url(:/Images/Images/ContinueBtn.png);"));
    }
    else
    {
        /** Reset Pause Flag */
        m_bIsPause = false;

        /** Reset Catched Flag */
        m_bIsCatched = false;

        /** Forbidden Curve Widget Draw Mouse Position Line */
        this->ui->Curve1Wgt->setIsDrawMousePos(false);
        this->ui->Curve1Wgt->setZoomParams(false, 0.0f, 0.0f);
        this->ui->Curve2Wgt->setIsDrawMousePos(false);
        this->ui->Curve2Wgt->setZoomParams(false, 0.0f, 0.0f);

        /** Restart Data Process Thread */
        m_pCH1DataProc->setPausePosition(false, 0);
        m_pCH2DataProc->setPausePosition(false, 0);

        /** Change Button Bounder-Image */
        this->ui->PauseCtrlBtn->setStyleSheet(
                    QString("border-image: url(:/Images/Images/PauseBtn.png);"));
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
        m_bIsCatched = false;

        /** Change Button Bounder-Image */
        this->ui->CatchCtrlBtn->setStyleSheet(
                    QString("border-image: url(:/Images/Images/AutoCatchCloseBtn.png);"));

        /** Change Label Bounder-Image */
        this->ui->LeftInfoLabel->setStyleSheet(
                    QString("border-image: url(:/Images/Images/AutoCatchOpenLabel.png);"));
    }
    else
    {
        /** Reset Pause Flag */
        m_bIsAutoCatch = false;
        m_bIsCatched = false;

        /** Change Button Bounder-Image */
        this->ui->CatchCtrlBtn->setStyleSheet(
                    QString("border-image: url(:/Images/Images/AutoCatchOpenBtn.png);"));

        /** Change Label Bounder-Image */
        this->ui->LeftInfoLabel->setStyleSheet(
                    QString("border-image: url(:/Images/Images/AutoCatchCloseLabel.png);"));
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
 * @brief MainWindow::UpdateCHDirectSlot
 */
void MainWindow::UpdateCHDirectSlot(void)
{
    /** Get Channel 1 & 2 Status */
    uint8 nCH1Direct = m_pCH1DataProc->getChannelDirect();
    uint8 nCH2Direct = m_pCH2DataProc->getChannelDirect();

    /** Check Sensor Direct */
    if ((nCH1Direct == Global::CH_Direct_None) ||
        (nCH2Direct == Global::CH_Direct_None))
    {
        /** Change Test Info Image */
        this->ui->RightInfoLabel->setStyleSheet(
                    QString("border-image: url(:/Images/Images/DirectUnknown.png);"));
    }
    /** Sensor Direct is Positive */
    else if ((nCH1Direct == Global::CH_Direct_Pos) ||
             (nCH2Direct == Global::CH_Direct_Neg))
    {
        /** In Auto Catch Mode, Pause Curves */
        if ((m_bIsAutoCatch == true) &&
            (m_bIsCatched == false))
        {
            /** Reset Catched Flag */
            m_bIsCatched = true;

            /** Pause Data Process Thread */
            emit this->ui->PauseCtrlBtn->click();
        }

        /** Change Test Info Image */
        this->ui->RightInfoLabel->setStyleSheet(
                    QString("border-image: url(:/Images/Images/DirectPos.png);"));
    }
    /** Sensor Direct is Negtive */
    else if ((nCH1Direct == Global::CH_Direct_Neg) ||
             (nCH2Direct == Global::CH_Direct_Pos))
    {
        /** In Auto Catch Mode, Pause Curves */
        if ((m_bIsAutoCatch == true) &&
            (m_bIsCatched == false))
        {
            /** Reset Catched Flag */
            m_bIsCatched = true;

            /** Pause Data Process Thread */
            emit this->ui->PauseCtrlBtn->click();
        }

        /** Change Test Info Image */
        this->ui->RightInfoLabel->setStyleSheet(
                    QString("border-image: url(:/Images/Images/DirectNeg.png);"));
    }
    /** Sensor Direct is Invalidity */
    else
    {
        /** Change Test Info Image */
        this->ui->RightInfoLabel->setStyleSheet(
                    QString("border-image: url(:/Images/Images/DirectError.png);"));
    }
}

/**
 * @brief DataProcPauseSlot
 * @param strProcName
 */
void MainWindow::DataProcPauseSlot(QString strProcName, double dNewMaxVal, double dNewMinVal)
{
    if (strProcName == Global::CH1_Serial_Name)
    {
        if (m_bIsAutoCatch == true)
        {
            this->ui->Curve1Wgt->setIsDrawMousePos(true);
            this->ui->Curve1Wgt->setMousePos(QPoint(665, 0));
            this->ui->Curve1Wgt->setZoomParams(true, dNewMaxVal, dNewMinVal);
        }
    }
    else if (strProcName == Global::CH2_Serial_Name)
    {
        if (m_bIsAutoCatch == true)
        {
            this->ui->Curve2Wgt->setIsDrawMousePos(true);
            this->ui->Curve2Wgt->setMousePos(QPoint(665, 0));
            this->ui->Curve2Wgt->setZoomParams(true, dNewMaxVal, dNewMinVal);
        }
    }
}

/**
 * @brief CtrlBattryAlarm
 * @param bAlarmStart
 * @param strResourceName
 */
void MainWindow::CtrlBattryAlarm(bool bAlarmStart, QString strResourceName)
{
    /** Start Alarm */
    if (bAlarmStart == true)
    {
        /** Only Start Movie 1 Time */
        if (m_bIsBatteryLow == false)
        {
            /** Set Battery Low Flag */
            m_bIsBatteryLow = true;

            /** Set Label Show Mode */
            this->ui->BatteryStatusLabel->setMovie(m_pBatteryAlarmMv);

            /** Start Show Gif Picture */
            m_pBatteryAlarmMv->start();
        }

    }
    /** End Alarm */
    else
    {
        /** Set Battery Low Flag */
        m_bIsBatteryLow = false;

        /** Stop Show Gif Picture */
        m_pBatteryAlarmMv->stop();

        /** Set Label Show Mode */
        this->ui->BatteryStatusLabel->setPixmap(QPixmap());

        /** Set Battery Capacity Image Full */
        this->ui->BatteryStatusLabel->setStyleSheet(strResourceName);
    }
}

/**
 * @brief MainWindow::UpdateBatteryStatusSlot
 * @param nBatteryCap
 */
void MainWindow::UpdateBatteryStatusSlot(int nBatteryCap)
{
    /** Battery Capcity Value isnot Avaliable */
    if (nBatteryCap < 0)
    {
        /** Set Battery Capacity Image Full */
        this->CtrlBattryAlarm(false, QString("border-image: url(:/Images/Images/Battery_Invalid.png);"));
    }
    /** Battery Capacity Value is Avaliable */
    else
    {
        /** Battery is Very Low */
        if (nBatteryCap < Global::Battery_Low_ThresVal)
        {
            /** Set Battery Capacity Image Low */
            this->CtrlBattryAlarm(true, QString(""));
        }
        else if (nBatteryCap < Global::Battery_G2_ThresVal)
        {
            /** Set Battery Capacity Image Low */
            this->CtrlBattryAlarm(false, QString("border-image: url(:/Images/Images/Battery_Low.png);"));
        }
        else if (nBatteryCap < Global::Battery_Medium_ThresVal)
        {
            /** Set Battery Capacity Image Full */
            this->CtrlBattryAlarm(false, QString("border-image: url(:/Images/Images/Battery_G2.png);"));
        }
        else if (nBatteryCap < Global::Battery_G1_ThresVal)
        {
            /** Set Battery Capacity Image Full */
            this->CtrlBattryAlarm(false, QString("border-image: url(:/Images/Images/Battery_Medium.png);"));
        }
        else if (nBatteryCap < Global::Battery_High_ThresVal)
        {
            /** Set Battery Capacity Image Full */
            this->CtrlBattryAlarm(false, QString("border-image: url(:/Images/Images/Battery_G1.png);"));
        }
        else
        {
            /** Set Battery Capacity Image Full */
            this->CtrlBattryAlarm(false, QString("border-image: url(:/Images/Images/Battery_Full.png);"));
        }
    }

#ifndef _DEBUG_OUTPUT
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

#ifdef _DEBUG_SCREENSHOT
/**
 * @brief mouseDoubleClickEvent
 * @param e
 */
void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    /** */
    QString strScrShotName = QTime::currentTime().toString(QString("hh_mm_ss.jpg"));
    QPixmap::grabWindow(QApplication::desktop()->winId()).save(strScrShotName, "jpg");

    /** */
    qDebug() << "Save Screen Shot Image " << strScrShotName << " Successed!";
}
#endif







