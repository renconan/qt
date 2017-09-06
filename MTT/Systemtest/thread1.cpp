#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include "Config.h"
#include "Testcom2.h"
#include "thread1.h"
#include "Systestseq.h"
#include "Gpio.h"
#include "Uartset.h"
#include "em335x_drivers.h"
#include "ComplexCaculate.h"
#include <QSemaphore>

QSemaphore sem(1);
TEMPRORARYDATA *temprorarydata;

int info_num=0;

extern int  current_pos_index1;     //当前位置
extern int  current_pos_index2;     //当前位置


extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

//extern variate define
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
extern float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;


extern int   input_par;
//input_par为参数是否处于编辑状态的标志。input_par=0：未编辑；input_par=1：编辑状态�
//Decimal为小数点的输入与否标志，Decimal=0：未输入；Decimal=1：已经输入小数点�
//sige_flag为负号的输入与否的标志。sige_flag=0：未输入；sige_flag=1：已经输入负数�

extern long auto_i,IRQcnt,auto_count,tice;
//中断程序的变量�

extern int   cx,cy,cx_start,cx_mid,cx_end,starttest,low,row,page;
extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC,TestResult;
extern float LogicResolution,SystemFrec,SystemVolt;
extern int   FaultForm,FaultType;
extern float KeepTime,LoadCurrent,Impedance,ImpedanceAngle,FixCurrent,FixTime,KL,WaitTime,DelayTime;
extern float timeout,FristTripTime,CloseTripTime,SecondTripTime;
extern int exit_par,save_f;
//exit_par：退出程序标志。save_f：保存结果标志，�表示有新的测试结果，可以保存，为0表示没有新的测试结果
extern int sys[4];
extern float LogicResolution,SystemFrec,SystemVolt;
extern int   VzOutput3U0;
extern float VzOutputAc,VzOutputDc;

extern float  V1PerFaultValue,V2PerFaultValue,V3PerFaultValue,I1PerFaultValue,I2PerFaultValue,I3PerFaultValue;
extern float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase,I1PerFaultPhase,I2PerFaultPhase,I3PerFaultPhase;
extern float  V1FaultValue,V2FaultValue,V3FaultValue,I1FaultValue,I2FaultValue,I3FaultValue;
extern float  V1FaultPhase,V2FaultPhase,V3FaultPhase,I1FaultPhase,I2FaultPhase,I3FaultPhase;
extern float  V1PostFaultValue,V2PostFaultValue,V3PostFaultValue,I1PostFaultValue,I2PostFaultValue,I3PostFaultValue;
extern float  V1PostFaultPhase,V2PostFaultPhase,V3PostFaultPhase,I1PostFaultPhase,I2PostFaultPhase,I3PostFaultPhase;
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;


extern  bool  Busvolcon;
extern  float KLphase;

extern bool Threadrunstate;


extern  float Postfaulttime;
extern  int   dcouttype;

extern bool SoftStartEndflag;

#define LEDNUM GPIO8

thread1::thread1(QObject *parent) :
    QThread(parent)
{
    temprorarydata=new TEMPRORARYDATA;
}




void thread1::DisplayResult(int ResultSum)
{
  emit  SIG_testresult(ResultSum);
}

