#include <QtGui/QApplication>
#include "templateprogram.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Templateprogram w;

    if(argc>1)
    {
    w.settemplatefilename(argv[1]);
    }
    w.show();


    return a.exec();
}
