#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include "Config.h"
#include <QSemaphore>
#include "Testcom2.h"
#include "thread1.h"
#include "Harseq.h"
#include "em335x_drivers.h"
#include "Gpio.h"
#include "Uartset.h"


float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;


float LogicResolution;
int   save_f;
int   sys[4];

float TripTime,TripValue;


extern float faultduration;




bool Threadrunstate;

int TripValueSteps=0;


TEMPRORARYDATA *temprorarydata;

bool SoftStartEndflag=false;




double I1ampPerStepError=0;
double I2ampPerStepError=0;
double I3ampPerStepError=0;
double  HarmonicStepError=0;



extern double max_output_v_a;   //,max_f_v_a=50,min_f_v_a=0.5,output_offset_v_a=0,amp_v_a=360;
extern double max_output_v_b;   //,max_f_v_b=50,min_f_v_b=0.5,output_offset_v_b=0,amp_v_b=120;
extern double max_output_v_c;   //,max_f_v_c=50,min_f_v_c=0.5,output_offset_v_c=0,amp_v_c=240;
extern double max_output_v_z;
extern double max_output_i_a;//,max_f_i_a=50,min_f_i_a=0.5,output_offset_i_a=0,amp_i_a=75;
extern double max_output_i_b;//,max_f_i_b=50,min_f_i_b=0.5,output_offset_i_b=0,amp_i_b=195;
extern double max_output_i_c;//,max_f_i_c=50,min_f_i_c=0.5,output_offset_i_c=0,amp_i_c=315;

#define LEDNUM GPIO8

thread1::thread1(QObject *parent) :
    QThread(parent)
{

    temprorarydata=new TEMPRORARYDATA;
}


