#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QDir>
#include <QDate>
#include <QTime>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDateTime>
#include <termios.h>


#include "Config.h"
#include "thread1.h"
#include "Gpio.h"
#include "em335x_drivers.h"
#include "Uartset.h"
#include "format.h"
#include "settingnodelink.h"
#include "Reportnode.h"
#include "nodeprocess.h"


extern int runmode;
extern struct SECTMENTSET sectset;//一段的设置
extern int  sumsectnum;  //段的个数
extern bool protecttiontype[6];  //段保护类型标识 true 该保护类型有效 false; 该保护类型无效 增加保护类型时没，更改数组大小
extern int  currentsectid; //当前段的ID
extern int  currentprotecttype;  //当前的显示的保护类型
extern bool TestResultValid; //测试结果有效
extern bool Saveflag;
extern char ReportName[1024];
extern int starttest;

extern int AuthorizeFileValid;  //1  有效


extern bool Threadrunstate;

extern bool SoftStartEndflag;

//串口设置如下
thread1::thread1(QObject *parent) :
    QThread(parent)
{
    sumfucos=8;
    currentpos=sumfucos;
    ReportLink=Reportnodecreate();//创建段节点

}


void  thread1::settemprorarydata(TEMPRORARYDATA *value)
 {
     temprorarydata=value;
 }

void  thread1::setsumfucos(int num)
 {
     sumfucos=num;
 }

void thread1::setsectmentnodelink(struct SECTMENTSETNODE *set)//设置段的节点链表
{
    if(set==NULL)
        return;
    sectmentnodelink=set;


/*
    struct SECTMENTSETNODE *next=sectmentnodelink->next;
    while(next!=NULL)
    {
        if(next->value.freset.valid)
        {
          if(next->value.freset.Tripdfdttime.valid==true)
          {
              qDebug()<<__func__<<__LINE__<<\
              next->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue<<\
              next->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_Negtive;
          }
        }
        next=next->next;
    }

  */


}

void thread1::setsettingnodelink(struct SETTINGNODELIST *set)  //设置定值的链表
{
    if(set==NULL)
        return;
    settingnodelink=set;
}


float Readexitstate()
{

    FILE *fp;
    float state;
    fp = fopen(RUNSTATFILE,"r");
    if(fp == NULL)
    {
      return -1;
    }
    fscanf(fp,"%f",&state);
    fclose(fp);
    return state;
}

void writereportfile(char *buffer)
{
    FILE *fp=fopen("./TemplateDir/TemplateReport.txt","a+");  //附加的方式打开读写文件
    fprintf(fp,"%s\n",buffer);
    fclose(fp);
}

void Clearreportfile()
{
    FILE *fp=fopen("./TemplateDir/TemplateReport.txt","w+");  //附加的方式打开读写文件
    fclose(fp);
}


void thread1::writeanytestsettingfile(ANYTESTSET anytestset) //写任意测试的定值文件
{
    if(anytestset.valid)
    {
         QString strFileName =QString(QLatin1String(Anytestsettingfile));
         FILE *fp;
         fp=fopen(strFileName.toLocal8Bit().data(),"w+b");
         if(fp==NULL)
         {
             return;
         }
         //增加写入 value
          fwrite(&anytestset,sizeof(ANYTESTSET),1,fp);
          fclose(fp);
    }


}

void thread1::creatReportLink()
{
     QFile *fp;
     if(fp->exists(QString(QLatin1String(TemplateReport)))==false)
     return;

     if(ReportLink==NULL)
         return;

     FILE *file;
     if((file=fopen(TemplateReport,"r")) == NULL)
       {
           printf("fopen error\n");
           return ;
       }
     SECTREPORT sectreport; //一段的测试结果
     char buf[512];
     memset(buf,'\0',BUFFSIZE);
     memset(&sectreport,0,sizeof( SECTREPORT));
     char *p1=NULL;
     int sequnence=0;
     int currentproctype=0; //当前的保护类型

     while(fgets(buf,BUFFSIZE,file)!=NULL)
     {
      p1=buf;
      if(memcmp(p1,"<SECTREPORTBEGIN>",strlen("<SECTREPORTBEGIN>"))==0)
      {
          sequnence=1;
      }
      else if(memcmp(p1,"sectment:",strlen("sectment:"))==0&&sequnence==1)
      {
          p1=p1+9;
          char sectid[10];          //段的id;
          int i=0;
          while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
          {
          sectid[i++]=*p1;
          p1++;
          }
          sectid[i]='\0';
          sectreport.id=atoi(sectid);
          sequnence=2;
      }

      else if((memcmp(p1,"protection_type:",strlen("protection_type:"))==0)&&(sequnence==2))
      {
       p1=p1+16;
       if(memcmp(p1,"current",strlen("current"))==0)
       {
       sectreport.current.valid=true;
       sequnence=3;
       currentproctype=1;
       }
       else  if(memcmp(p1,"voltage",strlen("voltage"))==0)
       {
       sectreport.voltage.valid=true;
       sequnence=3;
       currentproctype=2;

       }
       else  if(memcmp(p1,"direction",strlen("direction"))==0)
       {
       sectreport.direction.valid=true;
       sequnence=3;
       currentproctype=3;
       }
       else  if(memcmp(p1,"frequency",strlen("frequency"))==0)
       {
       sectreport.frec.valid=true;
       sequnence=3;
       currentproctype=4;
       }
       else if(memcmp(p1,"anytest",strlen("anytest"))==0)
       {
        sectreport.anytest.valid=true;
        sequnence=3;
        currentproctype=5;
       }

       else if(memcmp(p1,"Vectorshift",strlen("Vectorshift"))==0)
       {
        sectreport.anytest.valid=true;
        sequnence=3;
        currentproctype=6;
       }



      }

      else if(sequnence==3&&memcmp(p1,"<SECTREPORTEND>",strlen("<SECTREPORTEND>"))!=0)
      {

          if(currentproctype==1) //测试值
          {
              if(memcmp(p1,"TRIPVALUE=",strlen("TRIPVALUE="))==0)
               {   p1=p1+10;
                  int i=0;
                  char value[10];
                  while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                  {
                  value[i++]=*p1;
                  p1++;
                  }
                  value[i]='\0';
                  sectreport.current.tripvalue=atof(value);
               }
              else  if(memcmp(p1,"TRIPTIME=",strlen("TRIPTIME="))==0)
                   {   p1=p1+9;
                      int i=0;
                      char value[10];
                      while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                      {
                      value[i++]=*p1;
                      p1++;
                      }
                      value[i]='\0';
                      sectreport.current.triptime=atof(value);
                   }
              else  if(memcmp(p1,"testitem=",strlen("testitem="))==0)
                   {   p1=p1+9;
                      int i=0;
                      char value[10];
                      while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                      {
                      value[i++]=*p1;
                      p1++;
                      }
                      value[i]='\0';
                      sectreport.current.testitem=atoi(value);
                   }


          }

          if(currentproctype==2)
          {
              if(memcmp(p1,"TRIPVALUE=",strlen("TRIPVALUE="))==0)
               {   p1=p1+10;
                  int i=0;
                  char value[10];
                  while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                  {
                  value[i++]=*p1;
                  p1++;
                  }
                  value[i]='\0';
                  sectreport.voltage.tripvalue=atof(value);
               }
              else  if(memcmp(p1,"TRIPTIME=",strlen("TRIPTIME="))==0)
                   {   p1=p1+9;
                      int i=0;
                      char value[10];
                      while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                      {
                      value[i++]=*p1;
                      p1++;
                      }
                      value[i]='\0';
                      sectreport.voltage.triptime=atof(value);
                   }
              else  if(memcmp(p1,"testitem=",strlen("testitem="))==0)
                   {   p1=p1+9;
                      int i=0;
                      char value[10];
                      while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                      {
                      value[i++]=*p1;
                      p1++;
                      }
                      value[i]='\0';
                      sectreport.voltage.testitem=atoi(value);
                   }
          }
          if(currentproctype==3)
          {
              if(memcmp(p1,"ACTANGLE1=",strlen("ACTANGLE1="))==0)
               {   p1=p1+10;
                  int i=0;
                  char value[10];
                  while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                  {
                  value[i++]=*p1;
                  p1++;
                  }
                  value[i]='\0';
                  sectreport.direction.actangle1=atof(value);
               }
              else  if(memcmp(p1,"ACTANGLE2=",strlen("ACTANGLE2="))==0)
               {   p1=p1+10;
                  int i=0;
                  char value[10];
                  while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                  {
                  value[i++]=*p1;
                  p1++;
                  }
                  value[i]='\0';
                  sectreport.direction.actangle2=atof(value);
               }

              else  if(memcmp(p1,"PHRASEMTA=",strlen("PHRASEMTA="))==0)
               {   p1=p1+10;
                  int i=0;
                  char value[10];
                  while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                  {
                  value[i++]=*p1;
                  p1++;
                  }
                  value[i]='\0';
                  sectreport.direction.phrasemta=atof(value);
               }

          }
          if(currentproctype==4)
          {

              if(memcmp(p1,"TRIPFREQ=",strlen("TRIPFREQ="))==0)
                 {   p1=p1+9;
                    int i=0;
                    char value[10];
                    while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                    value[i++]=*p1;
                    p1++;
                    }
                    value[i]='\0';
                    sectreport.frec.tripfrec=atof(value);
                 }

              else if(memcmp(p1,"TRIPTIME=",strlen("TRIPTIME="))==0)
                  {   p1=p1+9;
                     int i=0;
                     char value[10];
                     while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                     {
                     value[i++]=*p1;
                     p1++;
                     }
                     value[i]='\0';
                     sectreport.frec.triptime=atof(value);
                  }


              else if(memcmp(p1,"TRIPDFDT=",strlen("TRIPDFDT="))==0)
                 {   p1=p1+9;
                    int i=0;
                    char value[10];
                    while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                    value[i++]=*p1;
                    p1++;
                    }
                    value[i]='\0';
                    sectreport.frec.dfdttrip=atof(value);
                 }

              else if(memcmp(p1,"TRIPDFDTTIME=",strlen("TRIPDFDTTIME="))==0)
                 {
                    int i=0;
                    p1=p1+13;
                    char value[10];
                    while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                    {
                    value[i++]=*p1;
                    p1++;
                    }
                    value[i]='\0';
                    sectreport.frec.dfdttriptime=atof(value);
                 }

              else if(memcmp(p1,"VBLOCKTRIP=",strlen("VBLOCKTRIP="))==0)
                  {   p1=p1+11;
                     int i=0;
                     char value[10];
                     while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                     {
                     value[i++]=*p1;
                     p1++;
                     }
                     value[i]='\0';
                     sectreport.frec.Vblocktrip=atof(value);
                  }

              else if(memcmp(p1,"testitem=",strlen("testitem="))==0)
                  {   p1=p1+9;
                     int i=0;
                     char value[10];
                     while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                     {
                     value[i++]=*p1;
                     p1++;
                     }
                     value[i]='\0';
                     sectreport.frec.testitem=atoi(value);
                  }
          }

          if(currentproctype==5)
          {
              if(memcmp(p1,"TRIPTIME=",strlen("TRIPTIME="))==0)
               {   p1=p1+9;
                  int i=0;
                  char value[10];
                  while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                  {
                  value[i++]=*p1;
                  p1++;
                  }
                  value[i]='\0';
                  sectreport.anytest.triptime=atof(value);
               }

          }

          if(currentproctype==6) //测试值
          {
              if(memcmp(p1,"TRIPVALUE=",strlen("TRIPVALUE="))==0)
               {   p1=p1+10;
                  int i=0;
                  char value[10];
                  while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                  {
                  value[i++]=*p1;
                  p1++;
                  }
                  value[i]='\0';
                  sectreport.phrase.tripvalue=atof(value);
               }
              else  if(memcmp(p1,"TRIPTIME=",strlen("TRIPTIME="))==0)
                   {   p1=p1+9;
                      int i=0;
                      char value[10];
                      while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                      {
                      value[i++]=*p1;
                      p1++;
                      }
                      value[i]='\0';
                      sectreport.phrase.triptime=atof(value);
                   }
              else  if(memcmp(p1,"testitem=",strlen("testitem="))==0)
                   {   p1=p1+9;
                      int i=0;
                      char value[10];
                      while((*p1!=',')&&(*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
                      {
                      value[i++]=*p1;
                      p1++;
                      }
                      value[i]='\0';
                      sectreport.phrase.triptime=atoi(value);
                   }


          }



          if(memcmp(p1,"protection_type:",strlen("protection_type:"))==0)
                {
                 p1=p1+16;

                 if(memcmp(p1,"current",strlen("current"))==0)
                 {
                 sectreport.current.valid=true;
                 currentproctype=1;

                 }
                 else  if(memcmp(p1,"voltage",strlen("voltage"))==0)
                 {
                 sectreport.voltage.valid=true;
                 currentproctype=2;


                 }
                 else  if(memcmp(p1,"direction",strlen("direction"))==0)
                 {
                 sectreport.direction.valid=true;
                 currentproctype=3;

                 }
                 else  if(memcmp(p1,"frequency",strlen("frequency"))==0)
                 {
                 sectreport.frec.valid=true;
                 currentproctype=4;

                 }
                 else if(memcmp(p1,"anytest",strlen("anytest"))==0)
                 {
                  sectreport.anytest.valid=true;
                  currentproctype=5;
                 }
                 else if(memcmp(p1,"Vectorshift",strlen("Vectorshift"))==0)
                 {
                  sectreport.anytest.valid=true;
                  currentproctype=6;
                 }
                }

      }
      else if(memcmp(p1,"<SECTREPORTEND>",strlen("<SECTREPORTEND>"))==0)
      {

          insertsectreport(ReportLink,sectreport,sectreport.id);

          memset(&sectreport,0,sizeof(SECTREPORT));
          currentproctype=0;
          sequnence=0;
      }

     }



}

