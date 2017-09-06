#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include "Phraseseq.h"


extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC;

extern float Ferequencer,HarmonicCurrentStart,Ferequencer1,I1Phase,I2Phase,I3Phase;
extern float V1Value,V2Value,V3Value,V1Phase,V2Phase,V3Phase,I1Value,I2Value,I3Value;
extern float HarmonicCount,FixHarmonic,Current,Phase;

extern float VaValueReviosn[10],VbValueReviosn[10],VcValueReviosn[10];
extern float IaValueReviosn[10],IbValueReviosn[10],IcValueReviosn[10];
extern float V1OutputValue,V2OutputValue,V3OutputValue;
extern float I1OutputValue,I2OutputValue,I3OutputValue;
extern float faultduration;

extern float prefaulttime;
extern int Firstrun;
extern float Preparetime;
extern int  CurrentOutputSel;
extern float HarmonicStart;
extern float HarmonicEnd;
extern float HarmonicStep;
extern float HarmonicStepTime;
extern double  HarmonicStepError;
extern float STEPTIME;
float  valuefrec(float Frequency,float Value,int type)
{
      float ValueFreqPara;
      if (type==1)//???
      {
          if(Frequency<50)
           {
            ValueFreqPara = 0.9998 + 0.0002 * (1-0.001*(Frequency-50)*(Frequency-50));

           }
           else
            ValueFreqPara = 0.9998 + 0.0002  * (1 + 0.0002 * (Frequency - 50)*(Frequency - 50));

      }
      else{//????
            if(Frequency <50)
                ValueFreqPara = 0.9998 +0.0002*(1 - 0.0005*(Frequency - 50)*(Frequency - 50));
            else
              ValueFreqPara = 0.9998 + 0.0002 *(1 + 0.00012*(Frequency - 50)*(Frequency - 50));
          }
     return ValueFreqPara*Value;
}



void ValueFrec()
  {
      HarmonicCurrentStart=valuefrec(Ferequencer1,HarmonicCurrentStart,2);
  }

