#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "Testcom2.h"
#include "Dfdtseq.h"
#include <QDebug>



extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;

extern float FixedVoltage,dv,dt;
extern float StartVoltage,EndVoltage,ActStep,StepTime;
extern float SystemFrec,Phase;
extern float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
extern float TripTime1,TripTime2,TripTime3,TripTime4,TripTime5,TripVoltage,TimeVoltage;

extern float faultduration;
extern float SystemVolt;
extern int  dvdtmode;
extern float ChangeValue,dv,dt,dvdt;
extern float prefaulttime;

extern float faultduration;

extern float postfaulttime;

extern float StartVoltage,EndVoltage;
extern float ChangeValue;

extern float dvdtStopVoltage;  //dvdt的停止电压
extern float dvdtstart;
extern float dvdtend;
extern float dvdtStep;
extern float currentdvdt;
extern int Variable; //1 VAB 2 VBC 3 VCA


extern  float StopVoltage_triptime;
extern float dvdt_triptime;
extern float holdtime_triptime;

extern float VStart,VStop,VStep,VSteptime;
extern float StopVoltage;
extern float StopVoltageHoldTime;

extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern float TripValue,TripValue1;

extern int   seq_count,TestItem1,TestItem2,TestItem3,TestItem4,TestItem5;

extern float dtofdvdt;
extern float dtofdvdttime;

void  OutputSequencer1(int fd)
  {//测试动作值的sequencer
    float ChangeStep;
    float StartVA=0;
    float StartVB=0;
    float StartVC=0;
    float StepVA=0;
    float StepVB=0;
    float StepVC=0;
    float PhaseVA=0;
    float PhaseVB=0;
    float PhaseVC=0;
    int sumstep;
    if(VStart>VStop)
      ChangeStep=0-VStep;
    else
      ChangeStep=VStep;
    switch(Variable)  //2009-10-08
     {
     case 1:  //VA
       StartVA=VStart;
       StepVA=ChangeStep;
       StartVB=SystemVolt;
       StepVB=0;
       StartVC=SystemVolt;
       StepVC=0;
       PhaseVA=0;
       PhaseVB=-120;
       PhaseVC=120;
       break;
     case 2:  //VB
       StartVB=VStart;
       StepVB=ChangeStep;
       StartVA=SystemVolt;
       StepVA=0;
       StartVC=SystemVolt;
       StepVC=0;
       PhaseVA=120;
       PhaseVB=-120;
       PhaseVC=0;

       break;
     case 3:  //VC
       StartVC=VStart;
       StepVC=ChangeStep;
       StartVA=SystemVolt;
       StepVA=0;
       StartVB=SystemVolt;
       StepVB=0;

       PhaseVA=-120;
       PhaseVB=120;
       PhaseVC=0;

       break;
     case 4:  //VAB
       StartVA=VStart/1.732;
       StepVA=ChangeStep/1.732;
       StartVB=VStart/1.732;
       StepVB=ChangeStep/1.732;
       StartVC=SystemVolt;
       StepVC=0;
       PhaseVA=-120;
       PhaseVB=120;
       PhaseVC=0;
       break;
     case 5:  //VBC
       StartVB=VStart/1.732;
       StepVB=ChangeStep/1.732;
       StartVC=VStart/1.732;
       StepVC=ChangeStep/1.732;
       StartVA=SystemVolt;
       StepVA=0;
       PhaseVA=0;
       PhaseVB=-120;
       PhaseVC=120;
       break;
    case 6:  //VCA
       StartVA=VStart/1.732;
       StepVA=ChangeStep/1.732;
       StartVC=VStart/1.732;
       StepVC=ChangeStep/1.732;
       StartVB=SystemVolt;
       StepVB=0;
       PhaseVA=120;
       PhaseVB=-120;
       PhaseVC=0;
       break;
   case 7:
         StartVA=VStart;
         StepVA=ChangeStep;
         StartVC=VStart;
         StepVC=ChangeStep;
         StartVB=VStart;
         StepVB=ChangeStep;
         PhaseVA=0;
         PhaseVB=-120;
         PhaseVC=120;
         break;
     }


    change_timedata=VSteptime;
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=0;
    artExec(fd,"seq:wait(0,1)",result,0);

    channel_input(fd,1,SystemVolt,PhaseVA,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,PhaseVB,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,PhaseVC,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
      out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA,360,SystemFrec,V1DC,1,StepVA,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

     artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB,360,SystemFrec,V2DC,2,StepVB,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

     artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC,360,SystemFrec,V3DC,3,StepVC,0,0); //2009-10-08
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    artExec(fd,"out:returnlimit",result,0);//???2016-6-1
    sumstep=floor(fabs((VStart-VStop)/VStep))+1;
    out_time=sumstep*VSteptime;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);

    if(TestItem2==0)   //2015.7.1
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);
    else
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,2,0,1)",result,0);



    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {   out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);
     }
    else
    {
       artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
       channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0);
       artExec(fd,"out:ana:v(1:1):on",result,0);
       artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
       channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
       artExec(fd,"out:ana:v(1:2):on",result,0);
       artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
       channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
       artExec(fd,"out:ana:v(1:3):on",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);



    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {   out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);
     }
    else
    {
       artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
       channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0);
       artExec(fd,"out:ana:v(1:1):on",result,0);
       artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
       channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
       artExec(fd,"out:ana:v(1:2):on",result,0);
       artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
       channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
       artExec(fd,"out:ana:v(1:3):on",result,0);
    }
    artExec(fd,"inp:buf:sam(bin,4)",result,0);
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


