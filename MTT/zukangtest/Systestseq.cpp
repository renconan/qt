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

const float EPSINON2 =-2.214e-1;

extern  float KLphase;


bool checkiszero(float x)
{

    if ((x >= EPSINON2) && (x <=-EPSINON2))
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


