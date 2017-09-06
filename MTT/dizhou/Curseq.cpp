
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


extern TEMPRORARYDATA *temprorarydata;
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern int starttest;
extern int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC;
extern int  FreqTriple;
extern int   TestItem1,TestItem2,TestItem3;
extern float FixedFrepuency,df,dt;

extern float Phase,SystemFrec;
extern float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
extern float TripTime1,TripTime2,TripTime4,TripFrepuency;
extern float SystemVolt;

extern int starttest;
extern int seq_count;

extern float faultduration;
extern float prefaulttime;
//动作值
extern float StartFrepuency,EndFrepuency,Freqstep,FreqStepTime;
//动作时间
extern int PickupType;
extern float TriptimeEndfrec; //触发时间的停止频率
extern float TriptimeHoldtime;
//dfdt闭锁值
extern float DfdtEndfrec;
extern float DfdtStart;
extern float DfdtEnd;
extern float DfdtStep;
//dfdt 触发时间
extern int   DfdtTriptimeType; //1 测试结果 2 用户输入
extern float DfdtOFTriptime;
extern float DfdtTriptimefrec; //dfdt触发时间的停止频率
extern float Dfdtholdtime;
extern float DfdtTimefreq;      //dfdt  的计时频率
//电压闭锁值
extern float Dfdt_VoltageBlock;
extern float VoltageBlockEndFrec; //电压闭锁值的停止频率
extern float VoltageBlockStart;
extern float VoltageBlockEnd;
extern float VoltageBlockStep;
extern float VoltageBlockHoldTime;
extern int VPNVPP;


extern float TimeFrequencer;
extern float TripFrepuency; //触发频率
extern float currentdfdt;
extern float dfdttripvalue; //dfdt的触发值
extern float currentvoltage; //当前的电压闭锁值

extern float postfaulttime;

extern float LoadCurrent;
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern float timeTime; //2016-05-20
void  OutputSequencer1(int fd)
  {

    float ChangeStep;
    if(StartFrepuency>EndFrepuency)
    ChangeStep=-Freqstep;
    else ChangeStep=Freqstep;
    change_timedata=FreqStepTime;
    int sumstep;

    if(LoadCurrent>0.0001)//2016-5-19电流设置为0，报过载处理
    {
        artExec(fd,"out:dig:on(0x74)",result,0);
        I1Output=1;
        I2Output=1;
        I3Output=1;
    }
    else
    {
        I1Output=0;
        I2Output=0;
        I3Output=0;
        artExec(fd,"out:dig:off(0x74)",result,0);
    }

    //每步时间
   // artExec(fd,"inp:dig(1):thres(1)",result,0);//打开放大器
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=0;
    artExec(fd,"seq:wait(0,1)",result,0);


    channel_input(fd,1,SystemVolt,0,SystemFrec/FreqTriple,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    channel_input(fd,0,LoadCurrent,75,SystemFrec/FreqTriple,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,LoadCurrent,-45,SystemFrec/FreqTriple,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,LoadCurrent,-165,SystemFrec/FreqTriple,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
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

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I1DC,1,0,0,ChangeStep/FreqTriple);
    artExec(fd,"out:ana:i(1:1):on",result,0);

    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I2DC,2,0,0,ChangeStep/FreqTriple);
    artExec(fd,"out:ana:i(1:2):on",result,0);

    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I3DC,3,0,0,ChangeStep/FreqTriple);
    artExec(fd,"out:ana:i(1:3):on",result,0);


    sumstep=floor(fabs((StartFrepuency-EndFrepuency)/ChangeStep))+1;
    out_time=sumstep*change_timedata;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);




    change_timedata=1;
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
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);

        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);




    change_timedata=1;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:v(1:4):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);

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

        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);

        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }


    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);
    //第4步，接点有动作，关断信号


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