void thread1::Writetestfunc(struct SECTMENTSETNODE *node,FILE *fp)
{
    if(node==NULL||fp==NULL)
        return;
    char strbuf[512];
    char tmpstr1[512];
    SECTREPORT report;   //从段的ID拿到 Report
    struct SECTMENTSETNODE  *sectnext=node;
    bool VolTestValid=(sectnext->value.volset.valid==true&&(sectnext->value.volset.TripValue.valid==true||sectnext->value.volset.TripTime.valid==true));
    bool CurTestValid=(sectnext->value.curset.valid==true&&(sectnext->value.curset.TripValue.valid==true||sectnext->value.curset.TripTime.valid==true));
    bool DirTestValid=sectnext->value.dirset.valid;
    bool FirTestValid=(sectnext->value.freset.valid==true&&\
         (sectnext->value.freset.TripValue.valid||\
          sectnext->value.freset.TripTime.valid||\
          sectnext->value.freset.Tripdfdt.valid||\
          sectnext->value.freset.Tripdfdttime.valid||\
          sectnext->value.freset.TripVBlock.valid));
    bool PhaseTestValid=(sectnext->value.phraseset.valid==true&&(sectnext->value.phraseset.TripValue.valid==true||sectnext->value.phraseset.TripTime.valid==true));
    bool HarmonicTestValid=(sectnext->value.harmonicset.valid==true&&(sectnext->value.harmonicset.TripValue.valid==true));



    getsectreportfromid(ReportLink,report,node->id);
    if(CurTestValid) //有电流测试功能
   {
     strcpy(strbuf,"<TestfunctionBegin>");
     fprintf(fp,"%s\n",strbuf);
     strcpy(strbuf,"TESTTYPE=OVERCURRENT.COM"); //是电流测试
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"TESTNAME=%s",node->value.curset.name);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"TESTITEM=%s",node->value.curset.Testitem.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"OUTPUTSEL=%s",node->value.curset.output.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"FAULTDURATION=%s",node->value.curset.Faultduraion.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VFAULT=%s",node->value.curset.Vfault.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"ANGLE=%s",node->value.curset.Angle.text);
     fprintf(fp,"%s\n",strbuf);



     int testitem=atoi(node->value.curset.Testitem.text);
     if((testitem&0x01)&&node->value.curset.TripValue.valid==true)
     {
        strcpy(strbuf,"<TESTITEMBEGIN>");
        fprintf(fp,"%s\n",strbuf);
        strcpy(strbuf,"TESTITEMNAME=OVERCURRENT.TRIPVALUE"); //用于区分 是动作值测试 还是动作时间测试
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"IENDSTART=%s",node->value.curset.TripValue.StartValue.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"IENDEND=%s",node->value.curset.TripValue.EndValue.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"ISTEP=%s",node->value.curset.TripValue.StepValue.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"HOLDTIME=%s",node->value.curset.TripValue.HoldTime.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"TRIPVALUE=%4.3f",report.current.tripvalue);
        fprintf(fp,"%s\n",strbuf);
        //触发值评估
        if(node->value.curset.TripValue.assesstripvalue.relerror.valid==true&&node->value.curset.TripValue.assesstripvalue.abserror.valid!=true)
        {
            sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.curset.TripValue.assesstripvalue.relerror.errorvalue);
            fprintf(fp,"%s\n",tmpstr1);
            sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.curset.TripValue.assesstripvalue.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }

        else if(node->value.curset.TripValue.assesstripvalue.abserror.valid==true&&node->value.curset.TripValue.assesstripvalue.relerror.valid!=true)
        {
            sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
            node->value.curset.TripValue.assesstripvalue.abserror.errorvalue,\
            node->value.curset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);

            fprintf(fp,"%s\n",tmpstr1);
            sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.curset.TripValue.assesstripvalue.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }


        else  if(node->value.curset.TripValue.assesstripvalue.relerror.valid==true&&\
        node->value.curset.TripValue.assesstripvalue.abserror.valid==true)
        {

            if(node->value.curset.TripValue.assesstripvalue.assessandor==1) //与的关系
            {
                 sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
                         node->value.curset.TripValue.assesstripvalue.relerror.errorvalue,\
                         node->value.curset.TripValue.assesstripvalue.abserror.errorvalue,\
                         node->value.curset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);


                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.curset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
            }
            else if(node->value.curset.TripValue.assesstripvalue.assessandor==2)
            {
                sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
                        node->value.curset.TripValue.assesstripvalue.relerror.errorvalue,\
                        node->value.curset.TripValue.assesstripvalue.abserror.errorvalue,\
                        node->value.curset.TripValue.assesstripvalue.abserror.errorvalue_Negtive
                        );
                fprintf(fp,"%s\n",strbuf);
                sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.curset.TripValue.assesstripvalue.CompareID);
                fprintf(fp,"%s\n",tmpstr1);
            }
        }


        strcpy(strbuf,"<TESTITEMEND>");
        fprintf(fp,"%s\n",strbuf);
     }
     if((testitem&0x02)&&node->value.curset.TripTime.valid==true)
     {
        strcpy(strbuf,"<TESTITEMBEGIN>");
        fprintf(fp,"%s\n",strbuf);
        strcpy(strbuf,"TESTITEMNAME=OVERCURRENT.TRIPTIME"); //用于区分 是动作值测试 还是动作时间测试
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"IEND=%s",node->value.curset.TripTime.EndVaule.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"HOLDTIME=%s",node->value.curset.TripTime.HoldTime.text);
        fprintf(fp,"%s\n",strbuf);
        sprintf(strbuf,"TRIPTIME=%4.3f",report.current.triptime);
        fprintf(fp,"%s\n",strbuf);
        if(node->value.curset.TripTime.assesstriptime.relerror.valid==true&&node->value.curset.TripTime.assesstriptime.abserror.valid!=true)
        {
            sprintf(tmpstr1,"ASSESSTRIPTIME=REL(%f)",node->value.curset.TripTime.assesstriptime.relerror.errorvalue);
            fprintf(fp,"%s\n",tmpstr1);
            sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.curset.TripTime.assesstriptime.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }

        else if(node->value.curset.TripTime.assesstriptime.abserror.valid==true&&node->value.curset.TripTime.assesstriptime.relerror.valid!=true)
        {
            sprintf(tmpstr1,"ASSESSTRIPTIME=ABS(%f,%f)",\
            node->value.curset.TripTime.assesstriptime.abserror.errorvalue,\
            node->value.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
            fprintf(fp,"%s\n",tmpstr1);
            sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.curset.TripTime.assesstriptime.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }
        else  if(node->value.curset.TripTime.assesstriptime.relerror.valid==true&&\
        node->value.curset.TripTime.assesstriptime.abserror.valid==true)
        {

            if(node->value.curset.TripTime.assesstriptime.assessandor==1) //与的关系
            {
                 sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)&ABS(%f,%f)",\
                 node->value.curset.TripTime.assesstriptime.relerror.errorvalue,\
                 node->value.curset.TripTime.assesstriptime.abserror.errorvalue,\
                 node->value.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                 fprintf(fp,"%s\n",strbuf);
            }
            else if(node->value.curset.TripTime.assesstriptime.assessandor==2)
            {
                sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)|ABS(%f,%f)",\
                        node->value.curset.TripTime.assesstriptime.relerror.errorvalue,\
                        node->value.curset.TripTime.assesstriptime.abserror.errorvalue,\
                        node->value.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                fprintf(fp,"%s\n",strbuf);
            }
            sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.curset.TripTime.assesstriptime.CompareID);
            fprintf(fp,"%s\n",tmpstr1);
        }

        strcpy(strbuf,"<TESTITEMEND>");
        fprintf(fp,"%s\n",strbuf);
     }

     sprintf(strbuf,"<TestfunctionEnd>");
     fprintf(fp,"%s\n",strbuf);

   }

    if(VolTestValid) //有电压测试功能
   {
       strcpy(strbuf,"<TestfunctionBegin>");
       fprintf(fp,"%s\n",strbuf);
       strcpy(strbuf,"TESTTYPE=UNDERVOLTAGE.COM"); //是电压测试
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"TESTNAME=%s",node->value.volset.name);
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"TESTITEM=%s",node->value.volset.Testitem.text);
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"FAULTTYPE=%s",node->value.volset.FaultType.text);
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"FAULTDURATION=%s",node->value.volset.Faultduraion.text);
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"VPNVPP=%d",node->value.volset.Vpn_Vpp);
       fprintf(fp,"%s\n",strbuf);

       int testitem=atoi(node->value.volset.Testitem.text);
       if((testitem&0x01)&&node->value.volset.TripValue.valid==true)
       {
          strcpy(strbuf,"<TESTITEMBEGIN>");
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TESTITEMNAME=UNDERVOLTAGE.TRIPVALUE"); //用于区分 是动作值测试 还是动作时间测试
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"VENDSTART=%s",node->value.volset.TripValue.StartValue.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"VENDEND=%s",node->value.volset.TripValue.EndValue.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"VSTEP=%s",node->value.volset.TripValue.StepValue.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"HOLDTIME=%s",node->value.volset.TripValue.HoldTime.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"TRIPVALUE=%4.3f",report.voltage.tripvalue);
          fprintf(fp,"%s\n",strbuf);
          //触发值评估
          if(node->value.volset.TripValue.assesstripvalue.relerror.valid==true\
                  &&node->value.volset.TripValue.assesstripvalue.abserror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.volset.TripValue.assesstripvalue.relerror.errorvalue);
              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.volset.TripValue.assesstripvalue.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }

          else if(node->value.volset.TripValue.assesstripvalue.abserror.valid==true\
                  &&node->value.volset.TripValue.assesstripvalue.relerror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
              node->value.volset.TripValue.assesstripvalue.abserror.errorvalue,\
              node->value.volset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);

              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.volset.TripValue.assesstripvalue.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }


          else  if(node->value.volset.TripValue.assesstripvalue.relerror.valid==true&&\
          node->value.volset.TripValue.assesstripvalue.abserror.valid==true)
          {

              if(node->value.volset.TripValue.assesstripvalue.assessandor==1) //与的关系
              {
                   sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
                   node->value.volset.TripValue.assesstripvalue.relerror.errorvalue,\
                   node->value.volset.TripValue.assesstripvalue.abserror.errorvalue,\
                   node->value.volset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);

                   fprintf(fp,"%s\n",strbuf);
                   sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.volset.TripValue.assesstripvalue.CompareID);
                   fprintf(fp,"%s\n",tmpstr1);
              }
              else if(node->value.volset.TripValue.assesstripvalue.assessandor==2)
              {
                  sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
                   node->value.volset.TripValue.assesstripvalue.relerror.errorvalue,\
                   node->value.volset.TripValue.assesstripvalue.abserror.errorvalue,\
                   node->value.volset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
                  fprintf(fp,"%s\n",strbuf);
                  sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.volset.TripValue.assesstripvalue.CompareID);
                  fprintf(fp,"%s\n",tmpstr1);
              }
          }

          strcpy(strbuf,"<TESTITEMEND>");
          fprintf(fp,"%s\n",strbuf);
       }
       if((testitem&0x02)&&node->value.volset.TripTime.valid==true)
       {
          strcpy(strbuf,"<TESTITEMBEGIN>");
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TESTITEMNAME=UNDERVOLTAGE.TRIPTIME"); //用于区分 是动作值测试 还是动作时间测试
          fprintf(fp,"%s\n",strbuf);


          sprintf(strbuf,"USEMEASUREDVALUE=%d",node->value.volset.TripTime.UseMeasureValue);
          fprintf(fp,"%s\n",strbuf);

          if(node->value.volset.TripTime.UseMeasureValue==0)
         { sprintf(strbuf,"VEND=%s",node->value.volset.TripTime.EndVaule.text);
           fprintf(fp,"%s\n",strbuf);
         }



          sprintf(strbuf,"HOLDTIME=%s",node->value.volset.TripTime.HoldTime.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"TRIPTIME=%4.3f",report.voltage.triptime);
          fprintf(fp,"%s\n",strbuf);




          if(node->value.volset.TripTime.assesstriptime.relerror.valid==true&&node->value.volset.TripTime.assesstriptime.abserror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPTIME=REL(%f)",node->value.volset.TripTime.assesstriptime.relerror.errorvalue);
              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.volset.TripTime.assesstriptime.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }

         else if(node->value.volset.TripTime.assesstriptime.abserror.valid==true&&node->value.volset.TripTime.assesstriptime.relerror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPTIME=ABS(%f,%f)",\
              node->value.volset.TripTime.assesstriptime.abserror.errorvalue,\
              node->value.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive);

              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.volset.TripTime.assesstriptime.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }
         else  if(node->value.volset.TripTime.assesstriptime.relerror.valid==true&&\
             node->value.volset.TripTime.assesstriptime.abserror.valid==true)
          {

              if(node->value.volset.TripTime.assesstriptime.assessandor==1) //与的关系
              {
                   sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)&ABS(%f,%f)",\
                   node->value.volset.TripTime.assesstriptime.relerror.errorvalue,\
                   node->value.volset.TripTime.assesstriptime.abserror.errorvalue,\
                   node->value.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                   fprintf(fp,"%s\n",strbuf);
                   sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.volset.TripTime.assesstriptime.CompareID);
                   fprintf(fp,"%s\n",tmpstr1);

              }
              else if(node->value.volset.TripTime.assesstriptime.assessandor==2)
              {
                  sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)|ABS(%f,%f)",\
                  node->value.volset.TripTime.assesstriptime.relerror.errorvalue,\
                  node->value.volset.TripTime.assesstriptime.abserror.errorvalue,\
                  node->value.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                  fprintf(fp,"%s\n",strbuf);
                  sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.volset.TripTime.assesstriptime.CompareID);
                  fprintf(fp,"%s\n",tmpstr1);
              }
          }



          strcpy(strbuf,"<TESTITEMEND>");
          fprintf(fp,"%s\n",strbuf);
       }
       sprintf(strbuf,"<TestfunctionEnd>");
       fprintf(fp,"%s\n",strbuf);

   }

    if(DirTestValid) //有方向测试功能
   {

       strcpy(strbuf,"<TestfunctionBegin>");
       fprintf(fp,"%s\n",strbuf);
       strcpy(strbuf,"TESTTYPE=DIRECTION.COM"); //是电压测试
       fprintf(fp,"%s\n",strbuf);
       sprintf(strbuf,"TESTNAME=%s",node->value.dirset.name);
       fprintf(fp,"%s\n",strbuf);
    if(node->value.dirset.TripMta.valid)
    { //以下打印出程序所需要的变量
     strcpy(strbuf,"<TESTITEMBEGIN>");
     fprintf(fp,"%s\n",strbuf);
     strcpy(strbuf,"TESTITEMNAME=DIRECTION.MTA"); //用于区分 是动作值测试 还是动作时间测试
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"IFAULT=%s",node->value.dirset.TripMta.Ifault.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"VFAULT=%s",node->value.dirset.TripMta.vfault.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"MTA=%s",node->value.dirset.TripMta.mta.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"FAULTTYPE=%s",node->value.dirset.TripMta.faulttype.text);
     fprintf(fp,"%s\n",strbuf);
     //评估
     sprintf(strbuf,"ACTANGLE1=%4.3f",report.direction.actangle1);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"ACTANGLE2=%4.3f",report.direction.actangle2);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"PHRASEMTA=%4.3f",report.direction.phrasemta);
     fprintf(fp,"%s\n",strbuf);

     if(node->value.dirset.TripMta.assessmta.relerror.valid==true\
      &&node->value.dirset.TripMta.assessmta.abserror.valid!=true)
     {
         sprintf(tmpstr1,"ASSESSMTA=REL(%f)",node->value.dirset.TripMta.assessmta.relerror.errorvalue);
         fprintf(fp,"%s\n",tmpstr1);
         sprintf(tmpstr1,"ASSESSMTAID=%s",node->value.dirset.TripMta.assessmta.CompareID);
         fprintf(fp,"%s\n",tmpstr1);
     }

     else if(node->value.dirset.TripMta.assessmta.abserror.valid==true&&\
            node->value.dirset.TripMta.assessmta.relerror.valid!=true)
     {
         sprintf(tmpstr1,"ASSESSMTA=ABS(%f,%f)",\
         node->value.dirset.TripMta.assessmta.abserror.errorvalue,\
         node->value.dirset.TripMta.assessmta.abserror.errorvalue_Negtive);


         fprintf(fp,"%s\n",tmpstr1);
         sprintf(tmpstr1,"ASSESSMTAID=%s",node->value.dirset.TripMta.assessmta.CompareID);
         fprintf(fp,"%s\n",tmpstr1);
     }
     else   if(node->value.dirset.TripMta.assessmta.relerror.valid==true&&\
        node->value.dirset.TripMta.assessmta.abserror.valid==true)
     {
         if(node->value.dirset.TripMta.assessmta.assessandor==1) //与的关系
         {
              sprintf(strbuf,"ASSESSMTA=REL(%f)&ABS(%f,%f)",\
              node->value.dirset.TripMta.assessmta.relerror.errorvalue,\
              node->value.dirset.TripMta.assessmta.abserror.errorvalue,\
              node->value.dirset.TripMta.assessmta.abserror.errorvalue_Negtive);
              fprintf(fp,"%s\n",strbuf);
              sprintf(tmpstr1,"ASSESSMTAID=%s",node->value.dirset.TripMta.assessmta.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
         }
         else if(node->value.dirset.TripMta.assessmta.assessandor==2)
         {
             sprintf(strbuf,"ASSESSMTA=REL(%f)|ABS(%f,%f)",\
             node->value.dirset.TripMta.assessmta.relerror.errorvalue,\
             node->value.dirset.TripMta.assessmta.abserror.errorvalue,\
             node->value.dirset.TripMta.assessmta.abserror.errorvalue_Negtive);
              fprintf(fp,"%s\n",strbuf);
              sprintf(tmpstr1,"ASSESSMTAID=%s",node->value.dirset.TripMta.assessmta.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
         }
     }

     strcpy(strbuf,"<TESTITEMEND>");
     fprintf(fp,"%s\n",strbuf);
    }
     sprintf(strbuf,"<TestfunctionEnd>");
     fprintf(fp,"%s\n",strbuf);
   }

    if(FirTestValid) //频率
   {
     strcpy(strbuf,"<TestfunctionBegin>");
     fprintf(fp,"%s\n",strbuf);
     strcpy(strbuf,"TESTTYPE=UNDERFREQUENCY.COM");
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"TESTNAME=%s",node->value.freset.name);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"TESTITEM=%s",node->value.freset.TestItem.text);
     fprintf(fp,"%s\n",strbuf);
     sprintf(strbuf,"FAULTDURATION=%s",node->value.freset.Faultduraion.text);
     fprintf(fp,"%s\n",strbuf);

     int testitem=atoi(node->value.freset.TestItem.text);
     if((testitem&0x01)&&node->value.freset.TripValue.valid==true)
     {
      strcpy(strbuf,"<TESTITEMBEGIN>");
      fprintf(fp,"%s\n",strbuf);
      strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.TRIPVALUE"); //用于区分 是动作值测试 还是动作时间测试
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"FREQENDSTART=%s",node->value.freset.TripValue.FreEndValueStart.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"FREQENDEND=%s",node->value.freset.TripValue.FreEndValueEnd.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"FREQSTEP=%s",node->value.freset.TripValue.FreStep.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"HOLDTIME=%s",node->value.freset.TripValue.HoldTime.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"TRIPFREQ=%4.2f",report.frec.tripfrec);
      fprintf(fp,"%s\n",strbuf);


      if(node->value.freset.TripValue.assesstripvalue.relerror.valid==true&&\
         node->value.freset.TripValue.assesstripvalue.abserror.valid!=true)
      {
          sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.freset.TripValue.assesstripvalue.relerror.errorvalue);
          fprintf(fp,"%s\n",tmpstr1);
          sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.freset.TripValue.assesstripvalue.CompareID);
          fprintf(fp,"%s\n",tmpstr1);
      }



     else if(node->value.freset.TripValue.assesstripvalue.abserror.valid==true&&\
             node->value.freset.TripValue.assesstripvalue.relerror.valid!=true)
      {
          sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
          node->value.freset.TripValue.assesstripvalue.abserror.errorvalue,\
          node->value.freset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);

          fprintf(fp,"%s\n",tmpstr1);
          sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.freset.TripValue.assesstripvalue.CompareID);
          fprintf(fp,"%s\n",tmpstr1);
      }
     else   if(node->value.freset.TripValue.assesstripvalue.relerror.valid==true&&\
               node->value.freset.TripValue.assesstripvalue.abserror.valid==true)
      {
          if(node->value.freset.TripValue.assesstripvalue.assessandor==1) //与的关系
          {
              sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
              node->value.freset.TripValue.assesstripvalue.relerror.errorvalue,\
              node->value.freset.TripValue.assesstripvalue.abserror.errorvalue,\
              node->value.freset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);

              fprintf(fp,"%s\n",strbuf);
              sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.freset.TripValue.assesstripvalue.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }
          else if(node->value.freset.TripValue.assesstripvalue.assessandor==2)
          {
              sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
              node->value.freset.TripValue.assesstripvalue.relerror.errorvalue,\
              node->value.freset.TripValue.assesstripvalue.abserror.errorvalue,\
              node->value.freset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
              fprintf(fp,"%s\n",strbuf);
              sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.freset.TripValue.assesstripvalue.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }

      }

      strcpy(strbuf,"<TESTITEMEND>");
      fprintf(fp,"%s\n",strbuf);
     }

     if((testitem&0x02)&&node->value.freset.TripTime.valid==true)
     {
         strcpy(strbuf,"<TESTITEMBEGIN>");
         fprintf(fp,"%s\n",strbuf);
         strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.TRIPTIME"); //用于区分 是动作值测试 还是动作时间测试
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"FREQENDVALUE=%s",node->value.freset.TripTime.FreEndValue.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"HOLDTIME=%s",node->value.freset.TripTime.HoldTime.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"TRIPTIME=%4.3f",report.frec.triptime);
         fprintf(fp,"%s\n",strbuf);
         if(node->value.freset.TripTime.assesstriptime.relerror.valid==true\
          &&node->value.freset.TripTime.assesstriptime.abserror.valid!=true)
         {
             sprintf(tmpstr1,"ASSESSTRIPTIME=REL(%f)",node->value.freset.TripTime.assesstriptime.relerror.errorvalue);
             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.freset.TripTime.assesstriptime.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }

        else if(node->value.freset.TripTime.assesstriptime.abserror.valid==true\
                &&node->value.freset.TripTime.assesstriptime.relerror.valid!=true)
         {
             sprintf(tmpstr1,"ASSESSTRIPTIME=ABS(%f,%f)",\
             node->value.freset.TripTime.assesstriptime.abserror.errorvalue,\
             node->value.freset.TripTime.assesstriptime.abserror.errorvalue_Negtive);


             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.freset.TripTime.assesstriptime.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }
        else   if(node->value.freset.TripTime.assesstriptime.relerror.valid==true&&\
                  node->value.freset.TripTime.assesstriptime.abserror.valid==true)
         {

             if(node->value.freset.TripTime.assesstriptime.assessandor==1) //与的关系
             {
                 sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)&ABS(%f,%f)",\
                 node->value.freset.TripTime.assesstriptime.relerror.errorvalue,\
                 node->value.freset.TripTime.assesstriptime.abserror.errorvalue,\
                 node->value.freset.TripTime.assesstriptime.abserror.errorvalue_Negtive);

                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.freset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }
             else if(node->value.freset.TripTime.assesstriptime.assessandor==2)
             {
                 sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)|ABS(%f,%f)",\
                 node->value.freset.TripTime.assesstriptime.relerror.errorvalue,\
                 node->value.freset.TripTime.assesstriptime.abserror.errorvalue,\
                 node->value.freset.TripTime.assesstriptime.abserror.errorvalue_Negtive);



                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.freset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }
         }


     }

     if((testitem&0x04)&&node->value.freset.Tripdfdt.valid==true)
     {
         strcpy(strbuf,"<TESTITEMBEGIN>");
         fprintf(fp,"%s\n",strbuf);
         strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.TRIPDFDT"); //用于区分 是动作值测试 还是动作时间测试
         fprintf(fp,"%s\n",strbuf);


         sprintf(strbuf,"FREQENDVALUE=%s",node->value.freset.Tripdfdt.FreEndValue.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"DFDTSTART=%s",node->value.freset.Tripdfdt.DfdtStart.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"DFDTEND=%s",node->value.freset.Tripdfdt.DfdtEnd.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"DFDTSTEP=%s",node->value.freset.Tripdfdt.DfdtStep.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"TRIPDFDT=%4.3f",report.frec.dfdttrip);
         fprintf(fp,"%s\n",strbuf);
         if(node->value.freset.Tripdfdt.assessdfdttrip.relerror.valid==true&&\
            node->value.freset.Tripdfdt.assessdfdttrip.abserror.valid!=true)
         {
             sprintf(tmpstr1,"ASSESSTRIPDFDT=REL(%f)",node->value.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue);
             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSTRIPDFDTID=%s",node->value.freset.Tripdfdt.assessdfdttrip.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }

         else if(node->value.freset.Tripdfdt.assessdfdttrip.abserror.valid==true&&\
                 node->value.freset.Tripdfdt.assessdfdttrip.relerror.valid!=true)
          {
              sprintf(tmpstr1,"ASSESSTRIPDFDT=ABS(%f,%f)",\
              node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,\
              node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_Negtive);


              fprintf(fp,"%s\n",tmpstr1);
              sprintf(tmpstr1,"ASSESSTRIPDFDTID=%s",node->value.freset.Tripdfdt.assessdfdttrip.CompareID);
              fprintf(fp,"%s\n",tmpstr1);
          }
         else   if(node->value.freset.Tripdfdt.assessdfdttrip.relerror.valid==true&&\
                   node->value.freset.Tripdfdt.assessdfdttrip.abserror.valid==true)
          {
              if(node->value.freset.Tripdfdt.assessdfdttrip.assessandor==1) //与的关系
              {
                  sprintf(strbuf,"ASSESSTRIPDFDT=REL(%f)&ABS(%f,%f)",\
                  node->value.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue,\
                  node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,\
                  node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_Negtive);


                  fprintf(fp,"%s\n",strbuf);
                  sprintf(tmpstr1,"ASSESSTRIPDFDTID=%s",node->value.freset.Tripdfdt.assessdfdttrip.CompareID);
                  fprintf(fp,"%s\n",tmpstr1);
              }
              else if(node->value.freset.Tripdfdt.assessdfdttrip.assessandor==2)
              {
                  sprintf(strbuf,"ASSESSTRIPDFDT=REL(%f)|ABS(%f,%f)",\
                  node->value.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue,\
                  node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,\
                  node->value.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_Negtive);
                  fprintf(fp,"%s\n",strbuf);
                  sprintf(tmpstr1,"ASSESSTRIPDFDTID=%s",node->value.freset.Tripdfdt.assessdfdttrip.CompareID);
                  fprintf(fp,"%s\n",tmpstr1);
              }

          }

         strcpy(strbuf,"<TESTITEMEND>");
         fprintf(fp,"%s\n",strbuf);
     }

     if((testitem&0x08)&&node->value.freset.Tripdfdttime.valid==true)
     {
         strcpy(strbuf,"<TESTITEMBEGIN>");
         fprintf(fp,"%s\n",strbuf);
         strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.DFDTTRIPTIME"); //用于区分 是动作值测试 还是动作时间测试
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"DFDT=%s",node->value.freset.Tripdfdttime.dfdt.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"FREQENDVALUE=%s",node->value.freset.Tripdfdttime.stopfreq.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"DURATION=%s",node->value.freset.Tripdfdttime.Duration.text);
         fprintf(fp,"%s\n",strbuf);
         sprintf(strbuf,"TRIPDFDTTIME=%4.3f",report.frec.dfdttriptime);
         fprintf(fp,"%s\n",strbuf);
         if(node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.valid==true&&\
            node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.valid!=true)
         {
             sprintf(tmpstr1,"ASSESSDFDTTRIPTIME=REL(%f)",\
             node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue);
             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSDFDTTRIPTIMEID=%s",node->value.freset.Tripdfdttime.assessdfdttriptime.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }
         else if(node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.valid!=true&&\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.valid==true)
         {

             sprintf(tmpstr1,"ASSESSDFDTTRIPTIME=ABS(%f,%f)",\
             node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue,\
             node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_Negtive);
             fprintf(fp,"%s\n",tmpstr1);
             sprintf(tmpstr1,"ASSESSDFDTTRIPTIMEID=%s",node->value.freset.Tripdfdttime.assessdfdttriptime.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }
         else if(node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.valid==true&&\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.valid==true)
         {

             if(node->value.freset.Tripdfdttime.assessdfdttriptime.assessandor==1) //与的关系
             {
                 sprintf(strbuf,"ASSESSDFDTTRIPTIME=REL(%f)&ABS(%f,%f)",\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue,\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue,\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_Negtive);
                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSDFDTTRIPTIMEID=%s",node->value.freset.Tripdfdttime.assessdfdttriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }
             else if(node->value.freset.Tripdfdttime.assessdfdttriptime.assessandor==2)
             {
                 sprintf(strbuf,"ASSESSDFDTTRIPTIME=REL(%f)|ABS(%f,%f)",\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue,\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue,\
                 node->value.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_Negtive);

                 fprintf(fp,"%s\n",strbuf);
                 sprintf(tmpstr1,"ASSESSDFDTTRIPTIMEID=%s",node->value.freset.Tripdfdttime.assessdfdttriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }

         }

         strcpy(strbuf,"<TESTITEMEND>");
         fprintf(fp,"%s\n",strbuf);

     }

     if((testitem&0x10)&&node->value.freset.TripVBlock.valid==true)
     {
      strcpy(strbuf,"<TESTITEMBEGIN>");
      fprintf(fp,"%s\n",strbuf);
      strcpy(strbuf,"TESTITEMNAME=UNDERFREQUENCY.VOLTAGEBLOCKING"); //用于区分 是动作值测试 还是动作时间测试
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"DFDT=%s",node->value.freset.TripVBlock.Dfdt.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"FREQENDVALUE=%s",node->value.freset.TripVBlock.FreEndValue.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"VSTART=%s",node->value.freset.TripVBlock.VStart.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"VEND=%s",node->value.freset.TripVBlock.VEnd.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"VSTEP=%s",node->value.freset.TripVBlock.VStep.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"HOLDTIME=%s",node->value.freset.TripVBlock.HoldTime.text);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"VSELECT=%d",node->value.freset.TripVBlock.VSelect);
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"VBLOCKTRIP=%4.3f",report.frec.Vblocktrip);
      fprintf(fp,"%s\n",strbuf);
      if(node->value.freset.TripVBlock.assessvblock.relerror.valid==true\
       &&node->value.freset.TripVBlock.assessvblock.abserror.valid!=true)
      {
          sprintf(tmpstr1,"ASSESSVBLOCK=REL(%f)",\
          node->value.freset.TripVBlock.assessvblock.relerror.errorvalue);
          fprintf(fp,"%s\n",tmpstr1);
          sprintf(tmpstr1,"ASSESSVBLOCKID=%s",node->value.freset.TripVBlock.assessvblock.CompareID);
          fprintf(fp,"%s\n",tmpstr1);
      }

     else if(node->value.freset.TripVBlock.assessvblock.abserror.valid==true\
           &&node->value.freset.TripVBlock.assessvblock.relerror.valid!=true)
      {
          sprintf(tmpstr1,"ASSESSVBLOCK=ABS(%f,%f)",\
          node->value.freset.TripVBlock.assessvblock.abserror.errorvalue,\
          node->value.freset.TripVBlock.assessvblock.abserror.errorvalue_Negtive);
          fprintf(fp,"%s\n",tmpstr1);
          sprintf(tmpstr1,"ASSESSVBLOCKID=%s",node->value.freset.TripVBlock.assessvblock.CompareID);
          fprintf(fp,"%s\n",tmpstr1);
      }
     else if(node->value.freset.TripVBlock.assessvblock.relerror.valid==true&&\
             node->value.freset.TripVBlock.assessvblock.abserror.valid==true)
      {

         if(node->value.freset.TripVBlock.assessvblock.assessandor==1) //与的关系
         {
               sprintf(strbuf,"ASSESSVBLOCK=REL(%f)&ABS(%f,%f)",\
               node->value.freset.TripVBlock.assessvblock.relerror.errorvalue,\
               node->value.freset.TripVBlock.assessvblock.abserror.errorvalue,\
               node->value.freset.TripVBlock.assessvblock.abserror.errorvalue_Negtive);



               fprintf(fp,"%s\n",strbuf);
               sprintf(tmpstr1,"ASSESSVBLOCKID=%s",node->value.freset.TripVBlock.assessvblock.CompareID);
               fprintf(fp,"%s\n",tmpstr1);
         }
         else if(node->value.freset.TripVBlock.assessvblock.assessandor==2)
         {
             sprintf(strbuf,"ASSESSVBLOCK=REL(%f)|ABS(%f,%f)",\
             node->value.freset.TripVBlock.assessvblock.relerror.errorvalue,\
             node->value.freset.TripVBlock.assessvblock.abserror.errorvalue,\
             node->value.freset.TripVBlock.assessvblock.abserror.errorvalue_Negtive);

             fprintf(fp,"%s\n",strbuf);
             sprintf(tmpstr1,"ASSESSVBLOCKID=%s",node->value.freset.TripVBlock.assessvblock.CompareID);
             fprintf(fp,"%s\n",tmpstr1);
         }
      }

     strcpy(strbuf,"<TESTITEMEND>");
     fprintf(fp,"%s\n",strbuf);
     }
     sprintf(strbuf,"<TestfunctionEnd>");
     fprintf(fp,"%s\n",strbuf);
   }


    if(PhaseTestValid)
    {

          strcpy(strbuf,"<TestfunctionBegin>");
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TESTTYPE=VECTORSHIFT.COM"); //是电流测试
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"TESTNAME=%s",node->value.phraseset.name);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"TESTITEM=%s",node->value.phraseset.Testitem.text);
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"FAULTDURATION=%s",node->value.phraseset.Faultduraion.text);
          fprintf(fp,"%s\n",strbuf);
          if(node->value.phraseset.IAch)
           fprintf(fp,"IACH=1");
          else fprintf(fp,"IACH=0");
           fprintf(fp,"\n");


          if(node->value.phraseset.IBch)
          fprintf(fp,"IBCH=1");
          else fprintf(fp,"IBCH=0");
          fprintf(fp,"\n");


          if(node->value.phraseset.ICch)
          fprintf(fp,"ICCH=1");
          else fprintf(fp,"ICCH=0");
         fprintf(fp,"\n");


          if(node->value.phraseset.VAch)
          fprintf(fp,"VACH=1");
          else fprintf(fp,"VACH=0");
          fprintf(fp,"\n");


          if(node->value.phraseset.VBch)
          fprintf(fp,"VBCH=1");
          else fprintf(fp,"VBCH=0");
          fprintf(fp,"\n");


          if(node->value.phraseset.VCch)
          fprintf(fp,"VCCH=1");
          else fprintf(fp,"VCCH=0");
          fprintf(fp,"\n");

          int testitem=atoi(node->value.phraseset.Testitem.text);
          if((testitem&0x01)&&node->value.phraseset.TripValue.valid==true)
          {
             strcpy(strbuf,"<TESTITEMBEGIN>");
             fprintf(fp,"%s\n",strbuf);
             strcpy(strbuf,"TESTITEMNAME=VECTORSHIFT.TRIPVALUE"); //用于区分 是动作值测试 还是动作时间测试
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"PENDSTART=%s",node->value.phraseset.TripValue.StartValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"PENDEND=%s",node->value.phraseset.TripValue.EndValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"PSTEP=%s",node->value.phraseset.TripValue.StepValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HOLDTIME=%s",node->value.phraseset.TripValue.HoldTime.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"TRIPVALUE=%4.3f",report.phrase.tripvalue);
             fprintf(fp,"%s\n",strbuf);
             //触发值评估
             if(node->value.phraseset.TripValue.assesstripvalue.relerror.valid==true&&node->value.phraseset.TripValue.assesstripvalue.abserror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.phraseset.TripValue.assesstripvalue.relerror.errorvalue);
                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.phraseset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }

             else if(node->value.phraseset.TripValue.assesstripvalue.abserror.valid==true&&node->value.phraseset.TripValue.assesstripvalue.relerror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
                 node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue,\
                 node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);



                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.phraseset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }


             else  if(node->value.phraseset.TripValue.assesstripvalue.relerror.valid==true&&\
                node->value.phraseset.TripValue.assesstripvalue.abserror.valid==true)
             {

                 if(node->value.phraseset.TripValue.assesstripvalue.assessandor==1) //与的关系
                 {
                      sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
                      node->value.phraseset.TripValue.assesstripvalue.relerror.errorvalue,\
                      node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue,\
                      node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);


                      fprintf(fp,"%s\n",strbuf);
                      sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.phraseset.TripValue.assesstripvalue.CompareID);
                      fprintf(fp,"%s\n",tmpstr1);
                 }
                 else if(node->value.phraseset.TripValue.assesstripvalue.assessandor==2)
                 {
                     sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
                     node->value.phraseset.TripValue.assesstripvalue.relerror.errorvalue,\
                     node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue,\
                     node->value.phraseset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
                     fprintf(fp,"%s\n",strbuf);
                     sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.phraseset.TripValue.assesstripvalue.CompareID);
                     fprintf(fp,"%s\n",tmpstr1);
                 }
             }


             strcpy(strbuf,"<TESTITEMEND>");
             fprintf(fp,"%s\n",strbuf);
          }
          if((testitem&0x02)&&node->value.phraseset.TripTime.valid==true)
          {
             strcpy(strbuf,"<TESTITEMBEGIN>");
             fprintf(fp,"%s\n",strbuf);
             strcpy(strbuf,"TESTITEMNAME=VECTORSHIFT.TRIPTIME"); //用于区分 是动作值测试 还是动作时间测试
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"PEND=%s",node->value.phraseset.TripTime.EndVaule.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HOLDTIME=%s",node->value.phraseset.TripTime.HoldTime.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"TRIPTIME=%4.3f",report.phrase.triptime);
             fprintf(fp,"%s\n",strbuf);
             if(node->value.phraseset.TripTime.assesstriptime.relerror.valid==true&&node->value.phraseset.TripTime.assesstriptime.abserror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPTIME=REL(%f)",node->value.phraseset.TripTime.assesstriptime.relerror.errorvalue);
                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.phraseset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }

             else if(node->value.phraseset.TripTime.assesstriptime.abserror.valid==true&&node->value.phraseset.TripTime.assesstriptime.relerror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPTIME=ABS(%f,%f)",\
                 node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue,\
                 node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive);



                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.phraseset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }
             else  if(node->value.phraseset.TripTime.assesstriptime.relerror.valid==true&&\
             node->value.phraseset.TripTime.assesstriptime.abserror.valid==true)
             {

                 if(node->value.phraseset.TripTime.assesstriptime.assessandor==1) //与的关系
                 {
                      sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)&ABS(%f,%f)",\
                      node->value.phraseset.TripTime.assesstriptime.relerror.errorvalue,\
                      node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue,\
                      node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                      fprintf(fp,"%s\n",strbuf);
                 }
                 else if(node->value.phraseset.TripTime.assesstriptime.assessandor==2)
                 {
                     sprintf(strbuf,"ASSESSTRIPTIME=REL(%f)|ABS(%f,%f)",\
                     node->value.phraseset.TripTime.assesstriptime.relerror.errorvalue,\
                     node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue,\
                     node->value.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive);

                     fprintf(fp,"%s\n",strbuf);
                 }
                 sprintf(tmpstr1,"ASSESSTRIPTIMEID=%s",node->value.phraseset.TripTime.assesstriptime.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }

             strcpy(strbuf,"<TESTITEMEND>");
             fprintf(fp,"%s\n",strbuf);
          }
          sprintf(strbuf,"<TestfunctionEnd>");
          fprintf(fp,"%s\n",strbuf);
    }

    if(HarmonicTestValid)
    {

          strcpy(strbuf,"<TestfunctionBegin>");
          fprintf(fp,"%s\n",strbuf);
          strcpy(strbuf,"TESTTYPE=HARMONIC.COM"); //是谐波测试
          fprintf(fp,"%s\n",strbuf);
          sprintf(strbuf,"TESTNAME=%s",node->value.harmonicset.name);
          fprintf(fp,"%s\n",strbuf);

          if(node->value.harmonicset.IAch)
           fprintf(fp,"IACH=1");
          else fprintf(fp,"IACH=0");
           fprintf(fp,"\n");


          if(node->value.harmonicset.IBch)
          fprintf(fp,"IBCH=1");
          else fprintf(fp,"IBCH=0");
          fprintf(fp,"\n");


          if(node->value.harmonicset.ICch)
          fprintf(fp,"ICCH=1");
          else fprintf(fp,"ICCH=0");
         fprintf(fp,"\n");

          if(node->value.harmonicset.TripValue.valid==true)
          {
             strcpy(strbuf,"<TESTITEMBEGIN>");
             fprintf(fp,"%s\n",strbuf);
             strcpy(strbuf,"TESTITEMNAME=HARMONIC.TRIPVALUE"); //用于区分 是动作值测试 还是动作时间测试
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HARMONICSTART=%s",node->value.harmonicset.TripValue.StartValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HARMONICEND=%s",node->value.harmonicset.TripValue.EndValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"STEP=%s",node->value.harmonicset.TripValue.StepValue.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"HOLDTIME=%s",node->value.harmonicset.TripValue.HoldTime.text);
             fprintf(fp,"%s\n",strbuf);
             sprintf(strbuf,"TRIPVALUE=%4.3f",report.harmonic.tripvalue);
             fprintf(fp,"%s\n",strbuf);
             //触发值评估
             if(node->value.harmonicset.TripValue.assesstripvalue.relerror.valid==true&&node->value.harmonicset.TripValue.assesstripvalue.abserror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPVALUE=REL(%f)",node->value.harmonicset.TripValue.assesstripvalue.relerror.errorvalue);
                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.harmonicset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }

             else if(node->value.harmonicset.TripValue.assesstripvalue.abserror.valid==true&&node->value.harmonicset.TripValue.assesstripvalue.relerror.valid!=true)
             {
                 sprintf(tmpstr1,"ASSESSTRIPVALUE=ABS(%f,%f)",\
                 node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue,\
                 node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);



                 fprintf(fp,"%s\n",tmpstr1);
                 sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.harmonicset.TripValue.assesstripvalue.CompareID);
                 fprintf(fp,"%s\n",tmpstr1);
             }


             else  if(node->value.harmonicset.TripValue.assesstripvalue.relerror.valid==true&&\
                node->value.harmonicset.TripValue.assesstripvalue.abserror.valid==true)
             {

                 if(node->value.harmonicset.TripValue.assesstripvalue.assessandor==1) //与的关系
                 {
                      sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)&ABS(%f,%f)",\
                      node->value.harmonicset.TripValue.assesstripvalue.relerror.errorvalue,\
                      node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue,\
                      node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);


                      fprintf(fp,"%s\n",strbuf);
                      sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.harmonicset.TripValue.assesstripvalue.CompareID);
                      fprintf(fp,"%s\n",tmpstr1);
                 }
                 else if(node->value.harmonicset.TripValue.assesstripvalue.assessandor==2)
                 {
                     sprintf(strbuf,"ASSESSTRIPVALUE=REL(%f)|ABS(%f,%f)",\
                     node->value.harmonicset.TripValue.assesstripvalue.relerror.errorvalue,\
                     node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue,\
                     node->value.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
                     fprintf(fp,"%s\n",strbuf);
                     sprintf(tmpstr1,"ASSESSTRIPVALUEID=%s",node->value.harmonicset.TripValue.assesstripvalue.CompareID);
                     fprintf(fp,"%s\n",tmpstr1);
                 }
             }


             strcpy(strbuf,"<TESTITEMEND>");
             fprintf(fp,"%s\n",strbuf);
          }
          sprintf(strbuf,"<TestfunctionEnd>");
          fprintf(fp,"%s\n",strbuf);
    }


}







