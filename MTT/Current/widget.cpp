﻿#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QDebug>
#include <stdlib.h>
#include <stdio.h>
#include <QTime>
#include <QTextCodec>
#include "Config.h"
#include <QFile>
#include <QString>
#include <QPainter>
#include <math.h>
#include <QMouseEvent>
#include "qtranslator.h"

QTranslator *tor;
int current_language=2;  //1中文  2英文

extern TEMPRORARYDATA *temprorarydata;


float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;

unsigned char result[70],receive_data[70];
double out_time,change_timedata;
int   input_par=0,Decimal=0,sige_flag=0,HelpFlag=0;

long auto_i=0,IRQcnt=0,auto_count=0,tice=0;
//脰脨露脧鲁脤脨貌碌脛卤盲脕驴隆拢
int   INPAR=0,seq_count,SeqStep=0;
int   testcount=0,time_relay_testcount=0,auto_end=0;
int   cx,cy,cx_start,cx_mid,cx_end,increase,starttest=0,low,row,page;
int   I1DC,I2DC,I3DC;
int   V1DC,V2DC,V3DC;
int   TestItem1,TestItem3,TestItem2,OutputMode;

float StartCurrent,EndCurrent,ActStep,ReturnStep,StepTime,CurCoarseStep=1;

float LogicResolution,SystemFrec;
float TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime,ReturnQuotiety;

float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;



float ChangeValue,timeout;
float TripTime1,TripTime2,TripTime3;
int exit_par,save_f;

int sys[4];
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

float SystemVolt=57;
float faultduration=5;
float prefaulttime=0;
float postfaulttime;
float Preparetime;
float StopCurrent=4; //动作时间的用户手动输入的值
int   TripTimeType=2;   //测动作时间的方式
float StopCurrentHoldTime=2;//动作时间的保持时间


int CurrentTabindex=0; //当前的TAB的值



float Vfault=40;
float Angle=15;
float VpnValue; //单相的幅值
float VpnAngle;

volatile bool Threadrunstate=true;


bool SoftStartEndflag=false;


double V1ampPerStepError=0;  //电压VA每步变化的误差值
double V2ampPerStepError=0;  //电压VB每步变化的误差值
double V3ampPerStepError=0;  //电压VB每步变化的误差值
double VzampPerStepError=0;  //电压VZ每步变化的误差值

double I1ampPerStepError=0;
double I2ampPerStepError=0;
double I3ampPerStepError=0;




Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
init_par();
/* 加载语言文件 默认中文 2016-9-7*/
tor = new QTranslator();
if (current_language == 1) {
    tor->load(":/trans/zh.qm");
}
else if (current_language == 2) {
    tor->load(":/trans/en.qm");
}
qApp->installTranslator(tor);
 updateUI();
 QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
 QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
 QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
QFont font("wenquanyi",16);
setFont(font);
setFixedSize(800,480);
//ui->label_14->setText(("<font color=blue>"+QString("Current Test(2.0.0)")+"</font>"));

    showFullScreen();
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();

     //init_par();
     ui->tabWidget->setCurrentIndex(CurrentTabindex);
     ui->lineEdit->setText(QString::number(StartCurrent));
     ui->lineEdit_2->setText(QString::number(EndCurrent));
     ui->lineEdit_3->setText(QString::number(ActStep));
     ui->lineEdit_4->setText(QString::number(StepTime));
     ui->lineEdit_5->setText(QString::number(faultduration));
     ui->lineEdit_6->setText(QString::number(StopCurrent));
     ui->lineEdit_7->setText(QString::number(StopCurrentHoldTime));

     ui->lineEdit_8->setText(QString::number(Vfault));
     ui->lineEdit_9->setText(QString::number(Angle));

     ui->lineEdit_10->setText(QString::number(prefaulttime));


     ui->pushButton->setFocusPolicy(Qt::NoFocus);
     ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
     ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
      ui->pushButton_4->setFocusPolicy(Qt::NoFocus);




        ui->lineEdit->installEventFilter(this);
        ui->lineEdit_2->installEventFilter(this);
        ui->lineEdit_3->installEventFilter(this);
        ui->lineEdit_4->installEventFilter(this);
        ui->lineEdit_5->installEventFilter(this);
        ui->lineEdit_6->installEventFilter(this);
        ui->lineEdit_7->installEventFilter(this);
        ui->lineEdit_8->installEventFilter(this);
        ui->lineEdit_9->installEventFilter(this);
        ui->lineEdit_10->installEventFilter(this);

        ui->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_5->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_8->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_9->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_10->setContextMenuPolicy(Qt::NoContextMenu);



        ui->tabWidget->setCurrentIndex(0);
        TestItem1=1;
        TestItem2=1;
        TestItem3=0;
        ui->checkBox->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");




        if(TestItem2==1)
            ui->checkBox->setChecked(true);
        else
            ui->checkBox->setChecked(false);

    ui->label_11->clear();
    currenteditpos=0;
    display_variry(OutputMode);
    uartthread =new thread1;
    connect(uartthread,SIGNAL(SIG_testresult(int)),SLOT(showtestresult(int)));
    connect(uartthread,SIGNAL(SIG_DisplayChangeValue(bool,float)),SLOT(DisplayChangeValue(bool,float)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));

    connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));



    uartthread->start();

}

