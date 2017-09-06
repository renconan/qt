#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "Config.h"



#include "Testcom2.h"

extern  unsigned  char result[70],receive_data[70];
extern double out_time,change_timedata;

int   FreqTriple=2;//����
extern int   VariatyVz2;

extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,VzRelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase;




float  A_V1PerFaultValue,A_V2PerFaultValue,A_V3PerFaultValue,A_VzPerFaultValue,A_I1PerFaultValue,A_I2PerFaultValue,A_I3PerFaultValue;
float  A_V1PerFaultPhase,A_V2PerFaultPhase,A_V3PerFaultPhase,A_VzPerFaultPhase,A_I1PerFaultPhase,A_I2PerFaultPhase,A_I3PerFaultPhase;



bool checkiszero(float x)
{

    if ((x >= -0.0001) && (x <=0.0001))
    return true;
    else
    return false;
}



float  valuefrec(float Frequency,float Value,int type)
{perror(" 2");
      float ValueFreqPara;
      if (type==1)//电压
      {
          if(Frequency<50)
           {
            ValueFreqPara = 0.9998 + 0.0002 * (1-0.001*(Frequency-50)*(Frequency-50));

           }
           else
            ValueFreqPara = 0.9998 + 0.0002  * (1 + 0.0002 * (Frequency - 50)*(Frequency - 50));

      }
      else{//电流
            if(Frequency <50)
                ValueFreqPara = 0.9998 +0.0002*(1 - 0.0005*(Frequency - 50)*(Frequency - 50));
            else
              ValueFreqPara = 0.9998 + 0.0002 *(1 + 0.00012*(Frequency - 50)*(Frequency - 50));
          }
      perror(" 3");
     return ValueFreqPara*Value;
}


void ChangeOutputValue()
  {
     perror(" 1");
     m_Ch[0].OutputValue=valuefrec(m_Ch[0].Freq,m_Ch[0].Amp,1);
     perror(" 4");
     m_Ch[1].OutputValue=valuefrec(m_Ch[1].Freq,m_Ch[1].Amp,1);
     m_Ch[2].OutputValue=valuefrec(m_Ch[2].Freq,m_Ch[2].Amp,1);
     m_Ch[3].OutputValue=valuefrec(m_Ch[3].Freq,m_Ch[3].Amp,1);

     m_Ch[4].OutputValue=valuefrec(m_Ch[4].Freq,m_Ch[4].Amp,2);
     m_Ch[5].OutputValue=valuefrec(m_Ch[5].Freq,m_Ch[5].Amp,2);
     m_Ch[6].OutputValue=valuefrec(m_Ch[6].Freq,m_Ch[6].Amp,2);
  }



