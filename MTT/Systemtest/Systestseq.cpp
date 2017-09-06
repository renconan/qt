#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include "Testcom2.h"
#include "Systestseq.h"
#include <QDebug>

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int   low,row;

extern int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC;
extern float SystemFrec,LogicResolution,SystemVolt;
extern int   FaultForm,FaultType;
extern float KeepTime,LoadCurrent,Impedance,ImpedanceAngle,FixCurrent,FixTime,KL,WaitTime,DelayTime;

extern float  V1PerFaultValue,V2PerFaultValue,V3PerFaultValue,I1PerFaultValue,I2PerFaultValue,I3PerFaultValue;
extern float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase,I1PerFaultPhase,I2PerFaultPhase,I3PerFaultPhase;

extern float  V1FaultValue,V2FaultValue,V3FaultValue,I1FaultValue,I2FaultValue,I3FaultValue;
extern float  V1FaultPhase,V2FaultPhase,V3FaultPhase,I1FaultPhase,I2FaultPhase,I3FaultPhase;

extern float  V1PostFaultValue,V2PostFaultValue,V3PostFaultValue,I1PostFaultValue,I2PostFaultValue,I3PostFaultValue;
extern float  V1PostFaultPhase,V2PostFaultPhase,V3PostFaultPhase,I1PostFaultPhase,I2PostFaultPhase,I3PostFaultPhase;

const float EPSINON2 =1.0e-04;

extern  float KLphase;


bool checkiszero(float x)
{

    if ((x >= -EPSINON2) && (x <= EPSINON2))
           return true;
    else
        return false;
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
    artExec(fd,"out:dig:off(0x74)",result,0);
    artExec(fd,"out:dig:off(0x08)",result,0);
  }

