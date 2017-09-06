#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QDebug>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <QTime>
#include <QTextCodec>
#include "Config.h"
#include <QFile>
#include <QString>
#include <QPainter>
#include <QWSServer>
#include <QMouseDriverFactory>
#include "qtranslator.h"

QTranslator *tor;
int current_language=2;  //1中文  2英文

float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

unsigned char result[70],receive_data[70];
double out_time,change_timedata;
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
long  auto_i,IRQcnt,auto_count,tice;
int   cursorF=0,cursor=0,cursorK=0;
int   input_par=0,Decimal=0,sige_flag=0,HelpFlag=0,starttest1=0;

int   starttest=0;
int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;

int   seq_count,TestItem1,TestItem2,TestItem3,TestItem4,TestItem5;

float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
float TripTime1,TripTime2,TripTime3,TripTime4,TripTime5,TripVoltage,TimeVoltage;
float ReturnValue;


float LogicResolution,SystemFrec,Phase,SystemVolt;
float timeout;
int save_f;
int sys[4];
char ParStr[20];  //ParStr:为参数输入时记录的输入参数的字符串。
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
float ReturnQuotiety;



float faultduration=5;
float prefaulttime=3;
float postfaulttime=2;
float Preparetime;
float StartVoltage,EndVoltage;
float ChangeValue;

float dvdtStopVoltage=48;  //dvdt的停止电压
float dvdtstart=4;
float dvdtend=1;
float dvdtStep=1;
float currentdvdt=1;
float dvdttripvalue;



float StopVoltage_triptime=48;
float dvdt_triptime=3;
float holdtime_triptime;

float dtofdvdt=0.01;
float dtofdvdttime=0.01;


int Variable=1; //1 VAB 2 VBC 3 VCA


float VStart=60,VStop=30,VStep=1,VSteptime=0.5;
float StopVoltage=30;
float StopVoltageHoldTime=2;

float TripValue,TripValue1;


bool Threadrunstate=true;

extern TEMPRORARYDATA *temprorarydata;


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
    QFont font("wenquanyi",13);
    setFont(font);
    //init_par();
    sysinit();
    setFixedSize(800,480);
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();
    ui->tabWidget->setCurrentIndex(0);
        showFullScreen();
    if(TestItem2==1)
    {

     ui->checkBox->setChecked(true);
    }


      //ui->label->setText("<font color=blue>"+QString("Voltage Relay(2.0.0)")+"</font>");

      if(Variable>=1&&Variable<=3)
     {
      holdtime_triptime=fabs(SystemVolt-StopVoltage_triptime)/dvdt_triptime;
      ui->label_9->setText(QString::number(fabs(SystemVolt-StopVoltage_triptime)/dvdt_triptime,'g',6));
     }
      else
      {
          holdtime_triptime=fabs(SystemVolt*1.732-StopVoltage_triptime)/dvdt_triptime;
          ui->label_9->setText(QString::number(fabs(SystemVolt*1.732-StopVoltage_triptime)/dvdt_triptime,'g',6));
      }


       ui->checkBox->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");

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
       ui->lineEdit_11->installEventFilter(this);
       ui->lineEdit_12->installEventFilter(this);
       ui->lineEdit_13->installEventFilter(this);
       ui->lineEdit_14->installEventFilter(this);
       ui->lineEdit_15->installEventFilter(this);
       ui->lineEdit_16->installEventFilter(this);
       ui->lineEdit_17->installEventFilter(this);



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
       ui->lineEdit_11->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_12->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_13->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_14->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_15->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_16->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_17->setContextMenuPolicy(Qt::NoContextMenu);



       ui->pushButton->setFocusPolicy(Qt::NoFocus);
       ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
       ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
       ui->pushButton_4->setFocusPolicy(Qt::NoFocus);


       uartthread =new thread1;
       connect(uartthread,SIGNAL(SIG_testresult(int)),this,SLOT(showtestresult(int)));
       connect(uartthread,SIGNAL(SIG_DisplayChangeValue(bool,float)),this,SLOT(DisplayChangeValue(bool,float)));
       connect(uartthread,SIGNAL(SIG_dspstate(int)),this,SLOT(showdspstate(int)));
       connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
       connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
       uartthread->start();
}
void Widget::updateUI()
{
    ui->label->setText("<font color=blue>"+QString(tr("电压继电器(2.1.0)"))+"</font>");
    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_2->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_4->setText(tr("测试"));

        ui->label_10->setText(tr("起始值(V)："));
        ui->label_17->setText(tr("终止值(V)："));
        ui->label_18->setText(tr("步长(V)："));
        ui->label_19->setText(tr("步长时间(s)："));
        ui->label_20->setText(tr("测试电压(V):"));
        ui->label_21->setText(tr("最长时间(s):"));
        ui->label_3->setText(tr("终止电压(V):"));
        ui->label_4->setText(tr("滑差起始值(V/s):"));
        ui->label_11->setText(tr("滑差终止值(V/s):"));
        ui->label_2->setText(tr("滑差步长(V/s):"));
        ui->label_22->setText(tr("滑差精度(s):"));
        ui->label_13->setText(tr("终止电压(V):"));
        ui->label_12->setText(tr("测试滑差(V/s):"));
        ui->label_23->setText(tr("滑差精度(s):"));
        ui->label_14->setText(tr("终止电压持续时间(s):"));
        ui->label_15->setText(tr("故障前时间(s):"));
        //ui->label_22->setText(tr("负荷电流(A):"));
        ui->label_16->setText(tr("动作保持时间(s):"));
        ui->label_27->setText(tr("故障后时间(s):"));

        ui->groupBox->setTitle(tr("故障类型"));
        ui->groupBox_4->setTitle(tr("测试结果"));

        ui->checkBox->setText(tr("返回值"));

        ui->tabWidget->setTabText(0,tr("动作值"));
        ui->tabWidget->setTabText(1,tr("固定电压测试"));
        ui->tabWidget->setTabText(2,tr("滑差闭锁值"));
        ui->tabWidget->setTabText(3,tr("动作时间"));
}
void Widget::moveEvent(QMoveEvent *event)
{
 QPoint oldpos=event->oldPos();
 this->move(oldpos);
}


