#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include <QFile>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>
#include <time.h>
#include <signal.h>
#include "Config.h"
#include "Defpam.h"
#include "Testcom2.h"
#include "Curseq.h"          //电流测试的seq
#include "CurReadlogic.h"
#include "Gpio.h"
#include "em335x_drivers.h"
#include "Uartset.h"



TEMPRORARYDATA *temprorarydata;
int info_num=0;
float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
unsigned char result[70],receive_data[70];
double out_time,change_timedata;

int   seq_count,SeqStep=0;

float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;

int   cx,cy,cx_start,cx_mid,cx_end,increase,starttest=0,low,row,page;
int   I1DC,I2DC,I3DC;
int   V1DC,V2DC,V3DC;
int   TestItem1,TestItem3,TestItem2,OutputMode;
float StartCurrent,EndCurrent,ActStep,ReturnStep,StepTime;
float LogicResolution,SystemFrec;
float TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime,ReturnQuotiety;
float   TripTime1=-1,TripTime2=-1,TripTime3=-1;
float ChangeValue,timeout;
int   exit_par,save_f;
int    sys[4];
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

float SystemVolt=57;
int template_runstate;



float IENDSTART;
float IENDEND;
float ISTEP;
float STEPTIME;
float CurrentI;

float IEND;
float HOLDTIME;
int   Testitem=3;
float faultduration=10;
float prefaulttime=3; //故障前

float V1Value,V1Phase;
float V2Value,V2Phase;
float V3Value,V3Phase;


float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase;

float PhaseIA;
float PhaseIB;
float PhaseIC;

float Vfault=40;
float Angle=15;
float VpnValue; //单相的幅值
float VpnAngle;
float Posttime=3;
float PrepareTime=3;

int Firstrun=1; //第一次运行



void init_par() //参数初始化
{
        I1DC=0;
        I2DC=0;
        I3DC=0;
        V1DC=0;
        V2DC=0;
        V3DC=0;

        MAX_V_VALUEDATA_DC=176;
        MIN_V_VALUEDATA_DC=-176;
        MAX_V_VALUEDATA_AC=125;
        MIN_V_VALUEDATA_AC=0;
        MAX_I_VALUEDATA_DC=70;
        MIN_I_VALUEDATA_DC=-70;
        MAX_I_VALUEDATA_AC=50;
        MIN_I_VALUEDATA_AC=0;
        sys[0]=1;
        sys[1]=1;
        sys[2]=1;
        sys[3]=1;
        LogicResolution=3;
        SystemFrec=25;
        FILE *fp;
        if((fp=fopen(SYSSET,"a+"))==NULL)
        return;
        fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&prefaulttime,&Posttime,&PrepareTime);
        fclose(fp);
}


void   Writelogicinfofile(unsigned char info1,unsigned char info2,unsigned char info3,char V1,char V2,char V3,char I1,char I2,char I3)
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


