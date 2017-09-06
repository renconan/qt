
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include "Curseq.h"
#include "QDebug"
#include "Variableformat.h"

extern TEMPRORARYDATA *temprorarydata;
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern int starttest;
extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;
extern int  FreqTriple;
extern int   TestItem1,TestItem2,TestItem3;
extern float FixedFrepuency,df,dt;

extern float Phase,SystemFrec;
extern float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
extern float TripTime1,TripTime2,TripTime4,TripFrepuency;
extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,VzRelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase;




extern int   dfdtcounter; //dfdt�仯�Ĵ��� �µ�dfdt=dfdtstart-dfdtstep*dfdtcounter;
extern float currentdfdt;//��ǰ��dfdtֵ
extern float currentvoltage; //��ǰ�ĵ�ѹ����ֵ

extern float SystemVolt;

extern FRECURRENCESET FreqVar;
extern float prefaulttime;  //����ǰʱ��
extern float postfaulttime; //���Ϻ�ʱ��

 extern float CurrentFreq;

extern float PrepareTime;
extern int Firstrun;     //��һ������


void  OutputSequencer1(int fd)
  {

    change_timedata=FreqVar.TripValue.HoldTime.value;
    //ÿ��ʱ��
   // artExec(fd,"inp:dig(1):thres(1)",result,0);//�򿪷Ŵ���
    artExec(fd,"inp:dig:interval(0)",result,0);//�Ϳ������ֱ���
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);


   if(Firstrun==1)
   {   out_time=prefaulttime+PrepareTime;
       channel_input(fd,1,SystemVolt,0,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
       artExec(fd,"out:ana:v(1:1):on",result,0);
       channel_input(fd,1,SystemVolt,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
       artExec(fd,"out:ana:v(1:2):on",result,0);
       channel_input(fd,1,SystemVolt,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
       artExec(fd,"out:ana:v(1:3):on",result,0);
       Firstrun=0;
   }
   else
    {
        out_time=prefaulttime;
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        Firstrun=0;
    }
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);


    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,CurrentFreq/FreqTriple ,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,CurrentFreq/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,CurrentFreq/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    out_time=FreqVar.TripValue.HoldTime.value;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);

    change_timedata=FreqVar.TripValue.HoldTime.value;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
        // artExec(fd,"out:ana:v(1:1):off",result,0);
       //  artExec(fd,"out:ana:v(1:2):off",result,0);
      //   artExec(fd,"out:ana:v(1:3):off",result,0);
     }
    else
    {
        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
    }


    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);
    //��3�����ӵ�û�ж�������ֹƵ�ʳ���3s

    change_timedata=0.01;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
     //    artExec(fd,"out:ana:v(1:1):off",result,0);
      //   artExec(fd,"out:ana:v(1:2):off",result,0);
     //    artExec(fd,"out:ana:v(1:3):off",result,0);
     }
    else
    {

        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);

    //��4�����ӵ��ж������ض��ź�
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
}

void  OutputSequencer2(int fd)
  {

    float Endfrequency=FreqVar.TripTime.FreEndValue.value;
    float Holdtime=FreqVar.TripTime.HoldTime.value;

    //��ʱ��֤ʱ����0.1s��0.02Hz��
   // artExec(fd,"inp:dig(1):thres(1)",result,0);//�򿪷Ŵ���
    artExec(fd,"inp:dig:interval(0)",result,0);//�Ϳ������ֱ���
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    channel_input(fd,1,SystemVolt,0,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    if(Firstrun==1)
   { out_time=prefaulttime+PrepareTime;
     Firstrun=0;
   }
    else
     out_time=prefaulttime;

    artExec(fd,"seq:wait(0,1)",result,0);



    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,Endfrequency/FreqTriple,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,Endfrequency/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,Endfrequency/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    out_time=Holdtime;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);



    change_timedata=0.01;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
     }
    else
    {

        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);



    change_timedata=0.01;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
     }
    else
    {

        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }


    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);


    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
}


void  OutputSequencer3(int fd)  //DFDT
  {
    float ChangeStep;
    float dt=0.01;
    dt=0.01;
    ChangeStep=currentdfdt*dt;
    if(FreqVar.Tripdfdt.FreEndValue.value<SystemFrec) //���Ƶ�����������ӵ�  ����Ϊ��ֵ
      ChangeStep=-ChangeStep;
    change_timedata=dt;

    int sumstep;
    float StartFrepuency=SystemFrec;
    float EndFrepuency=FreqVar.Tripdfdt.FreEndValue.value;

    artExec(fd,"inp:dig:interval(0)",result,0);//�Ϳ������ֱ���
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    if(Firstrun==1)
   { out_time=prefaulttime+PrepareTime;
     channel_input(fd,1,SystemVolt,0,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
     artExec(fd,"out:ana:v(1:1):on",result,0);
     channel_input(fd,1,SystemVolt,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
     artExec(fd,"out:ana:v(1:2):on",result,0);
     channel_input(fd,1,SystemVolt,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
     artExec(fd,"out:ana:v(1:3):on",result,0);
     Firstrun=0;
   }
    else
    {
    out_time=prefaulttime;
    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    Firstrun=0;
    }
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,StartFrepuency/FreqTriple,V1DC,1,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,StartFrepuency/FreqTriple,V2DC,2,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,StartFrepuency/FreqTriple,V3DC,3,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    sumstep=floor(fabs((StartFrepuency-EndFrepuency)/ChangeStep))+1;
    out_time=sumstep*change_timedata;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);


    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,EndFrepuency/FreqTriple,V1DC,1,0,0,-ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,EndFrepuency/FreqTriple,V2DC,2,0,0,-ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,EndFrepuency/FreqTriple,V3DC,3,0,0,-ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    sumstep=floor(fabs((StartFrepuency-EndFrepuency)/ChangeStep))+1;
    out_time=sumstep*change_timedata;

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);

    change_timedata=0.01;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
     //    artExec(fd,"out:ana:v(1:1):off",result,0);
     //    artExec(fd,"out:ana:v(1:2):off",result,0);
     //    artExec(fd,"out:ana:v(1:3):off",result,0);
     }
    else
    {

        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);
    //��4�����ӵ��ж������ض��ź�
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
}

