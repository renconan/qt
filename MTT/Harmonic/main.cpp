#include <QtGui/QApplication>
#include "harmonic.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Harmonic w;
    w.show();

    return a.exec();
}
