#include <QThread>
#include "KEY.h"
#ifndef THREAD1_H
#define THREAD1_H
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
#include  "Manual/ManSeq.h"
#include <QTimer>
#include "Config.h"
//#include "widget.h"

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision,VzPhaseRevision;
extern float IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;



//input_parΪ�����Ƿ��ڱ༭״̬�ı�־��input_par=0��δ�༭��input_par=1���༭״̬��
//DecimalΪС�������������־��Decimal=0��δ���룻Decimal=1���Ѿ�����С���㡣
//sige_flagΪ���ŵ��������ı�־��sige_flag=0��δ���룻sige_flag=1���Ѿ����븺����



extern float ChangeV1Value,ChangeV1Phase,ChangeV1Frec;
extern float ChangeV2Value,ChangeV2Phase,ChangeV2Frec;
extern float ChangeV3Value,ChangeV3Phase,ChangeV3Frec;
extern float ChangeVzValue,ChangeVzPhase,ChangeVzFrec;
extern float ChangeI1Value,ChangeI1Phase,ChangeI1Frec;
extern float ChangeI2Value,ChangeI2Phase,ChangeI2Frec;
extern float ChangeI3Value,ChangeI3Phase,ChangeI3Frec;




extern float fine_CurStep,fine_VolStep;
extern float fine_PhaStep,fine_FreStep;


extern float TripValue,TripTime;

//extern char ParStr[20];  //ParStr:Ϊ��������ʱ��¼������������ַ���.
extern  int binOut;

extern float faultduration;

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
    void key_response(KEY key);  //��Ҫ����
    int  Buzzer(char Address1,char Address2,char Address3);

    void logic_info(int fd,char info_result1,char info_result2,char info_result3,char info_result4);
    void ReadA_TripTimeLogic(int fd);
    void read_logic(int fd);
    void ReadAutologic(int fd);

    void key_deal_test_manual(int fd);
    void key_deal_stop_manual(int fd);
    int  checkAutoTestEndvalueValid();
    bool ManualOutputFlag;
    int WhichIndex;

    void stoprun();



signals:
    void SIG_showmainface();//��ʾ������
    void   SIG_dspstate(int state);
    void   SIG_binarystate(int state);
    void   SIG_testresult(bool k);
    void   SIG_setVanocheck();
    void   SIG_SendClose();
    void   SIG_Teststate(int);
    void   SIG_SendWarning(int,int);
     void   SIG_TestButtenState();
     void    SIG_DisplayAutoChangeValue(int flag);
     void  SIG_DspInfoState();
    /************

 ����1:
      1 VA
      2 VB
      3 VC
      4 Vz
      5 IA
      6 IB
      7 IC
 ����2��
      1 �������� ���ֵ
      2 �������� 0


      ******************/




protected slots:
    void Manualoutput();
    void onRecevieIndex(int index);
};

#endif // THREAD1_H
