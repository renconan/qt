#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "Testcom2.h"
#include "Curseq.h"
#include "Defpam.h"
#include "Gpio.h"
#include "Config.h"
#include "em335x_drivers.h"
#include <QDebug>

extern TEMPRORARYDATA *temprorarydata;


extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

extern  unsigned char result[70],receive_data[70];
extern int   seq_count,SeqStep;

extern int   starttest;
extern int   I1DC,I2DC,I3DC;
extern int   TestItem1,TestItem3,TestItem2,OutputMode;

extern float LogicResolution,SystemFrec;
extern float TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime,ReturnQuotiety;
extern float ChangeValue,timeout;
extern float TripTime1,TripTime2,TripTime3;

extern int    save_f;
extern int    sys[4];
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;


extern float SystemVolt;
extern float IENDSTART;
extern float IENDEND;
extern float ISTEP;
extern float STEPTIME;
extern int   OutputMode;

extern float IEND;
extern float HOLDTIME;
extern int   Testitem;
extern float faultduration;
extern float prefaulttime; //����ǰʱ��
extern float CurrentI;



int  Buzzer(char Address1,char Address2,char Address3)
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
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //����Ҫ�ؼ�
          artExec(fd,"out:dig:on(0x80)",result,0);
        else
          artExec(fd,"out:dig:off(0x80)",result,0);
      }

    if(Buzzer(info_result1,info_result2,info_result3)==0) //����Ҫ�ؼ�
      {
        //��������
          showdspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
             showdspstate(1);
            //IA����
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
              showdspstate(2);

            //IABC �¶ȱ���
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //IC��Ъ
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
          {  //info_result1�ǵ�1·��ַ��DB9~DB15��

                showdspstate(4);
            //IB����
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1�ǵ�1·��ַ��DB8~DB15��

                showdspstate(5);
           //IC����
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
                showdspstate(6);
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
                showdspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1�ǵ�1·��ַ��DB8~DB15��
          //IABC ��Դ����
                showdspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
                showdspstate(9);
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
                showdspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��
                showdspstate(11);
              //V2����
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3�ǵ�2·��ַ��DB0~DB7��

              //V3��Ъ
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
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
            //V2����
                showdspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��  //DB8
           // VABC �¶ȱ���
              showdspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
              showdspstate(15);
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

               showdspstate(16);
            //VC����
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��  //DB13
              showdspstate(17);
            //VABC��Դ����
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
              showdspstate(18);


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
              showdspstate(19);

             //ֱ������
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��
            showdspstate(20);

            // VABC �¶ȱ���
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3�ǵ�2·��ַ��DB0~DB7��
              showdspstate(21);
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


void ReadAutoLogic(int fd)
{
        int t4=0,t3=0,t5=0,t2=0;
        unsigned char signature;
        static double SequencerTime4;
        double u,v,w;
        float VDS,Temp;
        char outchar[512];
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
          TripTime1=-1;
        if(t4==1)
          TripTime1=1;

        if(t5==1)
        {
            switch(seq_count)
              {
                case 1:
                  //    output_signal_end(fd); //�رո����Ŵ���
                      starttest=0;           //�������Խ���
                     if(IENDSTART>=IENDEND)
                     {
                      if(TripTime1==-1&&CurrentI>=IENDEND)
                      {
                             starttest=1;
                             sprintf(outchar,"Cur(A):%.3f",CurrentI);
                             fprintf(stdout,outchar);
                             fflush(stdout);
                             artExec(fd,"out:dig:on(0x08)",result,0);
                             artExec(fd,"out:dig:on(0x04)",result,0);
                             OutputSequencer1(fd);
                             CurrentI=CurrentI-ISTEP;

                      }
                      else
                      {
                           output_signal_end(fd); //�رո����Ŵ���
                           starttest=0;           //�������Խ���
                           save_f=1;//����������־��1
                      }
                     }
                      else
                      {
                          if(TripTime1==-1&&CurrentI<=IENDEND)
                          {

                                 starttest=1;
                                 sprintf(outchar,"Cur(A):%.3f",CurrentI);
                                 fprintf(stdout,outchar);
                                 fflush(stdout);
                                 artExec(fd,"out:dig:on(0x08)",result,0);
                                 artExec(fd,"out:dig:on(0x04)",result,0);
                                 OutputSequencer1(fd);
                                 CurrentI=CurrentI+ISTEP;

                          }
                          else
                          {
                               output_signal_end(fd); //�رո����Ŵ���
                               starttest=0;           //�������Խ���
                               save_f=1;//����������־��1
                          }
                      }

                      break;
              }


        }
}


void ReadTripTimeLogic(int fd)
{

        int t2=0,t3=0,t4=0,t5=0;
        unsigned char signature;
        double u,v,w;
        float  Temp;
        static double SequencerTime2,SequencerTime4;
        artExec(fd,"inp:buf:get?",result,15);   //��DSP�õ��������ӵ���Ϣ��
        u=result[7];
        v=result[8];
        w=result[9];
        signature=int(result[13]&0x7f);
       switch(signature)
          {
          case 1:
          SeqStep=0;
          break;
       case 2:
         t2=1;
         SequencerTime2=gettime(u,v,w);//����ӿ�ʼ���Ե�sequencer��Step2��ʱ�䡣
         break;
       case 3:
         t3=1;
         break;
       case 4:
         t4=1;
         SequencerTime4=gettime(u,v,w);//����ӿ�ʼ���Ե�sequencer��Step4��ʱ�䡣
         break;
       case 5:
        t5=1;
         break;
          }



        if(t3==1)
          {
            TripTime3=-1;
          }
        if(t4==1)
          {
             TripTime3=SequencerTime4-SequencerTime2-LogicResolution/1000;
             Temp=TripTime3*1000;
             Temp=ceil(Temp);
             TripTime3=Temp/1000;     //������ʱ���������С��������ֻ����λ
                        //����������־��1
          }
        if(t5==1)
        {
            output_signal_end(fd);   //�رո����Ŵ���
            starttest=0;
            save_f=1;
        }
}
