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
#include "Volseq.h"
#include "KEY.h"





int OpenDev(char *Dev);
int set_speed(int fd, int speed);
void temprorarydatainit();
void UartsInit();
void UartInit(int uartno);
int set_Parity(int fd,int databits,char parity,int stopbits);
int UartSend(int fd,char *cmd,int length);
int UartFlush(int fd);



extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
extern float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
extern long auto_i,IRQcnt,auto_count,tice;

//中断程序的变量。

extern int   input_par,HelpFlag;
extern int   INPAR,seq_count,SeqStep;
extern int   testcount,time_relay_testcount,auto_end;
extern int   cx,cy,cx_start,cx_mid,cx_end,starttest,low,row,page;
extern int   V1DC,V2DC,V3DC;
extern int   TestItem1,TestItem2,TestItem3,OutputMode,TestObject;
extern float FixedUnderTime,FixedOverTime;
extern float StartValue,EndValue,ActStep,ReturnStep,StepTime;
extern float TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime1,TripTime2,TripTime3;
extern float Phase,LogicResolution,SystemFrec;
extern float ChangeValue,timeout;


extern int exit_par,save_f;
extern int sys[4];
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

extern float faultduration; //2014-11-13 杜凯

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
    void ReadAutoLogic(int fd); //读开关量状态
    void DisplayResult(int ReslutSum);
    void CaculateError();
    void key_deal_test_voltage(int fd);
    void key_deal_stop_voltage(int fd);
    void stoprun();
signals:
    void    SIG_binarystate(int state);
    void    SIG_dspstate(int state);
    void    SIG_testresult(int num);
    void    SIG_DisplayChangeValue(bool flag,float value);
    void   SIG_SendClose();
    void   SIG_TestState();
    void   SIG_Teststate(int);

};

#endif // THREAD1_H