void  OutputSequencer2(int fd)
  {

    float Endfrequency;
    if(PickupType==1)
         Endfrequency=TimeFrequencer;
    else Endfrequency=TriptimeEndfrec;

    float Holdtime=TriptimeHoldtime;

    if(LoadCurrent>0.0001)//2016-5-19电流设置为0，报过载处理
    {
        artExec(fd,"out:dig:on(0x74)",result,0);
        I1Output=1;
        I2Output=1;
        I3Output=1;
    }
    else
    {
        I1Output=0;
        I2Output=0;
        I3Output=0;
        artExec(fd,"out:dig:off(0x74)",result,0);
    }

   // artExec(fd,"inp:dig(1):thres(1)",result,0);//打开放大器
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=0;
    artExec(fd,"seq:wait(0,1)",result,0);

    channel_input(fd,1,SystemVolt,0,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);


    channel_input(fd,0,LoadCurrent,75,SystemFrec/FreqTriple,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,LoadCurrent,-45,SystemFrec/FreqTriple,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,LoadCurrent,-165,SystemFrec/FreqTriple,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
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
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,Endfrequency/FreqTriple,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,Endfrequency/FreqTriple,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,Endfrequency/FreqTriple,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    out_time=Holdtime;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);





    change_timedata=1;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:v(1:4):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);

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
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);

        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }


    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);


    change_timedata=1;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:v(1:4):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);

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
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);

        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }

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


void  OutputSequencer3(int fd)  //DFDT
  {
    double ChangeStep;
    float dt;
    dt=0.01;
    ChangeStep=currentdfdt*dt;
    if(DfdtEndfrec<SystemFrec) //如果频率是往上增加的  步长为正值
      ChangeStep=-ChangeStep;
    change_timedata=dt;



    int sumstep;
    float StartFrepuency=SystemFrec;
    float EndFrepuency=DfdtEndfrec;


    qDebug()<<__func__<<__LINE__<<ChangeStep<<change_timedata<<currentdfdt;

    if(LoadCurrent>0.0001)//2016-5-19电流设置为0，报过载处理
    {
        artExec(fd,"out:dig:on(0x74)",result,0);
        I1Output=1;
        I2Output=1;
        I3Output=1;
    }
    else
    {
        I1Output=0;
        I2Output=0;
        I3Output=0;
        artExec(fd,"out:dig:off(0x74)",result,0);
    }


   // artExec(fd,"inp:dig(1):thres(1)",result,0);//打开放大器
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
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
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    channel_input(fd,0,LoadCurrent,75,SystemFrec/FreqTriple,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,LoadCurrent,-45,SystemFrec/FreqTriple,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,LoadCurrent,-165,SystemFrec/FreqTriple,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
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
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);




    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I1DC,1,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);

    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I2DC,2,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);

    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I3DC,3,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    sumstep=floor(fabs((StartFrepuency-EndFrepuency)/ChangeStep))+1;
    out_time=sumstep*change_timedata;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);



    change_timedata=1;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:v(1:4):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);

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
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);


        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);

    change_timedata=1;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:v(1:4):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);

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
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);

        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);



    //第4步，接点有动作，关断信号
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

