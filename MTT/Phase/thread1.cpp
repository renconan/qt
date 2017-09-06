#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include "Config.h"
#include <QSemaphore>
#include <termios.h>
#include "Testcom2.h"
#include "thread1.h"
#include "Phraseseq.h"
#include "Gpio.h"
#include "em335x_drivers.h"
#include "Uartset.h"


#define LEDNUM GPIO8
int info_num=0;

extern TEMPRORARYDATA *temprorarydata;
extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
extern float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
extern int   FreqTriple;
extern int   starttest;
extern int   seq_count,V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;
extern int   TestItem1,TestItem2;


extern float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
extern float TripTime1,TripTime2,TripTime3;
extern float ChangeValue,timeout;
extern int save_f;
extern int sys[4];
extern float LogicResolution,SystemFrec,SystemVolt;//2009-6-20
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;


extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,VzRelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase;


extern float PENDSTART;
extern float PENDEND;
extern float PSTEP;
extern float PEND;
extern float ReturnStep;
extern float STEPTIME;
extern float faultduration;

extern int IACH;
extern int IBCH;
extern int ICCH;
extern int VACH;
extern int VBCH;
extern int VCCH;
extern float currentphrase;
extern float prefaulttime; //故障前
extern int input_par;
extern int HelpFlag;


extern int TriptimeType; //测试时间的方式
extern float StopPhase; //测试动作时间的相位
extern float TripTimeHoldtime; //做动作时间的保持时间
extern float postfaulttime; //故障后时间


extern bool Threadrunstate;

extern bool SoftStartEndflag;

thread1::thread1(QObject *parent) :
    QThread(parent)
{

}

void thread1::DisplayResult(int ResultSum)
{
    emit  SIG_testresult(ResultSum);
}


int  thread1::Buzzer(char Address1,char Address2,char Address3)
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


void thread1::logic_info(int fd,char info_result1,char info_result2,char info_result3)
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
          emit SIG_dspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1是第1路地址的DB9~DB15。
           emit SIG_dspstate(1);
            //IA过载
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
            emit SIG_dspstate(2);

            //IABC 温度保护
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //IC间歇
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
          {  //info_result1是第1路地址的DB9~DB15。

              emit SIG_dspstate(4);
            //IB过载
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1是第1路地址的DB8~DB15。

              emit SIG_dspstate(5);
           //IC闭锁
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1是第1路地址的DB9~DB15。
              emit SIG_dspstate(6);
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
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1是第1路地址的DB8~DB15。
          //IABC 电源保护
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1是第1路地址的DB9~DB15。
              emit SIG_dspstate(9);
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
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15。
              emit SIG_dspstate(11);
              //V2闭锁
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7。

              //V3间歇
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
          {//info_result2是第2路地址的DB9~DB15。
            //V2过载
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2是第2路地址的DB8~DB15。  //DB8
           // VABC 温度保护
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2是第2路地址的DB9~DB15。
            emit SIG_dspstate(15);
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

             emit SIG_dspstate(16);
            //VC过载
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2是第2路地址的DB8~DB15。  //DB13
            emit SIG_dspstate(17);
            //VABC电源保护
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2是第2路地址的DB9~DB15。
            emit SIG_dspstate(18);


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
            emit SIG_dspstate(19);

             //直流过载
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15。
          emit SIG_dspstate(20);

            // VABC 温度保护
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7。
            emit SIG_dspstate(21);
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



void thread1::ReadAutoLogic(int fd)
{
    int t2=0,t3=0,t4=0,t5=0;
    unsigned char signature;
    static double SequencerTime2,SequencerTime4;
    double u,v,w;
    float Temp;
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
      }

    if(t3==1)
      {
        switch(seq_count)
          {
            case 1:
              TripTime1=-1;
              break;
            case 2:
              TripTime2=-1;
              DisplayResult(2);
              break;
          }
      }
    if(t4==1)
      {

        switch(seq_count)
          {
            case 1:
              TripTime1=1;
              if(PENDSTART>=PENDEND)
              PEND=currentphrase+PSTEP;
              else
              PEND=currentphrase-PSTEP;    
              DisplayResult(1);
              break;
            case 2:
              TripTime2=SequencerTime4-prefaulttime-LogicResolution/1000;
              Temp=TripTime2*1000;
              Temp=ceil(Temp);
              TripTime2=Temp/1000;//将返回值的限制在小数点后最多只有两位]
              DisplayResult(2);
              break;
          }
      }
    if(t5==1)
      {
        switch(seq_count)
          {
            case 1:
             output_signal_end(fd); //关闭各个放大器
             starttest=0;           //整个测试结束
             if(TripTime1==-1)
             {
             if(PENDSTART>=PENDEND)
            {
             if(currentphrase>=PENDEND)
             {

                    starttest=1;
                    artExec(fd,"out:dig:on(0x74)",result,0);
                    emit SIG_DisplayChangeValue(1,currentphrase);
                    artExec(fd,"out:dig:on(0x08)",result,0);
                    OutputSequencer1(fd);
                    currentphrase=currentphrase-PSTEP;
                    return;
             }
             else
             {
                  output_signal_end(fd); //关闭各个放大器
                  starttest=0;           //整个测试结束
                  GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                  DisplayResult(1);
                  emit SIG_DisplayChangeValue(0,0);
             }
            }
             else
             {
                 if(currentphrase<=PENDEND)
                 {

                        starttest=1;
                        emit SIG_DisplayChangeValue(1,currentphrase);
                        artExec(fd,"out:dig:on(0x74)",result,0);
                        artExec(fd,"out:dig:on(0x08)",result,0);
                        OutputSequencer1(fd);
                        currentphrase=currentphrase+PSTEP;
                        return;
                 }
                 else
                 {
                      output_signal_end(fd); //关闭各个放大器
                      starttest=0;           //整个测试结束
                      GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                      DisplayResult(1);
                      emit SIG_DisplayChangeValue(0,0);

                 }
             }
            }


             if(TestItem2==1&&(TripTime1==1||(TripTime1==-1&&TriptimeType==2)))
             {
                  DisplayResult(1);
                  output_signal_end(fd); //关闭各个放大器
                  starttest=1;           //整个测试结束
                  seq_count=2;
                  if(TriptimeType==1)
                  {
                      if(PENDSTART>=PENDEND)
                      PEND=currentphrase+PSTEP;
                      else
                      PEND=currentphrase-PSTEP;
                  }
                  else PEND=StopPhase;
                  artExec(fd,"out:dig:on(0x74)",result,0);
                  emit SIG_DisplayChangeValue(1,PEND);
                  artExec(fd,"out:dig:on(0x08)",result,0);
                  OutputSequencer2(fd);
             }
             else
              {
                  output_signal_end(fd); //关闭各个放大器
                  starttest=0;           //整个测试结束
                  GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                  save_f=1;//将保存结果标志置1
              }
                break;
            case 2://测试动作时间处理
                  output_signal_end(fd);//关闭各个放大器
                  starttest=0; //整个测试结束
                  GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                  save_f=1;//将保存结果标志置1
                  break;
         }
    }

}

void thread1::key_deal_test_current(int fd)
{

    if(starttest==0)
      {

        GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);  //亮
        emit SIG_testresult(0);  //清空测试结果区域
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=1;
        if(TestItem1==1)
          { //要测试Trip value
            starttest=1;
            seq_count=1;
            currentphrase=PENDSTART;
            DisplayResult(0);
            artExec(fd,"out:dig:on(0x74)",result,0);
            emit SIG_DisplayChangeValue(1,currentphrase);
            artExec(fd,"out:dig:on(0x08)",result,0);
            OutputSequencer1(fd);//输出测试Trip value的sequencer

               if(PENDSTART>=PENDEND)
              {
                 currentphrase=currentphrase-PSTEP;
              }
              else
              {
                 currentphrase=currentphrase+PSTEP;
              }


          }

        else if(TestItem1==0&&TestItem2==1)
          {//只测试动作 间
            starttest=1;
            PEND=StopPhase;
            seq_count=2;
            DisplayResult(0);
            artExec(fd,"out:dig:on(0x08)",result,0);
            artExec(fd,"out:dig:on(0x74)",result,0);
            OutputSequencer2(fd);//输出测试动作 间的sequencer
            emit SIG_DisplayChangeValue(1,PEND);
          }

      }
}

