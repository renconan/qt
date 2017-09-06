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
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
extern unsigned char result[70],receive_data[70];
extern float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
extern float TripTime1,TripTime2,TripTime3,TripTime4,TripTime5;
extern float ChangeValue,timeout;
extern int save_f;
extern int sys[4];
extern int input_par;
extern int HelpFlag;
extern float LogicResolution,SystemFrec,SystemVolt;//2009-6-20
extern char  V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern  float VzOutput3Uo,VzOutputAc,VzOutputDc;


extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,VzRelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase;


extern int   TestItem1,TestItem2,TestItem3,TestItem4,TestItem5;






class thread1 : public QThread
{
    Q_OBJECT
public:
    explicit thread1(QObject *parent = 0);
    void run();

    int current_pos; //当前位置
    void key_response(KEY key);  //需要更改
    int  Buzzer(char Address1,char Address2,char Address3);
    void SaveReport();

    void logic_info(int fd,char info_result1,char info_result2,char info_result3,char info_result4);
    void key_deal_test_current(int fd);
    void key_deal_stop_current(int fd);
    int  ReadAutoLogic(int fd); //读开关量状态
    void DisplayResult(int ReslutSum);

    void stoprun();

signals:

    void   SIG_SendClose();
   void   SIG_dspstate(int state);
    void   SIG_binarystate(int state);
   void    SIG_testresult(int num);
   void    SIG_DisplayChangeValue(bool flag,float value);
     void   SIG_TestState();
     void   SIG_Teststate(int);
};

#endif // THREAD1_H
