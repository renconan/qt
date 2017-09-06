#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "Config.h"



#include "Testcom2.h"

extern  unsigned  char result[70],receive_data[70];
extern double out_time,change_timedata;
extern int   low,row,increase;
extern int   FreqTriple;
extern int   VariatyV1,VariatyV2,VariatyV3,VariatyVz,VariatyVz2,VariatyI1,VariatyI2,VariatyI3;
extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC,VzDC,Variable;
extern float V1Value,V1Phase,V1Frec;
extern float V2Value,V2Phase,V2Frec;
extern float V3Value,V3Phase,V3Frec;
extern float VzValue,VzPhase,VzFrec;
extern float I1Value,I1Phase,I1Frec;
extern float I2Value,I2Phase,I2Frec;
extern float I3Value,I3Phase,I3Frec;

extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,VzRelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase;
extern float CurStep,VolStep,PhaStep,FreStep,StepTime,StepNum,PreFaultTime,FaultTime,PostFaultTime;

extern float VaValueReviosn[10],VbValueReviosn[10],VcValueReviosn[10],VzValueReviosn[10];
extern float IaValueReviosn[10],IbValueReviosn[10],IcValueReviosn[10];
extern float V1OutputValue,V2OutputValue,V3OutputValue,VzOutputValue;
extern float I1OutputValue,I2OutputValue,I3OutputValue;
extern float SysFrec,SysVolt;
extern float  V1PerFaultValue,V2PerFaultValue,V3PerFaultValue,VzPerFaultValue,I1PerFaultValue,I2PerFaultValue,I3PerFaultValue;
extern float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase,VzPerFaultPhase,I1PerFaultPhase,I2PerFaultPhase,I3PerFaultPhase;

extern char   V1Output,V2Output,V3Output,VzOutput,I1Output,I2Output,I3Output;


bool checkiszero(float x)
{

    if ((x >= -0.0001) && (x <=0.0001))
    return true;
    else
    return false;
}



float  valuefrec(float Frequency,float Value,int type)
{
      float ValueFreqPara;
      if (type==1)//电压
      {
          if(Frequency<50)
           {
            ValueFreqPara = 0.9998 + 0.0002 * (1-0.001*(Frequency-50)*(Frequency-50));

           }
           else
            ValueFreqPara = 0.9998 + 0.0002  * (1 + 0.0002 * (Frequency - 50)*(Frequency - 50));

      }
      else{//电流
            if(Frequency <50)
                ValueFreqPara = 0.9998 +0.0002*(1 - 0.0005*(Frequency - 50)*(Frequency - 50));
            else
              ValueFreqPara = 0.9998 + 0.0002 *(1 + 0.00012*(Frequency - 50)*(Frequency - 50));
          }
     return ValueFreqPara*Value;
}


void ChangeOutputValue()
  {

     V1OutputValue=valuefrec(V1Frec,V1Value,1);
     V2OutputValue=valuefrec(V2Frec,V2Value,1);
     V3OutputValue=valuefrec(V3Frec,V3Value,1);
     VzOutputValue=valuefrec(VzFrec,VzValue,1);

     I1OutputValue=valuefrec(I1Frec,I1Value,2);
     I2OutputValue=valuefrec(I2Frec,I2Value,2);
     I3OutputValue=valuefrec(I3Frec,I3Value,2);
  }