void thread1::ReadTripTimeLogic(int fd)
{
        int t1=0,t2=0,t3=0,t4=0,t5=0,t6=0,t7=0,t8=0,t9=0,t10=0,t11=0,t12=0;
        unsigned char signature;
        double u,v,w;
        float Temp;
        static double SequencerTime2,SequencerTime3,SequencerTime5,SequencerTime7,SequencerTime9,SequencerTime11;
        artExec(fd,"inp:buf:get?",result,15);//从DSP得到开关量接点信息�
        u=result[7];
        v=result[8];
        w=result[9];
        signature=int(result[13]&0x7f);
        switch(signature)
          {
            case 1:
              t1=1;
              break;
            case 2:
              t2=1;
              SequencerTime2=gettime(u,v,w);//计算从开始测试到sequencer的Step2的时间�
              break;
            case 3:
              SequencerTime3=gettime(u,v,w);//计算从开始测试到sequencer的Step3的时间�
              t3=1;
              break;
            case 4:
              t4=1;
              break;
            case 5:
              t5=1;
              SequencerTime5=gettime(u,v,w);
              break;
            case 6:
              t6=1;
              break;
            case 7:
              SequencerTime7=gettime(u,v,w);//计算从开始测试到sequencer的Step5的时间�
              t7=1;
              break;
            case 8:
              t8=1;
              break;
            case 9:
              SequencerTime9=gettime(u,v,w);
              t9=1;
              break;
            case 10:
              t10=1;
              break;
            case 11:
              t11=1;
              SequencerTime11=gettime(u,v,w);//计算从开始测试到sequencer的Step9的时间�
              break;
            case 12:
              t12=1;
              break;
          }

        if(t3==1)
          {
            FristTripTime=SequencerTime3-SequencerTime2-LogicResolution/1000;
            Temp=FristTripTime*1000;
            Temp=ceil(Temp);
            FristTripTime=Temp/1000;
          }
        if(t4==1)//一次故障保护未动作
          {
            FristTripTime=-1;
            //CloseTripTime=-1;
          }
        if(t7==1)//故障形式为永久性故障时，重合闸保护动作
          {
            CloseTripTime=SequencerTime7-SequencerTime5-LogicResolution/1000;
            Temp=CloseTripTime*1000;
            Temp=ceil(Temp);
            CloseTripTime=Temp/1000;//将重合闸动作时间的限制在小数点后最多只有三�
          }
        if(t6==1)//故障形式为永久性故障时，重合闸保护未动�
          {
            CloseTripTime=-1;
            //if(FaultForm==2)//�����Թ���
              //SecondTripTime=-1;
          }
        if(t10==1&&FaultForm==2)//故障形式为永久性故障时，二次故障保护未动作
          SecondTripTime=-1;
        if(t11==1&&FaultForm==2)//故障形式为永久性故障时，二次故障保护动�
          {
            SecondTripTime=SequencerTime11-SequencerTime9-DelayTime-LogicResolution/1000;
            Temp=SecondTripTime*1000;
            Temp=ceil(Temp);
            SecondTripTime=Temp/1000;//将二次动作时间的限制在小数点后最多只有三�
          }
        if(t12==1)
          {
            output_signal_end(fd);
            GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
            //ClearLED();
            starttest=0;
            save_f=1;
            TestResult=1;

            DisplayResult(1);
          }
}


int thread1::ErrorWarning(int fault)
      {
    switch(fault)
          {
              case 1://故障类型为：A-E
              if(V1FaultValue>SystemVolt)
                {//单相接地故障 ，发生故�故障相的幅值范围在0~Vnom�
                  emit SIG_sendwarning(1);
                  return 1;
                }
              break;
            case 2://故障类型为：B-E
              if(V2FaultValue>SystemVolt)
                {//单相接地故障 ，发生故�故障相的幅值范围在0~Vnom�
                  emit SIG_sendwarning(1);
                  return 1;
                }
              break;
            case 3://故障类型为：C-E
              if(V3FaultValue>SystemVolt)
                {//单相接地故障 ，发生故�故障相的幅值范围在0~Vnom�
                 emit SIG_sendwarning(1);
                  return 1;
                }
              break;
            case 4://故障类型为：A-B
              if(V1FaultValue>SystemVolt)
                {//相间故障 ，发生故�故障相的幅值范围在0.5*Vnom~Vnom�
                  emit SIG_sendwarning(1);
                  return 1;
                }
              if(V1FaultValue<SystemVolt/2)
                {//相间故障 ，发生故�故障相的幅值范围在0.5*Vnom~Vnom�
                emit SIG_sendwarning(2);
                  return 1;
                }
              break;
            case 5://故障类型为：B-C
              if(V2FaultValue>SystemVolt)
                {//相间故障 ，发生故�故障相的幅值范围在0.5*Vnom~Vnom�
                  emit SIG_sendwarning(1);
                  return 1;
                }
              if(V2FaultValue<SystemVolt/2)
                {//相间故障 ，发生故�故障相的幅值范围在0.5*Vnom~Vnom�
                  emit SIG_sendwarning(2);
                  return 1;
                }
              break;
            case 6://故障类型为：C-A
              if(V3FaultValue>SystemVolt)
                {//相间故障 ，发生故�故障相的幅值范围在0.5*Vnom~Vnom�
                  emit SIG_sendwarning(1);
                  return 1;
                }
              if(V3FaultValue<SystemVolt/2)
                {//相间故障 ，发生故�故障相的幅值范围在0.5*Vnom~Vnom�
                emit SIG_sendwarning(2);
                  return 1;
                }
              break;
            case 7://故障类型为：A-B-C
              if(V1FaultValue>SystemVolt)
                {//三相相间故障 ，发生故�故障相的幅值范围在0~Vnom�
                  emit SIG_sendwarning(1);
                  return 1;
                }
              break;
          }
        return 0;
      }


