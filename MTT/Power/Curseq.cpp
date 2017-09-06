
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
extern float SystemFrec;
extern int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC;
extern float FixCurrent,FixVoltage,MSAngle,StartPhase,EndPhase,OutPhase;
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

extern int   faulttype;
extern float SystemVolt;

extern float prefaulttime;
extern float StepTime;


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
    artExec(fd,"seq:wait(1,1)",result,0);    
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);   
    artExec(fd,"out:dig:off(0x74)",result,0);
    artExec(fd,"out:dig:off(0x08)",result,0);
  }  

void OutputSequencer(int fd)
  {//测试动作时间的sequencer

    float OutPhasevalue;
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0); 
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    if(OutPhase>360)
       OutPhasevalue=OutPhase-360;
  else  if(OutPhase<-360)
       OutPhasevalue=OutPhase+360;
  else  OutPhasevalue=OutPhase;






artExec(fd,"out:dig:on(0x04)",result,0);
    switch(faulttype)
    {
    case 1://A-E
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=0;
        I3Output=0;
            channel_input(fd,1,SystemVolt,0,SystemFrec,V1DC,1,0,0,0);
            artExec(fd,"out:ana:v(1:1):on",result,0);
            channel_input(fd,1,SystemVolt,-120,SystemFrec,V2DC,2,0,0,0);
            artExec(fd,"out:ana:v(1:2):on",result,0);
            channel_input(fd,1,SystemVolt,120,SystemFrec,V3DC,3,0,0,0);
            artExec(fd,"out:ana:v(1:3):on",result,0);
            channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
            artExec(fd,"out:ana:v(1:4):on",result,0);
            break;

    case 2: //B-E
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=0;
        I2Output=1;
        I3Output=0;
        channel_input(fd,1,SystemVolt,120,SystemFrec,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        channel_input(fd,1,SystemVolt,0,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        channel_input(fd,1,SystemVolt,-120,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);
        break;
    case 3: //C-E
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=0;
        I2Output=0;
        I3Output=1;
        channel_input(fd,1,SystemVolt,-120,SystemFrec,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        channel_input(fd,1,SystemVolt,120,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        channel_input(fd,1,SystemVolt,0,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);
        break;
    case 4: //A-B-C
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=1;
        channel_input(fd,1,SystemVolt,0,SystemFrec,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        channel_input(fd,1,SystemVolt,-120,SystemFrec,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        channel_input(fd,1,SystemVolt,120,SystemFrec,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);
        break;
    }
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步


    //channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    //artExec(fd,"out:ana:v(1:4):on",result,0);

    switch(faulttype)
    {
    case 1:

         artExec(fd,"out:dig:on(0x14)",result,0);
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,FixVoltage,360,SystemFrec,V1DC,1,0,0,0); //VA输入值 相位0
        artExec(fd,"out:ana:v(1:1):on",result,0);
        channel_input(fd,0,FixCurrent,OutPhasevalue,SystemFrec,I1DC,1,0,0,0); //IA输入值 输入相位
        artExec(fd,"out:ana:i(1:1):on",result,0);
        break;
    case 2:
        artExec(fd,"out:dig:on(0x24)",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,FixVoltage,360,SystemFrec,V2DC,2,0,0,0); //VB输入值 0度
        artExec(fd,"out:ana:v(1:2):on",result,0);

        channel_input(fd,0,FixCurrent,OutPhasevalue,SystemFrec,I2DC,2,0,0,0);  //IB输入值 输入相位
        artExec(fd,"out:ana:i(1:2):on",result,0);
        break;
    case 3:
        artExec(fd,"out:dig:on(0x44)",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,FixVoltage,360,SystemFrec,V3DC,3,0,0,0); //VC系统电压 -120度
        artExec(fd,"out:ana:v(1:3):on",result,0);
        channel_input(fd,0,FixCurrent,OutPhasevalue,SystemFrec,I3DC,3,0,0,0);  //IC输入值 输入相位
        artExec(fd,"out:ana:i(1:3):on",result,0);
        break;



    case 4:

        artExec(fd,"out:dig:on(0x74)",result,0);
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,FixVoltage,360,SystemFrec,V1DC,1,0,0,0); //VA系统电压 120度
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,FixVoltage,360,SystemFrec,V2DC,2,0,0,0); //VB输入值 0度
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,FixVoltage,360,SystemFrec,V3DC,3,0,0,0); //VC系统电压 -120度
        artExec(fd,"out:ana:v(1:3):on",result,0);
        channel_input(fd,0,FixCurrent,OutPhasevalue,SystemFrec,I1DC,1,0,0,0);  //IC输入值 输入相位
        artExec(fd,"out:ana:i(1:1):on",result,0);
        channel_input(fd,0,FixCurrent,OutPhasevalue-120,SystemFrec,I2DC,2,0,0,0);  //IC输入值 输入相位
        artExec(fd,"out:ana:i(1:2):on",result,0);
        channel_input(fd,0,FixCurrent,OutPhasevalue+120,SystemFrec,I3DC,3,0,0,0);  //IC输入值 输入相位
        artExec(fd,"out:ana:i(1:3):on",result,0);
        break;
    }

    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=StepTime;
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);
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
   
double gettime(double x,double y,double z)  //compute logic act time 
  {
    double time;
    x=result[7];
    y=result[8];
    z=result[9];
    time=(double)(((x*65536+y*256+z)*50-50)/1000000);
    return time;
  }
