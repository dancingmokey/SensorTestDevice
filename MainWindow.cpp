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

    /** Create Curves */
    this->CreateCurves();

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

    /** Create Data Process of Channel 2 */
    m_pCH2DataProc = new DataProcess(Global::CH2_Serial_Name,
                                     ui->Curve1Wgt->getCurve(),
                                     ui->Curve2Wgt->getCurve(),
                                     m_pStatusCheck);
    m_pCH2DataProc->setEnableRunning(true);
    m_pCH2DataProc->start();
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
    if (m_bIsPause == true /*&&
        m_bIsAutoCatch == true*/)
    {
        int nCurve1MaxVal = this->ui->Curve1Wgt->getCurve()->getXAxis()->getMaxValue();
        int nCurve1MinVal = this->ui->Curve1Wgt->getCurve()->getXAxis()->getMinValue();

        nCurve1MaxVal -= 100;
        nCurve1MinVal += 100;

        if (nCurve1MaxVal - nCurve1MinVal <= 200)
        {
            return;
        }
        else
        {
            this->ui->Curve1Wgt->getCurve()->getXAxis()->UpdateAxisScale(nCurve1MaxVal, nCurve1MinVal, Global::Axis_Hor_TickVal);

            this->ui->Curve1Wgt->repaint();
        }

        int nCurve2MaxVal = this->ui->Curve2Wgt->getCurve()->getXAxis()->getMaxValue();
        int nCurve2MinVal = this->ui->Curve2Wgt->getCurve()->getXAxis()->getMinValue();

        nCurve2MaxVal -= 100;
        nCurve2MinVal += 100;

        if (nCurve2MaxVal - nCurve2MinVal <= 200)
        {
            return;
        }
        else
        {
            this->ui->Curve2Wgt->getCurve()->getXAxis()->UpdateAxisScale(nCurve2MaxVal, nCurve2MinVal, Global::Axis_Hor_TickVal);

            this->ui->Curve2Wgt->repaint();
        }
    }
}

/**
 * @brief on_ZoomOutCtrlBtn_clicked
 */