void thread1::savereport()
{
    //读取本地的模板测试报告文件
 ReportLink->next=NULL;
 creatReportLink(); //读报告

 //报告的名字应该以模板文件名来设置
 if(strlen(ReportName)==0)
     return;
 char  Reportfilename[1024];
 sprintf(Reportfilename,"./TemplateDir/%s.mrp",ReportName);

 FILE *fp;
 char strbuf[512];
 fp=fopen(Reportfilename,"w+");
 if(fp==NULL)
 {
         printf("fopen error\n");
         return ;
 }
 // 读取本地模板的段链表中的id是从0 开始计数的，而报告聊表中的ID是从1开始计数的
  if(sectmentnodelink==NULL||sectmentnodelink->next==NULL) //段的聊表
      return;
   struct SECTMENTSETNODE *sectnext=sectmentnodelink->next;
   int  testnum=0;


   QDateTime time=QDateTime::currentDateTime();
   QString currenttime=time.toString("yyyy-MM-dd hh:mm:ss");
   QByteArray ch=currenttime.toLatin1();
   char *str=ch.data();
   fprintf(fp,"TESTTIME=%s\n",str);

   setsettinglinknoprint(settingnodelink);
   if(settingnodelink!=NULL&&settingnodelink->next!=NULL)
   {
   struct SETTINGNODELIST *settingnext=settingnodelink->next;
   strcpy(strbuf,"<SETTINGBEGIN>");
   fprintf(fp,"%s\n",strbuf);
   while(settingnext!=NULL)
   {
    sprintf(strbuf,"ID=%s,NAME=%s,VALUE=%s",\
                    settingnext->set.ID,\
                    settingnext->set.name,\
                    settingnext->set.value);
                   // settingnext->set.ref);
    fprintf(fp,"%s\n",strbuf);
    settingnext=settingnext->next;
   }
   strcpy(strbuf,"<SETTINGEND>");
   fprintf(fp,"%s\n",strbuf);
   }
   //添加 TestObjectBegin


  while(sectnext!=NULL)
  {
      bool VolTestValid=(sectnext->value.volset.valid==true&&(sectnext->value.volset.TripValue.valid==true||sectnext->value.volset.TripTime.valid==true));
      bool CurTestValid=(sectnext->value.curset.valid==true&&(sectnext->value.curset.TripValue.valid==true||sectnext->value.curset.TripTime.valid==true));
      bool DirTestValid=sectnext->value.dirset.valid;
      bool FirTestValid=(sectnext->value.freset.valid==true&&\
           (sectnext->value.freset.TripValue.valid||\
            sectnext->value.freset.TripTime.valid||\
            sectnext->value.freset.Tripdfdt.valid||\
            sectnext->value.freset.Tripdfdttime.valid||\
            sectnext->value.freset.TripVBlock.valid));
      bool PhaseTestValid=(sectnext->value.phraseset.valid==true&&(sectnext->value.phraseset.TripValue.valid==true||sectnext->value.phraseset.TripTime.valid==true));


     if(VolTestValid||CurTestValid||DirTestValid||FirTestValid||PhaseTestValid)
     {
      strcpy(strbuf,"<SectionsettingBegin>");
      fprintf(fp,"%s\n",strbuf);
      sprintf(strbuf,"sectionname:%s",sectnext->value.name);
      fprintf(fp,"%s\n",strbuf);
      testnum=0;
      if(sectnext->value.curset.valid==true)
      { sprintf(strbuf,"TESTNAME%d:%s",testnum,sectnext->value.curset.name);
        testnum++;
        fprintf(fp,"%s\n",strbuf);
      }
      if(sectnext->value.volset.valid==true)
      { sprintf(strbuf,"TESTNAME%d:%s",testnum,sectnext->value.volset.name);
        testnum++;
        fprintf(fp,"%s\n",strbuf);
      }
      if(sectnext->value.dirset.valid==true)
      { sprintf(strbuf,"TESTNAME%d:%s",testnum,sectnext->value.dirset.name);
        testnum++;
        fprintf(fp,"%s\n",strbuf);
      }
      if(sectnext->value.freset.valid==true)
      { sprintf(strbuf,"TESTNAME%d:%s",testnum,sectnext->value.freset.name);
        testnum++;
        fprintf(fp,"%s\n",strbuf);
      }
      if(sectnext->value.anytestset.valid==true)
      { sprintf(strbuf,"TESTNAME%d:%s",testnum,sectnext->value.anytestset.name);
        testnum++;
        fprintf(fp,"%s\n",strbuf);
      }
      if(sectnext->value.phraseset.valid==true)
      { sprintf(strbuf,"TESTNAME%d:%s",testnum,sectnext->value.phraseset.name);
        testnum++;
        fprintf(fp,"%s\n",strbuf);
      }

      Writetestfunc(sectnext,fp);
      strcpy(strbuf,"<SectionsettingEnd>");
      fprintf(fp,"%s\n",strbuf);
      sectnext=sectnext->next;
     }
     else
     {
       sectnext=sectnext->next;
     }
  }
  fclose(fp);
}




