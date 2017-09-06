
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include "Volseq.h"



extern TEMPRORARYDATA *temprorarydata;
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int   V1DC,V2DC,V3DC;
extern float SystemFrec,Phase;
extern float SystemVolt;


extern float VENDSTART;
extern float VENDEND;
extern float VSTEP;
extern float STEPTIME;
extern float CurrentVoltage;

extern float VEND;
extern float HOLDTIME;

extern int  Faulttype;
extern int VPNVPP;
extern float prefaulttime;
extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,VzRelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase;


extern float postfaulttime;

extern int     Firstrun;
extern float   Preparetime;



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
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
    artExec(fd,"out:dig:off(0x84)",result,0);
    artExec(fd,"out:dig:off(0x08)",result,0);
  }

void OutputSequencer1(int fd)  //2009-10-08
  {//测试动作值的sequencer

    float StartVA=0;
    float StartVB=0;
    float StartVC=0;
    float PhaseVA=0;
    float PhaseVB=0;
    float PhaseVC=0;

    switch(Faulttype)  //2009-10-08
     {
     case 1:  //VA
       StartVA=CurrentVoltage;
       StartVB=SystemVolt;
       StartVC=SystemVolt;
       PhaseVA=0;
       PhaseVB=-120;
       PhaseVC=120;
       break;
     case 2:  //VB
        StartVB=CurrentVoltage;
        StartVA=SystemVolt;
        StartVC=SystemVolt;
        PhaseVA=120;
        PhaseVB=0;
        PhaseVC=-120;
       break;
     case 3:  //VC
        StartVC=CurrentVoltage;
        StartVA=SystemVolt;
        StartVB=SystemVolt;
        PhaseVA=-120;
        PhaseVB=120;
        PhaseVC=0;
       break;
     case 4:  //VAB
       StartVA=CurrentVoltage/1.732;
       StartVB=CurrentVoltage/1.732;
       StartVC=SystemVolt;
       PhaseVA=-120;
       PhaseVB=120;
       PhaseVC=0;
       break;
     case 5:  //VBC
        StartVB=CurrentVoltage/1.732;
        StartVC=CurrentVoltage/1.732;
        StartVA=SystemVolt;
        PhaseVA=0;
        PhaseVB=-120;
        PhaseVC=120;
       break;
    case 6:  //VCA
        StartVB=SystemVolt;
        StartVC=CurrentVoltage/1.732;
        StartVA=CurrentVoltage/1.732;
        PhaseVA=120;
        PhaseVB=0;
        PhaseVC=-120;
       break;
    case 7:  //VABC
        if(VPNVPP==1)
       {
        StartVA=CurrentVoltage;
        StartVB=CurrentVoltage;
        StartVC=CurrentVoltage;
        }
        else
        {
            StartVA=CurrentVoltage/1.732;
            StartVB=CurrentVoltage/1.732;
            StartVC=CurrentVoltage/1.732;
        }
        PhaseVA=0;
        PhaseVB=-120;
        PhaseVC=120;
        break;
     }



    change_timedata=STEPTIME;
    artExec(fd,"out:dig:on(0x04)",result,0);

    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    if(Firstrun==1)
  {
    channel_input(fd,1,SystemVolt,PhaseVA,SystemFrec,V1DC,1,0,0,0); //2009-6-20
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,PhaseVB,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,PhaseVC,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
  }
    else
    {       artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
            channel_input(fd,1,SystemVolt,360,SystemFrec,V1DC,1,0,0,0); //2009-6-20
            artExec(fd,"out:ana:v(1:1):on",result,0);
            artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
            channel_input(fd,1,SystemVolt,360,SystemFrec,V2DC,2,0,0,0);
            artExec(fd,"out:ana:v(1:2):on",result,0);
            artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
            channel_input(fd,1,SystemVolt,360,SystemFrec,V3DC,3,0,0,0);
            artExec(fd,"out:ana:v(1:3):on",result,0);
    }



    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    if(Firstrun==1)
   { out_time=prefaulttime+Preparetime;
     Firstrun=0;
   }
    else
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步



    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,StartVA,360,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,StartVB,360,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,StartVC,360,SystemFrec,V3DC,3,0,0,0); //2009-10-08
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    out_time=STEPTIME;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);

    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {   out_time=-1;
         /*
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);
         */
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
         /*
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);
         */
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
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
  }