void key_deal_test_current(int fd)
{

       char outchar[512];
       if(starttest==0)
         {

           starttest=1;
           V1Output=1;
           V2Output=1;
           V3Output=1;
           I1Output=0;
           I2Output=0;
           I3Output=0;
           switch(OutputMode)
             {
               case 1:
                  I1Output=1;
                  V1Value=Vfault;
                  V2Value=SystemVolt;
                  V3Value=SystemVolt;
                  V1Phase=0;
                  V2Phase=-120;
                  V3Phase=120;
                  PhaseIA=0-Angle;
                  PhaseIB=0;
                  PhaseIC=0;

                  V1PerFaultPhase=V1Phase-360*SystemFrec*prefaulttime;
                  V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                  V2PerFaultPhase=V1PerFaultPhase-120;
                  V3PerFaultPhase=V1PerFaultPhase+120;
                  if(V2PerFaultPhase<=-360)
                      V2PerFaultPhase+=360;
                  if(V3PerFaultPhase>=360)
                      V2PerFaultPhase-=360;

                   break;
               case 2:
                  I2Output=1;
                  V1Value=SystemVolt;
                  V2Value=Vfault;
                  V3Value=SystemVolt;
                  V1Phase=120;
                  V2Phase=0;
                  V3Phase=-120;
                  PhaseIB=0-Angle;
                  PhaseIA=0;
                  PhaseIC=0;

                  V2PerFaultPhase=V2Phase-360*SystemFrec*prefaulttime;
                  V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
                  V1PerFaultPhase=V2PerFaultPhase+120;
                  V3PerFaultPhase=V2PerFaultPhase-120;
                  if(V1PerFaultPhase>=360)
                     V1PerFaultPhase=V1PerFaultPhase-360;
                   if(V3PerFaultPhase<=-360)
                     V3PerFaultPhase+=360;
                   break;
               case 3:
                  I3Output=1;
                  V1Value=SystemVolt;
                  V2Value=SystemVolt;
                  V3Value=Vfault;
                  V1Phase=-120;
                  V2Phase=120;
                  V3Phase=0;
                  PhaseIC=0-Angle;
                  PhaseIA=0;
                  PhaseIB=0;
                  V3PerFaultPhase=V3Phase-360*SystemFrec*prefaulttime;
                  V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
                  V1PerFaultPhase=V3PerFaultPhase-120;
                  V2PerFaultPhase=V3PerFaultPhase+120;
                  if(V2PerFaultPhase>=360)
                     V2PerFaultPhase=V2PerFaultPhase-360;
                   if(V1PerFaultPhase<=-360)
                     V1PerFaultPhase+=360;

                   break;

               case 4:
                 I1Output=1;
                 I2Output=1;
                 VpnValue=sqrt(SystemVolt*SystemVolt/4+Vfault*Vfault/4);
                 VpnAngle=atan(Vfault/SystemVolt)*180/3.1415+180;
                 V1Value=VpnValue;
                 V2Value=VpnValue;
                 V3Value=SystemVolt;
                 V2Phase=-90-VpnAngle;
                 V1Phase=-90+VpnAngle;
                 V3Phase=-90;

                 PhaseIC=V3Phase-Angle;
                 PhaseIA=PhaseIC-90;
                 PhaseIB=PhaseIC+90;

                 V3PerFaultPhase=V3Phase-360*SystemFrec*prefaulttime;
                 V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
                 V1PerFaultPhase=V3PerFaultPhase-120;
                 V2PerFaultPhase=V3PerFaultPhase+120;
                 if(V2PerFaultPhase>=360)
                    V2PerFaultPhase=V2PerFaultPhase-360;
                  if(V1PerFaultPhase<=-360)
                    V1PerFaultPhase+=360;

                 break;

             case 5:
                 I3Output=1;
                 I2Output=1;
                 VpnValue=sqrt(SystemVolt*SystemVolt/4+Vfault*Vfault/4);
                 VpnAngle=atan(Vfault/SystemVolt)*180/3.1415+180;

                V1Value=SystemVolt;
                V3Value=VpnValue;
                V2Value=VpnValue;

                V1Phase=-90;
                V3Phase=-90-VpnAngle;
                V2Phase=-90+VpnAngle;


                PhaseIA=V1Phase-Angle;
                PhaseIB=PhaseIA-90;
                PhaseIC=PhaseIA+90;


                V1PerFaultPhase=V1Phase-360*SystemFrec*prefaulttime;
                V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                V2PerFaultPhase=V1PerFaultPhase-120;
                V3PerFaultPhase=V1PerFaultPhase+120;
                if(V2PerFaultPhase<=-360)
                    V2PerFaultPhase+=360;
                if(V3PerFaultPhase>=360)
                    V2PerFaultPhase-=360;



               break;

             case 6:
               I3Output=1;
               I1Output=1;

               VpnValue=sqrt(SystemVolt*SystemVolt/4+Vfault*Vfault/4);
               VpnAngle=atan(Vfault/SystemVolt)*180/3.1415+180;
               V1Value=VpnValue;
               V3Value=VpnValue;
               V2Value=SystemVolt;

               V2Phase=-90;
               V1Phase=-90-VpnAngle;
               V3Phase=-90+VpnAngle;


               PhaseIB=V2Phase-Angle;
               PhaseIA=PhaseIB+90;
               PhaseIC=PhaseIB-90;

               V2PerFaultPhase=V2Phase-360*SystemFrec*prefaulttime;
               V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
               V1PerFaultPhase=V2PerFaultPhase+120;
               V3PerFaultPhase=V2PerFaultPhase-120;
               if(V1PerFaultPhase>=360)
                  V1PerFaultPhase=V1PerFaultPhase-360;
                if(V3PerFaultPhase<=-360)
                  V3PerFaultPhase+=360;





               break;

               case 7:
                  I1Output=1;
                  I2Output=1;
                  I3Output=1;
                  V2Phase=-120;
                  V3Phase=120;
                  V1Phase=0;


                  V1Value=Vfault;
                  V3Value=Vfault;
                  V2Value=Vfault;
                  PhaseIA=0-Angle;
                  PhaseIB=0-Angle-120;
                  PhaseIC=0-Angle+120;

                  V1PerFaultPhase=V1Phase-360*SystemFrec*prefaulttime;
                  V2PerFaultPhase=V2Phase-360*SystemFrec*prefaulttime;
                  V3PerFaultPhase=V3Phase-360*SystemFrec*prefaulttime;
                  V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                  V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
                  V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;

                   break;
              }


           if(TestItem1==1)
             {
                 seq_count=1;
                 starttest=1;
                 CurrentI=IENDSTART;
                 sprintf(outchar,"Cur(A):%.3f",CurrentI);
                 fprintf(stdout,outchar);
                 fflush(stdout);
                 artExec(fd,"out:dig:on(0x04)",result,0);
                 artExec(fd,"out:dig:on(0x08)",result,0);
                 OutputSequencer1(fd);
                 if(IENDSTART>=IENDEND)
                  CurrentI=CurrentI-ISTEP;
                 else
                  CurrentI=CurrentI+ISTEP;

             }
           else if(TestItem1==0&&TestItem3==1)
             {   //只测试动作时间
               seq_count=3;
               switch(OutputMode)
               {
               case 1:
                   sprintf(outchar,"A-E:%.3fA",IEND);
                   break;
               case 2:
                   sprintf(outchar,"B-E:%.3fA",IEND);
                   break;
               case 3:
                   sprintf(outchar,"C-E:%.3fA",IEND);
                   break;
               case 4:
                   sprintf(outchar,"A-B:%.3fA",IEND);
                   break;
               case 5:
                   sprintf(outchar,"B-C:%.3fA",IEND);
                   break;
               case 6:
                   sprintf(outchar,"C-A:%.3fA",IEND);
                   break;
               case 7:
                   sprintf(outchar,"A-B-C:%.3fA",IEND);
                   break;
               }
               fprintf(stdout,outchar);
               fflush(stdout);

               artExec(fd,"out:dig:on(0x04)",result,0);
               artExec(fd,"out:dig:on(0x08)",result,0);
               OutputSequencer3(fd);             //输出测试动作 间的sequencer
             }
         }


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




int main(int argc, char **argv)
{
    KEY key;
    int rc;
    unsigned char info_flag1,info_flag2,info_flag3;
    if(argc>1) //参数必须为4个 执行文件名 Iset  Tset outputselect
    {
    char *p1=argv[1];
    if(memcmp(p1,"testitem=",9)==0)
    {
    p1=p1+9;
    Testitem=atoi(p1);
    }
    else
    return -1;

    if(Testitem==1)
    {
     TestItem1=1;
     TestItem2=0;
     TestItem3=0;
     OutputMode=atoi(argv[2]);
     faultduration=atof(argv[3]);
     Vfault=atof(argv[4]);
     Angle=atof(argv[5]);

     IENDSTART=atof(argv[6]);
     IENDEND=atof(argv[7]);
     ISTEP=atof(argv[8]);
     STEPTIME=atof(argv[9]);
     CurrentI=IENDSTART;

    }
    else if(Testitem==2)
    {
        TestItem1=0;
        TestItem2=0;
        TestItem3=1;
        OutputMode=atoi(argv[2]);
        faultduration=atof(argv[3]);
        Vfault=atof(argv[4]);
        Angle=atof(argv[5]);
        IEND=atof(argv[6]);
        HOLDTIME=atof(argv[7]);
    }
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
  }

     //这个可执行程序一旦开始运行，则表示已经开始开始测试了
     key_deal_test_current(temprorarydata->UartDev[0].uartfd);

     while(1){

         key=GetKey(temprorarydata->UartDev[1].uartfd);
         if(key==_STOP)
         starttest=0;

         ReadTemplateTestState();


     if(starttest==1)
      {
         if(TestItem1==1&&seq_count==1)
         {
            ReadAutoLogic(temprorarydata->UartDev[0].uartfd); //读开关量状态

         }
         else if(TestItem3==1&&seq_count==3)
         ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);  //在测试动作时间时读开关量状态、
         if(starttest==0&&save_f==1)
         {
             //本地保存测试结果 并写入退出 状态    并退出程序



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
             if(TestItem1==1)
               {
                 if(TripTime1==-1)
                { strcpy(strbuf,"TRIPVALUE=-1");
                  Writeexitstate(-1); //写入运行状态文件
                }
                else
               {
                   if(IENDSTART>=IENDEND)
                   {
                    CurrentI=CurrentI+ISTEP;
                   }
                   else
                   {
                    CurrentI=CurrentI-ISTEP;

                   }
                 sprintf(strbuf,"TRIPVALUE=%0.3f",CurrentI);
                 Writeexitstate(CurrentI);

               }

                 fprintf(fp,"%s\n",strbuf);


              }


             if(TestItem3==1)
               {
                 if(TripTime3==-1)
                  {  strcpy(strbuf,"TRIPTIME=-1");
                     Writeexitstate(-1); //写入运行状态文件
                  }
                   else
                   { sprintf(strbuf,"TRIPTIME=%4.3f",TripTime3);
                     Writeexitstate(TripTime3); //写入运行状态文件
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

         if(info_num++>5)
         {
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);
          info_flag1=result[8];
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
          info_flag2=result[8];
          info_flag3=result[9];
          logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3); 
          //将开关量信息写入文件
       //   Writelogicinfofile(info_flag1,info_flag2,info_flag3,V1Output,V2Output,V3Output,I1Output,I2Output,I3Output);
          info_num=0;
          }
     }

     else {
       char outchar[512];
       artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x80)",result,0);
       strcpy(outchar,"WARNING=Run normally");
       fprintf(stdout,outchar);
       fflush(stdout);

       output_signal_end(temprorarydata->UartDev[0].uartfd);
       artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
       close(temprorarydata->UartDev[1].uartfd);
       close(temprorarydata->UartDev[0].uartfd);
          free(temprorarydata);
          temprorarydata=NULL;

          ClearTemplateTestState();


       //写入运行状态文件 表示已经退出程序 文件中 -1表示 STOP 按键退出程序 0表示 测试完成退出程序
       Writeexitstate(-2);

       return 0;
     }
 }
}