void output_signal_manual(int k,int fd)  //manual way
  {
    ChangeOutputValue();//2016-4-28修改，电流从0开始增加，点第一次增加后未出值，再点一下才出值
    //原因为第一次的值没输入刷新，ChangeOutputValue()更新 I1OutputValue，I2OutputValue，I3OutputValue

    if(fabs(I1OutputValue)<=0.00001)
       artExec(fd,"out:dig:off(0x04)",result,0);//IA放大器对应DB2
    else
       artExec(fd,"out:dig:on(0x04)",result,0);

    if(fabs(I2OutputValue)<=0.00001)
       artExec(fd,"out:dig:off(0x10)",result,0);//IB放大器对应DB4
    else
       artExec(fd,"out:dig:on(0x10)",result,0);

    if(fabs(I3OutputValue)<=0.00001)
       artExec(fd,"out:dig:off(0x80)",result,0);//IC放大器对应DB7
    else
       artExec(fd,"out:dig:on(0x80)",result,0);


   // artExec(fd,"inp:dig(1):thres(1)",result,0);  //打开放大器
    artExec(fd,"inp:dig:interval(1)",result,0);
    artExec(fd,"out:ana:user:mem?",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    if(k!=2)
  {  artExec(fd,"inp:buf:sam(bin,1)",result,0);
     out_time=-1;
     artExec(fd,"seq:wait(0,1)",result,0);
  }
    if(V1Value==0)
      V1Output=0;
    else
      V1Output=1;

    if(V2Value==0)
      V2Output=0;
    else
      V2Output=1;

    if(V3Value==0)
      V3Output=0;
    else
      V3Output=1;

    if(VzValue==0)
        VzOutput=0;
    else
        VzOutput=1;



    if(fabs(I1Value)<=0.00001)
      I1Output=0;
    else
      I1Output=1;

    if(fabs(I2Value)<=0.00001)
      I2Output=0;
    else
      I2Output=1;

    if(fabs(I3Value)<=0.00001)
      I3Output=0;
    else
      I3Output=1;

    if(k==0)
      {


            channel_input(fd,1,V1OutputValue,V1Phase,V1Frec/FreqTriple,V1DC,1,0,0,0);
            artExec(fd,"out:ana:v(1:1):on",result,0);
            channel_input(fd,1,V2OutputValue,V2Phase,V2Frec/FreqTriple,V2DC,2,0,0,0);
            artExec(fd,"out:ana:v(1:2):on",result,0);
            channel_input(fd,1,V3OutputValue,V3Phase,V3Frec/FreqTriple,V3DC,3,0,0,0);
            artExec(fd,"out:ana:v(1:3):on",result,0);

            channel_input(fd,1,VzOutputValue,VzPhase,VzFrec/FreqTriple,VzDC,4,0,0,0);
            artExec(fd,"out:ana:v(1:4):on",result,0);

            channel_input(fd,0,I1OutputValue,I1Phase,I1Frec/FreqTriple,I1DC,1,0,0,0);
            artExec(fd,"out:ana:i(1:1):on",result,0);
            channel_input(fd,0,I2OutputValue,I2Phase,I2Frec/FreqTriple,I2DC,2,0,0,0);
            artExec(fd,"out:ana:i(1:2):on",result,0);
            channel_input(fd,0,I3OutputValue,I3Phase,I3Frec/FreqTriple,I3DC,3,0,0,0);
            artExec(fd,"out:ana:i(1:3):on",result,0);

      }
    else if(k==1)
      {
        switch(Variable)
          {
            case 1:
              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;
              ChangeOutputValue();



              artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
              channel_input(fd,1,V1OutputValue,360,V1Frec/FreqTriple,V1DC,1,0,0,0);
              artExec(fd,"out:ana:v(1:1):on",result,0);

              artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
              channel_input(fd,1,V2OutputValue,360,V2Frec/FreqTriple,V2DC,2,0,0,0);
              artExec(fd,"out:ana:v(1:2):on",result,0);

              artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
              channel_input(fd,1,V3OutputValue,360,V3Frec/FreqTriple,V3DC,3,0,0,0);
              artExec(fd,"out:ana:v(1:3):on",result,0);

              artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
              channel_input(fd,1,VzOutputValue,360,VzFrec/FreqTriple,VzDC,4,0,0,0);
              artExec(fd,"out:ana:v(1:4):on",result,0);



              artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
              channel_input(fd,0,I1OutputValue,360,I1Frec/FreqTriple,I1DC,1,0,0,0);
              artExec(fd,"out:ana:i(1:1):on",result,0);

              artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
              channel_input(fd,0,I2OutputValue,360,I2Frec/FreqTriple,I2DC,2,0,0,0);
              artExec(fd,"out:ana:i(1:2):on",result,0);

              artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
              channel_input(fd,0,I3OutputValue,360,I3Frec/FreqTriple,I3DC,3,0,0,0);
              artExec(fd,"out:ana:i(1:3):on",result,0);
              break;
            case 2:





              artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
              channel_input(fd,1,V1OutputValue,360,V1Frec/FreqTriple,V1DC,1,0,0,0);
              artExec(fd,"out:ana:v(1:1):on",result,0);

              artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
              channel_input(fd,1,V2OutputValue,360,V2Frec/FreqTriple,V2DC,2,0,0,0);
              artExec(fd,"out:ana:v(1:2):on",result,0);

              artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
              channel_input(fd,1,V3OutputValue,360,V3Frec/FreqTriple,V3DC,3,0,0,0);
              artExec(fd,"out:ana:v(1:3):on",result,0);

              artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
              channel_input(fd,1,VzOutputValue,360,VzFrec/FreqTriple,VzDC,4,0,0,0);
              artExec(fd,"out:ana:v(1:4):on",result,0);



              artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
              channel_input(fd,0,I1OutputValue,360,I1Frec/FreqTriple,I1DC,1,0,0,0);
              artExec(fd,"out:ana:i(1:1):on",result,0);
              artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
              channel_input(fd,0,I2OutputValue,360,I2Frec/FreqTriple,I2DC,2,0,0,0);
              artExec(fd,"out:ana:i(1:2):on",result,0);
              artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
              channel_input(fd,0,I3OutputValue,360,I3Frec/FreqTriple,I3DC,3,0,0,0);
              artExec(fd,"out:ana:i(1:3):on",result,0);
              break;
            case 3:
              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;
              ChangeOutputValue();




              artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
              channel_input(fd,1,V1OutputValue,360,V1Frec/FreqTriple,V1DC,1,0,0,0);
              artExec(fd,"out:ana:v(1:1):on",result,0);

              artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
              channel_input(fd,1,V2OutputValue,360,V2Frec/FreqTriple,V2DC,2,0,0,0);
              artExec(fd,"out:ana:v(1:2):on",result,0);

              artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
              channel_input(fd,1,V3OutputValue,360,V3Frec/FreqTriple,V3DC,3,0,0,0);
              artExec(fd,"out:ana:v(1:3):on",result,0);

              artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
              channel_input(fd,1,VzOutputValue,360,VzFrec/FreqTriple,VzDC,4,0,0,0);
              artExec(fd,"out:ana:v(1:4):on",result,0);



              artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
              channel_input(fd,0,I1OutputValue,360,I1Frec/FreqTriple,I1DC,1,0,0,0);
              artExec(fd,"out:ana:i(1:1):on",result,0);

              artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
              channel_input(fd,0,I2OutputValue,360,I2Frec/FreqTriple,I2DC,2,0,0,0);
              artExec(fd,"out:ana:i(1:2):on",result,0);

              artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
              channel_input(fd,0,I3OutputValue,360,I3Frec/FreqTriple,I3DC,3,0,0,0);
              artExec(fd,"out:ana:i(1:3):on",result,0);
              break;
          }
         }
    else if(k==2)
         {
            //故障前参数
            V1PerFaultValue=SysVolt;//57.73;
            V2PerFaultValue=SysVolt;//57.73;
            V3PerFaultValue=SysVolt;//57.73;
            VzPerFaultValue=SysVolt;//57.73;
            I1PerFaultValue=0;
            I2PerFaultValue=0;
            I3PerFaultValue=0;

            V1PerFaultPhase=V1Phase-360*SysFrec*PreFaultTime;
            V2PerFaultPhase=V2Phase-360*SysFrec*PreFaultTime;
            V3PerFaultPhase=V3Phase-360*SysFrec*PreFaultTime;
            VzPerFaultPhase=VzPhase-360*SysFrec*PreFaultTime;
            V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
            V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
            V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
            VzPerFaultPhase=VzPerFaultPhase-((int)(VzPerFaultPhase/360))*360;


            I1PerFaultPhase=I1Phase-360*SysFrec*PreFaultTime;
            I2PerFaultPhase=I2Phase-360*SysFrec*PreFaultTime;
            I3PerFaultPhase=I3Phase-360*SysFrec*PreFaultTime;

            I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
            I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
            I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
            //触发模式相位连续修改2016-8-16
            if(I1OutputValue>=0.00001&&fabs(I2OutputValue)<0.00001&&fabs(I3OutputValue)<0.00001)
            {
                V1PerFaultPhase=V2PerFaultPhase+120;
                V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                V3PerFaultPhase=V2PerFaultPhase-120;
                V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
            }
            if(I2OutputValue>=0.00001&&fabs(I1OutputValue)<0.00001&&fabs(I3OutputValue)<0.00001)
            {
                V1PerFaultPhase=V3PerFaultPhase-120;
                V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                V2PerFaultPhase=V3PerFaultPhase+120;
                V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
            }
            if(I3OutputValue>=0.00001&&fabs(I2OutputValue)<0.00001&&fabs(I3OutputValue)<0.00001)
            {
                V2PerFaultPhase=V1PerFaultPhase-120;
                V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
                V3PerFaultPhase=V1PerFaultPhase+120;
                V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
            }
            if(I1OutputValue>=0.00001&&I2OutputValue>=0.00001&&fabs(I3OutputValue)<0.00001)
            {
                V1PerFaultPhase=V3PerFaultPhase-120;
                V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                V2PerFaultPhase=V3PerFaultPhase+120;
                V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
            }
            if(I2OutputValue>=0.00001&&I3OutputValue>=0.00001&&fabs(I1OutputValue)<0.00001)
            {
                V2PerFaultPhase=V1PerFaultPhase-120;
                V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
                V3PerFaultPhase=V1PerFaultPhase+120;
                V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
            }
            if(I3OutputValue>=0.00001&&I1OutputValue>=0.00001&&fabs(I2OutputValue)<0.00001)
            {
                V1PerFaultPhase=V2PerFaultPhase+120;
                V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                V3PerFaultPhase=V2PerFaultPhase-120;
                V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
            }

            V1RelaPhase=0;
            V2RelaPhase=0;
            V3RelaPhase=0;
            VzRelaPhase=0;
            I1RelaPhase=0;
            I2RelaPhase=0;
            I3RelaPhase=0;

            //故障前电压每相输出57.73V，相位对称，电流输出为0。



            //第一步  故障前
            artExec(fd,"others:steptime(1)",result,0);//2009-6-23
            channel_input(fd,1,V1PerFaultValue,V1PerFaultPhase,SysFrec/FreqTriple,V1DC,1,0,0,0);
            artExec(fd,"out:ana:v(1:1):on",result,0);
            channel_input(fd,1,V2PerFaultValue,V2PerFaultPhase,SysFrec/FreqTriple,V2DC,2,0,0,0);
            artExec(fd,"out:ana:v(1:2):on",result,0);
            channel_input(fd,1,V3PerFaultValue,V3PerFaultPhase,SysFrec/FreqTriple,V3DC,3,0,0,0);
            artExec(fd,"out:ana:v(1:3):on",result,0);

            channel_input(fd,1,VzPerFaultValue,VzPerFaultPhase,SysFrec/FreqTriple,VzDC,4,0,0,0);
            artExec(fd,"out:ana:v(1:4):on",result,0);

            channel_input(fd,0,I1PerFaultValue,I1PerFaultPhase,SysFrec/FreqTriple,I1DC,1,0,0,0);
            artExec(fd,"out:ana:i(1:1):on",result,0);
            channel_input(fd,0,I2PerFaultValue,I2PerFaultPhase,SysFrec/FreqTriple,I2DC,2,0,0,0);
            artExec(fd,"out:ana:i(1:2):on",result,0);
            channel_input(fd,0,I3PerFaultValue,I3PerFaultPhase,SysFrec/FreqTriple,I3DC,3,0,0,0);
            artExec(fd,"out:ana:i(1:3):on",result,0);

            artExec(fd,"inp:buf:sam(bin,1)",result,0);

            if(checkiszero(PreFaultTime)==false)
            out_time=PreFaultTime; //;  故障前时间
            else
            out_time=-1;
            artExec(fd,"seq:wait(0,1)",result,0);//第一步


            //第二步  故障
            artExec(fd,"others:steptime(1)",result,0);//2009-6-23
            artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
            channel_input(fd,1,V1OutputValue,V1Phase,V1Frec/FreqTriple,V1DC,1,0,0,0);
            artExec(fd,"out:ana:v(1:1):on",result,0);

            artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
            channel_input(fd,1,V2OutputValue,V2Phase,V2Frec/FreqTriple,V2DC,2,0,0,0);
            artExec(fd,"out:ana:v(1:2):on",result,0);

            artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
            channel_input(fd,1,V3OutputValue,V3Phase,V3Frec/FreqTriple,V3DC,3,0,0,0);
            artExec(fd,"out:ana:v(1:3):on",result,0);


            artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
            if(VariatyVz2!=1)
            channel_input(fd,1,VzOutputValue,VzPhase,VzFrec/FreqTriple,VzDC,4,0,0,0);
            artExec(fd,"out:ana:v(1:4):on",result,0);



            artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
            channel_input(fd,0,I1OutputValue,I1Phase,I1Frec/FreqTriple,I1DC,1,0,0,0);
            artExec(fd,"out:ana:i(1:1):on",result,0);

            artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
            channel_input(fd,0,I2OutputValue,I2Phase,I2Frec/FreqTriple,I2DC,2,0,0,0);
            artExec(fd,"out:ana:i(1:2):on",result,0);

            artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
            channel_input(fd,0,I3OutputValue,I3Phase,I3Frec/FreqTriple,I3DC,3,0,0,0);
            artExec(fd,"out:ana:i(1:3):on",result,0);


            artExec(fd,"inp:buf:sam(bin,2)",result,0);
            out_time=FaultTime; //故障时间
            artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);
            //2007-8-3 13:44,改为或的关系

            // 故障后
             artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
             channel_input(fd,1,V1PerFaultValue,360,SysFrec/FreqTriple,V1DC,1,0,0,0);
             artExec(fd,"out:ana:v(1:1):on",result,0);

             artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
             channel_input(fd,1,V2PerFaultValue,360,SysFrec/FreqTriple,V2DC,2,0,0,0);
             artExec(fd,"out:ana:v(1:2):on",result,0);

             artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
             channel_input(fd,1,V3PerFaultValue,360,SysFrec/FreqTriple,V3DC,3,0,0,0);
             artExec(fd,"out:ana:v(1:3):on",result,0);


             artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
             channel_input(fd,1,VzPerFaultValue,360,SysFrec/FreqTriple,VzDC,4,0,0,0);
             artExec(fd,"out:ana:v(1:4):on",result,0);



             artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
             channel_input(fd,0,I1PerFaultValue,360,SysFrec/FreqTriple,I1DC,1,0,0,0);
             artExec(fd,"out:ana:i(1:1):on",result,0);

             artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
             channel_input(fd,0,I2PerFaultValue,360,SysFrec/FreqTriple,I2DC,2,0,0,0);
             artExec(fd,"out:ana:i(1:2):on",result,0);

             artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
             channel_input(fd,0,I3PerFaultValue,360,SysFrec/FreqTriple,I3DC,3,0,0,0);
             artExec(fd,"out:ana:i(1:3):on",result,0);

             artExec(fd,"inp:buf:sam(bin,3)",result,0);
             out_time=PostFaultTime; // 故障后时间 第三步
             artExec(fd,"seq:wait(0,2)",result,0);



             artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
             channel_input(fd,1,V1PerFaultValue,360,SysFrec/FreqTriple,V1DC,1,0,0,0);
             artExec(fd,"out:ana:v(1:1):on",result,0);

             artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
             channel_input(fd,1,V2PerFaultValue,360,SysFrec/FreqTriple,V2DC,2,0,0,0);
             artExec(fd,"out:ana:v(1:2):on",result,0);


             artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
             channel_input(fd,1,V3PerFaultValue,360,SysFrec/FreqTriple,V3DC,3,0,0,0);
             artExec(fd,"out:ana:v(1:3):on",result,0);

             artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
             channel_input(fd,1,VzPerFaultValue,360,SysFrec/FreqTriple,VzDC,4,0,0,0);
             artExec(fd,"out:ana:v(1:4):on",result,0);



             artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
             channel_input(fd,0,I1PerFaultValue,360,SysFrec/FreqTriple,I1DC,1,0,0,0);

             artExec(fd,"out:ana:i(1:1):on",result,0);


             artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
             channel_input(fd,0,I2PerFaultValue,360,SysFrec/FreqTriple,I2DC,2,0,0,0);

             artExec(fd,"out:ana:i(1:2):on",result,0);


             artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
             channel_input(fd,0,I3PerFaultValue,360,SysFrec/FreqTriple,I3DC,3,0,0,0);
             artExec(fd,"out:ana:i(1:3):on",result,0);

             //第四步电压保持动作值 faultduration
             artExec(fd,"inp:buf:sam(bin,4)",result,0);
             out_time=PostFaultTime;     //故障后时间 第四步
             artExec(fd,"seq:wait(0,1)",result,0);

             artExec(fd,"out:ana:v(1:1):off",result,0);
             artExec(fd,"out:ana:v(1:2):off",result,0);
             artExec(fd,"out:ana:v(1:3):off",result,0);
             artExec(fd,"out:ana:v(1:4):off",result,0);
             artExec(fd,"out:ana:i(2:1):off",result,0);
             artExec(fd,"out:ana:i(2:2):off",result,0);
             artExec(fd,"out:ana:i(2:3):off",result,0);

             artExec(fd,"inp:buf:sam(bin,5)",result,0);
             out_time=-1;
             artExec(fd,"seq:wait(0,1)",result,0);
         }

      if (k!=2)
      {
        artExec(fd,"inp:buf:sam(bin,2)",result,0);
        artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0)",result,0);
        //2007-8-3 13:44,改为或的关系

        artExec(fd,"inp:buf:sam(bin,3)",result,0);
        out_time=-1;
        artExec(fd,"seq:wait(0,2)",result,0);

        artExec(fd,"inp:buf:sam(bin,4)",result,0);
        out_time=-1;
        artExec(fd,"seq:wait(0,1)",result,0);

        artExec(fd,"out:ana:v(1:1):off",result,0);
        artExec(fd,"out:ana:v(1:2):off",result,0);
        artExec(fd,"out:ana:v(1:3):off",result,0);
        artExec(fd,"out:ana:v(1:4):off",result,0);
        artExec(fd,"out:ana:i(2:1):off",result,0);
        artExec(fd,"out:ana:i(2:2):off",result,0);
        artExec(fd,"out:ana:i(2:3):off",result,0);
        artExec(fd,"inp:buf:sam(bin,5)",result,0);
        out_time=-1;
        artExec(fd,"seq:wait(0,1)",result,0);
      }

      artExec(fd,"seq:end",result,0);
      artExec(fd,"seq:exec",result,0);
  }