void MainWindow::on_ZoomOutCtrlBtn_clicked(void)
{
    if (m_bIsPause == true /*&&
        m_bIsAutoCatch == true*/)
    {
        int nCurve1MaxVal = this->ui->Curve1Wgt->getCurve()->getXAxis()->getMaxValue();
        int nCurve1MinVal = this->ui->Curve1Wgt->getCurve()->getXAxis()->getMinValue();

        nCurve1MaxVal += 100;
        nCurve1MinVal -= 100;

        if (nCurve1MaxVal - nCurve1MinVal > 2000)
        {
            return;
        }
        else
        {
            this->ui->Curve1Wgt->getCurve()->getXAxis()->UpdateAxisScale(nCurve1MaxVal, nCurve1MinVal, Global::Axis_Hor_TickVal);

            this->ui->Curve1Wgt->repaint();
        }





        int nCurve2MaxVal = this->ui->Curve2Wgt->getCurve()->getXAxis()->getMaxValue();
        int nCurve2MinVal = this->ui->Curve2Wgt->getCurve()->getXAxis()->getMinValue();

        nCurve2MaxVal += 100;
        nCurve2MinVal -= 100;

        if (nCurve2MaxVal - nCurve2MinVal > 2000)
        {
            return;
        }
        else
        {
            this->ui->Curve2Wgt->getCurve()->getXAxis()->UpdateAxisScale(nCurve2MaxVal, nCurve2MinVal, Global::Axis_Hor_TickVal);

            this->ui->Curve2Wgt->repaint();
        }
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

        /** */
        m_pCH1DataProc->setPause(true);
        m_pCH2DataProc->setPause(true);

        /** */
        UpdateInfoWidget();

        /** Change Button Bounder-Image */
        this->ui->PauseCtrlBtn->setStyleSheet(QString("border-image: url(:/Images/Images/ContinueBtn.png);"));
    }
    else
    {
        /** Reset Pause Flag */
        m_bIsPause = false;

        /** */
        m_pCH1DataProc->setPause(false);
        m_pCH1DataProc->setIsCatched(false);
        m_pCH2DataProc->setPause(false);
        m_pCH2DataProc->setIsCatched(false);

        /** */
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
#ifndef _DEBUG_OUTPUT
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

#ifndef _DEBUG_OUTPUT
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

//    /** Channel 1 Status Update */
//    if (strCHName == Global::CH1_Serial_Name)
//    {
//        /** Status Changed to None */
//        if (nCHStatus == Global::Status_None)
//        {
//            this->ui->TestInfoWgt->m_strCH1Status = QString("无峰值");
//            this->ui->TestInfoWgt->m_strCH1Value = QString("");

//            this->ui->TestInfoWgt->m_crCH1Other = Qt::white;
//        }
//        /** Status Changed to Catch */
//        else if (nCHStatus == Global::Status_Catch)
//        {
//            this->ui->TestInfoWgt->m_strCH1Status = QString("捕获峰值");
//            this->ui->TestInfoWgt->m_strCH1Value = QString("%1V").arg(QString::number(dCHExtremumVal, 'f', 2));

//            this->ui->TestInfoWgt->m_crCH1Other = Qt::red;
//        }
//    }
//    /** Channel 2 Status Update */
//    else if (strCHName == Global::CH2_Serial_Name)
//    {
//        /** Status Changed to None */
//        if (nCHStatus == Global::Status_None)
//        {
//            this->ui->TestInfoWgt->m_strCH2Status = QString("无峰值");
//            this->ui->TestInfoWgt->m_strCH2Value = QString("");

//            this->ui->TestInfoWgt->m_crCH2Other = Qt::white;
//        }
//        /** Status Changed to Catch */
//        else if (nCHStatus == Global::Status_Catch)
//        {
//            this->ui->TestInfoWgt->m_strCH2Status = QString("捕获峰值");
//            this->ui->TestInfoWgt->m_strCH2Value = QString("%1V").arg(QString::number(dCHExtremumVal, 'f', 2));

//            this->ui->TestInfoWgt->m_crCH2Other = Qt::blue;
//        }
//    }

//    /** Repaint Test Info Widget */
//    this->ui->TestInfoWgt->repaint();

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
    /** */
    if (m_bIsAutoCatch == true)
    {
        /** */
        if (m_bIsPause == false)
        {
            m_pCH1DataProc->setIsCatched(true);
            m_pCH2DataProc->setIsCatched(true);

            emit this->ui->PauseCtrlBtn->click();
        }
    }



    if (m_bIsAutoCatch == true)
    {
        if (m_bIsPause == false)
        {

        }
//        {
//            emit this->ui->PauseCtrlBtn->click();

//            this->ui->TestInfoWgt->m_strCH1Status = QString("捕获峰值");
//            this->ui->TestInfoWgt->m_strCH1Value = QString("%1V").arg(QString::number(m_pStatusCheck->m_dCH1ExtremumVal, 'f', 2));
//            this->ui->TestInfoWgt->m_crCH1Other = Qt::red;
//            this->ui->TestInfoWgt->m_strCH1Order = nMSDiff > 0 ? QString("+%1ms").arg(nMSDiff) : QString("%1ms").arg(nMSDiff);
//            this->ui->TestInfoWgt->m_strCH2Status = QString("捕获峰值");
//            this->ui->TestInfoWgt->m_strCH2Value = QString("%1V").arg(QString::number(m_pStatusCheck->m_dCH2ExtremumVal, 'f', 2));
//            this->ui->TestInfoWgt->m_crCH2Other = Qt::blue;
//            this->ui->TestInfoWgt->m_strCH2Order = nMSDiff > 0 ? QString("%1ms").arg(nMSDiff * -1) : QString("+%1ms").arg(nMSDiff * -1);
//            this->ui->TestInfoWgt->repaint();

//            this->ui->Curve1Wgt->getCurve()->getXAxis()->m_nCatchVal = m_pStatusCheck->m_nCH1CatchOffset;
//            this->ui->Curve2Wgt->getCurve()->getXAxis()->m_nCatchVal = m_pStatusCheck->m_nCH2CatchOffset;
//        }
    }
}












