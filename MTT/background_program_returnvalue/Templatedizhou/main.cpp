#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <QDebug>
#include "Config.h"
#include "Curseq.h"
#include "Defpam.h"
#include "em335x_drivers.h"
#include "Gpio.h"
#include "Testcom2.h"
#include "Uartset.h"
#include "KEY.h"
#include "Variableformat.h"
extern TEMPRORARYDATA *temprorarydata;
float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
unsigned char result[70],receive_data[70];
double out_time,change_timedata;
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
int   FreqTriple=2;




int   starttest1;
int   starttest=0;
int   seq_count,V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;
int   TestItem1,TestItem2,TestItem3,TestItem4,TestItem5;



float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
float TripTime1,TripTime2,TripTime3,TripTime4,TripTime5=-1,TripFrepuency;
float ChangeValue,timeout;
int save_f;
int sys[4];
float LogicResolution,SystemFrec,SystemVolt;//2009-6-20
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;



 float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;


 int   testitem=1;
 float faultduration=10;
 float prefaulttime=3;  //故障前时间
 float postfaulttime=3; //故障后时间
 FRECURRENCESET FreqVar;

 int   dfdtcounter=0; //dfdt变化的次数 新的dfdt=dfdtstart-dfdtstep*dfdtcounter;
 float currentdfdt=0;//当前的dfdt值
 float currentvoltage=0; //当前的电压闭锁值
 int   Voltagecounter=0;  //电压的累加的计数值

 float CurrentFreq;

 float PrepareTime=3;
 int Firstrun=1;     //第一次运行


 void key_deal_test_current(int fd)
 {

     if(starttest==0)
       {

          char outchar[512];
           V1Output=1;
           V2Output=1;
           V3Output=1;
           I1Output=0;
           I2Output=0;
           I3Output=0;


           if(TestItem1==1)
            {
                seq_count=1;
                starttest=1;
                CurrentFreq=FreqVar.TripValue.FreEndValueStart.value;
                sprintf(outchar,"Freq(Hz):%.3f",CurrentFreq);
                fprintf(stdout,outchar);
                fflush(stdout);
                artExec(fd,"out:dig:on(0x04)",result,0);
                artExec(fd,"out:dig:on(0x08)",result,0);
                OutputSequencer1(fd);
                if(FreqVar.TripValue.FreEndValueStart.value>=\
                   FreqVar.TripValue.FreEndValueEnd.value)
                CurrentFreq=FreqVar.TripValue.FreEndValueStart.value-FreqVar.TripValue.FreStep.value;
                else
                CurrentFreq=FreqVar.TripValue.FreEndValueStart.value+FreqVar.TripValue.FreStep.value;

            }
           if(TestItem2==1)
           {
               starttest=1;
               seq_count=2;
               artExec(fd,"out:dig:on(0x04)",result,0);
               char outchar[512];
               sprintf(outchar,"End freq:%.3f",FreqVar.TripTime.FreEndValue.value);
               fprintf(stdout,outchar);
               fflush(stdout);
               artExec(fd,"out:dig:on(0x08)",result,0);
               OutputSequencer2(fd);

           }
           if(TestItem3==1)
           {
               starttest=1;
               seq_count=3;
               artExec(fd,"out:dig:on(0x04)",result,0);
               char outchar[512];
               sprintf(outchar,"dfdt:%.3f",currentdfdt);
               fprintf(stdout,outchar);
               fflush(stdout);
               artExec(fd,"out:dig:on(0x08)",result,0);
               OutputSequencer3(fd);
               if(FreqVar.Tripdfdt.DfdtStart.value>FreqVar.Tripdfdt.DfdtEnd.value) //若DFDT是下降的
              {
                currentdfdt=currentdfdt-FreqVar.Tripdfdt.DfdtStep.value;
              }
               else
               {
                 currentdfdt=currentdfdt+FreqVar.Tripdfdt.DfdtStep.value;
               }

           }
           if(TestItem4==1)
           {
               starttest=1;
               seq_count=4;
               artExec(fd,"out:dig:on(0x04)",result,0);
               char outchar[512];
               sprintf(outchar,"dfdt:%.3f duration:%.3f s",\
               FreqVar.Tripdfdttime.dfdt.value,\
               FreqVar.Tripdfdttime.Duration.value);
               fprintf(stdout,outchar);
               fflush(stdout);
               artExec(fd,"out:dig:on(0x08)",result,0);
               OutputSequencer4(fd);
           }

           if(TestItem5==1)
           {
               starttest=1;
               seq_count=5;
               output_signal_end(fd);
               artExec(fd,"out:dig:on(0x04)",result,0);

               char outchar[512];
               sprintf(outchar,"Voltage:%.3f",currentvoltage);
               fprintf(stdout,outchar);
               fflush(stdout);
               artExec(fd,"out:dig:on(0x08)",result,0);
               OutputSequencer5(fd);
               if(FreqVar.TripVBlock.VStart.value>FreqVar.TripVBlock.VEnd.value)
              {
                currentvoltage=currentvoltage-FreqVar.TripVBlock.VStep.value;
              }
              else
               {
                currentvoltage=currentvoltage+FreqVar.TripVBlock.VStep.value;
               }





           }
       }
 }