void Widget::updateUI()
{
    ui->label_14->setText(("<font color=blue>"+QString(tr("Current Test(2.1.0)"))+"</font>"));
    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_2->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_4->setText(tr("测试"));

        ui->label_12->setText(tr("测试电流(A)："));
        ui->label_15->setText(tr("最长时间(s):"));

        ui->label->setText(tr("电流初值(A)："));
        ui->label_3->setText(tr("电流终值(A)："));
        ui->label_2->setText(tr("电流步长(A)："));
        ui->label_4->setText(tr("步长时间(s):"));
        ui->label_5->setText(tr("保持时间(s):"));

        ui->label_9->setText(tr("故障前时间(s):"));
        ui->label_7->setText(tr("故障电压(V):"));
        ui->label_8->setText(tr("角(Deg):"));

        ui->checkBox->setText(tr("返回值"));


        ui->groupBox_3->setTitle(tr("故障类型"));
        ui->groupBox_4->setTitle(tr("测试结果"));

        ui->tabWidget->setTabText(0,tr("动作值"));
        ui->tabWidget->setTabText(1,tr("动作时间"));
}
void  Widget::vi_parameter_enter(int position)
{
    float Maxfactor=1;


        if(OutputMode>=1&&OutputMode<=3)
        Maxfactor=1;
        else if(OutputMode>=4&&OutputMode<=6)
        Maxfactor=1;
        else if(OutputMode==7)
        Maxfactor=1;

    switch(position)
    {
    case 1:
      if(StartCurrent>MAX_I_VALUEDATA_AC*Maxfactor)
        StartCurrent=MAX_I_VALUEDATA_AC*Maxfactor;
      else if(StartCurrent<0)
        StartCurrent=0;//鍙傛暟闄愬埗


      break;
   case 2:
      if(EndCurrent>MAX_I_VALUEDATA_AC*Maxfactor)
        EndCurrent=MAX_I_VALUEDATA_AC*Maxfactor;
      else if(EndCurrent<0)
        EndCurrent=0;//鍙傛暟闄愬埗

      break;
    case 3:
            if(ActStep>10)
              ActStep=10;
            else if(ActStep<0)
              ActStep=0;
            break;
    case 4:
            if(StepTime>999.99)
              StepTime=999.99;
            else if(StepTime<0)
              StepTime=0;
            break;
    case 5:
        if(faultduration>999.99)
          faultduration=999.99;
        else if(faultduration<0)
          faultduration=0;
        break;

    case 6:
        if(StopCurrent>MAX_I_VALUEDATA_AC*Maxfactor)
          StopCurrent=MAX_I_VALUEDATA_AC*Maxfactor;
        else if(StopCurrent<0)
          StopCurrent=0;
        break;
    case 7:
        if(StopCurrentHoldTime>999.99)
          StopCurrentHoldTime=999.99;
        else if(StopCurrentHoldTime<0)
          StopCurrentHoldTime=0;
        break;
    case 8:

        if(Vfault>MAX_V_VALUEDATA_AC*Maxfactor)
          Vfault=MAX_V_VALUEDATA_AC*Maxfactor;
        else if(Vfault<0)
          Vfault=0;//鍙傛暟闄愬埗
        break;

    case 9:
        if(Angle>=360)
          Angle=0;
        else if(Vfault<=-360)
          Vfault=0;
        break;

    case 10:
        if(prefaulttime>=999)
          prefaulttime=999;
        else if(prefaulttime<0)
          prefaulttime=0;
        break;

}

}


