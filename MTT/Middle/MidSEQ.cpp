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
//����ֵ
void OutputSequencer1(int fd)
  {//���Զ���ֵ��sequencer
  //����ԭ��a���̵�������Ϊ��ѹ������ѹ���أ�V12�ӳ�ʼ��ѹ�������Ͳ���ʱ������������������
  //(���û�ж������Խ���)��Ȼ��V12ֱ�ӵ����ѹ�󰴲����Ͳ���ʱ���½�������������(���û��
  //���أ���һֱ�½���0)��
  //b���̵�������Ϊ���������������أ�I1�ӳ�ʼ�����������Ͳ���ʱ������������������(���û��
  //�������Խ���)��Ȼ��I1ֱ�ӵ�������󰴲����Ͳ���ʱ���½�������������(���û��
  //���أ���һֱ�½���0)��
  //c���̵�������Ϊ���������������֣�I1�ӳ�ʼ�����������Ͳ���ʱ������������������(���û��
  //�������Խ���)��Ȼ��I2�����I1���ֶ���������3s��ر�I1��Ȼ��I2�������Ͳ���ʱ���½�������
  //������(���û�з��أ���һֱ�½���0)��
  //d���̵�������Ϊ��ѹ�����������֣�V12�ӳ�ʼ��ѹ�������Ͳ���ʱ������������������(���û��
  //�������Խ���)��Ȼ��I1�����V12���ֶ�����ѹ��3s��ر�V12��Ȼ��I1�������Ͳ���ʱ���½�������
  //������(���û�з��أ���һֱ�½���0)��
  //e���̵�������Ϊ����������ѹ���֣�I1�ӳ�ʼ�����������Ͳ���ʱ������������������(���û��
  //�������Խ���)��Ȼ��V12	�����I1���ֶ���������3s��ر�I1��Ȼ��V12�������Ͳ���ʱ���½�������
  //������(���û�з��أ���һֱ�½���0)��
    float StartValue,EndValue,Step;

    //artExec(fd,"out:dig:on(0x08)",result,0); //

    //��ʼǰ��ǿ�ƹر�IA IB IC 2016-5-19
    artExec(fd,"out:dig:off(0x94)",result,0);//IA ��IB ��IC �Ŵ�����ӦDB2��DB4�� DB7

    if(RelayType==1)//�̵�������Ϊ��ѹ������ѹ����
      {
        StartValue=StartVoltage/2;//��ѹ�������ΪV12��
        EndValue=EndVoltage/2;
        Step=VolActStep/2;
      //  artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==2)//�̵�������Ϊ����������������
      {
        StartValue=StartCurrent;//�����������ΪI1��
        EndValue=EndCurrent;
        Step=CurActStep;
        artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==3)//�̵�������Ϊ����������������
      {
      	StartValue=StartCurrent;//�����������ΪI1��
        EndValue=EndCurrent;
      	Step=CurActStep;
        artExec(fd,"out:dig:on(0x14)",result,0);//IA IB����
      }
    else if(RelayType==4)//�̵�������Ϊ��ѹ������������
      {
      	StartValue=StartVoltage/2;//��ѹ�������ΪV12��
        EndValue=EndVoltage/2;
        Step=VolActStep/2;
       // artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==5)//�̵�������Ϊ����������ѹ����
      {
      	StartValue=StartCurrent;//�����������ΪI1��
        EndValue=EndCurrent;
      	Step=CurActStep;
        artExec(fd,"out:dig:on(0x04)",result,0);
      }

    change_timedata=StepTime;

  //  artExec(fd,"inp:dig(1):thres(1)",result,0);//�򿪷Ŵ���
    artExec(fd,"inp:dig:interval(0)",result,0);//�Ϳ������ֱ���
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);
    
    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);//��һ��
    
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
    //��ѹ(����)�ӳ�ʼֵ����������
    artExec(fd,"inp:buf:sam(bin,2)",result,0);

    int sumstep=floor(fabs((StartValue-EndValue)/Step))+1;
    out_time=sumstep*StepTime;

    //artExec(fd,"seq:wait(orbin(1),2,0,0,1)",result,0);//�ڶ���
    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);
    //2007-8-3 13:44,��Ϊ��Ĺ�ϵ
    
