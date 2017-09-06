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

extern  float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;

extern  unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern  int   Decimal,sige_flag;
//input_parΪ�����Ƿ��ڱ༭״̬�ı�־��input_par=0��δ�༭��input_par=1���༭״̬��
//DecimalΪС�������������־��Decimal=0��δ���룻Decimal=1���Ѿ�����С���㡣
//sige_flagΪ���ŵ��������ı�־��sige_flag=0��δ���룻sige_flag=1���Ѿ����븺����
extern  long auto_i,IRQcnt,auto_count,tice;
//�жϳ���ı�����
extern int   INPAR,seq_count,SeqStep;
extern int   testcount,time_relay_testcount,auto_end;
extern int   cx,cy,cx_start,cx_mid,cx_end,increase,low,row,page;
extern int   TestItem1,TestItem3,TestItem2,OutputMode;

extern float StartCurrent,EndCurrent,ActStep,ReturnStep,StepTime,CurCoarseStep;
extern float TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime,ReturnQuotiety;
extern float timeout;
extern int exit_par,save_f;
//exit_par���˳������־��save_f����������־��Ϊ1��ʾ���µĲ��Խ�������Ա��棬Ϊ0��ʾû���µĲ��Խ��
extern int sys[4];

class thread1 : public QThread
{
    Q_OBJECT
public:
    explicit thread1(QObject *parent = 0);
    void run();

    void key_response(KEY key);  //��Ҫ����
    int  Buzzer(char Address1,char Address2,char Address3);
    void logic_info(int fd,char info_result1,char info_result2,char info_result3,char info_result4);
    void ReadTripTimeLogic(int fd);
    double CaculateError();
    void key_deal_test_current(int fd);
    void key_deal_stop_current(int fd);

    void stoprun();



signals:
    void   SIG_binarystate(int state);
    void   SIG_dspstate(int state);
    void   SIG_testresult(float num);
    void   SIG_DisplayChangeValue(int ,float);
     void   SIG_SendClose();
     void   SIG_TestState();
     void   SIG_Teststate(int);
};

#endif // THREAD1_H
