#include <QtGui/QApplication>
#include "widget.h"
#include <QFont>
#include <QTextCodec>
#include <QWSServer>
#include <QFont>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    QWSServer::setCursorVisible(false);

    w.show();

    return a.exec();
}