//    artExec(fd,"out:ana:v(1:1):off",result,0);
//    artExec(fd,"out:ana:v(1:2):off",result,0);
//    artExec(fd,"out:ana:v(1:3):off",result,0);
//    artExec(fd,"out:ana:i(2:1):off",result,0);
//    artExec(fd,"out:ana:i(2:2):off",result,0);
//    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,3)",result,0);//������
    out_time=-1;
    artExec(fd,"seq:wait(0,2)",result,0);//��2��
    
//    artExec(fd,"out:ana:v(1:1):off",result,0);
//    artExec(fd,"out:ana:v(1:2):off",result,0);
//    artExec(fd,"out:ana:v(1:3):off",result,0);
//    artExec(fd,"out:ana:i(2:1):off",result,0);
//    artExec(fd,"out:ana:i(2:2):off",result,0);
//    artExec(fd,"out:ana:i(2:3):off",result,0);
    artExec(fd,"inp:buf:sam(bin,4)",result,0);//���Ĳ�
    /*if(RelayType==3||RelayType==4||RelayType==5)
      out_time=10;//��������������ֵļ̵�����������Ҫ����3sʱ��
    else if(RelayType==1||RelayType==2)
      out_time=-1;//��������������صļ̵�����������û�б���ʱ��*/
    out_time=-1;  
    artExec(fd,"seq:wait(0,1)",result,0);//��һ��
            
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
//����ֵ
void OutputSequencer2(int fd)
  {
    float StartValue,EndValue,Step;
     artExec(fd,"out:dig:off(0x08)",result,0);
    if(RelayType==1)//�̵�������Ϊ��ѹ������ѹ����
      {
        StartValue=TripValue*1.2/2;//��ѹ�������ΪV12��
        EndValue=0;
        Step=0-VolReturnStep/2;
       //   artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==2)//�̵�������Ϊ����������������
      {
        StartValue=TripValue*1.2;//�����������ΪI1��
        EndValue=0;
        Step=0-CurReturnStep;
        artExec(fd,"out:dig:on(0x04)",result,0);

      }
    else if(RelayType==3)//�̵�������Ϊ����������������
      {
        StartValue=HoldCurrent;//�����������ΪI1��
        EndValue=0;
      	Step=0-CurReturnStep;
        artExec(fd,"out:dig:on(0x14)",result,0);//IA IB����
      }
    else if(RelayType==4)//�̵�������Ϊ��ѹ������������
      {
        StartValue=HoldCurrent;//�����������ΪI1��
        EndValue=0;
        Step=0-CurReturnStep;
        artExec(fd,"out:dig:on(0x04)",result,0);
      }
    else if(RelayType==5)//�̵�������Ϊ����������ѹ����
      {
        StartValue=HoldVoltage/2;//��ѹ�������ΪV12��
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
      out_time=HoldTime;//��������������ֵļ̵�����������Ҫ����3sʱ��
    else if(RelayType==1||RelayType==2)
      out_time=-1;//��������������صļ̵�����������û�б���ʱ��
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
    //2007-8-3 13:44,��Ϊ��Ĺ�ϵ
    
    
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
//����ʱ��
void OutputSequencer3(int fd)
  { //���Զ���ʱ��ʱ��
    
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
    //��0���䵽�ֵ
    artExec(fd,"inp:buf:sam(bin,2)",result,0);

    if(RelayType==1||RelayType==4)
    out_time=VolTesttime;
    else
    out_time=CurTesttime;



    artExec(fd,"seq:wait(orbin(1),bin(1),bin(1),bin(1),2,0,0,1)",result,0);
    //2007-8-3 13:44,��Ϊ��Ĺ�ϵ
      
    
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
