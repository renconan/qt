#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <KEY.h>
#include <QDebug>
#include "Gpio.h"

#include "Config.h"
#include "Testcom2.h"
#include "thread1.h"
#include "Dfdtseq.h"
#include "Defpam.h"
#include "em335x_drivers.h"
#include "Uartset.h"
#include "Testcom2.h"
#include <termios.h>



const float EPSINON =1.0e-05;
TEMPRORARYDATA *temprorarydata;
extern float prefaulttime;

extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
//extern variate define
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
extern float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
extern long  auto_i,IRQcnt,auto_count,tice;
extern int   input_par,HelpFlag,starttest1;


extern int   cx,cy,cx_start,cx_mid,cx_end,starttest,low,row,page;
extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;
extern int   seq_count,TestItem1,TestItem2,TestItem3,TestItem4,TestItem5;
extern float ChangeValue,FixedVoltage,dv,dt,FixedVT;
extern float StartVoltage,EndVoltage,ActStep,StepTime;
extern float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
extern float TripTime1,TripTime2,TripTime3,TripTime4,TripTime5,TripVoltage,TimeVoltage;

extern float LogicResolution,SystemFrec,Phase,SystemVolt;
extern float timeout;
extern int save_f;
extern int sys[4];
extern char ParStr[20];  //ParStr:为参数输入时记录的输入参数的字符串。
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

extern float VzOutput3Uo,VzOutputAc,VzOutputDc;
extern float V_coarse_step,V_fine_step,I_coarse_step,I_fine_step,F_coarse_step,F_fine_step;


extern float faultduration;
extern float ReturnValue;


extern float ChangeValue;


extern float faultduration;
extern float prefaulttime;
extern float postfaulttime;

extern float StartVoltage,EndVoltage;
extern float ChangeValue;

extern float dvdtStopVoltage;  //dvdt的停止电压
extern float dvdtstart;
extern float dvdtend;
extern float dvdtStep;
extern float currentdvdt;
extern int Variable; //1 VAB 2 VBC 3 VCA
extern float dvdttripvalue;



extern  float StopVoltage_triptime;
extern float dvdt_triptime;
extern float holdtime_triptime;




extern float VStart,VStop,VStep,VSteptime;
extern float StopVoltage;
extern float StopVoltageHoldTime;

extern float TripValue,TripValue1;


extern bool SoftStartEndflag;

float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;


int TripValueSteps=0;
int ReturnSteps=0;
extern bool Threadrunstate;


extern double V1ampPerStepError;  //电压VA每步变化的误差值
extern double V2ampPerStepError;  //电压VB每步变化的误差值
extern double V3ampPerStepError;  //电压VB每步变化的误差值
extern double VzampPerStepError;  //电压VZ每步变化的误差值

extern double I1ampPerStepError;
extern double I2ampPerStepError;
extern double I3ampPerStepError;

extern double max_output_v_a;   //,max_f_v_a=50,min_f_v_a=0.5,output_offset_v_a=0,amp_v_a=360;
extern double max_output_v_b;   //,max_f_v_b=50,min_f_v_b=0.5,output_offset_v_b=0,amp_v_b=120;
extern double max_output_v_c;   //,max_f_v_c=50,min_f_v_c=0.5,output_offset_v_c=0,amp_v_c=240;
extern double max_output_v_z;
extern double max_output_i_a;//,max_f_i_a=50,min_f_i_a=0.5,output_offset_i_a=0,amp_i_a=75;
extern double max_output_i_b;//,max_f_i_b=50,min_f_i_b=0.5,output_offset_i_b=0,amp_i_b=195;
extern double max_output_i_c;//,max_f_i_c=50,min_f_i_c=0.5,output_offset_i_c=0,amp_i_c=315;

float changedate;


thread1::thread1(QObject *parent) :
    QThread(parent)
{

    temprorarydata=new TEMPRORARYDATA;
}

void thread1::DisplayResult(int ReslutSum)
{
   emit SIG_testresult(ReslutSum);      
}