void thread1::LLLNormalFaultPar() //三相 相间故障
  { V1PerFaultValue=SystemVolt;//57.73;
    V2PerFaultValue=SystemVolt;//57.73;
    V3PerFaultValue=SystemVolt;//57.73;
    I1PerFaultValue=LoadCurrent;
    I2PerFaultValue=LoadCurrent;
    I3PerFaultValue=LoadCurrent;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //故障前电压每相输�7.73V，相位对称，电流输出��

    V1PostFaultValue=SystemVolt;//SystemVolt;//57.73;
    V2PostFaultValue=SystemVolt;//57.73;
    V3PostFaultValue=SystemVolt;//57.73;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;
    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
  }

void thread1::LLNormalFaultPar()
  { //两相相间故障 ，故障前和故障后参数
    V1PerFaultValue=SystemVolt;//57.73;
    V2PerFaultValue=SystemVolt;//57.73;
    V3PerFaultValue=SystemVolt;//57.73;
    I1PerFaultValue=LoadCurrent;
    I2PerFaultValue=LoadCurrent;
    I3PerFaultValue=LoadCurrent;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //故障前电压每相输�7.73V，相位对称，电流输出��

    V1PostFaultValue=SystemVolt;//57.73;
    V2PostFaultValue=SystemVolt;//57.73;
    V3PostFaultValue=SystemVolt;//57.73;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;
    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //故障后电压每相输�7.73V，相位对称，电流输出��
  }

