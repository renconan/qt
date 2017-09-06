#ifndef THREAD1_H
#define THREAD1_H
#include <QThread>


#ifdef ARMLINUX
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <math.h>
#include <dirent.h>
#include <sys/vfs.h>
#endif
#include <QTimer>
#include "Defpam.h"
#include "Config.h"
#include "Testcom2.h"
#include "Dfdtseq.h"
#include "KEY.h"







class thread1 : public QThread
{
    Q_OBJECT
public:
    explicit thread1(QObject *parent = 0);
    void run();

    int current_pos; //当前位置
    int  Buzzer(char Address1,char Address2,char Address3);
    void logic_info(int fd,char info_result1,char info_result2,char info_result3);
    void key_response(KEY key);  //需要更改
    void CaculateError();
    void key_deal_test_voltage(int fd);
    void key_deal_stop_voltage(int fd);
    void ReadAutoLogic(int fd); //读开关量状态
    void DisplayResult(int ReslutSum);
    void stoprun();


signals:
    void   SIG_dspstate(int state);
    void   SIG_testresult(int num);
    void   SIG_DisplayChangeValue(bool flag,float value);
      void   SIG_SendClose();
       void   SIG_TestState();
};

#endif // THREAD1_H
