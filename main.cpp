#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QWSServer>
#include <QDebug>
#include <QFile>

#define __PROGRAM_VERSION__     "1.0.2"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Sensor Test Device Version " << __PROGRAM_VERSION__;

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QWSServer::setCursorVisible(FALSE);

    MainWindow w;
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint & Qt::WindowCloseButtonHint);
    w.showFullScreen();

    QFile logFile;
    QString strLog = QString("%1 SensorTestDevice Start!\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    logFile.setFileName(QString("devstatus.log"));
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    logFile.write(strLog.toLatin1().data(), strLog.length());
    logFile.close();

    return a.exec();
}
