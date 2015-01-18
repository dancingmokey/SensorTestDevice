#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QWSServer>
#include <QDebug>

#define __PROGRAM_VERSION__     "1.0.1 alpha001"

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

    return a.exec();
}
