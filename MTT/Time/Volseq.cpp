#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <QDebug>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include "Volseq.h"

extern TEMPRORARYDATA *temprorarydata;
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int   V1DC,V2DC,V3DC,VzDC;
extern int I1DC;
extern int I2DC;
extern int I3DC;

extern int   OutputMode,TestItem1,TestItem2,TestItem3,TestObject;
extern float FixedUnderTime,FixedOverTime;
extern float StartValue,EndValue,ActStep,ReturnStep,StepTime;
extern float ChangeValue,SystemFrec,Phase;
extern float TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime1,TripTime2,TripTime3;
extern float faultduration; //2014-11-13 杜凯
extern float SystemVolt;
extern float prefaulttime;
extern float StopValue; //动作时间的用户手动输入的值
extern int   TripTimeType;   //测动作时间的方式
extern float StopValueHoldTime;//动作时间的保持时间

extern int   DCRelayType;
extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

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
    artExec(fd,"out:ana:v(1:4):off",result,0);
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

  void OutputSequencer1(int fd)  //2009-10-08
  {//测试动作值的sequencer
    float ChangeStep;
    float StartVA=0;
    float StartVB=0;
    float StartVC=0;
    float StepVA=0;
    float StepVB=0;
    float StepVC=0;

    float StartIA=0;
    float StartIB=0;
    float StartIC=0;
    float StepIA=0;
    float StepIB=0;
    float StepIC=0;




    int sumstep;
    if(StartValue>EndValue)
      ChangeStep=0-ActStep;
    else
      ChangeStep=ActStep;
    if(DCRelayType==1)
   { switch(OutputMode)  //2009-10-08
     {
     case 1:  //VA
       StartVA=StartValue;
       StepVA=ChangeStep;
       break;
     case 2:  //VB
       StartVB=StartValue;
       StepVB=ChangeStep;
       break;
     case 3:  //VC
       StartVC=StartValue;
       StepVC=ChangeStep;
       break;
     case 4:  //VAB
       StartVA=StartValue/2;
       StepVA=ChangeStep/2;
       StartVB=-StartValue/2;
       StepVB=-ChangeStep/2;

       break;
     case 5:  //VBC
       StartVB=StartValue/2;
       StepVB=ChangeStep/2;
       StartVC=-StartValue/2;
       StepVC=-ChangeStep/2;

       break;
    case 6:  //VCA
       StartVA=StartValue/2;
       StepVA=ChangeStep/2;
       StartVC=-StartValue/2;
       StepVC=-ChangeStep/2;
       break;
     }

   }
    else
    {
    switch(OutputMode)
    {
    case 1:  //VA
      StartIA=StartValue;
      StepIA=ChangeStep;
      break;
    case 2:  //VB
      StartIB=StartValue;
      StepIB=ChangeStep;
      break;
    case 3:  //VC
      StartIC=StartValue;
      StepIC=ChangeStep;
      break;

    case 4:
        StartIA=StartValue/3;
        StepIA=ChangeStep/3;
        StartIB=StartValue/3;
        StepIB=ChangeStep/3;
        StartIC=StartValue/3;
        StepIC=ChangeStep/3;
        break;
    }
    }


    change_timedata=StepTime;
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    if(DCRelayType==1)
   { channel_input(fd,1,StartVA,0,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,StartVB,0,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,StartVC,0,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);
   }
    else
    {
            channel_input(fd,0,StartIA,0,SystemFrec,V1DC,1,0,0,0); //2009-6-20
            artExec(fd,"out:ana:i(1:1):on",result,0);
            channel_input(fd,0,StartIB,0,SystemFrec,V2DC,2,0,0,0);
            artExec(fd,"out:ana:i(1:2):on",result,0);
            channel_input(fd,0,StartIC,0,SystemFrec,V3DC,3,0,0,0);
            artExec(fd,"out:ana:i(1:3):on",result,0);
    }



    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1/*prefaulttime*/;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步

    if(DCRelayType==1)
  {
      channel_input(fd,1,StartVA,0,SystemFrec,V1DC,1,StepVA,0,0);
      artExec(fd,"others:steptime(1)",result,0);
      artExec(fd,"out:ana:v(1:1):on",result,0);

      channel_input(fd,1,StartVB,0,SystemFrec,V2DC,2,StepVB,0,0);
      artExec(fd,"others:steptime(1)",result,0);
      artExec(fd,"out:ana:v(1:2):on",result,0);

      channel_input(fd,1,StartVC,0,SystemFrec,V3DC,3,StepVC,0,0); //2009-10-08
      artExec(fd,"others:steptime(1)",result,0);
      artExec(fd,"out:ana:v(1:3):on",result,0);

      channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
      artExec(fd,"out:ana:v(1:4):on",result,0);
  }
   else
  {
        channel_input(fd,0,StartIA,0,SystemFrec,I1DC,1,StepIA,0,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);

        channel_input(fd,0,StartIB,0,SystemFrec,I2DC,2,StepIB,0,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        channel_input(fd,0,StartIC,0,SystemFrec,I3DC,3,StepIC,0,0); //2009-10-08
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
  }
artExec(fd,"out:returnlimit",result,0);//???2016-6-1
    sumstep=floor(fabs((StartValue-EndValue)/ActStep))+1;
    out_time=sumstep*StepTime;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    if(TestItem2==0)   //2015.7.1
       artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);
       else
       artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,2,0,1)",result,0);

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    out_time=-1;

    artExec(fd,"seq:wait(0,1)",result,0);
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
  }


  void OutputSequencer2(int fd)  //2009-10-08
  {
    float ChangeStep;
    float StartVA=0;
    float StartVB=0;
    float StartVC=0;
    float StepVA=0;
    float StepVB=0;
    float StepVC=0;
    float StartVA1=0;
    float StartVB1=0;
    float StartVC1=0;

    float StartIA=0;
    float StartIB=0;
    float StartIC=0;
    float StepIA=0;
    float StepIB=0;
    float StepIC=0;
    float StartIA1=0;
    float StartIB1=0;
    float StartIC1=0;


    int sumstep=0;

    if(StartValue>EndValue)
      ChangeStep=ActStep;
    else
      ChangeStep=0-ActStep;

    if(DCRelayType==1)
   {
       switch(OutputMode)
     {
     case 1:  //VA
       StartVA=TripValue;
       StartVA1=TripValue1;
       StepVA=ChangeStep;
       break;
     case 2:  //VB
       StartVB=TripValue;
       StartVB1=TripValue1;
       StepVB=ChangeStep;
       break;
     case 3:  //VC
       StartVC=TripValue;
       StartVC1=TripValue1;
       StepVC=ChangeStep;
       break;
     case 4:  //VAB
       StartVA=TripValue/2;
       StartVA1=TripValue1/2;
       StepVA=ChangeStep/2;

       StartVB=-TripValue/2;
       StartVB1=-TripValue1/2;
       StepVB=-ChangeStep/2;
       break;
     case 5:  //VBC
       StartVB=TripValue/2;
       StartVB1=TripValue1/2;
       StepVB=ChangeStep/2;

       StartVC=-TripValue/2;
       StartVC1=-TripValue1/2;
       StepVC=-ChangeStep/2;

       break;
    case 6:  //VCA
       StartVA=TripValue/2;
       StartVA1=TripValue1/2;
       StepVA=ChangeStep/2;

       StartVC=-TripValue/2;
       StartVC1=-TripValue1/2;
       StepVC=-ChangeStep/2;
       break;
     }

   }
   else
   {
     switch(OutputMode)
     {

     case 1:  //VA
       StartIA=TripValue;
       StartIA1=TripValue1;
       StepIA=ChangeStep;
       break;
     case 2:  //VB
       StartIB=TripValue;
       StartIB1=TripValue1;
       StepIB=ChangeStep;
       break;
     case 3:  //VC
       StartIC=TripValue;
       StartIC1=TripValue1;
       StepIC=ChangeStep;
       break;
     case 4:  //IABC
           StartIC=TripValue/3;
           StartIC1=TripValue1/3;
           StepIC=ChangeStep/3;
           StartIA=TripValue/3;
           StartIA1=TripValue1/3;
           StepIA=ChangeStep/3;

           StartIB=TripValue/3;
           StartIB1=TripValue1/3;
           StepIB=ChangeStep/3;
           break;

     }

   }

    change_timedata=StepTime;
    artExec(fd,"inp:dig:interval(0)",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=0;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步


    if(DCRelayType==1)
  {  channel_input(fd,1,StartVA1,0,SystemFrec,V1DC,1,StepVA,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    channel_input(fd,1,StartVB1,0,SystemFrec,V2DC,2,StepVB,0,0);

    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    channel_input(fd,1,StartVC1,0,SystemFrec,V3DC,3,StepVC,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);
  }

    else
    {
            channel_input(fd,0,StartIA1,0,SystemFrec,I1DC,1,StepIA,0,0);
            artExec(fd,"others:steptime(1)",result,0);
            artExec(fd,"out:ana:i(1:1):on",result,0);

            channel_input(fd,0,StartIB1,0,SystemFrec,I2DC,2,StepIB,0,0);
            artExec(fd,"others:steptime(1)",result,0);
            artExec(fd,"out:ana:i(1:2):on",result,0);

            channel_input(fd,0,StartIC1,0,SystemFrec,I3DC,3,StepIC,0,0);
            artExec(fd,"others:steptime(1)",result,0);
            artExec(fd,"out:ana:i(1:3):on",result,0);
    }


    sumstep=floor(fabs((TripValue1-TripValue)/ActStep));
    out_time=sumstep*StepTime;
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);





    if(DCRelayType==1)
  {  channel_input(fd,1,StartVA,0,SystemFrec,V1DC,1,StepVA,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    channel_input(fd,1,StartVB,0,SystemFrec,V2DC,2,StepVB,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    channel_input(fd,1,StartVC,0,SystemFrec,V3DC,3,StepVC,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);
  }
    else
    {
            channel_input(fd,0,StartIA,0,SystemFrec,I1DC,1,StepIA,0,0);
            artExec(fd,"others:steptime(1)",result,0);
            artExec(fd,"out:ana:i(1:1):on",result,0);

            channel_input(fd,0,StartIB,0,SystemFrec,I2DC,2,StepIB,0,0);
            artExec(fd,"others:steptime(1)",result,0);
            artExec(fd,"out:ana:i(1:2):on",result,0);

            channel_input(fd,0,StartIC,0,SystemFrec,I3DC,3,StepIC,0,0);
            artExec(fd,"others:steptime(1)",result,0);
            artExec(fd,"out:ana:i(1:3):on",result,0);


    }

    if(StartValue>EndValue)
     {
     sumstep=floor(fabs((StartValue-TripValue)/ActStep))+1;
     out_time=sumstep*StepTime;
     }
     else if(StartValue<=EndValue)//(TestObject==2)
     {
        sumstep=floor(fabs((TripValue-0)/ActStep))+1;
        out_time=sumstep*StepTime;
     }
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);
    //2007-8-3 13:44,改为或的关系



    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);



    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
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

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
  }

  void OutputSequencer3(int fd)  //2009-10-08
  {


    float StartVA1=0;
    float StartVB1=0;
    float StartVC1=0;
    float StartIA1=0;
    float StartIB1=0;
    float StartIC1=0;

    float ValueOutput=0;
    ValueOutput=StopValue;

    if(DCRelayType==1)
   {
    switch(OutputMode)  //2009-10-08
     {
      case 1:  //VA

       StartVA1=ValueOutput;
       break;
     case 2:  //VB

       StartVB1=ValueOutput;
       break;
     case 3:  //VC

       StartVC1=ValueOutput;
       break;
     case 4:  //VAB

       StartVA1=ValueOutput/2;
       StartVB1=-ValueOutput/2;

       break;
     case 5:  //VBC
       StartVB1=ValueOutput/2;
       StartVC1=-ValueOutput/2;
       break;
    case 6:  //VCA

       StartVA1=ValueOutput/2;
       StartVC1=-ValueOutput/2;
       break;
     }
   }
    else
    {
        switch(OutputMode)  //2009-10-08
         {
          case 1:  //IA
           StartIA1=ValueOutput;
           break;
         case 2:  //IB
           StartIB1=ValueOutput;
           break;
         case 3:  //IC
           StartIC1=ValueOutput;
           break;
         case 4:  //IABC
           StartIA1=ValueOutput/3;
           StartIB1=ValueOutput/3;
           StartIC1=ValueOutput/3;
           break;
         }

    }
    artExec(fd,"inp:dig:interval(0)",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=0;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步

    out_time=-1/*prefaulttime*/;
    artExec(fd,"seq:wait(0,1)",result,0);

    if(DCRelayType==1)
  {
    channel_input(fd,1,StartVA1,0,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,StartVB1,0,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,StartVC1,0,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);
  }
    else
    {
        channel_input(fd,0,StartIA1,0,SystemFrec,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        channel_input(fd,0,StartIB1,0,SystemFrec,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        channel_input(fd,0,StartIC1,0,SystemFrec,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
    }

    //从0跳变到可靠动作值
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=StopValueHoldTime;
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);




    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);




    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
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

