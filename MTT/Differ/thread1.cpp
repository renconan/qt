#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include <QPointF>
#include "Config.h"
#include <QSemaphore>
#include "Testcom2.h"
#include "thread1.h"
#include "differseq.h"
#include "Gpio.h"
#include "Uartset.h"
#include "em335x_drivers.h"
#include <termios.h>


extern TEMPRORARYDATA *temprorarydata;
extern int info_num=0;
extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
extern float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
extern int   input_par,HelpFlag;
extern int   INPAR,seq_count,ResultPage,ResultFlag,ResultCount,DisResult,ResultEndPage;
extern int   testcount,time_relay_testcount,auto_end;
extern int   cx,cy,cx_start,cx_mid,cx_end,increase,starttest,low,row,page;
extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC,EndTest,ResultEndCount;
extern float LogicResolution,SystemFrec;
extern float ChangeValue,timeout,TripTime,TripTimeSum[63],IdInitValue;
extern int save_f;
extern int sys[4];



extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern float prefaulttime;
extern float  SystemVolt;

extern int ConnectType;
extern int Testobject;
extern float prefaulttime;

extern float PhaseCorrectMode; //△侧校正
extern float PhaseCorrectStep;

extern float ChangeAngel;  //变化的角度

extern float PhaseCorrectStepTime; //步长时间

extern float IdSearchStep;
extern float IdSearchStepTime;

extern float IrSeachStart;
extern float IrSeachEnd;
extern float IrSeachStep;
extern float CurrrentIrSearch;

extern float Kp1;
extern float Kp2;
extern float Kp3;


extern float IaValue,IaPhase,IaStep;
extern float IbValue,IbPhase,IbStep;
extern float IcValue,IcPhase,IcStep;
extern QVector<QPointF> TestResult;

extern float IhStdValue;
extern float ImSdtValue;
extern float IlStdValue;

extern VOLCHANGERINFO Volchanger;


extern float StepTime;

extern int CTHdir;
extern int CTMdir;
extern int CTLdir;
extern bool Threadrunstate;


extern  float Postfaulttime;
extern  int   dcouttype;

extern bool SoftStartEndflag;

PHASECHANGEINFO Phaseactinfo;

#define LEDNUM GPIO8
thread1::thread1(QObject *parent) :
    QThread(parent)
{
    temprorarydata=new TEMPRORARYDATA;
}


void thread1::DisplayResult(int ResultSum)
{

  emit  SIG_testresult(ResultSum);

}

void thread1::OutputAngle(int fd)//shuchu xiangwei jiao
{



    if(PhaseCorrectMode==1)
    {
        if((ConnectType==1||ConnectType==2)&&Testobject==1) //△1 △11 高中
        {
          OutputAngle1(fd);
        }
        else if((ConnectType==1||ConnectType==2)&&Testobject==2) //△1 △11  高低
        {
         OutputAngle2(fd);
        }
        else if(ConnectType==3&&Testobject==1)  //△5 高中
        {
         OutputAngle3(fd);
        }
        else if(ConnectType==3&&Testobject==2)
        {
         OutputAngle4(fd);
        }



    }
    else if(PhaseCorrectMode==2)
    {

        if(Testobject==1)
        {
            OutputAngle5(fd);
        }
        else if(Testobject==2)
        {

           switch(ConnectType)
           {
           case 1:
               OutputAngle6(fd);
               break;

           case 2:
               OutputAngle7(fd);
               break;

           case 3:
               OutputAngle8(fd);
               break;
           }

        }

    }

}

void thread1::OutputAngle1(int fd)  //△1 △11 高中
{

    IaValue=CurrrentIrSearch*IhStdValue*1.732;
    IaPhase=0;
    IbValue=CurrrentIrSearch*ImSdtValue*1.732;
    IbPhase=180-ChangeAngel;
    IcValue=(2*IaValue/1.732)*sin(ChangeAngel/2*3.1415/180)/Kp3;
    IcPhase=(IaPhase+IbPhase)/2;

    if(CTHdir==2)
         IaPhase=IaPhase+180;
    if(CTMdir==2)
     { IbPhase=IbPhase+180;
       if(IbPhase>=360)
          IbPhase=IbPhase-360;
     }
     IcPhase=(IaPhase+IbPhase)/2;
     if(CTLdir==2)
        IcPhase=IcPhase+180;
     if(IcPhase>=360)
        IcPhase=IcPhase-360;
    emit SIG_dischange(ChangeAngel);
    OutputSequencer2(fd);
}

