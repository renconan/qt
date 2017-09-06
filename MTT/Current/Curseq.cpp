
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include "Curseq.h"
#include <QDebug>
#include <math.h>



extern TEMPRORARYDATA *temprorarydata;

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern int   low,row,seq_count;
extern int   I1DC,I2DC,I3DC;
extern int   V1DC,V2DC,V3DC;
extern int   TestItem1,TestItem3,TestItem2,OutputMode;
extern float  StartCurrent,EndCurrent,ActStep,ReturnStep,StepTime;
extern float  LogicResolution,SystemFrec;
extern float  TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime;
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

extern float prefaulttime;
extern float StopCurrent; //动作时间的用户手动输入的值
extern int   TripTimeType;   //测动作时间的方式
extern float StopCurrentHoldTime ;//动作时间的保持时间

extern float Vfault;
extern float Angle;
extern float SystemVolt;
extern float V1Value,V1Phase;
extern float V2Value,V2Phase;
extern float V3Value,V3Phase;
extern float PhaseIA;
extern float PhaseIB;
extern float PhaseIC;


extern float V1PerPhase;
extern float V2PerPhase;
extern float V3PerPhase;

extern float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase;


extern int   TestItem1,TestItem3,TestItem2,OutputMode;


double gettime(double x,double y,double z)  //compute logic act time
  {
    double time;
    x=result[7];
    y=result[8];
    z=result[9];
    time=(double)(((x*65536+y*256+z)*50-50)/1000000);
    return time;
  }

void output_signal_end(int fd)
  {
    int i,j;
    unsigned char step;
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    artExec(fd,"inp:dig(1):thres(0)",result,0);
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
    artExec(fd,"out:dig:off(0xf4)",result,0);
    artExec(fd,"out:dig:off(0x08)",result,0);

  }

void OutputSequencer1(int fd)
  {
    float StartCurIA=0;
    float StartCurIB=0;
    float StartCurIC=0;
    float StepIA=0;
    float StepIB=0;
    float StepIC=0;
    int sumstep;
    float change_step;

      if(StartCurrent>EndCurrent)
        change_step=-ActStep;
      else
        change_step=ActStep;

    artExec(fd,"out:dig:on(0x04)",result,0);

    switch(OutputMode)
     {
     case 1:  //IA
       StartCurIA=StartCurrent;
       StepIA=change_step;
       artExec(fd,"out:dig:on(0x10)",result,0);
       break;
     case 2:  //IB
       StartCurIB=StartCurrent;
       StepIB=change_step;
       artExec(fd,"out:dig:on(0x20)",result,0);
       break;
     case 3:  //IC
       StartCurIC=StartCurrent;
       StepIC=change_step;
       artExec(fd,"out:dig:on(0x40)",result,0);
       break;
     case 4:
         StartCurIA=StartCurrent;
         StepIA=change_step;
         StartCurIB=StartCurrent;
         StepIB=change_step;
         artExec(fd,"out:dig:on(0x30)",result,0);
         break;
     case 5:
         StartCurIB=StartCurrent;
         StepIB=change_step;
         StartCurIC=StartCurrent;
         StepIC=change_step;
         artExec(fd,"out:dig:on(0x60)",result,0);
         break;
     case 6:
         StartCurIA=StartCurrent;
         StepIA=change_step;
         StartCurIC=StartCurrent;
         StepIC=change_step;
         artExec(fd,"out:dig:on(0x50)",result,0);
         break;
     case 7:  //IAIBIC
       StartCurIA=StartCurrent;
       StepIA=change_step;
       StartCurIB=StartCurrent;
       StepIB=change_step;
       StartCurIC=StartCurrent;
       StepIC=change_step;
       artExec(fd,"out:dig:on(0x70)",result,0);
       break;
     }



    change_timedata=StepTime;  //步长时间
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);


    channel_input(fd,1,SystemVolt,V1PerFaultPhase,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,V2PerFaultPhase,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,V3PerFaultPhase,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步

    channel_input(fd,0,StartCurIA,PhaseIA,SystemFrec,I1DC,1,StepIA,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);

    channel_input(fd,0,StartCurIB,PhaseIB,SystemFrec,I2DC,2,StepIB,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);

    channel_input(fd,0,StartCurIC,PhaseIC,SystemFrec,I3DC,3,StepIC,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);



    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1Value,V1Phase,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2Value,V2Phase,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3Value,V3Phase,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    artExec(fd,"out:returnlimit",result,0);//???2016-6-1
    sumstep=floor(fabs((StartCurrent-EndCurrent)/ActStep))+1;
    out_time=sumstep*change_timedata;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);

    if(TestItem2==0)
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);
    else
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,2,0,1)",result,0);




    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    //2007-8-3 13:44,改为或的关系
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


void OutputSequencer3(int fd)
  {
    float StartCurIA=0;
    float StartCurIB=0;
    float StartCurIC=0;

    float CurrentOutput=0;
    CurrentOutput=StopCurrent;

    artExec(fd,"out:dig:on(0x04)",result,0);
    switch(OutputMode)
     {
     case 1:  //IA
       StartCurIA=CurrentOutput;
       artExec(fd,"out:dig:on(0x10)",result,0);
       break;
     case 2:  //IB
       StartCurIB=CurrentOutput;
       artExec(fd,"out:dig:on(0x20)",result,0);
       break;
     case 3:  //IC
       StartCurIC=CurrentOutput;
       artExec(fd,"out:dig:on(0x40)",result,0);
       break;
     case 4:
      StartCurIA=CurrentOutput;
      StartCurIB=CurrentOutput;
      artExec(fd,"out:dig:on(0x30)",result,0);
      break;
     case 5:
      StartCurIC=CurrentOutput;
      StartCurIB=CurrentOutput;
      artExec(fd,"out:dig:on(0x60)",result,0);
      break;
     case 6:
      StartCurIC=CurrentOutput;
      StartCurIA=CurrentOutput;
      artExec(fd,"out:dig:on(0x50)",result,0);
      break;
     case 7:  //IAIBIC
       artExec(fd,"out:dig:on(0x70)",result,0);
       StartCurIA=CurrentOutput;
       StartCurIB=CurrentOutput;
       StartCurIC=CurrentOutput;
       break;
     }

    artExec(fd,"inp:dig:interval(0)",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    channel_input(fd,1,SystemVolt,V1PerFaultPhase,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,V2PerFaultPhase,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,V3PerFaultPhase,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);


    channel_input(fd,0,StartCurIA,PhaseIA,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,StartCurIB,PhaseIB,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,StartCurIC,PhaseIC,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1Value,V1Phase,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2Value,V2Phase,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3Value,V3Phase,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=StopCurrentHoldTime;
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);



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