void  OutputSequencer3(int fd)
  {

    float StartVA=0;
    float StartVB=0;
    float StartVC=0;
    float StartVA1=0;
    float StartVB1=0;
    float StartVC1=0;

    float PhaseVA=0;
    float PhaseVB=0;
    float PhaseVC=0;
    float VoltageOutput=0;

    VoltageOutput=StopVoltage;


    switch(Variable)  //2009-10-08
     {
     case 1:  //VA
       StartVA=SystemVolt;
       StartVA1=VoltageOutput;
       StartVB=SystemVolt;
       StartVB1=SystemVolt;
       StartVC=SystemVolt;
       StartVC1=SystemVolt;
       PhaseVA=0;
       PhaseVB=-120;
       PhaseVC=120;
       break;
     case 2:  //VB
       StartVB=SystemVolt;
       StartVB1=VoltageOutput;
       StartVA=SystemVolt;
       StartVA1=SystemVolt;
       StartVC=SystemVolt;
       StartVC1=SystemVolt;

       PhaseVA=120;
       PhaseVB=0;
       PhaseVC=-120;

       break;
     case 3:  //VC
       StartVC=SystemVolt;
       StartVC1=VoltageOutput;
       StartVA=SystemVolt;
       StartVA1=SystemVolt;
       StartVB=SystemVolt;
       StartVB1=SystemVolt;
       PhaseVA=-120;
       PhaseVB=120;
       PhaseVC=0;
       break;
     case 4:  //VAB
       StartVA=SystemVolt;
       StartVA1=VoltageOutput/1.732;
       StartVB=SystemVolt;
       StartVB1=VoltageOutput/1.732;

       StartVC=SystemVolt;
       StartVC1=SystemVolt;

       PhaseVA=-120;
       PhaseVB=120;
       PhaseVC=0;
       break;
     case 5:  //VBC
       StartVB=SystemVolt;
       StartVB1=VoltageOutput/1.732;
       StartVC=SystemVolt;
       StartVC1=VoltageOutput/1.732;

       StartVA=SystemVolt;
       StartVA1=SystemVolt;

       PhaseVA=0;
       PhaseVB=-120;
       PhaseVC=120;

       break;
    case 6:  //VCA
       StartVA=SystemVolt;
       StartVA1=VoltageOutput/1.732;
       StartVC=SystemVolt;
       StartVC1=VoltageOutput/1.732;
       StartVB=SystemVolt;
       StartVB1=SystemVolt;
       PhaseVA=120;
       PhaseVB=-120;
       PhaseVC=0;
       break;
    case 7:
         StartVA=SystemVolt;
         StartVA1=VoltageOutput;
         StartVB=SystemVolt;
         StartVB1=VoltageOutput;
         StartVC=SystemVolt;
         StartVC1=VoltageOutput;

         PhaseVA=0;
         PhaseVB=-120;
         PhaseVC=120;
         break;
     }

    artExec(fd,"inp:dig:interval(0)",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=0;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步

    channel_input(fd,1,StartVA,PhaseVA,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,StartVB,PhaseVB,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,StartVC,PhaseVC,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);



    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);


    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA1,360,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB1,360,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC1,360,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    //从0跳变到可靠动作值
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=StopVoltageHoldTime;
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);


    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     { out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);

     }
     else
    {

        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);



    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     { out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);
     }
     else
    {
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
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


void  OutputSequencer4(int fd)
  {
    float ChangeStep;
    float StartVA=0;
    float StartVB=0;
    float StartVC=0;

    float StartVA2=0;
    float StartVB2=0;
    float StartVC2=0;

    float StepVA=0;
    float StepVB=0;
    float StepVC=0;

    float StepVA2=0;
    float StepVB2=0;
    float StepVC2=0;



    float PhaseVA=0;
    float PhaseVB=0;
    float PhaseVC=0;

    switch(Variable)
    {
    case 1:
        V1Output=1;
        StartVoltage=SystemVolt;
        break;
    case 2:
        V2Output=1;
        StartVoltage=SystemVolt;
        break;
    case 3:
        V3Output=1;
        StartVoltage=SystemVolt;
        break;
    case 4:
        V1Output=1;
        V2Output=1;
        V3Output=0;
        StartVoltage=SystemVolt*1.732;
        break;
    case 5:
        V1Output=0;
        V2Output=1;
        V3Output=1;
        StartVoltage=SystemVolt*1.732;
        break;
    case 6:
        V1Output=1;
        V2Output=0;
        V3Output=1;
        StartVoltage=SystemVolt*1.732;
        break;
    case 7:
        V1Output=1;
        V2Output=1;
        V3Output=1;
        StartVoltage=SystemVolt*1.732;
        break;
    }


        change_timedata=dtofdvdt;
        if(dvdtStopVoltage>StartVoltage)  //增加的
        ChangeStep=currentdvdt*change_timedata;
        else
        ChangeStep=-currentdvdt*change_timedata;



    int sumstep;
    switch(Variable)
     {


     case 1:  //VA
       StartVA=StartVoltage;
       StepVA=ChangeStep;
       StartVB=SystemVolt;
       StepVB=0;
       StartVC=SystemVolt;
       StepVC=0;
       PhaseVA=0;
       PhaseVB=-120;
       PhaseVC=120;





       break;
     case 2:  //VB
       StartVB=StartVoltage;
       StepVB=ChangeStep;
       StartVC=SystemVolt;
       StepVC=0;
       StartVA=SystemVolt;
       StepVA=0;
       PhaseVA=120;
       PhaseVB=0;
       PhaseVC=-120;



       break;
     case 3:  //VC
       StartVC=StartVoltage;
       StepVC=ChangeStep;
       StartVA=SystemVolt;
       StepVA=0;
       StartVB=SystemVolt;
       StepVB=0;
       PhaseVA=-120;
       PhaseVB=120;
       PhaseVC=0;

       break;

     case 4:  //VAB
       StartVA=StartVoltage/1.732;
       StepVA=ChangeStep/1.732;
       StartVB=StartVoltage/1.732;
       StepVB=ChangeStep/1.732;
       StartVC=SystemVolt;
       StepVC=0;
       PhaseVA=-120;
       PhaseVB=120;
       PhaseVC=0;

       break;
     case 5:  //VBC
       StartVB=StartVoltage/1.732;
       StepVB=ChangeStep/1.732;
       StartVC=StartVoltage/1.732;
       StepVC=ChangeStep/1.732;

       StartVA=SystemVolt;
       StepVA=0;
       PhaseVB=-120;
       PhaseVC=120;
       PhaseVA=0;
       break;
    case 6:  //VCA
       StartVA=StartVoltage/1.732;
       StepVA=ChangeStep/1.732;
       StartVC=StartVoltage/1.732;
       StepVC=ChangeStep/1.732;
       StartVB=SystemVolt;
       StepVB=0;
       PhaseVB=0;
       PhaseVC=-120;
       PhaseVA=120;
       break;

     case 7:  //VABC
        StartVA=StartVoltage/1.732;
        StepVA=ChangeStep/1.732;
        StartVC=StartVoltage/1.732;
        StepVC=ChangeStep/1.732;
        StartVB=StartVoltage/1.732;
        StepVB=ChangeStep/1.732;
        PhaseVA=0;
        PhaseVB=-120;
        PhaseVC=120;
        break;

     }

 //   artExec(fd,"inp:dig(1):thres(1)",result,0);//打开放大器
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    channel_input(fd,1,SystemVolt,PhaseVA,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,PhaseVB,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,PhaseVC,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步



    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA,360,SystemFrec,V1DC,1,StepVA,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB,360,SystemFrec,V2DC,2,StepVB,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC,360,SystemFrec,V3DC,3,StepVC,0,0); //2009-10-08
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    if((Variable>=1&&Variable<=3))
    sumstep=floor(fabs((SystemVolt-EndVoltage)/ChangeStep))+1;

    else if((Variable>=4&&Variable<=6)||Variable==7)
    sumstep=floor(fabs((SystemVolt*1.732-EndVoltage)/ChangeStep))+1;

    out_time=sumstep*change_timedata;

    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);


    switch(Variable)
    {
    case 1:
        StartVA2=EndVoltage;
        StartVB2=SystemVolt;
        StartVC2=SystemVolt;

        break;

    case 2:
        StartVA2=SystemVolt;
        StartVB2=EndVoltage;
        StartVC2=SystemVolt;

        break;

    case 3:
        StartVA2=SystemVolt;
        StartVB2=SystemVolt;
        StartVC2=EndVoltage;

        break;

    case 4:
        StartVA2=EndVoltage/1.732;
        StartVB2=EndVoltage/1.732;
        StartVC2=SystemVolt;

        break;

    case 5:
        StartVA2=SystemVolt;
        StartVB2=EndVoltage/1.732;
        StartVC2=EndVoltage/1.732;



        break;

    case 6:
        StartVA2=EndVoltage/1.732;
        StartVB2=SystemVolt;
        StartVC2=EndVoltage/1.732;

        break;

    case 7:
        StartVA2=EndVoltage/1.732;
        StartVB2=EndVoltage/1.732;
        StartVC2=EndVoltage/1.732;
    }



     StepVA2=-StepVA;
     StepVB2=-StepVB;
     StepVC2=-StepVC;



    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA2,360,SystemFrec,V1DC,1,StepVA2,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

     artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB2,360,SystemFrec,V2DC,2,StepVB2,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

     artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC2,360,SystemFrec,V3DC,3,StepVC2,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    if((Variable>=1&&Variable<=3))
    sumstep=floor(fabs((SystemVolt-EndVoltage)/ChangeStep))+1;

    else if(Variable>=4&&Variable==7)
    sumstep=floor(fabs((SystemVolt*1.732-EndVoltage)/ChangeStep))+1;

    out_time=sumstep*change_timedata;


    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);





    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
    {
       out_time=-1;
       artExec(fd,"out:ana:v(1:1):off",result,0);
       artExec(fd,"out:ana:v(1:2):off",result,0);
       artExec(fd,"out:ana:v(1:3):off",result,0);
    }
    else
    {
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);


    //第4步，接点有动作，关断信号
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
}