void thread1::key_deal_test()
{

    if(sectmentnodelink==NULL||sectmentnodelink->next==NULL)
      return;
    struct SECTMENTSETNODE *tmp=sectmentnodelink->next;//指向下一个节点，头结点无数据

    char strbuf[100];
    Clearreportfile(); //清除报告文件
    clearallresult();          //清楚所有的测试结果
    starttest=1;
    //显示第一个不为

    GPIO_OutSet(temprorarydata->gpio_fd, LEDNUM);

    emit SIG_TestState();

    SoftStartEndflag=false;

    while(tmp!=NULL)
    {

        memset(&sectset,0,sizeof(struct SECTMENTSET));
        sectset=tmp->value;
        bool VolTestValid=(sectset.volset.valid==true&&(sectset.volset.TripValue.valid==true||sectset.volset.TripTime.valid==true));
        bool CurTestValid=(sectset.curset.valid==true&&(sectset.curset.TripValue.valid==true||sectset.curset.TripTime.valid==true));
        bool DirTestValid=sectset.dirset.valid;
        bool FirTestValid=(sectset.freset.valid==true&&\
             (sectset.freset.TripValue.valid||\
              sectset.freset.TripTime.valid||\
              sectset.freset.Tripdfdt.valid||\
              sectset.freset.Tripdfdttime.valid||\
              sectset.freset.TripVBlock.valid));
        bool PhaseTestValid=(sectset.phraseset.valid==true&&(sectset.phraseset.TripValue.valid==true||sectset.phraseset.TripTime.valid==true));
        bool HarmonicTestValid=sectset.harmonicset.valid;//2016-12-15

        if(VolTestValid||CurTestValid||DirTestValid||FirTestValid||PhaseTestValid||HarmonicTestValid)//2016-12-15
        {
        sprintf(strbuf,"<SECTREPORTBEGIN>");
        writereportfile(strbuf);
        sprintf(strbuf,"sectment:%d",tmp->id);
        writereportfile(strbuf);
        currentsectid=tmp->id;
        currentprotecttype=0;
        protecttiontype[0]=sectset.volset.valid;
        protecttiontype[1]=sectset.curset.valid;
        protecttiontype[2]=sectset.dirset.valid;
        protecttiontype[3]=sectset.freset.valid;
        protecttiontype[4]=sectset.anytestset.valid;
        protecttiontype[5]=sectset.phraseset.valid;
        protecttiontype[6]=sectset.harmonicset.valid;//2016-12-15
        TestResultValid=false;
        emit  SIG_showfirstfuncofsect();

        //发送信号显示 这段
        }
        else
        {     tmp=tmp->next;
              continue;
        }

        if(VolTestValid) //一段 电压的测试 测试完成 后读状态文件 若手动停止的 则跳出测试
        {
            int Currenttestpos=0;
            currentprotecttype=0;
            emit  SIG_showfirstfuncofsect();

            sprintf(strbuf,"protection_type:voltage");
            writereportfile(strbuf);
            VOLTAGESET VoltageVar; //电压的变量
            memcpy(&VoltageVar,&sectset.volset,sizeof(VOLTAGESET));
            if(VoltageVar.TripValue.valid==true||VoltageVar.TripTime.valid==true)
            {
             sprintf(strbuf,"testitem=%s",VoltageVar.Testitem.text);
             writereportfile(strbuf);
            }

            int Testitem=atoi(VoltageVar.Testitem.text);
            if(Testitem&0x01)
            {
                Currenttestpos++;
            }


            if(VoltageVar.TripValue.valid==true)
            {
               // Currenttestpos++;
                emit SIG_showcurrenttestpos(Currenttestpos);

                CaculateVariable(settingnodelink,VoltageVar.Faultduraion); //故障持续时间
                CaculateVariable(settingnodelink,VoltageVar.FaultType); //故障故障类型
                CaculateVariable(settingnodelink,VoltageVar.TripValue.StartValue); //电压起始值
                CaculateVariable(settingnodelink,VoltageVar.TripValue.EndValue); //终止值
                CaculateVariable(settingnodelink,VoltageVar.TripValue.StepValue); //终止值
                CaculateVariable(settingnodelink,VoltageVar.TripValue.HoldTime); //终止值

                int  faulttype=(int)VoltageVar.FaultType.value;
                char tempfile[1024];
                sprintf(tempfile,"./TemplateDir/TemplateVoltage testitem=1 %d %f %d %f %f %f %f",\
                        faulttype,\
                        VoltageVar.Faultduraion.value,\
                        VoltageVar.Vpn_Vpp,\
                        VoltageVar.TripValue.StartValue.value,\
                        VoltageVar.TripValue.EndValue.value,\
                        VoltageVar.TripValue.StepValue.value,\
                        VoltageVar.TripValue.HoldTime.value);

                proc=new QProcess;
                proc->setReadChannel(QProcess::StandardOutput); //读后台TemplateVoltage程序的标准输出
                proc->setProcessChannelMode(QProcess::SeparateChannels);
                connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                proc->start(tempfile);  //传递模板文件名
                proc->waitForStarted(-1);

                proc->waitForFinished(-1);
                emit SIG_SendProcessStr(" ");
                float runstate=Readexitstate(); //这个文件是TemplateVoltage 负责写的

                //读运行状态的文件 若手动停止则 退出整个自动测试
                if(runstate==-2)
                {   starttest=0;
                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                    emit SIG_cleartestitemcolor();
                    return;
                }
                sectset.volset.TripValue.tripvalue=runstate;  //不等于-2就是实际的测试值
                emit  SIG_showfirstfuncofsect();
                sleep(1);
            }

            if(Testitem&0x02)
            {
                Currenttestpos++;
            }


            if(VoltageVar.TripTime.valid==true)
            {


                CaculateVariable(settingnodelink,VoltageVar.Faultduraion); //故障持续时间
                CaculateVariable(settingnodelink,VoltageVar.FaultType); //故障故障类型
                CaculateVariable(settingnodelink,VoltageVar.TripTime.EndVaule); //结束值
                CaculateVariable(settingnodelink,VoltageVar.TripTime.HoldTime); //保持时间

                CaculateVariable(settingnodelink,VoltageVar.TripValue.StartValue); //电压起始值
                CaculateVariable(settingnodelink,VoltageVar.TripValue.EndValue); //终止值


                char tempfile[1024];
                int faulttype=(int)VoltageVar.FaultType.value;


                if(VoltageVar.TripTime.UseMeasureValue==1) //使用测试结果
                {
                   //如果使用测试结果，而动作值测试未进行或者 动作值未触发则退出
                   if(VoltageVar.TripValue.valid==true&&sectset.volset.TripValue.tripvalue!=-1)
                   {
                   float OutputValue;
                   if(VoltageVar.TripValue.StartValue.value>VoltageVar.TripValue.EndValue.value)
                   OutputValue=sectset.volset.TripValue.tripvalue-2;//动作值加减2    G59保护说明查表
                   else
                   OutputValue=sectset.volset.TripValue.tripvalue+2;


                   emit SIG_showcurrenttestpos(Currenttestpos);
                   sprintf(tempfile,"./TemplateDir/TemplateVoltage testitem=2 %d %f %d %f %f",\
                           faulttype,\
                           VoltageVar.Faultduraion.value,\
                           VoltageVar.Vpn_Vpp,\
                           OutputValue,\
                           VoltageVar.TripTime.HoldTime.value);
                   proc=new QProcess;
                   proc->setReadChannel(QProcess::StandardOutput);
                   proc->setProcessChannelMode(QProcess::SeparateChannels);
                   connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                   proc->start(tempfile);  //传递模板文件名
                   proc->waitForStarted(-1);
                   proc->waitForFinished(-1);
                   emit SIG_SendProcessStr(" ");
                   float runstate=Readexitstate();

                   if(runstate==-2)
                  { starttest=0;
                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                    emit SIG_cleartestitemcolor();
                    return;
                  }
                   sectset.volset.TripTime.triptime=runstate;
                   emit  SIG_showfirstfuncofsect();
                   sleep(1);
                   }

                }

               else
                {
                    //Currenttestpos++;
                    emit SIG_showcurrenttestpos(Currenttestpos);
                    //使用界面值测试
                    sprintf(tempfile,"./TemplateDir/TemplateVoltage testitem=2 %d %f %d %f %f",\
                        faulttype,\
                        VoltageVar.Faultduraion.value,\
                        VoltageVar.Vpn_Vpp,\
                        VoltageVar.TripTime.EndVaule.value,\
                        VoltageVar.TripTime.HoldTime.value);

                    proc=new QProcess;
                    proc->setReadChannel(QProcess::StandardOutput);
                    proc->setProcessChannelMode(QProcess::SeparateChannels);
                    connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                    proc->start(tempfile);  //传递模板文件名
                    proc->waitForStarted(-1);
                    proc->waitForFinished(-1);
                    emit SIG_SendProcessStr(" ");
                    float runstate=Readexitstate();

                    if(runstate==-2)
                   { starttest=0;
                     GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                     emit SIG_cleartestitemcolor();
                     return;
                   }
                    sectset.volset.TripTime.triptime=runstate;
                    emit  SIG_showfirstfuncofsect();
                    sleep(1);


                }

            }

        }

        if(CurTestValid) //一段 电流的测试
        {
            int Currenttestpos=0;

            sprintf(strbuf,"protection_type:current");
            writereportfile(strbuf);
            currentprotecttype=1;
            emit  SIG_showfirstfuncofsect();
            CURRENTSET CurrentVar; //电压的变量
            memcpy(&CurrentVar,&sectset.curset,sizeof(CURRENTSET));

            if(CurrentVar.TripValue.valid==true||CurrentVar.TripTime.valid==true)
            {
             sprintf(strbuf,"testitem=%s",CurrentVar.Testitem.text);
             writereportfile(strbuf);
            }

            int Testitem=atoi(CurrentVar.Testitem.text);
            if(Testitem&0x01)
            {
                Currenttestpos++;
            }


            if(CurrentVar.TripValue.valid==true)
            {
                // Currenttestpos++;
                 emit SIG_showcurrenttestpos(Currenttestpos);

                CaculateVariable(settingnodelink,CurrentVar.Faultduraion); //故障持续时间
                CaculateVariable(settingnodelink,CurrentVar.output); //故障故障类型
                CaculateVariable(settingnodelink,CurrentVar.Vfault);
                CaculateVariable(settingnodelink,CurrentVar.Angle); //故障故障类型

                CaculateVariable(settingnodelink,CurrentVar.TripValue.StartValue); //电压起始值
                CaculateVariable(settingnodelink,CurrentVar.TripValue.EndValue); //终止值
                CaculateVariable(settingnodelink,CurrentVar.TripValue.StepValue); //终止值
                CaculateVariable(settingnodelink,CurrentVar.TripValue.HoldTime); //终止值

                char tempfile[1024];
                int OutputType=(int)CurrentVar.output.value;
                sprintf(tempfile,"./TemplateDir/TemplateCurrent testitem=1 %d %f %f %f %f %f %f %f",\
                        OutputType,\
                        CurrentVar.Faultduraion.value,\
                        CurrentVar.Vfault.value,\
                        CurrentVar.Angle.value,\
                        CurrentVar.TripValue.StartValue.value,\
                        CurrentVar.TripValue.EndValue.value,\
                        CurrentVar.TripValue.StepValue.value,\
                        CurrentVar.TripValue.HoldTime.value);


                proc=new QProcess;
                proc->setReadChannel(QProcess::StandardOutput);
                proc->setProcessChannelMode(QProcess::SeparateChannels);
                connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                proc->start(tempfile);  //传递模板文件名
                proc->waitForStarted(-1);
                proc->waitForFinished(-1);
                emit SIG_SendProcessStr(" ");
                float runstate=Readexitstate();

                if(runstate==-2)
                   { starttest=0;
                     GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                     emit SIG_cleartestitemcolor();
                     return;}
                 sectset.curset.TripValue.tripvalue=runstate;
                 emit  SIG_showfirstfuncofsect();
                sleep(1);

            }

            if(Testitem&0x02)
            {
                Currenttestpos++;
            }
            if(CurrentVar.TripTime.valid==true)
            {
               //  Currenttestpos++;
                 emit SIG_showcurrenttestpos(Currenttestpos);

                CaculateVariable(settingnodelink,CurrentVar.Faultduraion); //故障持续时间
                CaculateVariable(settingnodelink,CurrentVar.output); //故障故障类型
                CaculateVariable(settingnodelink,CurrentVar.Vfault); //故障故障类型
                CaculateVariable(settingnodelink,CurrentVar.Angle); //故障故障类型



                CaculateVariable(settingnodelink,CurrentVar.TripTime.EndVaule); //结束值
                CaculateVariable(settingnodelink,CurrentVar.TripTime.HoldTime); //保持时间

                char tempfile[1024];
                int OutputType=(int)CurrentVar.output.value;
                sprintf(tempfile,"./TemplateDir/TemplateCurrent testitem=2 %d %f %f %f %f %f",\
                        OutputType,\
                        CurrentVar.Faultduraion.value,\
                        CurrentVar.Vfault.value,\
                        CurrentVar.Angle.value,\
                        CurrentVar.TripTime.EndVaule.value,\
                        CurrentVar.TripTime.HoldTime.value);
                proc=new QProcess;
                proc->setReadChannel(QProcess::StandardOutput);
                proc->setProcessChannelMode(QProcess::SeparateChannels);
                connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                proc->start(tempfile);  //传递模板文件名
                proc->waitForStarted(-1);
                proc->waitForFinished(-1);
                emit SIG_SendProcessStr(" ");
                float runstate=Readexitstate();



                if(runstate==-2)
                  { starttest=0;
                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                     emit SIG_cleartestitemcolor();
                    return;
                  }
                sectset.curset.TripTime.triptime=runstate;
                emit  SIG_showfirstfuncofsect();
                sleep(1);

            }
        }

        if(DirTestValid) //一段 方向的测试
        {
           // qDebug()<<__func__<<__LINE__<<"direction test";
            int Currenttestpos=0;
            sprintf(strbuf,"protection_type:direction");
            writereportfile(strbuf);
            currentprotecttype=2;
            emit  SIG_showfirstfuncofsect();
            DIRECTIONSET DirectionVar; //电压的变量
            memcpy(&DirectionVar,&sectset.dirset,sizeof(DIRECTIONSET));

            if(DirectionVar.TripMta.valid==true)
            {
                 Currenttestpos++;
                 emit SIG_showcurrenttestpos(Currenttestpos);
                CaculateVariable(settingnodelink,DirectionVar.TripMta.Ifault);
                CaculateVariable(settingnodelink,DirectionVar.TripMta.vfault);
                CaculateVariable(settingnodelink,DirectionVar.TripMta.mta);
                CaculateVariable(settingnodelink,DirectionVar.TripMta.faulttype);
                CaculateVariable(settingnodelink,DirectionVar.TripMta.step);
                CaculateVariable(settingnodelink,DirectionVar.TripMta.steptime);

                char tempfile[1024];
                sprintf(tempfile,"./TemplateDir/TemplatePower %f %f %f %f %f %f",\
                        DirectionVar.TripMta.Ifault.value,\
                        DirectionVar.TripMta.vfault.value,\
                        DirectionVar.TripMta.mta.value,\
                        DirectionVar.TripMta.faulttype.value,\
                        DirectionVar.TripMta.step.value,\
                        DirectionVar.TripMta.steptime.value);
                proc=new QProcess;
                proc->setReadChannel(QProcess::StandardOutput);
                proc->setProcessChannelMode(QProcess::SeparateChannels);
                connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                proc->start(tempfile);  //传递模板文件名
                proc->waitForStarted(-1);
                proc->waitForFinished(-1);
                emit SIG_SendProcessStr(" ");
                float runstate=Readexitstate();
                if((int)runstate==-1000) //-1000 退出返回 -500 未触发未动作
                  {   starttest=0;
                      GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                      emit SIG_cleartestitemcolor();
                      return;
                  }
                sectset.dirset.TripMta.phrasemta=runstate;
                emit  SIG_showfirstfuncofsect();
                sleep(1);
            }
        }

        if(FirTestValid) //一段 频率的测试 频率测试需提示 用户设置
        {
           // qDebug()<<__func__<<__LINE__<<"freq test";
            int Currenttestpos=0;
            char tempfile[1024];
            strcpy(strbuf,"protection_type:frequency");
            writereportfile(strbuf);
            currentprotecttype=3;
            emit  SIG_showfirstfuncofsect();
            FRECURRENCESET  FreqVAR; //电压的变量
            memcpy(&FreqVAR,&sectset.freset,sizeof(FRECURRENCESET));
            if(FreqVAR.TripValue.valid==true||FreqVAR.TripTime.valid==true||\
               FreqVAR.Tripdfdt.valid==true||FreqVAR.Tripdfdttime.valid==true||\
               FreqVAR.TripVBlock.valid==true)
            {
             sprintf(strbuf,"testitem=%s",FreqVAR.TestItem.text);
             writereportfile(strbuf);

            }

            int Testitem=atoi(FreqVAR.TestItem.text);

            if(Testitem&0x01)
            {
              Currenttestpos++;
            }

            if(FreqVAR.TripValue.valid==true)
            {
            //  Currenttestpos++;
              emit SIG_showcurrenttestpos(Currenttestpos);


             CaculateVariable(settingnodelink,FreqVAR.Faultduraion); //故障持续时间
             CaculateVariable(settingnodelink,FreqVAR.TripValue.FreEndValueStart);
             CaculateVariable(settingnodelink,FreqVAR.TripValue.FreEndValueEnd);
             CaculateVariable(settingnodelink,FreqVAR.TripValue.FreStep);
             CaculateVariable(settingnodelink,FreqVAR.TripValue.HoldTime);
             sprintf(tempfile,"./TemplateDir/Templatedizhou testitem=1 %f %f %f %f %f",\
                     FreqVAR.Faultduraion.value,\
                     FreqVAR.TripValue.FreEndValueStart.value,\
                     FreqVAR.TripValue.FreEndValueEnd.value,\
                     FreqVAR.TripValue.FreStep.value,\
                     FreqVAR.TripValue.HoldTime.value);
             proc=new QProcess;
             proc->setReadChannel(QProcess::StandardOutput);
             proc->setProcessChannelMode(QProcess::SeparateChannels);
             connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
             proc->start(tempfile);  //传递模板文件名
             proc->waitForStarted(-1);
             proc->waitForFinished(-1);
             emit SIG_SendProcessStr(" ");
             float runstate=Readexitstate();

             if(runstate==-2)
               { starttest=0;
                     GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                   emit SIG_cleartestitemcolor();
                   return;}
             sectset.freset.TripValue.tripfrec=runstate;
             emit  SIG_showfirstfuncofsect();
              sleep(1);
            }

            if(Testitem&0x02)
            {
              Currenttestpos++;
            }
            if(FreqVAR.TripTime.valid==true)
            {
             //   Currenttestpos++;
                emit SIG_showcurrenttestpos(Currenttestpos);

                CaculateVariable(settingnodelink,FreqVAR.Faultduraion); //故障持续时间
                CaculateVariable(settingnodelink,FreqVAR.TripTime.FreEndValue);
                CaculateVariable(settingnodelink,FreqVAR.TripTime.HoldTime);
                sprintf(tempfile,"./TemplateDir/Templatedizhou testitem=2 %f %f %f",\
                        FreqVAR.Faultduraion.value,\
                        FreqVAR.TripTime.FreEndValue.value,\
                        FreqVAR.TripTime.HoldTime.value);
                proc=new QProcess;
                proc->setReadChannel(QProcess::StandardOutput);
                proc->setProcessChannelMode(QProcess::SeparateChannels);
                connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                proc->start(tempfile);  //传递模板文件名
                proc->waitForStarted(-1);
                proc->waitForFinished(-1);
                emit SIG_SendProcessStr(" ");
                float runstate=Readexitstate();

                if(runstate==-2)
                  { starttest=0;

                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                      emit SIG_cleartestitemcolor();
                      return;}
                sectset.freset.TripTime.triptime=runstate;
                emit  SIG_showfirstfuncofsect();
                sleep(1);
            }

            if(Testitem&0x04)
            {
              Currenttestpos++;
            }

            if(FreqVAR.Tripdfdt.valid==true)
            {
             // Currenttestpos++;
              emit SIG_showcurrenttestpos(Currenttestpos);

             CaculateVariable(settingnodelink,FreqVAR.Faultduraion); //故障持续时间
             CaculateVariable(settingnodelink,FreqVAR.Tripdfdt.DfdtStart);
             CaculateVariable(settingnodelink,FreqVAR.Tripdfdt.DfdtEnd);
             CaculateVariable(settingnodelink,FreqVAR.Tripdfdt.DfdtStep);
             CaculateVariable(settingnodelink,FreqVAR.Tripdfdt.FreEndValue);


             sprintf(tempfile,"./TemplateDir/Templatedizhou testitem=4 %f %f %f %f %f",\
                     FreqVAR.Faultduraion.value,\
                     FreqVAR.Tripdfdt.FreEndValue.value,\
                     FreqVAR.Tripdfdt.DfdtStart.value,\
                     FreqVAR.Tripdfdt.DfdtEnd.value,\
                     FreqVAR.Tripdfdt.DfdtStep.value);
             proc=new QProcess;
             proc->setReadChannel(QProcess::StandardOutput);
             proc->setProcessChannelMode(QProcess::SeparateChannels);
             connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
             proc->start(tempfile);  //传递模板文件名
             proc->waitForStarted(-1);
             proc->waitForFinished(-1);
             emit SIG_SendProcessStr(" ");
             float runstate=Readexitstate();

             if(runstate==-2)
              { starttest=0;
                     GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                   emit SIG_cleartestitemcolor();
                  return;}
             sectset.freset.Tripdfdt.tripdfdt=runstate;
             emit  SIG_showfirstfuncofsect();
             sleep(1);
            }

            if(Testitem&0x08)
            {
              Currenttestpos++;
            }

            if(FreqVAR.Tripdfdttime.valid==true)
            {
             Currenttestpos++;
            emit SIG_showcurrenttestpos(Currenttestpos);

             CaculateVariable(settingnodelink,FreqVAR.Faultduraion); //故障持续时间
             CaculateVariable(settingnodelink,FreqVAR.Tripdfdttime.dfdt);
             CaculateVariable(settingnodelink,FreqVAR.Tripdfdttime.Duration);
             CaculateVariable(settingnodelink,FreqVAR.Tripdfdttime.stopfreq);

             sprintf(tempfile,"./TemplateDir/Templatedizhou testitem=8 %f %f %f %f",\
                     FreqVAR.Faultduraion.value,\
                     FreqVAR.Tripdfdttime.dfdt.value,\
                     FreqVAR.Tripdfdttime.stopfreq.value,\
                     FreqVAR.Tripdfdttime.Duration.value);



             proc=new QProcess;
             proc->setReadChannel(QProcess::StandardOutput);
             proc->setProcessChannelMode(QProcess::SeparateChannels);
             connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
             proc->start(tempfile);  //传递模板文件名
             proc->waitForStarted(-1);
             proc->waitForFinished(-1);
             emit SIG_SendProcessStr(" ");
             float runstate=Readexitstate();
             if(runstate==-2)
              { starttest=0;
                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                   emit SIG_cleartestitemcolor();
                  return;}
             sectset.freset.Tripdfdttime.tripdfdttime=runstate;
             emit  SIG_showfirstfuncofsect();
             sleep(1);
            }

            if(Testitem&0x10)
            {
              Currenttestpos++;
            }
            if(FreqVAR.TripVBlock.valid==true)
            {
           //   Currenttestpos++;
              emit SIG_showcurrenttestpos(Currenttestpos);
             CaculateVariable(settingnodelink,FreqVAR.Faultduraion); //故障持续时间
             CaculateVariable(settingnodelink,FreqVAR.TripVBlock.Dfdt);
             CaculateVariable(settingnodelink,FreqVAR.TripVBlock.FreEndValue);
             CaculateVariable(settingnodelink,FreqVAR.TripVBlock.VStart);
             CaculateVariable(settingnodelink,FreqVAR.TripVBlock.VEnd);
             CaculateVariable(settingnodelink,FreqVAR.TripVBlock.VStep);
             CaculateVariable(settingnodelink,FreqVAR.TripVBlock.HoldTime);
             sprintf(tempfile,"./TemplateDir/Templatedizhou testitem=16 %f %f %f %f %f %f %f %d",\
                     FreqVAR.Faultduraion.value,\
                     FreqVAR.TripVBlock.Dfdt.value,\
                     FreqVAR.TripVBlock.FreEndValue.value,\
                     FreqVAR.TripVBlock.VStart.value,\
                     FreqVAR.TripVBlock.VEnd.value,\
                     FreqVAR.TripVBlock.VStep.value,\
                     FreqVAR.TripVBlock.HoldTime.value,\
                     FreqVAR.TripVBlock.VSelect);
             proc=new QProcess;
             proc->setReadChannel(QProcess::StandardOutput);
             proc->setProcessChannelMode(QProcess::SeparateChannels);
             connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
             proc->start(tempfile);  //传递模板文件名
             proc->waitForStarted(-1);
             proc->waitForFinished(-1);
             emit SIG_SendProcessStr(" ");
             float runstate=Readexitstate();


             if(runstate==-2)
               { starttest=0;
                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                    emit SIG_cleartestitemcolor();
                   return;}
             sectset.freset.TripVBlock.VBlockTrip=runstate;
             emit  SIG_showfirstfuncofsect();
              sleep(1);
            }
        }

        if(sectset.anytestset.valid==true&&sectset.anytestset.done==true) //任意测试
        {
            int Currenttestpos=0;
            emit SIG_showcurrenttestpos(Currenttestpos);
          //  qDebug()<<__func__<<__LINE__<<"anytest";
            sprintf(strbuf,"protection_type:anytest");
            writereportfile(strbuf);
            currentprotecttype=4;
            emit  SIG_showfirstfuncofsect();

            ANYTESTSET AnytestVar;
            memcpy(&AnytestVar,&sectset.anytestset,sizeof(ANYTESTSET));

            CaculateVariable(settingnodelink,AnytestVar.VAAP);
            CaculateVariable(settingnodelink,AnytestVar.VAPH);
            CaculateVariable(settingnodelink,AnytestVar.VAFR);

            CaculateVariable(settingnodelink,AnytestVar.VBAP);
            CaculateVariable(settingnodelink,AnytestVar.VBPH);
            CaculateVariable(settingnodelink,AnytestVar.VBFR);

            CaculateVariable(settingnodelink,AnytestVar.VCAP);
            CaculateVariable(settingnodelink,AnytestVar.VCPH);
            CaculateVariable(settingnodelink,AnytestVar.VCFR);

            CaculateVariable(settingnodelink,AnytestVar.IAAP);
            CaculateVariable(settingnodelink,AnytestVar.IAPH);
            CaculateVariable(settingnodelink,AnytestVar.IAFR);

            CaculateVariable(settingnodelink,AnytestVar.IBAP);
            CaculateVariable(settingnodelink,AnytestVar.IBPH);
            CaculateVariable(settingnodelink,AnytestVar.IBFR);

            CaculateVariable(settingnodelink,AnytestVar.ICAP);
            CaculateVariable(settingnodelink,AnytestVar.ICPH);
            CaculateVariable(settingnodelink,AnytestVar.ICFR);
            //步长
            CaculateVariable(settingnodelink,AnytestVar.Istep);
            CaculateVariable(settingnodelink,AnytestVar.Vstep);
            CaculateVariable(settingnodelink,AnytestVar.Pstep);
            CaculateVariable(settingnodelink,AnytestVar.Fstep);

            CaculateVariable(settingnodelink,AnytestVar.Steptime);
            CaculateVariable(settingnodelink,AnytestVar.Stepnum);

            CaculateVariable(settingnodelink,AnytestVar.Prefault);
            CaculateVariable(settingnodelink,AnytestVar.Infault);
            CaculateVariable(settingnodelink,AnytestVar.Postfault);
            CaculateVariable(settingnodelink,AnytestVar.Faultduraion);

            CaculateVariable(settingnodelink,AnytestVar.VariatyVA);
            CaculateVariable(settingnodelink,AnytestVar.VariatyVB);
            CaculateVariable(settingnodelink,AnytestVar.VariatyVC);
            CaculateVariable(settingnodelink,AnytestVar.VariatyVZ);

            CaculateVariable(settingnodelink,AnytestVar.VariatyIA);
            CaculateVariable(settingnodelink,AnytestVar.VariatyIB);
            CaculateVariable(settingnodelink,AnytestVar.VariatyIC);
            CaculateVariable(settingnodelink,AnytestVar.Testmode);
            CaculateVariable(settingnodelink,AnytestVar.ChangeItem);

            writeanytestsettingfile(AnytestVar);
            char tempfile[100];
            strcpy(tempfile,"./TemplateDir/Manual ./TemplateDir/anytestsetting.cfg");
            proc=new QProcess;
            proc->setReadChannel(QProcess::StandardOutput);
            proc->setProcessChannelMode(QProcess::SeparateChannels);
            connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
            proc->start(tempfile);  //传递模板文件名
            proc->waitForStarted(-1);
            proc->waitForFinished(-1);
            emit SIG_SendProcessStr(" ");
            float runstate=Readexitstate();

            if(runstate==-2)
              { starttest=0;
                   GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                   emit SIG_cleartestitemcolor();

                  return;}
           sectset.anytestset.triptime=runstate;
           emit  SIG_showfirstfuncofsect();
           sleep(1);
        }


        if(PhaseTestValid)//相位测试
        {
           // qDebug()<<__func__<<__LINE__<<"Vectorshift test";
            int Currenttestpos=0;
            sprintf(strbuf,"protection_type:Vectorshift");
            writereportfile(strbuf);
            currentprotecttype=5;
            emit  SIG_showfirstfuncofsect();


            PHRASESET phraseset; //电压的变量
            memcpy(&phraseset,&sectset.phraseset,sizeof(PHRASESET));
            if(phraseset.TripValue.valid==true||phraseset.TripTime.valid==true)
            {
             sprintf(strbuf,"testitem=%s",phraseset.Testitem.text);
             writereportfile(strbuf);
            }

            int Testitem=atoi(phraseset.Testitem.text);
            if(Testitem&0x01)
            {
                Currenttestpos++;
            }



            if(phraseset.TripValue.valid==true)
            {
               // Currenttestpos++;
                emit SIG_showcurrenttestpos(Currenttestpos);



                CaculateVariable(settingnodelink,phraseset.Faultduraion); //故障持续时间
                CaculateVariable(settingnodelink,phraseset.FixVoltage); //固定电压
                CaculateVariable(settingnodelink,phraseset.FixCurrent); //固定电压



                CaculateVariable(settingnodelink,phraseset.TripValue.StartValue); //电压起始值
                CaculateVariable(settingnodelink,phraseset.TripValue.EndValue); //终止值
                CaculateVariable(settingnodelink,phraseset.TripValue.StepValue); //终止值
                CaculateVariable(settingnodelink,phraseset.TripValue.HoldTime); //终止值


                char tempfile[1024];




                sprintf(tempfile,"./TemplateDir/TemplatePhase testitem=1 %f %f %f %d %d %d %d %d %d %f %f %f %f",\
                        phraseset.Faultduraion.value,\
                        phraseset.FixVoltage.value,\
                        phraseset.FixCurrent.value,\
                        phraseset.VAch,\
                        phraseset.VBch,\
                        phraseset.VCch,\
                        phraseset.IAch,\
                        phraseset.IBch,\
                        phraseset.ICch,\
                        phraseset.TripValue.StartValue.value,\
                        phraseset.TripValue.EndValue.value,\
                        phraseset.TripValue.StepValue.value,\
                        phraseset.TripValue.HoldTime.value);
                proc=new QProcess;
                proc->setReadChannel(QProcess::StandardOutput);
                proc->setProcessChannelMode(QProcess::SeparateChannels);
                connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                proc->start(tempfile);  //传递模板文件名
                proc->waitForStarted(-1);
                proc->waitForFinished(-1);
                emit SIG_SendProcessStr(" ");
                float runstate=Readexitstate();
                if(((int)runstate)==-1000)
                  {
                   starttest=0;
                   GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                   emit SIG_cleartestitemcolor();
                   return;
                  }
                sectset.phraseset.TripValue.tripvalue=runstate;
                emit  SIG_showfirstfuncofsect();
                sleep(1);
            }

            if(Testitem&0x02)
            {
                Currenttestpos++;
            }

            if(phraseset.TripTime.valid==true)
            {
              //  Currenttestpos++;
                emit SIG_showcurrenttestpos(Currenttestpos);

                CaculateVariable(settingnodelink,phraseset.Faultduraion); //故障持续时间
                CaculateVariable(settingnodelink,phraseset.FixVoltage); //固定电压
                CaculateVariable(settingnodelink,phraseset.FixCurrent); //固定电压


                CaculateVariable(settingnodelink,phraseset.TripTime.EndVaule); //结束值
                CaculateVariable(settingnodelink,phraseset.TripTime.HoldTime); //保持时间
                char tempfile[1024];
             //   qDebug()<<__func__<<__LINE__<<phraseset.TripTime.EndVaule.value<<phraseset.TripTime.HoldTime.value;
                sprintf(tempfile,"./TemplateDir/TemplatePhase testitem=2 %f %f %f %d %d %d %d %d %d %f %f",\
                        phraseset.Faultduraion.value,\
                        phraseset.FixVoltage.value,\
                        phraseset.FixCurrent.value,\
                        phraseset.VAch,\
                        phraseset.VBch,\
                        phraseset.VCch,\
                        phraseset.IAch,\
                        phraseset.IBch,\
                        phraseset.ICch,\
                        phraseset.TripTime.EndVaule.value,\
                        phraseset.TripTime.HoldTime.value);
                proc=new QProcess;
                proc->setReadChannel(QProcess::StandardOutput);
                proc->setProcessChannelMode(QProcess::SeparateChannels);
                connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                proc->start(tempfile);  //传递模板文件名
                proc->waitForStarted(-1);
                proc->waitForFinished(-1);
                emit SIG_SendProcessStr(" ");
                float runstate=Readexitstate();

                if(((int)runstate)==-1000)
                  { starttest=0;
                      GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                       emit SIG_cleartestitemcolor();

                      return;
                  }
                sectset.phraseset.TripTime.triptime=runstate;
                emit  SIG_showfirstfuncofsect();
                sleep(1);
            }
        }

        if(HarmonicTestValid) //一段 谐波的测试   2016-12-15
        {
           // qDebug()<<__func__<<__LINE__<<"harmonic test";
            int Currenttestpos=0;
            sprintf(strbuf,"protection_type:harmonic");
            writereportfile(strbuf);
            currentprotecttype=6;
            emit  SIG_showfirstfuncofsect();
            HARMONICSET HarmonicVar; //谐波的变量
            memcpy(&HarmonicVar,&sectset.harmonicset,sizeof(HARMONICSET));

            if(HarmonicVar.TripValue.valid==true)
            {
                 Currenttestpos++;
                 emit SIG_showcurrenttestpos(Currenttestpos);
                CaculateVariable(settingnodelink,HarmonicVar.HarmonicCount);
                CaculateVariable(settingnodelink,HarmonicVar.Current);
                CaculateVariable(settingnodelink,HarmonicVar.Phase);

                CaculateVariable(settingnodelink,HarmonicVar.TripValue.StartValue);
                CaculateVariable(settingnodelink,HarmonicVar.TripValue.EndValue);
                CaculateVariable(settingnodelink,HarmonicVar.TripValue.StepValue);
                CaculateVariable(settingnodelink,HarmonicVar.TripValue.HoldTime);

                char tempfile[1024];
                sprintf(tempfile,"./TemplateDir/TemplateHarmonic %f %f %f %f %f %f %f %d %d %d",\
                        HarmonicVar.HarmonicCount.value,\
                        HarmonicVar.Current.value,\
                        HarmonicVar.Phase.value,\
                        HarmonicVar.TripValue.StartValue.value,\
                        HarmonicVar.TripValue.EndValue.value,\
                        HarmonicVar.TripValue.StepValue.value,\
                        HarmonicVar.TripValue.HoldTime.value,\
                        HarmonicVar.IAch,\
                        HarmonicVar.IBch,\
                        HarmonicVar.ICch);
                proc=new QProcess;
                proc->setReadChannel(QProcess::StandardOutput);
                proc->setProcessChannelMode(QProcess::SeparateChannels);
                connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(getprocessstr()));
                proc->start(tempfile);  //传递模板文件名
                proc->waitForStarted(-1);
                proc->waitForFinished(-1);
                emit SIG_SendProcessStr(" ");
                float runstate=Readexitstate();
                if((int)runstate==-2) //
                  {   starttest=0;
                      GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
                      emit SIG_cleartestitemcolor();
                      return;
                  }
                sectset.harmonicset.TripValue.tripvalue=runstate;
                emit  SIG_showfirstfuncofsect();
                sleep(1);
            }
        }


        //测试完成后 将该段写回链表
        setsectfromid(sectmentnodelink,currentsectid,sectset);
        tmp=tmp->next;
        sprintf(strbuf,"<SECTREPORTEND>");
        writereportfile(strbuf);
        TestResultValid=true;
    }

   emit SIG_cleartestitemcolor();
   starttest=0;
   emit SIG_TestState();

   GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);



}

