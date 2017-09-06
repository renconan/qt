#ifndef THREAD1_H
#define THREAD1_H
#include <QThread>

#include <QTimer>
#include "Defpam.h"
#include "Config.h"

class thread1 : public QThread
{
    Q_OBJECT
public:
    explicit thread1(QObject *parent = 0);
    void setdcoutput( int);
    int dcouttype;
    void run();



private:
};

#endif // THREAD1_H
