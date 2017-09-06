#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <QDebug>
#include<ctype.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"


extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;

extern int   I1DC,I2DC,I3DC,V1DC,V2DC,V3DC;
extern int   RelayType,DCAC;
extern int   TestItem1,TestItem2,TestItem3;
extern float StepTime;
extern float StartCurrent,EndCurrent,CurActStep,CurReturnStep,StepTime;
extern float StartVoltage,EndVoltage,VolActStep,VolReturnStep;
extern float LogicResolution,SystemFrec,Phase,TripValue;

extern float TestCurrent;
extern float CurTesttime;

extern float TestVoltage;
extern float VolTesttime;

extern float HoldCurrent,HoldVoltage;
extern float StepTime;
extern float HoldTime;


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
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
    artExec(fd,"out:dig:off(0x94)",result,0);
    artExec(fd,"out:dig:off(0x08)",result,0);
  }  
//动作值
void OutputSequencer1(int fd)
  {//测试动作值的sequencer
  //测试原理：a、继电器类型为电压启动电压返回，V12从初始电压按步长和步长时间上升，到保护动作
  //(如果没有动作测试结束)，然后V12直接到额定电压后按步长和步长时间下降，到保护返回(如果没有
  //返回，则一直下降到0)。
  //b、继电器类型为电流启动电流返回，I1从初始电流按步长和步长时间上升，到保护动作(如果没有
  //动作测试结束)，然后I1直接到额定电流后按步长和步长时间下降，到保护返回(如果没有
  //返回，则一直下降到0)。
  //c、继电器类型为电流启动电流保持，I1从初始电流按步长和步长时间上升，到保护动作(如果没有
  //动作测试结束)，然后I2输出，I1保持动作电流，3s后关闭I1；然后I2按步长和步长时间下降，到保
  //护返回(如果没有返回，则一直下降到0)。
  //d、继电器类型为电压启动电流保持，V12从初始电压按步长和步长时间上升，到保护动作(如果没有
  //动作测试结束)，然后I1输出，V12保持动作电压，3s后关闭V12；然后I1按步长和步长时间下降，到保
  //护返回(如果没有返回，则一直下降到0)。
  //e、继电器类型为电流启动电压保持，I1从初始电流按步长和步长时间上升，到保护动作(如果没有
  //动作测试结束)，然后V12	输出，I1保持动作电流，3s后关闭I1；然后V12按步长和步长时间下降，到保
  //护返回(如果没有返回，则一直下降到0)。
    float StartValue,EndValue,Step;

    //artExec(fd,"out:dig:on(0x08)",result,0); //

    //开始前，强制关闭IA IB IC 2016-5-19
    artExec(fd,"out:dig:off(0x94)",result,0);//IA ，IB ，IC 放大器对应DB2，DB4， DB7

    if(RelayType==1)//继电器类型为电压启动电压返回
      {
        StartValue=StartVoltage/2;//电压输出接线为V12。
        EndValue=EndVoltage/2;
        Step=VolActStep/2;
      //  artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==2)//继电器类型为电流启动电流返回
      {
        StartValue=StartCurrent;//电流输出接线为I1。
        EndValue=EndCurrent;
        Step=CurActStep;
        artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==3)//继电器类型为电流启动电流保持
      {
      	StartValue=StartCurrent;//电流输出接线为I1。
        EndValue=EndCurrent;
      	Step=CurActStep;
        artExec(fd,"out:dig:on(0x14)",result,0);//IA IB都打开
      }
    else if(RelayType==4)//继电器类型为电压启动电流保持
      {
      	StartValue=StartVoltage/2;//电压输出接线为V12。
        EndValue=EndVoltage/2;
        Step=VolActStep/2;
       // artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==5)//继电器类型为电流启动电压保持
      {
      	StartValue=StartCurrent;//电流输出接线为I1。
        EndValue=EndCurrent;
      	Step=CurActStep;
        artExec(fd,"out:dig:on(0x04)",result,0);
      }

    change_timedata=StepTime;

  //  artExec(fd,"inp:dig(1):thres(1)",result,0);//打开放大器
    artExec(fd,"inp:dig:interval(0)",result,0);//送开关量分辨率
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);//第一步
    
    switch(RelayType)
      {
        case 1:
          channel_input(fd,1,StartValue,0,SystemFrec,DCAC,1,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:v(1:1):on",result,0);
          if(DCAC==0)
            channel_input(fd,1,StartValue,180,SystemFrec,DCAC,2,Step,0,0);
          else if(DCAC==1)
            channel_input(fd,1,0-StartValue,0,SystemFrec,DCAC,2,0-Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:v(1:2):on",result,0);
          break;
        case 2:
          channel_input(fd,0,StartValue,0,SystemFrec,DCAC,1,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:i(1:1):on",result,0);
          break;
        case 3:
          channel_input(fd,0,StartValue,0,SystemFrec,DCAC,1,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:i(1:1):on",result,0);
          break;
        case 4:
          channel_input(fd,1,StartValue,0,SystemFrec,DCAC,1,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:v(1:1):on",result,0);
          if(DCAC==0)
            channel_input(fd,1,StartValue,180,SystemFrec,DCAC,2,Step,0,0);
          else if(DCAC==1)
            channel_input(fd,1,0-StartValue,0,SystemFrec,DCAC,2,0-Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:v(1:2):on",result,0);
          break;
        case 5:
          channel_input(fd,0,StartValue,0,SystemFrec,DCAC,1,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:i(1:1):on",result,0);
          break;
      }
    //电压(电流)从初始值按步长上升
    artExec(fd,"inp:buf:sam(bin,2)",result,0);

    int sumstep=floor(fabs((StartValue-EndValue)/Step))+1;
    out_time=sumstep*StepTime;

    //artExec(fd,"seq:wait(orbin(1),2,0,0,1)",result,0);//第二步
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);
    //2007-8-3 13:44,改为或的关系
    
//    artExec(fd,"out:ana:v(1:1):off",result,0);
//    artExec(fd,"out:ana:v(1:2):off",result,0);
//    artExec(fd,"out:ana:v(1:3):off",result,0);
//    artExec(fd,"out:ana:i(2:1):off",result,0);
//    artExec(fd,"out:ana:i(2:2):off",result,0);
//    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);//第三步
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);//跳2步
    
//    artExec(fd,"out:ana:v(1:1):off",result,0);
//    artExec(fd,"out:ana:v(1:2):off",result,0);
//    artExec(fd,"out:ana:v(1:3):off",result,0);
//    artExec(fd,"out:ana:i(2:1):off",result,0);
//    artExec(fd,"out:ana:i(2:2):off",result,0);
//    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,4)",result,0);//第四步
    /*if(RelayType==3||RelayType==4||RelayType==5)
      out_time=10;//如果是做启动保持的继电器，动作后要保持3s时间
    else if(RelayType==1||RelayType==2)
      out_time=-1;//如果是做启动返回的继电器，动作后没有保持时间*/
    out_time=-1;  
    artExec(fd,"seq:wait(0,1)",result,0);//跳一步
            
    /*if(RelayType==3)
      artExec(fd,"out:ana:i(2:1):off",result,0);
    else if(RelayType==4)
      {
        artExec(fd,"out:ana:v(1:1):off",result,0);
        artExec(fd,"out:ana:v(1:2):off",result,0);
      }
    else if(RelayType==5)
      artExec(fd,"out:ana:i(2:1):off",result,0); */

//    artExec(fd,"out:ana:v(1:1):off",result,0);
//    artExec(fd,"out:ana:v(1:2):off",result,0);
//    artExec(fd,"out:ana:v(1:3):off",result,0);
//    artExec(fd,"out:ana:i(2:1):off",result,0);
//    artExec(fd,"out:ana:i(2:2):off",result,0);
//    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
    
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
  }
//返回值
void OutputSequencer2(int fd)
  {
    float StartValue,EndValue,Step;
     artExec(fd,"out:dig:off(0x08)",result,0);
    if(RelayType==1)//继电器类型为电压启动电压返回
      {
        StartValue=TripValue*1.2/2;//电压输出接线为V12。
        EndValue=0;
        Step=0-VolReturnStep/2;
       //   artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==2)//继电器类型为电流启动电流返回
      {
        StartValue=TripValue*1.2;//电流输出接线为I1。
        EndValue=0;
        Step=0-CurReturnStep;
        artExec(fd,"out:dig:on(0x04)",result,0);

      }
    else if(RelayType==3)//继电器类型为电流启动电流保持
      {
        StartValue=HoldCurrent;//电流输出接线为I1。
        EndValue=0;
      	Step=0-CurReturnStep;
        artExec(fd,"out:dig:on(0x14)",result,0);//IA IB都打开
      }
    else if(RelayType==4)//继电器类型为电压启动电流保持
      {
        StartValue=HoldCurrent;//电流输出接线为I1。
        EndValue=0;
        Step=0-CurReturnStep;
        artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==5)//继电器类型为电流启动电压保持
      {
        StartValue=HoldVoltage/2;//电压输出接线为V12。
        EndValue=0;
      	Step=0-VolReturnStep/2;
//        artExec(fd,"out:dig:on(0x04)",result,0);
      }
    
    //artExec(fd,"inp:dig(1):thres(1)",result,0);
    artExec(fd,"inp:dig:interval(0)",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    
    change_timedata=0;
    if(RelayType==3)
      {
        channel_input(fd,0,TripValue,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        channel_input(fd,0,StartValue,180,SystemFrec,DCAC,2,0,0,0);
        artExec(fd,"out:ana:i(1:2):on",result,0);
      }
    else if(RelayType==4)
      {
        channel_input(fd,1,TripValue/2,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        if(DCAC==1)
          channel_input(fd,1,0-TripValue/2,0,SystemFrec,DCAC,2,0,0,0);
        else if(DCAC==0)
          channel_input(fd,1,TripValue/2,180,SystemFrec,DCAC,2,0,0,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
        channel_input(fd,0,StartValue,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
      }
    else if(RelayType==5)
      {
        channel_input(fd,0,TripValue,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"others:steptime(1)",result,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
        channel_input(fd,1,StartValue,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        if(DCAC==1)
          channel_input(fd,1,0-StartValue,180,SystemFrec,DCAC,2,0,0,0);
        else if(DCAC==0)
          channel_input(fd,1,StartValue,180,SystemFrec,DCAC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
      }
    
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    if(RelayType==3||RelayType==4||RelayType==5)
      out_time=HoldTime;//如果是做启动保持的继电器，动作后要保持3s时间
    else if(RelayType==1||RelayType==2)
      out_time=-1;//如果是做启动返回的继电器，动作后没有保持时间
    artExec(fd,"seq:wait(0,1)",result,0);
    
    change_timedata=StepTime;
    switch(RelayType)
      {
        case 1:
          channel_input(fd,1,StartValue,0,SystemFrec,DCAC,1,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:v(1:1):on",result,0);
          if(DCAC==1)
            channel_input(fd,1,0-StartValue,180,SystemFrec,DCAC,2,0-Step,0,0);
          else if(DCAC==0)
            channel_input(fd,1,StartValue,180,SystemFrec,DCAC,2,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:v(1:2):on",result,0);
          break;
        case 2:
          channel_input(fd,0,StartValue,0,SystemFrec,DCAC,1,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:i(1:1):on",result,0);
          break;
        case 3:
          channel_input(fd,0,StartValue,0,SystemFrec,DCAC,2,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:i(1:2):on",result,0);
          
          artExec(fd,"out:ana:i(2:1):off",result,0);
          break;
        case 4:
          channel_input(fd,0,StartValue,0,SystemFrec,DCAC,1,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:i(1:1):on",result,0);
          
          artExec(fd,"out:ana:v(1:1):off",result,0);
        artExec(fd,"out:ana:v(1:2):off",result,0);
          break;
        case 5:
          channel_input(fd,1,StartValue,0,SystemFrec,DCAC,1,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:v(1:1):on",result,0);
          if(DCAC==1)
            channel_input(fd,1,0-StartValue,180,SystemFrec,DCAC,2,0-Step,0,0);
          else if(DCAC==0)
            channel_input(fd,1,StartValue,180,SystemFrec,DCAC,2,Step,0,0);
          artExec(fd,"others:steptime(1)",result,0);
          artExec(fd,"out:ana:v(1:2):on",result,0);
          
          artExec(fd,"out:ana:i(2:1):off",result,0);
          break;
      }
        
    artExec(fd,"inp:buf:sam(bin,2)",result,0);
 //   out_time=fabs((EndValue-StartValue)/Step*StepTime);


    int sumstep=floor(fabs((StartValue-EndValue)/Step))+1;
    out_time=sumstep*StepTime;





    //artExec(fd,"seq:wait(orbin(1),2,0,0,1)",result,0);
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);
    //2007-8-3 13:44,改为或的关系
    
    
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);
    
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
      

    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,5)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);
    
    artExec(fd,"seq:end",result,0);
    artExec(fd,"seq:exec",result,0);
  }
//动作时间
void OutputSequencer3(int fd)
  { //测试动作时间时，
    
     artExec(fd,"out:dig:off(0x08)",result,0);
    //artExec(fd,"inp:dig(1):thres(1)",result,0);
    artExec(fd,"inp:dig:interval(0)",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
       
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    if(StepTime>=1)
      out_time=StepTime*2;
    else 
      out_time=2;
    artExec(fd,"seq:wait(0,1)",result,0);
    
    if(RelayType==1)
      {
       //  artExec(fd,"out:dig:on(0x04)",result,0);
        channel_input(fd,1,TestVoltage/2,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        if(DCAC==1)
          channel_input(fd,1,0-TestVoltage/2,180,SystemFrec,DCAC,2,0,0,0);
        else if(DCAC==0)
          channel_input(fd,1,TestVoltage/2,180,SystemFrec,DCAC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
      }
    else if(RelayType==2)
      {
        artExec(fd,"out:dig:on(0x04)",result,0);
        channel_input(fd,0,TestCurrent,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
      }
    else if(RelayType==3)
      {
        artExec(fd,"out:dig:on(0x04)",result,0);
        channel_input(fd,0,TestCurrent,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
      }
    else if(RelayType==4)
      {
      //  artExec(fd,"out:dig:on(0x04)",result,0);
        channel_input(fd,1,TestVoltage/2,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"out:ana:v(1:1):on",result,0);
        if(DCAC==1)
          channel_input(fd,1,0-TestVoltage/2,180,SystemFrec,DCAC,2,0,0,0);
        else if(DCAC==0)
          channel_input(fd,1,TestVoltage/2,180,SystemFrec,DCAC,2,0,0,0);
        artExec(fd,"out:ana:v(1:2):on",result,0);
      }
    else if(RelayType==5)
      {
        artExec(fd,"out:dig:on(0x04)",result,0);
        channel_input(fd,0,TestCurrent,0,SystemFrec,DCAC,1,0,0,0);
        artExec(fd,"out:ana:i(1:1):on",result,0);
      }
    //从0跳变到额定值
    artExec(fd,"inp:buf:sam(bin,2)",result,0);

    if(RelayType==1||RelayType==4)
    out_time=VolTesttime;
    else
    out_time=CurTesttime;



    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);
    //2007-8-3 13:44,改为或的关系
      
    
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);
    
    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
    artExec(fd,"out:ana:i(2:1):off",result,0);
    artExec(fd,"out:ana:i(2:2):off",result,0);
    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,4)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);


    artExec(fd,"out:ana:v(1:1):off",result,0);
    artExec(fd,"out:ana:v(1:2):off",result,0);
    artExec(fd,"out:ana:v(1:3):off",result,0);
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
