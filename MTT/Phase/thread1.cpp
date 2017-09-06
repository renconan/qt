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
extern float prefaulttime; //����ǰ
extern int input_par;
extern int HelpFlag;


extern int TriptimeType; //����ʱ��ķ�ʽ
extern float StopPhase; //���Զ���ʱ�����λ
extern float TripTimeHoldtime; //������ʱ��ı���ʱ��
extern float postfaulttime; //���Ϻ�ʱ��


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
  {//sys[1]:���أ�sys[2]:��Ъ��sys[3]:����
    if(sys[1]==1)
      {//������Ϣ
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1����
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2����
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3����
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1����
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2����
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3����
        if((Address3&0x20)!=0x00)
          return 1;//ֱ������
      }
    else if(sys[1]==0)
      {//������Ϣ
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1����
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2����
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3����
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1����
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2����
        if((Address3&0x40)!=0x00&&V3Output==1)
          return -1;//V3����
        if((Address3&0x20)!=0x00)
          return 1;//ֱ������
      }
    if(sys[2]==1)
      {//��Ъ��Ϣ
        if((Address1&0x10)!=0x00)
          return 1;//I1��Ъ
        if((Address1&0x04)!=0x00)
          return 1;//I2��Ъ
        if((Address1&0x01)!=0x00)
          return 1;//I3��Ъ
        if((Address2&0x08)!=0x00)
          return 1;//V1��Ъ
        if((Address2&0x04)!=0x00)
          return 1;//V2��Ъ
        if((Address3&0x80)!=0x00)
          return 1;//V3��Ъ
      }
    else if(sys[2]==0)
      {//��Ъ��Ϣ
        if((Address1&0x10)!=0x00)
          return -1;//I1��Ъ
        if((Address1&0x04)!=0x00)
          return -1;//I2��Ъ
        if((Address1&0x01)!=0x00)
          return -1;//I3��Ъ
        if((Address2&0x08)!=0x00)
          return -1;//V1��Ъ
        if((Address2&0x04)!=0x00)
          return -1;//V2��Ъ
        if((Address3&0x80)!=0x00)
          return -1;//V3��Ъ
      }
    if(sys[3]==1)
      {//������Ϣ
        if((Address1&0x80)!=0x00)
          return 1;//I1����
        if((Address1&0x40)!=0x00)
          return 1;//I2����
        if((Address1&0x20)!=0x00)
          return 1;//I3����
        if((Address2&0x20)!=0x00)
          return 1;//V1����
        if((Address2&0x40)!=0x00)
          return 1;//V2����
        if((Address2&0x01)!=0x00)
          return 1;//V3����
      }
    else if(sys[3]==0)
      {//������Ϣ
        if((Address1&0x80)!=0x00)
          return -1;//I1����
        if((Address1&0x40)!=0x00)
          return -1;//I2����
        if((Address1&0x20)!=0x00)
          return -1;//I3����
        if((Address2&0x20)!=0x00)
          return -1;//V1����
        if((Address2&0x40)!=0x00)
          return -1;//V2����
        if((Address2&0x01)!=0x00)
          return -1;//V3����
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
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //����Ҫ�ؼ�
          artExec(fd,"out:dig:on(0x80)",result,0);
        else
          artExec(fd,"out:dig:off(0x80)",result,0);
      }

    if(Buzzer(info_result1,info_result2,info_result3)==0) //����Ҫ�ؼ�
      {
        //��������
          emit SIG_dspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
           emit SIG_dspstate(1);
            //IA����
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
            emit SIG_dspstate(2);

            //IABC �¶ȱ���
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //IC��Ъ
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
          {  //info_result1�ǵ�1·��ַ��DB9~DB15��

              emit SIG_dspstate(4);
            //IB����
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1�ǵ�1·��ַ��DB8~DB15��

              emit SIG_dspstate(5);
           //IC����
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
              emit SIG_dspstate(6);
          //IA��Ъ
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
          { //info_result2�ǵ�2·��ַ��DB9~DB15��
           //IC����
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1�ǵ�1·��ַ��DB8~DB15��
          //IABC ��Դ����
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
              emit SIG_dspstate(9);
          //I2��Ъ  ?�¶ȱ�����Ъ
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
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
           //VA����
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��
              emit SIG_dspstate(11);
              //V2����
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3�ǵ�2·��ַ��DB0~DB7��

              //V3��Ъ
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
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
            //V2����
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��  //DB8
           // VABC �¶ȱ���
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
            emit SIG_dspstate(15);
           //VA��Ъ
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
          { //info_result3�ǵ�2·��ַ��DB0~DB7��

             emit SIG_dspstate(16);
            //VC����
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��  //DB13
            emit SIG_dspstate(17);
            //VABC��Դ����
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
            emit SIG_dspstate(18);


          //Vz����

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
          {//info_result3�ǵ�2·��ַ��DB0~DB7��
            emit SIG_dspstate(19);

             //ֱ������
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��
          emit SIG_dspstate(20);

            // VABC �¶ȱ���
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3�ǵ�2·��ַ��DB0~DB7��
            emit SIG_dspstate(21);
           //V3��Ъ
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
    artExec(fd,"inp:buf:get?",result,15);//��DSP�õ��������ӵ���Ϣ��
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
          SequencerTime2=gettime(u,v,w);//����ӿ�ʼ���Ե�sequencer��Step2��ʱ�䡣
          break;
        case 3:
          t3=1;
          break;
        case 4:
          SequencerTime4=gettime(u,v,w);//����ӿ�ʼ���Ե�sequencer��Step4��ʱ�䡣
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
              TripTime2=Temp/1000;//������ֵ��������С��������ֻ����λ]
              DisplayResult(2);
              break;
          }
      }
    if(t5==1)
      {
        switch(seq_count)
          {
            case 1:
             output_signal_end(fd); //�رո����Ŵ���
             starttest=0;           //�������Խ���
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
                  output_signal_end(fd); //�رո����Ŵ���
                  starttest=0;           //�������Խ���
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
                      output_signal_end(fd); //�رո����Ŵ���
                      starttest=0;           //�������Խ���
                      GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                      DisplayResult(1);
                      emit SIG_DisplayChangeValue(0,0);

                 }
             }
            }


             if(TestItem2==1&&(TripTime1==1||(TripTime1==-1&&TriptimeType==2)))
             {
                  DisplayResult(1);
                  output_signal_end(fd); //�رո����Ŵ���
                  starttest=1;           //�������Խ���
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
                  output_signal_end(fd); //�رո����Ŵ���
                  starttest=0;           //�������Խ���
                  GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                  save_f=1;//����������־��1
              }
                break;
            case 2://���Զ���ʱ�䴦��
                  output_signal_end(fd);//�رո����Ŵ���
                  starttest=0; //�������Խ���
                  GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);
                  save_f=1;//����������־��1
                  break;
         }
    }

}