void output_signal_end(int fd)
  {  
    //artExec(fd,"out:dig:off(0x94)",result,0);//2016-6-8手动模式，停止按键后，电流先几个周波停止，电压后停止
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    artExec(fd,"inp:dig(1):thres(0)",result,0);
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(1,1)",result,0);
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
    artExec(fd,"out:dig:off(0x94)",result,0);
    artExec(fd,"out:dig:off(0x08)",result,0);
  }

void Output_signal_Auto(int fd)
  {

    artExec(fd,"out:dig:off(0x94)",result,0);//IC放大器对应DB7
    if(fabs(I1OutputValue)>0.0001||VariatyI1==1)
       artExec(fd,"out:dig:on(0x04)",result,0);

    if(fabs(I2OutputValue)>0.0001||VariatyI2==1)
       artExec(fd,"out:dig:on(0x10)",result,0);

    if(fabs(I3OutputValue)>0.0001||VariatyI3==1)
       artExec(fd,"out:dig:on(0x80)",result,0);

    float CStep1=0,CStep2=0,CStep3=0; //电流幅值步长
    float VStep1=0,VStep2=0,VStep3=0,VStep4=0; //电压幅值步长
    float PStep1=0,PStep2=0,PStep3=0,VPStep1=0,VPStep2=0,VPStep3=0,VPStep4=0;// 电流 电压相位步长
    float FStep1=0,FStep2=0,FStep3=0,VFStep1=0,VFStep2=0,VFStep3=0,VFStep4=0; //电流 电压频率步长

    if(fabs(V1Value)>0.00001 || (VariatyV1==1 && Variable==1))  //2016-6-4自动测试从0开始未报警修改
      V1Output=1;
    else
      V1Output=0;

    if(fabs(V2Value)>0.00001 || (VariatyV2==1 && Variable==1))
      V2Output=1;
    else
      V2Output=0;

    if(fabs(V3Value)>0.00001 || (VariatyV3==1 && Variable==1))
      V3Output=1;
    else
      V3Output=0;

    if(fabs(VzValue)>0.00001 || (VariatyVz==1 && Variable==1))
          VzOutput=1;
        else
          VzOutput=0;

    if(fabs(I1Value)>0.00001 || (VariatyI1==1 && Variable==1))  //2016-6-4自动测试从0开始未报警修改
      I1Output=1;
    else
      I1Output=0;

    if(fabs(I2Value)>0.00001 || (VariatyI2==1 && Variable==1))
      I2Output=1;
    else
      I2Output=0;

    if(fabs(I3Value)>0.00001 || (VariatyI3==1 && Variable==1))
      I3Output=1;
    else
      I3Output=0;

  change_timedata=StepTime;

    artExec(fd,"inp:dig:interval(1)",result,0);
    artExec(fd,"out:ana:user:mem?",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步

    switch(Variable)
      {
          case 1:
              if(VariatyV1==1)
                VStep1=VolStep;
              if(VariatyV2==1)
                VStep2=VolStep;
              if(VariatyV3==1)
                VStep3=VolStep;
              if(VariatyVz==1)
                VStep4=VolStep;


              if(VariatyI1==1)
                CStep1=CurStep;
              if(VariatyI2==1)
                CStep2=CurStep;
              if(VariatyI3==1)
                CStep3=CurStep;
              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;
              //相位步长
              PStep1=0;
              PStep2=0;
              PStep3=0;


              VPStep1=0;
              VPStep2=0;
              VPStep3=0;
              VPStep4=0;
              //频率步长
              VFStep1=0;
              VFStep2=0;
              VFStep3=0;
              VFStep4=0;

              FStep1=0;
              FStep2=0;
              FStep3=0;

              break;
          case 2:
              if(VariatyV1==1)
                VPStep1=PhaStep;
              if(VariatyV2==1)
                VPStep2=PhaStep;
              if(VariatyV3==1)
                VPStep3=PhaStep;
              if(VariatyVz==1)
                VPStep4=PhaStep;


              if(VariatyI1==1)
                PStep1=PhaStep;
              if(VariatyI2==1)
                PStep2=PhaStep;
              if(VariatyI3==1)
                PStep3=PhaStep;
              //幅度步长
              VStep1=0;
              VStep2=0;
              VStep3=0;
              VStep4=0;

              CStep1=0;
              CStep2=0;
              CStep3=0;
              //频率步长
              VFStep1=0;
              VFStep2=0;
              VFStep3=0;
              VFStep4=0;

              FStep1=0;
              FStep2=0;
              FStep3=0;
              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;

              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;

              break;
          case 3:
              if(VariatyV1==1)
                VFStep1=FreStep;
              if(VariatyV2==1)
                VFStep2=FreStep;
              if(VariatyV3==1)
                VFStep3=FreStep;

              if(VariatyVz==1)
                VFStep4=FreStep;


              if(VariatyI1==1)
                FStep1=FreStep;
              if(VariatyI2==1)
                FStep2=FreStep;
              if(VariatyI3==1)
                FStep3=FreStep;

              VStep1=0;
              VStep2=0;
              VStep3=0;
              VStep4=0;
              CStep1=0;
              CStep2=0;
              CStep3=0;
              //相位
              PStep1=0;
              PStep2=0;
              PStep3=0;
              VPStep1=0;
              VPStep2=0;
              VPStep3=0;
              VPStep4=0;

              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;
              break;
          }

              ChangeOutputValue();
              channel_input(fd,0,I1OutputValue,I1Phase,I1Frec/FreqTriple,I1DC,1,CStep1,PStep1,FStep1/2);
              artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:i(1:1):on",result,0);
              channel_input(fd,0,I2OutputValue,I2Phase,I2Frec/FreqTriple,I2DC,2,CStep2,PStep2,FStep2/2);
              artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:i(1:2):on",result,0);
              channel_input(fd,0,I3OutputValue,I3Phase,I3Frec/FreqTriple,I3DC,3,CStep3,PStep3,FStep3/2);
              artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:i(1:3):on",result,0);

              artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
              channel_input(fd,1,V1OutputValue,V1Phase,V1Frec/FreqTriple,V1DC,1,VStep1,VPStep1,VFStep1/2);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:v(1:1):on",result,0);
              artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
              channel_input(fd,1,V2OutputValue,V2Phase,V2Frec/FreqTriple,V2DC,2,VStep2,VPStep2,VFStep2/2);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:v(1:2):on",result,0);
              artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
              channel_input(fd,1,V3OutputValue,V3Phase,V3Frec/FreqTriple,V3DC,3,VStep3,VPStep3,VFStep3/2);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:v(1:3):on",result,0);

              if(VariatyVz2==1)
               {
                channel_input(fd,1,VzOutputValue,VzPhase,VzFrec/FreqTriple,VzDC,4,0,0,0);
               }
               else
                {
                artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
                channel_input(fd,1,VzOutputValue,VzPhase,VzFrec/FreqTriple,VzDC,4,0,0,0/*VStep4,VPStep4,VFStep4/2*/);//2016-7-2  vz屏蔽，自动变化出值修改
                artExec(fd,"others:steptime(1)",result,0);
                }
                artExec(fd,"out:ana:v(1:4):on",result,0);



    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=fabs((StepNum+1)*StepTime);
    //artExec(fd,"seq:wait(orbin(1),2)",result,0);//第二步
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);


    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);


    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);


    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
  }

double gettime(double x,double y,double z)  //compute logic act time
  {
    double time;
    x=result[7];
    y=result[8];
    z=result[9];
    time=(double)(((x*65536+y*256+z)*50-50)/1000000);
    return time;
  }
