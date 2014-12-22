#include "MainWindow.h"
#include "ui_MainWindow.h"

/**
 * @brief MainWindow Custom Constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bIsSplit(true),
    m_bIsPause(false),
    m_pCH1DataSrc(NULL),
    m_pCH2DataSrc(NULL),
    m_pCH1DataProc(NULL),
    m_pCH2DataProc(NULL)
{
    /** Setup UI of Main Window */
    ui->setupUi(this);

    /** Create Curves */
    this->CreateCurves();

    /** Create Data Source of Channel 1 and Channel 2 */
    this->CreateDataSource();

    /** Create Data Process of Channel 1 and Channel 2 */
    this->CreateDataProcess();

    /** Create Layout of Mainwindow */
    this->CreateLayout();

    /** Create Mainwindow Repaint Timer */
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
 * @brief CreateDataSource
 */
void MainWindow::CreateDataSource(void)
{
    /** Create Data Source of Channel 1 */
    m_pCH1DataSrc = new DataSource(Global::CH1_Serial_Name);
#ifdef _DEBUG_SIMULATE
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

    /** Create Data Process of Channel 2 */
    m_pCH2DataProc = new DataProcess(Global::CH2_Serial_Name,
                                     ui->Curve1Wgt->getCurve(),
                                     ui->Curve2Wgt->getCurve());
    m_pCH2DataProc->setEnableRunning(true);
    m_pCH2DataProc->start();
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
 * @brief on_SplitCtrlBtn_clicked
 */
void MainWindow::on_SplitCtrlBtn_clicked()
{
    /** Current is Split Status, So Merge Widget */
    if (m_bIsSplit == true)
    {
        /** Resize and Show Curve 2 Widget */
        this->ui->Curve2Wgt->setVisible(false);

        /** Resize Curve 1 Widget */
        this->ui->Curve1Wgt->move(0, 0);
        this->ui->Curve1Wgt->resize(1280, 700);

        /** Add Serial Channel2 from Curve 1 */
        this->ui->Curve1Wgt->getCurve()->AddSerial(Global::CH2_Serial_Name);

        /** Reset Split Flag */
        m_bIsSplit = false;
    }
    else    /** Current is Merge Status, So Split Widget */
    {
        /** Resize and Hide Curve 2 Widget */
        this->ui->Curve2Wgt->setVisible(true);

        /** Resize Curve 1 Widget */
        this->ui->Curve1Wgt->move(0, 0);
        this->ui->Curve1Wgt->resize(1280, 350);

        /** Remove Serial Channel2 from Curve 1 */
        this->ui->Curve1Wgt->getCurve()->RemoveSerial(Global::CH2_Serial_Name);

        /** Reset Split Flag */
        m_bIsSplit = true;
    }
}

/**
 * @brief on_ZoomInCtrlBtn_clicked
 */
void MainWindow::on_ZoomInCtrlBtn_clicked(void)
{
}

/**
 * @brief on_ZoomOutCtrlBtn_clicked
 */
void MainWindow::on_ZoomOutCtrlBtn_clicked(void)
{
}

/**
 * @brief on_PauseCtrlBtn_clicked
 */
void MainWindow::on_PauseCtrlBtn_clicked(void)
{
    /** Stop Update */
    if (m_bIsPause == false)
    {
        m_bIsPause = true;
    }
    else
    {
        m_bIsPause = false;
    }
}

/**
 * @brief on_CatchCtrlBtn_clicked
 */
void MainWindow::on_CatchCtrlBtn_clicked(void)
{
}

/**
 * @brief on_ParamCtrlBtn_clicked
 */
void MainWindow::on_ParamCtrlBtn_clicked(void)
{
}

/**
 * @brief UpdateSlot
 */
void MainWindow::UpdateSlot(void)
{
    /** Pause Status, Do Nothing but Return */
    if (m_bIsPause == true)
    {
         return;
    }

    /** Get Start Time */
    QTime vStartTime = QTime::currentTime();

    /** Repaint Curve 1 */
    g_mutexCurve1Locker.lock();
    this->ui->Curve1Wgt->repaint();
    g_mutexCurve1Locker.unlock();

    /** Repaint Curve 2 */
    g_mutexCurve2Locker.lock();
    this->ui->Curve2Wgt->repaint();
    g_mutexCurve2Locker.unlock();

    /** Get End Time & Repaint Used Time */
    QTime vEndTime = QTime::currentTime();
    int nMSecond = vStartTime.msecsTo(vEndTime);

    /** Output Paint Used Time */
    qDebug() << "Widget Update Time " << nMSecond << "ms";
    qDebug() << "Received Update Timer timeout() Signal at Time Tick = " << QTime::currentTime().toString("hh:mm:ss.zzz");
}