void thread1::OutputAngle2(int fd) //△1 △11  高低
{


    IaValue=CurrrentIrSearch*IhStdValue*1.732;
    IaPhase=0;
    IbValue=CurrrentIrSearch*IlStdValue;
    IbPhase=180-ChangeAngel;
    IcValue=IaValue/1.732/Kp3;
    IcPhase=IaPhase;

    if(CTHdir==2)
        IaPhase=IaPhase+180;
     IcPhase=IaPhase;
     if(CTLdir==2)
     {
       IbPhase=IbPhase+180;
       IcPhase=IcPhase+180;
       if(IbPhase>=360)
           IbPhase=IbPhase-360;
       if(IcPhase>=360)
           IcPhase=IcPhase-360;
     }
    emit SIG_dischange(ChangeAngel);
    OutputSequencer2(fd);

}

void thread1::OutputAngle3(int fd) //△5 高中
{



    IaValue=CurrrentIrSearch*IhStdValue*1.732;
    IaPhase=0;
    IbValue=CurrrentIrSearch*ImSdtValue*1.732;
    IbPhase=180-ChangeAngel;
    IcValue=(2*IaValue/1.732)*sin(ChangeAngel/2*3.1415/180)/Kp3;
    IcPhase=(IaPhase+IbPhase)/2+180;
    if(CTHdir==2)
        IaPhase=IaPhase+180;
    if(CTMdir==2)
      { IbPhase=IbPhase+180;
        if(IbPhase>=360)
         IbPhase=IbPhase-360;
      }

      IcPhase=(IaPhase+IbPhase)/2+180;
      if(CTLdir==2)
         IcPhase=IcPhase+180;
      if(IcPhase>=360)
         IcPhase=IcPhase-360;

    emit SIG_dischange(ChangeAngel);
    OutputSequencer2(fd);

}
void thread1::OutputAngle4(int fd)// //△5  高低
{


    IaValue=CurrrentIrSearch*IhStdValue*1.732;
    IaPhase=0;
    IbValue=CurrrentIrSearch*IlStdValue;
    IbPhase=0+ChangeAngel;
    IcValue=IaValue/1.732/Kp3;
    IcPhase=IaPhase+180;
    if(CTHdir==2)
        IaPhase=IaPhase+180;
     IcPhase=IaPhase+180;
     if(IcPhase>=360)
         IcPhase=IcPhase-360;

     if(CTLdir==2)
     {
       IbPhase=IbPhase+180;
       IcPhase=IcPhase+180;
       if(IbPhase>=360)
           IbPhase=IbPhase-360;
       if(IcPhase>=360)
           IcPhase=IcPhase-360;
     }
    emit SIG_dischange(ChangeAngel);
    OutputSequencer2(fd);
}

void thread1::OutputAngle5(int fd)// △1 △11   △5  高中
{


    IaValue=CurrrentIrSearch*IhStdValue;
    IaPhase=0;
    IbValue=CurrrentIrSearch*ImSdtValue;
    IbPhase=180-ChangeAngel;
    IcValue=0;
    IcPhase=0;
    if(CTHdir==2) //高压侧
        IaPhase=IaPhase+180;
    if(CTMdir==2) //中压侧
        IbPhase=IbPhase+180;
    if(IbPhase>=360)
        IbPhase=IbPhase-360;
    emit SIG_dischange(ChangeAngel);
    OutputSequencer2(fd);
}


