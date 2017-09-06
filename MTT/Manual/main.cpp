#include <QtGui/QApplication>
#include <QFont>
#include <QWSServer>
#include "manualtest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Manualtest w;
   // QWSServer::setCursorVisible(true);
    w.show();

    return a.exec();
}