void thread1::key_deal_test_current(int fd)
{

    if(starttest==0)
      {

        GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);  //��
        emit SIG_testresult(0);  //��ղ��Խ������
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=1;
        if(TestItem1==1)
          { //Ҫ����Trip value
            starttest=1;
            seq_count=1;
            currentphrase=PENDSTART;
            DisplayResult(0);
            artExec(fd,"out:dig:on(0x74)",result,0);
            emit SIG_DisplayChangeValue(1,currentphrase);
            artExec(fd,"out:dig:on(0x08)",result,0);
            OutputSequencer1(fd);//�������Trip value��sequencer

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
          {//ֻ���Զ��� ��
            starttest=1;
            PEND=StopPhase;
            seq_count=2;
            DisplayResult(0);
            artExec(fd,"out:dig:on(0x08)",result,0);
            artExec(fd,"out:dig:on(0x74)",result,0);
            OutputSequencer2(fd);//������Զ��� ���sequencer
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
          emit SIG_testresult(0);  //��ղ��Խ������
          emit SIG_DisplayChangeValue(0,0); //���ֵ������0
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
      GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
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
     //�򿪲����ô���
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
   GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
   read_max(temprorarydata->UartDev[0].uartfd);
   artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);

       if((result[7]&0xf0)==0x60)
        { //125V��ѹ��
          MAX_V_VALUEDATA_AC=300;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=424;
          MIN_V_VALUEDATA_DC=-424;
        }
       else if((result[7]&0xf0)==0x80)
        { //125V��ѹ��
          MAX_V_VALUEDATA_AC=125;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=176;
          MIN_V_VALUEDATA_DC=-176;
        }
      else if((result[7]&0xf0)==0x00)
        { //75V��ѹ��
          MAX_V_VALUEDATA_AC=75;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=110;
          MIN_V_VALUEDATA_DC=-110;
        }
      if((result[7]&0x0f)==0x01)
        { //30A������
          MAX_I_VALUEDATA_AC=30;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=42;
          MIN_I_VALUEDATA_DC=-42;
        }
      else if((result[7]&0x0f)==0x08)
        { //40A������
          MAX_I_VALUEDATA_AC=40;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=56;
          MIN_I_VALUEDATA_DC=-56;
        }
      else if((result[7]&0x0f)==0x05)
        { //50A������
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
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //����Ҫ����
         info_flag1=result[8];
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
         info_flag2=result[8];
         info_flag3=result[9];
         logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);  //��Ҫemit
         info_num=0;
       }
       }
     else { 
         ReadAutoLogic(temprorarydata->UartDev[0].uartfd); //��������״̬
         if(starttest==0)
         GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
         if(info_num++>5)
         {
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);          //����Ҫ����
          info_flag1=result[8];
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
          info_flag2=result[8];
          info_flag3=result[9];
          logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);  //��Ҫemit
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