void thread1::ReadAutoLogic(int fd)
{ 
       int t1=0,t2=0,t3=0,t4=0,t5=0;
       unsigned char signature;
       int stepnum=0;

       static double SequencerTime2,SequencerTime4;
       double u,v,w;
       float VDS,Temp;
       artExec(fd,"inp:buf:get?",result,15);//从DSP得到开关量接点信息。
       u=result[7];
       v=result[8];
       w=result[9];
       signature=int(result[13]&0x7f);
       switch(signature)
         {
           case 1:
             t1=1;
             break;
           case 2:
             t2=1;
             SequencerTime2=gettime(u,v,w);//计算从开始测试到sequencer的Step2的时间。
             break;
           case 3:
             t3=1;
             break;
           case 4:
             SequencerTime4=gettime(u,v,w);//计算从开始测试到sequencer的Step4的时间。
             t4=1;
             break;
           case 5:
             t5=1;
             break;
         }
       if(t1==1)
          {

           switch(seq_count)
           {
           case 1:
                emit SIG_DisplayChangeValue(0,0);
                break;
           case 2:
                break;
            }
          }

       if(t2==1)
       {
        artExec(fd,"seq:status?(step)",result,16);
           stepnum=result[9]*256+result[10];

        if(stepnum>32767)
            stepnum=32768-stepnum;
        TripValueSteps=result[11]*256+result[12];
        if(TripValueSteps>32767)
            TripValueSteps=32768-TripValueSteps;
        if(TestItem2==1||TestItem1==1)
        {
             ReturnSteps=result[13]*256+result[14];
             if(ReturnSteps>32767)
                 ReturnSteps=32768-ReturnSteps;
        }
        switch(seq_count)
        {
        case 1:
            //changedate=VStart;
            if(TripValueSteps<=stepnum)
            {
                if(VStart>VStop)
                     ChangeValue=VStart-VStep*(stepnum);
                else if(VStart<VStop)
                     ChangeValue=VStart+VStep*(stepnum);
                emit SIG_DisplayChangeValue(1,ChangeValue);
                changedate=ChangeValue;
            }
            else if(TripValueSteps>=(fabs(VStart-VStop)/VStep))
            {
                stepnum=fabs(VStart-VStop)/VStep;
                if(VStart>VStop)
                     ChangeValue=VStart-VStep*(stepnum);
                else if(VStart<VStop)
                     ChangeValue=VStart+VStep*(stepnum);
                emit SIG_DisplayChangeValue(1,ChangeValue);
                changedate=ChangeValue;
            }
            else if(stepnum==0)
            {
                emit SIG_DisplayChangeValue(1,VStart);
            }
            else
            {
                if(VStart<VStop)
                    ChangeValue=changedate-(TripValueSteps-ReturnSteps)*VStep;
                else if(VStart>VStop)
                    ChangeValue=changedate+(TripValueSteps-ReturnSteps)*VStep;
                emit SIG_DisplayChangeValue(1,ChangeValue);
            }
//            if(VStart>VStop)
//            ChangeValue=VStart-(VStep)*stepnum;
//            else
//            ChangeValue=VStart+(VStep)*stepnum;
//            emit SIG_DisplayChangeValue(1,ChangeValue);

            break;
        case 2:
            break;
        }
       }


       if(t3==1)
         {
           switch(seq_count)
             {
               case 1:
                 TripTime1=-1;

                 break;
               case 2:
                 break;
               case 3:
                 TripTime3=-1;
                 DisplayResult(3);
                 break;
               case 4:
                 TripTime4=-1;
                 break;
               case 5:
                 TripTime5=-1;

                 break;
             }
         }
       if(t4==1)
         {
           switch(seq_count)
             {
              case 1:
                 TripTime1=1;

                 break;
             case 2:

                 break;
             case 3:
                 TripTime3=SequencerTime4-prefaulttime-LogicResolution/1000;
                 Temp=TripTime3*1000;
                 Temp=ceil(Temp);
                 TripTime3=Temp/1000;
                 DisplayResult(3);
                 break;

               case 4:
                 TripTime4=1;
                 if(dvdtstart>dvdtend)
                 dvdttripvalue=currentdvdt+dvdtStep;
                 else
                 dvdttripvalue=currentdvdt-dvdtStep;

                 break;
               case 5:
                 TripTime5=SequencerTime4-prefaulttime-faultduration-LogicResolution/1000;
                 Temp=TripTime2*1000;
                 Temp=ceil(Temp);
                 TripTime2=Temp/1000;//将返回值的限制在小数点后最多只有两位

                 break;
             }
         }
       if(t5==1)
         {
           switch(seq_count)
             {
               case 1:
                 if(TripTime1==1)
                 {
                     if(VStart>VStop)
                       TripValue=VStart-TripValueSteps*(VStep);
                     else
                       TripValue=VStart+TripValueSteps*(VStep);
                     DisplayResult(1);
                       if(TestItem2==1)
                       {
                         if(ReturnSteps==0)
                           TripTime2=-1;
                         else
                           TripTime2=1;
                         if(VStart>VStop)
                           ReturnValue=VStart-ReturnSteps*(VStep);
                         else
                           ReturnValue=VStart+ReturnSteps*(VStep);
                         DisplayResult(2);
                       }
                 }
                 else
                 {
                    DisplayResult(1);
                 }

                 output_signal_end(fd);
                 starttest=0;
                 emit SIG_DisplayChangeValue(0,0);
                 GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //?
                 save_f=1;
                 break;
               case 2:
                 break;
               case 3:
                 output_signal_end(fd);
                 starttest=0;
                 emit SIG_DisplayChangeValue(0,0);
                 GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                 save_f=1;
                 break;

               case 4:
                 if(TripTime4==-1)
                  {
                      if(dvdtstart>=dvdtend)
                      {
                          if(currentdvdt>=dvdtend)
                          {

                             // output_signal_end(fd);//关闭各个放大器
                              artExec(fd,"out:dig:on(0x04)",result,0);
                              emit SIG_DisplayChangeValue(1,currentdvdt);
                              artExec(fd,"out:dig:on(0x08)",result,0);
                              switch(Variable)
                              {
                              case 1:
                                  V1Output=1;
                                  StartVoltage=SystemVolt;
                                  break;
                              case 2:
                                  V2Output=1;
                                  StartVoltage=SystemVolt;
                                  break;
                              case 3:
                                  V3Output=1;
                                  StartVoltage=SystemVolt;
                                  break;
                              case 4:
                                  V1Output=1;
                                  V2Output=1;
                                  V3Output=0;
                                  StartVoltage=SystemVolt*1.732;
                                  break;
                              case 5:
                                  V1Output=0;
                                  V2Output=1;
                                  V3Output=1;
                                  StartVoltage=SystemVolt*1.732;
                                  break;
                              case 6:
                                  V1Output=1;
                                  V2Output=0;
                                  V3Output=1;
                                  StartVoltage=SystemVolt*1.732;
                                  break;
                              case 7:
                                  V1Output=1;
                                  V2Output=1;
                                  V3Output=1;
                                  StartVoltage=SystemVolt*1.732;
                                  break;
                              }

                              EndVoltage=dvdtStopVoltage;
                              OutputSequencer4(fd);//送测试滑差闭锁的sequencer
                              starttest=1;
                              currentdvdt=currentdvdt-dvdtStep;
                              return ;
                          }
                          else
                          {
                              DisplayResult(4);
                              output_signal_end(fd);//关闭各个放大器
                              starttest=0; //整个测试结束
                              GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);

                              save_f=1;
                              emit SIG_DisplayChangeValue(0,0);
                          }
                      }


                      else
                      {
                          if(currentdvdt<=dvdtend)
                          {
                              output_signal_end(fd);//关闭各个放大器
                              artExec(fd,"out:dig:on(0x04)",result,0);
                              emit SIG_DisplayChangeValue(1,currentdvdt);
                              artExec(fd,"out:dig:on(0x08)",result,0);
                              switch(Variable)
                              {
                              case 1:
                                  V1Output=1;
                                  StartVoltage=SystemVolt;
                                  break;
                              case 2:
                                  V2Output=1;
                                  StartVoltage=SystemVolt;
                                  break;
                              case 3:
                                  V3Output=1;
                                  StartVoltage=SystemVolt;
                                  break;
                              case 4:
                                  V1Output=1;
                                  V2Output=1;
                                  V3Output=0;
                                  StartVoltage=SystemVolt*1.732;
                                  break;
                              case 5:
                                  V1Output=0;
                                  V2Output=1;
                                  V3Output=1;
                                  StartVoltage=SystemVolt*1.732;
                                  break;
                              case 6:
                                  V1Output=1;
                                  V2Output=0;
                                  V3Output=1;
                                  StartVoltage=SystemVolt*1.732;
                                  break;
                              case 7:
                                  V1Output=1;
                                  V2Output=1;
                                  V3Output=1;
                                  StartVoltage=SystemVolt*1.732;
                                  break;
                              }

                              EndVoltage=dvdtStopVoltage;
                              OutputSequencer4(fd);
                              starttest=1;
                              currentdvdt=currentdvdt+dvdtStep;
                              return ;
                          }
                          else
                          {
                              DisplayResult(4);
                              output_signal_end(fd);
                              starttest=0;
                              save_f=1;
                              GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);

                              emit SIG_DisplayChangeValue(0,0);
                          }
                      }
                  }

                 else
                 {

                     DisplayResult(4);
                     output_signal_end(fd);
                     starttest=0;
                     save_f=1;
                     GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                     emit SIG_DisplayChangeValue(0,0);
                 }

                 break;
               case 5:
                 DisplayResult(5);
                 output_signal_end(fd);
                 starttest=0;
                 emit SIG_DisplayChangeValue(0,0);
                 GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                 save_f=1;
                 break;
             }
         }
}