void Widget::showdspstate(int state)
{

    switch(state)
    {
    case 0: ui->label_6->setText(tr("run normally")); break;
    case 1: ui->label_6->setText(tr("IA overload")); break;
    case 2: ui->label_6->setText(tr("IABC temperature protect")); break;
    case 3: ui->label_6->setText(tr("IC resume")); break;
    case 4: ui->label_6->setText(tr("IB overload")); break;
    case 5: ui->label_6->setText(tr("IC closedown")); break;
    case 6: ui->label_6->setText(tr("IA resume")); break;
    case 7: ui->label_6->setText(tr("IC overload")); break;
    case 8: ui->label_6->setText(tr("IABC power protection")); break;
    case 9: ui->label_6->setText(tr("IB resume")); break;
    case 10: ui->label_6->setText(tr("VA overload")); break;

    case 12: ui->label_6->setText(tr("VC resume ")); break;
    case 13: ui->label_6->setText(tr("VB overload ")); break;
    case 14: ui->label_6->setText(tr("VABC temperature protection")); break;
    case 15: ui->label_6->setText(tr("VA resume")); break;
    case 16: ui->label_6->setText(tr("VC overload")); break;
    case 17: ui->label_6->setText(tr("VABC power protection")); break;
    case 18: ui->label_6->setText(tr("Vz overload")); break;
    case 19: ui->label_6->setText(tr("direct current overload")); break;
    case 20: ui->label_6->setText(tr("IABC power protection")); break;
    case 21: ui->label_6->setText(tr("VC resume")); break;
    }

}


void Widget::CloseWidget()
{

    //  while(uartthread->isRunning());
      free(temprorarydata);
      temprorarydata=NULL;
      close();
}


void Widget::DisplayChangeValue(bool flag, float value)
{
    if(flag)
    {
        ui->label_11->setText(QString::number(value,'g',6));
    }
    else ui->label_11->clear();
}



void Widget::showtestresult(int num)
{
 if(num==0)
     ui->textBrowser->clear();
 else if(num==1)
 {
     ui->textBrowser->clear();
     if(TripTime1==-1)
       ui->textBrowser->append(tr("trip value(A):no trip"));
     else
       ui->textBrowser->append(tr("trip value(A):")+QString::number(TripValue,'g',4));
 }
 else if(num==2)
 {
     if(TripTime2==-1)
       ui->textBrowser->append(tr("drop off(A):no trip"));
     else
     {  ui->textBrowser->append(tr("drop off(A):")+QString::number(ReturnValue,'g',4));
        float Temp;
         ReturnQuotiety=ReturnValue/TripValue;
         Temp=ReturnQuotiety*1000;
         Temp=ceil(Temp);
         ReturnQuotiety=Temp/1000;
         ui->textBrowser->append(tr("drop off ratio:")+QString::number(ReturnQuotiety,'g',4));
     }
 }
 else if(num==3)
 {

     ui->textBrowser->clear();
     if(TripTime!=-1)
     ui->textBrowser->setText(tr("trip time(s):")+QString::number(TripTime,'g',5));
     else
     ui->textBrowser->setText(tr("trip time(s):no trip"));


 }


}