void output_signal_manual(int k,int fd)  //manual way
  {
    ChangeOutputValue();//2016-4-28修改，电流从0开始增加，点第一次增加后未出值，再点一下才出�
    //原因为第一次的值没输入刷新，ChangeOutputValue()更新 A_A_I1OutputValue，I2OutputValue，I3OutputValue
    perror(" 5");
    if(fabs(m_Ch[4].OutputValue)<=0.00001)
       artExec(fd,"out:dig:off(0x20)",result,0);//IA放大器对应DB2//2017-7-7���������޸�
    else
       artExec(fd,"out:dig:on(0x20)",result,0);

    if(fabs(m_Ch[5].OutputValue)<=0.00001)
       artExec(fd,"out:dig:off(0x20)",result,0);//IB放大器对应DB4
    else
       artExec(fd,"out:dig:on(0x20)",result,0);

    if(fabs(m_Ch[6].OutputValue)<=0.00001)
       artExec(fd,"out:dig:off(0x40)",result,0);//IC放大器对应DB7
    else
       artExec(fd,"out:dig:on(0x40)",result,0);


   // artExec(fd,"inp:dig(1):thres(1)",result,0);  //打开放大�
    artExec(fd,"inp:dig:interval(1)",result,0);
    artExec(fd,"out:ana:user:mem?",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    if(k!=2)
  {  artExec(fd,"inp:buf:sam(bin,1)",result,0);
     out_time=-1;
     artExec(fd,"seq:wait(0,1)",result,0);
  }
    if(m_Ch[0].Amp==0)
      m_Ch[0].Output=0;
    else
      m_Ch[0].Output=1;

    if(m_Ch[1].Amp==0)
      m_Ch[1].Output=0;
    else
      m_Ch[1].Output=1;

    if(m_Ch[2].Amp==0)
      m_Ch[2].Output=0;
    else
      m_Ch[2].Output=1;

    if(m_Ch[3].Amp==0)
        m_Ch[3].Output=0;
    else
        m_Ch[3].Output=1;



    if(fabs(m_Ch[4].Amp)<=0.00001)
      m_Ch[4].Output=0;
    else
      m_Ch[4].Output=1;

    if(fabs(m_Ch[5].Amp)<=0.00001)
      m_Ch[5].Output=0;
    else
      m_Ch[5].Output=1;

    if(fabs(m_Ch[6].Amp)<=0.00001)
      m_Ch[6].Output=0;
    else
      m_Ch[6].Output=1;

    if(k==0)
      {


            channel_input(fd,1,m_Ch[0].OutputValue,m_Ch[0].Phase,m_Ch[0].Freq/FreqTriple,m_Ch[0].DC,1,0,0,0);
            artExec(fd,"out:ana:v(1:1):on",result,0);
            channel_input(fd,1,m_Ch[1].OutputValue,m_Ch[1].Phase,m_Ch[1].Freq/FreqTriple,m_Ch[1].DC,2,0,0,0);
            artExec(fd,"out:ana:v(1:2):on",result,0);
            channel_input(fd,1,m_Ch[2].OutputValue,m_Ch[2].Phase,m_Ch[2].Freq/FreqTriple,m_Ch[2].DC,3,0,0,0);
            artExec(fd,"out:ana:v(1:3):on",result,0);

            channel_input(fd,1,m_Ch[3].OutputValue,m_Ch[3].Phase,m_Ch[3].Freq/FreqTriple,m_Ch[3].DC,4,0,0,0);
            artExec(fd,"out:ana:v(1:4):on",result,0);

            channel_input(fd,0,m_Ch[4].OutputValue,m_Ch[4].Phase,m_Ch[4].Freq/FreqTriple,m_Ch[4].DC,1,0,0,0);
            artExec(fd,"out:ana:i(1:1):on",result,0);
            channel_input(fd,0,m_Ch[5].OutputValue,m_Ch[5].Phase,m_Ch[5].Freq/FreqTriple,m_Ch[5].DC,2,0,0,0);
            artExec(fd,"out:ana:i(1:2):on",result,0);
            channel_input(fd,0,m_Ch[6].OutputValue,m_Ch[6].Phase,m_Ch[6].Freq/FreqTriple,m_Ch[6].DC,3,0,0,0);
            artExec(fd,"out:ana:i(1:3):on",result,0);

      }
    else if(k==1)
      {
        switch(A_Variable)
          {
            case 1:
              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;
              ChangeOutputValue();



              artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[0].OutputValue,360,m_Ch[0].Freq/FreqTriple,m_Ch[0].DC,1,0,0,0);
              artExec(fd,"out:ana:v(1:1):on",result,0);

              artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[1].OutputValue,360,m_Ch[1].Freq/FreqTriple,m_Ch[1].DC,2,0,0,0);
              artExec(fd,"out:ana:v(1:2):on",result,0);

              artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[2].OutputValue,360,m_Ch[2].Freq/FreqTriple,m_Ch[2].DC,3,0,0,0);
              artExec(fd,"out:ana:v(1:3):on",result,0);

              artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[3].OutputValue,360,m_Ch[3].Freq/FreqTriple,m_Ch[3].DC,4,0,0,0);
              artExec(fd,"out:ana:v(1:4):on",result,0);



              artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
              channel_input(fd,0,m_Ch[4].OutputValue,360,m_Ch[4].Freq/FreqTriple,m_Ch[4].DC,1,0,0,0);
              artExec(fd,"out:ana:i(1:1):on",result,0);

              artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
              channel_input(fd,0,m_Ch[5].OutputValue,360,m_Ch[5].Freq/FreqTriple,m_Ch[5].DC,2,0,0,0);
              artExec(fd,"out:ana:i(1:2):on",result,0);

              artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
              channel_input(fd,0,m_Ch[6].OutputValue,360,m_Ch[6].Freq/FreqTriple,m_Ch[6].DC,3,0,0,0);
              artExec(fd,"out:ana:i(1:3):on",result,0);
              break;
            case 2:





              artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[0].OutputValue,360,m_Ch[0].Freq/FreqTriple,m_Ch[0].DC,1,0,0,0);
              artExec(fd,"out:ana:v(1:1):on",result,0);

              artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[1].OutputValue,360,m_Ch[1].Freq/FreqTriple,m_Ch[1].DC,2,0,0,0);
              artExec(fd,"out:ana:v(1:2):on",result,0);

              artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[2].OutputValue,360,m_Ch[2].Freq/FreqTriple,m_Ch[2].DC,3,0,0,0);
              artExec(fd,"out:ana:v(1:3):on",result,0);

              artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[3].OutputValue,360,m_Ch[3].Freq/FreqTriple,m_Ch[3].DC,4,0,0,0);
              artExec(fd,"out:ana:v(1:4):on",result,0);



              artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
              channel_input(fd,0,m_Ch[4].OutputValue,360,m_Ch[4].Freq/FreqTriple,m_Ch[4].DC,1,0,0,0);
              artExec(fd,"out:ana:i(1:1):on",result,0);
              artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
              channel_input(fd,0,m_Ch[5].OutputValue,360,m_Ch[5].Freq/FreqTriple,m_Ch[5].DC,2,0,0,0);
              artExec(fd,"out:ana:i(1:2):on",result,0);
              artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
              channel_input(fd,0,m_Ch[6].OutputValue,360,m_Ch[6].Freq/FreqTriple,m_Ch[6].DC,3,0,0,0);
              artExec(fd,"out:ana:i(1:3):on",result,0);
              break;
            case 3:
              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;
              ChangeOutputValue();




              artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[0].OutputValue,360,m_Ch[0].Freq/FreqTriple,m_Ch[0].DC,1,0,0,0);
              artExec(fd,"out:ana:v(1:1):on",result,0);

              artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[1].OutputValue,360,m_Ch[1].Freq/FreqTriple,m_Ch[1].DC,2,0,0,0);
              artExec(fd,"out:ana:v(1:2):on",result,0);

              artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[2].OutputValue,360,m_Ch[2].Freq/FreqTriple,m_Ch[2].DC,3,0,0,0);
              artExec(fd,"out:ana:v(1:3):on",result,0);

              artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[3].OutputValue,360,m_Ch[3].Freq/FreqTriple,m_Ch[3].DC,4,0,0,0);
              artExec(fd,"out:ana:v(1:4):on",result,0);



              artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
              channel_input(fd,0,m_Ch[4].OutputValue,360,m_Ch[4].Freq/FreqTriple,m_Ch[4].DC,1,0,0,0);
              artExec(fd,"out:ana:i(1:1):on",result,0);

              artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
              channel_input(fd,0,m_Ch[5].OutputValue,360,m_Ch[5].Freq/FreqTriple,m_Ch[5].DC,2,0,0,0);
              artExec(fd,"out:ana:i(1:2):on",result,0);

              artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
              channel_input(fd,0,m_Ch[6].OutputValue,360,m_Ch[6].Freq/FreqTriple,m_Ch[6].DC,3,0,0,0);
              artExec(fd,"out:ana:i(1:3):on",result,0);
              break;
          }
         }
    else if(k==2)
         {
            //故障前参�
            A_V1PerFaultValue=SysVolt;//57.73;
            A_V2PerFaultValue=SysVolt;//57.73;
            A_V3PerFaultValue=SysVolt;//57.73;
            A_VzPerFaultValue=SysVolt;//57.73;
            A_I1PerFaultValue=0;
            A_I2PerFaultValue=0;
            A_I3PerFaultValue=0;

            A_V1PerFaultPhase=m_Ch[0].Phase-360*SysFrec*A_PreFaultTime;
            A_V2PerFaultPhase=m_Ch[1].Phase-360*SysFrec*A_PreFaultTime;
            A_V3PerFaultPhase=m_Ch[2].Phase-360*SysFrec*A_PreFaultTime;
            A_VzPerFaultPhase=m_Ch[3].Phase-360*SysFrec*A_PreFaultTime;
            A_V1PerFaultPhase=A_V1PerFaultPhase-((int)(A_V1PerFaultPhase/360))*360;
            A_V2PerFaultPhase=A_V2PerFaultPhase-((int)(A_V2PerFaultPhase/360))*360;
            A_V3PerFaultPhase=A_V3PerFaultPhase-((int)(A_V3PerFaultPhase/360))*360;
            A_VzPerFaultPhase=A_VzPerFaultPhase-((int)(A_VzPerFaultPhase/360))*360;


            A_I1PerFaultPhase=m_Ch[4].Phase-360*SysFrec*A_PreFaultTime;
            A_I2PerFaultPhase=m_Ch[5].Phase-360*SysFrec*A_PreFaultTime;
            A_I3PerFaultPhase=m_Ch[6].Phase-360*SysFrec*A_PreFaultTime;

            A_I1PerFaultPhase=A_I1PerFaultPhase-((int)(A_I1PerFaultPhase/360))*360;
            A_I2PerFaultPhase=A_I2PerFaultPhase-((int)(A_I2PerFaultPhase/360))*360;
            A_I3PerFaultPhase=A_I3PerFaultPhase-((int)(A_I3PerFaultPhase/360))*360;
            //触发模式相位连续修改2016-8-16
            if(m_Ch[4].OutputValue>=0.00001&&fabs(m_Ch[5].OutputValue)<0.00001&&fabs(m_Ch[6].OutputValue)<0.00001)
            {
                A_V1PerFaultPhase=A_V2PerFaultPhase+120;
                A_V1PerFaultPhase=A_V1PerFaultPhase-((int)(A_V1PerFaultPhase/360))*360;
                A_V3PerFaultPhase=A_V2PerFaultPhase-120;
                A_V3PerFaultPhase=A_V3PerFaultPhase-((int)(A_V3PerFaultPhase/360))*360;
            }
            if(m_Ch[5].OutputValue>=0.00001&&fabs(m_Ch[4].OutputValue)<0.00001&&fabs(m_Ch[6].OutputValue)<0.00001)
            {
                A_V1PerFaultPhase=A_V3PerFaultPhase-120;
                A_V1PerFaultPhase=A_V1PerFaultPhase-((int)(A_V1PerFaultPhase/360))*360;
                A_V2PerFaultPhase=A_V3PerFaultPhase+120;
                A_V2PerFaultPhase=A_V2PerFaultPhase-((int)(A_V2PerFaultPhase/360))*360;
            }
            if(m_Ch[6].OutputValue>=0.00001&&fabs(m_Ch[5].OutputValue)<0.00001&&fabs(m_Ch[6].OutputValue)<0.00001)
            {
                A_V2PerFaultPhase=A_V1PerFaultPhase-120;
                A_V2PerFaultPhase=A_V2PerFaultPhase-((int)(A_V2PerFaultPhase/360))*360;
                A_V3PerFaultPhase=A_V1PerFaultPhase+120;
                A_V3PerFaultPhase=A_V3PerFaultPhase-((int)(A_V3PerFaultPhase/360))*360;
            }
            if(m_Ch[4].OutputValue>=0.00001&&m_Ch[5].OutputValue>=0.00001&&fabs(m_Ch[6].OutputValue)<0.00001)
            {
                A_V1PerFaultPhase=A_V3PerFaultPhase-120;
                A_V1PerFaultPhase=A_V1PerFaultPhase-((int)(A_V1PerFaultPhase/360))*360;
                A_V2PerFaultPhase=A_V3PerFaultPhase+120;
                A_V2PerFaultPhase=A_V2PerFaultPhase-((int)(A_V2PerFaultPhase/360))*360;
            }
            if(m_Ch[5].OutputValue>=0.00001&&m_Ch[6].OutputValue>=0.00001&&fabs(m_Ch[4].OutputValue)<0.00001)
            {
                A_V2PerFaultPhase=A_V1PerFaultPhase-120;
                A_V2PerFaultPhase=A_V2PerFaultPhase-((int)(A_V2PerFaultPhase/360))*360;
                A_V3PerFaultPhase=A_V1PerFaultPhase+120;
                A_V3PerFaultPhase=A_V3PerFaultPhase-((int)(A_V3PerFaultPhase/360))*360;
            }
            if(m_Ch[6].OutputValue>=0.00001&&m_Ch[4].OutputValue>=0.00001&&fabs(m_Ch[5].OutputValue)<0.00001)
            {
                A_V1PerFaultPhase=A_V2PerFaultPhase+120;
                A_V1PerFaultPhase=A_V1PerFaultPhase-((int)(A_V1PerFaultPhase/360))*360;
                A_V3PerFaultPhase=A_V2PerFaultPhase-120;
                A_V3PerFaultPhase=A_V3PerFaultPhase-((int)(A_V3PerFaultPhase/360))*360;
            }

            V1RelaPhase=0;
            V2RelaPhase=0;
            V3RelaPhase=0;
            VzRelaPhase=0;
            I1RelaPhase=0;
            I2RelaPhase=0;
            I3RelaPhase=0;

            //故障前电压每相输�7.73V，相位对称，电流输出��



            //第一� 故障�
            artExec(fd,"others:steptime(1)",result,0);//2009-6-23
            channel_input(fd,1,A_V1PerFaultValue,A_V1PerFaultPhase,SysFrec/FreqTriple,m_Ch[0].DC,1,0,0,0);
            artExec(fd,"out:ana:v(1:1):on",result,0);
            channel_input(fd,1,A_V2PerFaultValue,A_V2PerFaultPhase,SysFrec/FreqTriple,m_Ch[1].DC,2,0,0,0);
            artExec(fd,"out:ana:v(1:2):on",result,0);
            channel_input(fd,1,A_V3PerFaultValue,A_V3PerFaultPhase,SysFrec/FreqTriple,m_Ch[2].DC,3,0,0,0);
            artExec(fd,"out:ana:v(1:3):on",result,0);

            channel_input(fd,1,A_VzPerFaultValue,A_VzPerFaultPhase,SysFrec/FreqTriple,m_Ch[3].DC,4,0,0,0);
            artExec(fd,"out:ana:v(1:4):on",result,0);

            channel_input(fd,0,A_I1PerFaultValue,A_I1PerFaultPhase,SysFrec/FreqTriple,m_Ch[4].DC,1,0,0,0);
            artExec(fd,"out:ana:i(1:1):on",result,0);
            channel_input(fd,0,A_I2PerFaultValue,A_I2PerFaultPhase,SysFrec/FreqTriple,m_Ch[5].DC,2,0,0,0);
            artExec(fd,"out:ana:i(1:2):on",result,0);
            channel_input(fd,0,A_I3PerFaultValue,A_I3PerFaultPhase,SysFrec/FreqTriple,m_Ch[6].DC,3,0,0,0);
            artExec(fd,"out:ana:i(1:3):on",result,0);

            artExec(fd,"inp:buf:sam(bin,1)",result,0);

            if(checkiszero(A_PreFaultTime)==false)
            out_time=A_PreFaultTime; //;  故障前时�
            else
            out_time=-1;
            artExec(fd,"seq:wait(0,1)",result,0);//第一�


            //第二� 故障
            artExec(fd,"others:steptime(1)",result,0);//2009-6-23
            artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
            channel_input(fd,1,m_Ch[0].OutputValue,m_Ch[0].Phase,m_Ch[0].Freq/FreqTriple,m_Ch[0].DC,1,0,0,0);
            artExec(fd,"out:ana:v(1:1):on",result,0);

            artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
            channel_input(fd,1,m_Ch[1].OutputValue,m_Ch[1].Phase,m_Ch[1].Freq/FreqTriple,m_Ch[1].DC,2,0,0,0);
            artExec(fd,"out:ana:v(1:2):on",result,0);

            artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
            channel_input(fd,1,m_Ch[2].OutputValue,m_Ch[2].Phase,m_Ch[2].Freq/FreqTriple,m_Ch[2].DC,3,0,0,0);
            artExec(fd,"out:ana:v(1:3):on",result,0);


            artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
            if(VariatyVz2!=1)
            channel_input(fd,1,m_Ch[3].OutputValue,m_Ch[3].Phase,m_Ch[3].Freq/FreqTriple,m_Ch[3].DC,4,0,0,0);
            artExec(fd,"out:ana:v(1:4):on",result,0);



            artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
            channel_input(fd,0,m_Ch[4].OutputValue,m_Ch[4].Phase,m_Ch[4].Freq/FreqTriple,m_Ch[4].DC,1,0,0,0);
            artExec(fd,"out:ana:i(1:1):on",result,0);

            artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
            channel_input(fd,0,m_Ch[5].OutputValue,m_Ch[5].Phase,m_Ch[5].Freq/FreqTriple,m_Ch[5].DC,2,0,0,0);
            artExec(fd,"out:ana:i(1:2):on",result,0);

            artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
            channel_input(fd,0,m_Ch[6].OutputValue,m_Ch[6].Phase,m_Ch[6].Freq/FreqTriple,m_Ch[6].DC,3,0,0,0);
            artExec(fd,"out:ana:i(1:3):on",result,0);


            artExec(fd,"inp:buf:sam(bin,2)",result,0);
            out_time=A_FaultTime; //故障时间
            artExec(fd,"seq:wait(orbin(1),bin(1),2,0,0,1)",result,0);
            //2007-8-3 13:44,改为或的关系

            // 故障�
             artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
             channel_input(fd,1,A_V1PerFaultValue,A_V1PerFaultPhase,SysFrec/FreqTriple,m_Ch[0].DC,1,0,0,0);
             artExec(fd,"out:ana:v(1:1):on",result,0);

             artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
             channel_input(fd,1,A_V2PerFaultValue,A_V2PerFaultPhase,SysFrec/FreqTriple,m_Ch[1].DC,2,0,0,0);
             artExec(fd,"out:ana:v(1:2):on",result,0);

             artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
             channel_input(fd,1,A_V3PerFaultValue,A_V3PerFaultPhase,SysFrec/FreqTriple,m_Ch[2].DC,3,0,0,0);
             artExec(fd,"out:ana:v(1:3):on",result,0);


             artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
             channel_input(fd,1,A_VzPerFaultValue,A_VzPerFaultPhase,SysFrec/FreqTriple,m_Ch[3].DC,4,0,0,0);
             artExec(fd,"out:ana:v(1:4):on",result,0);



             artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
             channel_input(fd,0,A_I1PerFaultValue,A_I1PerFaultPhase,SysFrec/FreqTriple,m_Ch[4].DC,1,0,0,0);
             artExec(fd,"out:ana:i(1:1):on",result,0);

             artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
             channel_input(fd,0,A_I2PerFaultValue,A_I2PerFaultPhase,SysFrec/FreqTriple,m_Ch[5].DC,2,0,0,0);
             artExec(fd,"out:ana:i(1:2):on",result,0);

             artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
             channel_input(fd,0,A_I3PerFaultValue,A_I3PerFaultPhase,SysFrec/FreqTriple,m_Ch[6].DC,3,0,0,0);
             artExec(fd,"out:ana:i(1:3):on",result,0);

             artExec(fd,"inp:buf:sam(bin,3)",result,0);
             out_time=A_PostFaultTime; // 故障后时�第三�
             artExec(fd,"seq:wait(0,2)",result,0);



             artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
             channel_input(fd,1,A_V1PerFaultValue,A_V1PerFaultPhase,SysFrec/FreqTriple,m_Ch[0].DC,1,0,0,0);
             artExec(fd,"out:ana:v(1:1):on",result,0);

             artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
             channel_input(fd,1,A_V2PerFaultValue,A_V2PerFaultPhase,SysFrec/FreqTriple,m_Ch[1].DC,2,0,0,0);
             artExec(fd,"out:ana:v(1:2):on",result,0);


             artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
             channel_input(fd,1,A_V3PerFaultValue,A_V3PerFaultPhase,SysFrec/FreqTriple,m_Ch[2].DC,3,0,0,0);
             artExec(fd,"out:ana:v(1:3):on",result,0);

             artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
             channel_input(fd,1,A_VzPerFaultValue,A_VzPerFaultPhase,SysFrec/FreqTriple,m_Ch[3].DC,4,0,0,0);
             artExec(fd,"out:ana:v(1:4):on",result,0);



             artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
             channel_input(fd,0,A_I1PerFaultValue,A_I1PerFaultPhase,SysFrec/FreqTriple,m_Ch[4].DC,1,0,0,0);

             artExec(fd,"out:ana:i(1:1):on",result,0);


             artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
             channel_input(fd,0,A_I2PerFaultValue,A_I2PerFaultPhase,SysFrec/FreqTriple,m_Ch[5].DC,2,0,0,0);

             artExec(fd,"out:ana:i(1:2):on",result,0);


             artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
             channel_input(fd,0,A_I3PerFaultValue,A_I3PerFaultPhase,SysFrec/FreqTriple,m_Ch[6].DC,3,0,0,0);
             artExec(fd,"out:ana:i(1:3):on",result,0);

             //第四步电压保持动作�faultduration
             artExec(fd,"inp:buf:sam(bin,4)",result,0);
             out_time=A_PostFaultTime;     //故障后时�第四�
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
         }

      if (k!=2)
      {
        artExec(fd,"inp:buf:sam(bin,2)",result,0);
        artExec(fd,"seq:wait(orbin(1),bin(1),2,0)",result,0);
        //2007-8-3 13:44,改为或的关系

        artExec(fd,"inp:buf:sam(bin,3)",result,0);
        out_time=-1;
        artExec(fd,"seq:wait(0,2)",result,0);

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
      }

      artExec(fd,"seq:end",result,0);
      artExec(fd,"seq:exec",result,0);
  }