void  OutputSequencer4(int fd)//动作时间
{
    float StartFrepuency;
    float EndFrepuency;
    float Timefreq=DfdtTimefreq;
    float dt;
    float ChangeStep;

    int sumstep=0;
    int Timesumstep=0;

    StartFrepuency=SystemFrec;
    EndFrepuency=DfdtTriptimefrec;

    if(LoadCurrent>0.0001)//2016-5-19电流设置为0，报过载处理
    {
        artExec(fd,"out:dig:on(0x74)",result,0);
        I1Output=1;
        I2Output=1;
        I3Output=1;
    }
    else
    {
        I1Output=0;
        I2Output=0;
        I3Output=0;
        artExec(fd,"out:dig:off(0x74)",result,0);
    }

    dt=0.01;
    ChangeStep=DfdtOFTriptime*dt;


    if(EndFrepuency<SystemFrec)
        ChangeStep=-ChangeStep; //频率是下降的
    change_timedata=dt;



    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=0;
    artExec(fd,"seq:wait(0,1)",result,0);

    channel_input(fd,1,SystemVolt,0,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    channel_input(fd,0,LoadCurrent,75,SystemFrec/FreqTriple,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,LoadCurrent,-45,SystemFrec/FreqTriple,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,LoadCurrent,-165,SystemFrec/FreqTriple,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
    artExec(fd,"seq:wait(0,1)",result,0);

/*   //2016-5-21故障前无输出修改
        channel_input(fd,1,SystemVolt,0,StartFrepuency/FreqTriple ,V1DC,1,0,0,ChangeStep/FreqTriple);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        channel_input(fd,1,SystemVolt,-120,StartFrepuency/FreqTriple,V2DC,2,0,0,ChangeStep/FreqTriple);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        channel_input(fd,1,SystemVolt,120,StartFrepuency/FreqTriple,V3DC,3,0,0,ChangeStep/FreqTriple);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:v(1:3):on",result,0);

        channel_input(fd,0,LoadCurrent,75,StartFrepuency/FreqTriple,I1DC,1,0,0,ChangeStep/FreqTriple);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        channel_input(fd,0,LoadCurrent,-45,StartFrepuency/FreqTriple,I2DC,2,0,0,ChangeStep/FreqTriple);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        channel_input(fd,0,LoadCurrent,-165,StartFrepuency/FreqTriple,I3DC,3,0,0,ChangeStep/FreqTriple);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);

        artExec(fd,"inp:buf:sam(bin,1)",result,0);
        out_time=prefaulttime;//sumstep*change_timedata;
        artExec(fd,"seq:wait(0,1)",result,0);
 */



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

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);


    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I1DC,1,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);

    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I2DC,2,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);

    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I3DC,3,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    Timesumstep=floor(fabs((StartFrepuency-Timefreq)/ChangeStep))+1;
    timeTime=Timesumstep*change_timedata;
    sumstep=floor(fabs((StartFrepuency-EndFrepuency)/ChangeStep))+1;
    out_time=sumstep*change_timedata;
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,1,0,1)",result,0);


    artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,EndFrepuency/FreqTriple,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,EndFrepuency/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
    channel_input(fd,1,SystemVolt,360,EndFrepuency/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,EndFrepuency/FreqTriple,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,EndFrepuency/FreqTriple,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,EndFrepuency/FreqTriple,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    out_time=Dfdtholdtime;
    artExec(fd,"inp:buf:sam(bin,3)",result,0);//第三步
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),1,1,0,2)",result,0);



    out_time=-1;
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);

    artExec(fd,"inp:buf:sam(bin,4)",result,0);//第四步
    artExec(fd,"seq:wait(0,2)",result,0);
    //第4步，接点有动作，关断信号




    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,5)",result,0);//第五步
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);



    artExec(fd,"out:ana:v(1:1):off",result,0);//2016-5-21
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,6)",result,0);//第六步
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
}