void Widget::init_par()
{

    FILE *fp;
   if((fp=fopen("sysset.txt","a+"))==NULL)
    return;
   fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
          &SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&Preparetime,&current_language);
    fclose(fp);

        exit_par=0;
        TestItem1=1;
        TestItem2=1;
        TestItem3=0;

        StartCurrent=4;//0.8*StartCurrent;
        EndCurrent=6;//1.2*StartCurrent;
        ActStep=0.05;//0.01*StartCurrent;
        StepTime=0.5;  //FixedTime=0.5;
        ReturnStep=0.05;//0.01*StartCurrent;
        OutputMode=1;
        if(SystemFrec==0)
        {
        V1DC=1;
        V2DC=1;
        V3DC=1;

        I1DC=1;
        I2DC=1;
        I3DC=1;
        }
        else
        {
            V1DC=0;
            V2DC=0;
            V3DC=0;

            I1DC=0;
            I2DC=0;
            I3DC=0;
        }


        MAX_V_VALUEDATA_DC=176;
        MIN_V_VALUEDATA_DC=-176;
        MAX_V_VALUEDATA_AC=125;
        MIN_V_VALUEDATA_AC=0;
        MAX_I_VALUEDATA_DC=70;
        MIN_I_VALUEDATA_DC=-70;
        MAX_I_VALUEDATA_AC=50;
        MIN_I_VALUEDATA_AC=0;
}

void Widget::display_variry(int Outputmode)
{


    switch(Outputmode)
    {
         case 1:
           ui->radioButton->setChecked(true);
           ui->radioButton_2->setChecked(false);
           ui->radioButton_3->setChecked(false);
           ui->radioButton_4->setChecked(false);
           break;
         case 2:
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(true);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(false);
           break;
         case 3:
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(true);
        ui->radioButton_4->setChecked(false);
        break;
        case 4:
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(true);


        break;
    }

}



Widget::~Widget()
{
    delete ui;
}