int  thread1::Buzzer(char Address1,char Address2,char Address3)
  {
    if(sys[1]==1)
      {
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1¹ýÔØ
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2¹ýÔØ
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3¹ýÔØ
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1¹ýÔØ
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2¹ýÔØ
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3¹ýÔØ
        if((Address3&0x20)!=0x00)
          return 1;//Ö±Á÷¹ýÔØ
      }
    else if(sys[1]==0)
      {//¹ýÔØÐÅÏ¢
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1¹ýÔØ
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2¹ýÔØ
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3¹ýÔØ
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1¹ýÔØ
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2¹ýÔØ
        if((Address3&0x40)!=0x00&&V3Output==1)
          return -1;//V3¹ýÔØ
        if((Address3&0x20)!=0x00)
          return 1;//Ö±Á÷¹ýÔØ
      }
    if(sys[2]==1)
      {//ŒäÐªÐÅÏ¢
        if((Address1&0x10)!=0x00)
          return 1;//I1ŒäÐª
        if((Address1&0x04)!=0x00)
          return 1;//I2ŒäÐª
        if((Address1&0x01)!=0x00)
          return 1;//I3ŒäÐª
        if((Address2&0x08)!=0x00)
          return 1;//V1ŒäÐª
        if((Address2&0x04)!=0x00)
          return 1;//V2ŒäÐª
        if((Address3&0x80)!=0x00)
          return 1;//V3ŒäÐª
      }
    else if(sys[2]==0)
      {//ŒäÐªÐÅÏ¢
        if((Address1&0x10)!=0x00)
          return -1;//I1ŒäÐª
        if((Address1&0x04)!=0x00)
          return -1;//I2ŒäÐª
        if((Address1&0x01)!=0x00)
          return -1;//I3ŒäÐª
        if((Address2&0x08)!=0x00)
          return -1;//V1ŒäÐª
        if((Address2&0x04)!=0x00)
          return -1;//V2ŒäÐª
        if((Address3&0x80)!=0x00)
          return -1;//V3ŒäÐª
      }
    if(sys[3]==1)
      {//±ÕËøÐÅÏ¢
        if((Address1&0x80)!=0x00)
          return 1;//I1±ÕËø
        if((Address1&0x40)!=0x00)
          return 1;//I2±ÕËø
        if((Address1&0x20)!=0x00)
          return 1;//I3±ÕËø
        if((Address2&0x20)!=0x00)
          return 1;//V1±ÕËø
        if((Address2&0x40)!=0x00)
          return 1;//V2±ÕËø
        if((Address2&0x01)!=0x00)
          return 1;//V3±ÕËø
      }
    else if(sys[3]==0)
      {//±ÕËøÐÅÏ¢
        if((Address1&0x80)!=0x00)
          return -1;//I1±ÕËø
        if((Address1&0x40)!=0x00)
          return -1;//I2±ÕËø
        if((Address1&0x20)!=0x00)
          return -1;//I3±ÕËø
        if((Address2&0x20)!=0x00)
          return -1;//V1±ÕËø
        if((Address2&0x40)!=0x00)
          return -1;//V2±ÕËø
        if((Address2&0x01)!=0x00)
          return -1;//V3±ÕËø
      }
    return 0;
  }


