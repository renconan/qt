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
extern float timeTime; //2016-05-20
extern float dtofdvdtHoldtime;
extern float SystemVolt;
extern int  dvdtmode;
extern float ChangeValue,dv,dt,dvdt;
//extern float prefaulttime;

extern float faultduration;
extern float prefaulttime;
extern float postfaulttime;

extern float StartVoltage,EndVoltage;
extern float ChangeValue;

extern float dvdtStopVoltage;  //dvdt的停止电压
extern float dvdtstart;
extern float dvdtend;
extern float dvdtStep;
extern float currentdvdt;
extern int Variable; //1 VAB 2 VBC 3 VCA


extern  float StopVoltage_triptime;//滑差闭锁终止电压
extern float dvdt_triptime;
extern float holdtime_triptime;

extern float VStart,VStop,VStep,VSteptime;
extern float StopVoltage;
extern float StopVoltageHoldTime;


extern float TripValue,TripValue1;

extern int   seq_count,TestItem1,TestItem2,TestItem3,TestItem4,TestItem5;

extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;


extern bool LowVoltageload;
extern float Current;


extern float dtofdvdt;
extern float dtofdvdttime;
extern float dtofdvdtvalue;


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
    float StepAdd=0.01;
    int sumstep;
    if(VStart>VStop)
      ChangeStep=0-VStep;
    else
      ChangeStep=VStep;


         StartVA=VStart;
         StepVA=ChangeStep;
         StartVC=VStart;
         StepVC=ChangeStep;
         StartVB=VStart;
         StepVB=ChangeStep;
         PhaseVA=0;
         PhaseVB=-120;
         PhaseVC=120;




        if(Current>0.0001)//2016-5-19
        {
            artExec(fd,"out:dig:on(0x74)",result,0);
            I1Output=1;
            I2Output=1;
            I3Output=1;
        }
        else
        {
            I1Output=0;
            I2Output=0;
            I3Output=0;
            artExec(fd,"out:dig:off(0x74)",result,0);
        }


    change_timedata=VSteptime;
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    channel_input(fd,1,SystemVolt,0,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,0,Current,75,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,Current,-45,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,Current,-165,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);


    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA,PhaseVA,SystemFrec,V1DC,1,StepVA,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

     artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB,PhaseVB,SystemFrec,V2DC,2,StepVB,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

     artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC,PhaseVC,SystemFrec,V3DC,3,StepVC,0,0); //2009-10-08
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

        channel_input(fd,0,Current,75,SystemFrec,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        channel_input(fd,0,Current,-45,SystemFrec,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        channel_input(fd,0,Current,-165,SystemFrec,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);



    sumstep=floor(fabs((VStart-VStop)/VStep))+1;
    out_time=sumstep*VSteptime;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);

    if(TestItem2==0)   //2015.7.1
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);
    else
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,2,0,1)",result,0);



    out_time=-1;
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
   /*
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
   */
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);



    out_time=-1;
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    /*
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
    */
    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);


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
    if(Current>0.0001)//2016-5-19
    {
        artExec(fd,"out:dig:on(0x74)",result,0);
        I1Output=1;
        I2Output=1;
        I3Output=1;
    }
    else
    {
        I1Output=0;
        I2Output=0;
        I3Output=0;
        artExec(fd,"out:dig:off(0x74)",result,0);
    }
    //artExec(fd,"out:dig:off(0x74)",result,0);


         StartVA1=VoltageOutput;
         StartVB1=VoltageOutput;
         StartVC1=VoltageOutput;

         PhaseVA=0;
         PhaseVB=-120;
         PhaseVC=120;

    artExec(fd,"inp:dig:interval(0)",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    channel_input(fd,1,SystemVolt,0,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,0,Current,75,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,Current,-45,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,Current,-165,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA1,PhaseVA,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB1,PhaseVB,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC1,PhaseVC,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,0,Current,75,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,Current,-45,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,Current,-165,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    //从0跳变到可靠动作值
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=StopVoltageHoldTime;
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);


    out_time=-1;
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
  /*
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
*/
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);



    out_time=-1;
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    /*
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

    */
    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);

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
    float StepVA=0;
    float StepVB=0;
    float StepVC=0;
    float PhaseVA=0;
    float PhaseVB=0;
    float PhaseVC=0;


      V1Output=1;
      V3Output=1;
      V2Output=1;
      StartVoltage=SystemVolt;





        if(Current>0.0001)//2016-5-19
        {
            artExec(fd,"out:dig:on(0x74)",result,0);
            I1Output=1;
            I2Output=1;
            I3Output=1;
        }
        else
        {
            I1Output=0;
            I2Output=0;
            I3Output=0;
            artExec(fd,"out:dig:off(0x74)",result,0);
        }





    change_timedata=dtofdvdt;
    if(dvdtStopVoltage>StartVoltage)  //增加的
    ChangeStep=currentdvdt*change_timedata;
    else
    ChangeStep=-currentdvdt*change_timedata;
    int sumstep;





        StartVA=StartVoltage;
        StepVA=ChangeStep;
        StartVC=StartVoltage;
        StepVC=ChangeStep;
        StartVB=StartVoltage;
        StepVB=ChangeStep;
        PhaseVA=0;
        PhaseVB=-120;
        PhaseVC=120;


 //   //artExec(fd,"inp:dig(1):thres(1)",result,0);//打开放大器
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    channel_input(fd,1,SystemVolt,0,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,0,Current,75,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,Current,-45,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,Current,-165,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA,PhaseVA,SystemFrec,V1DC,1,StepVA,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB,PhaseVB,SystemFrec,V2DC,2,StepVB,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

     artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC,PhaseVC,SystemFrec,V3DC,3,StepVC,0,0); //2009-10-08
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);


        channel_input(fd,0,Current,75,SystemFrec,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        channel_input(fd,0,Current,-45,SystemFrec,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        channel_input(fd,0,Current,-165,SystemFrec,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);







    sumstep=floor(fabs((SystemVolt-EndVoltage)/ChangeStep))+1;



    out_time=sumstep*change_timedata;


    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);


    out_time=-1;
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);


    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);

    out_time=-1;
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);



    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);


    //第4步，接点有动作，关断信号
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

