#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include "Config.h"
#include <QSemaphore>
#include "Testcom2.h"
#include "thread1.h"
#include "Zoneseq.h"
#include "Gpio.h"
#include "Uartset.h"
#include "em335x_drivers.h"
#include "ComplexCaculate.h"
#include "math.h"
//extern int caculateflag;
QSemaphore sem(1);
TEMPRORARYDATA *temprorarydata;

int info_num=0;

extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
extern float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;

extern int   input_par;
extern long auto_i,IRQcnt,auto_count,tic;
//涓柇绋嬪簭鐨勫彉閲忋€
extern int   seq_count,Sequencer,ResultFlag,DisResult,EndSequencer;
extern int   testcount,time_relay_testcount,auto_end;
extern int   cx,cy,cx_start,cx_mid,cx_end,increase,starttest,low,row,page;
extern int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC,EndTest;
//涓€娈浜屾涓夋 鍥涙
extern bool  Zone1,Zone2,Zone3,Zone4,EqualizeQuotiety;
extern bool  FaultType1,FaultType2,FaultType3,FaultType4,FaultType5,FaultType6,FaultType7;
extern int   VzOutput3U0;
extern float VzOutputAc,VzOutputDc;
extern float LogicResolution,SystemFrec,SystemVolt;
//闃绘姉瀹氬€
extern float Impedance1,Impedance2,Impedance3,Impedance4;
//鐏垫晱瑙
extern float DelicacyAngle1,DelicacyAngle2,DelicacyAngle3,DelicacyAngle4;
extern float FixupCurrent1,FixupCurrent2,FixupCurrent3,FixupCurrent4;
extern float FixupTime1,FixupTime2,FixupTime3,FixupTime4;
extern float EqualizeValue,TestPoint1,TestPoint2,TestPoint3;
//EqualizeValue 闆跺簭琛ュ伩绯绘暟
extern float PerFaultTime,PostFaultTime,FaultTime;
extern float ChangeValue,timeout,TripTime,TripTimeSum[63];
extern int   exit_par,save_f;
//exit_par锛氶€€鍑虹▼搴忔爣蹇椼€俿ave_f锛氫繚瀛樼粨鏋滄爣蹇楋紝涓琛ㄧず鏈夋柊鐨勬祴璇曠粨鏋滐紝鍙互淇濆瓨锛屼负0琛ㄧず娌℃湁鏂扮殑娴嬭瘯缁撴灉
extern char  report_time[30],report_load[30];
extern float save_time;
extern int   sys[4];

extern float  V1PerFaultValue,V2PerFaultValue,V3PerFaultValue,I1PerFaultValue,I2PerFaultValue,I3PerFaultValue;
extern float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase,I1PerFaultPhase,I2PerFaultPhase,I3PerFaultPhase;
extern float  V1FaultValue,V2FaultValue,V3FaultValue,I1FaultValue,I2FaultValue,I3FaultValue;
extern float  V1FaultPhase,V2FaultPhase,V3FaultPhase,I1FaultPhase,I2FaultPhase,I3FaultPhase;
extern float  V1PostFaultValue,V2PostFaultValue,V3PostFaultValue,I1PostFaultValue,I2PostFaultValue,I3PostFaultValue;
extern float  V1PostFaultPhase,V2PostFaultPhase,V3PostFaultPhase,I1PostFaultPhase,I2PostFaultPhase,I3PostFaultPhase;
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

extern float V_coarse_step,V_fine_step,I_coarse_step,I_fine_step,F_coarse_step,F_fine_step;
extern float R_coarse_step,R_fine_step,P_coarse_step,P_fine_step;
extern int HelpFlag;
extern int showpage;

extern bool Busvolcon;

extern float KLphase;

extern bool Threadrunstate;

extern bool SoftStartEndflag;

extern  int   dcouttype;

extern bool  istransfer;
#define LEDNUM GPIO8
thread1::thread1(QObject *parent) :
    QThread(parent)
{

    temprorarydata=new TEMPRORARYDATA;

}