void thread1::OutputAngle6(int fd)// △11 高低
{



    IaValue=CurrrentIrSearch*IhStdValue;
    IaPhase=0;
    IbValue=CurrrentIrSearch*IlStdValue*1.732;
    IbPhase=180-ChangeAngel;
    IcValue=IbValue*Kp3/1.732;
    IcPhase=180-ChangeAngel;

      if(CTLdir==2)
      {
       IbPhase=IbPhase+180;
       if(IbPhase>=360)
         IbPhase=IbPhase-360;
      }
    if(ConnectType==1||ConnectType==2)
       IcPhase=IbPhase;
    else
       IcPhase=IbPhase+180;
    if(IcPhase>=360)
        IcPhase=IcPhase-360;

    if(CTHdir==2)
    {
        IaPhase=IaPhase+180;
        IcPhase=IcPhase+180;
        if(IcPhase>=360)
           IcPhase=IcPhase-360;
    }
      emit SIG_dischange(ChangeAngel);
      OutputSequencer2(fd);



}
void thread1::OutputAngle7(int fd)// △1 高低
{

    IaValue=CurrrentIrSearch*IhStdValue;
    IaPhase=0;
    IbValue=CurrrentIrSearch*IlStdValue*1.732;
    IbPhase=180-ChangeAngel;
    IcValue=IbValue*Kp3/1.732;
    IcPhase=180-ChangeAngel;


      if(CTLdir==2)
      {
       IbPhase=IbPhase+180;
       if(IbPhase>=360)
         IbPhase=IbPhase-360;
      }
    if(ConnectType==1||ConnectType==2)
       IcPhase=IbPhase;
    else
       IcPhase=IbPhase+180;
    if(IcPhase>=360)
        IcPhase=IcPhase-360;

    if(CTHdir==2)
    {
        IaPhase=IaPhase+180;
        IcPhase=IcPhase+180;
        if(IcPhase>=360)
           IcPhase=IcPhase-360;
    }
   emit SIG_dischange(ChangeAngel);
    OutputSequencer2(fd);


}
void thread1::OutputAngle8(int fd)// △5 高低
{


    IaValue=CurrrentIrSearch*IhStdValue;
    IaPhase=0;
    IbValue=CurrrentIrSearch*IlStdValue*1.732;
    IbPhase=0-ChangeAngel;


    IcValue=IbValue*Kp3/1.732;
    IcPhase=180-ChangeAngel;



      if(CTLdir==2)
      {
       IbPhase=IbPhase+180;
       if(IbPhase>=360)
         IbPhase=IbPhase-360;
      }
    if(ConnectType==1||ConnectType==2)
       IcPhase=IbPhase;
    else
       IcPhase=IbPhase+180;
    if(IcPhase>=360)
        IcPhase=IcPhase-360;

    if(CTHdir==2)
    {
        IaPhase=IaPhase+180;
        IcPhase=IcPhase+180;
        if(IcPhase>=360)
           IcPhase=IcPhase-360;
    }
      emit SIG_dischange(ChangeAngel);
      OutputSequencer2(fd);


}
void thread1::Getresult(){

        float Id,Ir;

        if(PhaseCorrectMode==1)    // sanjiao
         {
             Id=(2*IaValue/1.732)*sin(ChangeAngel/2*3.1415/180);
             Ir=CurrrentIrSearch*IhStdValue;
             TestResult.append(QPointF(Ir,Id));
             ChangeAngel=0;
             DisplayResult(1);
         }

        else if(PhaseCorrectMode==2)
         {
             float SinAngle=ChangeAngel/2*3.1415/180;

             Id=(2*IaValue)*sin(SinAngle);
             Ir=CurrrentIrSearch*IhStdValue;
             TestResult.append(QPointF(Ir,Id));
             ChangeAngel=0;
             DisplayResult(1);
         }

    }


