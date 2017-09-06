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
#include <semaphore.h>
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
#include "Zoneseq.h"
#include "KEY.h"






class thread1 : public QThread
{
    Q_OBJECT
public:
    explicit thread1(QObject *parent = 0);
    void run();
    void key_response(KEY key);  //需要更改
    int  Buzzer(char Address1,char Address2,char Address3);
    void logic_info(int fd,char info_result1,char info_result2,char info_result3,char info_result4);
    void ReadTripTimeLogic(int fd);
    void key_deal_test_current(int fd);
    void key_deal_stop_current(int fd);
    void DisplayResult(int ReslutSum);
    int  ErrorWarning(int segment,int fault);

    void CaculatePerfalultPhase();
    float CaculatePhase(float r,float y);


    void LLLNormalFaultPar();
    void LLNormalFaultPar();
    void LNNormalFaultPar();
    int  OutputSignalCompute(int flag,int k);
    void OutSequencer(int SeqCount,int fd);
    void OutputSignal(int fd);
    int  WarningInformation();

    void stoprun();


signals:
     void   SIG_SendClose();
     void   SIG_binarystate(int state);
    void   SIG_dspstate(int state);
    void   SIG_testresult(int num);
    void   SIG_sendwarning(int sectnum,int num); //sectnum段号 发送错误信息 1 电压大于系统电压 2 小于系统电压一半
    void   SIG_changetab(int); //改变显示页
    void   SIG_TestState();
    void   SIG_Teststate(int);
    // zone 哪个段 testpoint第几测试点 测试点的值
    void SIG_SendHeader(QString str);
    void SIG_Sendzoneshow(int i);

};

#endif // THREAD1_H