void thread1::LNNormalFaultPar()
  {//单相接地故障 ，故障前和故障后参数
    V1PerFaultValue=SystemVolt;//57.73;
    V2PerFaultValue=SystemVolt;//57.73;
    V3PerFaultValue=SystemVolt;//57.73;
    I1PerFaultValue=LoadCurrent;
    I2PerFaultValue=LoadCurrent;
    I3PerFaultValue=LoadCurrent;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //故障前电压每相输�7.73V，相位对称，电流输出��

    V1PostFaultValue=SystemVolt;//57.73;
    V2PostFaultValue=SystemVolt;//57.73;
    V3PostFaultValue=SystemVolt;//57.73;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;
    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //故障后电压每相输�7.73V，相位对称，电流输出��
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

    V1PerFaultPhase=V1FaultPhase-360*SystemFrec*2*KeepTime;
    V2PerFaultPhase=V2FaultPhase-360*SystemFrec*2*KeepTime;
    V3PerFaultPhase=V3FaultPhase-360*SystemFrec*2*KeepTime;
    V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
    V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
    V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;

    I1PerFaultPhase=I1FaultPhase-360*SystemFrec*2*KeepTime;
    I2PerFaultPhase=I2FaultPhase-360*SystemFrec*2*KeepTime;
    I3PerFaultPhase=I3FaultPhase-360*SystemFrec*2*KeepTime;
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








int thread1::OutputSignalCompute(int Fault)
{

        int ErrorFlag;
        COMPLEX C;
        C.x=3*KL*cos(KLphase*3.1415/180)+2;
        C.y=3*KL*sin(KLphase*3.1415/180);
        float Angel_C=CaculatePhase(C.y,C.x);
        if(Angel_C>=180)
            Angel_C-=360;
        else if(Angel_C<=-180)
          Angel_C+=360;

        switch(Fault)
          {
            case 1:
              V1Output=1;
              V2Output=1;
              V3Output=1;
              I1Output=1;
              I2Output=0;
              I3Output=0;
              LNNormalFaultPar();
              I1FaultValue=FixCurrent;
              if(Busvolcon==true)
            {
              COMPLEX Va=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
              V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
              V2FaultValue=SystemVolt;//SystemVolt;
              V3FaultValue=SystemVolt;//SystemVolt;
              I2FaultValue=0;
              I3FaultValue=0;
              //����ӵع���ʱ��δ�������ϵĵ�ѹΪVnom��SystemVolt����δ�������ϵĵ���Ϊ0��
              V1FaultPhase=CaculatePhase(Va.y,Va.x);
              V2FaultPhase=-120+V1FaultPhase;
              V3FaultPhase=120+V1FaultPhase;
            }
            else
              {
                  I2FaultValue=0;
                  I3FaultValue=0;

                  COMPLEX Va=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
                  COMPLEX Vb=VbCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
                  COMPLEX Vc=VcCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);

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

              ErrorFlag=ErrorWarning(1);//判断参数是否设置正确，第一个参数表示段，第二个参数表示故障类型
              if(ErrorFlag==1)
              return 1;

              I1FaultPhase=0-ImpedanceAngle-Angel_C;
              I2FaultPhase=I1FaultPhase-120;
              I3FaultPhase=I1FaultPhase+120;
              LNNormalFaultPar();
              break;
            case 2://B-E故障
              V1Output=1;
              V2Output=1;
              V3Output=1;
              I1Output=1;
              I2Output=1;
              I3Output=1;
              LNNormalFaultPar();
              I2FaultValue=FixCurrent;

              if(Busvolcon==true)
            {
              COMPLEX Vb=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
              V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
              V1FaultValue=SystemVolt;//57.73;
              V3FaultValue=SystemVolt;//57.73;
              I1FaultValue=LoadCurrent;
              I3FaultValue=LoadCurrent;
              //单相接地故障 ，未发生故障的电压为Vnom�7.73），未发生故障的电流��
              V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
              V1FaultPhase=V2FaultPhase+120;
              V3FaultPhase=V2FaultPhase-120;
            }
            else
            {
                I1FaultValue=0;
                I3FaultValue=0;
                COMPLEX Vb=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
                COMPLEX Vc=VbCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
                COMPLEX Va=VcCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);

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

              ErrorFlag=ErrorWarning(2);//判断参数是否设置正确，第一个参数表示段，第二个参数表示故障类型
              if(ErrorFlag==1)
              return 1;

              I2FaultPhase=0-ImpedanceAngle-Angel_C;
              I1FaultPhase=I2FaultPhase+120;
              I3FaultPhase=I2FaultPhase-120;
              LNNormalFaultPar();
              break;
            case 3://C-E故障
              V1Output=1;
              V2Output=1;
              V3Output=1;
              I1Output=1;
              I2Output=1;
              I3Output=1;
              LNNormalFaultPar();
              I3FaultValue=FixCurrent;

              if(Busvolcon==true)
             {
              COMPLEX Vc=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
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
                  COMPLEX Vc=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
                  COMPLEX Va=VbCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
                  COMPLEX Vb=VcCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);


                  V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
                  V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
                  V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
                  V1FaultPhase=CaculatePhase(Va.y,Va.x);
                  V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
                  V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
                  //�����ʱ���ǹ��ϵ�ѹ����λ���䣬
                  V1FaultPhase=fmod(V1FaultPhase,360);
                  V2FaultPhase=fmod(V2FaultPhase,360);
                  V3FaultPhase=fmod(V3FaultPhase,360);

              }

              ErrorFlag=ErrorWarning(3);//判断参数是否设置正确，第一个参数表示段，第二个参数表示故障类型
              if(ErrorFlag==1)
              return 1;

              I3FaultPhase=0-ImpedanceAngle-Angel_C;
              I1FaultPhase=I3FaultPhase-120;
              I2FaultPhase=I3FaultPhase+120;
              LNNormalFaultPar();
              break;
            case 4://A-B故障
            {
              V1Output=1;
              V2Output=1;
              V3Output=1;
              I1Output=1;
              I2Output=1;
              I3Output=1;
              LLNormalFaultPar();
              I1FaultValue=FixCurrent;
              I2FaultValue=FixCurrent;
              I3FaultValue=I3PerFaultValue;

              COMPLEX Va1=V1LLCaculate(FixCurrent,Impedance,SystemVolt);
              COMPLEX Vb1=V2LLCaculate(FixCurrent,Impedance,SystemVolt);
              V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
              V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);

              V3FaultValue=V3PerFaultValue;//相间故障�非故障相电压幅�故障前相应的相电压幅�
              V3FaultPhase=0;//相间故障�非故障相电压相位=故障前相应的相电压的相位
              V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
              V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);

              V1FaultPhase=fmod(V1FaultPhase,360);
              V2FaultPhase=fmod(V2FaultPhase,360);

              ErrorFlag=ErrorWarning(4);//判断参数是否设置正确，第一个参数表示段，第二个参数表示故障类型
              if(ErrorFlag==1)
              return 1;


              I3FaultPhase=V3FaultPhase-ImpedanceAngle;
              I1FaultPhase=I3FaultPhase-90;
              I2FaultPhase=I3FaultPhase+90;

              LLNormalFaultPar();//故障前和故障后参�
          }

              break;
            case 5://B-C故障
            {  V1Output=1;
              V2Output=1;
              V3Output=1;
              I1Output=1;
              I2Output=1;
              I3Output=1;

              LLNormalFaultPar();//故障前和故障后参�
              I2FaultValue=FixCurrent;//相间故障 ，故障相电流的幅�固定电流
              I3FaultValue=FixCurrent;//相间故障 ，故障相电流的幅�固定电流
              I1FaultValue=I1PerFaultValue;//相间故障 ，非故障相电流的幅�故障前相应的相电流幅�

              V1FaultValue=V1PerFaultValue;//相间故障�非故障相电压幅�故障前相应的相电压幅�
              V1FaultPhase=0;//相间故障�非故障相电压相位=故障前相应的相电压的相位
              COMPLEX Vb2=V1LLCaculate(FixCurrent,Impedance,SystemVolt);
              COMPLEX Vc2=V2LLCaculate(FixCurrent,Impedance,SystemVolt);
              V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
              V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);
              //调用出错处理程序，如果返�，说明参数设置不对�
              V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
              V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);


              V2FaultPhase=fmod(V2FaultPhase,360);
              V3FaultPhase=fmod(V3FaultPhase,360);



              ErrorFlag=ErrorWarning(5);//判断参数是否设置正确，第一个参数表示段，第二个参数表示故障类型
              if(ErrorFlag==1)
              return 1;

              I1FaultPhase=V1FaultPhase-ImpedanceAngle;//相间故障 ,非故障相电流相位=故障前相应的相电流的相位
              I2FaultPhase=I1FaultPhase-90;
              I3FaultPhase=I1FaultPhase+90;




              LLNormalFaultPar();//故障前和故障后参�
          }

              break;
            case 6://C-A故障
            {  V1Output=1;
              V2Output=1;
              V3Output=1;
              I1Output=1;
              I2Output=1;
              I3Output=1;
              LLNormalFaultPar();//故障前和故障后参�
              I1FaultValue=FixCurrent;//相间故障 ，故障相电流的幅�固定电流
              I3FaultValue=FixCurrent;//相间故障 ，故障相电流的幅�固定电流
              I2FaultValue=I2PerFaultValue;//相间故障 ，非故障相电流的幅�故障前相应的相电流幅�

              V2FaultValue=V2PerFaultValue;//相间故障�非故障相电压幅�故障前相应的相电压幅�
              V2FaultPhase=0;//相间故障�非故障相电压相位=故障前相应的相电压的相位
              COMPLEX Vc3=V1LLCaculate(FixCurrent,Impedance,SystemVolt);
              COMPLEX Va3=V2LLCaculate(FixCurrent,Impedance,SystemVolt);
              V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
              V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);


              //调用出错处理程序，如果返�，说明参数设置不对�
              V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
              V1FaultPhase=CaculatePhase(Va3.y,Va3.x);


              V1FaultPhase=fmod(V1FaultPhase,360);
              V3FaultPhase=fmod(V3FaultPhase,360);



              ErrorFlag=ErrorWarning(6);//判断参数是否设置正确，第一个参数表示段，第二个参数表示故障类型
              if(ErrorFlag==1)
              return 1;


              I2FaultPhase=0-ImpedanceAngle;//相间故障 ,非故障相电流相位=故障前相应的相电流的相位
              I3FaultPhase=I2FaultPhase-90;
              I1FaultPhase=I2FaultPhase+90;


              LLNormalFaultPar();//故障前和故障后参�
          }

              break;
            case 7://A-B-C故障
              V1Output=1;
              V2Output=1;
              V3Output=1;
              I1Output=1;
              I2Output=1;
              I3Output=1;

              LLLNormalFaultPar();//故障前和故障后参�

              I1FaultValue=FixCurrent;//三相相间故障 ，故障相电流的幅�固定电流
              I2FaultValue=FixCurrent;//三相相间故障 ，故障相电流的幅�固定电流
              I3FaultValue=FixCurrent;//三相相间故障 ，故障相电流的幅�固定电流

              V1FaultValue=FixCurrent*Impedance;
              V2FaultValue=V1FaultValue;
              V3FaultValue=V1FaultValue;


              V1FaultPhase=V1PerFaultPhase;
              V2FaultPhase=V2PerFaultPhase;
              V3FaultPhase=V3PerFaultPhase;

              ErrorFlag=ErrorWarning(7);//判断参数是否设置正确，第一个参数表示段，第二个参数表示故障类型
              if(ErrorFlag==1)
              return 1;

              I1FaultPhase=V1FaultPhase-ImpedanceAngle;
              I2FaultPhase=I1FaultPhase-120;
              I3FaultPhase=I1FaultPhase+120;



              LLLNormalFaultPar();
              break;
          }




        return 0;
}


