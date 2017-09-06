
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



extern TEMPRORARYDATA *temprorarydata;

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int    I1DC,I2DC,I3DC;
extern int    V1DC,V2DC,V3DC;

extern int    OutputMode;
extern float  LogicResolution,SystemFrec;
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

extern float IENDSTART;
extern float IENDEND;
extern float ISTEP;
extern float STEPTIME;

extern float IEND;
extern float HOLDTIME;
extern int   Testitem;
extern float faultduration;
extern float prefaulttime; //故障前时间

extern float V1Value,V1Phase;
extern float V2Value,V2Phase;
extern float V3Value,V3Phase;


extern float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase;

extern float PhaseIA;
extern float PhaseIB;
extern float PhaseIC;

extern float Vfault;
extern float Angle;
extern float VpnValue; //单相的幅值
extern float VpnAngle;

extern float SystemVolt;

extern float CurrentI;

extern int Firstrun; //第一次运行
extern float Posttime;
extern float PrepareTime;




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

    switch(OutputMode)
     {
     case 1:  //IA
       StartCurIA=CurrentI;
       artExec(fd,"out:dig:on(0x10)",result,0);
       break;
     case 2:  //IB
       StartCurIB=CurrentI;
       artExec(fd,"out:dig:on(0x20)",result,0);
       break;
     case 3:  //IC
       StartCurIC=CurrentI;
       artExec(fd,"out:dig:on(0x40)",result,0);
       break;
     case 4:
         StartCurIA=CurrentI;
         StartCurIB=CurrentI;
         artExec(fd,"out:dig:on(0x30)",result,0);
         break;

     case 5:
         StartCurIB=CurrentI;
         StartCurIC=CurrentI;
         artExec(fd,"out:dig:on(0x60)",result,0);
         break;
     case 6:
         StartCurIA=CurrentI;
         StartCurIC=CurrentI;
         artExec(fd,"out:dig:on(0x50)",result,0);
         break;
     case 7:  //IAIBIC
       StartCurIA=CurrentI;
       StartCurIB=CurrentI;
       StartCurIC=CurrentI;
       artExec(fd,"out:dig:on(0x70)",result,0);
       break;
     }





    change_timedata=STEPTIME;  //步长时间
   // artExec(fd,"inp:dig(1):thres(1)",result,0);//打开放大器
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
    if(Firstrun==1)
   { out_time=prefaulttime+PrepareTime;
     Firstrun=0;
   }
    else
    out_time=prefaulttime;

    artExec(fd,"seq:wait(0,1)",result,0);//第一步



    channel_input(fd,0,StartCurIA,PhaseIA,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,StartCurIB,PhaseIB,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,StartCurIC,PhaseIC,SystemFrec,I3DC,3,0,0,0);
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
    out_time=STEPTIME;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);
    //2007-8-3 13:44,改为或的关系

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=Posttime;
    artExec(fd,"seq:wait(0,2)",result,0);



    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    out_time=Posttime;
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



    switch(OutputMode)
     {
     case 1:  //IA
       StartCurIA=IEND;
       artExec(fd,"out:dig:on(0x10)",result,0);
       break;
     case 2:  //IB
       StartCurIB=IEND;
       artExec(fd,"out:dig:on(0x20)",result,0);
       break;
     case 3:  //IC
       StartCurIC=IEND;
       artExec(fd,"out:dig:on(0x40)",result,0);
       break;
     case 4:
      StartCurIA=IEND;
      StartCurIB=IEND;
      artExec(fd,"out:dig:on(0x30)",result,0);
      break;
     case 5:
      StartCurIC=IEND;
      StartCurIB=IEND;
      artExec(fd,"out:dig:on(0x60)",result,0);
      break;
     case 6:
      StartCurIC=IEND;
      StartCurIA=IEND;
      artExec(fd,"out:dig:on(0x50)",result,0);
      break;
     case 7:  //IAIBIC
       artExec(fd,"out:dig:on(0x70)",result,0);
       StartCurIA=IEND;
       StartCurIB=IEND;
       StartCurIC=IEND;
       break;
     }


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

    //测试动作时间时，先输出0
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    if(Firstrun==1)
   { out_time=prefaulttime+PrepareTime;
     Firstrun=0;
   }
    else
    out_time=prefaulttime;



    artExec(fd,"seq:wait(0,1)",result,0);

    channel_input(fd,0,StartCurIA,PhaseIA,SystemFrec,I1DC,1,0,0,0);
    //artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,StartCurIB,PhaseIB,SystemFrec,I2DC,2,0,0,0);
  //  artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,StartCurIC,PhaseIC,SystemFrec,I3DC,3,0,0,0);
  //  artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
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

    //从0跳变到可靠动作值
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=HOLDTIME;
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);





    channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,0,0,360,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):off",result,0);
    channel_input(fd,0,0,360,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):off",result,0);
    channel_input(fd,0,0,360,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):off",result,0);

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=Posttime;
    artExec(fd,"seq:wait(0,2)",result,0);




    channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,0,0,360,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):off",result,0);
    channel_input(fd,0,0,360,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):off",result,0);
    channel_input(fd,0,0,360,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):off",result,0);

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    out_time=Posttime;
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


