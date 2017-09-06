#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include "Zoneseq.h"
#include <QDebug>

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern int   low,row,seq_count;
extern int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC;

extern int   Segment1,Segment2,Segment3,Segment4,EqualizeQuotiety;


extern float Impedance1,Impedance2,Impedance3,Impedance4;
extern float DelicacyAngle1,DelicacyAngle2,DelicacyAngle3,DelicacyAngle4;
extern float FixupCurrent1,FixupCurrent2,FixupCurrent3,FixupCurrent4;
extern float FixupTime1,FixupTime2,FixupTime3,FixupTime4;
extern float EqualizeValue;

extern int   FaultType1,FaultType2,FaultType3,FaultType4,FaultType5,FaultType6,FaultType7;
extern float PerFaultTime,PostFaultTime,FaultTime,SystemFrec;

extern float  V1PerFaultValue,V2PerFaultValue,V3PerFaultValue,I1PerFaultValue,I2PerFaultValue,I3PerFaultValue;
extern float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase,I1PerFaultPhase,I2PerFaultPhase,I3PerFaultPhase;

extern float  V1FaultValue,V2FaultValue,V3FaultValue,I1FaultValue,I2FaultValue,I3FaultValue;
extern float  V1FaultPhase,V2FaultPhase,V3FaultPhase,I1FaultPhase,I2FaultPhase,I3FaultPhase;

extern float  V1PostFaultValue,V2PostFaultValue,V3PostFaultValue,I1PostFaultValue,I2PostFaultValue,I3PostFaultValue;
extern float  V1PostFaultPhase,V2PostFaultPhase,V3PostFaultPhase,I1PostFaultPhase,I2PostFaultPhase,I3PostFaultPhase;
extern float faultduration;

extern float KLphase;

const float EPSINON2 =1.0e-04;
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


    artExec(fd,"out:dig:on(0x08)",result,0);
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

    if(FaultType1==1)
    {
        V1PerFaultPhase=V2PerFaultPhase+120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V3PerFaultPhase=V2PerFaultPhase-120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
    }
    if(FaultType2==1)
    {
        V1PerFaultPhase=V3PerFaultPhase-120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V2PerFaultPhase=V3PerFaultPhase+120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
    }
    if(FaultType3==1)
    {
        V2PerFaultPhase=V1PerFaultPhase-120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        V3PerFaultPhase=V1PerFaultPhase+120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
    }
    if(FaultType4==1)
    {
        V1PerFaultPhase=V3PerFaultPhase-120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V2PerFaultPhase=V3PerFaultPhase+120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
    }
    if(FaultType5==1)
    {
        V2PerFaultPhase=V1PerFaultPhase-120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        V3PerFaultPhase=V1PerFaultPhase+120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
    }
    if(FaultType6==1)
    {
        V1PerFaultPhase=V2PerFaultPhase+120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V3PerFaultPhase=V2PerFaultPhase-120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
    }
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
    out_time=PerFaultTime;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步

    artExec(fd,"others:steptime(1)",result,0);//2009-6-23






    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1FaultValue,V1FaultPhase/*360*/,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2FaultValue,V2FaultPhase/*360*/,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);


    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3FaultValue,V3FaultPhase/*360*/,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,I1FaultValue,360,SystemFrec,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);


    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,I2FaultValue,360,SystemFrec,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);


    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,I3FaultValue,360,SystemFrec,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);


    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=FaultTime;
    //artExec(fd,"seq:wait(orbin(1),2,0,1)",result,0);//第二步
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);
    //2007-8-3 13:44,改为或的关系






    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1PostFaultValue,V1PerFaultPhase/*360*/,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);


    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2PostFaultValue,V2PerFaultPhase/*360*/,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3PostFaultValue,V3PerFaultPhase/*360*/,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
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
    out_time=PostFaultTime;
    artExec(fd,"seq:wait(0,2)",result,0);

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1PostFaultValue,V1PerFaultPhase/*360*/,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2PostFaultValue,V2PerFaultPhase/*360*/,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);


    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3PostFaultValue,V3PerFaultPhase/*360*/,SystemFrec,V3DC,3,0,0,0);
   artExec(fd,"out:ana:v(1:3):on",result,0);

   artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
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
    out_time=PostFaultTime;
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