void thread1::ReadTripTimeLogic(int fd)
{
        int t2=0,t3=0,t4=0,t5=0;
        unsigned char signature;
        double u,v,w;
        static double SequencerTime4;
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
              break;
            case 3:
              t3=1;
              break;
            case 4:
              t4=1;
              SequencerTime4=gettime(u,v,w);//计算从开始测试到sequencer的Step4的时间。
              break;
            case 5:
              t5=1;
              break;
          }

        if(t3==1)
          {   //未动作
           TripTime=-1;


          }
        if(t4==1)
       {//动作
            TripTime=1;
       }

        if(t5==1)
        {

           output_signal_end(fd);
           if(TripTime==-1)
           {
               if(seq_count==1)
               {
                   seq_count=2;
                   Phaseactinfo.flagstart=0;
                   ChangeAngel=Phaseactinfo.EndAngle;
                   OutputAngle(fd);
                   return;
               }

               else if(seq_count==2)
               {
                   Phaseactinfo.flagend=0;
                   if(Phaseactinfo.flagstart==0&&Phaseactinfo.flagend==0) // 下一个循环 ir循环
                   {
                       if(CurrrentIrSearch<IrSeachEnd) //下一个循环 ir循环
                       {
                           CurrrentIrSearch=CurrrentIrSearch+IrSeachStep;
                           memset(&Phaseactinfo,0,sizeof(PHASECHANGEINFO));
                           Phaseactinfo.OutAngel=0;
                           Phaseactinfo.StartAngle=0;
                           Phaseactinfo.EndAngle=180;
                           seq_count=1;
                           ChangeAngel=0;

                           OutputAngle(fd);
                           return;
                       }

                       else //停止输出
                       {
                           output_signal_end(fd);
                           starttest=0;
                           save_f=1;
                           //ClearLED();
                           GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                           return;
                       }
                   }
                   else
                   {
                       seq_count=3;// 有中间值动
                      //第一个动，第二个未动
                       Phaseactinfo.OutAngel=(Phaseactinfo.StartAngle+Phaseactinfo.EndAngle)/2;
                       Phaseactinfo.EndAngle=0;
                       Phaseactinfo.StartAngle=180;
                       ChangeAngel=Phaseactinfo.OutAngel;

                       OutputAngle(fd);
                       return;
                   }

               }

               else if(seq_count==3)
               {
                   if(fabs(Phaseactinfo.StartAngle-Phaseactinfo.EndAngle)<=0.3) //该ir已经搜索完成
                   {
                       Getresult();
                       if(CurrrentIrSearch<IrSeachEnd) //下一个循环 ir循环
                       {
                           CurrrentIrSearch=CurrrentIrSearch+IrSeachStep;
                           memset(&Phaseactinfo,0,sizeof(PHASECHANGEINFO));
                           Phaseactinfo.OutAngel=0;
                           Phaseactinfo.StartAngle=0;
                           Phaseactinfo.EndAngle=180;
                           seq_count=1;
                           ChangeAngel=0;

                           OutputAngle(fd);

                           return;
                       }

                       else //停止输出
                       {
                           output_signal_end(fd);
                           starttest=0;
                           save_f=1;
                           GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                           //ClearLED();
                           return;
                       }

                   }
                   else  // 继续搜索
                   {

                       Phaseactinfo.StartAngle=Phaseactinfo.OutAngel;
                       Phaseactinfo.OutAngel=(Phaseactinfo.StartAngle+Phaseactinfo.EndAngle)/2;
                       ChangeAngel=Phaseactinfo.OutAngel;

                       OutputAngle(fd);

                       return;
                   }
               }

           }

           else if(TripTime==1)
           {
               if(seq_count==1)
               {
                   seq_count=2;
                   Phaseactinfo.flagstart=1;
                   ChangeAngel=Phaseactinfo.EndAngle;

                   OutputAngle(fd);

                   return;
               }
               else if(seq_count==2)
               {
                   Phaseactinfo.flagend=1;
                   seq_count=3;// 有中间值动
                   Phaseactinfo.OutAngel=(Phaseactinfo.StartAngle+Phaseactinfo.EndAngle)/2;
                   ChangeAngel=Phaseactinfo.OutAngel;

                   OutputAngle(fd);
                   return;
               }
               else if(seq_count==3)
               {
                   if(fabs(Phaseactinfo.StartAngle-Phaseactinfo.EndAngle)<=0.3) //该ir已经搜索完成
                   {
                       Getresult();
                       if(CurrrentIrSearch<IrSeachEnd) //下一个循环 ir循环
                       {
                           CurrrentIrSearch=CurrrentIrSearch+IrSeachStep;
                           memset(&Phaseactinfo,0,sizeof(PHASECHANGEINFO));
                           Phaseactinfo.OutAngel=0;
                           Phaseactinfo.StartAngle=0;
                           Phaseactinfo.EndAngle=180;
                           seq_count=1;
                           ChangeAngel=0;

                           OutputAngle(fd);

                           return;
                       }

                       else //停止输出
                       {
                           output_signal_end(fd);
                           starttest=0;
                           save_f=1;
                           //ClearLED();
                           GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                           return;
                       }
                   }
                   else  // 继续搜索
                   {
                       Phaseactinfo.EndAngle=Phaseactinfo.OutAngel;
                       Phaseactinfo.OutAngel=(Phaseactinfo.StartAngle+Phaseactinfo.EndAngle)/2;
                       ChangeAngel=Phaseactinfo.OutAngel;
                       OutputAngle(fd);
                       return;
                   }
               }

           }

        }
}