void Widget::init_par()
{
       FILE *fp;
       if((fp=fopen("sysset.txt","a+"))==NULL)
       return;
       fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
              &SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&Preparetime,&current_language);
       fclose(fp);

               TestItem1=1;
               TestItem2=1;
               TestItem3=0;
               TestItem4=0;
               TestItem5=0;



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

void Widget::sysinit()
{

  ui->lineEdit->setText(QString::number(dvdtStopVoltage));
  ui->lineEdit_2->setText(QString::number(dvdtstart));
  ui->lineEdit_3->setText(QString::number(dvdtend));
  ui->lineEdit_4->setText(QString::number(dvdtStep));

  ui->lineEdit_5->setText(QString::number(StopVoltage_triptime));
  ui->lineEdit_6->setText(QString::number(dvdt_triptime));

  ui->lineEdit_7->setText(QString::number(prefaulttime));
  ui->lineEdit_8->setText(QString::number(postfaulttime));
  ui->lineEdit_9->setText(QString::number(faultduration));

  ui->lineEdit_10->setText(QString::number(VStart));
  ui->lineEdit_11->setText(QString::number(VStop));
  ui->lineEdit_12->setText(QString::number(VStep));
  ui->lineEdit_13->setText(QString::number(VSteptime));

  ui->lineEdit_14->setText(QString::number(StopVoltage));
  ui->lineEdit_15->setText(QString::number(StopVoltageHoldTime));

  ui->lineEdit_16->setText(QString::number(dtofdvdt));
  ui->lineEdit_17->setText(QString::number(dtofdvdttime));


  ui->checkBox->setChecked(TestItem2);

  ui->radioButton->setChecked(true);
  ui->radioButton_2->setChecked(false);
  ui->radioButton_3->setChecked(false);
  ui->radioButton_4->setChecked(false);
  ui->radioButton_5->setChecked(false);
  ui->radioButton_6->setChecked(false);
  ui->radioButton_7->setChecked(false);

}

