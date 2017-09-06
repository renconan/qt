#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include "differseq.h"

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int   seq_count;

extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;
extern float SystemFrec;
extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;


extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern float prefaulttime;


extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern float prefaulttime;
extern float  SystemVolt;

extern int ConnectType;
extern int Testobject;
extern float prefaulttime;

extern float PhaseCorrectMode; //△侧校正
extern float PhaseCorrectStep;
extern float PhaseCorrectStepTime; //步长时间

extern float IdSearchStep;
extern float IdSearchStepTime;

extern float IrSeachStart;
extern float IrSeachEnd;
extern float IrSeachStep;

extern float Kp1;
extern float Kp2;
extern float Kp3;


extern float IaValue,IaPhase,IaStep;
extern float IbValue,IbPhase,IbStep;
extern float IcValue,IcPhase,IcStep;


extern float StepTime;

extern int CTHdir;
extern int CTMdir;
extern int CTLdir;


extern float CurrrentIrSearch;




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
    artExec(fd,"out:dig:off(0x74)",result,0); //将蜂鸣器和电流ON 关闭
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
    artExec(fd,"out:dig:off(0x08)",result,0);

}


void OutputSequencer2(int fd)
  {

    artExec(fd,"out:dig:on(0x08)",result,0);
    //2016-05-19
    if(IcValue>0.001)
    {
        artExec(fd,"out:dig:on(0x74)",result,0);
    }
    else
    {
        artExec(fd,"out:dig:on(0x34)",result,0); //On IA and IB
    }
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    if(CurrrentIrSearch<=3)
    out_time=0.5;
    else if(CurrrentIrSearch>=3)
     {
       out_time=1.5*(CurrrentIrSearch-3)/3+1.5;
     }
    artExec(fd,"seq:wait(0,1)",result,0);//第一步

     channel_input(fd,0,IaValue,IaPhase,SystemFrec,I1DC,1,0,0,0);
     artExec(fd,"out:ana:i(1:1):on",result,0);
     channel_input(fd,0,IbValue,IbPhase,SystemFrec,I2DC,2,0,0,0);
     artExec(fd,"out:ana:i(1:2):on",result,0);
     channel_input(fd,0,IcValue,IcPhase,SystemFrec,I3DC,3,0,0,0);
     artExec(fd,"out:ana:i(1:3):on",result,0);
     artExec(fd,"inp:buf:sam(bin,2)",result,0);//2
     out_time=0.5;
     artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);

     artExec(fd,"out:ana:i(1:1):off",result,0);
     artExec(fd,"out:ana:i(1:2):off",result,0);
     artExec(fd,"out:ana:i(1:3):off",result,0);

     artExec(fd,"out:ana:i(2:1):off",result,0);
     artExec(fd,"out:ana:i(2:2):off",result,0);
     artExec(fd,"out:ana:i(2:3):off",result,0);
     artExec(fd,"inp:buf:sam(bin,3)",result,0);//3
     out_time=-1;
     artExec(fd,"seq:wait(0,2)",result,0);

     artExec(fd,"out:ana:i(1:1):off",result,0);
     artExec(fd,"out:ana:i(1:2):off",result,0);
     artExec(fd,"out:ana:i(1:3):off",result,0);
     artExec(fd,"out:ana:i(2:1):off",result,0);
     artExec(fd,"out:ana:i(2:2):off",result,0);
     artExec(fd,"out:ana:i(2:3):off",result,0);
     artExec(fd,"inp:buf:sam(bin,4)",result,0);//4
     out_time=-1;
     artExec(fd,"seq:wait(0,1)",result,0);


     artExec(fd,"out:ana:v(1:1):off",result,0);
     artExec(fd,"out:ana:v(1:2):off",result,0);
     artExec(fd,"out:ana:v(1:3):off",result,0);
     artExec(fd,"out:ana:i(2:1):off",result,0);
     artExec(fd,"out:ana:i(2:2):off",result,0);
     artExec(fd,"out:ana:i(2:3):off",result,0);

     artExec(fd,"inp:buf:sam(bin,5)",result,0);//5
     out_time=-1;
     artExec(fd,"seq:wait(0,1)",result,0);

     artExec(fd,"seq:end",result,0);
     artExec(fd,"seq:exec",result,0);
  }

