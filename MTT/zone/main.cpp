#include <QtGui/QApplication>
#include "widget.h"
#include <QTextCodec>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.show();

    return a.exec();
}