int  ReadAutoLogic(int fd)
 {
        int t2=0,t3=0,t4=0,t5=0,t6=0;
        unsigned char signature;
        static double SequencerTime2,SequencerTime4;
        double u,v,w;
        float VDS,Temp;
        char outchar[512];
        artExec(fd,"inp:buf:get?",result,15);//从DSP得到开关量接点信息。
        u=result[7];
        v=result[8];
        w=result[9];
        signature=int(result[13]&0x7f);
        switch(signature)
          {
            case 1:
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
            case 6:
              t6=1;
              break;
          }



        if(t3==1)
          {
            switch(seq_count)
              {
                case 1:          //动作频率
                  TripTime1=-1;
                  break;
                case 2:
                  TripTime2=-1; //dfdt闭锁
                  break;
                case 3:
                  TripTime3=-1; //动作时间
                  break;
                case 4:
                  TripTime4=-1; //电压闭锁值
                  break;
                case 5:

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
                  TripTime2=SequencerTime4-SequencerTime2-LogicResolution/1000;
                  Temp=TripTime2*1000;
                  Temp=ceil(Temp);
                  TripTime2=Temp/1000;//将返回值的限制在小数点后最多只有两位
                  break;
                case 3:
                  TripTime3=1;  //dfdt触发了
                  break;
                case 4:
                  TripTime4=SequencerTime4-SequencerTime2-faultduration-LogicResolution/1000;
                  Temp=TripTime4*1000;
                  Temp=ceil(Temp);
                  TripTime4=Temp/1000;//将返回值的限制在小数点后最多只有两位
                  break;
                case 5:
                   TripTime5=1;

                break;
              }
          }
        if(t5==1)
          {
            switch(seq_count)
              {
                case 1:
                 // output_signal_end(fd); //关闭各个放大器
                      starttest=0;           //整个测试结束
              {
                  float FENDSTART=FreqVar.TripValue.FreEndValueStart.value;
                  float FENDEND=FreqVar.TripValue.FreEndValueEnd.value;
                  float FSTEP=FreqVar.TripValue.FreStep.value;
                  if(FENDSTART>=FENDEND)
                 {

                      if(TripTime1==-1&&CurrentFreq>=FENDEND)
                  {
                         starttest=1;
                         sprintf(outchar,"Freq(Hz):%.3f",CurrentFreq);
                         fprintf(stdout,outchar);
                         fflush(stdout);
                         artExec(fd,"out:dig:on(0x08)",result,0);
                         artExec(fd,"out:dig:on(0x04)",result,0);
                         OutputSequencer1(fd);
                         CurrentFreq=CurrentFreq-FSTEP;

                  }
                  else
                  {
                       output_signal_end(fd); //关闭各个放大器
                       starttest=0;           //整个测试结束
                       save_f=1;//将保存结果标志置1
                  }
                 }
                  else
                  {

                      if(TripTime1==-1&&CurrentFreq<=FENDEND)
                      {
                          starttest=1;
                          sprintf(outchar,"Freq(Hz):%.3f",CurrentFreq);
                          fprintf(stdout,outchar);
                          fflush(stdout);
                          artExec(fd,"out:dig:on(0x08)",result,0);
                          artExec(fd,"out:dig:on(0x04)",result,0);
                          OutputSequencer1(fd);
                          CurrentFreq=CurrentFreq+FSTEP;
                      }
                      else
                      {
                           output_signal_end(fd); //关闭各个放大器
                           starttest=0;           //整个测试结束
                           save_f=1;//将保存结果标志置1
                      }
                  }

              }
                      break;
                case 2://测试动作时间处理
                      output_signal_end(fd);//关闭各个放大器
                      starttest=0; //整个测试结束
                      save_f=1;//将保存结果标志置1
                      break;

                case 3:
                   starttest=0;
                //   output_signal_end(fd);//先关闭各个放大器
                   if(FreqVar.Tripdfdt.DfdtStart.value>FreqVar.Tripdfdt.DfdtEnd.value) //若DFDT是下降的
                  {
                    if(currentdfdt>=FreqVar.Tripdfdt.DfdtEnd.value&&TripTime3==-1)
                   {

                    artExec(fd,"out:dig:on(0x04)",result,0);
                    artExec(fd,"out:dig:on(0x08)",result,0);
                    OutputSequencer3(fd);//送测试滑差闭锁的sequencer
                    starttest=1;
                    char outchar[512];
                    sprintf(outchar,"dfdt:%.3f",currentdfdt);
                    fprintf(stdout,outchar);
                    fflush(stdout);
                    currentdfdt=currentdfdt-FreqVar.Tripdfdt.DfdtStep.value;
                   }
                   else
                    {
                    output_signal_end(fd);//关闭各个放大器
                    starttest=0; //整个测试结束
                    save_f=1;//将保存结果标志置1
                    }
                  }
                  else if(FreqVar.Tripdfdt.DfdtStart.value<FreqVar.Tripdfdt.DfdtEnd.value)
                  {
                     if(currentdfdt<=FreqVar.Tripdfdt.DfdtEnd.value&&TripTime3==-1)
                     {
                         artExec(fd,"out:dig:on(0x04)",result,0);
                         artExec(fd,"out:dig:on(0x08)",result,0);
                         OutputSequencer3(fd);//送测试滑差闭锁的sequencer
                         starttest=1;
                         char outchar[512];
                         sprintf(outchar,"dfdt:%.3f",currentdfdt);
                         fprintf(stdout,outchar);
                         fflush(stdout);
                         currentdfdt=currentdfdt+FreqVar.Tripdfdt.DfdtStep.value;

                     }
                     else
                     {
                      output_signal_end(fd);//关闭各个放大器
                      starttest=0; //整个测试结束
                      save_f=1;//将保存结果标志置1
                     }
                  }
                   break;
            case 4:
                output_signal_end(fd);//关闭各个放大器
                starttest=0; //整个测试结束
                save_f=1;//将保存结果标志置1
                break;
            case 5:
                 TripTime5=-1;
                 break;

             }

        }

        if(t6==1)
        {

            starttest=0;
          //  output_signal_end(fd);//先关闭各个放大器
            if(FreqVar.TripVBlock.VStart.value>FreqVar.TripVBlock.VEnd.value)
           {
             if(currentvoltage>=FreqVar.TripVBlock.VEnd.value&&TripTime5==-1)
            {

             artExec(fd,"out:dig:on(0x04)",result,0);
             char outchar[512];
             sprintf(outchar,"Voltage:%.3f",currentvoltage);
             fprintf(stdout,outchar);
             fflush(stdout);
             artExec(fd,"out:dig:on(0x08)",result,0);
             OutputSequencer5(fd);//送测试滑差闭锁的sequencer
             starttest=1;

             currentvoltage=currentvoltage-FreqVar.TripVBlock.VStep.value;

            }
            else
             {
             output_signal_end(fd);//关闭各个放大器
             starttest=0; //整个测试结束
             save_f=1;//将保存结果标志置1
             }
           }
            else if(FreqVar.TripVBlock.VStart.value<FreqVar.TripVBlock.VEnd.value)
            {
              if(currentvoltage<=FreqVar.TripVBlock.VEnd.value&&TripTime5==-1)
             {

              artExec(fd,"out:dig:on(0x04)",result,0);

              char outchar[512];
              sprintf(outchar,"Voltage:%.3f",currentvoltage);
              fprintf(stdout,outchar);
              fflush(stdout);
              artExec(fd,"out:dig:on(0x08)",result,0);
              OutputSequencer5(fd);//送测试滑差闭锁的sequencer
              currentvoltage=currentvoltage+FreqVar.TripVBlock.VStep.value;
              starttest=1;
             }
             else
              {
              output_signal_end(fd);   //关闭各个放大器
              starttest=0;             //整个测试结束
              save_f=1;               //将保存结果标志置1
              }
            }

           }


}