void GetReviosnValue(int fd)//???????
  {
    int i,SIGNED=1;
    unsigned char response[3];
    artExec(fd,"others:revision:valueget:v(1:1)",result,38);
    response[0]=result[7];
    response[1]=result[8];
    response[2]=result[9];
    VaValueReviosn[0]=BytesToFloat(response,4,3,SIGNED);//Va??????100Hz????????
    response[0]=result[10];
    response[1]=result[11];
    response[2]=result[12];
    VaValueReviosn[1]=BytesToFloat(response,4,3,SIGNED);//Va??????200Hz????????
    response[0]=result[13];
    response[1]=result[14];
    response[2]=result[15];
    VaValueReviosn[2]=BytesToFloat(response,4,3,SIGNED);//Va??????300Hz????????
    response[0]=result[16];
    response[1]=result[17];
    response[2]=result[18];
    VaValueReviosn[3]=BytesToFloat(response,4,3,SIGNED);//Va??????400Hz????????
    response[0]=result[19];
    response[1]=result[20];
    response[2]=result[21];
    VaValueReviosn[4]=BytesToFloat(response,4,3,SIGNED);//Va??????500Hz????????
    response[0]=result[22];
    response[1]=result[23];
    response[2]=result[24];
    VaValueReviosn[5]=BytesToFloat(response,4,3,SIGNED);//Va??????600Hz????????
    response[0]=result[25];
    response[1]=result[26];
    response[2]=result[27];
    VaValueReviosn[6]=BytesToFloat(response,4,3,SIGNED);//Va??????700Hz????????
    response[0]=result[28];
    response[1]=result[29];
    response[2]=result[30];
    VaValueReviosn[7]=BytesToFloat(response,4,3,SIGNED);//Va??????800Hz????????
    response[0]=result[31];
    response[1]=result[32];
    response[2]=result[33];
    VaValueReviosn[8]=BytesToFloat(response,4,3,SIGNED);//Va??????900Hz????????
    response[0]=result[34];
    response[1]=result[35];
    response[2]=result[36];
    VaValueReviosn[9]=BytesToFloat(response,4,3,SIGNED);//Va??????1000Hz????????

    artExec(fd,"others:revision:valueget:v(1:2)",result,38);
    response[0]=result[7];
    response[1]=result[8];
    response[2]=result[9];
    VbValueReviosn[0]=BytesToFloat(response,4,3,SIGNED);//Vb??????100Hz????????
    response[0]=result[10];
    response[1]=result[11];
    response[2]=result[12];
    VbValueReviosn[1]=BytesToFloat(response,4,3,SIGNED);//Vb??????200Hz????????
    response[0]=result[13];
    response[1]=result[14];
    response[2]=result[15];
    VbValueReviosn[2]=BytesToFloat(response,4,3,SIGNED);//Vb??????300Hz????????
    response[0]=result[16];
    response[1]=result[17];
    response[2]=result[18];
    VbValueReviosn[3]=BytesToFloat(response,4,3,SIGNED);//Vb??????400Hz????????
    response[0]=result[19];
    response[1]=result[20];
    response[2]=result[21];
    VbValueReviosn[4]=BytesToFloat(response,4,3,SIGNED);//Vb??????500Hz????????
    response[0]=result[22];
    response[1]=result[23];
    response[2]=result[24];
    VbValueReviosn[5]=BytesToFloat(response,4,3,SIGNED);//Vb??????600Hz????????
    response[0]=result[25];
    response[1]=result[26];
    response[2]=result[27];
    VbValueReviosn[6]=BytesToFloat(response,4,3,SIGNED);//Vb??????700Hz????????
    response[0]=result[28];
    response[1]=result[29];
    response[2]=result[30];
    VbValueReviosn[7]=BytesToFloat(response,4,3,SIGNED);//Vb??????800Hz????????
    response[0]=result[31];
    response[1]=result[32];
    response[2]=result[33];
    VbValueReviosn[8]=BytesToFloat(response,4,3,SIGNED);//Vb??????900Hz????????
    response[0]=result[34];
    response[1]=result[35];
    response[2]=result[36];
    VbValueReviosn[9]=BytesToFloat(response,4,3,SIGNED);//Vb??????1000Hz????????

    artExec(fd,"others:revision:valueget:v(1:3)",result,38);
    response[0]=result[7];
    response[1]=result[8];
    response[2]=result[9];
    VcValueReviosn[0]=BytesToFloat(response,4,3,SIGNED);//Vc??????100Hz????????
    response[0]=result[10];
    response[1]=result[11];
    response[2]=result[12];
    VcValueReviosn[1]=BytesToFloat(response,4,3,SIGNED);//Vc??????200Hz????????
    response[0]=result[13];
    response[1]=result[14];
    response[2]=result[15];
    VcValueReviosn[2]=BytesToFloat(response,4,3,SIGNED);//Vc??????300Hz????????
    response[0]=result[16];
    response[1]=result[17];
    response[2]=result[18];
    VcValueReviosn[3]=BytesToFloat(response,4,3,SIGNED);//Vc??????400Hz????????
    response[0]=result[19];
    response[1]=result[20];
    response[2]=result[21];
    VcValueReviosn[4]=BytesToFloat(response,4,3,SIGNED);//Vc??????500Hz????????
    response[0]=result[22];
    response[1]=result[23];
    response[2]=result[24];
    VcValueReviosn[5]=BytesToFloat(response,4,3,SIGNED);//Vc??????600Hz????????
    response[0]=result[25];
    response[1]=result[26];
    response[2]=result[27];
    VcValueReviosn[6]=BytesToFloat(response,4,3,SIGNED);//Vc??????700Hz????????
    response[0]=result[28];
    response[1]=result[29];
    response[2]=result[30];
    VcValueReviosn[7]=BytesToFloat(response,4,3,SIGNED);//Vc??????800Hz????????
    response[0]=result[31];
    response[1]=result[32];
    response[2]=result[33];
    VcValueReviosn[8]=BytesToFloat(response,4,3,SIGNED);//Vc??????900Hz????????
    response[0]=result[34];
    response[1]=result[35];
    response[2]=result[36];
    VcValueReviosn[9]=BytesToFloat(response,4,3,SIGNED);//Vc??????1000Hz????????

    artExec(fd,"others:revision:valueget:i(1:1)",result,38);
    response[0]=result[7];
    response[1]=result[8];
    response[2]=result[9];
    IaValueReviosn[0]=BytesToFloat(response,4,3,SIGNED);//Ia??????100Hz????????
    response[0]=result[10];
    response[1]=result[11];
    response[2]=result[12];
    IaValueReviosn[1]=BytesToFloat(response,4,3,SIGNED);//Ia??????200Hz????????
    response[0]=result[13];
    response[1]=result[14];
    response[2]=result[15];
    IaValueReviosn[2]=BytesToFloat(response,4,3,SIGNED);//Ia??????300Hz????????
    response[0]=result[16];
    response[1]=result[17];
    response[2]=result[18];
    IaValueReviosn[3]=BytesToFloat(response,4,3,SIGNED);//Ia??????400Hz????????
    response[0]=result[19];
    response[1]=result[20];
    response[2]=result[21];
    IaValueReviosn[4]=BytesToFloat(response,4,3,SIGNED);//Ia??????500Hz????????
    response[0]=result[22];
    response[1]=result[23];
    response[2]=result[24];
    IaValueReviosn[5]=BytesToFloat(response,4,3,SIGNED);//Ia??????600Hz????????
    response[0]=result[25];
    response[1]=result[26];
    response[2]=result[27];
    IaValueReviosn[6]=BytesToFloat(response,4,3,SIGNED);//Ia??????700Hz????????
    response[0]=result[28];
    response[1]=result[29];
    response[2]=result[30];
    IaValueReviosn[7]=BytesToFloat(response,4,3,SIGNED);//Ia??????800Hz????????
    response[0]=result[31];
    response[1]=result[32];
    response[2]=result[33];
    IaValueReviosn[8]=BytesToFloat(response,4,3,SIGNED);//Ia??????900Hz????????
    response[0]=result[34];
    response[1]=result[35];
    response[2]=result[36];
    IaValueReviosn[9]=BytesToFloat(response,4,3,SIGNED);//Ia??????1000Hz????????

    artExec(fd,"others:revision:valueget:i(1:2)",result,38);
    response[0]=result[7];
    response[1]=result[8];
    response[2]=result[9];
    IbValueReviosn[0]=BytesToFloat(response,4,3,SIGNED);//Ib??????100Hz????????
    response[0]=result[10];
    response[1]=result[11];
    response[2]=result[12];
    IbValueReviosn[1]=BytesToFloat(response,4,3,SIGNED);//Ib??????200Hz????????
    response[0]=result[13];
    response[1]=result[14];
    response[2]=result[15];
    IbValueReviosn[2]=BytesToFloat(response,4,3,SIGNED);//Ib??????300Hz????????
    response[0]=result[16];
    response[1]=result[17];
    response[2]=result[18];
    IbValueReviosn[3]=BytesToFloat(response,4,3,SIGNED);//Ib??????400Hz????????
    response[0]=result[19];
    response[1]=result[20];
    response[2]=result[21];
    IbValueReviosn[4]=BytesToFloat(response,4,3,SIGNED);//Ib??????500Hz????????
    response[0]=result[22];
    response[1]=result[23];
    response[2]=result[24];
    IbValueReviosn[5]=BytesToFloat(response,4,3,SIGNED);//Ib??????600Hz????????
    response[0]=result[25];
    response[1]=result[26];
    response[2]=result[27];
    IbValueReviosn[6]=BytesToFloat(response,4,3,SIGNED);//Ib??????700Hz????????
    response[0]=result[28];
    response[1]=result[29];
    response[2]=result[30];
    IbValueReviosn[7]=BytesToFloat(response,4,3,SIGNED);//Ib??????800Hz????????
    response[0]=result[31];
    response[1]=result[32];
    response[2]=result[33];
    IbValueReviosn[8]=BytesToFloat(response,4,3,SIGNED);//Ib??????900Hz????????
    response[0]=result[34];
    response[1]=result[35];
    response[2]=result[36];
    IbValueReviosn[9]=BytesToFloat(response,4,3,SIGNED);//Ib??????1000Hz????????

    artExec(fd,"others:revision:valueget:i(1:3)",result,38);
    response[0]=result[7];
    response[1]=result[8];
    response[2]=result[9];
    IcValueReviosn[0]=BytesToFloat(response,4,3,SIGNED);//Ic??????100Hz????????
    response[0]=result[10];
    response[1]=result[11];
    response[2]=result[12];
    IcValueReviosn[1]=BytesToFloat(response,4,3,SIGNED);//Ic??????200Hz????????
    response[0]=result[13];
    response[1]=result[14];
    response[2]=result[15];
    IcValueReviosn[2]=BytesToFloat(response,4,3,SIGNED);//Ic??????300Hz????????
    response[0]=result[16];
    response[1]=result[17];
    response[2]=result[18];
    IcValueReviosn[3]=BytesToFloat(response,4,3,SIGNED);//Ic??????400Hz????????
    response[0]=result[19];
    response[1]=result[20];
    response[2]=result[21];
    IcValueReviosn[4]=BytesToFloat(response,4,3,SIGNED);//Ic??????500Hz????????
    response[0]=result[22];
    response[1]=result[23];
    response[2]=result[24];
    IcValueReviosn[5]=BytesToFloat(response,4,3,SIGNED);//Ic??????600Hz????????
    response[0]=result[25];
    response[1]=result[26];
    response[2]=result[27];
    IcValueReviosn[6]=BytesToFloat(response,4,3,SIGNED);//Ic??????700Hz????????
    response[0]=result[28];
    response[1]=result[29];
    response[2]=result[30];
    IcValueReviosn[7]=BytesToFloat(response,4,3,SIGNED);//Ic??????800Hz????????
    response[0]=result[31];
    response[1]=result[32];
    response[2]=result[33];
    IcValueReviosn[8]=BytesToFloat(response,4,3,SIGNED);//Ic??????900Hz????????
    response[0]=result[34];
    response[1]=result[35];
    response[2]=result[36];
    IcValueReviosn[9]=BytesToFloat(response,4,3,SIGNED);//Ic??????1000Hz????????
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
    artExec(fd,"out:dig:off(0x74)",result,0);
    artExec(fd,"out:dig:off(0x08)",result,0);
  }