void thread1::clearallresult()
{
    if(sectmentnodelink==NULL||sectmentnodelink->next==NULL)
        return;
    struct SECTMENTSETNODE *next=sectmentnodelink->next;
    while(next!=NULL)
    {
        if(next->value.curset.valid)
        {
         next->value.curset.TripValue.tripvalue=-2;
         next->value.curset.TripTime.triptime=-2;
        }
        if(next->value.volset.valid)
        {
            next->value.volset.TripValue.tripvalue=-2;
            next->value.volset.TripTime.triptime=-2;

        }
        if(next->value.dirset.valid)
        {
          next->value.dirset.TripMta.actangle1=-1000;
          next->value.dirset.TripMta.actangle2=-1000;
          next->value.dirset.TripMta.phrasemta=-1000;
        }
        if(next->value.freset.valid)
        {
        next->value.freset.TripValue.tripfrec=-2;
        next->value.freset.TripTime.triptime=-2;
        next->value.freset.Tripdfdt.tripdfdt=-2;
        next->value.freset.Tripdfdttime.tripdfdttime=-2;
        next->value.freset.TripVBlock.VBlockTrip=-2;
        }

        if(next->value.anytestset.valid)
        {
          next->value.anytestset.triptime=-2;
        }

        if(next->value.phraseset.valid)
        {
            next->value.phraseset.TripValue.tripvalue=-1000;
            next->value.phraseset.TripTime.triptime=-2;
        }



        next=next->next;
    }
 }

