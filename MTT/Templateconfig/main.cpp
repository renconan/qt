#include <QtGui/QApplication>
#include "widget.h"
#include <QMotifStyle>
#include <QWindowsStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
 //   a.setStyle(new QWindowsStyle);
    QApplication::setStyle("plastique");
    Widget w;
    w.show();

    return a.exec();
}