void thread1::key_deal_test_current(int fd)
{
       if(starttest==0)
         {
            emit SIG_Teststate(1);
            GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
            //SetLED();
            if(OutputSignalCompute(FaultType))
            {
                GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                //ClearLED();
                return;
            }
            V1PerFaultPhase=V1FaultPhase-360*SystemFrec*KeepTime;
            V2PerFaultPhase=V2FaultPhase-360*SystemFrec*KeepTime;
            V3PerFaultPhase=V3FaultPhase-360*SystemFrec*KeepTime;
            V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
            V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
            V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;

            I1PerFaultPhase=I1FaultPhase-360*SystemFrec*KeepTime;
            I2PerFaultPhase=I2FaultPhase-360*SystemFrec*KeepTime;
            I3PerFaultPhase=I3FaultPhase-360*SystemFrec*KeepTime;
            I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
            I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
            I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;






           emit SIG_changetab(2);
           artExec(fd,"out:dig:on(0x08)",result,0);
           OutputSequencer(fd);
           starttest=1;
           TestResult=0;
           DisplayResult(0);
         }
}



void thread1::key_deal_stop_current(int fd)
{
       if(starttest==1)
         {
           output_signal_end(fd);
           starttest=0;
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
          GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //�
          //ClearLED();
          key_deal_stop_current(temprorarydata->UartDev[0].uartfd);
          emit SIG_TestState();
    }
}





