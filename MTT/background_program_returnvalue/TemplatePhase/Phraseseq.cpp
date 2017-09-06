#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include <QDebug>



extern TEMPRORARYDATA *temprorarydata;
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern int    I1DC,I2DC,I3DC;
extern int    OutputMode;
extern float  LogicResolution,SystemFrec,SystemVolt;
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern  int FreqTriple;
extern  int   seq_count,V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;
extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,VzRelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase;



extern float PENDSTART;
extern float PENDEND;
extern float PSTEP;
extern float STEPTIME;

extern float PEND;
extern float HOLDTIME;

extern float prefaulttime; //故障前
extern float currentphrase;
extern int IACH;
extern int IBCH;
extern int ICCH;
extern int VACH;
extern int VBCH;
extern int VCCH;

extern float FixVoltage;
extern float FixCurrent;
extern float Postfaulttime; //故障后

extern int Firstrun;
extern float Preparetime;


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



void  OutputSequencer1(int fd)
  {


   artExec(fd,"inp:dig:interval(1)",result,0);
   artExec(fd,"out:ana:user:mem?",result,0);
   artExec(fd,"seq:brk",result,0);
   artExec(fd,"inp:buf:clr",result,0);
   artExec(fd,"seq:clr",result,0);
   artExec(fd,"seq:begin",result,0);
   V1RelaPhase=0;
   V2RelaPhase=0;
   V3RelaPhase=0;
   I1RelaPhase=0;
   I2RelaPhase=0;
   I3RelaPhase=0;





   if(Firstrun==1)
  {   out_time=prefaulttime+Preparetime;
      channel_input(fd,0,FixCurrent,0,SystemFrec/FreqTriple,I1DC,1,0,0,0);
      artExec(fd,"out:ana:i(1:1):on",result,0);
      channel_input(fd,0,FixCurrent,-120,SystemFrec/FreqTriple,I2DC,2,0,0,0);
      artExec(fd,"out:ana:i(1:2):on",result,0);
      channel_input(fd,0,FixCurrent,120,SystemFrec/FreqTriple,I3DC,3,0,0,0);
      artExec(fd,"out:ana:i(1:3):on",result,0);

      channel_input(fd,1,FixVoltage,0,SystemFrec/FreqTriple,V1DC,1,0,0,0);
      artExec(fd,"out:ana:v(1:1):on",result,0);
      channel_input(fd,1,FixVoltage,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
      artExec(fd,"out:ana:v(1:2):on",result,0);
      channel_input(fd,1,FixVoltage,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
      artExec(fd,"out:ana:v(1:3):on",result,0);
      Firstrun=0;
  }
  else
 {

     out_time=prefaulttime;
     artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
     channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
     artExec(fd,"out:ana:i(1:1):on",result,0);
     artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
     channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
     artExec(fd,"out:ana:i(1:2):on",result,0);
     artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
     channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
     artExec(fd,"out:ana:i(1:3):on",result,0);

     artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
     channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V1DC,1,0,0,0);
     artExec(fd,"out:ana:v(1:1):on",result,0);
     artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
     channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
     artExec(fd,"out:ana:v(1:2):on",result,0);
     artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
     channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
     artExec(fd,"out:ana:v(1:3):on",result,0);
 }
   artExec(fd,"inp:buf:sam(bin,1)",result,0);
   artExec(fd,"seq:wait(0,1)",result,0);//第一步

   if(IACH==1)
   {
   I1RelaPhase=currentphrase;
   if(I1RelaPhase<0)
       I1RelaPhase+=360;
   artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
   channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
   artExec(fd,"out:ana:i(1:1):on",result,0);

   }

   if(IBCH==1)
   {
   I2RelaPhase=currentphrase;
   if(I2RelaPhase<0)
       I2RelaPhase+=360;
   artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
   channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
   artExec(fd,"out:ana:i(1:2):on",result,0);

   }

   if(ICCH==1)
  { I3RelaPhase=currentphrase;
      if(I3RelaPhase<0)
          I3RelaPhase+=360;
   artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
   channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
   artExec(fd,"out:ana:i(1:3):on",result,0);

  }


  if(VACH==1)
  {
   V1RelaPhase=currentphrase;
   if(V1RelaPhase<0)
       V1RelaPhase+=360;
   artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
   channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V1DC,1,0,0,0);
   artExec(fd,"out:ana:v(1:1):on",result,0);
  }

   if(VBCH==1)
  {
  V2RelaPhase=currentphrase;
  if(V2RelaPhase<0)
      V2RelaPhase+=360;
   artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
   channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
   artExec(fd,"out:ana:v(1:2):on",result,0);

  }


   if(VCCH==1)
 {
     V3RelaPhase=currentphrase;

     if(V3RelaPhase<0)
         V3RelaPhase+=360;
     artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
   channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
   artExec(fd,"out:ana:v(1:3):on",result,0);

 }

   artExec(fd,"inp:buf:sam(bin,2)",result,0);
   out_time=STEPTIME;
   artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);






   out_time=Postfaulttime;
   if(out_time<0.0001&&out_time>-0.0001)
   {
      out_time=-1;
 //   artExec(fd,"out:ana:v(1:1):off",result,0);
  //  artExec(fd,"out:ana:v(1:2):off",result,0);
 //   artExec(fd,"out:ana:v(1:3):off",result,0);
 //   artExec(fd,"out:ana:i(2:1):off",result,0);
  //  artExec(fd,"out:ana:i(2:2):off",result,0);
 //   artExec(fd,"out:ana:i(2:3):off",result,0);
   }



   {
       if(IACH==1)
       {
       I1RelaPhase=-currentphrase;
       if(I1RelaPhase<0)
           I1RelaPhase+=360;
       artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
       channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
       artExec(fd,"out:ana:i(1:1):on",result,0);

       }

       if(IBCH==1)
       {
       I2RelaPhase=-currentphrase;
       if(I2RelaPhase<0)
           I2RelaPhase+=360;
       artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
       channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
       artExec(fd,"out:ana:i(1:2):on",result,0);
       }
       if(ICCH==1)
      { I3RelaPhase=-currentphrase;
          if(I3RelaPhase<0)
              I3RelaPhase+=360;
       artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
       channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
       artExec(fd,"out:ana:i(1:3):on",result,0);
      }


      if(VACH==1)
      {
       V1RelaPhase=-currentphrase;
       if(V1RelaPhase<0)
           V1RelaPhase+=360;

       artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
       channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V1DC,1,0,0,0);
       artExec(fd,"out:ana:v(1:1):on",result,0);
      }

       if(VBCH==1)
      {
       V2RelaPhase=-currentphrase;
       if(V2RelaPhase<0)
           V2RelaPhase+=360;
       artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
       channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
       artExec(fd,"out:ana:v(1:2):on",result,0);

      }

      if(VCCH==1)
     { V3RelaPhase=-currentphrase;
         if(V3RelaPhase<0)
             V3RelaPhase+=360;
       artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
       channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
       artExec(fd,"out:ana:v(1:3):on",result,0);

     }

   }

   artExec(fd,"inp:buf:sam(bin,3)",result,0);
   artExec(fd,"seq:wait(0,2)",result,0);




   out_time=Postfaulttime;
   if(out_time<0.0001&&out_time>-0.0001)
   {
    out_time=-1;
 //   artExec(fd,"out:ana:v(1:1):off",result,0);
 //   artExec(fd,"out:ana:v(1:2):off",result,0);
 //   artExec(fd,"out:ana:v(1:3):off",result,0);
 //   artExec(fd,"out:ana:i(2:1):off",result,0);
 //   artExec(fd,"out:ana:i(2:2):off",result,0);
 //   artExec(fd,"out:ana:i(2:3):off",result,0);
   }

   {
       if(IACH==1)
       {
       I1RelaPhase=-currentphrase;
       if(I1RelaPhase<0)
           I1RelaPhase+=360;
       artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
       channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
       artExec(fd,"out:ana:i(1:1):on",result,0);

       }

       if(IBCH==1)
       {
       I2RelaPhase=-currentphrase;
       if(I2RelaPhase<0)
           I2RelaPhase+=360;
       artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
       channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
       artExec(fd,"out:ana:i(1:2):on",result,0);
       }
       if(ICCH==1)
      { I3RelaPhase=-currentphrase;
          if(I3RelaPhase<0)
              I3RelaPhase+=360;
       artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
       channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
       artExec(fd,"out:ana:i(1:3):on",result,0);
      }


      if(VACH==1)
      {
       V1RelaPhase=-currentphrase;
       if(V1RelaPhase<0)
           V1RelaPhase+=360;

       artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
       channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V1DC,1,0,0,0);
       artExec(fd,"out:ana:v(1:1):on",result,0);
      }

       if(VBCH==1)
      {
       V2RelaPhase=-currentphrase;
       if(V2RelaPhase<0)
           V2RelaPhase+=360;
       artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
       channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
       artExec(fd,"out:ana:v(1:2):on",result,0);

      }

      if(VCCH==1)
     { V3RelaPhase=-currentphrase;
         if(V3RelaPhase<0)
             V3RelaPhase+=360;
       artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
       channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
       artExec(fd,"out:ana:v(1:3):on",result,0);

     }

   }

   artExec(fd,"inp:buf:sam(bin,4)",result,0);
   artExec(fd,"seq:wait(0,1)",result,0);

   artExec(fd,"inp:buf:sam(bin,5)",result,0);
   out_time=-1;
   artExec(fd,"seq:wait(0,1)",result,0);

   artExec(fd,"seq:end",result,0);
   artExec(fd,"seq:exec",result,0);
 }

