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
#include <QSemaphore>
#include "Testcom2.h"
#include "Volseq.h"          //电流测试的seq
#include "VolReadlogic.h"
#include "Gpio.h"
#include "em335x_drivers.h"
#include "Uartset.h"





TEMPRORARYDATA *temprorarydata;
float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
unsigned char result[70],receive_data[70];
double out_time,change_timedata;
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
long auto_i=0,IRQcnt=0,auto_count=0,tice=0;

int   INPAR=0,seq_count,SeqStep=0;
int   testcount=0,time_relay_testcount=0,auto_end=0;
int   cx,cy,cx_start,cx_mid,cx_end,starttest=0,low,row,page;
int   V1DC,V2DC,V3DC;
int   TestItem1,TestItem2,TestItem3,OutputMode,TestObject;
float FixedUnderTime,FixedOverTime;
float StartVoltage,EndVoltage,ActStep,ReturnStep,StepTime,Coarse_Voltage_step=1,coarse_fine_devision=10;
float TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime1,TripTime2,TripTime3;
float Phase,LogicResolution,SystemFrec;
float ChangeValue,timeout;
int exit_par,save_f;
int sys[4];
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
float VzOutput3Uo=1,VzOutputAc=57.00,VzOutputDc=220;
float SystemVolt=57.5;
float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;





float VENDSTART;
float VENDEND;
float VSTEP;
float STEPTIME;
float CurrentVoltage;

float VEND;
float HOLDTIME;




int TestItems=3;
int Faulttype=1;
int VPNVPP=1;

float faultduration=5;
float prefaulttime=3;
float postfaulttime=3;



int     Firstrun=1;
float   Preparetime=3;






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



void init_par() //参数初始化
{
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
          LogicResolution=3;
          SystemFrec=25;
          FILE *fp;
          if((fp=fopen(SYSSET,"a+"))==NULL)
          return;
          fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&Preparetime);
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

void key_deal_test_vol(int fd)
{
    if(starttest==0)
      {
        artExec(fd,"out:dig:on(0x04)",result,0);
        V1Output=0;
        V2Output=0;
        V3Output=0;
        I1Output=0;
        I2Output=0;
        I3Output=0;
        switch(Faulttype)
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
               V2Output=1;
               V3Output=1;
                break;
           }

        char outchar[512];
        if(TestItem1==1)
        {
          seq_count=1;
          starttest=1;
          CurrentVoltage=VENDSTART;
          sprintf(outchar,"Vol(V):%.3f",CurrentVoltage);
          fprintf(stdout,outchar);
          fflush(stdout);
          artExec(fd,"out:dig:on(0x04)",result,0);
          artExec(fd,"out:dig:on(0x08)",result,0);
          OutputSequencer1(fd);
          if(VENDSTART>=VENDEND)
          {
           CurrentVoltage=CurrentVoltage-VSTEP;
          }
          else
          {
           CurrentVoltage=CurrentVoltage+VSTEP;
          }

        }
        else if(TestItem1==0&&TestItem3==1)
        {
          seq_count=3;
          char outchar[512];
          starttest=1;
          switch(Faulttype)
          {
          case 1:
              sprintf(outchar,"A-E:%.3fV",VEND);
              break;
          case 2:
              sprintf(outchar,"B-E:%.3fV",VEND);
              break;
          case 3:
              sprintf(outchar,"C-E:%.3fV",VEND);
              break;
          case 4:
              sprintf(outchar,"A-B:%.3fV",VEND);
              break;
          case 5:
              sprintf(outchar,"B-C:%.3fV",VEND);
              break;
          case 6:
              sprintf(outchar,"C-A:%.3fV",VEND);
              break;
          case 7:
              sprintf(outchar,"A-B-C:%.3fV",VEND);
              break;

          }


          fprintf(stdout,outchar);
          fflush(stdout);
          artExec(fd,"out:dig:on(0x08)",result,0);
          OutputSequencer3(fd);
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
   int info_num=0;
   if(argc>1)
   {
       char *p1=argv[1];
       if(memcmp(p1,"testitem=",9)==0)
       {
           p1=p1+9;
           TestItems=atoi(p1);
       }
       else return -1;

       if(TestItems==1) //动作值测试
       {
           TestItem1=1; //动作值
           TestItem2=0;
           TestItem3=0;
           Faulttype=atoi(argv[2]);
           faultduration=atof(argv[3]);
           VPNVPP=atoi(argv[4]);
           VENDSTART=atof(argv[5]);
           VENDEND=atof(argv[6]);
           VSTEP=atof(argv[7]);
           STEPTIME=atof(argv[8]);
       }
       else if(TestItems==2)  //动作时间测试
       {
           TestItem1=0;
           TestItem2=0;
           TestItem3=1; //动作时间
           Faulttype=atoi(argv[2]);
           faultduration=atof(argv[3]);
           VPNVPP=atoi(argv[4]);
           VEND=atof(argv[5]);
           HOLDTIME=atof(argv[6]);
       }
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

  key_deal_test_vol(temprorarydata->UartDev[0].uartfd);
  while(1){
         key=GetKey(temprorarydata->UartDev[1].uartfd);
         if(key==_STOP)
             starttest=0;

         ReadTemplateTestState(); //这个文件信息由Templateprogram 写的



         if(starttest==1)
        {

         if(TestItem1==1&&seq_count==1)
         {
            ReadAutoLogic(temprorarydata->UartDev[0].uartfd); //动作值测试走的函数

         }
         else if(TestItem3==1&&seq_count==3)
         ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);  //在测试动作时间时读开关量状态、

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
             if(TestItem1==1)
             {
                         if(TripTime1==-1)
                          { strcpy(strbuf,"TRIPVALUE=-1");
                            Writeexitstate(-1);
                          }
                         else
                          {
                              if(VENDSTART>=VENDEND)
                              {
                               CurrentVoltage=CurrentVoltage+VSTEP;
                              }
                              else
                              {
                               CurrentVoltage=CurrentVoltage-VSTEP;

                              }
                            sprintf(strbuf,"TRIPVALUE=%0.3f",CurrentVoltage);
                            Writeexitstate(CurrentVoltage);
                          }
                           fprintf(fp,"%s\n",strbuf);
               }
             if(TestItem3==1)
               {
                        if(TripTime3==-1)
                        {  strcpy(strbuf,"TRIPTIME=-1");
                          Writeexitstate(-1);
                        }
                        else
                         { sprintf(strbuf,"TRIPTIME=%4.3f",TripTime3);
                           Writeexitstate(TripTime3);
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
          //将开关量信息写入文件
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


             //写入运行状态文件 表示已经退出程序 文件中 -1表示未动作  -2表示STOP按键退出程序  >0 测试完成退出程序
                Writeexitstate(-2);
          //   GPIO_OutClear(temprorarydata->gpio_fd,LEDNUM);  //亮
                return 0;
         }
     }
}