void thread1::key_deal_stop()
{

}

void thread1::getprocessstr()
{
    QByteArray out;
    out=proc->readAll();
    char *outsr=out.data();
    emit SIG_SendProcessStr(outsr);

}

void thread1::getprocessstr2()
{

}




void thread1::key_response(KEY key)
{

 if(key==_NOP)   return;

  if(key==_TEST)
    {
       key_deal_test();

    }
    else if(key==_STOP)
    {
       key_deal_stop();
    }
}


void thread1::run()
{
    KEY key;
    temprorarydata->gpio_fd=open("/dev/em335x_gpio", O_RDWR);
    int rc = GPIO_OutEnable(temprorarydata->gpio_fd, 0xffffffff);
    if(rc < 0)
     {
       printf("GPIO_OutEnable::failed %d\n", rc);
       temprorarydata->gpio_fd=-1;
     }
     GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);

    while(Threadrunstate)
    {
      if(temprorarydata->setmanage_on==false&&temprorarydata->Wholekeypadon==false&&\
         temprorarydata->ShowParaOn==false&&temprorarydata->helpopenflag==false)
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

             }


        if(Saveflag==true&&TestResultValid==true)
        { //保存
        savereport();
        Saveflag=false;
        TestResultValid=false;
        }
        if(starttest==0)
        {
             emit SIG_TestState();

             msleep(100);
        }


      }
      else
      {

       tcflush(temprorarydata->UartDev[1].uartfd, TCIOFLUSH);
       msleep(500);

      }
}

          starttest=0;
          close(temprorarydata->UartDev[1].uartfd);


}


void thread1::stoprun(){

 Threadrunstate=false;
}