void  OutputSequencer5(int fd)//动作时间
  {
    float StartVoltage;
    float EndVoltage;
    float Timevol=dtofdvdtvalue;//计时电压
    TimeVoltage=Timevol;
    int sumstep=0;
    int timesumstep=0;

    StartVoltage=SystemVolt;
    EndVoltage=StopVoltage_triptime;

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
    change_timedata=dtofdvdttime;//滑差的步长时间
    if(StopVoltage_triptime>=StartVoltage)//滑差闭锁终止电压>=起始电压
    ChangeStep=dvdt_triptime*change_timedata;//滑差一步的步长
    else
    ChangeStep=-dvdt_triptime*change_timedata;


         StartVA=StartVoltage;
         StepVA=ChangeStep;
         StartVC=StartVoltage;
         StepVC=ChangeStep;
         StartVB=StartVoltage;
         StepVB=ChangeStep;
         PhaseVA=0;
         PhaseVB=-120;
         PhaseVC=120;


        if(Current>0.0001)//2016-5-19电流为0，报过载
        {
            artExec(fd,"out:dig:on(0x74)",result,0);
            I1Output=1;
            I2Output=1;
            I3Output=1;
        }
        else
        {
            I1Output=0;
            I2Output=0;
            I3Output=0;
            artExec(fd,"out:dig:off(0x74)",result,0);
        }


    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=0;
    artExec(fd,"seq:wait(0,1)",result,0);

    channel_input(fd,1,SystemVolt,0,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,0,Current,75,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,Current,-45,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,Current,-165,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);



      artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA,PhaseVA,SystemFrec,V1DC,1,StepVA,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

     artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB,PhaseVB,SystemFrec,V2DC,2,StepVB,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

     artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC,PhaseVC,SystemFrec,V3DC,3,StepVC,0,0); //2009-10-08
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);




        channel_input(fd,0,Current,75,SystemFrec,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        channel_input(fd,0,Current,-45,SystemFrec,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        channel_input(fd,0,Current,-165,SystemFrec,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);



       timesumstep=floor(fabs((StartVoltage-Timevol)/ChangeStep))+1;
       sumstep=floor(fabs((StartVoltage-EndVoltage)/ChangeStep))+1;
       //sumstep=floor(fabs((SystemVolt-EndVoltage)/ChangeStep))+1;

    timeTime=timesumstep*change_timedata;  //计时时间 ，用于结果处理
    out_time=sumstep*change_timedata;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);//第二步
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);


    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,EndVoltage,PhaseVA,SystemFrec,V1DC,1,0,0,0);//后三位为幅值，相位，频率的步长2016-5-21
//    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

     artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,EndVoltage,PhaseVB,SystemFrec,V2DC,2,0,0,0);
//    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

     artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,EndVoltage,PhaseVC,SystemFrec,V3DC,3,0,0,0); //2009-10-08
//    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);


        channel_input(fd,0,Current,75,SystemFrec,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        channel_input(fd,0,Current,-45,SystemFrec,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        channel_input(fd,0,Current,-165,SystemFrec,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);


    out_time=dtofdvdtHoldtime;
//    artExec(fd,"seq:wait(0,2)",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);//第三步
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),1,1,0,2)",result,0);

    out_time=-1;
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);

    artExec(fd,"inp:buf:sam(bin,4)",result,0);//第四步
    artExec(fd,"seq:wait(0,1)",result,0);

    //第4步，接点有动作，关断信号

    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);

    artExec(fd,"inp:buf:sam(bin,5)",result,0);//第五步
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
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);

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