int  Buzzer(char Address1,char Address2,char Address3)
  {//sys[1]:过载，sys[2]:间歇，sys[3]:闭锁
    if(sys[1]==1)
      {//过载信息
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1过载
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2过载
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3过载
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1过载
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2过载
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    else if(sys[1]==0)
      {//过载信息
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1过载
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2过载
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3过载
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1过载
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2过载
        if((Address3&0x40)!=0x00&&V3Output==1)
          return -1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    if(sys[2]==1)
      {//间歇信息
        if((Address1&0x10)!=0x00)
          return 1;//I1间歇
        if((Address1&0x04)!=0x00)
          return 1;//I2间歇
        if((Address1&0x01)!=0x00)
          return 1;//I3间歇
        if((Address2&0x08)!=0x00)
          return 1;//V1间歇
        if((Address2&0x04)!=0x00)
          return 1;//V2间歇
        if((Address3&0x80)!=0x00)
          return 1;//V3间歇
      }
    else if(sys[2]==0)
      {//间歇信息
        if((Address1&0x10)!=0x00)
          return -1;//I1间歇
        if((Address1&0x04)!=0x00)
          return -1;//I2间歇
        if((Address1&0x01)!=0x00)
          return -1;//I3间歇
        if((Address2&0x08)!=0x00)
          return -1;//V1间歇
        if((Address2&0x04)!=0x00)
          return -1;//V2间歇
        if((Address3&0x80)!=0x00)
          return -1;//V3间歇
      }
    if(sys[3]==1)
      {//闭锁信息
        if((Address1&0x80)!=0x00)
          return 1;//I1闭锁
        if((Address1&0x40)!=0x00)
          return 1;//I2闭锁
        if((Address1&0x20)!=0x00)
          return 1;//I3闭锁
        if((Address2&0x20)!=0x00)
          return 1;//V1闭锁
        if((Address2&0x40)!=0x00)
          return 1;//V2闭锁
        if((Address2&0x01)!=0x00)
          return 1;//V3闭锁
      }
    else if(sys[3]==0)
      {//闭锁信息
        if((Address1&0x80)!=0x00)
          return -1;//I1闭锁
        if((Address1&0x40)!=0x00)
          return -1;//I2闭锁
        if((Address1&0x20)!=0x00)
          return -1;//I3闭锁
        if((Address2&0x20)!=0x00)
          return -1;//V1闭锁
        if((Address2&0x40)!=0x00)
          return -1;//V2闭锁
        if((Address2&0x01)!=0x00)
          return -1;//V3闭锁
      }
    return 0;
  }


void showdspstate(int state)
{

    char outchar[512];
    switch(state)
    {
    case 0: strcpy(outchar,"WARNING=Run normally"); break;
    case 1: strcpy(outchar,"WARNING=IA overload"); break;
    case 2: strcpy(outchar,"WARNING=IABC temperature protect"); break;
    case 3: strcpy(outchar,"WARNING=IC resume"); break;
    case 4: strcpy(outchar,"WARNING=IB overload"); break;
    case 5: strcpy(outchar,"WARNING=IC closedown"); break;
    case 6: strcpy(outchar,"WARNING=IA resume"); break;
    case 7: strcpy(outchar,"WARNING=IC overload"); break;
    case 8: strcpy(outchar,"WARNING=IABC power protection"); break;
    case 9: strcpy(outchar,"WARNING=IB resume"); break;
    case 10: strcpy(outchar,"WARNING=VA overload"); break;

    case 12: strcpy(outchar,"WARNING=VC resume "); break;
    case 13: strcpy(outchar,"WARNING=VB overload "); break;
    case 14: strcpy(outchar,"WARNING=VABC temperature protection"); break;
    case 15: strcpy(outchar,"WARNING=VA resume"); break;
    case 16: strcpy(outchar,"WARNING=VC overload"); break;
    case 17: strcpy(outchar,"WARNING=VABC power protection"); break;
    case 18: strcpy(outchar,"WARNING=Vz overload"); break;
    case 19: strcpy(outchar,"WARNING=direct current overload"); break;
    case 20: strcpy(outchar,"WARNING=IABC power protection"); break;
    case 21: strcpy(outchar,"WARNING=VC resume"); break;
    }

    fprintf(stdout,outchar);
    fflush(stdout);
}



void logic_info(int fd,char info_result1,char info_result2,char info_result3)
  {

    static int info_count=1;
    int  k1=0;
    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);
    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //不需要控件
          artExec(fd,"out:dig:on(0x80)",result,0);
        else
          artExec(fd,"out:dig:off(0x80)",result,0);
      }

    if(Buzzer(info_result1,info_result2,info_result3)==0) //不需要控件
      {
        //运行正常
          showdspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1是第1路地址的DB9~DB15。
             showdspstate(1);
            //IA过载
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
              showdspstate(2);

            //IABC 温度保护
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //IC间歇
                showdspstate(3);
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
          {  //info_result1是第1路地址的DB9~DB15。

                showdspstate(4);
            //IB过载
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1是第1路地址的DB8~DB15。

                showdspstate(5);
           //IC闭锁
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1是第1路地址的DB9~DB15。
                showdspstate(6);
          //IA间歇
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
          { //info_result2是第2路地址的DB9~DB15。
           //IC过载
                showdspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1是第1路地址的DB8~DB15。
          //IABC 电源保护
                showdspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1是第1路地址的DB9~DB15。
                showdspstate(9);
          //I2间歇  ?温度保护间歇
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
          {//info_result2是第2路地址的DB9~DB15。
           //VA过载
                showdspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15。
                showdspstate(11);
              //V2闭锁
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7。

              //V3间歇
                showdspstate(12);
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
          {//info_result2是第2路地址的DB9~DB15。
            //V2过载
                showdspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2是第2路地址的DB8~DB15。  //DB8
           // VABC 温度保护
              showdspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2是第2路地址的DB9~DB15。
              showdspstate(15);
           //VA间歇
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
          { //info_result3是第2路地址的DB0~DB7。

               showdspstate(16);
            //VC过载
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2是第2路地址的DB8~DB15。  //DB13
              showdspstate(17);
            //VABC电源保护
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2是第2路地址的DB9~DB15。
              showdspstate(18);


          //Vz过载

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
          {//info_result3是第2路地址的DB0~DB7。
              showdspstate(19);

             //直流过载
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15。
            showdspstate(20);

            // VABC 温度保护
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7。
              showdspstate(21);
           //V3间歇
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


void Writelogicinfofile(unsigned char info1,unsigned char info2,unsigned char info3,char V1,char V2,char V3,char I1,char I2,char I3)
{
        FILE *fp;
        fp = fopen(Logicinfoname,"w+");
        if(fp == NULL)
        {
          return ;
        }
        fprintf(fp,"%d,%d,%d,%d,%d,%d,%d,%d,%d",info1,info2,info3,V1,V2,V3,I1,I2,I3);
        fclose(fp);
}


void Writeexitstate(float state)
{

    FILE *fp;
    fp = fopen(RUNSTATFILE,"w+");
    if(fp == NULL)
    {
      return ;
    }
    fprintf(fp,"%f",state);
    fclose(fp);
}




void init_par()
{
    FILE *fp;
    SystemVolt=57.735;
    if((fp=fopen(SYSSET,"a+"))==NULL)
      return;
    fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&PrepareTime);
    SystemFrec=SystemFrec*FreqTriple;
    fclose(fp);

    V1Value=SystemVolt;
    V2Value=SystemVolt;
    V3Value=SystemVolt;
    I1Value=0;
    I2Value=0;
    I3Value=0;
    V1DC=0;
    V2DC=0;
    V3DC=0;
    I1DC=0;
    I2DC=0;
    I3DC=0;
    MAX_V_VALUEDATA_DC=176;
    MIN_V_VALUEDATA_DC=-176;
    MAX_V_VALUEDATA_AC=125;
    MIN_V_VALUEDATA_AC=0;
    MAX_I_VALUEDATA_DC=70;
    MIN_I_VALUEDATA_DC=-70;
    MAX_I_VALUEDATA_AC=50;
    MIN_I_VALUEDATA_AC=0;
}



void ClearTemplateTestState(){


    FILE *fp2=fopen(TemplateTestState,"w+");
    if(fp2!=NULL)
        fclose(fp2);
}

void ReadTemplateTestState(){

    FILE *fp=fopen(TemplateTestState,"r");
    if(fp!=NULL)
  {
    char buf[50];
    fread(buf,strlen("STOPTEST"),1,fp);
    if(memcmp(buf,"STOPTEST",strlen("STOPTEST"))==0)
          starttest=0;
    fclose(fp);
    }

    ClearTemplateTestState();
}



//传入参数格式  ./程序名  Fset Tset  df/dt   Vblock testitem  faultduration

int main(int argc, char *argv[])
{
    unsigned char info_flag1,info_flag2,info_flag3;
    KEY key;
    int info_num=0;
     int rc=0;
    if(argc>1)
    {
        char *p1=argv[1];
        if(memcmp(p1,"testitem=",9)==0)
        {
        p1=p1+9;
        testitem=atoi(p1);
        }
        else return -1;
        if(testitem==1)
        {
        TestItem1=1;
        TestItem2=0;
        TestItem3=0;
        TestItem4=0;
        TestItem5=0;
        faultduration=atof(argv[2]);
        FreqVar.TripValue.FreEndValueStart.value=atof(argv[3]);
        FreqVar.TripValue.FreEndValueEnd.value=atof(argv[4]);
        FreqVar.TripValue.FreStep.value=atof(argv[5]);
        FreqVar.TripValue.HoldTime.value=atof(argv[6]);
        }
        else if(testitem==2)
        {
        TestItem1=0;
        TestItem2=1;
        TestItem3=0;
        TestItem4=0;
        TestItem5=0;
        faultduration=atof(argv[2]);
        FreqVar.TripTime.FreEndValue.value=atof(argv[3]);
        FreqVar.TripTime.HoldTime.value=atof(argv[4]);
        }
        else if(testitem==4)
        {
            TestItem1=0;
            TestItem2=0;
            TestItem3=1;
            TestItem4=0;
            TestItem5=0;
            faultduration=atof(argv[2]);
            FreqVar.Tripdfdt.FreEndValue.value=atof(argv[3]);
            FreqVar.Tripdfdt.DfdtStart.value=atof(argv[4]);

            FreqVar.Tripdfdt.DfdtEnd.value=atof(argv[5]);
            FreqVar.Tripdfdt.DfdtStep.value=atof(argv[6]);

            currentdfdt=FreqVar.Tripdfdt.DfdtStart.value;


        }
        else if(testitem==8)
        {
            TestItem1=0;
            TestItem2=0;
            TestItem3=0;
            TestItem4=1;
            TestItem5=0;
            faultduration=atof(argv[2]);
            FreqVar.Tripdfdttime.dfdt.value=atof(argv[3]);
            FreqVar.Tripdfdttime.stopfreq.value=atof(argv[4]);
            FreqVar.Tripdfdttime.Duration.value=atof(argv[5]);

        }
        else if(testitem==16)
        {
            TestItem1=0;
            TestItem2=0;
            TestItem3=0;
            TestItem4=0;
            TestItem5=1;
            faultduration=atof(argv[2]);
            FreqVar.TripVBlock.Dfdt.value=atof(argv[3]);
            FreqVar.TripVBlock.FreEndValue.value=atof(argv[4]);
            FreqVar.TripVBlock.VStart.value=atof(argv[5]);
            FreqVar.TripVBlock.VEnd.value=atof(argv[6]);
            FreqVar.TripVBlock.VStep.value=atof(argv[7]);
            FreqVar.TripVBlock.HoldTime.value=atof(argv[8]);
            FreqVar.TripVBlock.VSelect=atoi(argv[9]);


            currentvoltage=FreqVar.TripVBlock.VStart.value;//当前的电压值
        }
        init_par();

    }
    else return -1;

    temprorarydatainit();
    UartsInit();

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
          { //125V电压板
            MAX_V_VALUEDATA_AC=125;
            MIN_V_VALUEDATA_AC=0;
            MAX_V_VALUEDATA_DC=176;
            MIN_V_VALUEDATA_DC=-176;
          }
        else if((result[7]&0xf0)==0x00)
          { //75V电压板
            MAX_V_VALUEDATA_AC=75;
            MIN_V_VALUEDATA_AC=0;
            MAX_V_VALUEDATA_DC=110;
            MIN_V_VALUEDATA_DC=-110;
          }
        if((result[7]&0x0f)==0x01)
          { //30A电流板
            MAX_I_VALUEDATA_AC=30;
            MIN_I_VALUEDATA_AC=0;
            MAX_I_VALUEDATA_DC=42;
            MIN_I_VALUEDATA_DC=-42;
          }
        else if((result[7]&0x0f)==0x08)
          { //40A电流板
            MAX_I_VALUEDATA_AC=40;
            MIN_I_VALUEDATA_AC=0;
            MAX_I_VALUEDATA_DC=56;
            MIN_I_VALUEDATA_DC=-56;
          }
        else if((result[7]&0x0f)==0x05)
          { //50A电流板
            MAX_I_VALUEDATA_AC=50;
            MIN_I_VALUEDATA_AC=0;
            MAX_I_VALUEDATA_DC=70;
            MIN_I_VALUEDATA_DC=-70;
          }
     key_deal_test_current(temprorarydata->UartDev[0].uartfd);
     while(1)
     {
        key=GetKey(temprorarydata->UartDev[1].uartfd);
        if(key==_STOP)
          starttest=0;

         ReadTemplateTestState();

        if(starttest==1)
        {
            ReadAutoLogic(temprorarydata->UartDev[0].uartfd);
            if(info_num++>10)
            {
             artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);
             info_flag1=result[8];
             artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
             info_flag2=result[8];
             info_flag3=result[9];
             logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);
             //将开关量信息写入文件
           //  Writelogicinfofile(info_flag1,info_flag2,info_flag3,V1Output,V2Output,V3Output,I1Output,I2Output,I3Output);
             info_num=0;
             }
             if(starttest==0&&save_f==1) //保存数据
             {

                   artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x80)",result,0);
                   char strbuf[512];
                   char outchar[512];
                   strcpy(outchar,"WARNING=Run normally");
                   fprintf(stdout,outchar);
                   fflush(stdout);

                   FILE *fp=fopen(TemplateReport,"a+");  //附加的方式打开读写文件
                   if(TestItem1==1)
                      {
                        if(TripTime1==-1)
                        {  strcpy(strbuf,"TRIPFREQ=-1");
                           Writeexitstate(-1);

                        }
                        else
                        {
                            float FENDSTART=FreqVar.TripValue.FreEndValueStart.value;
                            float FENDEND=FreqVar.TripValue.FreEndValueEnd.value;
                            float FSTEP=FreqVar.TripValue.FreStep.value;

                            if(FENDSTART>=FENDEND)
                             CurrentFreq=CurrentFreq+FSTEP;
                            else
                             CurrentFreq=CurrentFreq-FSTEP;
                         sprintf(strbuf,"TRIPFREQ=%0.3f",CurrentFreq);
                         Writeexitstate(CurrentFreq);
                         }
                         fprintf(fp,"%s\n",strbuf);

                      }

                   if(TestItem2==1)
                     {
                       if(TripTime2==-1)
                       {  strcpy(strbuf,"TRIPTIME=-1");
                          Writeexitstate(-1);
                       }
                       else
                        { sprintf(strbuf,"TRIPTIME=%4.3f",TripTime2);
                          Writeexitstate(TripTime2);
                        }
                         fprintf(fp,"%s\n",strbuf);
                     }
                    if(TestItem3==1)
                      {
                        if(TripTime3==-1)
                         { strcpy(strbuf,"TRIPDFDT=-1");
                           Writeexitstate(-1);
                         }
                        else
                         {
                           if(FreqVar.Tripdfdt.DfdtStart.value>FreqVar.Tripdfdt.DfdtEnd.value) //若DFDT是下降的
                            currentdfdt=currentdfdt+FreqVar.Tripdfdt.DfdtStep.value;
                            else
                            currentdfdt=currentdfdt-FreqVar.Tripdfdt.DfdtStep.value;

                           sprintf(strbuf,"TRIPDFDT=%f",currentdfdt);
                           Writeexitstate(currentdfdt);
                         }
                        fprintf(fp,"%s\n",strbuf);
                      }
                    if(TestItem4==1)
                      {
                        if(TripTime4==-1)
                         { strcpy(strbuf,"TRIPDFDTTIME=-1");
                           Writeexitstate(-1);
                         }
                        else
                         { sprintf(strbuf,"TRIPDFDTTIME=%f",TripTime4);
                           Writeexitstate(TripTime4);
                         }
                        fprintf(fp,"%s\n",strbuf);
                      }
                    if(TestItem5==1)
                      {
                        if(TripTime5==-1)
                         { strcpy(strbuf,"VBLOCKTRIP=-1");
                           Writeexitstate(-1);
                         }
                        else
                          {

                             if(FreqVar.TripVBlock.VStart.value>FreqVar.TripVBlock.VEnd.value)

                             currentvoltage=currentvoltage+FreqVar.TripVBlock.VStep.value;

                             else

                             currentvoltage=currentvoltage-FreqVar.TripVBlock.VStep.value;


                           sprintf(strbuf,"VBLOCKTRIP=%4.3f",currentvoltage);
                           Writeexitstate(currentvoltage);
                          }
                          fprintf(fp,"%s\n",strbuf);
                      }
                    fclose(fp);

                                output_signal_end(temprorarydata->UartDev[0].uartfd);
                                artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
                                close(temprorarydata->UartDev[1].uartfd);
                                close(temprorarydata->UartDev[0].uartfd);
                                free(temprorarydata);
                                temprorarydata=NULL;

                                ClearTemplateTestState();
                                 return 0;
             }
        }
        else
        {

                 char outchar[512];
                 strcpy(outchar,"WARNING=Run normally");
                 fprintf(stdout,outchar);
                 fflush(stdout);
                 artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x80)",result,0);

                              output_signal_end(temprorarydata->UartDev[0].uartfd);
                              artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
                              close(temprorarydata->UartDev[1].uartfd);
                              close(temprorarydata->UartDev[0].uartfd);
                              free(temprorarydata);
                              temprorarydata=NULL;
                              ClearTemplateTestState();


                              Writeexitstate(-2);

                              return 0;
        }
     }
}
