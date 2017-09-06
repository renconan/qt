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
#include "Defpam.h"
#include "Testcom2.h"
#include  "ManSeq.h"
#include <KEY.h>
#include <QTimer>




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

extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision,VzPhaseRevision;
extern float IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;

extern int   cursorF,cursor,cursorK;
extern int   input_par,Decimal,sige_flag,HelpFlag,T4;
//input_par为参数是否处于编辑状态的标志。input_par=0：未编辑；input_par=1：编辑状态。
//Decimal为小数点的输入与否标志，Decimal=0：未输入；Decimal=1：已经输入小数点。
//sige_flag为负号的输入与否的标志。sige_flag=0：未输入；sige_flag=1：已经输入负数。

extern int   VariatyV1,VariatyV2,VariatyV3,VariatyI1,VariatyI2,VariatyI3,VariatyVz,VariatyVz2;
extern int   VariatyP,Variable,TestMode,INPAR;
extern int   channum,blocking_num,variate_num,testcount,time_relay_testcount,auto_end;
extern int   cx,cy,cx_start,cx_mid,cx_end,increase,starttest,low,row,page;
extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC,VzDC;

extern float V1Value,V1Phase,V1Frec;
extern float V2Value,V2Phase,V2Frec;
extern float V3Value,V3Phase,V3Frec;
extern float I1Value,I1Phase,I1Frec;
extern float I2Value,I2Phase,I2Frec;
extern float I3Value,I3Phase,I3Frec;
extern float VzValue,VzPhase,VzFrec;

extern float ChangeV1Value,ChangeV1Phase,ChangeV1Frec;
extern float ChangeV2Value,ChangeV2Phase,ChangeV2Frec;
extern float ChangeV3Value,ChangeV3Phase,ChangeV3Frec;
extern float ChangeVzValue,ChangeVzPhase,ChangeVzFrec;
extern float ChangeI1Value,ChangeI1Phase,ChangeI1Frec;
extern float ChangeI2Value,ChangeI2Phase,ChangeI2Frec;
extern float ChangeI3Value,ChangeI3Phase,ChangeI3Frec;

extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase,VzRelaPhase;
extern float CurStep,VolStep,PhaStep,FreStep,StepTime,StepNum;


extern float fine_CurStep,fine_VolStep;
extern float fine_PhaStep,fine_FreStep;


extern float PreFaultTime,FaultTime,PostFaultTime;
extern float timeout;
extern float ChangeValue,TripValue,ReturnValue,TripTime;
extern float VaValueReviosn[10],VbValueReviosn[10],VcValueReviosn[10],VzValueReviosn[10];
extern float IaValueReviosn[10],IbValueReviosn[10],IcValueReviosn[10];
extern float V1OutputValue,V2OutputValue,V3OutputValue,VzOutputValue;
extern float I1OutputValue,I2OutputValue,I3OutputValue;

extern int StartTestFlag;
extern int exit_par,save_f;
extern char report_time[30],report_load[30];
extern float save_time;
extern int   sys[4];
extern float logic_delaytime,SysFrec,SysVolt;
extern float  V1PerFaultValue,V2PerFaultValue,V3PerFaultValue,VzPerFaultValue,I1PerFaultValue,I2PerFaultValue,I3PerFaultValue;
extern float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase,VzPerFaultPhase,I1PerFaultPhase,I2PerFaultPhase,I3PerFaultPhase;

extern char ParStr[20];  //ParStr:为参数输入时记录的输入参数的字符串.
extern char V1Output,V2Output,V3Output,VzOutput,I1Output,I2Output,I3Output;


extern float faultduration;

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
    void read_logic(int fd);
    void ReadAutologic(int fd);
    void CaculateError();
    void key_deal_test_manual(int fd);
    void key_deal_stop_manual(int fd);
    int  checkAutoTestEndvalueValid();
    bool ManualOutputFlag;

    void stoprun();

signals:
    void   SIG_dspstate(int state);
    void   SIG_binarystate(int state);
    void   SIG_testresult(bool k);
    void   SIG_setVanocheck();
    void   SIG_SendClose();
    void   SIG_Teststate(int);
    void   SIG_SendWarning(int,int);
     void   SIG_TestState();
     void    SIG_DisplayAutoChangeValue(int flag);
    /************

 参数1:
      1 VA
      2 VB
      3 VC
      4 Vz
      5 IA
      6 IB
      7 IC
 参数2：
      1 超出上线 最大值
      2 低于下限 0


      ******************/




protected slots:
    void Manualoutput();
};

#endif // THREAD1_H
