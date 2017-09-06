
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include "Curseq.h"



extern TEMPRORARYDATA *temprorarydata;
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC;

extern float V1Value,V2Value,V3Value,V1Phase,V2Phase,V3Phase,I1Value,I2Value,I3Value;
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern int   FreqTriple;




extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;


extern float V1Value,V2Value,V3Value,V1Phase,V2Phase,V3Phase,I1Value,I2Value,I3Value;

extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;

extern float Ferequencer,CurrentValue,VoltageValue,I1Phase,I2Phase,I3Phase,PostTime,WaitTime;
extern float V1Value,V2Value,V3Value,V1Phase,V2Phase,V3Phase,I1Value,I2Value,I3Value;

extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern int TestObject;
extern float postfaulttime;
extern float SystemFrec,SystemVolt;

extern int Faulttype;

extern float CurrentStart;



void output_signal_end(int fd)
  {
    int i,j;

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


artExec(fd,"out:dig:on(0x04)",result,0);
    if(TestObject==1)
    {

      Ferequencer=SystemFrec;
      V1Value=0;
      V2Value=0;
      V3Value=0;
      V1Phase=0;
      V2Phase=0;
      V3Phase=0;
      switch(Faulttype)
      {
      case 1:
          I1Output=1;
          I2Output=0;
          I3Output=0;
          I1Value=CurrentValue;
          I2Value=0;
          I3Value=0;
          I1Phase=0;
          I2Phase=0;
          I3Phase=0;

          artExec(fd,"out:dig:on(0x14)",result,0);
          break;

      case 2:
          I1Output=0;
          I2Output=1;
          I3Output=0;
          I1Value=0;
          I2Value=CurrentValue;
          I3Value=0;
          I1Phase=0;
          I2Phase=0;
          I3Phase=0;
          artExec(fd,"out:dig:on(0x24)",result,0);
          break;

      case 3:
          I1Output=0;
          I2Output=0;
          I3Output=1;
          I1Value=0;
          I2Value=0;
          I3Value=CurrentValue;
          I1Phase=0;
          I2Phase=0;
          I3Phase=0;

          artExec(fd,"out:dig:on(0x44)",result,0);

          break;


     case 4:
          I1Output=1;
          I2Output=1;
          I3Output=0;
          I1Value=CurrentValue/2;
          I2Value=CurrentValue/2;
          I3Value=0;
          I1Phase=0;
          I2Phase=0;
          I3Phase=0;
          artExec(fd,"out:dig:on(0x34)",result,0);
          break;

     case 5:
          I1Output=0;
          I2Output=1;
          I3Output=1;
          I1Value=0;
          I2Value=CurrentValue/2;
          I3Value=CurrentValue/2;
          I1Phase=0;
          I2Phase=0;
          I3Phase=0;
          artExec(fd,"out:dig:on(0x64)",result,0);
          break;


      case 6:
           I1Output=1;
           I2Output=0;
           I3Output=1;
           I1Value=CurrentValue/2;
           I2Value=0;
           I3Value=CurrentValue/2;
           I1Phase=0;
           I2Phase=0;
           I3Phase=0;
           artExec(fd,"out:dig:on(0x54)",result,0);
           break;
      case 7:
          I1Output=1;
          I2Output=1;
          I3Output=1;
          I1Value=CurrentValue/3;
          I2Value=CurrentValue/3;
          I3Value=CurrentValue/3;
          I1Phase=0;
          I2Phase=0;
          I3Phase=0;
          artExec(fd,"out:dig:on(0x74)",result,0);
          break;
      }


    }
    else if(TestObject==2)
     {

         Ferequencer=SystemFrec;
         I1Value=0;
         I2Value=0;
         I3Value=0;
         I1Phase=0;
         I2Phase=0;
         I3Phase=0;
         V1Output=1;
         V2Output=1;
         V3Output=1;
         switch(Faulttype)
         {
         case 1:
             V1Value=VoltageValue;
             V2Value=0;
             V3Value=0;
             V1Phase=0;
             V2Phase=-120;
             V3Phase=120;
             break;

         case 2:
             V1Value=0;
             V2Value=VoltageValue;
             V3Value=0;
             V1Phase=120;
             V2Phase=0;
             V3Phase=-120;
             break;

         case 3:
             V1Value=0;
             V2Value=0;
             V3Value=VoltageValue;
             V1Phase=-120;
             V2Phase=120;
             V3Phase=0;
             break;

         case 4:
             V1Value=VoltageValue/2;
             V2Value=VoltageValue/2;
             V3Value=0;
             V1Phase=0;
             V2Phase=180;
             V3Phase=0;
             break;

        case 5:
             V1Value=0;
             V2Value=VoltageValue/2;
             V3Value=VoltageValue/2;
             V1Phase=0;
             V2Phase=0;
             V3Phase=180;
             break;
         case 6:
             V1Value=VoltageValue/2;
             V2Value=0;
             V3Value=VoltageValue/2;
             V1Phase=0;
             V2Phase=0;
             V3Phase=180;
             break;


         case 7:
             V1Value=VoltageValue;
             V2Value=VoltageValue;
             V3Value=VoltageValue;
             V1Phase=0;
             V2Phase=-120;
             V3Phase=120;
             break;
         }
     }

    else if(TestObject==3)
    {
  //  artExec(fd,"out:dig:on(0x10)",result,0);
    I1Value=0;
    I2Value=0;
    I3Value=0;
    I1Phase=0;
    I2Phase=0;
    I3Phase=0;
    V1Output=1;
    V2Output=1;
    V3Output=1;
    V1Value=SystemVolt;
    V2Value=SystemVolt;
    V3Value=SystemVolt;
    V1Phase=0;
    V2Phase=-120;
    V3Phase=120;
    }

    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    /*
    switch(TestObject)
    {
    case 1:
        break;
    case 2:
    case 3:
        channel_input(fd,1,SystemVolt,V1Phase,SystemFrec/FreqTriple,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        channel_input(fd,1,SystemVolt,V2Phase,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        channel_input(fd,1,SystemVolt,V3Phase,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        break;
    }


*/

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=WaitTime;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步


    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,V1Value,V1Phase,Ferequencer/FreqTriple,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,V2Value,V2Phase,Ferequencer/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,V3Value,V3Phase,Ferequencer/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);


    channel_input(fd,0,I1Value,I1Phase,Ferequencer/FreqTriple,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,I2Value,I2Phase,Ferequencer/FreqTriple,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,I3Value,I3Phase,Ferequencer/FreqTriple,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);


    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=PostTime;
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);  //第二步

    /*
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
    {
        out_time=-1;
        artExec(fd,"out:ana:v(1:1):off",result,0);
        artExec(fd,"out:ana:v(1:2):off",result,0);
        artExec(fd,"out:ana:v(1:3):off",result,0);
        artExec(fd,"out:ana:i(2:1):off",result,0);
        artExec(fd,"out:ana:i(2:2):off",result,0);
        artExec(fd,"out:ana:i(2:3):off",result,0);
    }
    else
    {
      if(TestObject!=1)
      {
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
      }

        artExec(fd,"out:ana:i(2:1):off",result,0);
        artExec(fd,"out:ana:i(2:2):off",result,0);
        artExec(fd,"out:ana:i(2:3):off",result,0);
    }


    */
     out_time=-1;
     artExec(fd,"out:ana:v(1:1):off",result,0);
     artExec(fd,"out:ana:v(1:2):off",result,0);
     artExec(fd,"out:ana:v(1:3):off",result,0);
     artExec(fd,"out:ana:v(1:4):off",result,0);
     artExec(fd,"out:ana:i(2:1):off",result,0);
     artExec(fd,"out:ana:i(2:2):off",result,0);
     artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);

/*
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
    {
        out_time=-1;
        artExec(fd,"out:ana:v(1:1):off",result,0);
        artExec(fd,"out:ana:v(1:2):off",result,0);
        artExec(fd,"out:ana:v(1:3):off",result,0);
        artExec(fd,"out:ana:i(2:1):off",result,0);
        artExec(fd,"out:ana:i(2:2):off",result,0);
        artExec(fd,"out:ana:i(2:3):off",result,0);
    }
    else
    {

        if(TestObject!=1)
       { artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
       }



        artExec(fd,"out:ana:i(2:1):off",result,0);
        artExec(fd,"out:ana:i(2:2):off",result,0);
        artExec(fd,"out:ana:i(2:3):off",result,0);
    }



    */
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    out_time=-1;
    artExec(fd,"inp:buf:sam(bin,4)",result,0);
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