void  OutputSequencer2(int fd)
  {
    //  artExec(fd,"inp:dig(1):thres(1)",result,0);  //打开放大器
      artExec(fd,"inp:dig:interval(1)",result,0);
      artExec(fd,"out:ana:user:mem?",result,0);
      artExec(fd,"seq:brk",result,0);
      artExec(fd,"inp:buf:clr",result,0);
      artExec(fd,"seq:clr",result,0);
      artExec(fd,"seq:begin",result,0);
      V1RelaPhase=0;
      V2RelaPhase=0;
      V3RelaPhase=0;
      I1RelaPhase=0;
      I2RelaPhase=0;
      I3RelaPhase=0;


      channel_input(fd,0,FixCurrent,0,SystemFrec/FreqTriple,I1DC,1,0,0,0);
      artExec(fd,"out:ana:i(1:1):on",result,0);
      channel_input(fd,0,FixCurrent,-120,SystemFrec/FreqTriple,I2DC,2,0,0,0);
      artExec(fd,"out:ana:i(1:2):on",result,0);
      channel_input(fd,0,FixCurrent,120,SystemFrec/FreqTriple,I3DC,3,0,0,0);
      artExec(fd,"out:ana:i(1:3):on",result,0);

      channel_input(fd,1,FixVoltage,0,SystemFrec/FreqTriple,V1DC,1,0,0,0);
      artExec(fd,"out:ana:v(1:1):on",result,0);
      channel_input(fd,1,FixVoltage,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
      artExec(fd,"out:ana:v(1:2):on",result,0);
      channel_input(fd,1,FixVoltage,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
      artExec(fd,"out:ana:v(1:3):on",result,0);
      artExec(fd,"inp:buf:sam(bin,1)",result,0);


      if(Firstrun==1)
     { out_time=prefaulttime+Preparetime;
       Firstrun=0;
     }
     else
      out_time=prefaulttime;

      artExec(fd,"seq:wait(0,1)",result,0);//第一步


      if(IACH==1)
      {
      I1RelaPhase=PEND;
      if(I1RelaPhase<0)
          I1RelaPhase+=360;
      artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
      channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
      artExec(fd,"out:ana:i(1:1):on",result,0);

      }

      if(IBCH==1)
      {
      I2RelaPhase=PEND;
      if(I2RelaPhase<0)
          I2RelaPhase+=360;
      artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
      channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
      artExec(fd,"out:ana:i(1:2):on",result,0);

      }

      if(ICCH==1)
     { I3RelaPhase=PEND;
         if(I3RelaPhase<0)
             I3RelaPhase+=360;
      artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
      channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
      artExec(fd,"out:ana:i(1:3):on",result,0);

     }


     if(VACH==1)
     {
      V1RelaPhase=PEND;
      if(V1RelaPhase<0)
          V1RelaPhase+=360;
      artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
      channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V1DC,1,0,0,0);
      artExec(fd,"out:ana:v(1:1):on",result,0);
     }

      if(VBCH==1)
     {
      V2RelaPhase=PEND;
      if(V2RelaPhase<0)
          V2RelaPhase+=360;
      artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
      channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
      artExec(fd,"out:ana:v(1:2):on",result,0);

     }


      if(VCCH==1)
    { V3RelaPhase=PEND;
        if(V3RelaPhase<0)
          V3RelaPhase+=360;
      artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
      channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
      artExec(fd,"out:ana:v(1:3):on",result,0);

    }
      artExec(fd,"inp:buf:sam(bin,2)",result,0);
      out_time=HOLDTIME;
      artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);

      out_time=Postfaulttime;
      if(out_time<0.0001&&out_time>-0.0001)
      {
       out_time=-1;
   //    artExec(fd,"out:ana:v(1:1):off",result,0);
  //     artExec(fd,"out:ana:v(1:2):off",result,0);
   //    artExec(fd,"out:ana:v(1:3):off",result,0);
   //    artExec(fd,"out:ana:i(2:1):off",result,0);
    //   artExec(fd,"out:ana:i(2:2):off",result,0);
    //   artExec(fd,"out:ana:i(2:3):off",result,0);
      }


      {
          if(IACH==1)
          {
          I1RelaPhase=-currentphrase;
          if(I1RelaPhase<0)
              I1RelaPhase+=360;
          artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
          channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
          artExec(fd,"out:ana:i(1:1):on",result,0);

          }

          if(IBCH==1)
          {
          I2RelaPhase=-currentphrase;
          if(I2RelaPhase<0)
              I2RelaPhase+=360;
          artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
          channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
          artExec(fd,"out:ana:i(1:2):on",result,0);
          }
          if(ICCH==1)
         { I3RelaPhase=-currentphrase;
             if(I3RelaPhase<0)
                 I3RelaPhase+=360;
          artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
          channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
          artExec(fd,"out:ana:i(1:3):on",result,0);
         }


         if(VACH==1)
         {
          V1RelaPhase=-currentphrase;
          if(V1RelaPhase<0)
              V1RelaPhase+=360;

          artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
          channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V1DC,1,0,0,0);
          artExec(fd,"out:ana:v(1:1):on",result,0);
         }

          if(VBCH==1)
         {
          V2RelaPhase=-currentphrase;
          if(V2RelaPhase<0)
              V2RelaPhase+=360;
          artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
          channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
          artExec(fd,"out:ana:v(1:2):on",result,0);

         }

         if(VCCH==1)
        { V3RelaPhase=-currentphrase;
            if(V3RelaPhase<0)
                V3RelaPhase+=360;
          artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
          channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
          artExec(fd,"out:ana:v(1:3):on",result,0);

        }

      }


      artExec(fd,"inp:buf:sam(bin,3)",result,0);
      artExec(fd,"seq:wait(0,2)",result,0);



      out_time=Postfaulttime;
      if(out_time<0.0001&&out_time>-0.0001)
      {
       out_time=-1;
      // artExec(fd,"out:ana:v(1:1):off",result,0);
    //   artExec(fd,"out:ana:v(1:2):off",result,0);
     //  artExec(fd,"out:ana:v(1:3):off",result,0);
     //  artExec(fd,"out:ana:i(2:1):off",result,0);
    //   artExec(fd,"out:ana:i(2:2):off",result,0);
    //   artExec(fd,"out:ana:i(2:3):off",result,0);
      }



      {
          if(IACH==1)
          {
          I1RelaPhase=-currentphrase;
          if(I1RelaPhase<0)
              I1RelaPhase+=360;
          artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
          channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
          artExec(fd,"out:ana:i(1:1):on",result,0);

          }

          if(IBCH==1)
          {
          I2RelaPhase=-currentphrase;
          if(I2RelaPhase<0)
              I2RelaPhase+=360;
          artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
          channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
          artExec(fd,"out:ana:i(1:2):on",result,0);
          }
          if(ICCH==1)
         { I3RelaPhase=-currentphrase;
             if(I3RelaPhase<0)
                 I3RelaPhase+=360;
          artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
          channel_input(fd,0,FixCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
          artExec(fd,"out:ana:i(1:3):on",result,0);
         }


         if(VACH==1)
         {
          V1RelaPhase=-currentphrase;
          if(V1RelaPhase<0)
              V1RelaPhase+=360;

          artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
          channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V1DC,1,0,0,0);
          artExec(fd,"out:ana:v(1:1):on",result,0);
         }

          if(VBCH==1)
         {
          V2RelaPhase=-currentphrase;
          if(V2RelaPhase<0)
              V2RelaPhase+=360;
          artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
          channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
          artExec(fd,"out:ana:v(1:2):on",result,0);

         }

         if(VCCH==1)
        { V3RelaPhase=-currentphrase;
            if(V3RelaPhase<0)
                V3RelaPhase+=360;
          artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
          channel_input(fd,1,FixVoltage,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
          artExec(fd,"out:ana:v(1:3):on",result,0);

        }

      }

      artExec(fd,"inp:buf:sam(bin,4)",result,0);
      artExec(fd,"seq:wait(0,1)",result,0);

      artExec(fd,"inp:buf:sam(bin,5)",result,0);
      out_time=-1;
      artExec(fd,"seq:wait(0,1)",result,0);

      artExec(fd,"seq:end",result,0);
      artExec(fd,"seq:exec",result,0);
  }