void Widget::on_pushButton_clicked()
{
    if(starttest==1)
        return;

Helpdialog *helpdialog=new Helpdialog;
temprorarydata->helponflag=true;
connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
if(current_language==2)
helpdialog->SetHelpfilename("Current.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("Current1.txt");
//helpdialog->SetHelpfilename("Current.txt");
helpdialog->show();
}


void Widget::on_pushButton_2_clicked()
{
    if(save_f==0)
        return;
    save_f=0;
    if(current_language==2)
    {
            FILE *fp;
            char strbuf[80];
            int day,mon,year;
            int hour,minute,second;
            fp=fopen("report.txt","a+");
            if(fp==NULL)
            return;
            fprintf(fp,"\n");
            day=QDate::currentDate().day();
            mon=QDate::currentDate().month();
            year=QDate::currentDate().year();
            fprintf(fp,"Date:%d.%d.%d\r\n",mon,day,year);

            hour=QTime::currentTime().hour();
            minute=QTime::currentTime().minute();
            second=QTime::currentTime().second();
            fprintf(fp,"Time:%d.%d.%d\r\n",hour,minute,second);
            strcpy(strbuf,"Current test result");
            fprintf(fp,"%s\r\n",strbuf);

            if(TestItem1==1)
              {
                strcpy(strbuf,"pick up(A):");
                fprintf(fp,"%s",strbuf);
                if(TripValue==-1)
                  strcpy(strbuf,"No Trip");
                else
                  gcvt(TripValue,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }

            if(TestItem2==1)
              {
                strcpy(strbuf,"drop off(A):");
                fprintf(fp,"%s",strbuf);
                if(ReturnValue==-1)
                  strcpy(strbuf,"No Trip");
                else
                  gcvt(ReturnValue ,7,strbuf);
                fprintf(fp," %s\r\n",strbuf);
              }

            if(TestItem3==1)
              {
                strcpy(strbuf,"Trip time(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTime==-1)
                  strcpy(strbuf,"No trip");
                else
                  gcvt(TripTime ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }

            if(TestItem2==1&&TestItem1==1&&TripValue!=-1&&ReturnValue!=-1)
              {
                strcpy(strbuf,"drop off ratio:");
                fprintf(fp,"%s",strbuf);

                gcvt(ReturnQuotiety,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            fclose(fp);
            system("sync");
     }
    else if(current_language==1)
    {
        FILE *fp;
        char strbuf[80];
        int day,mon,year;
        int hour,minute,second;
        fp=fopen("report.txt","a+");
        if(fp==NULL)
        return;
        fprintf(fp,"\n");
        day=QDate::currentDate().day();
        mon=QDate::currentDate().month();
        year=QDate::currentDate().year();
        fprintf(fp,"Date:%d.%d.%d\r\n",mon,day,year);

        hour=QTime::currentTime().hour();
        minute=QTime::currentTime().minute();
        second=QTime::currentTime().second();
        fprintf(fp,"Time:%d.%d.%d\r\n",hour,minute,second);
        strcpy(strbuf,"Current test result");
        fprintf(fp,"%s\r\n",strbuf);

        if(TestItem1==1)
          {
            strcpy(strbuf,"动作值(A):");
            fprintf(fp,"%s",strbuf);
            if(TripValue==-1)
              strcpy(strbuf,"未动作");
            else
              gcvt(TripValue,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);
          }

        if(TestItem2==1)
          {
            strcpy(strbuf,"返回值(A):");
            fprintf(fp,"%s",strbuf);
            if(ReturnValue==-1)
              strcpy(strbuf,"未动作");
            else
              gcvt(ReturnValue ,7,strbuf);
            fprintf(fp," %s\r\n",strbuf);
          }

        if(TestItem3==1)
          {
            strcpy(strbuf,"动作时间(s):");
            fprintf(fp,"%s",strbuf);
            if(TripTime==-1)
              strcpy(strbuf,"未动作");
            else
              gcvt(TripTime ,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);
          }

        if(TestItem2==1&&TestItem1==1&&TripValue!=-1&&ReturnValue!=-1)
          {
            strcpy(strbuf,"返回系数:");
            fprintf(fp,"%s",strbuf);

            gcvt(ReturnQuotiety,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);
          }
        fclose(fp);
        system("sync");

    }


}

void Widget::on_pushButton_3_clicked()
{
    if(starttest==1)
        return;
  uartthread->stoprun();
  while(uartthread->isRunning())
  {
      uartthread->stoprun();
      sleep(1);
      break;
  }
    free(temprorarydata);
    temprorarydata=NULL;
    close();
}



void Widget::on_radioButton_clicked()
{
  OutputMode=1;
  //起始值
  if(StartCurrent>MAX_I_VALUEDATA_AC)
  {
      StartCurrent=MAX_I_VALUEDATA_AC;
      ui->lineEdit->setText(QString::number(StartCurrent));
  }
  else if(StartCurrent<MIN_I_VALUEDATA_AC)
  {
      StartCurrent=MIN_I_VALUEDATA_AC;
      ui->lineEdit->setText(QString::number(StartCurrent));
  }
  //终止值
  if(EndCurrent>MAX_I_VALUEDATA_AC)
  {
      EndCurrent=MAX_I_VALUEDATA_AC;
      ui->lineEdit_2->setText(QString::number(EndCurrent));
  }
  else if(EndCurrent<MIN_I_VALUEDATA_AC)
  {
      EndCurrent=MIN_I_VALUEDATA_AC;
      ui->lineEdit_2->setText(QString::number(EndCurrent));
  }
  ui->radioButton->setChecked(true);
  ui->radioButton_2->setChecked(false);
  ui->radioButton_3->setChecked(false);
  ui->radioButton_4->setChecked(false);
  ui->radioButton_5->setChecked(false);
  ui->radioButton_6->setChecked(false);
  ui->radioButton_7->setChecked(false);




}

void Widget::on_radioButton_2_clicked()
{
   OutputMode=2;
   //起始值
   if(StartCurrent>MAX_I_VALUEDATA_AC)
   {
       StartCurrent=MAX_I_VALUEDATA_AC;
       ui->lineEdit->setText(QString::number(StartCurrent));
   }
   else if(StartCurrent<MIN_I_VALUEDATA_AC)
   {
       StartCurrent=MIN_I_VALUEDATA_AC;
       ui->lineEdit->setText(QString::number(StartCurrent));
   }
   //终止值
   if(EndCurrent>MAX_I_VALUEDATA_AC)
   {
       EndCurrent=MAX_I_VALUEDATA_AC;
       ui->lineEdit_2->setText(QString::number(EndCurrent));
   }
   else if(EndCurrent<MIN_I_VALUEDATA_AC)
   {
       EndCurrent=MIN_I_VALUEDATA_AC;
       ui->lineEdit_2->setText(QString::number(EndCurrent));
   }
   ui->radioButton->setChecked(false);
   ui->radioButton_2->setChecked(true);
   ui->radioButton_3->setChecked(false);
   ui->radioButton_4->setChecked(false);
   ui->radioButton_5->setChecked(false);
   ui->radioButton_6->setChecked(false);
   ui->radioButton_7->setChecked(false);
}

void Widget::on_radioButton_3_clicked()
{
     OutputMode=3;
     //起始值
     if(StartCurrent>MAX_I_VALUEDATA_AC)
     {
         StartCurrent=MAX_I_VALUEDATA_AC;
         ui->lineEdit->setText(QString::number(StartCurrent));
     }
     else if(StartCurrent<MIN_I_VALUEDATA_AC)
     {
         StartCurrent=MIN_I_VALUEDATA_AC;
         ui->lineEdit->setText(QString::number(StartCurrent));
     }
     //终止值
     if(EndCurrent>MAX_I_VALUEDATA_AC)
     {
         EndCurrent=MAX_I_VALUEDATA_AC;
         ui->lineEdit_2->setText(QString::number(EndCurrent));
     }
     else if(EndCurrent<MIN_I_VALUEDATA_AC)
     {
         EndCurrent=MIN_I_VALUEDATA_AC;
         ui->lineEdit_2->setText(QString::number(EndCurrent));
     }
     ui->radioButton->setChecked(false);
     ui->radioButton_2->setChecked(false);
     ui->radioButton_3->setChecked(true);
     ui->radioButton_4->setChecked(false);
     ui->radioButton_5->setChecked(false);
     ui->radioButton_6->setChecked(false);
     ui->radioButton_7->setChecked(false);
}

void Widget::on_radioButton_4_clicked()
{
    OutputMode=4;
    //起始值
    if(StartCurrent>MAX_I_VALUEDATA_AC)
    {
        StartCurrent=MAX_I_VALUEDATA_AC;
        ui->lineEdit->setText(QString::number(StartCurrent));
    }
    else if(StartCurrent<MIN_I_VALUEDATA_AC)
    {
        StartCurrent=MIN_I_VALUEDATA_AC;
        ui->lineEdit->setText(QString::number(StartCurrent));
    }
    //终止值
    if(EndCurrent>MAX_I_VALUEDATA_AC)
    {
        EndCurrent=MAX_I_VALUEDATA_AC;
        ui->lineEdit_2->setText(QString::number(EndCurrent));
    }
    else if(EndCurrent<MIN_I_VALUEDATA_AC)
    {
        EndCurrent=MIN_I_VALUEDATA_AC;
        ui->lineEdit_2->setText(QString::number(EndCurrent));
    }
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(false);
    ui->radioButton_3->setChecked(false);
    ui->radioButton_4->setChecked(true);
    ui->radioButton_5->setChecked(false);
    ui->radioButton_6->setChecked(false);
    ui->radioButton_7->setChecked(false);
}
void Widget::on_radioButton_5_clicked()
{
   OutputMode=5;
   //起始值
   if(StartCurrent>MAX_I_VALUEDATA_AC)
   {
       StartCurrent=MAX_I_VALUEDATA_AC;
       ui->lineEdit->setText(QString::number(StartCurrent));
   }
   else if(StartCurrent<MIN_I_VALUEDATA_AC)
   {
       StartCurrent=MIN_I_VALUEDATA_AC;
       ui->lineEdit->setText(QString::number(StartCurrent));
   }
   //终止值
   if(EndCurrent>MAX_I_VALUEDATA_AC)
   {
       EndCurrent=MAX_I_VALUEDATA_AC;
       ui->lineEdit_2->setText(QString::number(EndCurrent));
   }
   else if(EndCurrent<MIN_I_VALUEDATA_AC)
   {
       EndCurrent=MIN_I_VALUEDATA_AC;
       ui->lineEdit_2->setText(QString::number(EndCurrent));
   }
   ui->radioButton->setChecked(false);
   ui->radioButton_2->setChecked(false);
   ui->radioButton_3->setChecked(false);
   ui->radioButton_4->setChecked(false);
   ui->radioButton_5->setChecked(true);
   ui->radioButton_6->setChecked(false);
   ui->radioButton_7->setChecked(false);
}

void Widget::on_radioButton_6_clicked()
{
  OutputMode=6;
  //起始值
  if(StartCurrent>MAX_I_VALUEDATA_AC)
  {
      StartCurrent=MAX_I_VALUEDATA_AC;
      ui->lineEdit->setText(QString::number(StartCurrent));
  }
  else if(StartCurrent<MIN_I_VALUEDATA_AC)
  {
      StartCurrent=MIN_I_VALUEDATA_AC;
      ui->lineEdit->setText(QString::number(StartCurrent));
  }
  //终止值
  if(EndCurrent>MAX_I_VALUEDATA_AC)
  {
      EndCurrent=MAX_I_VALUEDATA_AC;
      ui->lineEdit_2->setText(QString::number(EndCurrent));
  }
  else if(EndCurrent<MIN_I_VALUEDATA_AC)
  {
      EndCurrent=MIN_I_VALUEDATA_AC;
      ui->lineEdit_2->setText(QString::number(EndCurrent));
  }
  ui->radioButton->setChecked(false);
  ui->radioButton_2->setChecked(false);
  ui->radioButton_3->setChecked(false);
  ui->radioButton_4->setChecked(false);
  ui->radioButton_5->setChecked(false);
  ui->radioButton_6->setChecked(true);
  ui->radioButton_7->setChecked(false);
}

void Widget::on_radioButton_7_clicked()
{
   OutputMode=7;
   //起始值
   if(StartCurrent>MAX_I_VALUEDATA_AC)
   {
       StartCurrent=MAX_I_VALUEDATA_AC;
       ui->lineEdit->setText(QString::number(StartCurrent));
   }
   else if(StartCurrent<MIN_I_VALUEDATA_AC)
   {
       StartCurrent=MIN_I_VALUEDATA_AC;
       ui->lineEdit->setText(QString::number(StartCurrent));
   }
   //终止值
   if(EndCurrent>MAX_I_VALUEDATA_AC)
   {
       EndCurrent=MAX_I_VALUEDATA_AC;
       ui->lineEdit_2->setText(QString::number(EndCurrent));
   }
   else if(EndCurrent<MIN_I_VALUEDATA_AC)
   {
       EndCurrent=MIN_I_VALUEDATA_AC;
       ui->lineEdit_2->setText(QString::number(EndCurrent));
   }
   ui->radioButton->setChecked(false);
   ui->radioButton_2->setChecked(false);
   ui->radioButton_3->setChecked(false);
   ui->radioButton_4->setChecked(false);
   ui->radioButton_5->setChecked(false);
   ui->radioButton_6->setChecked(false);
   ui->radioButton_7->setChecked(true);
}





void Widget::setlineedittext(QString str)
{
    bool ok;
    float number=str.toFloat(&ok);
    if(ok==false)
        return;
    switch(currenteditpos)
    {
    case 1:
        StartCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StartCurrent,'g',6));
        break;
    case 2:
        EndCurrent=number;
        vi_parameter_enter(currenteditpos);

        currentedit->setText(QString::number(EndCurrent,'g',6));
        break;
    case 3:
        ActStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(ActStep,'g',6));
        break;
    case 4:
        StepTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StepTime,'g',6));
        break;
    case 5:
        faultduration=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(faultduration,'g',6));
        break;

    case 6:
        StopCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StopCurrent,'g',6));
        break;
    case 7:
        StopCurrentHoldTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StopCurrentHoldTime,'g',6));
        break;
    case 8:
        Vfault=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Vfault,'g',6));
        break;
    case 9:
        Angle=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Angle,'g',6));
        break;


    case 10:
        prefaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(prefaulttime,'g',6));
        break;

    }

}

