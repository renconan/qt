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
#include "Curseq.h"
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
extern  float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
extern  unsigned char result[70],receive_data[70];
extern  double out_time,change_timedata;
extern  float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
extern  float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
extern  long auto_i,IRQcnt,auto_count,tice;
extern int   input_par,HelpFlag;
extern int   INPAR,seq_count,SeqStep;
extern int   testcount,time_relay_testcount,auto_end;
extern int   cx,cy,cx_start,cx_mid,cx_end,starttest,low,row,page;
extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;
extern float LogicResolution,SystemFrec;
extern float CurrentValue,VoltageValue,Ferequencer,TripTime;
extern int   exit_par,save_f;
extern int   sys[4];
extern int   TestObject;
extern float CurrentStart,CurrentEnd,CurrentStep,VoltageStart,VoltageEnd,VoltageStep,FrequencyStart,FrequencyEnd,FrequencyStep,PostTime,WaitTime;
extern float I1Value,I2Value,I3Value,I1Phase,I2Phase,I3Phase;
extern float V1Value,V2Value,V3Value,V1Phase,V2Phase,V3Phase;
extern float TripTimeValue[500];
extern int   ResultEndCount,ResultPage,ResultFlag,ResultCount,DisResult,ResultEndPage;
extern int   FreqTriple;
extern  char  V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

extern float V_coarse_step,V_fine_step,I_coarse_step,I_fine_step,F_coarse_step,F_fine_step;

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
    void display_TestResult(int num);
    void stoprun();

signals:
    void   SIG_binarystate(int state);
    void   SIG_dspstate(int state);
    void   SIG_testresult(int num);
    void   SIG_DisplayChangeValue(bool flag,float value);
     void   SIG_SendClose();
      void   SIG_TestState();
      void   SIG_Teststate(int);
};

#endif // THREAD1_H