int thread1::ErrorWarning(int segment,int fault)
      {
        switch(segment)
          {
            case 1://1娈
              switch(fault)
                {
                  case 1://鏁呴殰绫诲瀷涓猴細A-E
                    if(V1FaultValue>SystemVolt)
                      {
                       //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇澶т簬SystemVolt 璇锋敼鍙樼數娴佸€
                        emit SIG_sendwarning(segment,1);
                        return 1;
                       }
                    break;
                  case 2://鏁呴殰绫诲瀷涓猴細B-E
                    if(V2FaultValue>SystemVolt)
                      {
                        //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇澶т簬SystemVolt 璇锋敼鍙樼數娴佸€
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    break;
                  case 3: //鏁呴殰绫诲瀷涓猴細C-E
                    if(V3FaultValue>SystemVolt)
                      {
                         //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇澶т簬SystemVolt 璇锋敼鍙樼數娴佸€
                         emit SIG_sendwarning(segment,1);
                        return 1;
                      }

                    break;
                  case 4://鏁呴殰绫诲瀷涓猴細A-B
                    if(V1FaultValue>SystemVolt)
                      {
                         emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V1FaultValue<SystemVolt/2)
                      {
                        //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇灏忎簬SystemVolt/2 璇锋敼鍙樼數娴佸€
                         emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    break;
                  case 5://鏁呴殰绫诲瀷涓猴細B-C
                    if(V2FaultValue>SystemVolt)
                      {//鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇澶т簬SystemVolt 璇锋敼鍙樼數娴佸€
                         emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V2FaultValue<SystemVolt/2)
                      {
                        //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇灏忎簬SystemVolt/2 璇锋敼鍙樼數娴佸€
                         emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 6://鏁呴殰绫诲瀷涓猴細C-A
                    if(V3FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V3FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         emit SIG_sendwarning(segment,2);
                        return 1;
                      }

                    break;
                  case 7://鏁呴殰绫诲瀷涓猴細A-B-C
                    if(V1FaultValue>SystemVolt)
                      {//涓夌浉鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                       emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    break;
                }
              break;
            case 2://2娈
              switch(fault)
                {
                  case 1://鏁呴殰绫诲瀷涓猴細A-E
                    if(V1FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }

                    break;
                  case 2://鏁呴殰绫诲瀷涓猴細B-E
                    if(V2FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 3://鏁呴殰绫诲瀷涓猴細C-E
                    if(V3FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 4://鏁呴殰绫诲瀷涓猴細A-B
                    if(V1FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                       emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V1FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    break;
                  case 5://鏁呴殰绫诲瀷涓猴細B-C
                    if(V2FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                       emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V2FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 6://鏁呴殰绫诲瀷涓猴細C-A
                    if(V3FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                       emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V3FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                   /* else
                      return 0;*/
                    break;
                  case 7://鏁呴殰绫诲瀷涓猴細A-B-C
                    if(V1FaultValue>SystemVolt)
                      {//涓夌浉鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                       emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                }
              break;
            case 3://3娈
              switch(fault)
                {
                  case 1://鏁呴殰绫诲瀷涓猴細A-E
                    if(V1FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                   /* else
                      return 0;*/
                    break;
                  case 2://鏁呴殰绫诲瀷涓猴細B-E
                    if(V2FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 3://鏁呴殰绫诲瀷涓猴細C-E
                    if(V3FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                   /* else
                      return 0;*/
                    break;
                  case 4://鏁呴殰绫诲瀷涓猴細A-B
                    if(V1FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V1FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 5://鏁呴殰绫诲瀷涓猴細B-C
                    if(V2FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V2FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 6://鏁呴殰绫诲瀷涓猴細C-A
                    if(V3FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V3FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 7://鏁呴殰绫诲瀷涓猴細A-B-C
                    if(V1FaultValue>SystemVolt)
                      {//涓夌浉鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                         emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    break;
                }
              break;
            case 4://4娈
              switch(fault)
                {
                  case 1://鏁呴殰绫诲瀷涓猴細A-E
                    if(V1FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 2://鏁呴殰绫诲瀷涓猴細B-E
                    if(V2FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                         emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 3://鏁呴殰绫诲瀷涓猴細C-E
                    if(V3FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                         emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 4://鏁呴殰绫诲瀷涓猴細A-B
                    if(V1FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V1FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 5://鏁呴殰绫诲瀷涓猴細B-C
                    if(V2FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V2FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 6://鏁呴殰绫诲瀷涓猴細C-A
                    if(V3FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    if(V3FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         emit SIG_sendwarning(segment,2);
                        return 1;
                      }
                    break;
                  case 7://鏁呴殰绫诲瀷涓猴細A-B-C
                    if(V1FaultValue>SystemVolt)
                      {//涓夌浉鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                         emit SIG_sendwarning(segment,1);
                        return 1;
                      }
                    break;
                }
              break;
          }
        return 0;
      }

void thread1::DisplayResult(int ResultSum)
{

  emit  SIG_testresult(ResultSum);

}

void thread1::ReadTripTimeLogic(int fd)
{

    int t3=0,t4=0,t5=0;
    unsigned char signature;
    double u,v,w;
    float  Temp;
    static double SequencerTime2,SequencerTime4;
    artExec(fd,"inp:buf:get?",result,15);//浠嶥SP寰楀埌寮€鍏抽噺鎺ョ偣淇℃伅銆
    u=result[7];
    v=result[8];
    w=result[9];
    signature=int(result[13]&0x7f);
    switch(signature)
      {
        case 1:
          break;
        case 2:
          SequencerTime2=gettime(u,v,w);//璁＄畻浠庡紑濮嬫祴璇曞埌sequencer鐨凷tep2鐨勬椂闂淬€
          break;
        case 3:
          t3=1;
          break;
        case 4:
          t4=1;
          SequencerTime4=gettime(u,v,w);//璁＄畻浠庡紑濮嬫祴璇曞埌sequencer鐨凷tep4鐨勬椂闂淬€
          break;
        case 5:
          t5=1;
          break;
      }
    if(t3==1&&ResultFlag==0)
      {//鏈姩浣

        ResultFlag=1;
        TripTime=0;

      }
    if(t4==1&&ResultFlag==0)
      {//鍔ㄤ綔
        ResultFlag=1;
        TripTime=SequencerTime4-SequencerTime2-LogicResolution/1000;
        Temp=TripTime*1000;
        Temp=ceil(Temp);
        TripTime=Temp/1000;//灏嗗姩浣滄椂闂寸殑闄愬埗鍦ㄥ皬鏁扮偣鍚庢渶澶氬彧鏈変笁浣

      }

    if(t5==1)
      {//鍔ㄤ綔

        switch(Sequencer)
                 {
                   case 1:
                     TripTimeSum[0]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(1);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 2:
                     TripTimeSum[1]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(2);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 3:
                     TripTimeSum[2]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(3);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 4:
                     TripTimeSum[3]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(4);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 5:
                     TripTimeSum[4]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(5);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 6:
                     TripTimeSum[5]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(6);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 7:
                     TripTimeSum[6]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(7);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 8:
                     TripTimeSum[7]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(8);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 9:
                     TripTimeSum[8]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(9);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 10:
                     TripTimeSum[9]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(10);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 11:
                     TripTimeSum[10]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(11);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 12:
                     TripTimeSum[11]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(12);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 13:
                     TripTimeSum[12]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(13);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 14:
                     TripTimeSum[13]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(14);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 15:
                     TripTimeSum[14]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(15);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 16:
                     TripTimeSum[15]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(16);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 17:
                     TripTimeSum[16]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(17);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 18:
                     TripTimeSum[17]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(18);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 19:
                     TripTimeSum[18]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(19);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 20:
                     TripTimeSum[19]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(20);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 21:
                     TripTimeSum[20]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(21);//鏄剧ず娴嬭瘯缁撴灉
                     DisResult=0;
                     break;
                   case 22:
                     if(Zone1!=0)
                       DisResult=0;
                     TripTimeSum[21]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(22);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 23:
                     TripTimeSum[22]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(23);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 24:
                     TripTimeSum[23]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(24);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 25:
                     TripTimeSum[24]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(25);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 26:
                     TripTimeSum[25]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(26);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 27:
                     TripTimeSum[26]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(27);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 28:
                     TripTimeSum[27]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(28);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 29:
                     TripTimeSum[28]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(29);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 30:
                     TripTimeSum[29]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(30);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 31:
                     TripTimeSum[30]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(31);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 32:
                     TripTimeSum[31]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(32);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 33:
                     TripTimeSum[32]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(33);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 34:
                     TripTimeSum[33]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(34);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 35:
                     TripTimeSum[34]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(35);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 36:
                     TripTimeSum[35]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(36);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 37:
                     TripTimeSum[36]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(37);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 38:
                     TripTimeSum[37]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(38);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 39:
                     TripTimeSum[38]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(39);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 40:
                     TripTimeSum[39]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(40);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 41:
                     TripTimeSum[40]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(41);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 42:
                     TripTimeSum[41]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(42);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 43:
                     TripTimeSum[42]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(43);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 44:
                     TripTimeSum[43]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(44);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 45:
                     TripTimeSum[44]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(45);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 46:
                     TripTimeSum[45]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(46);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 47:
                     TripTimeSum[46]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(47);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 48:
                     TripTimeSum[47]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(48);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 49:
                     TripTimeSum[48]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(49);//鏄剧ず娴嬭瘯缁撴灉
                     DisResult=0;
                     break;

                   case 50:
                     if(Zone1!=0||Zone2!=0)
                       DisResult=0;
                     TripTimeSum[49]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(50);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 51:
                     TripTimeSum[50]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(51);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 52:
                     TripTimeSum[51]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(52);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 53:
                     TripTimeSum[52]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(53);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 54:
                     TripTimeSum[53]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(54);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 55:
                     TripTimeSum[54]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(55);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 56:
                     TripTimeSum[55]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(56);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 57:
                     TripTimeSum[56]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(57);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 58:
                     TripTimeSum[57]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(58);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 59:
                     TripTimeSum[58]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(59);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 60:
                     TripTimeSum[59]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(60);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 61:
                     TripTimeSum[60]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(61);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 62:
                     TripTimeSum[61]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(62);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                   case 63:
                     TripTimeSum[62]=TripTime;//TripTimeSum鏄祴璇曠粨鏋滀繚瀛樼殑鏁扮粍
                     DisplayResult(63);//鏄剧ず娴嬭瘯缁撴灉
                     break;
                 }

        ResultFlag=0;
        output_signal_end(fd);
        if(Sequencer<EndSequencer)  //杈撳嚭涓嬩竴涓猻equencer鐨勫€
          {

            OutSequencer(Sequencer,fd);
          }
        else
          {
            GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //鐏
            //ClearLED();
            starttest=0;
            //caculateflag=0;
            EndTest=1;
            Sequencer=1;
            save_f=1;
          }
          }
}


void thread1::key_deal_test_current(int fd)
{
    int i;
    if(starttest==0)
          {
            emit SIG_Teststate(1);
             //SetLED();
              GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
            if(WarningInformation())
              {
                //ClearLED();
                GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                artExec(fd,"out:dig:off(0x74)",result,0);
                return;
              }

            starttest=1;
            for(i=0;i<63;i++)
                TripTimeSum[i]=-1;
            OutputSignal(fd);
            emit SIG_testresult(0);

            DisResult=1;
            EndTest=0;
            emit SIG_changetab(2);

          }
}

void thread1::key_deal_stop_current(int fd)
{
        if(starttest==1)
         {
           output_signal_end(fd);
           starttest=0;
           ResultFlag=0;
         }
}


void thread1::key_response(KEY key)
{

if(key==_NOP)   return;

if(key==_TEST)
{
     if(starttest==0)
     {
        output_signal_end(temprorarydata->UartDev[0].uartfd);
        key_deal_test_current(temprorarydata->UartDev[0].uartfd);
        emit SIG_TestState();
     }
}
else if(key==_STOP)
{
      GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
      //ClearLED();
      key_deal_stop_current(temprorarydata->UartDev[0].uartfd);
      emit SIG_TestState();
}
}



void thread1::stoprun(){

    Threadrunstate=false;
}


void thread1::run()
{
    KEY key;
    int rc;
    unsigned char info_flag1,info_flag2,info_flag3,info_flag4;
    temprorarydatainit();
    UartsInit();

    temprorarydata->gpio_fd= open("/dev/em335x_gpio", O_RDWR);
    printf("open file = %d\n", temprorarydata->gpio_fd);
    rc = GPIO_OutEnable(temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
    if(rc < 0)
    {
       printf("GPIO_OutEnable::failed %d\n", rc);
       temprorarydata->gpio_fd=-1;
    }



 // GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
  //ClearLED();
  online(temprorarydata->UartDev[0].uartfd);



  read_max(temprorarydata->UartDev[0].uartfd);
  artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);
  if((result[7]&0xf0)==0x60)
       { //125V鐢靛帇鏉
         MAX_V_VALUEDATA_AC=300;
         MIN_V_VALUEDATA_AC=0;
         MAX_V_VALUEDATA_DC=424;
         MIN_V_VALUEDATA_DC=-424;
       }
      else if((result[7]&0xf0)==0x80)
        { //125V鐢靛帇鏉
          MAX_V_VALUEDATA_AC=125;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=176;
          MIN_V_VALUEDATA_DC=-176;
        }
      else if((result[7]&0xf0)==0x00)
        { //75V鐢靛帇鏉
          MAX_V_VALUEDATA_AC=75;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=110;
          MIN_V_VALUEDATA_DC=-110;
        }
      if((result[7]&0x0f)==0x01)
        { //30A鐢垫祦鏉
          MAX_I_VALUEDATA_AC=30;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=42;
          MIN_I_VALUEDATA_DC=-42;
        }
      else if((result[7]&0x0f)==0x08)
        { //40A鐢垫祦鏉
          MAX_I_VALUEDATA_AC=40;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=56;
          MIN_I_VALUEDATA_DC=-56;
        }
      else if((result[7]&0x0f)==0x05)
        { //50A鐢垫祦鏉
          MAX_I_VALUEDATA_AC=50;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=70;
          MIN_I_VALUEDATA_DC=-70;
        }

     while(Threadrunstate)
      {
     if(temprorarydata->keypadopenflag==false&&temprorarydata->helpopenflag==false)
     {
     key=GetKey(temprorarydata->UartDev[1].uartfd);
     key_response(key);
        if(SoftStartEndflag==true)
          {
             if(starttest==1)
             {
                 key=_STOP;
             }
             else
             {
                 key=_TEST;
             }
             key_response(key);
             SoftStartEndflag=false;
          }


     if(starttest==0)
     {
       if(info_num++>10)
       {
         emit    SIG_TestState();
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //涓嶉渶瑕佹洿鏀
         info_flag1=result[8];
         info_flag4=result[9];
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
         info_flag2=result[8];
         info_flag3=result[9];
         logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //闇€瑕乪mit
         info_num=0;
       }
     }
     else {
         ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);

         if(info_num++>10)
         {
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //涓嶉渶瑕佹洿鏀
          info_flag1=result[8];
          info_flag4=result[9];
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
          info_flag2=result[8];
          info_flag3=result[9];
          logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //闇€瑕乪mit
          info_num=0;
          }}}
     else
     {


         tcflush(temprorarydata->UartDev[1].uartfd, TCIOFLUSH);
         msleep(500);

     }
}


     starttest=0;
    //caculateflag=0;
          output_signal_end(temprorarydata->UartDev[0].uartfd);
          artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
          close(temprorarydata->UartDev[1].uartfd);
          close(temprorarydata->UartDev[0].uartfd);
          emit SIG_SendClose();
                msleep(100);
}




int  thread1::Buzzer(char Address1,char Address2,char Address3)
  {//sys[1]:杩囪浇锛宻ys[2]:闂存瓏锛宻ys[3]:闂攣
    if(sys[1]==1)
      {//杩囪浇淇℃伅
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1杩囪浇
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2杩囪浇
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3杩囪浇
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1杩囪浇
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2杩囪浇
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3杩囪浇
        if((Address3&0x20)!=0x00)
          return 1;//鐩存祦杩囪浇
      }
    else if(sys[1]==0)
      {//杩囪浇淇℃伅
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1杩囪浇
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2杩囪浇
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3杩囪浇
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1杩囪浇
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2杩囪浇
        if((Address3&0x40)!=0x00&&V3Output==1)
          return -1;//V3杩囪浇
        if((Address3&0x20)!=0x00)
          return 1;//鐩存祦杩囪浇
      }
    if(sys[2]==1)
      {//闂存瓏淇℃伅
        if((Address1&0x10)!=0x00)
          return 1;//I1闂存瓏
        if((Address1&0x04)!=0x00)
          return 1;//I2闂存瓏
        if((Address1&0x01)!=0x00)
          return 1;//I3闂存瓏
        if((Address2&0x08)!=0x00)
          return 1;//V1闂存瓏
        if((Address2&0x04)!=0x00)
          return 1;//V2闂存瓏
        if((Address3&0x80)!=0x00)
          return 1;//V3闂存瓏
      }
    else if(sys[2]==0)
      {//闂存瓏淇℃伅
        if((Address1&0x10)!=0x00)
          return -1;//I1闂存瓏
        if((Address1&0x04)!=0x00)
          return -1;//I2闂存瓏
        if((Address1&0x01)!=0x00)
          return -1;//I3闂存瓏
        if((Address2&0x08)!=0x00)
          return -1;//V1闂存瓏
        if((Address2&0x04)!=0x00)
          return -1;//V2闂存瓏
        if((Address3&0x80)!=0x00)
          return -1;//V3闂存瓏
      }
    if(sys[3]==1)
      {//闂攣淇℃伅
        if((Address1&0x80)!=0x00)
          return 1;//I1闂攣
        if((Address1&0x40)!=0x00)
          return 1;//I2闂攣
        if((Address1&0x20)!=0x00)
          return 1;//I3闂攣
        if((Address2&0x20)!=0x00)
          return 1;//V1闂攣
        if((Address2&0x40)!=0x00)
          return 1;//V2闂攣
        if((Address2&0x01)!=0x00)
          return 1;//V3闂攣
      }
    else if(sys[3]==0)
      {//闂攣淇℃伅
        if((Address1&0x80)!=0x00)
          return -1;//I1闂攣
        if((Address1&0x40)!=0x00)
          return -1;//I2闂攣
        if((Address1&0x20)!=0x00)
          return -1;//I3闂攣
        if((Address2&0x20)!=0x00)
          return -1;//V1闂攣
        if((Address2&0x40)!=0x00)
          return -1;//V2闂攣
        if((Address2&0x01)!=0x00)
          return -1;//V3闂攣
      }
    return 0;
  }


void thread1::logic_info(int fd,char info_result1,char info_result2,char info_result3,char info_result4)
  {

    static int info_count=1;
    int  k1=0;

    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);
    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //涓嶉渶瑕佹帶浠
          artExec(fd,"out:dig:on(0x80)",result,0);
        else
          artExec(fd,"out:dig:off(0x80)",result,0);
      }

    if((info_result4&0x0F)==0x00)
    {
        emit SIG_dspstate(100);
    }
    else
    {
        if((info_result4&0x01)==0x01)
        {
            emit SIG_dspstate(101);
        }
        else if((info_result4&0x01)==0x00)
        {
            emit SIG_dspstate(111);
        }
        if((info_result4&0x02)==0x02)
        {
            emit SIG_dspstate(102);
        }
        else if((info_result4&0x02)==0x00)
        {
            emit SIG_dspstate(112);
        }
        if((info_result4&0x04)==0x04)
        {
            emit SIG_dspstate(103);
        }
        else if((info_result4&0x04)==0x00)
        {
            emit SIG_dspstate(113);
        }
        if((info_result4&0x08)==0x08)
        {
            emit SIG_dspstate(104);
        }
        else if((info_result4&0x08)==0x00)
        {
            emit SIG_dspstate(114);
        }
    }
    if(Buzzer(info_result1,info_result2,info_result3)==0) //涓嶉渶瑕佹帶浠
      {
        //杩愯姝ｅ父
          emit SIG_dspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1鏄1璺湴鍧€鐨凞B9~DB15銆
           emit SIG_dspstate(1);
            //IA杩囪浇
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
            emit SIG_dspstate(2);

            //IABC 娓╁害淇濇姢
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //IC闂存瓏
              emit SIG_dspstate(3);
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==2)
      {
        if((info_result1&0x02)==0x02&&I2Output==1)
          {  //info_result1鏄1璺湴鍧€鐨凞B9~DB15銆

              emit SIG_dspstate(4);
            //IB杩囪浇
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1鏄1璺湴鍧€鐨凞B8~DB15銆

              emit SIG_dspstate(5);
           //IC闂攣
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1鏄1璺湴鍧€鐨凞B9~DB15銆
              emit SIG_dspstate(6);
          //IA闂存瓏
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==3)
      {
        if((info_result2&0x80)==0x80&&I3Output==1)
          { //info_result2鏄2璺湴鍧€鐨凞B9~DB15銆
           //IC杩囪浇
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1鏄1璺湴鍧€鐨凞B8~DB15銆
          //IABC 鐢垫簮淇濇姢
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1鏄1璺湴鍧€鐨凞B9~DB15銆
              emit SIG_dspstate(9);
          //I2闂存瓏  ?娓╁害淇濇姢闂存瓏
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==4)
      {
        if((info_result2&0x10)==0x10&&V1Output==1)
          {//info_result2鏄2璺湴鍧€鐨凞B9~DB15銆
           //VA杩囪浇
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2鏄2璺湴鍧€鐨凞B8~DB15銆
              emit SIG_dspstate(11);
              //V2闂攣
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3鏄2璺湴鍧€鐨凞B0~DB7銆

              //V3闂存瓏
              emit SIG_dspstate(12);
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==5)
      {
        if((info_result2&0x02)==0x02&&V2Output==1)
          {//info_result2鏄2璺湴鍧€鐨凞B9~DB15銆
            //V2杩囪浇
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2鏄2璺湴鍧€鐨凞B8~DB15銆 //DB8
           // VABC 娓╁害淇濇姢
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2鏄2璺湴鍧€鐨凞B9~DB15銆
            emit SIG_dspstate(15);
           //VA闂存瓏
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }

    else if(info_count==6)
      {
        if((info_result3&0x40)==0x40&&V3Output==1)
          { //info_result3鏄2璺湴鍧€鐨凞B0~DB7銆

             emit SIG_dspstate(16);
            //VC杩囪浇
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2鏄2璺湴鍧€鐨凞B8~DB15銆 //DB13
            emit SIG_dspstate(17);
            //VABC鐢垫簮淇濇姢
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2鏄2璺湴鍧€鐨凞B9~DB15銆
            emit SIG_dspstate(18);


          //Vz杩囪浇

            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==7)
      {
        if((info_result3&0x20)==0x20)
          {//info_result3鏄2璺湴鍧€鐨凞B0~DB7銆
            emit SIG_dspstate(19);

             //鐩存祦杩囪浇
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2鏄2璺湴鍧€鐨凞B8~DB15銆
          emit SIG_dspstate(20);

            // VABC 娓╁害淇濇姢
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3鏄2璺湴鍧€鐨凞B0~DB7銆
            emit SIG_dspstate(21);
           //V3闂存瓏
            k1=1;
          }
        if(k1==1)
          {
            info_count=1;
            return;
          }
        else
          info_count=1;
      }

  }


void thread1::LLLNormalFaultPar()
  {//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
    V1PerFaultValue=SystemVolt;//SystemVolt;  //2009-6-20
    V2PerFaultValue=SystemVolt;//SystemVolt;
    V3PerFaultValue=SystemVolt;//SystemVolt;
    I1PerFaultValue=0;
    I2PerFaultValue=0;
    I3PerFaultValue=0;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //鏁呴殰鍓嶇數鍘嬫瘡鐩歌緭鍑篠ystemVoltV锛岀浉浣嶅绉帮紝鐢垫祦杈撳嚭涓銆

    V1PostFaultValue=SystemVolt;//SystemVolt;
    V2PostFaultValue=SystemVolt;//SystemVolt;
    V3PostFaultValue=SystemVolt;//SystemVolt;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;
    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //鏁呴殰鍚庣數鍘嬫瘡鐩歌緭鍑篠ystemVoltV锛岀浉浣嶅绉帮紝鐢垫祦杈撳嚭涓銆
  }


void thread1::LLNormalFaultPar()
  {//涓ょ浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
    V1PerFaultValue=SystemVolt;//SystemVolt;
    V2PerFaultValue=SystemVolt;//SystemVolt;
    V3PerFaultValue=SystemVolt;//SystemVolt;
    I1PerFaultValue=0;
    I2PerFaultValue=0;
    I3PerFaultValue=0;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;

    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //鏁呴殰鍓嶇數鍘嬫瘡鐩歌緭鍑篠ystemVoltV锛岀浉浣嶅绉帮紝鐢垫祦杈撳嚭涓銆

    V1PostFaultValue=SystemVolt;//SystemVolt;
    V2PostFaultValue=SystemVolt;//SystemVolt;
    V3PostFaultValue=SystemVolt;//SystemVolt;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;

    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //鏁呴殰鍚庣數鍘嬫瘡鐩歌緭鍑篠ystemVoltV锛岀浉浣嶅绉帮紝鐢垫祦杈撳嚭涓銆
  }


void thread1::LNNormalFaultPar()
  {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
    V1PerFaultValue=SystemVolt;//SystemVolt;
    V2PerFaultValue=SystemVolt;//SystemVolt;
    V3PerFaultValue=SystemVolt;//SystemVolt;
    I1PerFaultValue=0;
    I2PerFaultValue=0;
    I3PerFaultValue=0;

    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //鏁呴殰鍓嶇數鍘嬫瘡鐩歌緭鍑篠ystemVoltV锛岀浉浣嶅绉帮紝鐢垫祦杈撳嚭涓銆





    V1PostFaultValue=SystemVolt;//SystemVolt;
    V2PostFaultValue=SystemVolt;//SystemVolt;
    V3PostFaultValue=SystemVolt;//SystemVolt;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;

    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //鏁呴殰鍚庣數鍘嬫瘡鐩歌緭鍑篠ystemVoltV锛岀浉浣嶅绉帮紝鐢垫祦杈撳嚭涓銆
  }




extern bool checkiszero(float x);


float thread1::CaculatePhase(float y,float r)
{
   if(checkiszero(r)==true&&checkiszero(y)==true)
       return 0;
   else if(r>0&&checkiszero(y)==true)
       return 0;
   else if(r<0&&checkiszero(y)==true)
       return 180;
   else if(checkiszero(r)==true&&y>0)
       return 90;
   else if(checkiszero(r)==true&&y<0)
       return -90;

   float angle=atan(y/r)*180/3.1415;
   if(r<0)
    angle=angle+180;

   if(angle>180)
       angle=angle-360;
   else  if(angle<-180)
       angle=angle+360;

    return angle;
}



void thread1::CaculatePerfalultPhase()
{

    V1PerFaultPhase=V1FaultPhase-360*SystemFrec*2*PerFaultTime;
    V2PerFaultPhase=V2FaultPhase-360*SystemFrec*2*PerFaultTime;
    V3PerFaultPhase=V3FaultPhase-360*SystemFrec*2*PerFaultTime;
    V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
    V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
    V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;

    I1PerFaultPhase=I1FaultPhase-360*SystemFrec*2*PerFaultTime;
    I2PerFaultPhase=I2FaultPhase-360*SystemFrec*2*PerFaultTime;
    I3PerFaultPhase=I3FaultPhase-360*SystemFrec*2*PerFaultTime;
    I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
    I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
    I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;


    if(V1PerFaultPhase>180)
        V1PerFaultPhase=V1PerFaultPhase-360;
    else if(V1PerFaultPhase<-180)
        V1PerFaultPhase=V1PerFaultPhase+360;

    if(V2PerFaultPhase>180)
        V2PerFaultPhase=V2PerFaultPhase-360;
    else if(V2PerFaultPhase<-180)
        V2PerFaultPhase=V2PerFaultPhase+360;

    if(V3PerFaultPhase>180)
        V3PerFaultPhase=V3PerFaultPhase-360;
    else if(V3PerFaultPhase<-180)
        V3PerFaultPhase=V3PerFaultPhase+360;

    if(I1PerFaultPhase>180)
        I1PerFaultPhase=I1PerFaultPhase-360;
    else if(I1PerFaultPhase<-180)
        I1PerFaultPhase=I1PerFaultPhase+360;

    if(I2PerFaultPhase>180)
        I2PerFaultPhase=I2PerFaultPhase-360;
    else if(I2PerFaultPhase<-180)
        I2PerFaultPhase=I2PerFaultPhase+360;

    if(I3PerFaultPhase>180)
        I3PerFaultPhase=I3PerFaultPhase-360;
    else if(I3PerFaultPhase<-180)
        I3PerFaultPhase=I3PerFaultPhase+360;

    if(I1FaultPhase>180)
        I1FaultPhase=I1FaultPhase-360;
    else if(I1FaultPhase<-180)
        I1FaultPhase=I1FaultPhase+360;

    if(I2FaultPhase>180)
        I2FaultPhase=I2FaultPhase-360;
    else if(I2FaultPhase<-180)
        I2FaultPhase=I2FaultPhase+360;

    if(I3FaultPhase>180)
        I3FaultPhase=I3FaultPhase-360;
    else if(I3FaultPhase<-180)
        I3FaultPhase=I3FaultPhase+360;
}



int thread1::OutputSignalCompute(int flag,int k)
  {
    float Impedance; //Impedance 闃绘姉
    int   ErrorFlag;

    COMPLEX C;
    C.x=3*EqualizeValue*cos(KLphase*3.1415/180)+2;
    C.y=3*EqualizeValue*sin(KLphase*3.1415/180);
    float Angel_C=CaculatePhase(C.y,C.x);
    if(Angel_C>=180)
        Angel_C-=360;
    else if(Angel_C<=-180)
      Angel_C+=360;





    switch(flag)
     {

     case 1:  //1段的A-E故障
       LNNormalFaultPar();//故障前和故障后参数电压都为系统电压，电流为0
       V1Output=1;
       V2Output=1;
       V3Output=1;
       I1Output=1;
       I2Output=0;
       I3Output=0;

          //不同测试点阻抗不一样
       if(k==1)
         Impedance=Impedance1*TestPoint1;
       else if(k==0)
         Impedance=Impedance1*TestPoint2;
       else if(k==-1)
         Impedance=Impedance1*TestPoint3;

       I1FaultValue=FixupCurrent1;
       if(Busvolcon==true)
     {  //调用出错处理程序，如果返回1，说明参数设置不对。

       COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
       V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
       V2FaultValue=SystemVolt;//SystemVolt;
       V3FaultValue=SystemVolt;//SystemVolt;
       I2FaultValue=0;
       I3FaultValue=0;
       //单相接地故障时，未发生故障的电压为Vnom（SystemVolt），未发生故障的电流为0。
       V1FaultPhase=CaculatePhase(Va.y,Va.x);
       V2FaultPhase=-120+V1FaultPhase;
       V3FaultPhase=120+V1FaultPhase;
       //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
     }
     else
       {

        I2FaultValue=0;
        I3FaultValue=0;

        COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
        COMPLEX Vb=VbCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
        COMPLEX Vc=VcCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);


        V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
        V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
        V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);

        V1FaultPhase=CaculatePhase(Va.y,Va.x);
        V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
        V3FaultPhase=CaculatePhase(Vc.y,Vc.x);


        V1FaultPhase=fmod(V1FaultPhase,360);
        V2FaultPhase=fmod(V2FaultPhase,360);
        V3FaultPhase=fmod(V3FaultPhase,360);
       }

       ErrorFlag=ErrorWarning(1,1);
       if(ErrorFlag==1)
       return 1;


       I1FaultPhase=0-DelicacyAngle1-Angel_C;
       if(k==-1)
       I1FaultPhase=I1FaultPhase-180;


       I2FaultPhase=I1FaultPhase-120;
       I3FaultPhase=I1FaultPhase+120;
       LNNormalFaultPar();//故障前和故障后参数
       //return 0;
      break;
    case 2://1段的B-E故障
      LNNormalFaultPar();//故障前和故障后参数
      V1Output=1;
      V2Output=1;
      V3Output=1;
      I1Output=0;
      I2Output=1;
      I3Output=0;

      if(k==1)
        Impedance=Impedance1*TestPoint1;
      else if(k==0)
        Impedance=Impedance1*TestPoint2;
      else if(k==-1)
        Impedance=Impedance1*TestPoint3;
      I2FaultValue=FixupCurrent1;
      //单相接地故障时，电流故障相I=I(固定电流)
      if(Busvolcon==true)
     {
      COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
      V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
      V1FaultValue=SystemVolt;//SystemVolt;
      V3FaultValue=SystemVolt;//SystemVolt;
      I1FaultValue=0;
      I3FaultValue=0;

      V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
      V1FaultPhase=V2FaultPhase+120;
      V3FaultPhase=V2FaultPhase-120;
     }
     else
      {
          I1FaultValue=0;
          I3FaultValue=0;
          COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
          COMPLEX Vc=VbCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
          COMPLEX Va=VcCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);

          V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
          V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
          V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
          V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
          V1FaultPhase=CaculatePhase(Va.y,Va.x);
          V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
          V1FaultPhase=fmod(V1FaultPhase,360);
          V2FaultPhase=fmod(V2FaultPhase,360);
          V3FaultPhase=fmod(V3FaultPhase,360);

      }

      ErrorFlag=ErrorWarning(1,2);
      if(ErrorFlag==1)
      return 1;

      I2FaultPhase=0-DelicacyAngle1-Angel_C;
      if(k==-1)
        I2FaultPhase=I2FaultPhase-180;
      I1FaultPhase=I2FaultPhase+120;
      I3FaultPhase=I2FaultPhase-120;
      LNNormalFaultPar();//故障前和故障后参数
      //return 0;
      break;
    case 3://1段的C-E故障
      LNNormalFaultPar();//故障前和故障后参数
      V1Output=1;
      V2Output=1;
      V3Output=1;
      I1Output=0;
      I2Output=0;
      I3Output=1;

      if(k==1)
        Impedance=Impedance1*TestPoint1;
      else if(k==0)
        Impedance=Impedance1*TestPoint2;
      else if(k==-1)
        Impedance=Impedance1*TestPoint3;
      I3FaultValue=FixupCurrent1;
      //单相接地故障时，电流故障相I=I(固定电流)


      //调用出错处理程序，如果返回1，说明参数设置不对。
      if(Busvolcon==true)
     {
      COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
      V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
      V1FaultValue=SystemVolt;//SystemVolt;
      V2FaultValue=SystemVolt;//SystemVolt;
      I1FaultValue=0;
      I2FaultValue=0;
      V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
      V2FaultPhase=120+V3FaultPhase;
      V1FaultPhase=V3FaultPhase-120;
     }

      else
      {

          I1FaultValue=0;
          I2FaultValue=0;
          COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
          COMPLEX Va=VbCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
          COMPLEX Vb=VcCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);


          V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
          V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
          V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
          V1FaultPhase=CaculatePhase(Va.y,Va.x);
          V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
          V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
          //相故障时，非故障电压的相位不变，
          V1FaultPhase=fmod(V1FaultPhase,360);
          V2FaultPhase=fmod(V2FaultPhase,360);
          V3FaultPhase=fmod(V3FaultPhase,360);

      }

      ErrorFlag=ErrorWarning(1,3);
      if(ErrorFlag==1)
      return 1;



      //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
      I3FaultPhase=0-DelicacyAngle1-Angel_C;
      if(k==-1)
        I3FaultPhase=I3FaultPhase-180;

      I1FaultPhase=I3FaultPhase-120;
      I2FaultPhase=I3FaultPhase+120;
      //单相接地故障时，故障电流的相位=故障电压的相位-灵敏角。
      LNNormalFaultPar();//故障前和故障后参数

         // return 0;
          break;
        case 4://1娈电殑A-B鏁呴殰
        {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=0;

          if(k==1)
            Impedance=Impedance1*TestPoint1;
          else if(k==0)
            Impedance=Impedance1*TestPoint2;
          else if(k==-1)
            Impedance=Impedance1*TestPoint3;

          I1FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I2FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I3FaultValue=I3PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊


          COMPLEX Va1=V1LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          COMPLEX Vb1=V2LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
          V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);



          //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
          V3FaultValue=V3PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
          V3FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅
          V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
          V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);

          V1FaultPhase=V1FaultPhase;
          V2FaultPhase=V2FaultPhase;

          ErrorFlag=ErrorWarning(1,4);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
           if(ErrorFlag==1)
          return 1;







          V1FaultPhase=fmod(V1FaultPhase,360);
          V2FaultPhase=fmod(V2FaultPhase,360);
          if(V1FaultPhase>180)
            V1FaultPhase=V1FaultPhase-360;
          else if(V1FaultPhase<-180)
            V1FaultPhase=V1FaultPhase+360;
          if(V2FaultPhase>180)
            V2FaultPhase=V2FaultPhase-360;
          else if(V2FaultPhase<-180)
            V2FaultPhase=V2FaultPhase+360;



          I3FaultPhase=0;
          I1FaultPhase=-90-DelicacyAngle1;
          I2FaultPhase=90-DelicacyAngle1;
          if(k==-1)
            I1FaultPhase=I1FaultPhase-180;
          if(I1FaultPhase>180)
            I1FaultPhase=I1FaultPhase-360;
          else if(I1FaultPhase<-180)
            I1FaultPhase=I1FaultPhase+360;

          if(k==-1)
            I2FaultPhase=I2FaultPhase-180;
          if(I2FaultPhase>180)
            I2FaultPhase=I2FaultPhase-360;
          else if(I2FaultPhase<-180)
            I2FaultPhase=I2FaultPhase+360;

          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
          //return 0;
      }
          break;
        case 5://1娈电殑B-C鏁呴殰
        {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=0;
          I2Output=1;
          I3Output=1;

          if(k==1)
            Impedance=Impedance1*TestPoint1;
          else if(k==0)
            Impedance=Impedance1*TestPoint2;
          else if(k==-1)
            Impedance=Impedance1*TestPoint3;

          I2FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I3FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I1FaultValue=I1PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

          V1FaultValue=V1PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
          V1FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅

          COMPLEX Vb2=V1LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          COMPLEX Vc2=V2LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
          V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);



          ErrorFlag=ErrorWarning(1,5);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
           if(ErrorFlag==1)
          return 1;


          //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

          V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
          V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);
          V3FaultPhase=V3FaultPhase;
          V2FaultPhase=V2FaultPhase;




          //鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅數鍘嬬殑鐩镐綅涓嶅彉锛
          V2FaultPhase=fmod(V2FaultPhase,360);
          V3FaultPhase=fmod(V3FaultPhase,360);
          if(V2FaultPhase>180)
            V2FaultPhase=V2FaultPhase-360;
          else if(V2FaultPhase<-180)
            V2FaultPhase=V2FaultPhase+360;
          if(V3FaultPhase>180)
            V3FaultPhase=V3FaultPhase-360;
          else if(V3FaultPhase<-180)
            V3FaultPhase=V3FaultPhase+360;


          I1FaultPhase=0;
          I2FaultPhase=-90-DelicacyAngle1;
          I3FaultPhase=90-DelicacyAngle1;
          if(k==-1)
            I2FaultPhase=I2FaultPhase-180;
          if(I2FaultPhase>180)
            I2FaultPhase=I2FaultPhase-360;
          else if(I2FaultPhase<-180)
            I2FaultPhase=I2FaultPhase+360;

          if(k==-1)
            I3FaultPhase=I3FaultPhase-180;
          if(I3FaultPhase>180)
            I3FaultPhase=I3FaultPhase-360;
          else if(I3FaultPhase<-180)
            I3FaultPhase=I3FaultPhase+360;
          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
         // return 0;
          }
          break;
        case 6://1娈电殑C-A鏁呴殰
        {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=0;
          I3Output=1;

          if(k==1)
            Impedance=Impedance1*TestPoint1;
          else if(k==0)
            Impedance=Impedance1*TestPoint2;
          else if(k==-1)
            Impedance=Impedance1*TestPoint3;

          I1FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I3FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I2FaultValue=I2PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

          V2FaultValue=V2PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
          V2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅


          COMPLEX Vc3=V1LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          COMPLEX Va3=V2LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
          V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);



          ErrorFlag=ErrorWarning(1,6);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
           if(ErrorFlag==1)
          return 1;


          //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
          V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
          V1FaultPhase=CaculatePhase(Va3.y,Va3.x);
          V3FaultPhase=V3FaultPhase;
          V1FaultPhase=V1FaultPhase;


          V1FaultPhase=fmod(V1FaultPhase,360);
          V3FaultPhase=fmod(V3FaultPhase,360);
          if(V3FaultPhase>180)
            V3FaultPhase=V3FaultPhase-360;
          else if(V3FaultPhase<-180)
            V3FaultPhase=V3FaultPhase+360;
          if(V1FaultPhase>180)
            V1FaultPhase=V1FaultPhase-360;
          else if(V1FaultPhase<-180)
            V1FaultPhase=V1FaultPhase+360;

          I2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢垫祦鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佺殑鐩镐綅
          I3FaultPhase=-90-DelicacyAngle1;
          I1FaultPhase=90-DelicacyAngle1;
          if(k==-1)
            I3FaultPhase=I3FaultPhase-180;
          if(I3FaultPhase>180)
            I3FaultPhase=I3FaultPhase-360;
          else if(I3FaultPhase<-180)
            I3FaultPhase=I3FaultPhase+360;

          if(k==-1)
            I1FaultPhase=I1FaultPhase-180;
          if(I1FaultPhase>180)
            I1FaultPhase=I1FaultPhase-360;
          else if(I1FaultPhase<-180)
            I1FaultPhase=I1FaultPhase+360;
          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
          //return 0;
      }
          break;
        case 7://1娈电殑A-B-C鏁呴殰
       {   LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=1;

          if(k==1)
            Impedance=Impedance1*TestPoint1;
          else if(k==0)
            Impedance=Impedance1*TestPoint2;
          else if(k==-1)
            Impedance=Impedance1*TestPoint3;

          I1FaultValue=FixupCurrent1;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I2FaultValue=FixupCurrent1;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I3FaultValue=FixupCurrent1;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦

          V1FaultValue=FixupCurrent1*Impedance;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝U=I*Z锛堢浉鐢靛帇=鍥哄畾鐢垫祦*闃绘姉瀹氬€硷級
          V2FaultValue=V1FaultValue;
          V3FaultValue=V1FaultValue;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝V1銆乂2銆乂3骞呭€煎ぇ灏忕浉绛


          ErrorFlag=ErrorWarning(1,7);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
           if(ErrorFlag==1)
          return 1;



          //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

          V1FaultPhase=V1PerFaultPhase;
          V2FaultPhase=V2PerFaultPhase;
          V3FaultPhase=V3PerFaultPhase;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐢靛帇鐩哥殑鐩镐綅绛変簬鏁呴殰鍓嶇浉搴旂殑鐩哥殑鐩镐綅銆
          I1FaultPhase=V1FaultPhase-DelicacyAngle1;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐏垫晱瑙掔瓑浜嶪1涓嶸1涔嬮棿鐨勫す瑙掞紝I1鐨勭浉浣嶇瓑浜嶸1鐨勭浉浣嶅噺鐏垫晱瑙
          I2FaultPhase=I1FaultPhase-120;
          I3FaultPhase=I1FaultPhase+120;
          if(k==-1)
            I1FaultPhase=I1FaultPhase-180;
          if(I1FaultPhase>180)
            I1FaultPhase=I1FaultPhase-360;
          else if(I1FaultPhase<-180)
            I1FaultPhase=I1FaultPhase+360;

          if(k==-1)
            I2FaultPhase=I2FaultPhase-180;
          if(I2FaultPhase>180)
            I2FaultPhase=I2FaultPhase-360;
          else if(I2FaultPhase<-180)
            I2FaultPhase=I2FaultPhase+360;

          if(k==-1)
            I3FaultPhase=I3FaultPhase-180;
          if(I3FaultPhase>180)
            I3FaultPhase=I3FaultPhase-360;
          else if(I3FaultPhase<-180)
            I3FaultPhase=I3FaultPhase+360;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐩镐綅鎴20搴﹀绉板叧绯汇€
          LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
           }

          break;

 case 8:
    LNNormalFaultPar();//故障前和故障后参数电压都为系统电压，电流为0
    V1Output=1;
    V2Output=1;
    V3Output=1;
    I1Output=1;
    I2Output=0;
    I3Output=0;

       //不同测试点阻抗不一样
    if(k==1)
      Impedance=Impedance2*TestPoint1;
    else if(k==0)
      Impedance=Impedance2*TestPoint2;
    else if(k==-1)
      Impedance=Impedance2*TestPoint3;

    I1FaultValue=FixupCurrent2;
    if(Busvolcon==true)
  {  //调用出错处理程序，如果返回1，说明参数设置不对。
    COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
    V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
    V2FaultValue=SystemVolt;//SystemVolt;
    V3FaultValue=SystemVolt;//SystemVolt;
    I2FaultValue=0;
    I3FaultValue=0;
    //单相接地故障时，未发生故障的电压为Vnom（SystemVolt），未发生故障的电流为0。
    V1FaultPhase=CaculatePhase(Va.y,Va.x);
    V2FaultPhase=-120+V1FaultPhase;
    V3FaultPhase=120+V1FaultPhase;
    //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
  }
  else
    {

     I2FaultValue=0;
     I3FaultValue=0;

     COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
     COMPLEX Vb=VbCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
     COMPLEX Vc=VcCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);


     V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
     V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
     V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
     V1FaultPhase=CaculatePhase(Va.y,Va.x);
     V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
     V3FaultPhase=CaculatePhase(Vc.y,Vc.x);


     V1FaultPhase=fmod(V1FaultPhase,360);
     V2FaultPhase=fmod(V2FaultPhase,360);
     V3FaultPhase=fmod(V3FaultPhase,360);
    }

    ErrorFlag=ErrorWarning(2,1);
    if(ErrorFlag==1)
    return 1;

    I1FaultPhase=0-DelicacyAngle2-Angel_C;
    if(k==-1)
    I1FaultPhase=I1FaultPhase-180;
    I2FaultPhase=I1FaultPhase-120;
    I3FaultPhase=I1FaultPhase+120;
    LNNormalFaultPar();//故障前和故障后参数
    break;

   case 9:
         LNNormalFaultPar();//故障前和故障后参数
         V1Output=1;
         V2Output=1;
         V3Output=1;
         I1Output=0;
         I2Output=1;
         I3Output=0;

         if(k==1)
           Impedance=Impedance2*TestPoint1;
         else if(k==0)
           Impedance=Impedance2*TestPoint2;
         else if(k==-1)
           Impedance=Impedance2*TestPoint3;
         I2FaultValue=FixupCurrent2;
         //单相接地故障时，电流故障相I=I(固定电流)
         if(Busvolcon==true)
        {
         COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
         V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
         V1FaultValue=SystemVolt;//SystemVolt;
         V3FaultValue=SystemVolt;//SystemVolt;
         I1FaultValue=0;
         I3FaultValue=0;

         V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
         V1FaultPhase=V2FaultPhase+120;
         V3FaultPhase=V2FaultPhase-120;
        }
        else
         {
             I1FaultValue=0;
             I3FaultValue=0;
             COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
             COMPLEX Vc=VbCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
             COMPLEX Va=VcCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);

             V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
             V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
             V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
             V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
             V1FaultPhase=CaculatePhase(Va.y,Va.x);
             V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
             V1FaultPhase=fmod(V1FaultPhase,360);
             V2FaultPhase=fmod(V2FaultPhase,360);
             V3FaultPhase=fmod(V3FaultPhase,360);

         }

         ErrorFlag=ErrorWarning(2,2);
         if(ErrorFlag==1)
         return 1;

         I2FaultPhase=0-DelicacyAngle2-Angel_C;
         if(k==-1)
           I2FaultPhase=I2FaultPhase-180;
         I1FaultPhase=I2FaultPhase+120;
         I3FaultPhase=I2FaultPhase-120;
         LNNormalFaultPar();//故障前和故障后参数

        break;
      case 10://2娈电殑C-E鏁呴殰
               LNNormalFaultPar();//故障前和故障后参数
               V1Output=1;
               V2Output=1;
               V3Output=1;
               I1Output=0;
               I2Output=0;
               I3Output=1;

               if(k==1)
                 Impedance=Impedance2*TestPoint1;
               else if(k==0)
                 Impedance=Impedance2*TestPoint2;
               else if(k==-1)
                 Impedance=Impedance2*TestPoint3;
               I3FaultValue=FixupCurrent2;
               //单相接地故障时，电流故障相I=I(固定电流)

               //调用出错处理程序，如果返回1，说明参数设置不对。
               if(Busvolcon==true)
              {
               COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
               V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
               V1FaultValue=SystemVolt;//SystemVolt;
               V2FaultValue=SystemVolt;//SystemVolt;
               I1FaultValue=0;
               I2FaultValue=0;
               V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
               V2FaultPhase=120+V3FaultPhase;
               V1FaultPhase=V3FaultPhase-120;
              }

               else
               {

                   I1FaultValue=0;
                   I2FaultValue=0;
                   COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
                   COMPLEX Va=VbCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
                   COMPLEX Vb=VcCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);


                   V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
                   V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
                   V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
                   V1FaultPhase=CaculatePhase(Va.y,Va.x);
                   V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
                   V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
                   //相故障时，非故障电压的相位不变，
                   V1FaultPhase=fmod(V1FaultPhase,360);
                   V2FaultPhase=fmod(V2FaultPhase,360);
                   V3FaultPhase=fmod(V3FaultPhase,360);

               }

               ErrorFlag=ErrorWarning(2,3);
               if(ErrorFlag==1)
               return 1;

               //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
               I3FaultPhase=0-DelicacyAngle2-Angel_C;
               if(k==-1)
                 I3FaultPhase=I3FaultPhase-180;

               I1FaultPhase=I3FaultPhase-120;
               I2FaultPhase=I3FaultPhase+120;
               //单相接地故障时，故障电流的相位=故障电压的相位-灵敏角。
               LNNormalFaultPar();//故障前和故障后参数


        break;
      case 11://2娈电殑A-B鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=0;

        if(k==1)
          Impedance=Impedance2*TestPoint1;
        else if(k==0)
          Impedance=Impedance2*TestPoint2;
        else if(k==-1)
          Impedance=Impedance2*TestPoint3;

        I1FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=I3PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊


        COMPLEX Va1=V1LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        COMPLEX Vb1=V2LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
        V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);



        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultValue=V3PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V3FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅
        V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
        V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);
        V1FaultPhase=V1FaultPhase;
        V2FaultPhase=V2FaultPhase;


        ErrorFlag=ErrorWarning(2,4);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;





        V1FaultPhase=fmod(V1FaultPhase,360);
        V2FaultPhase=fmod(V2FaultPhase,360);
        if(V1FaultPhase>180)
          V1FaultPhase=V1FaultPhase-360;
        else if(V1FaultPhase<-180)
          V1FaultPhase=V1FaultPhase+360;
        if(V2FaultPhase>180)
          V2FaultPhase=V2FaultPhase-360;
        else if(V2FaultPhase<-180)
          V2FaultPhase=V2FaultPhase+360;



        I3FaultPhase=0;
        I1FaultPhase=-90-DelicacyAngle2;
        I2FaultPhase=90-DelicacyAngle2;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 12://2娈电殑B-C鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=0;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance2*TestPoint1;
        else if(k==0)
          Impedance=Impedance2*TestPoint2;
        else if(k==-1)
          Impedance=Impedance2*TestPoint3;

        I2FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I1FaultValue=I1PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V1FaultValue=V1PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V1FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅

        COMPLEX Vb2=V1LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        COMPLEX Vc2=V2LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
        V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€


        ErrorFlag=ErrorWarning(2,5);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;




        V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
        V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);
        V3FaultPhase=V3FaultPhase;
        V2FaultPhase=V2FaultPhase;



        V2FaultPhase=fmod(V2FaultPhase,360);
        V3FaultPhase=fmod(V3FaultPhase,360);
        if(V2FaultPhase>180)
          V2FaultPhase=V2FaultPhase-360;
        else if(V2FaultPhase<-180)
          V2FaultPhase=V2FaultPhase+360;
        if(V3FaultPhase>180)
          V3FaultPhase=V3FaultPhase-360;
        else if(V3FaultPhase<-180)
          V3FaultPhase=V3FaultPhase+360;


        I1FaultPhase=0;
        I2FaultPhase=-90-DelicacyAngle2;
        I3FaultPhase=90-DelicacyAngle2;
        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
       // return 0;
        }
        break;
      case 13://2娈电殑C-A鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=0;
        I3Output=1;

        if(k==1)
          Impedance=Impedance2*TestPoint1;
        else if(k==0)
          Impedance=Impedance2*TestPoint2;
        else if(k==-1)
          Impedance=Impedance2*TestPoint3;

        I1FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=I2PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V2FaultValue=V2PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅


        COMPLEX Vc3=V1LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        COMPLEX Va3=V2LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
        V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);


        ErrorFlag=ErrorWarning(2,6);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;




        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
        V1FaultPhase=CaculatePhase(Va3.y,Va3.x);
        V3FaultPhase=V3FaultPhase;
        V1FaultPhase=V1FaultPhase;


        V1FaultPhase=fmod(V1FaultPhase,360);
        V3FaultPhase=fmod(V3FaultPhase,360);
        if(V3FaultPhase>180)
          V3FaultPhase=V3FaultPhase-360;
        else if(V3FaultPhase<-180)
          V3FaultPhase=V3FaultPhase+360;
        if(V1FaultPhase>180)
          V1FaultPhase=V1FaultPhase-360;
        else if(V1FaultPhase<-180)
          V1FaultPhase=V1FaultPhase+360;

        I2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢垫祦鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佺殑鐩镐綅
        I3FaultPhase=-90-DelicacyAngle2;
        I1FaultPhase=90-DelicacyAngle2;
        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;

        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 14://2娈电殑A-B-C鏁呴殰
        {
        LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance2*TestPoint1;
        else if(k==0)
          Impedance=Impedance2*TestPoint2;
        else if(k==-1)
          Impedance=Impedance2*TestPoint3;

        I1FaultValue=FixupCurrent2;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent2;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent2;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦

        V1FaultValue=FixupCurrent2*Impedance;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝U=I*Z锛堢浉鐢靛帇=鍥哄畾鐢垫祦*闃绘姉瀹氬€硷級
        V2FaultValue=V1FaultValue;
        V3FaultValue=V1FaultValue;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝V1銆乂2銆乂3骞呭€煎ぇ灏忕浉绛


        ErrorFlag=ErrorWarning(2,7);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V1FaultPhase=V1PerFaultPhase;
        V2FaultPhase=V2PerFaultPhase;
        V3FaultPhase=V3PerFaultPhase;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐢靛帇鐩哥殑鐩镐綅绛変簬鏁呴殰鍓嶇浉搴旂殑鐩哥殑鐩镐綅銆
        I1FaultPhase=V1FaultPhase-DelicacyAngle2;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐏垫晱瑙掔瓑浜嶪1涓嶸1涔嬮棿鐨勫す瑙掞紝I1鐨勭浉浣嶇瓑浜嶸1鐨勭浉浣嶅噺鐏垫晱瑙
        I2FaultPhase=I1FaultPhase-120;
        I3FaultPhase=I1FaultPhase+120;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐩镐綅鎴20搴﹀绉板叧绯汇€
        LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
         }
        break;


  case 15:
    LNNormalFaultPar();//故障前和故障后参数电压都为系统电压，电流为0
    V1Output=1;
    V2Output=1;
    V3Output=1;
    I1Output=1;
    I2Output=0;
    I3Output=0;

       //不同测试点阻抗不一样
    if(k==1)
      Impedance=Impedance3*TestPoint1;
    else if(k==0)
      Impedance=Impedance3*TestPoint2;
    else if(k==-1)
      Impedance=Impedance3*TestPoint3;

    I1FaultValue=FixupCurrent3;
    if(Busvolcon==true)
  {  //调用出错处理程序，如果返回1，说明参数设置不对。

    COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
    V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
    V2FaultValue=SystemVolt;//SystemVolt;
    V3FaultValue=SystemVolt;//SystemVolt;
    I2FaultValue=0;
    I3FaultValue=0;
    //单相接地故障时，未发生故障的电压为Vnom（SystemVolt），未发生故障的电流为0。
    V1FaultPhase=CaculatePhase(Va.y,Va.x);
    V2FaultPhase=-120+V1FaultPhase;
    V3FaultPhase=120+V1FaultPhase;
    //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
  }
  else
    {

     I2FaultValue=0;
     I3FaultValue=0;

     COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
     COMPLEX Vb=VbCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
     COMPLEX Vc=VcCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);


     V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
     V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
     V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);

     V1FaultPhase=CaculatePhase(Va.y,Va.x);
     V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
     V3FaultPhase=CaculatePhase(Vc.y,Vc.x);


     V1FaultPhase=fmod(V1FaultPhase,360);
     V2FaultPhase=fmod(V2FaultPhase,360);
     V3FaultPhase=fmod(V3FaultPhase,360);
    }

    ErrorFlag=ErrorWarning(3,1);
    if(ErrorFlag==1)
    return 1;



    I1FaultPhase=0-DelicacyAngle3-Angel_C;
    if(k==-1)
    I1FaultPhase=I1FaultPhase-180;

    I2FaultPhase=I1FaultPhase-120;
    I3FaultPhase=I1FaultPhase+120;
    LNNormalFaultPar();//故障前和故障后参数
 break;
      case 16://3娈电殑B-E鏁呴殰
         LNNormalFaultPar();//故障前和故障后参数
         V1Output=1;
         V2Output=1;
         V3Output=1;
         I1Output=0;
         I2Output=1;
         I3Output=0;
         if(k==1)
           Impedance=Impedance3*TestPoint1;
         else if(k==0)
           Impedance=Impedance3*TestPoint2;
         else if(k==-1)
           Impedance=Impedance3*TestPoint3;
         I2FaultValue=FixupCurrent3;
         if(Busvolcon==true)
        {
         COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
         V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
         V1FaultValue=SystemVolt;//SystemVolt;
         V3FaultValue=SystemVolt;//SystemVolt;
         I1FaultValue=0;
         I3FaultValue=0;

         V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
         V1FaultPhase=V2FaultPhase+120;
         V3FaultPhase=V2FaultPhase-120;
        }
        else
         {
             I1FaultValue=0;
             I3FaultValue=0;
             COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
             COMPLEX Vc=VbCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
             COMPLEX Va=VcCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);

             V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
             V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
             V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
             V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
             V1FaultPhase=CaculatePhase(Va.y,Va.x);
             V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
             V1FaultPhase=fmod(V1FaultPhase,360);
             V2FaultPhase=fmod(V2FaultPhase,360);
             V3FaultPhase=fmod(V3FaultPhase,360);

         }

         ErrorFlag=ErrorWarning(3,2);
         if(ErrorFlag==1)
         return 1;

         I2FaultPhase=0-DelicacyAngle3-Angel_C;
         if(k==-1)
           I2FaultPhase=I2FaultPhase-180;
         I1FaultPhase=I2FaultPhase+120;
         I3FaultPhase=I2FaultPhase-120;
         LNNormalFaultPar();//故障前和故障后参数
         //return 0;
        break;
      case 17://1娈电殑C-E鏁呴殰
         LNNormalFaultPar();//故障前和故障后参数
               V1Output=1;
               V2Output=1;
               V3Output=1;
               I1Output=0;
               I2Output=0;
               I3Output=1;

               if(k==1)
                 Impedance=Impedance3*TestPoint1;
               else if(k==0)
                 Impedance=Impedance3*TestPoint2;
               else if(k==-1)
                 Impedance=Impedance3*TestPoint3;
               I3FaultValue=FixupCurrent3;


               if(Busvolcon==true)
              {
               COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
               V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
               V1FaultValue=SystemVolt;//SystemVolt;
               V2FaultValue=SystemVolt;//SystemVolt;
               I1FaultValue=0;
               I2FaultValue=0;
               V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
               V2FaultPhase=120+V3FaultPhase;
               V1FaultPhase=V3FaultPhase-120;
              }
               else
               {
                   I1FaultValue=0;
                   I2FaultValue=0;
                   COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
                   COMPLEX Va=VbCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
                   COMPLEX Vb=VcCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);

                   V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
                   V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
                   V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
                   V1FaultPhase=CaculatePhase(Va.y,Va.x);
                   V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
                   V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
                   //相故障时，非故障电压的相位不变，
                   V1FaultPhase=fmod(V1FaultPhase,360);
                   V2FaultPhase=fmod(V2FaultPhase,360);
                   V3FaultPhase=fmod(V3FaultPhase,360);

               }
               ErrorFlag=ErrorWarning(3,3);
               if(ErrorFlag==1)
               return 1;

               I3FaultPhase=0-DelicacyAngle3-Angel_C;
               if(k==-1)
                 I3FaultPhase=I3FaultPhase-180;

               I1FaultPhase=I3FaultPhase-120;
               I2FaultPhase=I3FaultPhase+120;
               //单相接地故障时，故障电流的相位=故障电压的相位-灵敏角。
               LNNormalFaultPar();//故障前和故障后参数
        break;
      case 18://1娈电殑A-B鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=0;

        if(k==1)
          Impedance=Impedance3*TestPoint1;
        else if(k==0)
          Impedance=Impedance3*TestPoint2;
        else if(k==-1)
          Impedance=Impedance3*TestPoint3;

        I1FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=I3PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊


        COMPLEX Va1=V1LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        COMPLEX Vb1=V2LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
        V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);



        ErrorFlag=ErrorWarning(3,4);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultValue=V3PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V3FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅
        V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
        V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);


        V2FaultPhase=V2FaultPhase;
        V1FaultPhase=V1FaultPhase;





        V1FaultPhase=fmod(V1FaultPhase,360);
        V2FaultPhase=fmod(V2FaultPhase,360);
        if(V1FaultPhase>180)
          V1FaultPhase=V1FaultPhase-360;
        else if(V1FaultPhase<-180)
          V1FaultPhase=V1FaultPhase+360;
        if(V2FaultPhase>180)
          V2FaultPhase=V2FaultPhase-360;
        else if(V2FaultPhase<-180)
          V2FaultPhase=V2FaultPhase+360;



        I3FaultPhase=0;
        I1FaultPhase=-90-DelicacyAngle3;
        I2FaultPhase=90-DelicacyAngle3;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 19://1娈电殑B-C鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=0;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance3*TestPoint1;
        else if(k==0)
          Impedance=Impedance3*TestPoint2;
        else if(k==-1)
          Impedance=Impedance3*TestPoint3;

        I2FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I1FaultValue=I1PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V1FaultValue=V1PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V1FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅

        COMPLEX Vb2=V1LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        COMPLEX Vc2=V2LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
        V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);



        ErrorFlag=ErrorWarning(3,5);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
        V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);


        V2FaultPhase=V2FaultPhase;
        V3FaultPhase=V3FaultPhase;




        //鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅數鍘嬬殑鐩镐綅涓嶅彉锛
        V2FaultPhase=fmod(V2FaultPhase,360);
        V3FaultPhase=fmod(V3FaultPhase,360);
        if(V2FaultPhase>180)
          V2FaultPhase=V2FaultPhase-360;
        else if(V2FaultPhase<-180)
          V2FaultPhase=V2FaultPhase+360;
        if(V3FaultPhase>180)
          V3FaultPhase=V3FaultPhase-360;
        else if(V3FaultPhase<-180)
          V3FaultPhase=V3FaultPhase+360;


        I1FaultPhase=0;
        I2FaultPhase=-90-DelicacyAngle3;
        I3FaultPhase=90-DelicacyAngle3;
        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
       // return 0;
        }
        break;
      case 20://1娈电殑C-A鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=0;
        I3Output=1;

        if(k==1)
          Impedance=Impedance3*TestPoint1;
        else if(k==0)
          Impedance=Impedance3*TestPoint2;
        else if(k==-1)
          Impedance=Impedance3*TestPoint3;

        I1FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=I2PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V2FaultValue=V2PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅


        COMPLEX Vc3=V1LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        COMPLEX Va3=V2LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
        V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);


        ErrorFlag=ErrorWarning(3,6);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
        V1FaultPhase=CaculatePhase(Va3.y,Va3.x);
        V1FaultPhase=V1FaultPhase;
        V3FaultPhase=V3FaultPhase;


        V1FaultPhase=fmod(V1FaultPhase,360);
        V3FaultPhase=fmod(V3FaultPhase,360);
        if(V3FaultPhase>180)
          V3FaultPhase=V3FaultPhase-360;
        else if(V3FaultPhase<-180)
          V3FaultPhase=V3FaultPhase+360;
        if(V1FaultPhase>180)
          V1FaultPhase=V1FaultPhase-360;
        else if(V1FaultPhase<-180)
          V1FaultPhase=V1FaultPhase+360;

        I2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢垫祦鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佺殑鐩镐綅
        I3FaultPhase=-90-DelicacyAngle3;
        I1FaultPhase=90-DelicacyAngle3;
        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;

        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 21://1娈电殑A-B-C鏁呴殰
     {   LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance3*TestPoint1;
        else if(k==0)
          Impedance=Impedance3*TestPoint2;
        else if(k==-1)
          Impedance=Impedance3*TestPoint3;

        I1FaultValue=FixupCurrent3;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent3;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent3;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦

        V1FaultValue=FixupCurrent3*Impedance;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝U=I*Z锛堢浉鐢靛帇=鍥哄畾鐢垫祦*闃绘姉瀹氬€硷級
        V2FaultValue=V1FaultValue;
        V3FaultValue=V1FaultValue;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝V1銆乂2銆乂3骞呭€煎ぇ灏忕浉绛


        ErrorFlag=ErrorWarning(3,7);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V1FaultPhase=V1PerFaultPhase;
        V2FaultPhase=V2PerFaultPhase;
        V3FaultPhase=V3PerFaultPhase;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐢靛帇鐩哥殑鐩镐綅绛変簬鏁呴殰鍓嶇浉搴旂殑鐩哥殑鐩镐綅銆
        I1FaultPhase=V1FaultPhase-DelicacyAngle3;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐏垫晱瑙掔瓑浜嶪1涓嶸1涔嬮棿鐨勫す瑙掞紝I1鐨勭浉浣嶇瓑浜嶸1鐨勭浉浣嶅噺鐏垫晱瑙
        I2FaultPhase=I1FaultPhase-120;
        I3FaultPhase=I1FaultPhase+120;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐩镐綅鎴20搴﹀绉板叧绯汇€
        LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
         }

        break;

      case 22:  //4娈电殑A-E鏁呴殰

    LNNormalFaultPar();//故障前和故障后参数电压都为系统电压，电流为0
    V1Output=1;
    V2Output=1;
    V3Output=1;
    I1Output=1;
    I2Output=0;
    I3Output=0;

       //不同测试点阻抗不一样
    if(k==1)
      Impedance=Impedance4*TestPoint1;
    else if(k==0)
      Impedance=Impedance4*TestPoint2;
    else if(k==-1)
      Impedance=Impedance4*TestPoint3;

    I1FaultValue=FixupCurrent4;
    if(Busvolcon==true)
  {

    COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
    V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
    V2FaultValue=SystemVolt;//SystemVolt;
    V3FaultValue=SystemVolt;//SystemVolt;
    I2FaultValue=0;
    I3FaultValue=0;
    //单相接地故障时，未发生故障的电压为Vnom（SystemVolt），未发生故障的电流为0。
    V1FaultPhase=CaculatePhase(Va.y,Va.x);
    V2FaultPhase=-120+V1FaultPhase;
    V3FaultPhase=120+V1FaultPhase;
    //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
  }
  else
    {

     I2FaultValue=0;
     I3FaultValue=0;

     COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
     COMPLEX Vb=VbCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
     COMPLEX Vc=VcCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);


     V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
     V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
     V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);

     V1FaultPhase=CaculatePhase(Va.y,Va.x);
     V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
     V3FaultPhase=CaculatePhase(Vc.y,Vc.x);


     V1FaultPhase=fmod(V1FaultPhase,360);
     V2FaultPhase=fmod(V2FaultPhase,360);
     V3FaultPhase=fmod(V3FaultPhase,360);
    }

    ErrorFlag=ErrorWarning(4,1);
    if(ErrorFlag==1)
    return 1;



    I1FaultPhase=0-DelicacyAngle4-Angel_C;
    if(k==-1)
    I1FaultPhase=I1FaultPhase-180;

    I2FaultPhase=I1FaultPhase-120;
    I3FaultPhase=I1FaultPhase+120;
    LNNormalFaultPar();//故障前和故障后参数
        break;
      case 23://4娈电殑B-E鏁呴殰
         LNNormalFaultPar();//故障前和故障后参数
         V1Output=1;
         V2Output=1;
         V3Output=1;
         I1Output=0;
         I2Output=1;
         I3Output=0;
         if(k==1)
           Impedance=Impedance4*TestPoint1;
         else if(k==0)
           Impedance=Impedance4*TestPoint2;
         else if(k==-1)
           Impedance=Impedance4*TestPoint3;
         I2FaultValue=FixupCurrent4;
         if(Busvolcon==true)
        {
         COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
         V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
         V1FaultValue=SystemVolt;//SystemVolt;
         V3FaultValue=SystemVolt;//SystemVolt;
         I1FaultValue=0;
         I3FaultValue=0;

         V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
         V1FaultPhase=V2FaultPhase+120;
         V3FaultPhase=V2FaultPhase-120;
        }
        else
         {
             I1FaultValue=0;
             I3FaultValue=0;
             COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
             COMPLEX Vc=VbCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
             COMPLEX Va=VcCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);

             V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
             V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
             V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
             V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
             V1FaultPhase=CaculatePhase(Va.y,Va.x);
             V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
             V1FaultPhase=fmod(V1FaultPhase,360);
             V2FaultPhase=fmod(V2FaultPhase,360);
             V3FaultPhase=fmod(V3FaultPhase,360);

         }

         ErrorFlag=ErrorWarning(4,2);
         if(ErrorFlag==1)
         return 1;

         I2FaultPhase=0-DelicacyAngle4-Angel_C;
         if(k==-1)
           I2FaultPhase=I2FaultPhase-180;
         I1FaultPhase=I2FaultPhase+120;
         I3FaultPhase=I2FaultPhase-120;
         LNNormalFaultPar();//故障前和故障后参数

        break;
      case 24://4娈电殑C-E鏁呴殰
               LNNormalFaultPar();//故障前和故障后参数
               V1Output=1;
               V2Output=1;
               V3Output=1;
               I1Output=0;
               I2Output=0;
               I3Output=1;

               if(k==1)
                 Impedance=Impedance4*TestPoint1;
               else if(k==0)
                 Impedance=Impedance4*TestPoint2;
               else if(k==-1)
                 Impedance=Impedance4*TestPoint3;
               I3FaultValue=FixupCurrent4;
               //单相接地故障时，电流故障相I=I(固定电流)


               //调用出错处理程序，如果返回1，说明参数设置不对。
               if(Busvolcon==true)
              {
               COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
               V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
               V1FaultValue=SystemVolt;//SystemVolt;
               V2FaultValue=SystemVolt;//SystemVolt;
               I1FaultValue=0;
               I2FaultValue=0;
               V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
               V2FaultPhase=120+V3FaultPhase;
               V1FaultPhase=V3FaultPhase-120;
              }

               else
               {

                   I1FaultValue=0;
                   I2FaultValue=0;
                   COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
                   COMPLEX Va=VbCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
                   COMPLEX Vb=VcCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);


                   V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
                   V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
                   V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
                   V1FaultPhase=CaculatePhase(Va.y,Va.x);
                   V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
                   V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
                   //相故障时，非故障电压的相位不变，
                   V1FaultPhase=fmod(V1FaultPhase,360);
                   V2FaultPhase=fmod(V2FaultPhase,360);
                   V3FaultPhase=fmod(V3FaultPhase,360);

               }

               ErrorFlag=ErrorWarning(4,3);
               if(ErrorFlag==1)
               return 1;



               //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
               I3FaultPhase=0-DelicacyAngle4-Angel_C;
               if(k==-1)
                 I3FaultPhase=I3FaultPhase-180;

               I1FaultPhase=I3FaultPhase-120;
               I2FaultPhase=I3FaultPhase+120;
               //单相接地故障时，故障电流的相位=故障电压的相位-灵敏角。
               LNNormalFaultPar();//故障前和故障后参数

                  // return 0;
        break;
      case 25://4娈电殑A-B鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=0;

        if(k==1)
          Impedance=Impedance4*TestPoint1;
        else if(k==0)
          Impedance=Impedance4*TestPoint2;
        else if(k==-1)
          Impedance=Impedance4*TestPoint3;

        I1FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=I3PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊


        COMPLEX Va1=V1LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        COMPLEX Vb1=V2LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
        V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);



        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultValue=V3PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V3FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅
        V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
        V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);

        V2FaultPhase=V2FaultPhase;
        V1FaultPhase=V1FaultPhase;




        ErrorFlag=ErrorWarning(4,4);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;





        V1FaultPhase=fmod(V1FaultPhase,360);
        V2FaultPhase=fmod(V2FaultPhase,360);
        if(V1FaultPhase>180)
          V1FaultPhase=V1FaultPhase-360;
        else if(V1FaultPhase<-180)
          V1FaultPhase=V1FaultPhase+360;
        if(V2FaultPhase>180)
          V2FaultPhase=V2FaultPhase-360;
        else if(V2FaultPhase<-180)
          V2FaultPhase=V2FaultPhase+360;



        I3FaultPhase=0;
        I1FaultPhase=-90-DelicacyAngle4;
        I2FaultPhase=90-DelicacyAngle4;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 26://4娈电殑B-C鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=0;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance4*TestPoint1;
        else if(k==0)
          Impedance=Impedance4*TestPoint2;
        else if(k==-1)
          Impedance=Impedance4*TestPoint3;

        I2FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I1FaultValue=I1PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V1FaultValue=V1PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V1FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅

        COMPLEX Vb2=V1LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        COMPLEX Vc2=V2LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
        V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);


        ErrorFlag=ErrorWarning(4,5);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
        V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);
        V2FaultPhase=V2FaultPhase;
        V3FaultPhase=V3FaultPhase;




        //鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅數鍘嬬殑鐩镐綅涓嶅彉锛
        V2FaultPhase=fmod(V2FaultPhase,360);
        V3FaultPhase=fmod(V3FaultPhase,360);
        if(V2FaultPhase>180)
          V2FaultPhase=V2FaultPhase-360;
        else if(V2FaultPhase<-180)
          V2FaultPhase=V2FaultPhase+360;
        if(V3FaultPhase>180)
          V3FaultPhase=V3FaultPhase-360;
        else if(V3FaultPhase<-180)
          V3FaultPhase=V3FaultPhase+360;


        I1FaultPhase=0;
        I2FaultPhase=-90-DelicacyAngle4;
        I3FaultPhase=90-DelicacyAngle4;
        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
       // return 0;
        }
        break;
      case 27://4娈电殑C-A鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=0;
        I3Output=1;

        if(k==1)

          Impedance=Impedance4*TestPoint1;
        else if(k==0)
          Impedance=Impedance4*TestPoint2;
        else if(k==-1)
          Impedance=Impedance4*TestPoint3;

        I1FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=I2PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V2FaultValue=V2PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅


        COMPLEX Vc3=V1LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        COMPLEX Va3=V2LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
        V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);


        ErrorFlag=ErrorWarning(4,6);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
        V1FaultPhase=CaculatePhase(Va3.y,Va3.x);
        V1FaultPhase=V1FaultPhase;
        V3FaultPhase=V3FaultPhase;



        V1FaultPhase=fmod(V1FaultPhase,360);
        V3FaultPhase=fmod(V3FaultPhase,360);
        if(V3FaultPhase>180)
          V3FaultPhase=V3FaultPhase-360;
        else if(V3FaultPhase<-180)
          V3FaultPhase=V3FaultPhase+360;
        if(V1FaultPhase>180)
          V1FaultPhase=V1FaultPhase-360;
        else if(V1FaultPhase<-180)
          V1FaultPhase=V1FaultPhase+360;

        I2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢垫祦鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佺殑鐩镐綅
        I3FaultPhase=-90-DelicacyAngle4;
        I1FaultPhase=90-DelicacyAngle4;
        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;

        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 28://4娈电殑A-B-C鏁呴殰
     {   LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance4*TestPoint1;
        else if(k==0)
          Impedance=Impedance4*TestPoint2;
        else if(k==-1)
          Impedance=Impedance4*TestPoint3;

        I1FaultValue=FixupCurrent4;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent4;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent4;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦

        V1FaultValue=FixupCurrent4*Impedance;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝U=I*Z锛堢浉鐢靛帇=鍥哄畾鐢垫祦*闃绘姉瀹氬€硷級
        V2FaultValue=V1FaultValue;
        V3FaultValue=V1FaultValue;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝V1銆乂2銆乂3骞呭€煎ぇ灏忕浉绛


        ErrorFlag=ErrorWarning(4,7);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V1FaultPhase=V1PerFaultPhase;
        V2FaultPhase=V2PerFaultPhase;
        V3FaultPhase=V3PerFaultPhase;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐢靛帇鐩哥殑鐩镐綅绛変簬鏁呴殰鍓嶇浉搴旂殑鐩哥殑鐩镐綅銆
        I1FaultPhase=V1FaultPhase-DelicacyAngle4;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐏垫晱瑙掔瓑浜嶪1涓嶸1涔嬮棿鐨勫す瑙掞紝I1鐨勭浉浣嶇瓑浜嶸1鐨勭浉浣嶅噺鐏垫晱瑙
        I2FaultPhase=I1FaultPhase-120;
        I3FaultPhase=I1FaultPhase+120;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐩镐綅鎴20搴﹀绉板叧绯汇€
        LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
         }

        break;


      }

    return 0;
  }