void  OutputSequencer5(int fd)
{
    int sumstep;
    float ChangeStep;
    float dt=0.01;
    dt=0.01;
    change_timedata=dt;

    ChangeStep=Dfdt_VoltageBlock*dt; //频率的步长

    float StartFrepuency=SystemFrec;
    float EndFrepuency=VoltageBlockEndFrec;

    if(EndFrepuency<StartFrepuency)
        ChangeStep=-ChangeStep;
    float HoldTime=VoltageBlockHoldTime;
    float Voltageoutput=currentvoltage;
    if(VPNVPP==2)
        Voltageoutput=Voltageoutput/1.732;


    if(LoadCurrent>0.0001)//2016-5-19电流设置为0，报过载处理
    {
        artExec(fd,"out:dig:on(0x74)",result,0);
        I1Output=1;
        I2Output=1;
        I3Output=1;
    }
    else
    {
        I1Output=0;
        I2Output=0;
        I3Output=0;
        artExec(fd,"out:dig:off(0x74)",result,0);
    }

    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    out_time=0;
    artExec(fd,"seq:wait(0,1)",result,0);

    //第一步
    channel_input(fd,1,SystemVolt,0,SystemFrec/FreqTriple ,V1DC,1,0,0,0);
    artExec(fd,"out:ana:v(1:1):on",result,0);
    channel_input(fd,1,SystemVolt,-120,SystemFrec/FreqTriple,V2DC,2,0,0,0);
    artExec(fd,"out:ana:v(1:2):on",result,0);
    channel_input(fd,1,SystemVolt,120,SystemFrec/FreqTriple,V3DC,3,0,0,0);
    artExec(fd,"out:ana:v(1:3):on",result,0);
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);
    channel_input(fd,0,LoadCurrent,75,SystemFrec/FreqTriple,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    channel_input(fd,0,LoadCurrent,-45,SystemFrec/FreqTriple,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    channel_input(fd,0,LoadCurrent,-165,SystemFrec/FreqTriple,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);


    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=prefaulttime;
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
    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple ,I1DC,1,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);
    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I2DC,2,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);
    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,StartFrepuency/FreqTriple,I3DC,3,0,0,ChangeStep/FreqTriple);
    artExec(fd,"others:steptime(1)",result,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

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

    channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
    artExec(fd,"out:ana:v(1:4):on",result,0);

    artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,EndFrepuency/FreqTriple ,I1DC,1,0,0,0);
    artExec(fd,"out:ana:i(1:1):on",result,0);

    artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,EndFrepuency/FreqTriple,I2DC,2,0,0,0);
    artExec(fd,"out:ana:i(1:2):on",result,0);

    artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
    channel_input(fd,0,LoadCurrent,360,EndFrepuency/FreqTriple,I3DC,3,0,0,0);
    artExec(fd,"out:ana:i(1:3):on",result,0);

    out_time=HoldTime;
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),1,0,0,2)",result,0);



    change_timedata=0.01;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:v(1:4):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);

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
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);

        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }

    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    artExec(fd,"seq:wait(0,2)",result,0);




    change_timedata=0.01;
    out_time=postfaulttime;
    if(out_time<0.0001&&out_time>-0.0001)
     {
         out_time=-1;
         artExec(fd,"out:ana:v(1:1):off",result,0);
         artExec(fd,"out:ana:v(1:2):off",result,0);
         artExec(fd,"out:ana:v(1:3):off",result,0);
         artExec(fd,"out:ana:v(1:4):off",result,0);
         artExec(fd,"out:ana:i(2:1):off",result,0);
         artExec(fd,"out:ana:i(2:2):off",result,0);
         artExec(fd,"out:ana:i(2:3):off",result,0);

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
        channel_input(fd,1,0,0,0,VzDC,4,0,0,0);
        artExec(fd,"out:ana:v(1:4):on",result,0);

        artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I1DC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I2DC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
        channel_input(fd,0,LoadCurrent,360,SystemFrec/FreqTriple,I3DC,3,0,0,0);
        artExec(fd,"out:ana:i(1:3):on",result,0);
        artExec(fd,"others:steptime(1)",result,0);
    }


    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    artExec(fd,"seq:wait(0,1)",result,0);



    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:v(1:4):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,6)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);

    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);

}





void output_signal_end(int fd)
  {
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    artExec(fd,"inp:dig(1):thres(0)",result,0);//关闭放大器，控制风扇转速
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

double gettime(double x,double y,double z)  //compute logic act time
  {
    double time;
    x=result[7];
    y=result[8];
    z=result[9];
    time=(double)(((x*65536+y*256+z)*50-50)/1000000);
    return time;
  }