void Widget::clearkeypadflag()
{
 temprorarydata->keypadopenflag=false;
}



void Widget::HelpDialogClose()
{
temprorarydata->helponflag=false;

}


bool Widget::eventFilter(QObject *obj, QEvent *event)
{
     if(starttest==1)
        return QWidget::eventFilter(obj,event);




    if(event->type()==QEvent::MouseButtonPress)
    {

         return QWidget::eventFilter(obj,event);

    }
    else if(event->type()==QEvent::MouseButtonDblClick)
   {

        QMouseEvent *mouseevent=static_cast<QMouseEvent *>(event);
        if(mouseevent->button()==Qt::RightButton)
        {
            return true;
        }
        if(obj==ui->lineEdit)
        {     temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit;
             currenteditpos=1;
        }
       else if(obj==ui->lineEdit_2)
        {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_2;
             currenteditpos=2;
        }
        else if(obj==ui->lineEdit_3)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_3;
              currenteditpos=3;
         }
        else if(obj==ui->lineEdit_4)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_4;
              currenteditpos=4;
         }
        else if(obj==ui->lineEdit_5)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_5;
              currenteditpos=5;
         }
        else if(obj==ui->lineEdit_6)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_6;
              currenteditpos=6;
         }
        else if(obj==ui->lineEdit_7)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_7;
              currenteditpos=7;
         }
        else if(obj==ui->lineEdit_8)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_8;
              currenteditpos=8;
         }
        else if(obj==ui->lineEdit_9)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_9;
              currenteditpos=9;
         }

        else if(obj==ui->lineEdit_10)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_10;
              currenteditpos=10;
         }



        pad=new keypads;
        pad->setModal(true);
        connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
        connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
        pad->show();
        pad->raise();
        pad->activateWindow();

      return true;
   }
    else
    return QWidget::eventFilter(obj,event);

}

