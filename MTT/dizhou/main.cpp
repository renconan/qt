#include <QtGui/QApplication>
#include "widget.h"
#include <QFont>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