int  thread1::Buzzer(char Address1,char Address2,char Address3)
  {//sys[1]:过载，sys[2]:间歇，sys[3]:闭锁
    if(sys[1]==1)
      {//过载信息
        if((Address1&0x08)!=0x00&&m_Ch[4].Output==1)
          return 1;//I1过载
        if((Address1&0x02)!=0x00&&m_Ch[5].Output==1)
          return 1;//I2过载
        if((Address2&0x80)!=0x00&&m_Ch[6].Output==1)
          return 1;//I3过载
        if((Address2&0x10)!=0x00&&m_Ch[0].Output==1)
          return 1;//V1过载
        if((Address2&0x02)!=0x00&&m_Ch[1].Output==1)
          return 1;//V2过载
        if((Address3&0x40)!=0x00&&m_Ch[2].Output==1)
          return 1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    else if(sys[1]==0)
      {//过载信息
        if((Address1&0x08)!=0x00&&m_Ch[4].Output==1)
          return -1;//I1过载
        if((Address1&0x02)!=0x00&&m_Ch[5].Output==1)
          return -1;//I2过载
        if((Address2&0x80)!=0x00&&m_Ch[6].Output==1)
          return -1;//I3过载
        if((Address2&0x10)!=0x00&&m_Ch[0].Output==1)
          return -1;//V1过载
        if((Address2&0x02)!=0x00&&m_Ch[1].Output==1)
          return -1;//V2过载
        if((Address3&0x40)!=0x00&&m_Ch[2].Output==1)
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



void thread1::logic_info(int fd,char info_result1,char info_result2,char info_result3,char info_result4)
  {

    static int info_count=1;
    int  k1=0;

    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);


    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //不需要控件
         { artExec(fd,"out:dig:on(0x80)",result,0);

          }
          else
         {  artExec(fd,"out:dig:off(0x80)",result,0);

         }
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
        if((info_result1&0x08)==0x08&&m_Ch[4].Output==1)
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
        if((info_result1&0x02)==0x02&&m_Ch[5].Output==1)
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
        if((info_result2&0x80)==0x80&&m_Ch[6].Output==1)
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
        if((info_result2&0x10)==0x10&&m_Ch[0].Output==1)
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
        if((info_result2&0x02)==0x02&&m_Ch[1].Output==1)
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
        if((info_result3&0x40)==0x40&&m_Ch[2].Output==1)
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


void thread1::ReadTripTimeLogic(int fd)
{
       int t2=0,t3=0,t4=0,t5=0;
       unsigned char signature;
       double u,v,w;
       static double SequencerTime4;
       static double SequencerTime2;
       float ChangeValue;
       int stepnum;

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
             SequencerTime2=gettime(u,v,w);
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

       if(t2==1)
       {

           artExec(fd,"seq:status?(step)",result,16);
           stepnum=result[9]*256+result[10];
           TripValueSteps=result[11]*256+result[12];
            ChangeValue=HarmonicStart-(HarmonicStep/*-HarmonicStepError*/)*stepnum;
           emit SIG_DisplayChangeValue(1,ChangeValue);

       }

       if(t3==1)
         {//未动作
           TripTime=-1;
            emit SIG_DisplayChangeValue(0,0);

         }
       if(t4==1)
         {//动作
           TripTime=1;
           emit SIG_DisplayChangeValue(0,0);
         }
       if(t5==1)
       {




           output_signal_end(fd);
           if(TripTime==-1)
           {
               TripValue=-1;
               //ClearLED();
               GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
               starttest=0;
               save_f=1;
               emit SIG_testresult(TripValue);
           }
           else
           {

                TripValue=HarmonicStart-TripValueSteps*(HarmonicStep/*-HarmonicStepError*/);
               //ClearLED();
                GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
               starttest=0;
               save_f=1;
               emit SIG_testresult(TripValue);







               /*

                float runtime=SequencerTime4-SequencerTime2-LogicResolution/1000;
                TripValue=HarmonicStart-HarmonicStep*(runtime/A_StepTime);
                emit SIG_testresult(TripValue);

                ClearLED();
                starttest=0;
                save_f=1;
                */





           }
              emit SIG_TestState();


       }

}






double thread1::CaculateError(){//2016-5-11谐波changvalue变化范围修改

    float y;

    float HarmonicStepValue;
    HarmonicStepValue=H_Current*HarmonicStep/100;

    y=chang_amp(HarmonicStepValue,&max_output_i_a);
    I1ampPerStepError=HarmonicStepValue-(double)(max_output_i_a*y/32767);
    y=chang_amp(HarmonicStepValue,&max_output_i_b);
    I2ampPerStepError=HarmonicStepValue-(double)(max_output_i_b*y/32767);
    y=chang_amp(HarmonicStepValue,&max_output_i_c);
    I3ampPerStepError=HarmonicStepValue-(double)(max_output_i_c*y/32767);



    switch(H_CurrentOutputSel)
    {
    case 1:
        HarmonicStepError=I1ampPerStepError*100/H_Current; //谐波步长造成的误差值
        break;

    case 2:
        HarmonicStepError=I2ampPerStepError*100/H_Current; //谐波步长造成的误差值

        break;

    case 3:
        HarmonicStepError=I3ampPerStepError*100/H_Current; //谐波步长造成的误差值
        break;
    }
    return HarmonicStepError;//2016-5-11谐波changvalue变化范围修改
}





void thread1::key_deal_test_current(int fd)
{
        if(starttest==0)
          {
            emit SIG_Teststate(1);
            GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
              //SetLED();
            emit SIG_testresult(-2);  //清楚结果显示
            emit SIG_DisplayChangeValue(0,0);
            starttest=1;
            m_Ch[0].Output=1;
            m_Ch[1].Output=1;
            m_Ch[2].Output=1;

            m_Ch[4].Output=0;
            m_Ch[5].Output=0;
            m_Ch[6].Output=0;
            H_Ferequencer=SysFrec;
            m_Ch[0].Amp=SysVolt;//57.735; 2009-6-20
            m_Ch[1].Amp=SysVolt;//57.735;
            m_Ch[2].Amp=SysVolt;//57.735;
            m_Ch[0].H_Phase=0;
            m_Ch[1].H_Phase=-120;
            m_Ch[2].H_Phase=120;

            m_Ch[4].Amp=0;
            m_Ch[5].Amp=0;
            m_Ch[6].Amp=0;
            m_Ch[4].H_Phase=0;
            m_Ch[5].H_Phase=0;
            m_Ch[6].H_Phase=0;

            CaculateError();//2016-5-11谐波changvalue变化范围修改

            switch(H_CurrentOutputSel)
            {
            case 1: m_Ch[4].Output=1; break;
            case 2: m_Ch[5].Output=1; break;
            case 3: m_Ch[6].Output=1; break;
            }

            GetReviosnValue(fd);// 获得幅频补偿系数 2011-06-17
            HarmonicCurrentStart=H_Current*HarmonicStart/100;     //FixHarmonic谐波定值 算出谐波电流值
            H_Ferequencer1=H_Ferequencer*HarmonicCount;     //谐波频率
            if(H_Ferequencer1>=100)
              ValueFrec();            //根据频率整定谐波电流输出值
            artExec(fd,"out:dig:on(0x08)",result,0);
            OutputSequencer(fd);
            save_f=0;
          }

}


void thread1::key_deal_stop_current(int fd)
{
    if(starttest==1)
         {
          output_signal_end(fd);
          starttest=0;
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
        CaculateError();
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
    int info_num=0;
    unsigned char info_flag1,info_flag2,info_flag3,info_flag4;
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


       //GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
       //ClearLED();

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
          MAX_I_VALUEDATA_DC=5;
          MIN_I_VALUEDATA_DC=-5;
//          MAX_I_VALUEDATA_AC=30;
//          MIN_I_VALUEDATA_AC=0;
//          MAX_I_VALUEDATA_DC=42;
//          MIN_I_VALUEDATA_DC=-42;
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
   {
     key=GetKey(temprorarydata->UartDev[1].uartfd);
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
         }
     }
     else {
          ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);
          if(info_num++>=10)
          {
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11); //读DSP的各放大器状态信息。第一路地址
          info_flag1=result[8];//第一路地址的DB8~DB15
          info_flag4=result[9];
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);//读DSP的各放大器状态信息。第二路地址
          info_flag2=result[8];//第二路地址的DB8~DB15
          info_flag3=result[9];//第二路地址的DB0~DB7
          logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);//解析放大器的状态信息函数
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