void Widget::moveEvent(QMoveEvent *event)
{
    QPoint oldpos=event->oldPos();
    this->move(oldpos);
}





void Widget::on_tabWidget_currentChanged(int index)
{
    CurrentTabindex=index;
    if(CurrentTabindex==0)
    {
    TestItem1=1;
   // TestItem2=1;
    TestItem3=0;
    }
    else
    {
        TestItem1=0;
      //  TestItem2=0;
        TestItem3=1;

    }
}

void Widget::on_checkBox_clicked()
{
    if(TestItem2==1)
      TestItem2=0;
    else TestItem2=1;
}



void Widget::on_pushButton_4_clicked()
{
    if(SoftStartEndflag==true)
        return;
    if(starttest==1)
    {
        ui->pushButton_4->setText(tr("Test"));
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushButton_4->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
    }
    else
    {
        ui->pushButton_4->setText(tr("Stop"));
        ui->pushButton_4->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton->setEnabled(false);
    }
    SoftStartEndflag=true;


}
void Widget::GetTestState(){
    if(starttest==1)
    {
        ui->pushButton_4->setText(tr("Stop"));
         ui->pushButton_2->setEnabled(false);
         ui->pushButton_3->setEnabled(false);
         ui->pushButton->setEnabled(false);
        ui->pushButton_4->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
    }
    else
    {
        ui->pushButton_4->setText(tr("Test"));
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushButton_4->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
    }

}

void Widget::on_pushButton_6_clicked()//中
{
    current_language=1;
    tor = new QTranslator();
    if (current_language == 1) {
        tor->load(":/trans/zh.qm");
    }
    else if (current_language == 2) {
        tor->load(":/trans/en.qm");
    }
    qApp->installTranslator(tor);
     updateUI();
}

void Widget::on_pushButton_7_clicked()//En
{
    current_language=2;
    tor = new QTranslator();
    if (current_language == 1) {
        tor->load(":/trans/zh.qm");
    }
    else if (current_language == 2) {
        tor->load(":/trans/en.qm");
    }
    qApp->installTranslator(tor);
     updateUI();
}