int  thread1::Buzzer(char Address1,char Address2,char Address3)
  {//sys[1]:过载，sys[2]:间歇，sys[3]:闭锁
    if(sys[1]==1)
      {//过载信息
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1过载
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2过载
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3过载
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1过载
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2过载
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    else if(sys[1]==0)
      {//过载信息
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1过载
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2过载
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3过载
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1过载
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2过载
        if((Address3&0x40)!=0x00&&V3Output==1)
          return -1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    if(sys[2]==1)
      {//间歇信息
        if((Address1&0x10)!=0x00)
          return 1;//I1间歇
        if((Address1&0x04)!=0x00)
          return 1;//I2间歇
        if((Address1&0x01)!=0x00)
          return 1;//I3间歇
        if((Address2&0x08)!=0x00)
          return 1;//V1间歇
        if((Address2&0x04)!=0x00)
          return 1;//V2间歇
        if((Address3&0x80)!=0x00)
          return 1;//V3间歇
      }
    else if(sys[2]==0)
      {//间歇信息
        if((Address1&0x10)!=0x00)
          return -1;//I1间歇
        if((Address1&0x04)!=0x00)
          return -1;//I2间歇
        if((Address1&0x01)!=0x00)
          return -1;//I3间歇
        if((Address2&0x08)!=0x00)
          return -1;//V1间歇
        if((Address2&0x04)!=0x00)
          return -1;//V2间歇
        if((Address3&0x80)!=0x00)
          return -1;//V3间歇
      }
    if(sys[3]==1)
      {//闭锁信息
        if((Address1&0x80)!=0x00)
          return 1;//I1闭锁
        if((Address1&0x40)!=0x00)
          return 1;//I2闭锁
        if((Address1&0x20)!=0x00)
          return 1;//I3闭锁
        if((Address2&0x20)!=0x00)
          return 1;//V1闭锁
        if((Address2&0x40)!=0x00)
          return 1;//V2闭锁
        if((Address2&0x01)!=0x00)
          return 1;//V3闭锁
      }
    else if(sys[3]==0)
      {//闭锁信息
        if((Address1&0x80)!=0x00)
          return -1;//I1闭锁
        if((Address1&0x40)!=0x00)
          return -1;//I2闭锁
        if((Address1&0x20)!=0x00)
          return -1;//I3闭锁
        if((Address2&0x20)!=0x00)
          return -1;//V1闭锁
        if((Address2&0x40)!=0x00)
          return -1;//V2闭锁
        if((Address2&0x01)!=0x00)
          return -1;//V3闭锁
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
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //不需要控�
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
    if(Buzzer(info_result1,info_result2,info_result3)==0) //不需要控�
      {
        //运行正常
          emit SIG_dspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1是第1路地址的DB9~DB15�
           emit SIG_dspstate(1);
            //IA过载
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
            emit SIG_dspstate(2);

            //IABC 温度保护
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //IC间歇
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
          {  //info_result1是第1路地址的DB9~DB15�

              emit SIG_dspstate(4);
            //IB过载
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1是第1路地址的DB8~DB15�

              emit SIG_dspstate(5);
           //IC闭锁
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1是第1路地址的DB9~DB15�
              emit SIG_dspstate(6);
          //IA间歇
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
          { //info_result2是第2路地址的DB9~DB15�
           //IC过载
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1是第1路地址的DB8~DB15�
          //IABC 电源保护
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1是第1路地址的DB9~DB15�
              emit SIG_dspstate(9);
          //I2间歇  ?温度保护间歇
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
          {//info_result2是第2路地址的DB9~DB15�
           //VA过载
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15�
              emit SIG_dspstate(11);
              //V2闭锁
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7�

              //V3间歇
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
          {//info_result2是第2路地址的DB9~DB15�
            //V2过载
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2是第2路地址的DB8~DB15� //DB8
           // VABC 温度保护
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2是第2路地址的DB9~DB15�
            emit SIG_dspstate(15);
           //VA间歇
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
          { //info_result3是第2路地址的DB0~DB7�

             emit SIG_dspstate(16);
            //VC过载
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2是第2路地址的DB8~DB15� //DB13
            emit SIG_dspstate(17);
            //VABC电源保护
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2是第2路地址的DB9~DB15�
            emit SIG_dspstate(18);


          //Vz过载

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
          {//info_result3是第2路地址的DB0~DB7�
            emit SIG_dspstate(19);

             //直流过载
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15�
          emit SIG_dspstate(20);

            // VABC 温度保护
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7�
            emit SIG_dspstate(21);
           //V3间歇
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



void thread1::stoprun()
{
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

    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //�

    //ClearLED();
    online(temprorarydata->UartDev[0].uartfd);


    read_max(temprorarydata->UartDev[0].uartfd);
    artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);
    if((result[7]&0xf0)==0x60)
         { //125V电压�
           MAX_V_VALUEDATA_AC=300;
           MIN_V_VALUEDATA_AC=0;
           MAX_V_VALUEDATA_DC=424;
           MIN_V_VALUEDATA_DC=-424;
         }
        else if((result[7]&0xf0)==0x80)
        { //125V电压�
          MAX_V_VALUEDATA_AC=125;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=176;
          MIN_V_VALUEDATA_DC=-176;
        }
      else if((result[7]&0xf0)==0x00)
        { //75V电压�
          MAX_V_VALUEDATA_AC=75;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=110;
          MIN_V_VALUEDATA_DC=-110;
        }
      if((result[7]&0x0f)==0x01)
        { //30A电流�
          MAX_I_VALUEDATA_AC=30;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=42;
          MIN_I_VALUEDATA_DC=-42;
        }
      else if((result[7]&0x0f)==0x08)
        { //40A电流�
          MAX_I_VALUEDATA_AC=40;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=56;
          MIN_I_VALUEDATA_DC=-56;
        }
      else if((result[7]&0x0f)==0x05)
        { //50A电流�
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
          emit SIG_TestState();
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //不需要更�
         info_flag1=result[8];
         info_flag4=result[9];
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
         info_flag2=result[8];
         info_flag3=result[9];
         logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //需要emit
         info_num=0;
       }
     }
     else {
          ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);
          if(info_num++>10)
         {
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //不需要更�
          info_flag1=result[8];
          info_flag4=result[9];
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
          info_flag2=result[8];
          info_flag3=result[9];
          logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //需要emit
          info_num=0;
          }}}
     else
     {


         tcflush(temprorarydata->UartDev[1].uartfd, TCIOFLUSH);
         msleep(500);

     }
      }

          starttest=0;
          output_signal_end(temprorarydata->UartDev[0].uartfd);
          artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
          close(temprorarydata->UartDev[1].uartfd);
          close(temprorarydata->UartDev[0].uartfd);

}