void thread1::logic_info(int fd,char info_result1,char info_result2,char info_result3)
  {

    static int info_count=1;
    int  k1=0;
    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);
    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //²»ÐèÒª¿ØŒþ
          artExec(fd,"out:dig:on(0x80)",result,0);
        else
          artExec(fd,"out:dig:off(0x80)",result,0);
      }

    if(Buzzer(info_result1,info_result2,info_result3)==0) //²»ÐèÒª¿ØŒþ
      {
        //ÔËÐÐÕý³£
          emit SIG_dspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB9~DB15¡£
           emit SIG_dspstate(1);
            //IA¹ýÔØ
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
            emit SIG_dspstate(2);

            //IABC ÎÂ¶È±£»€
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //ICŒäÐª
              emit SIG_dspstate(3);
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==2)
      {
        if((info_result1&0x02)==0x02&&I2Output==1)
          {  //info_result1ÊÇµÚ1Â·µØÖ·µÄDB9~DB15¡£

              emit SIG_dspstate(4);
            //IB¹ýÔØ
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB8~DB15¡£

              emit SIG_dspstate(5);
           //IC±ÕËø
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB9~DB15¡£
              emit SIG_dspstate(6);
          //IAŒäÐª
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==3)
      {
        if((info_result2&0x80)==0x80&&I3Output==1)
          { //info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
           //IC¹ýÔØ
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB8~DB15¡£
          //IABC µçÔŽ±£»€
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB9~DB15¡£
              emit SIG_dspstate(9);
          //I2ŒäÐª  ?ÎÂ¶È±£»€ŒäÐª
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==4)
      {
        if((info_result2&0x10)==0x10&&V1Output==1)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
           //VA¹ýÔØ
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB8~DB15¡£
              emit SIG_dspstate(11);
              //V2±ÕËø
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3ÊÇµÚ2Â·µØÖ·µÄDB0~DB7¡£

              //V3ŒäÐª
              emit SIG_dspstate(12);
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==5)
      {
        if((info_result2&0x02)==0x02&&V2Output==1)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
            //V2¹ýÔØ
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB8~DB15¡£  //DB8
           // VABC ÎÂ¶È±£»€
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
            emit SIG_dspstate(15);
           //VAŒäÐª
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }

    else if(info_count==6)
      {
        if((info_result3&0x40)==0x40&&V3Output==1)
          { //info_result3ÊÇµÚ2Â·µØÖ·µÄDB0~DB7¡£

             emit SIG_dspstate(16);
            //VC¹ýÔØ
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB8~DB15¡£  //DB13
            emit SIG_dspstate(17);
            //VABCµçÔŽ±£»€
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
            emit SIG_dspstate(18);


          //Vz¹ýÔØ

            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==7)
      {
        if((info_result3&0x20)==0x20)
          {//info_result3ÊÇµÚ2Â·µØÖ·µÄDB0~DB7¡£
            emit SIG_dspstate(19);

             //Ö±Á÷¹ýÔØ
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB8~DB15¡£
          emit SIG_dspstate(20);

            // VABC ÎÂ¶È±£»€
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3ÊÇµÚ2Â·µØÖ·µÄDB0~DB7¡£
            emit SIG_dspstate(21);
           //V3ŒäÐª
            k1=1;
          }
        if(k1==1)
          {
            info_count=1;
            return;
          }
        else
          info_count=1;
      }

  }


void thread1::CaculateError(){

    float y;
    y=chang_amp(VStep,&max_output_v_a);
    V1ampPerStepError=VStep-(double)(max_output_v_a*y/32767);

    y=chang_amp(VStep,&max_output_v_b);
    V2ampPerStepError=VStep-(double)(max_output_v_b*y/32767);

    y=chang_amp(VStep,&max_output_v_c);
    V3ampPerStepError=VStep-(double)(max_output_v_c*y/32767);

}






void thread1::key_deal_test_voltage(int fd)
{

        if(starttest==0)
          {

              emit SIG_testresult(0);
              emit SIG_DisplayChangeValue(0,0);
              if(TestItem1==1)
              {

                  V1Output=0;
                  V2Output=0;
                  V3Output=0;
                  I1Output=0;
                  I2Output=0;
                  I3Output=0;

                switch(Variable)
                 {
                   case 1:
                      V1Output=1;
                       break;
                   case 2:
                      V2Output=1;
                       break;
                   case 3:
                      V3Output=1;
                       break;
                   case 4:
                      V1Output=1;
                      V2Output=1;
                       break;
                   case 5:
                      V2Output=1;
                      V3Output=1;
                       break;
                   case 6:
                      V1Output=1;
                      V3Output=1;
                       break;
                   case 7:
                     V1Output=1;
                     V3Output=1;
                     V2Output=1;
                     break;

                  }

               GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
               artExec(fd,"out:dig:on(0x04)",result,0);
               seq_count=1;
               ChangeValue=VStart;
               artExec(fd,"out:dig:on(0x08)",result,0);
               OutputSequencer1(fd);

               starttest=1;
              }

              else if(TestItem1==0&&TestItem3==1)
              {
                  V1Output=0;
                  V2Output=0;
                  V3Output=0;
                  I1Output=0;
                  I2Output=0;
                  I3Output=0;

               switch(Variable)
                 {
                   case 1:
                      V1Output=1;
                       break;
                   case 2:
                      V2Output=1;
                       break;
                   case 3:
                      V3Output=1;
                       break;
                   case 4:
                      V1Output=1;
                      V2Output=1;
                       break;
                   case 5:
                      V2Output=1;
                      V3Output=1;
                       break;
                   case 6:
                      V1Output=1;
                      V3Output=1;
                       break;
                   case 7:
                     V1Output=1;
                     V3Output=1;
                     V2Output=1;
                     break;

                  }

               ChangeValue=StopVoltage;
               TripValue1=StopVoltage;
               starttest=1;
               seq_count=3;
               GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
               artExec(fd,"out:dig:on(0x08)",result,0);
               artExec(fd,"out:dig:on(0x04)",result,0);
               OutputSequencer3(fd);
               emit SIG_DisplayChangeValue(1,ChangeValue);
              }


              else if(TestItem1==0&&TestItem3==0&&TestItem4==1)
              {
                  starttest=1;
                  seq_count=4;
                  GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
                  V1Output=0;
                  V2Output=0;
                  V3Output=0;
                  switch(Variable)
                  {
                  case 1:
                      V1Output=1;
                      StartVoltage=SystemVolt;
                      break;
                  case 2:
                      V2Output=1;
                      StartVoltage=SystemVolt;
                      break;
                  case 3:
                      V3Output=1;
                      StartVoltage=SystemVolt;
                      break;
                  case 4:
                      V1Output=1;
                      V2Output=1;
                      V3Output=0;
                      StartVoltage=SystemVolt*1.732;
                      break;
                  case 5:
                      V1Output=0;
                      V2Output=1;
                      V3Output=1;
                      StartVoltage=SystemVolt*1.732;
                      break;
                  case 6:
                      V1Output=1;
                      V2Output=0;
                      V3Output=1;
                      StartVoltage=SystemVolt*1.732;
                      break;
                  case 7:
                      V1Output=1;
                      V2Output=1;
                      V3Output=1;
                      StartVoltage=SystemVolt*1.732;
                      break;
                  }


                  EndVoltage=dvdtStopVoltage;
                  artExec(fd,"out:dig:on(0x04)",result,0);
                  currentdvdt=dvdtstart;
                  emit SIG_DisplayChangeValue(1,currentdvdt);
                  artExec(fd,"out:dig:on(0x08)",result,0);
                  OutputSequencer4(temprorarydata->UartDev[0].uartfd);
                  if(dvdtstart>dvdtend)
                  currentdvdt=currentdvdt-dvdtStep;
                  else
                  currentdvdt=currentdvdt+dvdtStep;
              }
              else if(TestItem1==0&&TestItem3==0&&TestItem4==0&&TestItem5==1)
              {
                  GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
                  V1Output=0;
                  V2Output=0;
                  V3Output=0;
                  switch(Variable)
                  {
                  case 1:
                      V1Output=1;
                       StartVoltage=SystemVolt;
                      break;
                  case 2:
                      V2Output=1;
                       StartVoltage=SystemVolt;
                      break;
                  case 3:
                      V3Output=1;
                       StartVoltage=SystemVolt;
                      break;
                  case 4:
                      V1Output=1;
                      V2Output=1;
                      V3Output=0;
                       StartVoltage=SystemVolt*1.732;
                      break;
                  case 5:
                      V1Output=0;
                      V2Output=1;
                      V3Output=1;
                      StartVoltage=SystemVolt*1.732;
                      break;
                  case 6:
                      V1Output=1;
                      V2Output=0;
                      V3Output=1;
                      StartVoltage=SystemVolt*1.732;
                      break;
                  case 7:
                      V1Output=1;
                      V2Output=1;
                      V3Output=1;
                      StartVoltage=SystemVolt*1.732;
                      break;
                  }

                  starttest=1;
                  seq_count=5;

                  EndVoltage=StopVoltage_triptime;
                  emit SIG_DisplayChangeValue(1,dvdt_triptime);
                  artExec(fd,"out:dig:on(0x04)",result,0);
                  artExec(fd,"out:dig:on(0x08)",result,0);
                  OutputSequencer5(temprorarydata->UartDev[0].uartfd);
              }


          }
}


void thread1::key_deal_stop_voltage(int fd)
{
         if(starttest==1)
          {
            output_signal_end(fd);
            starttest=0;
            emit SIG_DisplayChangeValue(0,0);
          }
}


void thread1::key_response(KEY key)
{
    if(key==_NOP) return;

 if(key==_TEST)
{
    if(starttest==0)
     {
        output_signal_end(temprorarydata->UartDev[0].uartfd);
        key_deal_test_voltage(temprorarydata->UartDev[0].uartfd);
        CaculateError();
        emit SIG_TestState();

     }
}

else if(key==_STOP)
{
    GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
    key_deal_stop_voltage(temprorarydata->UartDev[0].uartfd);
    emit SIG_TestState();

}

}


void thread1::stoprun(){

  Threadrunstate=false;
}



void thread1::run()
{
     KEY key;
     int info_num=0;
     int rc=0;
     unsigned char info_flag1,info_flag2,info_flag3;
     temprorarydatainit();
     UartsInit();
     temprorarydata->gpio_fd= open("/dev/em335x_gpio", O_RDWR);
     rc = GPIO_OutEnable( temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
     if(rc < 0)
       {
          printf("GPIO_OutEnable::failed %d\n", rc);
          temprorarydata->gpio_fd=-1;
       }
     GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭

     online(temprorarydata->UartDev[0].uartfd);
     read_max(temprorarydata->UartDev[0].uartfd);
     artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);

       if((result[7]&0xf0)==0x60)
          { //125V电压板
            MAX_V_VALUEDATA_AC=300;
            MIN_V_VALUEDATA_AC=0;
            MAX_V_VALUEDATA_DC=424;
            MIN_V_VALUEDATA_DC=-424;
          }
         else if((result[7]&0xf0)==0x80)
        {
          MAX_V_VALUEDATA_AC=125;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=176;
          MIN_V_VALUEDATA_DC=-176;
        }
      else if((result[7]&0xf0)==0x00)
        {
          MAX_V_VALUEDATA_AC=75;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=110;
          MIN_V_VALUEDATA_DC=-110;
        }
      if((result[7]&0x0f)==0x01)
        {
          MAX_I_VALUEDATA_AC=30;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=42;
          MIN_I_VALUEDATA_DC=-42;
        }
      else if((result[7]&0x0f)==0x08)
        {

          MAX_I_VALUEDATA_AC=40;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=56;
          MIN_I_VALUEDATA_DC=-56;
        }
      else if((result[7]&0x0f)==0x05)
        {
          MAX_I_VALUEDATA_AC=50;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=70;
          MIN_I_VALUEDATA_DC=-70;
        }


   while(Threadrunstate)
    {
     if(temprorarydata->keypadopenflag==false&&temprorarydata->helpopenflag==false)
    { key=GetKey(temprorarydata->UartDev[1].uartfd);
      key_response(key);

      if(SoftStartEndflag==true)
           {
              if(starttest==1)
              {
                  key=_STOP;
              }
              else
              {
                  key=_TEST;
              }
              key_response(key);
              SoftStartEndflag=false;
           }

     if(starttest==0)
     {
         if(info_num++>=10)
        {
            emit SIG_TestState();
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //²»ÐèÒªžüžÄ
         info_flag1=result[8];
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
         info_flag2=result[8];
         info_flag3=result[9];   
         logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);
         info_num=0;
         }
         }
     else if(starttest==1)
     {
         if(starttest1==0)
         {

         ReadAutoLogic(temprorarydata->UartDev[0].uartfd);
         if(info_num++>=20&&HelpFlag==0)
          {
             artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //²»ÐèÒªžüžÄ
             info_flag1=result[8];
             artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
             info_flag2=result[8];
             info_flag3=result[9];
             logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);
             info_num=0;
          }
         }

 }
}
     else
     {

         tcflush(temprorarydata->UartDev[1].uartfd, TCIOFLUSH);
         msleep(500);
     }
}

          starttest=0;
          output_signal_end(temprorarydata->UartDev[0].uartfd);
          artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
          close(temprorarydata->UartDev[1].uartfd);
          close(temprorarydata->UartDev[0].uartfd);

}