void thread1::key_deal_stop_current(int fd)
{
    if(starttest==1)
      {
          output_signal_end(fd);
          starttest=0;
          emit SIG_testresult(0);  //清空测试结果区域
          emit SIG_DisplayChangeValue(0,0); //输出值区域清0
      }
}

void thread1::key_response(KEY key)
{

    if(key==_NOP)   return;

    if(key==_TEST)
{
     if(starttest==0)
     {
        output_signal_end(temprorarydata->UartDev[0].uartfd);
        key_deal_test_current(temprorarydata->UartDev[0].uartfd);
         emit SIG_TestState();
     }
}
else if(key==_STOP)
{
      GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
      key_deal_stop_current(temprorarydata->UartDev[0].uartfd);
       emit SIG_TestState();
}
}



void thread1::stoprun(){

  Threadrunstate=false;

}


void thread1::run()
{
    KEY key;
    int rc;
     //打开并配置串口
    unsigned char info_flag1,info_flag2,info_flag3;
    temprorarydatainit();
    UartsInit();
    temprorarydata->gpio_fd= open("/dev/em335x_gpio", O_RDWR);
    printf("open file = %d\n", temprorarydata->gpio_fd);

    rc = GPIO_OutEnable(temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
    if(rc < 0)
    {
            printf("GPIO_OutEnable::failed %d\n", rc);
            temprorarydata->gpio_fd=-1;
    }

   online(temprorarydata->UartDev[0].uartfd);
   GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
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
         if(info_num++>10)
       {
         emit SIG_TestState();
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //不需要更改
         info_flag1=result[8];
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
         info_flag2=result[8];
         info_flag3=result[9];
         logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);  //需要emit
         info_num=0;
       }
       }
     else { 
         ReadAutoLogic(temprorarydata->UartDev[0].uartfd); //读开关量状态
         if(starttest==0)
         GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
         if(info_num++>5)
         {
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);          //不需要更改
          info_flag1=result[8];
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
          info_flag2=result[8];
          info_flag3=result[9];
          logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);  //需要emit
          info_num=0;
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


