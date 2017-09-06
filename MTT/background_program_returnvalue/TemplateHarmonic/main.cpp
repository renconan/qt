#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <QDebug>
#include "Config.h"
#include "Defpam.h"
#include "em335x_drivers.h"
#include "Gpio.h"
#include "Testcom2.h"
#include "Uartset.h"
#include "KEY.h"
#include "Phraseseq.h"


extern  TEMPRORARYDATA *temprorarydata;
float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
unsigned char result[70],receive_data[70];
double out_time,change_timedata;
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
int   FreqTriple=2;
int   starttest=0;
int   seq_count,V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;
float TripTime1,TripTime2;
float ChangeValue,timeout;
int save_f;
int sys[4];
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;
//float HARMONICSTART;
//float HARMONICEND;
//float HARMONICSTEP;
//float STEPTIME;
float PEND;
float HOLDTIME;
int   Testitem=3;
float faultduration=10;
float prefaulttime=3; //����ǰ
float Postfaulttime=3; //���Ϻ�
int IACH=1;
int IBCH=0;
int ICCH=0;
int VACH=1;
int VBCH=0;
int VCCH=0;
float CURRENTHARMONIC=0;
int Firstrun=1;

float Preparetime=3;
int current_language;
int   input_par,HelpFlag;
float LogicResolution,SystemFrec,Ferequencer,SystemVolt;

float I1Value,I2Value,I3Value,I1Phase,I2Phase,I3Phase;
float V1Value,V2Value,V3Value,V1Phase,V2Phase,V3Phase;
float HarmonicCurrentStart,Ferequencer1,TripTime,TripValue;
float HarmonicCount,FixHarmonic,Current,Phase;
float VaValueReviosn[10],VbValueReviosn[10],VcValueReviosn[10];
float IaValueReviosn[10],IbValueReviosn[10],IcValueReviosn[10];
float V1OutputValue,V2OutputValue,V3OutputValue;
float I1OutputValue,I2OutputValue,I3OutputValue;
int   CurrentOutputSel;
float HarmonicStart;
float HarmonicEnd;
float HarmonicStep;
float HarmonicStepTime;
bool Threadrunstate;
int TripValueSteps=0;
int   dcouttype;
bool SoftStartEndflag;
double  HarmonicStepError;
extern double max_output_v_a;   //,max_f_v_a=50,min_f_v_a=0.5,output_offset_v_a=0,amp_v_a=360;
extern double max_output_v_b;   //,max_f_v_b=50,min_f_v_b=0.5,output_offset_v_b=0,amp_v_b=120;
extern double max_output_v_c;   //,max_f_v_c=50,min_f_v_c=0.5,output_offset_v_c=0,amp_v_c=240;
extern double max_output_v_z;
extern double max_output_i_a;//,max_f_i_a=50,min_f_i_a=0.5,output_offset_i_a=0,amp_i_a=75;
extern double max_output_i_b;//,max_f_i_b=50,min_f_i_b=0.5,output_offset_i_b=0,amp_i_b=195;
extern double max_output_i_c;//,max_f_i_c=50,min_f_i_c=0.5,output_offset_i_c=0,amp_i_c=315;
void key_deal_test_current(int fd)
{
        if(starttest==0)
          {
            artExec(fd,"out:dig:on(0x04)",result,0);
            char outchar[512];
            starttest=1;
            V1Output=1;
            V2Output=1;
            V3Output=1;

            I1Output=0;
            I2Output=0;
            I3Output=0;
            Ferequencer=SystemFrec;
            V1Value=SystemVolt;//57.735; 2009-6-20
            V2Value=SystemVolt;//57.735;
            V3Value=SystemVolt;//57.735;
            V1Phase=0;
            V2Phase=-120;
            V3Phase=120;

            I1Value=0;
            I2Value=0;
            I3Value=0;
            I1Phase=0;
            I2Phase=0;
            I3Phase=0;



            switch(CurrentOutputSel)
            {
            case 1: I1Output=1; break;
            case 2: I2Output=1; break;
            case 3: I3Output=1; break;
            }
            GetReviosnValue(fd);// ��÷�Ƶ����ϵ�� 2011-06-17
            HarmonicCurrentStart=Current*HarmonicStart/100;     //FixHarmonicг����ֵ ���г������ֵ
            Ferequencer1=Ferequencer*HarmonicCount;     //г��Ƶ��
            if(Ferequencer1>=100)
              ValueFrec();            //����Ƶ���������ֵ
            CURRENTHARMONIC=HarmonicStart;
            sprintf(outchar,"Harmonic:%.3f",CURRENTHARMONIC);
            fprintf(stdout,outchar);
            fflush(stdout);
            artExec(fd,"out:dig:on(0x08)",result,0);
            OutputSequencer(fd);
            if(HarmonicStart>=HarmonicEnd)
                CURRENTHARMONIC=CURRENTHARMONIC-HarmonicStep;
            else
                CURRENTHARMONIC=CURRENTHARMONIC+HarmonicStep;
            save_f=0;
          }

}