void thread1::key_deal_test_current(int fd)
{
        if(starttest==0)
          {
              emit SIG_Teststate(1);
              output_signal_end(fd);
              GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
              //SetLED();
              I1Output=1;
              I2Output=1;
              I3Output=1;
              starttest=1;
              ChangeAngel=0;
              TestResult.clear();
              DisplayResult(0);
              CurrrentIrSearch=IrSeachStart;
              memset(&Phaseactinfo,0,sizeof(PHASECHANGEINFO));
              Phaseactinfo.OutAngel=0;
              Phaseactinfo.StartAngle=0;
              Phaseactinfo.EndAngle=180;
              OutputAngle(fd);
              seq_count=1;
              save_f=0;
          }
}

void thread1::key_deal_stop_current(int fd)
{
    if(starttest==1)
     {
          emit SIG_dischange(-500);
          output_signal_end(fd);
          starttest=0;
          ResultFlag=0;
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
      //ClearLED();
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
    unsigned char info_flag1,info_flag2,info_flag3,info_flag4;
    temprorarydatainit();
    UartsInit();
    temprorarydata->gpio_fd= open("/dev/em335x_gpio", O_RDWR);
    printf("open file = %d\n", temprorarydata->gpio_fd);

             rc = GPIO_OutEnable( temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
             if(rc < 0)
             {
                     printf("GPIO_OutEnable::failed %d\n", rc);
                     temprorarydata->gpio_fd=-1;
             }
     //ClearLED();
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

     if(temprorarydata->keypadopenflag==false&&temprorarydata->helpopenflag==false&&\
        temprorarydata->AuxCaculateflag==false)
   {  key=GetKey(temprorarydata->UartDev[1].uartfd);
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
           info_flag4=result[9];
           artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
           info_flag2=result[8];
           info_flag3=result[9];
           logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //需要emit
           info_num=0;
       }
     }
     else {
         ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);
         if(info_num++>50)
         {
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //不需要更改
          info_flag1=result[8];
          info_flag4=result[9];
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
          info_flag2=result[8];
          info_flag3=result[9];
          logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //需要emit
          info_num=0;
          }}}

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



void thread1::logic_info(int fd,unsigned char info_result1,unsigned char info_result2,unsigned char info_result3,unsigned char info_result4)
  {
     //info_result1为第1路地址的DB8-DB15，info_result2为第2路地址的DB8-DB15，info_result3为第2路地址的DB0-DB7。

    static int info_count=1;
    int  k1=0;
    //蜂鸣器
    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);
    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //不需要控件
          artExec(fd,"out:dig:on(0x80)",result,0);
        else
          artExec(fd,"out:dig:off(0x80)",result,0);


      }
    if((info_result4&0x0F)==0x00)
    {
        emit SIG_dspstate(100);
    }
    else
    {
        if((info_result4&0x01)==0x01)
        {
            emit SIG_dspstate(101);
        }
        else if((info_result4&0x01)==0x00)
        {
            emit SIG_dspstate(111);
        }
        if((info_result4&0x02)==0x02)
        {
            emit SIG_dspstate(102);
        }
        else if((info_result4&0x02)==0x00)
        {
            emit SIG_dspstate(112);
        }
        if((info_result4&0x04)==0x04)
        {
            emit SIG_dspstate(103);
        }
        else if((info_result4&0x04)==0x00)
        {
            emit SIG_dspstate(113);
        }
        if((info_result4&0x08)==0x08)
        {
            emit SIG_dspstate(104);
        }
        else if((info_result4&0x08)==0x00)
        {
            emit SIG_dspstate(114);
        }
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
           {           //停止输出测试结束
            //ClearLED();
                GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
            starttest=0;
            emit SIG_dischange(-500);
            output_signal_end(fd);
            starttest=0;
            ResultFlag=0;
            }
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

           ;
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