void OutputSequencer(int fd)
  {
   // artExec(fd,"inp:dig(1):thres(1)",result,0);//打开放大器


    artExec(fd,"out:dig:on(0x04)",result,0);
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    artExec(fd,"others:steptime(1)",result,0);//2009-6-23

    if(checkiszero(I1PerFaultValue)==false||checkiszero(I1FaultValue)==false||checkiszero(I1PostFaultValue)==false)
    artExec(fd,"out:dig:on(0x10)",result,0);
    if(checkiszero(I2PerFaultValue)==false||checkiszero(I2FaultValue)==false||checkiszero(I2PostFaultValue)==false)
    artExec(fd,"out:dig:on(0x20)",result,0);
    if(checkiszero(I3PerFaultValue)==false||checkiszero(I3FaultValue)==false||checkiszero(I3PostFaultValue)==false)
    artExec(fd,"out:dig:on(0x40)",result,0);

    //2016-8-27相位连续修改
    switch(FaultType)
    {
    case 1:
        V1PerFaultPhase=V2PerFaultPhase+120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V3PerFaultPhase=V2PerFaultPhase-120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
        I1PerFaultPhase=I2PerFaultPhase+120;
        I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
        I3PerFaultPhase=I2PerFaultPhase-120;
        I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        break;
    case 2:
        V1PerFaultPhase=V3PerFaultPhase-120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V2PerFaultPhase=V3PerFaultPhase+120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        I1PerFaultPhase=I3PerFaultPhase-120;
        I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
        I2PerFaultPhase=I3PerFaultPhase+120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        break;
    case 3:
        V2PerFaultPhase=V1PerFaultPhase-120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        V3PerFaultPhase=V1PerFaultPhase+120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
        I2PerFaultPhase=I1PerFaultPhase-120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        I3PerFaultPhase=I1PerFaultPhase+120;
        I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        break;
    case 4:
        V1PerFaultPhase=V3PerFaultPhase-120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V2PerFaultPhase=V3PerFaultPhase+120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        I1PerFaultPhase=I3PerFaultPhase-120;
        I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
        I2PerFaultPhase=I3PerFaultPhase+120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        break;
    case 5:
        V2PerFaultPhase=V1PerFaultPhase-120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        V3PerFaultPhase=V1PerFaultPhase+120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
        I2PerFaultPhase=I1PerFaultPhase-120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        I3PerFaultPhase=I1PerFaultPhase+120;
        I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        break;
    case 6:
         V1PerFaultPhase=V2PerFaultPhase+120;
         V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
         V3PerFaultPhase=V2PerFaultPhase-120;
         V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
         I1PerFaultPhase=I2PerFaultPhase+120;
         I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
         I3PerFaultPhase=I2PerFaultPhase-120;
         I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        break;
    }

    channel_input(fd,1,V1PerFaultValue,V1PerFaultPhase,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,V2PerFaultValue,V2PerFaultPhase,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,V3PerFaultValue,V3PerFaultPhase,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);
    channel_input(fd,0,I1PerFaultValue,I1PerFaultPhase,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,I2PerFaultValue,I2PerFaultPhase,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,I3PerFaultValue,I3PerFaultPhase,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=KeepTime;
    artExec(fd,"seq:wait(0,1)",result,0);
    //第一步,正常态,输出正常电压电流，timeout后往下跳1步到故障态。

    artExec(fd,"others:steptime(1)",result,0);//2009-6-23

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1FaultValue,V1FaultPhase,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2FaultValue,V2FaultPhase,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3FaultValue,V3FaultPhase,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I1FaultValue,I1FaultPhase,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);


    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,I2FaultValue,I2FaultPhase,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);


    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,I3FaultValue,I3FaultPhase,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=FixTime;
    artExec(fd,"seq:wait(orbin(1),1,0,0,2)",result,0);
    //第二步，一次故障态，timeout后往下跳2步到第4步，trip后跳1步到第3步重合闸等待态。动作接点为1号接点

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1PostFaultValue,360,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2PostFaultValue,360,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3PostFaultValue,360,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I1PostFaultValue,360,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I2PostFaultValue,360,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);


    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,I3PostFaultValue,360,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);
    //07-4-23 15:05 第三步 一次故障跳闸后，应该等待保护恢复为高电平，所以要加入一个等待态，timeout跳到第5步。

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1PostFaultValue,360,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);


    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2PostFaultValue,360,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3PostFaultValue,360,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I1PostFaultValue,360,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I2PostFaultValue,360,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);


    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,I3PostFaultValue,360,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    out_time=-1;
    //artExec(fd,"seq:wait(0,2)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);//timemout后跳1步到重合态2016-9-2
    //第4步，表示一次故障或重合闸保护未动作，输出正常态。timeout后跳2步到第6步。

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1PostFaultValue,0/*360*/,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2PostFaultValue,-120/*360*/,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3PostFaultValue,120/*360*/,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I1PostFaultValue,360,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I2PostFaultValue,360,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);

    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,I3PostFaultValue,360,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=WaitTime;
    artExec(fd,"seq:wait(orbin(0),bin(1),2,0,0,1)",result,0);
    //第5步为重合闸态，2号接点 ，trip后下跳2步到第7步，timeout后下跳1步到第6步

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1PostFaultValue,360,SystemFrec,V1DC,1,0,0,0);

    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2PostFaultValue,360,SystemFrec,V2DC,2,0,0,0);

    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3PostFaultValue,360,SystemFrec,V3DC,3,0,0,0);

    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

     artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I1PostFaultValue,360,SystemFrec,I1DC,1,0,0,0);

    artExec(fd,"out:ana:i(1:1):on",result,0);

    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I2PostFaultValue,360,SystemFrec,I2DC,2,0,0,0);

    artExec(fd,"out:ana:i(1:2):on",result,0);

    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,I3PostFaultValue,360,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,6)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);
    //第6步，表示一次故障或重合闸保护未动作，输出正常态。timeout后跳2步到第8步。

    artExec(fd,"inp:buf:sam(bin,7)",result,0);
    out_time=DelayTime;
    artExec(fd,"seq:wait(0,2)",result,0);
    //第7步，开关重合延时，timeout后下跳2步到第9步二次故障态

    artExec(fd,"inp:buf:sam(bin,8)",result,0);
    out_time=-1;
    //artExec(fd,"seq:wait(0,2)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);//timeout后下跳1步到第10步2016-9-2
    //第8步，timeout后下跳2步到第10步

    if(FaultForm==2)
      {
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,V1FaultValue,V1FaultPhase,SystemFrec,V1DC,1,0,0,0);

        artExec(fd,"out:ana:v(1:1):on",result,0);

        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,V2FaultValue,V2FaultPhase,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);


        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,V3FaultValue,V3FaultPhase,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);

        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);


        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,I1FaultValue,I1FaultPhase,SystemFrec,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);


        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,I2FaultValue,I2FaultPhase,SystemFrec,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);

        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,I3FaultValue,I3FaultPhase,SystemFrec,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
      }//故障形式为永久性故障时，第7步才输出故障信号，为瞬时性故障时，第7步不输出信号

    artExec(fd,"inp:buf:sam(bin,9)",result,0);

    if(FaultForm==1)
      {
        out_time=-1;
        artExec(fd,"seq:wait(0,2)",result,0);
        //故障形式为瞬时性故障时，第9步不输出信号，timeout后下跳2步
      }
    else if(FaultForm==2)
      {
        out_time=FixTime;
        artExec(fd,"seq:wait(orbin(1),2,0,0,1)",result,0);
        //故障形式为永久性故障时，第9步为二次故障态，1号接点 ，timeout后往下跳一步到第10步，trip后跳2步到第11步。
      }

    artExec(fd,"inp:buf:sam(bin,10)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);
    //第10步，timeout后跳2步到第12步。

    artExec(fd,"inp:buf:sam(bin,11)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
    //第11步。timeout后跳1步到第12步。

    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,12)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
  }

