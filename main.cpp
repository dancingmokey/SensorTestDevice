#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QWSServer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QWSServer::setCursorVisible(FALSE);

    MainWindow w;
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint & Qt::WindowCloseButtonHint);
    w.showFullScreen();

    return a.exec();
}