void Widget::ShowRadio(int type)
{


    if(Variable>=1&&Variable<=3)
   {
    holdtime_triptime=fabs(SystemVolt-StopVoltage_triptime)/dvdt_triptime;
    ui->label_9->setText(QString::number(fabs(SystemVolt-StopVoltage_triptime)/dvdt_triptime,'g',6));
   }
    else
    {
        holdtime_triptime=fabs(SystemVolt*1.732-StopVoltage_triptime)/dvdt_triptime;
        ui->label_9->setText(QString::number(fabs(SystemVolt*1.732-StopVoltage_triptime)/dvdt_triptime,'g',6));
    }

    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(false);
    ui->radioButton_3->setChecked(false);
    ui->radioButton_4->setChecked(false);
    ui->radioButton_5->setChecked(false);
    ui->radioButton_6->setChecked(false);
    ui->radioButton_7->setChecked(false);
    switch(type)
    {
    case 1:
        //起始值
        if(VStart>MAX_V_VALUEDATA_AC)
        {
            VStart=MAX_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        else if(VStart<MIN_V_VALUEDATA_AC)
        {
            VStart=MIN_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        //终止值
        if(VStop>MAX_V_VALUEDATA_AC)
        {
            VStop=MAX_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        else if(VStop<MIN_V_VALUEDATA_AC)
        {
            VStop=MIN_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        ui->radioButton->setChecked(true);
        break;


    case 2:
        //起始值
        if(VStart>MAX_V_VALUEDATA_AC)
        {
            VStart=MAX_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        else if(VStart<MIN_V_VALUEDATA_AC)
        {
            VStart=MIN_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        //终止值
        if(VStop>MAX_V_VALUEDATA_AC)
        {
            VStop=MAX_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        else if(VStop<MIN_V_VALUEDATA_AC)
        {
            VStop=MIN_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        ui->radioButton_2->setChecked(true);

        break;

    case 3:
        //起始值
        if(VStart>MAX_V_VALUEDATA_AC)
        {
            VStart=MAX_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        else if(VStart<MIN_V_VALUEDATA_AC)
        {
            VStart=MIN_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        //终止值
        if(VStop>MAX_V_VALUEDATA_AC)
        {
            VStop=MAX_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        else if(VStop<MIN_V_VALUEDATA_AC)
        {
            VStop=MIN_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        ui->radioButton_3->setChecked(true);

        break;

    case 4:
        //起始值
        if(VStart>MAX_V_VALUEDATA_AC*1.732)
        {
            VStart=MAX_V_VALUEDATA_AC*1.732;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        else if(VStart<MIN_V_VALUEDATA_AC)
        {
            VStart=MIN_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        //终止值
        if(VStop>MAX_V_VALUEDATA_AC*1.732)
        {
            VStop=MAX_V_VALUEDATA_AC*1.732;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        else if(VStop<MIN_V_VALUEDATA_AC)
        {
            VStop=MIN_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        ui->radioButton_4->setChecked(true);

        break;

    case 5:
        //起始值
        if(VStart>MAX_V_VALUEDATA_AC*1.732)
        {
            VStart=MAX_V_VALUEDATA_AC*1.732;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        else if(VStart<MIN_V_VALUEDATA_AC)
        {
            VStart=MIN_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        //终止值
        if(VStop>MAX_V_VALUEDATA_AC*1.732)
        {
            VStop=MAX_V_VALUEDATA_AC*1.732;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        else if(VStop<MIN_V_VALUEDATA_AC)
        {
            VStop=MIN_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
         ui->radioButton_5->setChecked(true);
        break;

    case 6:
        //起始值
        if(VStart>MAX_V_VALUEDATA_AC*1.732)
        {
            VStart=MAX_V_VALUEDATA_AC*1.732;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        else if(VStart<MIN_V_VALUEDATA_AC)
        {
            VStart=MIN_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        //终止值
        if(VStop>MAX_V_VALUEDATA_AC*1.732)
        {
            VStop=MAX_V_VALUEDATA_AC*1.732;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        else if(VStop<MIN_V_VALUEDATA_AC)
        {
            VStop=MIN_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        ui->radioButton_6->setChecked(true);
        break;


    case 7:
        //起始值
        if(VStart>MAX_V_VALUEDATA_AC)
        {
            VStart=MAX_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        else if(VStart<MIN_V_VALUEDATA_AC)
        {
            VStart=MIN_V_VALUEDATA_AC;
            ui->lineEdit_10->setText(QString::number(VStart));
        }
        //终止值
        if(VStop>MAX_V_VALUEDATA_AC)
        {
            VStop=MAX_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        else if(VStop<MIN_V_VALUEDATA_AC)
        {
            VStop=MIN_V_VALUEDATA_AC;
            ui->lineEdit_11->setText(QString::number(VStop));
        }
        ui->radioButton_7->setChecked(true);
        break;



    }



}




void Widget::showdspstate(int state)
{

    switch(state)
       {
        case 0: ui->label_8->setText(tr("run normally")); break;
        case 1: ui->label_8->setText(tr("IA overload")); break;
        case 2: ui->label_8->setText(tr("IABC temperature protect")); break;
        case 3: ui->label_8->setText(tr("IC resume")); break;
        case 4: ui->label_8->setText(tr("IB overload")); break;
        case 5: ui->label_8->setText(tr("IC closedown")); break;
        case 6: ui->label_8->setText(tr("IA resume")); break;
        case 7: ui->label_8->setText(tr("IC overload")); break;
        case 8: ui->label_8->setText(tr("IABC power protection")); break;
        case 9: ui->label_8->setText(tr("IB resume")); break;
        case 10: ui->label_8->setText(tr("VA overload")); break;

        case 12: ui->label_8->setText(tr("VC resume ")); break;
        case 13: ui->label_8->setText(tr("VB overload ")); break;
        case 14: ui->label_8->setText(tr("VABC temperature protection")); break;
        case 15: ui->label_8->setText(tr("VA resume")); break;
        case 16: ui->label_8->setText(tr("VC overload")); break;
        case 17: ui->label_8->setText(tr("VABC power protection")); break;
        case 18: ui->label_8->setText(tr("Vz overload")); break;
        case 19: ui->label_8->setText(tr("direct current overload")); break;
        case 20: ui->label_8->setText(tr("IABC power protection")); break;
        case 21: ui->label_8->setText(tr("VC resume")); break;
       }


}



void Widget::CloseWidget()
{

}


void Widget::DisplayChangeValue(bool flag, float value)
{
    if(flag)
    {

    ui->label_5->setText(QString::number(value,'g',6));
    }
    else
   {
    ui->label_5->setText(tr(""));
   }
}

void Widget::showtestresult(int num)
{
switch(num)
{
case 0:
     ui->textBrowser->clear();
     break;

case 1:
        ui->textBrowser->clear();
        if(TripTime1==-1)
        ui->textBrowser->append(tr("pick up(V):no trip"));
        else
        ui->textBrowser->append(tr("pick up(V):")+QString::number(TripValue));
        break;
case 2:
    if(TripTime2==-1)
     ui->textBrowser->append(tr("drop off(V):no trip"));
    else
     ui->textBrowser->append(tr("drop off(V):")+QString::number(ReturnValue));
    if((TripTime1!=-1&&TripTime2!=-1))
       {
            float Temp;
             ReturnQuotiety=ReturnValue/TripValue;
             Temp=ReturnQuotiety*1000;
             Temp=ceil(Temp);
             ReturnQuotiety=Temp/1000;
             ui->textBrowser->append(tr("drop off ratio:")+QString::number(ReturnQuotiety,'g',4));
       }
    break;

case 3:

    ui->textBrowser->clear();
    if(TripTime3!=-1)
    ui->textBrowser->setText(tr("trip time(s):")+QString::number(TripTime3,'g',5));
    else
    ui->textBrowser->setText(tr("trip time(s):no trip"));
    break;

case 4:
    if(TripTime4==-1)
      ui->textBrowser->append(tr("dvdt trip value(V/s):no trip"));
    else
      ui->textBrowser->append(tr("dvdt trip value(V/s):")+QString::number(dvdttripvalue,'g',6));
    break;
case 5:
    if(TripTime5==-1)
    ui->textBrowser->append(tr("dvdt trip time(s):no trip"));
    else
    ui->textBrowser->append(tr("dvdt trip time(s):")+QString::number(TripTime5,'g',6));
    break;

}





}

void Widget::vi_parameter_enter(int position)
{
    float Maxfactor=1;


        if(Variable>=1&&Variable<=3)
        Maxfactor=1;
        else if(Variable>=4&&Variable<=6)
        Maxfactor=1.732;
        else if(Variable==7)
        Maxfactor=1;
    switch(position)
    {
    case 1:
        if(dvdtStopVoltage>MAX_V_VALUEDATA_AC*Maxfactor)
        dvdtStopVoltage=MAX_V_VALUEDATA_AC*Maxfactor;
        else if(dvdtStopVoltage<0)
        dvdtStopVoltage=0;
        break;
    case 2:
        if(dvdtstart>999.9)
            dvdtstart=999.9;
        else if(dvdtstart<0)
            dvdtstart=0;
        break;
    case 3:
        if(dvdtend>999.9)
            dvdtend=999.9;
        else if(dvdtend<0)
            dvdtend=0;
        break;
    case 4:
        if(dvdtStep>999.9)
            dvdtStep=999.9;
        else if(dvdtStep<0)
            dvdtStep=0;
        break;
    case 5:
        if(StopVoltage_triptime>MAX_V_VALUEDATA_AC*Maxfactor)
        StopVoltage_triptime=MAX_V_VALUEDATA_AC*Maxfactor;
        else if(StopVoltage_triptime<0)
        StopVoltage_triptime=0;

        break;
    case 6:
        if(dvdt_triptime>999.9)
        dvdt_triptime=999.9;
        else if(dvdt_triptime<0)
        dvdt_triptime=0;
        break;
    case 7:
        if(prefaulttime>999.9)
        prefaulttime=999.9;
        else if(prefaulttime<0)
        prefaulttime=0;
        break;
    case 8:
        if(postfaulttime>999.9)
        postfaulttime=999.9;
        else if(postfaulttime<0)
        postfaulttime=0;
        break;
    case 9:
        if(faultduration>999.9)
        faultduration=999.9;
        else if(faultduration<0)
        faultduration=0;
        break;

    case 10:
        if(VStart>MAX_V_VALUEDATA_AC*Maxfactor)
        VStart=MAX_V_VALUEDATA_AC*Maxfactor;
        else if(VStart<0)
        VStart=0;
        break;
    case 11:
        if(VStop>MAX_V_VALUEDATA_AC*Maxfactor)
           VStop=MAX_V_VALUEDATA_AC*Maxfactor;
          else if(VStop<0)
           VStop=0;
          break;
    case 12:
        if(VStep>10)
            VStep=10;
        else if(VStep<0)
            VStep=0.1;//
        break;
    case 13:
        if(VSteptime>999.99)
            VSteptime=999.99;
        else if(VSteptime<0)
            VSteptime=0.1;//
        break;

    case 14:
        if(StopVoltage>MAX_V_VALUEDATA_AC*Maxfactor)
        StopVoltage=MAX_V_VALUEDATA_AC*Maxfactor;
        else if(StopVoltage<0)
        StopVoltage=0;
        break;
    case 15:
        if(StopVoltageHoldTime>999.99)
            StopVoltageHoldTime=999.99;
        else if(StopVoltageHoldTime<0)
            StopVoltageHoldTime=0.1;
        break;


    case 16:
        if(dtofdvdt>999.99)
            dtofdvdt=999.99;
        else if(dtofdvdt<0)
            dtofdvdt=0.1;
        break;

    case 17:
        if(dtofdvdttime>999.99)
            dtofdvdttime=999.99;
        else if(dtofdvdttime<0)
            dtofdvdttime=0.1;
        break;



    }

}


Widget::~Widget()
{
    delete ui;
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

void Widget::on_pushButton_2_clicked()//save
{
    if(save_f==0)
        return;
    save_f=0;
    if(current_language==1)
    {
              FILE *fp;
              char strbuf[80];
              int day,mon,year;
              int hour,minute,second;
              fp=fopen("report.txt","a+");
              if(fp==NULL)
              return;
              fprintf(fp,"\r\n");
              day=QDate::currentDate().day();
              mon=QDate::currentDate().month();
              year=QDate::currentDate().year();
              hour=QTime::currentTime().hour();
              minute=QTime::currentTime().minute();
              second=QTime::currentTime().second();

              fprintf(fp,"%d年%d月%d日 %d时%d分%d秒\r\n",year,mon,day,hour,minute,second);
              strcpy(strbuf,"低压减载");
              fprintf(fp,"%s\r\n",strbuf);


      if(TestItem1==1)
        {
          strcpy(strbuf,"动作值(V):");
          fprintf(fp,"%s",strbuf);
          if(TripTime1==-1)
            strcpy(strbuf,"动作值(V):未动作");
          else
            gcvt(TripValue ,7,strbuf);
          fprintf(fp,"%s\r\n",strbuf);
        }

      if(TestItem2==1)
        {
          strcpy(strbuf,"返回值(V):");
          fprintf(fp,"%s",strbuf);
          if(TripTime2==-1)
            strcpy(strbuf,"返回值(V):未动作");
          else
            gcvt(ReturnValue ,7,strbuf);
          fprintf(fp,"%s\r\n",strbuf);
        }

      if(TestItem3==1)
        {
          strcpy(strbuf,"固定电压动作时间(s):");
          fprintf(fp,"%s",strbuf);
          if(TripTime3==-1)
            strcpy(strbuf,"固定电压动作时间(s):未动作");
          else
            gcvt(TripTime3 ,7,strbuf);
          fprintf(fp,"%s\r\n",strbuf);

        }

      if(TestItem2==1&&TestItem1==1&&TripTime1!=-1&&TripTime2!=-1)
        {
          float ReturnQuotiety,Temp;
          strcpy(strbuf,"返回系数:");
          fprintf(fp,"%s",strbuf);
          ReturnQuotiety=ReturnValue/TripValue;
          Temp=ReturnQuotiety*10000;
          Temp=ceil(Temp);
          ReturnQuotiety=Temp/10000;
          gcvt(ReturnQuotiety,7,strbuf);
          fprintf(fp," %s\r\n",strbuf);
        }

      if(TestItem4==1)
      {
        strcpy(strbuf,"滑差闭锁值(V/s):");
        fprintf(fp,"%s",strbuf);

        if(TripTime4==-1)
          strcpy(strbuf,"滑差闭锁值(V/s):未动作");
        else
         sprintf(strbuf,"%f",dvdttripvalue);
        fprintf(fp,"%s\r\n",strbuf);
      }
      if(TestItem5==1)
      {

          if(TripTime5==-1)
            strcpy(strbuf,"动作时间(s):未动作");
          else
            sprintf(strbuf,"动作时间(s):%f",TripTime5);
            fprintf(fp," %s\r\n",strbuf);
      }

      fclose(fp);
      system("sync");
    }
    else if(current_language==2)
    {
        FILE *fp;
                    char strbuf[80];
                    int day,mon,year;
                    int hour,minute,second;
                    fp=fopen("report.txt","a+");
                    if(fp==NULL)
                    return;
                    fprintf(fp,"\r\n");
                    day=QDate::currentDate().day();
                    mon=QDate::currentDate().month();
                    year=QDate::currentDate().year();
                    fprintf(fp,"Date:%d.%d.%d\n",mon,day,year);

                    hour=QTime::currentTime().hour();
                    minute=QTime::currentTime().minute();
                    second=QTime::currentTime().second();
                    fprintf(fp,"Time:%d.%d.%d\r\n",hour,minute,second);
                    strcpy(strbuf,"Voltage test result");
                    fprintf(fp,"%s\r\n",strbuf);


            if(TestItem1==1)
              {
                strcpy(strbuf,"Trip Value(V):");
                fprintf(fp,"%s",strbuf);
                if(TripTime1==-1)
                  strcpy(strbuf,"Trip Value(V):No Trip");
                else
                  gcvt(TripValue ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }

            if(TestItem2==1)
              {
                strcpy(strbuf,"Drop Off Value(V):");
                fprintf(fp,"%s",strbuf);
                if(TripTime2==-1)
                  strcpy(strbuf,"Drop Off Value(V):No Trip");
                else
                  gcvt(ReturnValue ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }

            if(TestItem3==1)
              {
                strcpy(strbuf,"Fixed Voltage(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTime3==-1)
                  strcpy(strbuf,"Fixed Voltage(s):No Trip");
                else
                  gcvt(TripTime3 ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

              }

            if(TestItem2==1&&TestItem1==1&&TripTime1!=-1&&TripTime2!=-1)
              {
                float ReturnQuotiety,Temp;
                strcpy(strbuf,"drop off ratio:");
                fprintf(fp,"%s",strbuf);
                ReturnQuotiety=ReturnValue/TripValue;
                Temp=ReturnQuotiety*10000;
                Temp=ceil(Temp);
                ReturnQuotiety=Temp/10000;
                gcvt(ReturnQuotiety,7,strbuf);
                fprintf(fp," %s\r\n",strbuf);
              }

            if(TestItem4==1)
            {
              strcpy(strbuf,"dv/dt Blocking value(V/s):");
              fprintf(fp,"%s",strbuf);

              if(TripTime4==-1)
                strcpy(strbuf,"dv/dt Blocking value(V/s):No Trip");
              else
               sprintf(strbuf,"%f",dvdttripvalue);
              fprintf(fp,"%s\r\n",strbuf);
            }
            if(TestItem5==1)
            {

                if(TripTime5==-1)
                  strcpy(strbuf,"Trip time(s):No Trip");
                else
                  sprintf(strbuf,"Trip time(s):%f",TripTime5);
                  fprintf(fp," %s\r\n",strbuf);
            }

            fclose(fp);
            system("sync");
    }
   /*
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
    fprintf(fp,"Date:%d.%d.%d\n",mon,day,year);

    hour=QTime::currentTime().hour();
    minute=QTime::currentTime().minute();
    second=QTime::currentTime().second();
    fprintf(fp,"Time:%d.%d.%d\n",hour,minute,second);
    strcpy(strbuf,"Voltage test result");
    fprintf(fp,"%s\r\n",strbuf);



    if(TestItem1==1)
      {
        strcpy(strbuf,"pick up(V):");
        fprintf(fp,"%s",strbuf);
        if(TripTime1==-1)
          strcpy(strbuf,"No Trip");
        else
          gcvt(TripValue ,7,strbuf);
        fprintf(fp,"%s\r\n",strbuf);
      }

    if(TestItem2==1)
      {
        strcpy(strbuf,"drop off(V):");
        fprintf(fp,"%s",strbuf);
        if(TripTime2==-1)
          strcpy(strbuf,"No Trip");
        else
          gcvt(ReturnValue ,7,strbuf);
        fprintf(fp,"%s\r\n",strbuf);
      }

    if(TestItem3==1)
      {
        strcpy(strbuf,"Trip time(s):");
        fprintf(fp,"%s",strbuf);
        if(TripTime3==-1)
          strcpy(strbuf,"No Trip");
        else
          gcvt(TripTime3 ,7,strbuf);
        fprintf(fp,"%s\r\n",strbuf);

      }

    if(TestItem2==1&&TestItem1==1&&TripTime1!=-1&&TripTime2!=-1)
      {
        float ReturnQuotiety,Temp;
        strcpy(strbuf,"drop off ratio:");
        fprintf(fp,"%s",strbuf);
        ReturnQuotiety=ReturnValue/TripValue;
        Temp=ReturnQuotiety*10000;
        Temp=ceil(Temp);
        ReturnQuotiety=Temp/10000;
        gcvt(ReturnQuotiety,7,strbuf);
        fprintf(fp,"%s\r\n",strbuf);
      }

    if(TestItem4==1)
    {
      strcpy(strbuf,"dv/dt trip(V/s):");
      fprintf(fp,"%s",strbuf);

      if(TripTime4==-1)
        strcpy(strbuf,"No Trip");
      else
       sprintf(strbuf,"%f",dvdttripvalue);
      fprintf(fp,"%s\r\n",strbuf);
    }
    if(TestItem5==1)
    {

        if(TripTime5==-1)
          strcpy(strbuf,"dv/dt trip time(s):No Trip");
        else
          sprintf(strbuf,"dv/dt trip time(s):%f",TripTime5);
          fprintf(fp,"%s\r\n",strbuf);
    }

    fclose(fp);
     */
}

void Widget::on_pushButton_clicked()
{
    if(starttest==1)
        return;

Helpdialog *helpdialog=new Helpdialog;
temprorarydata->helpopenflag=true;
connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
if(current_language==2)
helpdialog->SetHelpfilename("Voltage.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("Voltage1.txt");
//helpdialog->SetHelpfilename("Voltage.txt");
helpdialog->show();
}
void Widget::HelpDialogClose()
{
temprorarydata->helpopenflag=false;

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
        dvdtStopVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(dvdtStopVoltage,'g',6));
        break;
    case 2:
       dvdtstart=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(dvdtstart,'g',6));
        break;
    case 3:
        dvdtend=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(dvdtend,'g',6));
        break;
    case 4:
        dvdtStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(dvdtStep,'g',6));
        break;

    case 5:
        StopVoltage_triptime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StopVoltage_triptime,'g',6));
        if(Variable>=1&&Variable<=3)
       {
        holdtime_triptime=fabs(SystemVolt-StopVoltage_triptime)/dvdt_triptime;
        ui->label_9->setText(QString::number(fabs(SystemVolt-StopVoltage_triptime)/dvdt_triptime,'g',6));
       }
        else
        {
            holdtime_triptime=fabs(SystemVolt*1.732-StopVoltage_triptime)/dvdt_triptime;
            ui->label_9->setText(QString::number(fabs(SystemVolt*1.732-StopVoltage_triptime)/dvdt_triptime,'g',6));
        }

        break;
    case 6:
        dvdt_triptime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(dvdt_triptime,'g',6));
        if(Variable>=1&&Variable<=3)
       {
        holdtime_triptime=fabs(SystemVolt-StopVoltage_triptime)/dvdt_triptime;
        ui->label_9->setText(QString::number(fabs(SystemVolt-StopVoltage_triptime)/dvdt_triptime,'g',6));
       }
        else
        {
            holdtime_triptime=fabs(SystemVolt*1.732-StopVoltage_triptime)/dvdt_triptime;
            ui->label_9->setText(QString::number(fabs(SystemVolt*1.732-StopVoltage_triptime)/dvdt_triptime,'g',6));
        }
        break;
    case 7:
        prefaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(prefaulttime,'g',6));
        break;
    case 8:
        postfaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(postfaulttime,'g',6));
        break;
    case 9:
        faultduration=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(faultduration,'g',6));
        break;

    case 10:
        VStart=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VStart,'g',6));
        break;
    case 11:
        VStop=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VStop,'g',6));
        break;
    case 12:
        VStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VStep,'g',6));
        break;
    case 13:
        VSteptime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VSteptime,'g',6));
        break;
    case 14:
        StopVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StopVoltage,'g',6));
        break;
    case 15:
        StopVoltageHoldTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StopVoltageHoldTime,'g',6));
        break;

    case 16:
        dtofdvdt=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(dtofdvdt,'g',6));
        break;
    case 17:
        dtofdvdttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(dtofdvdttime,'g',6));
        break;




    }

}


void Widget::clearkeypadflag()
{
 temprorarydata->keypadopenflag=false;
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
        else if(obj==ui->lineEdit_11)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_11;
              currenteditpos=11;
         }
        else if(obj==ui->lineEdit_12)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_12;
              currenteditpos=12;
         }
        else if(obj==ui->lineEdit_13)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_13;
              currenteditpos=13;
         }
        else if(obj==ui->lineEdit_14)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_14;
              currenteditpos=14;
         }
        else if(obj==ui->lineEdit_15)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_15;
              currenteditpos=15;
         }

        else if(obj==ui->lineEdit_16)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_16;
              currenteditpos=16;
         }
        else if(obj==ui->lineEdit_17)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_17;
              currenteditpos=17;
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



void Widget::on_radioButton_clicked()
{
  Variable=1;
  ShowRadio(Variable);

}

void Widget::on_radioButton_2_clicked()
{
 Variable=2;
 ShowRadio(Variable);
}

void Widget::on_radioButton_3_clicked()
{
 Variable=3;
 ShowRadio(Variable);
}

void Widget::on_radioButton_4_clicked()
{
Variable=4;
ShowRadio(Variable);
}

void Widget::on_radioButton_5_clicked()
{
Variable=5;
ShowRadio(Variable);
}

void Widget::on_radioButton_6_clicked()
{
Variable=6;
ShowRadio(Variable);
}


void Widget::on_radioButton_7_clicked()
{
Variable=7;
ShowRadio(Variable);
}



void Widget::on_tabWidget_currentChanged(int index)
{
    TestItem1=0;
    TestItem3=0;
    TestItem4=0;
    TestItem5=0;



    switch(index)
    {
    case 0:
         TestItem1=1;
         break;

    case 1:
         TestItem3=1;
         break;

    case 2:
        TestItem4=1;

        break;

    case 3:
        TestItem5=1;

        break;
    }

}





void Widget::on_checkBox_clicked()
{
if(TestItem2==1)
    TestItem2=0;
else
    TestItem2=1;

ui->checkBox->setChecked(TestItem2);

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
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton->setEnabled(false);
            ui->pushButton_4->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
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
