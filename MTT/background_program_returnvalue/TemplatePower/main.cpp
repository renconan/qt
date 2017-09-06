#include <stdlib.h>
#include <stdio.h>
#include <QFile>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>
#include "Config.h"
#include "Defpam.h"
#include <QSemaphore>
#include "Powerseq.h"
#include "Testcom2.h"
#include "KEY.h"


#include "Gpio.h"
#include "em335x_drivers.h"
#include "Uartset.h"
#include "PowerReadlogic.h"



 TEMPRORARYDATA *temprorarydata;
 float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
 float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
 float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;



 unsigned char result[70],receive_data[70];
 double out_time,change_timedata;

 float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
 float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
 int   input_par=0,Decimal=0,sige_flag=0,HelpFlag=0;

 int   INPAR=0;
 int   starttest=0;

 int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC,SequencerCount;
 float LogicResolution,SystemFrec,Angle1,Angle2;
 int exit_par,save_f;
 int sys[4];
 char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
 float prefaulttime=3;

 float FixCurrent,FixVoltage,MSAngle,StartPhase,EndPhase,OutPhase,StepPhase,StepTime=5;

        float Triptime=0;

 float  SystemVolt=57;
 float  faultduration=0;
 int    faulttype=1; //A-E
 float  mta;

 int StartPhaseActState=0;  //0δ����,1����
 int SearchType=0;  //��������  0 ��ʱ��  1 ˳ʱ��

 QVector<ACTSTATE> AngleActStateVect; //

 int   Firstrun=1;
 float Preparetime=3;
 float Posttime=3;





void key_deal_test_power(int fd)
{

    if(starttest==0)
      {
        AngleActStateVect.clear();
        Angle1=-1000;
        Angle2=-1000;
        SearchType=0;
        starttest=1;
        OutPhase=StartPhase;
        SequencerCount=1;
        artExec(fd,"out:dig:on(0x08)",result,0);
        outputchar();
        OutputSequencer(fd);
        save_f=0;
      }
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

void init_par() //������ʼ��
{
   FILE *fp;
   LogicResolution=3;
   SystemFrec=25;
   SystemVolt=57.735; //ϵͳ��ѹ������ϵͳ��ѹ�͹��ڲ�һ������Ҫ��ϵͳ����������
   if((fp=fopen(SYSSET,"a+"))==NULL)
   return;
   fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&prefaulttime,&Posttime,&Preparetime);
   fclose(fp);
   exit_par=0;


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




int main(int argc, char *argv[])
{
    int rc;

    int info_num=0;
    KEY key;
    char info_flag1,info_flag2,info_flag3;

    if(argc>1)
    {
    FixCurrent=atof(argv[1]);
    FixVoltage=atof(argv[2]);
    mta=atof(argv[3]);
    faulttype=atoi(argv[4]);
    StepPhase=atof(argv[5]);
    StepTime=atof(argv[6]);
    StartPhase=mta;
    }
    init_par();
    
  temprorarydata=new TEMPRORARYDATA;
  temprorarydatainit();
  UartsInit();


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
     //�����ִ�г���һ����ʼ���У����ʾ�Ѿ���ʼ��ʼ������
     key_deal_test_power(temprorarydata->UartDev[0].uartfd);


     while(1)
     {
              key=GetKey(temprorarydata->UartDev[1].uartfd);
              if(key==_STOP)
              {
               starttest=0;
              }

              ReadTemplateTestState();

              if(starttest==1)
            {

               ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);
              //�ڲ��Զ���ʱ��ʱ��������״̬
              if(starttest==0&&save_f==1)
              {
                  //���ر�����Խ�� ��д���˳� ״̬    ���˳�����
                  artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x80)",result,0);
                  char strbuf[100];

                  char outchar[512];
                  strcpy(outchar,"WARNING=Run normally");
                  fprintf(stdout,outchar);
                  fflush(stdout);
                  if(Angle1>360)
                    Angle1=Angle1-360;
                  else if(Angle1<-360)
                    Angle1+=360;
                  if(Angle2>360)
                    Angle2=Angle2-360;
                  else if(Angle2<-360)
                    Angle2+=360;

                  MSAngle=(Angle1+Angle2)/2;
                  if(MSAngle>360)
                    MSAngle=MSAngle-360;
                  else if(MSAngle<-360)
                    MSAngle+=360;

                   MSAngle=-MSAngle;
                  if(MSAngle<-180)
                      MSAngle=MSAngle+360;
                  else if(MSAngle>180)
                      MSAngle=MSAngle-360;

                  FILE *fp;
                  fp=fopen(TemplateReport,"a+");
                  if(fp==NULL)
                  return 0;

                  sprintf(strbuf,"ACTANGLE1=%.2f",Angle1);
                  fprintf(fp,"%s\n",strbuf);
                  sprintf(strbuf,"ACTANGLE2=%.2f",Angle2);
                  fprintf(fp,"%s\n",strbuf);
                  sprintf(strbuf,"PHRASEMTA=%.2f",MSAngle);
                  fprintf(fp,"%s\n",strbuf);
                  fclose(fp);
                  output_signal_end(temprorarydata->UartDev[0].uartfd);
                               artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
                               close(temprorarydata->UartDev[1].uartfd);
                               close(temprorarydata->UartDev[0].uartfd);
                                  free(temprorarydata);
                                  temprorarydata=NULL;
                  Writeexitstate(MSAngle);
                  ClearTemplateTestState();
                  return 0;
              }

              if(info_num++>10)
             {
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
                  //д������״̬�ļ� ��ʾ�Ѿ��˳����� �ļ��� -1��ʾ STOP �����˳����� 0��ʾ ��������˳�����
                  ClearTemplateTestState();
                  Writeexitstate(-1000);
                  return 0;
              }
}
}