int  ReadAutoLogic(int fd)
 {
        int t2=0,t3=0,t4=0,t5=0;
        unsigned char signature;
        char outchar[512];
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
                  TripTime1=-1;
        if(t4==1)
                  TripTime1=1;
        if(t5==1)
          {
            switch(seq_count)
              {
                case 1:
              //   output_signal_end(fd); //�رո����Ŵ���
                 starttest=0;           //�������Խ���
                 if(HarmonicStart>=HarmonicEnd)
                {
                     if(TripTime1==-1&&CURRENTHARMONIC>=HarmonicEnd)
                 {

                        starttest=1;
                        sprintf(outchar,"Harmonic:%.3f",CURRENTHARMONIC);
                        fprintf(stdout,outchar);
                        fflush(stdout);
                        artExec(fd,"out:dig:on(0x08)",result,0);
                        artExec(fd,"out:dig:on(0x74)",result,0);
                        OutputSequencer(fd);
                        CURRENTHARMONIC=CURRENTHARMONIC-HarmonicStep;

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
                     if(TripTime1==-1&&CURRENTHARMONIC<=HarmonicEnd)
                     {

                            starttest=1;
                            sprintf(outchar,"Harmonic:%.3f",CURRENTHARMONIC);
                            fprintf(stdout,outchar);
                            fflush(stdout);
                            artExec(fd,"out:dig:on(0x08)",result,0);
                            artExec(fd,"out:dig:on(0x74)",result,0);
                            OutputSequencer(fd);
                            CURRENTHARMONIC=CURRENTHARMONIC+HarmonicStep;

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
       char outchar[512];
       double u,v,w;
       static double SequencerTime4;
       static double SequencerTime2;
       float ChangeValue;
       int stepnum;

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
             SequencerTime2=gettime(u,v,w);
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
         {//δ����
           TripTime=-1;

         }
       if(t4==1)
         {//����
           TripTime=1;
         }
       if(t5==1)
       {
           //output_signal_end(fd);
           starttest=0;           //�������Խ���
           if(HarmonicStart>=HarmonicEnd)
          {
               if(TripTime1==-1&&CURRENTHARMONIC>=HarmonicEnd)
           {

                  starttest=1;
                  sprintf(outchar,"Harmonic:%.3f",CURRENTHARMONIC);
                  fprintf(stdout,outchar);
                  fflush(stdout);
                  artExec(fd,"out:dig:on(0x08)",result,0);
                  artExec(fd,"out:dig:on(0x74)",result,0);
                  OutputSequencer(fd);
                  CURRENTHARMONIC=CURRENTHARMONIC-HarmonicStep;

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
               if(TripTime1==-1&&CURRENTHARMONIC<=HarmonicEnd)
               {

                      starttest=1;
                      sprintf(outchar,"Harmonic:%.3f",CURRENTHARMONIC);
                      fprintf(stdout,outchar);
                      fflush(stdout);
                      artExec(fd,"out:dig:on(0x08)",result,0);
                      artExec(fd,"out:dig:on(0x74)",result,0);
                      OutputSequencer(fd);
                      CURRENTHARMONIC=CURRENTHARMONIC+HarmonicStep;

               }
               else
               {

                    output_signal_end(fd); //�رո����Ŵ���
                    starttest=0;           //�������Խ���
                    save_f=1;//����������־��1
               }
           }
//           if(TripTime==-1)
//           {
//               TripValue=-1;

//               starttest=0;
//               save_f=1;
//               emit SIG_testresult(TripValue);
//           }
//           else
//           {

//                TripValue=HarmonicStart-TripValueSteps*(HarmonicStep/*-HarmonicStepError*/);

//               starttest=0;
//               save_f=1;
//               emit SIG_testresult(TripValue);

//           }
//              emit SIG_TestState();
       }

}


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



void   Writeexitstate(float state)
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
        HarmonicCount=2;
        FixHarmonic=15;
        Current=5;
        Phase=0;

        V1DC=0;
        V2DC=0;
        V3DC=0;
        I1DC=0;
        I2DC=0;
        I3DC=0;

        FILE *fp;
        if((fp=fopen("sysset.txt","a+"))==NULL)
        return;
        //fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&prefaulttime,&Postfaulttime,&dcouttype);
        fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
               &SystemVolt,&LogicResolution,&prefaulttime,&Postfaulttime,&Preparetime,&current_language);
        fclose(fp);

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


//���������ʽ  ./������  Fset Tset  df/dt   Vblock testitem  faultduration
int main(int argc, char *argv[])
{
    int rc=0;
    int info_num=0;
    KEY key;
    char info_flag1,info_flag2,info_flag3,info_flag4;

    if(argc>1)
    {
    HarmonicCount=atof(argv[1]);
    Current=atof(argv[2]);
    Phase=atof(argv[3]);
    HarmonicStart=atoi(argv[4]);
    HarmonicEnd=atof(argv[5]);
    HarmonicStep=atof(argv[6]);
    HarmonicStepTime=atof(argv[7]);
    int IACH,IBCH,ICCH;
    IACH=atof(argv[8]);
    IBCH=atof(argv[9]);
    ICCH=atof(argv[10]);
    if(IACH)
        CurrentOutputSel=1;
    else if(IBCH)
        CurrentOutputSel=2;
    else if(ICCH)
        CurrentOutputSel=3;
    }
    else return -1;
    init_par();
  temprorarydata=new TEMPRORARYDATA;
  temprorarydatainit();
  UartsInit();
   // temprorarydata->gpio_fd=open("/dev/em335x_gpio", O_RDWR);


    {
    online(temprorarydata->UartDev[0].uartfd);
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
    }

    key_deal_test_current(temprorarydata->UartDev[0].uartfd);
    while(1){
           key=GetKey(temprorarydata->UartDev[1].uartfd);
           if(key==_STOP)
               starttest=0;

           ReadTemplateTestState(); //����ļ���Ϣ��Templateprogram д��



           if(starttest==1)
          {
               ReadAutoLogic(temprorarydata->UartDev[0].uartfd); //����ֵ�����ߵĺ���
//               if(info_num++>10)
//               {
//                artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);
//                info_flag1=result[8];
//                artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
//                info_flag2=result[8];
//                info_flag3=result[9];
//                logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);
//                //����������Ϣд���ļ�
//             //   Writelogicinfofile(info_flag1,info_flag2,info_flag3,V1Output,V2Output,V3Output,I1Output,I2Output,I3Output);
//                info_num=0;
//                }
           if(starttest==0&&save_f==1)
           {

               artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x80)",result,0);
               char strbuf[100];
               char outchar[512];
               strcpy(outchar,"WARNING=Run normally");
               fprintf(stdout,outchar);
               fflush(stdout);
               FILE *fp;
               fp=fopen(TemplateReport,"a+");
               if(fp==NULL)
               return 0;
               if(/*TestItem1==*/1)
               {
                           if(TripTime1==-1)
                            { strcpy(strbuf,"TRIPVALUE=-1");
                              Writeexitstate(-1);
                            }
                           else
                            {
                                if(HarmonicStart>=HarmonicEnd)
                                {
                                 CURRENTHARMONIC=CURRENTHARMONIC+HarmonicStep;
                                }
                                else
                                {
                                 CURRENTHARMONIC=CURRENTHARMONIC-HarmonicStep;

                                }
                              sprintf(strbuf,"TRIPVALUE=%0.3f",CURRENTHARMONIC);
                              Writeexitstate(CURRENTHARMONIC);
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
            if(info_num++>20)
           {
            artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);
            info_flag1=result[8];
            artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
            info_flag2=result[8];
            info_flag3=result[9];
            logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);
            //����������Ϣд���ļ�
        //    Writelogicinfofile(info_flag1,info_flag2,info_flag3,V1Output,V2Output,V3Output,I1Output,I2Output,I3Output);
            info_num=0;
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


               //д������״̬�ļ� ��ʾ�Ѿ��˳����� �ļ��� -1��ʾδ����  -2��ʾSTOP�����˳�����  >0 ��������˳�����
                  Writeexitstate(-2);
            //   GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);  //��
                  return 0;
           }
       }
  }