void  OutputSequencer4(int fd)
{
    float StartFrepuency;
    float EndFrepuency;
    float dt;
    float ChangeStep;
    int sumstep;



    StartFrepuency=SystemFrec;

    dt=0.01;
    ChangeStep=FreqVar.Tripdfdttime.dfdt.value*dt;

    if(FreqVar.Tripdfdttime.stopfreq.value<SystemFrec)
        ChangeStep=-ChangeStep; //Ƶ�����½���
    change_timedata=dt;


    artExec(fd,"inp:dig:interval(0)",result,0);//�Ϳ������ֱ���
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    channel_input(fd,1,SystemVolt,0,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    if(Firstrun==1)
   { out_time=prefaulttime+PrepareTime;
     Firstrun=0;
   }
    else
     out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);


    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,StartFrepuency/FreqTriple ,V1DC,1,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,StartFrepuency/FreqTriple,V2DC,2,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,StartFrepuency/FreqTriple,V3DC,3,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);

    out_time=FreqVar.Tripdfdttime.Duration.value;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);

    sumstep=floor(fabs(out_time/change_timedata))+1;
    EndFrepuency=StartFrepuency+sumstep*ChangeStep;



    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,EndFrepuency/FreqTriple,V1DC,1,0,0,-ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,EndFrepuency/FreqTriple,V2DC,2,0,0,-ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,EndFrepuency/FreqTriple,V3DC,3,0,0,-ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    sumstep=floor(fabs((StartFrepuency-EndFrepuency)/ChangeStep))+1;
    out_time=sumstep*change_timedata;

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);


    change_timedata=0.01;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
     }
    else
    {

        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }


    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);
    //��4�����ӵ��ж������ض��ź�
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);



}


void  OutputSequencer5(int fd)
{

    float ChangeStep;
    int sumstep;
    float dt=0.01;
    dt=0.01;
    change_timedata=dt;
    ChangeStep=FreqVar.TripVBlock.Dfdt.value*dt; //Ƶ�ʵĲ���

    float StartFrepuency=SystemFrec;
    float EndFrepuency=FreqVar.TripVBlock.FreEndValue.value;
    if(EndFrepuency<StartFrepuency)
        ChangeStep=-ChangeStep;
    float HoldTime=FreqVar.TripVBlock.HoldTime.value;
    float Voltageoutput=currentvoltage;
    if(FreqVar.TripVBlock.VSelect==2)
        Voltageoutput=Voltageoutput/1.732;

    artExec(fd,"inp:dig:interval(0)",result,0);//�Ϳ������ֱ���
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    if(Firstrun==1)
    {   out_time=prefaulttime+PrepareTime;
        channel_input(fd,1,SystemVolt,0,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        channel_input(fd,1,SystemVolt,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        channel_input(fd,1,SystemVolt,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        Firstrun=0;
    }
    else
     {
         out_time=prefaulttime;
         artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
         channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
         artExec(fd,"out:ana:v(1:1):on",result,0);
         artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
         channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
         artExec(fd,"out:ana:v(1:2):on",result,0);
         artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
         channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
         artExec(fd,"out:ana:v(1:3):on",result,0);
         Firstrun=0;
     }
     artExec(fd,"inp:buf:sam(bin,1)",result,0);
     artExec(fd,"seq:wait(0,1)",result,0);




    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,Voltageoutput,360,StartFrepuency/FreqTriple ,V1DC,1,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,Voltageoutput,360,StartFrepuency/FreqTriple,V2DC,2,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,Voltageoutput,360,StartFrepuency/FreqTriple,V3DC,3,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    sumstep=floor(fabs((StartFrepuency-EndFrepuency)/ChangeStep))+1;
    out_time=sumstep*change_timedata;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);

    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,Voltageoutput,360,EndFrepuency/FreqTriple ,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);

    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,Voltageoutput,360,EndFrepuency/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);

    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,Voltageoutput,360,EndFrepuency/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    out_time=HoldTime;
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),1,0,0,2)",result,0);


    change_timedata=0.01;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
     //    artExec(fd,"out:ana:v(1:1):off",result,0);
      //   artExec(fd,"out:ana:v(1:2):off",result,0);
      //   artExec(fd,"out:ana:v(1:3):off",result,0);
     }
    else
    {

        artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V2DC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
        channel_input(fd,1,SystemVolt,360,SystemFrec/FreqTriple,V3DC,3,0,0,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);




    change_timedata=0.01;
    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,Voltageoutput,360,EndFrepuency/FreqTriple ,V1DC,1,0,0,-ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,Voltageoutput,360,EndFrepuency/FreqTriple,V2DC,2,0,0,-ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,Voltageoutput,360,EndFrepuency/FreqTriple,V3DC,3,0,0,-ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    sumstep=floor(fabs((StartFrepuency-EndFrepuency)/ChangeStep))+1;
    out_time=sumstep*change_timedata;

    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"inp:buf:sam(bin,6)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);

}




void output_signal_end(int fd)
  {
    int i,j;
    unsigned char step;
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    artExec(fd,"inp:dig(1):thres(0)",result,0);//�رշŴ��������Ʒ���ת��
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(1,1)",result,0);
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
    artExec(fd,"out:dig:off(0xf4)",result,0);
    artExec(fd,"out:dig:off(0x08)",result,0);
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