void thread1::OutSequencer(int SeqCount,int fd)
  {
    if(Zone1==1&&SeqCount>=1&&SeqCount<21)
      {
        if(SeqCount==1&&FaultType1==1)
          { //SeqCount涓琛ㄧず瑕佹祴璇曠殑鏄娈礎-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(1,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=2;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
                }
        if(SeqCount==2&&FaultType1==1)
          { //SeqCount涓琛ㄧず瑕佹祴璇曠殑鏄娈礎-E鏁呴殰鐨勫弽鐩窽estPoint3杩欎釜鐐
            OutputSignalCompute(1,-1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=3;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }

        if(SeqCount==3&&FaultType2==1)
          {//SeqCount涓琛ㄧず瑕佹祴璇曠殑鏄娈礏-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(2,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=4;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==3&&FaultType2==0)
          SeqCount+=3;//濡傛灉1娈礏-E涓嶆祴璇曪紝SeqCount鍔鍒娈礐-E鐨凾estPoint2
        if(SeqCount==4)
          {//SeqCount涓琛ㄧず瑕佹祴璇曠殑鏄娈礏-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(2,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=5;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==5)
          {//SeqCount涓琛ㄧず瑕佹祴璇曠殑鏄娈礏-E鏁呴殰鐨勫弽鐩窽estPoint3杩欎釜鐐
            OutputSignalCompute(2,-1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=6;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }

        if(SeqCount==6&&FaultType3==1)
          {//SeqCount涓琛ㄧず瑕佹祴璇曠殑鏄娈礐-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(3,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=7;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==6&&FaultType3==0)
          SeqCount+=3;//濡傛灉1娈礐-E涓嶆祴璇曪紝SeqCount鍔鍒娈礎-B鐨凾estPoint2
        if(SeqCount==7)
          {//SeqCount涓琛ㄧず瑕佹祴璇曠殑鏄娈礐-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(3,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=8;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==8)
          {//SeqCount涓琛ㄧず瑕佹祴璇曠殑鏄娈礐-E鏁呴殰鐨勫弽鐩窽estPoint3杩欎釜鐐
            OutputSignalCompute(3,-1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=9;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }

        if(SeqCount==9&&FaultType4==1)
          {//SeqCount涓琛ㄧず瑕佹祴璇曠殑鏄娈礎-B鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(4,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=10;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==9&&FaultType4==0)
          SeqCount+=3;//濡傛灉1娈礎-B涓嶆祴璇曪紝SeqCount鍔鍒娈礏-C鐨凾estPoint2
        if(SeqCount==10)
          {//SeqCount涓0琛ㄧず瑕佹祴璇曠殑鏄娈礎-B鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(4,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=11;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==11)
          {//SeqCount涓1琛ㄧず瑕佹祴璇曠殑鏄娈礎-B鏁呴殰鐨勫弽鐩窽estPoint3杩欎釜鐐
            OutputSignalCompute(4,-1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=12;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }

        if(SeqCount==12&&FaultType5==1)
          {//SeqCount涓2琛ㄧず瑕佹祴璇曠殑鏄娈礏-C鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(5,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=13;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==12&&FaultType5==0)
          SeqCount+=3;//濡傛灉1娈礏-C涓嶆祴璇曪紝SeqCount鍔鍒娈礐-A鐨凾estPoint2
        if(SeqCount==13)
          {//SeqCount涓3琛ㄧず瑕佹祴璇曠殑鏄娈礏-C鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(5,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=14;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==14)
          {//SeqCount涓4琛ㄧず瑕佹祴璇曠殑鏄娈礏-C鏁呴殰鐨勫弽鐩窽estPoint3杩欎釜鐐
            OutputSignalCompute(5,-1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=15;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }

        if(SeqCount==15&&FaultType6==1)
          {//SeqCount涓5琛ㄧず瑕佹祴璇曠殑鏄娈礐-A鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(6,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=16;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==15&&FaultType6==0)
          SeqCount+=3;//濡傛灉1娈礐-A涓嶆祴璇曪紝SeqCount鍔鍒娈礎-B-C鐨凾estPoint2
        if(SeqCount==16)
          {//SeqCount涓6琛ㄧず瑕佹祴璇曠殑鏄娈礐-A鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(6,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=17;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==17)
          {//SeqCount涓7琛ㄧず瑕佹祴璇曠殑鏄娈礐-A鏁呴殰鐨勫弽鐩窽estPoint3杩欎釜鐐
            OutputSignalCompute(6,-1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=18;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }

        if(SeqCount==18&&FaultType7==1)
          {//SeqCount涓8琛ㄧず瑕佹祴璇曠殑鏄娈礎-B-C鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(7,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=19;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==18&&FaultType7==0)
          {
            SeqCount+=3;//濡傛灉1娈礎-B-C涓嶆祴璇曪紝SeqCount鍔鍒娈礎-E鐨凾estPoint2
            DisResult=0;
          }
        if(SeqCount==19)
          {//SeqCount涓9琛ㄧず瑕佹祴璇曠殑鏄娈礎-B-C鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(7,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=20;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==20)
          {//SeqCount涓0琛ㄧず瑕佹祴璇曠殑鏄娈礎-B-C鏁呴殰鐨勫弽鐩窽estPoint3杩欎釜鐐
            OutputSignalCompute(7,-1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=21;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
      }


    if(Zone2==1&&SeqCount>=21&&SeqCount<35)
      {
        if(SeqCount==21&&FaultType1==1)
          {//SeqCount涓1琛ㄧず瑕佹祴璇曠殑鏄娈礎-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(8,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=22;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==21&&FaultType1==0)
          SeqCount+=2;//濡傛灉2娈礎-E涓嶆祴璇曪紝SeqCount鍔鍒娈礏-E鐨凾estPoint2
        if(SeqCount==22)
          {//SeqCount涓2琛ㄧず瑕佹祴璇曠殑鏄娈礎-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(8,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=23;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==23&&FaultType2==1)
          {//SeqCount涓3琛ㄧず瑕佹祴璇曠殑鏄娈礏-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(9,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=24;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==23&&FaultType2==0)
          SeqCount+=2;//濡傛灉2娈礏-E涓嶆祴璇曪紝SeqCount鍔鍒娈礐-E鐨凾estPoint2
        if(SeqCount==24)
          {//SeqCount涓4琛ㄧず瑕佹祴璇曠殑鏄娈礏-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(9,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=25;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==25&&FaultType3==1)
          {//SeqCount涓5琛ㄧず瑕佹祴璇曠殑鏄娈礐-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(10,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=26;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==25&&FaultType3==0)
          SeqCount+=2;//濡傛灉2娈礐-E涓嶆祴璇曪紝SeqCount鍔鍒娈礎-B鐨凾estPoint2
        if(SeqCount==26)
          {//SeqCount涓6琛ㄧず瑕佹祴璇曠殑鏄娈礐-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(10,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=27;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==27&&FaultType4==1)
          {//SeqCount涓7琛ㄧず瑕佹祴璇曠殑鏄娈礎-B鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(11,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=28;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==27&&FaultType4==0)
          SeqCount+=2;//濡傛灉2娈礎-B涓嶆祴璇曪紝SeqCount鍔鍒娈礏-C鐨凾estPoint2
        if(SeqCount==28)
          {//SeqCount涓8琛ㄧず瑕佹祴璇曠殑鏄娈礎-B鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(11,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=29;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==29&&FaultType5==1)
          {//SeqCount涓9琛ㄧず瑕佹祴璇曠殑鏄娈礏-C鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(12,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=30;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==29&&FaultType5==0)
          SeqCount+=2;//濡傛灉2娈礏-C涓嶆祴璇曪紝SeqCount鍔鍒娈礐-A鐨凾estPoint2
        if(SeqCount==30)
          {//SeqCount涓0琛ㄧず瑕佹祴璇曠殑鏄娈礏-C鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(12,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=31;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==31&&FaultType6==1)
          {//SeqCount涓1琛ㄧず瑕佹祴璇曠殑鏄娈礐-A鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(13,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=32;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==31&&FaultType6==0)
          SeqCount+=2;//濡傛灉2娈礐-A涓嶆祴璇曪紝SeqCount鍔鍒娈礎-B-C鐨凾estPoint2
        if(SeqCount==32)
          {//SeqCount涓2琛ㄧず瑕佹祴璇曠殑鏄娈礐-A鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(13,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=33;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==33&&FaultType7==1)
          {//SeqCount涓3琛ㄧず瑕佹祴璇曠殑鏄娈礎-B-C鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(14,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=34;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==33&&FaultType7==0)
          SeqCount+=2;//濡傛灉2娈礎-B-C涓嶆祴璇曪紝SeqCount鍔鍒娈礎-E鐨凾estPoint2
        if(SeqCount==34)
          {//SeqCount涓4琛ㄧず瑕佹祴璇曠殑鏄娈礎-B-C鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(14,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=35;
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
      }
    else if(Zone2==0&&SeqCount==21)
      {
        SeqCount=35;
        DisResult=0;
      }

    if(Zone3==1&&SeqCount>=35&&SeqCount<49)
      {
        if(SeqCount==35&&FaultType1==1)
          {//SeqCount涓5琛ㄧず瑕佹祴璇曠殑鏄娈礎-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(15,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=36;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==35&&FaultType1==0)
          SeqCount+=2;//濡傛灉3娈礎-E涓嶆祴璇曪紝SeqCount鍔鍒娈礏-E鐨凾estPoint2
        if(SeqCount==36)
          {//SeqCount涓6琛ㄧず瑕佹祴璇曠殑鏄娈礎-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(15,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=37;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==37&&FaultType2==1)
          {//SeqCount涓7琛ㄧず瑕佹祴璇曠殑鏄娈礏-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(16,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=38;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==37&&FaultType2==0)
          SeqCount+=2;//濡傛灉3娈礏-E涓嶆祴璇曪紝SeqCount鍔鍒娈礐-E鐨凾estPoint2
        if(SeqCount==38)
          {//SeqCount涓1琛ㄧず瑕佹祴璇曠殑鏄娈礏-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(16,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=39;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==39&&FaultType3==1)
          {//SeqCount涓9琛ㄧず瑕佹祴璇曠殑鏄娈礐-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(17,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=40;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==39&&FaultType3==0)
          SeqCount+=2;//濡傛灉3娈礐-E涓嶆祴璇曪紝SeqCount鍔鍒娈礎-B鐨凾estPoint2
        if(SeqCount==40)
          {//SeqCount涓0琛ㄧず瑕佹祴璇曠殑鏄娈礐-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(17,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=41;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==41&&FaultType4==1)
          {//SeqCount涓1琛ㄧず瑕佹祴璇曠殑鏄娈礎-B鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(18,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=42;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==41&&FaultType4==0)
          SeqCount+=2;//濡傛灉3娈礎-B涓嶆祴璇曪紝SeqCount鍔鍒娈礏-C鐨凾estPoint2
        if(SeqCount==42)
          {//SeqCount涓5琛ㄧず瑕佹祴璇曠殑鏄娈礎-B鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(18,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=43;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==43&&FaultType5==1)
          {//SeqCount涓3琛ㄧず瑕佹祴璇曠殑鏄娈礏-C鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(19,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=44;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==43&&FaultType5==0)
          SeqCount+=2;//濡傛灉3娈礏-C涓嶆祴璇曪紝SeqCount鍔鍒娈礐-A鐨凾estPoint2
        if(SeqCount==44)
          {//SeqCount涓4琛ㄧず瑕佹祴璇曠殑鏄娈礏-C鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(19,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=45;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==45&&FaultType6==1)
          {//SeqCount涓5琛ㄧず瑕佹祴璇曠殑鏄娈礐-A鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(20,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=46;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==45&&FaultType6==0)
          SeqCount+=2;//濡傛灉3娈礐-A涓嶆祴璇曪紝SeqCount鍔鍒娈礎-B-C鐨凾estPoint2
        if(SeqCount==46)
          {//SeqCount涓6琛ㄧず瑕佹祴璇曠殑鏄娈礐-A鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(20,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=47;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==47&&FaultType7==1)
          {//SeqCount涓7琛ㄧず瑕佹祴璇曠殑鏄娈礎-B-C鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(21,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=48;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==47&&FaultType7==0)
          {
            SeqCount+=2;//濡傛灉3娈礎-B-C涓嶆祴璇曪紝SeqCount鍔鍒娈礎-E鐨凾estPoint2
            DisResult=0;
          }
        if(SeqCount==48)
          {//SeqCount涓8琛ㄧず瑕佹祴璇曠殑鏄娈礎-B-C鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(21,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=49;
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
      }
    else if(Zone3==0&&SeqCount==35)
      {
        SeqCount=49;
        DisResult=0;
      }

    if(Zone4==1&&SeqCount>=49&&SeqCount<63)
      {
        if(SeqCount==49&&FaultType1==1)
          {//SeqCount涓9琛ㄧず瑕佹祴璇曠殑鏄娈礎-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(22,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=50;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==49&&FaultType1==0)
          SeqCount+=2;//濡傛灉4娈礎-E涓嶆祴璇曪紝SeqCount鍔鍒娈礏-E鐨凾estPoint2
        if(SeqCount==50)
          {//SeqCount涓0琛ㄧず瑕佹祴璇曠殑鏄娈礎-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(22,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=51;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==51&&FaultType2==1)
          {//SeqCount涓1琛ㄧず瑕佹祴璇曠殑鏄娈礏-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(23,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=52;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==51&&FaultType2==0)
          SeqCount+=2;//濡傛灉4娈礏-E涓嶆祴璇曪紝SeqCount鍔鍒娈礐-E鐨凾estPoint2
        if(SeqCount==52)
          {//SeqCount涓2琛ㄧず瑕佹祴璇曠殑鏄娈礏-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(23,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=53;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==53&&FaultType3==1)
          {//SeqCount涓3琛ㄧず瑕佹祴璇曠殑鏄娈礐-E鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(24,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=54;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==53&&FaultType3==0)
          SeqCount+=2;//濡傛灉4娈礐-E涓嶆祴璇曪紝SeqCount鍔鍒娈礎-B鐨凾estPoint2
        if(SeqCount==54)
          {//SeqCount涓4琛ㄧず瑕佹祴璇曠殑鏄娈礐-E鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(24,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=55;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==55&&FaultType4==1)
          {//SeqCount涓5琛ㄧず瑕佹祴璇曠殑鏄娈礎-B鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(25,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=56;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==55&&FaultType4==0)
          SeqCount+=2;//濡傛灉4娈礎-B涓嶆祴璇曪紝SeqCount鍔鍒娈礏-C鐨凾estPoint2
        if(SeqCount==56)
          {//SeqCount涓6琛ㄧず瑕佹祴璇曠殑鏄娈礎-B鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(25,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=57;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==57&&FaultType5==1)
          {//SeqCount涓7琛ㄧず瑕佹祴璇曠殑鏄娈礏-C鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(26,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=58;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==57&&FaultType5==0)
          SeqCount+=2;//濡傛灉4娈礏-C涓嶆祴璇曪紝SeqCount鍔鍒娈礐-A鐨凾estPoint2
        if(SeqCount==58)
          {//SeqCount涓8琛ㄧず瑕佹祴璇曠殑鏄娈礏-C鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(26,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=59;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==59&&FaultType6==1)
          {//SeqCount涓9琛ㄧず瑕佹祴璇曠殑鏄娈礐-A鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(27,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=60;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==59&&FaultType6==0)
          SeqCount+=2;//濡傛灉4娈礐-A涓嶆祴璇曪紝SeqCount鍔鍒娈礎-B-C鐨凾estPoint2
        if(SeqCount==60)
          {//SeqCount涓0琛ㄧず瑕佹祴璇曠殑鏄娈礐-A鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(27,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=61;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        if(SeqCount==61&&FaultType7==1)
          {//SeqCount涓1琛ㄧず瑕佹祴璇曠殑鏄娈礎-B-C鏁呴殰鐨凾estPoint2杩欎釜鐐
            OutputSignalCompute(28,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=62;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
        else if(SeqCount==61&&FaultType7==0)
          SeqCount+=2;//濡傛灉4娈礎-B-C涓嶆祴璇曪紝SeqCount鍔鍒3,琛ㄧず娴嬭瘯缁撴潫
        if(SeqCount==62)
          {//SeqCount涓2琛ㄧず瑕佹祴璇曠殑鏄娈礎-B-C鏁呴殰鐨凾estPoint1杩欎釜鐐
            OutputSignalCompute(28,1);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=63;
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
            return;
          }
      }



  }

//姝ゅ嚱鏁癘K
void thread1::OutputSignal(int fd)
  {

    if(Zone1==1)//濡傛灉瑕佹祴璇娈
      {
        if(FaultType1==1)//濡傛灉瑕佹祴璇娈电殑A-E鏁呴殰銆
          {
            OutputSignalCompute(1,0);
            Sequencer=1;
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType2==1)//濡傛灉瑕佹祴璇娈电殑B-E鏁呴殰銆
          {
            OutputSignalCompute(2,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=4;//灏嗚緭鍑篠equencer鏀逛负4銆
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType3==1)//濡傛灉瑕佹祴璇娈电殑C-E鏁呴殰銆
          {
            OutputSignalCompute(3,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=7;//灏嗚緭鍑篠equencer鏀逛负7銆
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType4==1)//濡傛灉瑕佹祴璇娈电殑A-B鏁呴殰銆
          {
            OutputSignalCompute(4,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=10;//灏嗚緭鍑篠equencer鏀逛负10銆
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType5==1)//濡傛灉瑕佹祴璇娈电殑B-C鏁呴殰銆
          {
            OutputSignalCompute(5,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=13;//灏嗚緭鍑篠equencer鏀逛负13銆
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType6==1)//濡傛灉瑕佹祴璇娈电殑C-A鏁呴殰銆
          {
            OutputSignalCompute(6,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=16;//灏嗚緭鍑篠equencer鏀逛负16銆
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType7==1)//濡傛灉瑕佹祴璇娈电殑A-B-C鏁呴殰銆
          {
            OutputSignalCompute(7,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=19;//灏嗚緭鍑篠equencer鏀逛负7銆
            FaultTime=FixupTime1;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
      }
    else if(Zone1==0&&Zone2==1)//濡傛灉瑕佹祴璇娈
      {
        if(FaultType1==1)//濡傛灉瑕佹祴璇娈电殑A-E鏁呴殰銆
          {
            OutputSignalCompute(8,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=22;//灏嗚緭鍑篠equencer鏀逛负8銆
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType2==1)//濡傛灉瑕佹祴璇娈电殑B-E鏁呴殰銆
          {
            OutputSignalCompute(9,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=24;//灏嗚緭鍑篠equencer鏀逛负9銆
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType3==1)//濡傛灉瑕佹祴璇娈电殑C-E鏁呴殰銆
          {
            OutputSignalCompute(10,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=26;//灏嗚緭鍑篠equencer鏀逛负10銆
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType4==1)//濡傛灉瑕佹祴璇娈电殑A-B鏁呴殰銆
          {
            OutputSignalCompute(11,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=28;//灏嗚緭鍑篠equencer鏀逛负11銆
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType5==1)//濡傛灉瑕佹祴璇娈电殑B-C鏁呴殰銆
          {
            OutputSignalCompute(12,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=30;//灏嗚緭鍑篠equencer鏀逛负12銆
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType6==1)//濡傛灉瑕佹祴璇娈电殑C-A鏁呴殰銆
          {
            OutputSignalCompute(13,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=32;//灏嗚緭鍑篠equencer鏀逛负13銆
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType7==1)//濡傛灉瑕佹祴璇娈电殑A-B-C鏁呴殰銆
          {
            OutputSignalCompute(14,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=34;//灏嗚緭鍑篠equencer鏀逛负14銆
            FaultTime=FixupTime2;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
      }
    else if(Zone1==0&&Zone2==0&&Zone3==1)//濡傛灉瑕佹祴璇娈
      {
        if(FaultType1==1)//濡傛灉瑕佹祴璇娈电殑A-E鏁呴殰銆
          {
            OutputSignalCompute(15,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=36;//灏嗚緭鍑篠equencer鏀逛负15銆

            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType2==1)//濡傛灉瑕佹祴璇娈电殑B-E鏁呴殰銆
          {
            OutputSignalCompute(16,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=38;//灏嗚緭鍑篠equencer鏀逛负16銆
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType3==1)//濡傛灉瑕佹祴璇娈电殑C-E鏁呴殰銆
          {
            OutputSignalCompute(17,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=40;//灏嗚緭鍑篠equencer鏀逛负17銆
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType4==1)//濡傛灉瑕佹祴璇娈电殑A-B鏁呴殰銆
          {
            OutputSignalCompute(18,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=42;//灏嗚緭鍑篠equencer鏀逛负18銆
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType5==1)//濡傛灉瑕佹祴璇娈电殑B-C鏁呴殰銆
          {
            OutputSignalCompute(19,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=44;//灏嗚緭鍑篠equencer鏀逛负19銆
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType6==1)//濡傛灉瑕佹祴璇娈电殑C-A鏁呴殰銆
          {
            OutputSignalCompute(20,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=46;//灏嗚緭鍑篠equencer鏀逛负20銆
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType7==1)//濡傛灉瑕佹祴璇娈电殑A-B-C鏁呴殰銆
          {
            OutputSignalCompute(21,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=48;//灏嗚緭鍑篠equencer鏀逛负21銆
            FaultTime=FixupTime3;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
      }
    else if(Zone1==0&&Zone2==0&&Zone3==0&&Zone4==1)//濡傛灉瑕佹祴璇娈
      {
        if(FaultType1==1)//濡傛灉瑕佹祴璇娈电殑A-E鏁呴殰銆
          {
            OutputSignalCompute(22,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=50;//灏嗚緭鍑篠equencer鏀逛负22銆
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType2==1)//濡傛灉瑕佹祴璇娈电殑B-E鏁呴殰銆
          {
            OutputSignalCompute(23,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=52;//灏嗚緭鍑篠equencer鏀逛负23銆
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType3==1)//濡傛灉瑕佹祴璇娈电殑C-E鏁呴殰銆
          {
            OutputSignalCompute(24,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=54;//灏嗚緭鍑篠equencer鏀逛负24銆
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType4==1)//濡傛灉瑕佹祴璇娈电殑A-B鏁呴殰銆
          {
            OutputSignalCompute(25,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=56;//灏嗚緭鍑篠equencer鏀逛负25銆
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType5==1)//濡傛灉瑕佹祴璇娈电殑B-C鏁呴殰銆
          {
            OutputSignalCompute(26,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=58;//灏嗚緭鍑篠equencer鏀逛负26銆
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType6==1)//濡傛灉瑕佹祴璇娈电殑C-A鏁呴殰銆
          {
            OutputSignalCompute(27,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=60;//灏嗚緭鍑篠equencer鏀逛负27銆
            FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
        else if(FaultType7==1)//濡傛灉瑕佹祴璇娈电殑A-B-C鏁呴殰銆
          {
            OutputSignalCompute(28,0);//璋冪敤鍚勪釜杈撳嚭鐩稿弬鏁拌绠楀嚱鏁般€
            Sequencer=62;//灏嗚緭鍑篠equencer鏀逛负28銆
                FaultTime=FixupTime4;
            OutputSequencer(fd);//璋冪敤杈撳嚭Sequencer鍑芥暟
          }
      }
  }


int  thread1::WarningInformation()
  {
    int k;
    if(Zone1==1)
      {
        if(FaultType1==1)
          {
            EndSequencer=3;
            if(OutputSignalCompute(1,0)||OutputSignalCompute(1,1)||OutputSignalCompute(1,-1))
              return 1;
            else
              k=0;
          }
        if(FaultType2==1)
          {
            EndSequencer=6;
            if(OutputSignalCompute(2,0)||OutputSignalCompute(2,1)||OutputSignalCompute(2,-1))
              return 1;
            else
              k=0;
          }
        if(FaultType3==1)
          {
            EndSequencer=9;
            if(OutputSignalCompute(3,0)||OutputSignalCompute(3,1)||OutputSignalCompute(3,-1))
              return 1;
            else
              k=0;
          }
        if(FaultType4==1)
          {
            EndSequencer=12;
            if(OutputSignalCompute(4,0)||OutputSignalCompute(4,1)||OutputSignalCompute(4,-1))
              return 1;
            else
              k=0;
          }
        if(FaultType5==1)
          {
            EndSequencer=15;
            if(OutputSignalCompute(5,0)||OutputSignalCompute(5,1)||OutputSignalCompute(5,-1))
              return 1;
            else
              k=0;
          }
        if(FaultType6==1)
          {
            EndSequencer=18;
            if(OutputSignalCompute(6,0)||OutputSignalCompute(6,1)||OutputSignalCompute(6,-1))
              return 1;
            else
              k=0;
          }
        if(FaultType7==1)
          {
            EndSequencer=21;
            if(OutputSignalCompute(7,0)||OutputSignalCompute(7,1)||OutputSignalCompute(7,-1))
              return 1;
            else
              k=0;
          }
      }
    if(Zone2==1)
      {
        if(FaultType1==1)
          {
            EndSequencer=23;
            if(OutputSignalCompute(8,0)||OutputSignalCompute(8,1))
              return 1;
            else
              k=0;
          }
        if(FaultType2==1)
          {
            EndSequencer=25;
            if(OutputSignalCompute(9,0)||OutputSignalCompute(9,1))
              return 1;
            else
              k=0;
          }
        if(FaultType3==1)
          {
            EndSequencer=27;
            if(OutputSignalCompute(10,0)||OutputSignalCompute(10,1))
              return 1;
            else
              k=0;
          }
        if(FaultType4==1)
          {
            EndSequencer=29;
            if(OutputSignalCompute(11,0)||OutputSignalCompute(11,1))
              return 1;
            else
              k=0;
          }
        if(FaultType5==1)
          {
            EndSequencer=31;
            if(OutputSignalCompute(12,0)||OutputSignalCompute(12,1))
              return 1;
            else
              k=0;
          }
        if(FaultType6==1)
          {
            EndSequencer=33;
            if(OutputSignalCompute(13,0)||OutputSignalCompute(13,1))
              return 1;
            else
              k=0;
          }
        if(FaultType7==1)
          {
            EndSequencer=35;
            if(OutputSignalCompute(14,0)||OutputSignalCompute(14,1))
              return 1;
            else
              k=0;
          }
      }
    if(Zone3==1)
      {
        if(FaultType1==1)
          {
            EndSequencer=37;
            if(OutputSignalCompute(15,0)||OutputSignalCompute(15,1))
              return 1;
            else k=0;
          }
        if(FaultType2==1)
          {
            EndSequencer=39;
            if(OutputSignalCompute(16,0)||OutputSignalCompute(16,1))
              return 1;
            else
              k=0;
          }
        if(FaultType3==1)
          {
            EndSequencer=41;
            if(OutputSignalCompute(17,0)||OutputSignalCompute(17,1))
              return 1;
            else
              k=0;
          }
        if(FaultType4==1)
          {
            EndSequencer=43;
            if(OutputSignalCompute(18,0)||OutputSignalCompute(18,1))
              return 1;
            else
              k=0;
          }
        if(FaultType5==1)
          {
            EndSequencer=45;
            if(OutputSignalCompute(19,0)||OutputSignalCompute(19,1))
              return 1;
            else
              k=0;
          }
        if(FaultType6==1)
          {
            EndSequencer=47;
            if(OutputSignalCompute(20,0)||OutputSignalCompute(20,1))
              return 1;
            else
              k=0;
          }
        if(FaultType7==1)
          {
            EndSequencer=49;
            if(OutputSignalCompute(21,0)||OutputSignalCompute(21,1))
              return 1;
            else
              k=0;
          }
      }
    if(Zone4==1)
      {
        if(FaultType1==1)
          {
            EndSequencer=51;
            if(OutputSignalCompute(22,0)||OutputSignalCompute(22,1))
              return 1;
            else
              k=0;
          }
        if(FaultType2==1)
          {
            EndSequencer=53;
            if(OutputSignalCompute(23,0)||OutputSignalCompute(23,1))
              return 1;
            else
              k=0;
          }
        if(FaultType3==1)
          {
            EndSequencer=55;
            if(OutputSignalCompute(24,0)||OutputSignalCompute(24,1))
              return 1;
            else
              k=0;
          }
        if(FaultType4==1)
          {
            EndSequencer=57;
            if(OutputSignalCompute(25,0)||OutputSignalCompute(25,1))
              return 1;
            else
              k=0;
          }
        if(FaultType5==1)
          {
            EndSequencer=59;
            if(OutputSignalCompute(26,0)||OutputSignalCompute(26,1))
              return 1;
            else
              k=0;
          }
        if(FaultType6==1)
          {
            EndSequencer=61;
            if(OutputSignalCompute(27,0)||OutputSignalCompute(27,1))
              return 1;
            else
              k=0;
          }
        if(FaultType7==1)
          {
            EndSequencer=63;
            if(OutputSignalCompute(28,0)||OutputSignalCompute(28,1))
              return 1;
            else
              k=0;
          }
      }
    return k;
  }