//动作时间的seq
void OutputSequencer3(int fd)  //2009-10-08
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



    switch(Faulttype)  //2009-10-08
     {
     case 1:  //VA
       StartVA=SystemVolt;
       StartVA1=VEND;
       StartVB=SystemVolt;
       StartVB1=SystemVolt;
       StartVC=SystemVolt;
       StartVC1=SystemVolt;
       PhaseVA=0;
       PhaseVB=-120;
       PhaseVC=120;
       break;
     case 2:  //VB
        StartVA=SystemVolt;
        StartVA1=SystemVolt;
        StartVB=SystemVolt;
        StartVB1=VEND;
        StartVC=SystemVolt;
        StartVC1=SystemVolt;
        PhaseVA=120;
        PhaseVB=0;
        PhaseVC=-120;
       break;
     case 3:  //VC
        StartVA=SystemVolt;
        StartVA1=SystemVolt;
        StartVB=SystemVolt;
        StartVB1=SystemVolt;
        StartVC=SystemVolt;
        StartVC1=VEND;
        PhaseVA=-120;
        PhaseVB=120;
        PhaseVC=0;
       break;
     case 4:  //VAB
       StartVA=SystemVolt;
       StartVA1=VEND/1.732;
       StartVB=SystemVolt;
       StartVB1=VEND/1.732;
       StartVC=SystemVolt;
       StartVC1=SystemVolt;

       PhaseVA=-120;
       PhaseVB=120;
       PhaseVC=0;
       break;
     case 5:  //VBC
        StartVA=SystemVolt;
        StartVA1=SystemVolt;

        StartVB=SystemVolt;
        StartVB1=VEND/1.732;
        StartVC=SystemVolt;
        StartVC1=VEND/1.732;
        PhaseVA=0;
        PhaseVB=-120;
        PhaseVC=120;
       break;
    case 6:  //VCA
        StartVA=SystemVolt;
        StartVA1=VEND/1.732;

        StartVB=SystemVolt;
        StartVB1=SystemVolt;

        StartVC=SystemVolt;
        StartVC1=VEND/1.732;
        PhaseVA=120;
        PhaseVB=0;
        PhaseVC=-120;
     case 7:
        if(VPNVPP==1)
      {  StartVA=SystemVolt;
        StartVA1=VEND;
        StartVB=SystemVolt;
        StartVB1=VEND;
        StartVC=SystemVolt;
        StartVC1=VEND;
       }
        else
        {
            StartVA=SystemVolt;
            StartVA1=VEND/1.732;
            StartVB=SystemVolt;
            StartVB1=VEND/1.732;
            StartVC=SystemVolt;
            StartVC1=VEND/1.732;
        }
        PhaseVA=0;
        PhaseVB=-120;
        PhaseVC=120;

       break;
     }

    artExec(fd,"out:dig:on(0x04)",result,0);
 //   artExec(fd,"inp:dig(1):thres(1)",result,0);
    artExec(fd,"inp:dig:interval(0)",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步

    //2007-10-26，之前为PerValue，输出值成2倍额定电压了，改为PerValue/2
    channel_input(fd,1,StartVA,PhaseVA,SystemFrec,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,StartVB,PhaseVB,SystemFrec,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,StartVC,PhaseVC,SystemFrec,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    //测试动作时间时，先输出0
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    if(Firstrun==1)
   { out_time=prefaulttime+Preparetime;
     Firstrun=0;
   }
    else
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
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=HOLDTIME;
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);



    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {   out_time=-1;
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
        {   out_time=-1;
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

