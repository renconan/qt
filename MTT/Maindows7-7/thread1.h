#ifndef THREAD1_H
#define THREAD1_H
#include <QThread>


#include <QTimer>
#include "Defpam.h"
#include "Config.h"
#include "KEY.h"



class thread1 : public QThread
{
    Q_OBJECT
public:
    explicit thread1(QObject *parent = 0);
    void connectdsp();
    void setcmd(char *cmd);
    char cmdstr[512];
    void WriteSerialNumber(char *s);
    void run();
signals:
    void SIG_showmainface();//显示主界面

private:

};

#endif // THREAD1_H