/*
void OutputSequencer(int fd)
  {
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    artExec(fd,"others:steptime(1)",result,0);//2009-6-23

    if(checkiszero(I1PerFaultValue)==false||checkiszero(I1FaultValue)==false||checkiszero(I1PostFaultValue)==false)
    artExec(fd,"out:dig:on(0x10)",result,0);
    if(checkiszero(I2PerFaultValue)==false||checkiszero(I2FaultValue)==false||checkiszero(I2PostFaultValue)==false)
    artExec(fd,"out:dig:on(0x20)",result,0);
    if(checkiszero(I3PerFaultValue)==false||checkiszero(I3FaultValue)==false||checkiszero(I3PostFaultValue)==false)
    artExec(fd,"out:dig:on(0x40)",result,0);

    //2016-8-27相位连续修改
    switch(FaultType)
    {
    case 1:
        V1PerFaultPhase=V2PerFaultPhase+120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V3PerFaultPhase=V2PerFaultPhase-120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
        I1PerFaultPhase=I2PerFaultPhase+120;
        I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
        I3PerFaultPhase=I2PerFaultPhase-120;
        I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        break;
    case 2:
        V1PerFaultPhase=V3PerFaultPhase-120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V2PerFaultPhase=V3PerFaultPhase+120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        I1PerFaultPhase=I3PerFaultPhase-120;
        I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
        I2PerFaultPhase=I3PerFaultPhase+120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        break;
    case 3:
        V2PerFaultPhase=V1PerFaultPhase-120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        V3PerFaultPhase=V1PerFaultPhase+120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
        I2PerFaultPhase=I1PerFaultPhase-120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        I3PerFaultPhase=I1PerFaultPhase+120;
        I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        break;
    case 4:
        V1PerFaultPhase=V3PerFaultPhase-120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V2PerFaultPhase=V3PerFaultPhase+120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        I1PerFaultPhase=I3PerFaultPhase-120;
        I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
        I2PerFaultPhase=I3PerFaultPhase+120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        break;
    case 5:
        V2PerFaultPhase=V1PerFaultPhase-120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        V3PerFaultPhase=V1PerFaultPhase+120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
        I2PerFaultPhase=I1PerFaultPhase-120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        I3PerFaultPhase=I1PerFaultPhase+120;
        I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        break;
    case 6:
         V1PerFaultPhase=V2PerFaultPhase+120;
         V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
         V3PerFaultPhase=V2PerFaultPhase-120;
         V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
         I1PerFaultPhase=I2PerFaultPhase+120;
         I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
         I3PerFaultPhase=I2PerFaultPhase-120;
         I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        break;
    }

    channel_input(fd,1,V1PerFaultValue,V1PerFaultPhase,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,V2PerFaultValue,V2PerFaultPhase,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,V3PerFaultValue,V3PerFaultPhase,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);
    channel_input(fd,0,I1PerFaultValue,I1PerFaultPhase,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,I2PerFaultValue,I2PerFaultPhase,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,I3PerFaultValue,I3PerFaultPhase,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=KeepTime;
    artExec(fd,"seq:wait(0,1)",result,0);
    //第一步,正常态,输出正常电压电流，timeout后往下跳1步到故障态。

    artExec(fd,"others:steptime(1)",result,0);//2009-6-23

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1FaultValue,V1FaultPhase,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2FaultValue,V2FaultPhase,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3FaultValue,V3FaultPhase,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I1FaultValue,I1FaultPhase,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);


    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,I2FaultValue,I2FaultPhase,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);


    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,I3FaultValue,I3FaultPhase,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=FixTime;
    artExec(fd,"seq:wait(orbin(1),2,0)",result,0);
    //第二步，一次故障态，timeout跳1步到第3步重合闸，trip后立即跳1步到第3步重合闸。动作接点为1号接点



    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1PostFaultValue,360,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2PostFaultValue,360,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3PostFaultValue,360,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I1PostFaultValue,360,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I2PostFaultValue,360,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);

    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,I3PostFaultValue,360,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=WaitTime;
    artExec(fd,"seq:wait(orbin(0),bin(1),2,0)",result,0);
    //第3步为重合闸态，2号接点 ，trip后立即跳1步到第4步二次故障态，timeout跳1步到第4步二次故障态

    if(FaultForm==2)
      {
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,V1FaultValue,V1FaultPhase,SystemFrec,V1DC,1,0,0,0);

        artExec(fd,"out:ana:v(1:1):on",result,0);

        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,V2FaultValue,V2FaultPhase,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);


        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,V3FaultValue,V3FaultPhase,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);

        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);


        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,I1FaultValue,I1FaultPhase,SystemFrec,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);


        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,I2FaultValue,I2FaultPhase,SystemFrec,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);

        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,I3FaultValue,I3FaultPhase,SystemFrec,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
      }//故障形式为永久性故障时，第4步才输出故障信号，为瞬时性故障时，第4步不输出信号

    artExec(fd,"inp:buf:sam(bin,4)",result,0);

    if(FaultForm==1)
      {
        out_time=-1;
        artExec(fd,"seq:wait(0,1)",result,0);
        //故障形式为瞬时性故障时，timeout后下跳1步
      }
    else if(FaultForm==2)
      {
        out_time=FixTime;
        artExec(fd,"seq:wait(orbin(1),2,0)",result,0);
        //故障形式为永久性故障时，第4步为二次故障态，1号接点 ，timeout往下跳一步到第5步，trip后立即跳1步到第5步。
      }


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
*/