void  OutputSequencer5(int fd) //延时验证
  {


    float ChangeStep;
    float StartVA=0;
    float StartVB=0;
    float StartVC=0;
    float StepVA=0;
    float StepVB=0;
    float StepVC=0;
    float PhaseVA=0;
    float PhaseVB=0;
    float PhaseVC=0;
    int sumstep;

    change_timedata=dtofdvdttime;
    if(StopVoltage_triptime>=StartVoltage)
    ChangeStep=dvdt_triptime*change_timedata;
    else
    ChangeStep=-dvdt_triptime*change_timedata;




    switch(Variable)  //2009-10-08
     {


    case 1:  //VA
      StartVA=StartVoltage;
      StepVA=ChangeStep;
      StartVB=SystemVolt;
      StepVB=0;
      StartVC=SystemVolt;
      StepVC=0;
      PhaseVB=-120;
      PhaseVC=120;
      PhaseVA=0;




      break;
    case 2:  //VB
      StartVB=StartVoltage;
      StepVB=ChangeStep;
      StartVC=SystemVolt;
      StepVC=0;
      StartVA=SystemVolt;
      StepVA=0;
      PhaseVB=0;
      PhaseVC=-120;
      PhaseVA=120;


      break;
    case 3:  //VC
      StartVC=StartVoltage;
      StepVC=ChangeStep;
      StartVA=SystemVolt;
      StepVA=0;
      StartVB=SystemVolt;
      StepVB=0;
      PhaseVB=120;
      PhaseVC=0;
      PhaseVA=-120;
      break;


     case 4:  //VAB
       StartVA=StartVoltage/1.732;
       StepVA=ChangeStep/1.732;
       StartVB=StartVoltage/1.732;
       StepVB=ChangeStep/1.732;
       StartVC=SystemVolt;
       StepVC=0;
       PhaseVB=120;
       PhaseVC=0;
       PhaseVA=-120;
       break;
     case 5:  //VBC
       StartVB=StartVoltage/1.732;
       StepVB=ChangeStep/1.732;
       StartVC=StartVoltage/1.732;
       StepVC=ChangeStep/1.732;

       StartVA=SystemVolt;
       StepVA=0;
       PhaseVB=-120;
       PhaseVC=120;
       PhaseVA=0;
       break;
    case 6:  //VCA
       StartVA=StartVoltage/1.732;
       StepVA=ChangeStep/1.732;
       StartVC=StartVoltage/1.732;
       StepVC=ChangeStep/1.732;
       StartVB=SystemVolt;
       StepVB=0;
       PhaseVB=0;
       PhaseVC=-120;
       PhaseVA=120;
       break;
    case 7:
         StartVA=StartVoltage/1.732;
         StepVA=ChangeStep/1.732;
         StartVC=StartVoltage/1.732;
         StepVC=ChangeStep/1.732;
         StartVB=StartVoltage/1.732;
         StepVB=ChangeStep/1.732;
         PhaseVA=0;
         PhaseVB=-120;
         PhaseVC=120;
         break;
     }





    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    channel_input(fd,1,SystemVolt,PhaseVA,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,PhaseVB,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,PhaseVC,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步


    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA,360,SystemFrec,V1DC,1,StepVA,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

     artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB,360,SystemFrec,V2DC,2,StepVB,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

     artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC,360,SystemFrec,V3DC,3,StepVC,0,0); //2009-10-08
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

   if(Variable>=1&&Variable<=3)
   {
      sumstep=floor(fabs((SystemVolt-EndVoltage)/ChangeStep))+1;
   }
    else if((Variable>=4&&Variable<=6)||Variable==7)
   sumstep=floor(fabs((SystemVolt*1.732-EndVoltage)/ChangeStep))+1;

   out_time=sumstep*change_timedata;


    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);


    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
    {
       out_time=-1;
       artExec(fd,"out:ana:v(1:1):off",result,0);
       artExec(fd,"out:ana:v(1:2):off",result,0);
       artExec(fd,"out:ana:v(1:3):off",result,0);
    }
    else
    {
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
    }
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);

    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
    {
       out_time=-1;
       artExec(fd,"out:ana:v(1:1):off",result,0);
       artExec(fd,"out:ana:v(1:2):off",result,0);
       artExec(fd,"out:ana:v(1:3):off",result,0);
    }
    else
    {
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
    }
    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);

    //第4步，接点有动作，关断信号
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
}




void output_signal_end(int fd)
  {
    int i,j;
    unsigned char step;
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    artExec(fd,"inp:dig(1):thres(0)",result,0);//关闭放大器，控制风扇转速
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(1,1)",result,0);
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
    artExec(fd,"out:dig:off(0x74)",result,0);
    artExec(fd,"out:dig:off(0x08)",result,0);
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