void OutputSequencer(int fd)
  {
    //????????????sequencer
    int  sumstep=0;
    float HarmonicStepValue;
    HarmonicStepValue=Current*HarmonicStep/100;
    change_timedata=HarmonicStepTime;


   //  artExec(fd,"inp:dig(1):thres(1)",result,0);//???????

    switch(CurrentOutputSel)
    {
    case 1:
        artExec(fd,"out:dig:on(0x14)",result,0);
        break;
    case 2:
        artExec(fd,"out:dig:on(0x24)",result,0);
        break;
    case 3:
        artExec(fd,"out:dig:on(0x44)",result,0);
        break;
    }


    artExec(fd,"inp:dig:interval(0)",result,0);//???????????
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
   if(Firstrun==1)
   {
    out_time=Preparetime;
    Firstrun=0;
   }
   else
   {out_time=-1;
   }
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);//?????


    channel_input1(fd,1,V1Value,V1Phase,Ferequencer,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input1(fd,1,V2Value,V2Phase,Ferequencer,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input1(fd,1,V3Value,V3Phase,Ferequencer,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    switch(CurrentOutputSel)
    {
    case 1:
     channel_input(fd,0,HarmonicCurrentStart,Phase,Ferequencer1,Current,Phase,Ferequencer,I1DC,1,0/*-HarmonicStepValue*/,0,0);
     artExec(fd,"others:steptime(1)",result,0);
     artExec(fd,"out:ana:i(1:1):on",result,0);
     break;
    case 2:
     channel_input(fd,0,HarmonicCurrentStart,Phase,Ferequencer1,Current,Phase,Ferequencer,I2DC,2,0/*-HarmonicStepValue*/,0,0);
     artExec(fd,"others:steptime(1)",result,0);
     artExec(fd,"out:ana:i(1:2):on",result,0);
     break;
    case 3:
     channel_input(fd,0,HarmonicCurrentStart,Phase,Ferequencer1,Current,Phase,Ferequencer,I3DC,3,0/*-HarmonicStepValue*/,0,0);
     artExec(fd,"others:steptime(1)",result,0);
     artExec(fd,"out:ana:i(1:3):on",result,0);
     break;
    }

    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=HarmonicStepTime;

    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);
    //2007-8-3 13:44,?????????

//    artExec(fd,"out:ana:v(1:1):off",result,0);
//    artExec(fd,"out:ana:v(1:2):off",result,0);
//    artExec(fd,"out:ana:v(1:3):off",result,0);
//    artExec(fd,"out:ana:v(1:4):off",result,0);
//    artExec(fd,"out:ana:i(2:1):off",result,0);
//    artExec(fd,"out:ana:i(2:2):off",result,0);
//    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);


//    artExec(fd,"out:ana:v(1:1):off",result,0);
//    artExec(fd,"out:ana:v(1:2):off",result,0);
//    artExec(fd,"out:ana:v(1:3):off",result,0);
//    artExec(fd,"out:ana:v(1:4):off",result,0);
//    artExec(fd,"out:ana:i(2:1):off",result,0);
//    artExec(fd,"out:ana:i(2:2):off",result,0);
//    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);


//    artExec(fd,"out:ana:v(1:1):off",result,0);
//    artExec(fd,"out:ana:v(1:2):off",result,0);
//    artExec(fd,"out:ana:v(1:3):off",result,0);
//    artExec(fd,"out:ana:v(1:4):off",result,0);
//    artExec(fd,"out:ana:i(2:1):off",result,0);
//    artExec(fd,"out:ana:i(2:2):off",result,0);
//    artExec(fd,"out:ana:i(2:3):off",result,0);
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



///************************************¡ý