void output_signal_end(int fd)
  {  
    //artExec(fd,"out:dig:off(0x74)",result,0);//2016-6-8手动模式，停止按键后，电流先几个周波停止，电压后停止  0x04电压通道
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

void Output_signal_Auto(int fd)
  {

    artExec(fd,"out:dig:off(0x70)",result,0);//IC放大器对应DB7
    if(fabs(m_Ch[4].OutputValue)>0.0001||m_Ch[4].checked==1)
       artExec(fd,"out:dig:on(0x20)",result,0);//IA�Ŵ�����ӦDB2

    if(fabs(m_Ch[5].OutputValue)>0.0001||m_Ch[5].checked==1)
       artExec(fd,"out:dig:on(0x20)",result,0);

    if(fabs(m_Ch[6].OutputValue)>0.0001||m_Ch[6].checked==1)
       artExec(fd,"out:dig:on(0x40)",result,0);

    float CStep1=0,CStep2=0,CStep3=0; //电流幅值步�
    float VStep1=0,VStep2=0,VStep3=0,VStep4=0; //电压幅值步�
    float PStep1=0,PStep2=0,PStep3=0,VPStep1=0,VPStep2=0,VPStep3=0,VPStep4=0;// 电流 电压相位步长
    float FStep1=0,FStep2=0,FStep3=0,VFStep1=0,VFStep2=0,VFStep3=0,VFStep4=0; //电流 电压频率步长

    if(fabs(m_Ch[0].Amp)>0.00001 || (m_Ch[0].checked==1 && A_Variable==1))  //2016-6-4自动测试�开始未报警修改
      m_Ch[0].Output=1;
    else
      m_Ch[0].Output=0;

    if(fabs(m_Ch[1].Amp)>0.00001 || (m_Ch[1].checked==1 && A_Variable==1))
      m_Ch[1].Output=1;
    else
      m_Ch[1].Output=0;

    if(fabs(m_Ch[2].Amp)>0.00001 || (m_Ch[2].checked==1 && A_Variable==1))
      m_Ch[2].Output=1;
    else
      m_Ch[2].Output=0;

    if(fabs(m_Ch[3].Amp)>0.00001 || (m_Ch[3].checked==1 && A_Variable==1))
          m_Ch[3].Output=1;
        else
          m_Ch[3].Output=0;

    if(fabs(m_Ch[4].Amp)>0.00001 || (m_Ch[4].checked==1 && A_Variable==1))  //2016-6-4自动测试�开始未报警修改
      m_Ch[4].Output=1;
    else
      m_Ch[4].Output=0;

    if(fabs(m_Ch[5].Amp)>0.00001 || (m_Ch[5].checked==1 && A_Variable==1))
      m_Ch[5].Output=1;
    else
      m_Ch[5].Output=0;

    if(fabs(m_Ch[6].Amp)>0.00001 || (m_Ch[6].checked==1 && A_Variable==1))
      m_Ch[6].Output=1;
    else
      m_Ch[6].Output=0;

  change_timedata=A_StepTime;

    artExec(fd,"inp:dig:interval(1)",result,0);
    artExec(fd,"out:ana:user:mem?",result,0);
    artExec(fd,"seq:brk",result,0);
    artExec(fd,"inp:buf:clr",result,0);
    artExec(fd,"seq:clr",result,0);
    artExec(fd,"seq:begin",result,0);

    artExec(fd,"inp:buf:sam(bin,1)",result,0);
    out_time=-1;
    artExec(fd,"seq:wait(0,1)",result,0);//第一�

    switch(A_Variable)
      {
          case 1:
              if(m_Ch[0].checked==1)
                VStep1=A_VolStep;
              if(m_Ch[1].checked==1)
                VStep2=A_VolStep;
              if(m_Ch[2].checked==1)
                VStep3=A_VolStep;
              if(m_Ch[3].checked==1)
                VStep4=A_VolStep;


              if(m_Ch[4].checked==1)
                CStep1=A_CurStep;
              if(m_Ch[5].checked==1)
                CStep2=A_CurStep;
              if(m_Ch[6].checked==1)
                CStep3=A_CurStep;
              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;
              //相位步长
              PStep1=0;
              PStep2=0;
              PStep3=0;


              VPStep1=0;
              VPStep2=0;
              VPStep3=0;
              VPStep4=0;
              //频率步长
              VFStep1=0;
              VFStep2=0;
              VFStep3=0;
              VFStep4=0;

              FStep1=0;
              FStep2=0;
              FStep3=0;

              break;
          case 2:
              if(m_Ch[0].checked==1)
                VPStep1=A_PhaStep;
              if(m_Ch[1].checked==1)
                VPStep2=A_PhaStep;
              if(m_Ch[2].checked==1)
                VPStep3=A_PhaStep;
              if(m_Ch[3].checked==1)
                VPStep4=A_PhaStep;


              if(m_Ch[4].checked==1)
                PStep1=A_PhaStep;
              if(m_Ch[5].checked==1)
                PStep2=A_PhaStep;
              if(m_Ch[6].checked==1)
                PStep3=A_PhaStep;
              //幅度步长
              VStep1=0;
              VStep2=0;
              VStep3=0;
              VStep4=0;

              CStep1=0;
              CStep2=0;
              CStep3=0;
              //频率步长
              VFStep1=0;
              VFStep2=0;
              VFStep3=0;
              VFStep4=0;

              FStep1=0;
              FStep2=0;
              FStep3=0;
              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;

              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;

              break;
          case 3:
              if(m_Ch[0].checked==1)
                VFStep1=A_FreStep;
              if(m_Ch[1].checked==1)
                VFStep2=A_FreStep;
              if(m_Ch[2].checked==1)
                VFStep3=A_FreStep;

              if(m_Ch[3].checked==1)
                VFStep4=A_FreStep;


              if(m_Ch[4].checked==1)
                FStep1=A_FreStep;
              if(m_Ch[5].checked==1)
                FStep2=A_FreStep;
              if(m_Ch[6].checked==1)
                FStep3=A_FreStep;

              VStep1=0;
              VStep2=0;
              VStep3=0;
              VStep4=0;
              CStep1=0;
              CStep2=0;
              CStep3=0;
              //相位
              PStep1=0;
              PStep2=0;
              PStep3=0;
              VPStep1=0;
              VPStep2=0;
              VPStep3=0;
              VPStep4=0;

              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;
              break;
          }

              ChangeOutputValue();
              channel_input(fd,0,m_Ch[4].OutputValue,m_Ch[4].Phase,m_Ch[4].Freq/FreqTriple,m_Ch[4].DC,1,CStep1,PStep1,FStep1/2);
              artExec(fd,"others:ana:i(1:1):rel_phase(0)",result,0);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:i(1:1):on",result,0);
              channel_input(fd,0,m_Ch[5].OutputValue,m_Ch[5].Phase,m_Ch[5].Freq/FreqTriple,m_Ch[5].DC,2,CStep2,PStep2,FStep2/2);
              artExec(fd,"others:ana:i(1:2):rel_phase(0)",result,0);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:i(1:2):on",result,0);
              channel_input(fd,0,m_Ch[6].OutputValue,m_Ch[6].Phase,m_Ch[6].Freq/FreqTriple,m_Ch[6].DC,3,CStep3,PStep3,FStep3/2);
              artExec(fd,"others:ana:i(1:3):rel_phase(0)",result,0);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:i(1:3):on",result,0);

              artExec(fd,"others:ana:v(1:1):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[0].OutputValue,m_Ch[0].Phase,m_Ch[0].Freq/FreqTriple,m_Ch[0].DC,1,VStep1,VPStep1,VFStep1/2);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:v(1:1):on",result,0);
              artExec(fd,"others:ana:v(1:2):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[1].OutputValue,m_Ch[1].Phase,m_Ch[1].Freq/FreqTriple,m_Ch[1].DC,2,VStep2,VPStep2,VFStep2/2);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:v(1:2):on",result,0);
              artExec(fd,"others:ana:v(1:3):rel_phase(0)",result,0);
              channel_input(fd,1,m_Ch[2].OutputValue,m_Ch[2].Phase,m_Ch[2].Freq/FreqTriple,m_Ch[2].DC,3,VStep3,VPStep3,VFStep3/2);
              artExec(fd,"others:steptime(1)",result,0);
              artExec(fd,"out:ana:v(1:3):on",result,0);

              if(VariatyVz2==1)
               {
                channel_input(fd,1,m_Ch[3].OutputValue,m_Ch[3].Phase,m_Ch[3].Freq/FreqTriple,m_Ch[3].DC,4,0,0,0);
               }
               else
                {
                artExec(fd,"others:ana:v(1:4):rel_phase(0)",result,0);
                channel_input(fd,1,m_Ch[3].OutputValue,m_Ch[3].Phase,m_Ch[3].Freq/FreqTriple,m_Ch[3].DC,4,0,0,0/*VStep4,VPStep4,VFStep4/2*/);//2016-7-2  vz屏蔽，自动变化出值修�
                artExec(fd,"others:steptime(1)",result,0);
                }
                artExec(fd,"out:ana:v(1:4):on",result,0);



    artExec(fd,"inp:buf:sam(bin,2)",result,0);
    out_time=fabs((A_StepNum+1)*A_StepTime);
    //artExec(fd,"seq:wait(orbin(1),2)",result,0);//第二�
    artExec(fd,"seq:wait(orbin(1),bin(1),2,1,0,1)",result,0);


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
