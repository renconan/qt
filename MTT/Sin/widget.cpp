#include "widget.h"
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
#include <unistd.h>
#include "qtranslator.h"
QTranslator *tor;
int current_language=2;  //1中文  2英文

extern TEMPRORARYDATA *temprorarydata;
float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
unsigned char result[70],receive_data[70];
double out_time,change_timedata;
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;

float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;



long auto_i=0,IRQcnt=0,auto_count=0,tice=0;


int   input_par=0,Decimal=0,sige_flag=0,HelpFlag=0;

int   INPAR=0,seq_count,SeqStep=0;
int   testcount=0,time_relay_testcount=0,auto_end=0;
int   cx,cy,cx_start,cx_mid,cx_end,starttest=0,low,row,page;
int   V1DC=0,V2DC=0,V3DC=0,VzDC=0;
int I1DC=0;
int I2DC=0;
int I3DC=0;
int   TestItem1,TestItem2,TestItem3,OutputMode,TestObject;
float FixedUnderTime,FixedOverTime;


float TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime1,TripTime2,TripTime3;
float Phase,LogicResolution,SystemFrec;
float ChangeValue,timeout;

int exit_par,save_f;
int sys[4];

char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
float VzOutput3Uo=1,VzOutputAc=57.00,VzOutputDc=220;
float SystemVolt=0;


float StartValue,EndValue,ActStep,ReturnStep,StepTime;

float faultduration=5; //2014-11-13 du kai
float prefaulttime=0;

float StopValue=30;
int   TripTimeType=2;
float StopValueHoldTime=2;
int   DCRelayType,ACRelayType=1/*voltage relay*/;

TMPPARA  Volpara,Curpara;



bool SoftStartEndflag=false;

bool Threadrunstate=true;


float Postfaulttime=3;
int   dcouttype=1;

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

#ifdef ARMLINUX
  this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
#endif
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",13);
    setFont(font);
    init_par();
    prefaulttime=0;
    currenteditpos=0;
    setFixedSize(800,480);
    ui->label_13->setHidden(true);//yinchang
    ui->lineEdit_6->setHidden(true);//yinchang
    showFullScreen();

    QPixmap img(":/picture/hei.png");
    ui->label_7->setPixmap(img);
    ui->label_8->setPixmap(img);
    ui->label_10->setPixmap(img);
    ui->label_11->setPixmap(img);

    ui->tabWidget->setCurrentIndex(0);
    TestItem1=1;
    TestItem2=1;
    TestItem3=0;
    ui->checkBox->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");

    //ui->label_14->setText(("<font color=blue>"+QString("交流继电器(2.2.1)")+"</font>"));


    if(ACRelayType==1)
    {
        ui->radioButton_7->setChecked(true);
        ui->radioButton_8->setChecked(false);
    }
    else
    {
        ui->radioButton_7->setChecked(false);
        ui->radioButton_8->setChecked(true);
    }





    if(TestItem2==1)
        ui->checkBox->setChecked(true);
    else  ui->checkBox->setChecked(false);




    switch(OutputMode)
    {
    case 1:
          ui->radioButton->setChecked(true);
          ui->radioButton_2->setChecked(false);
          ui->radioButton_3->setChecked(false);
          ui->radioButton_4->setChecked(false);
          ui->radioButton_5->setChecked(false);
          ui->radioButton_6->setChecked(false);
          break;
    case 2:
          ui->radioButton->setChecked(false);
          ui->radioButton_2->setChecked(true);
          ui->radioButton_3->setChecked(false);
          ui->radioButton_4->setChecked(false);
          ui->radioButton_5->setChecked(false);
          ui->radioButton_6->setChecked(false);
          break;
    case 3:
          ui->radioButton->setChecked(false);
          ui->radioButton_2->setChecked(false);
          ui->radioButton_3->setChecked(true);
          ui->radioButton_4->setChecked(false);
          ui->radioButton_5->setChecked(false);
          ui->radioButton_6->setChecked(false);
          break;
    case 4:
          ui->radioButton->setChecked(false);
          ui->radioButton_2->setChecked(false);
          ui->radioButton_3->setChecked(false);
          ui->radioButton_4->setChecked(true);
          ui->radioButton_5->setChecked(false);
          ui->radioButton_6->setChecked(false);
          break;
    case 5:
          ui->radioButton->setChecked(false);
          ui->radioButton_2->setChecked(false);
          ui->radioButton_3->setChecked(false);
          ui->radioButton_4->setChecked(false);
          ui->radioButton_5->setChecked(true);
          ui->radioButton_6->setChecked(false);
          break;
    case 6:
          ui->radioButton->setChecked(false);
          ui->radioButton_2->setChecked(false);
          ui->radioButton_3->setChecked(false);
          ui->radioButton_4->setChecked(false);
          ui->radioButton_5->setChecked(false);
          ui->radioButton_6->setChecked(true);
          break;
    }


    ui->lineEdit->setText(QString::number(StartValue));
    ui->lineEdit_2->setText(QString::number(EndValue));
    ui->lineEdit_3->setText(QString::number(ActStep));
    ui->lineEdit_4->setText(QString::number(StepTime));
    ui->lineEdit_5->setText(QString::number(faultduration));
    ui->lineEdit_6->setText(QString::number(prefaulttime));
    ui->lineEdit_7->setText(QString::number(StopValue));
    ui->lineEdit_8->setText(QString::number(StopValueHoldTime));




    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
    ui->lineEdit_5->installEventFilter(this);
    ui->lineEdit_6->installEventFilter(this);
    ui->lineEdit_7->installEventFilter(this);
    ui->lineEdit_8->installEventFilter(this);

    ui->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_5->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_8->setContextMenuPolicy(Qt::NoContextMenu);


    ui->label_6->clear();



    //ui->pushButton->setFocusPolicy(Qt::NoFocus);
     //ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
      //ui->pushButton_3->setFocusPolicy(Qt::NoFocus);

      //ui->pushButton_4->setFocusPolicy(Qt::NoFocus);



    uartthread =new thread1;
    connect(uartthread,SIGNAL(SIG_binarystate(int)),this,SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_testresult(int)),SLOT(showtestresult(int)));
    connect(uartthread,SIGNAL(SIG_DisplayChangeValue(bool,float)),SLOT(DisplayChangeValue(bool,float)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),SLOT(showdspstate(int)));
 connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
 connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));
    connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));

    uartthread->start();
}

void Widget::updateUI()
{
    ui->label_14->setText(("<font color=blue>"+QString(tr("交流继电器(2.2.1)"))+"</font>"));
    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_2->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_4->setText(tr("测试"));
        ui->label->setText(tr("动作保持时间(s)："));
        ui->label_5->setText(tr("步长时间(S)："));
        ui->label_15->setText(tr("最大输出时间(s):"));
        ui->radioButton_7->setText(tr("电压继电器"));
        ui->radioButton_8->setText(tr("电流继电器"));
        ui->groupBox->setTitle(tr("输出选择"));
        ui->groupBox_2->setTitle(tr("继电器类型"));
        ui->groupBox_5->setTitle(tr("测试结果"));
        ui->checkBox->setText(tr("返回值"));
        ui->tabWidget->setTabText(0,tr("动作值"));
        ui->tabWidget->setTabText(1,tr("动作时间"));

      if(ACRelayType==1)
      {
        ui->label_2->setText(tr("起始值(V)："));
        ui->label_3->setText(tr("终止值(V)："));
        ui->label_4->setText(tr("步长(V)："));
        ui->label_12->setText(tr("测试电压(V):"));
        ui->radioButton->setText(tr("VA"));
        ui->radioButton_2->setText(tr("VB"));
        ui->radioButton_3->setText(tr("VC"));
        ui->radioButton_4->setText(tr("VAB串联"));
        ui->radioButton_5->setText(tr("VBC串联"));
        ui->radioButton_6->setText(tr("VCA串联"));
      }
      else
      {
        ui->label_2->setText(tr("起始值(A):"));
        ui->label_3->setText(tr("终止值(A):"));
        ui->label_4->setText(tr("步长(A):"));
        ui->label_12->setText(tr("测试电流(A):"));
        ui->radioButton->setText(tr("IA"));
        ui->radioButton_2->setText(tr("IB"));
        ui->radioButton_3->setText(tr("IC"));
        ui->radioButton_4->setText(tr("IA、B、C并联"));
      }
}

void Widget::init_par()
{
       FILE *fp;
        OutputMode=1;
        TestObject=1;


        TestItem1=1;
        TestItem2=0;
        TestItem3=0;
        StartValue=0;            //RatedVoltage;
        EndValue=125;             //0.4*RatedVoltage;//FixedUnderVoltage;
        ActStep=0.5;
        StepTime=0.1;

        Volpara.StartValue=StartValue;
        Volpara.EndValue=EndValue;
        Volpara.Step=ActStep;
        Volpara.Steptime=StepTime;
        Volpara.StopValue=StopValue;
        Volpara.StopValueHoldtime=StopValueHoldTime;

        Curpara.StartValue=0;
        Curpara.EndValue=5;
        Curpara.Step=0.1;
        Curpara.Steptime=0.1;
        Curpara.StopValue=5;
        Curpara.StopValueHoldtime=2;

        V1DC=0;
        V2DC=0;
        V3DC=0;
        VzDC=0;
        I1DC=0;
        I2DC=0;
        I3DC=0;



       if((fp=fopen("sysset.txt","a+"))==NULL)
          return;
       fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&prefaulttime,&Postfaulttime,&dcouttype);
       fclose(fp);
       exit_par=0;
}


void Widget::showdspstate(int state)
{

    switch(state)
       {
    case 0: ui->label_9->setText(tr("运行正常")); break;
    case 1: ui->label_9->setText(tr("IA 过载")); break;
    case 2: ui->label_9->setText(tr("IABC 温度保护")); break;
    case 3: ui->label_9->setText(tr("IC 间歇")); break;
    case 4: ui->label_9->setText(tr("IB 过载")); break;
    case 5: ui->label_9->setText(tr("IC 闭锁")); break;
    case 6: ui->label_9->setText(tr("IA 间歇")); break;
    case 7: ui->label_9->setText(tr("IC 过载")); break;
    case 8: ui->label_9->setText(tr("IABC 电源保护")); break;
    case 9: ui->label_9->setText(tr("IB 间歇")); break;
    case 10: ui->label_9->setText(tr("VA 过载")); break;

    case 12: ui->label_9->setText(tr("VC 间歇")); break;
    case 13: ui->label_9->setText(tr("VB 过载")); break;
    case 14: ui->label_9->setText(tr("VABC 温度保护")); break;
    case 15: ui->label_9->setText(tr("VA 间歇")); break;
    case 16: ui->label_9->setText(tr("VC 过载")); break;
    case 17: ui->label_9->setText(tr("VABC 电源保护")); break;
    case 18: ui->label_9->setText(tr("Vz 过载")); break;
    case 19: ui->label_9->setText(tr("直流过载")); break;
    case 20: ui->label_9->setText(tr("IABC 电源保护")); break;
    case 21: ui->label_9->setText(tr("VC 间歇")); break;
       }


    QPixmap img(":/picture/hei.png");
    QPixmap pic(":/picture/hong.png");
    switch(state)
    {
    case 100:
        ui->label_7->setPixmap(img);
        ui->label_8->setPixmap(img);
        ui->label_10->setPixmap(img);
        ui->label_11->setPixmap(img);
        break;
    case 101: ui->label_7->setPixmap(pic); break;
    case 102: ui->label_8->setPixmap(pic); break;
    case 103: ui->label_10->setPixmap(pic); break;
    case 104: ui->label_11->setPixmap(pic); break;
    case 111: ui->label_7->setPixmap(img); break;
    case 112: ui->label_8->setPixmap(img); break;
    case 113: ui->label_10->setPixmap(img); break;
    case 114: ui->label_11->setPixmap(img); break;
    }


}





void Widget::vi_parameter_enter(int position)
{

    float Maxfactor=1;
    if(ACRelayType==1)
   {

       if(OutputMode>=1&&OutputMode<=3)
       Maxfactor=1;
       else   if(OutputMode>=4&&OutputMode<=6)
       Maxfactor=2;

   }
    else
    {
        if(OutputMode>=1&&OutputMode<=3)
        Maxfactor=1;
        else   if(OutputMode==4)
        Maxfactor=3;


    }


    switch(position)
    {
    case 1://起始值
        if(ACRelayType==1)
       { if(StartValue>MAX_V_VALUEDATA_AC*Maxfactor)
        StartValue=MAX_V_VALUEDATA_AC*Maxfactor;
        else if(StartValue<0)
        StartValue=0;
       }
       else
        {
            if(StartValue>MAX_I_VALUEDATA_AC*Maxfactor)
              StartValue=MAX_I_VALUEDATA_AC*Maxfactor;
            else if(StartValue<0)
              StartValue=0;
        }
        break;
    case 2://终止值
        if(ACRelayType==1)
       { if(EndValue>MAX_V_VALUEDATA_AC*Maxfactor)
           EndValue=MAX_V_VALUEDATA_AC*Maxfactor;
          else if(EndValue<0)
           EndValue=0;
       }
       else
        {
            if(EndValue>MAX_I_VALUEDATA_AC*Maxfactor)
                EndValue=MAX_I_VALUEDATA_AC*Maxfactor;
              else if(EndValue<0)
                EndValue=0;
        }
          break;
    case 3://步长
        if(ActStep>10)
            ActStep=10;
        else if(ActStep<0)
            ActStep=0.1;//
        break;
    case 4://步长time
        if(StepTime>999.99)
            StepTime=999.99;
        else if(StepTime<0)
            StepTime=0.1;//
        break;
    case 5://holdtime
        if(faultduration>999.99)
            faultduration=999.99;
        else if(faultduration<0)
            faultduration=0.1;//
        break;
    case 6://故障前时间
        if(prefaulttime>999.99)
            prefaulttime=999.99;
        else if(prefaulttime<0)
            prefaulttime=0.1;//
        break;
    case 7:
        if(ACRelayType==1)
       { if(StopValue>MAX_V_VALUEDATA_AC*Maxfactor)
        StopValue=MAX_V_VALUEDATA_AC*Maxfactor;
        else if(StopValue<0)
        StopValue=0;
       }
       else
        {
            if(StopValue>MAX_I_VALUEDATA_AC*Maxfactor)
               StopValue=MAX_I_VALUEDATA_AC*Maxfactor;
              else if(StopValue<0)
            StopValue=0;
        }

        break;
    case 8://
        if(StopValueHoldTime>999.99)
            StopValueHoldTime=999.99;
        else if(StopValueHoldTime<0)
            StopValueHoldTime=0.1;//
        break;

}




}


void Widget::DisplayChangeValue(bool flag, float value)
{
    if(flag)
    {
        ui->label_6->setText(QString::number(value,'g',4));
    }
    else ui->label_6->clear();
}



void Widget::showtestresult(int num)
{

    float ReturnQuotiety,Temp;
    char danwei[10];
    if(ACRelayType==1)
    strcpy(danwei,"V");
    else
    strcpy(danwei,"A");
    if(current_language==1)
    {
      switch(num)
   {

 case 0:   ui->textBrowser->clear();
           break;
 case 1:
         ui->textBrowser->clear();
         if(TripTime1==-1)
         ui->textBrowser->append(tr("动作值(%1):未动作").arg(danwei));
         else
             ui->textBrowser->append(tr("动作值(%1):").arg(danwei)+QString::number(TripValue,'g',4));
        break;

 case 2:
         if(TripTime2==-1)
          ui->textBrowser->append(tr("返回值(%1):未动作").arg(danwei));
         else
             ui->textBrowser->append(tr("返回值(%1):").arg(danwei)+QString::number(ReturnValue,'g',4));
         if((TripTime1!=-1&&TripTime2!=-1)&&TestItem3==0)
            {
                  ReturnQuotiety=ReturnValue/TripValue;
                  Temp=ReturnQuotiety*1000;
                  Temp=ceil(Temp);
                  ReturnQuotiety=Temp/1000;
                  ui->textBrowser->append(tr("返回系数:")+QString::number(ReturnQuotiety,'g',4));
            }

         break;

  case 3:
         ui->textBrowser->clear();
         if(TripTime3!=-1)
         ui->textBrowser->setText(tr("动作时间(s):")+QString::number(TripTime3,'g',5));
         else
         ui->textBrowser->setText(tr("动作时间(s):未动作"));
     break;
     }
    }
    else if(current_language==2)
    {
        switch(num)
           {

         case 0:   ui->textBrowser->clear();
                   break;
         case 1:
                 ui->textBrowser->clear();
                 if(TripTime1==-1)
                 ui->textBrowser->append(tr("pick up(%1):no trip").arg(danwei));
                 else
                     ui->textBrowser->append(tr("pick up(%1):").arg(danwei)+QString::number(TripValue,'g',4));
                break;

         case 2:
                 if(TripTime2==-1)
                  ui->textBrowser->append(tr("drop off(%1):no trip").arg(danwei));
                 else
                     ui->textBrowser->append(tr("drop off(%1):").arg(danwei)+QString::number(ReturnValue,'g',4));
                 if((TripTime1!=-1&&TripTime2!=-1)&&TestItem3==0)
                    {
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
             }

    }
}



Widget::~Widget()
{
    delete ui;
}

void Widget::on_radioButton_clicked()
{

   OutputMode=1;
   if(ACRelayType==1)//dianya jidianqi
   {
       //起始值
       if(StartValue>MAX_V_VALUEDATA_AC)
       {
           StartValue=MAX_V_VALUEDATA_AC;
           ui->lineEdit->setText(QString::number(StartValue));
       }
       else if(StartValue<MIN_V_VALUEDATA_AC)
       {
           StartValue=MIN_V_VALUEDATA_AC;
           ui->lineEdit->setText(QString::number(StartValue));
       }
       //终止值
       if(EndValue>MAX_V_VALUEDATA_AC)
       {
           EndValue=MAX_V_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(EndValue));
       }
       else if(EndValue<MIN_V_VALUEDATA_AC)
       {
           EndValue=MIN_V_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(EndValue));
       }
   }
   else if(ACRelayType==2)//dianliu jidianqi
   {
       //起始值
       if(StartValue>MAX_I_VALUEDATA_AC)
       {
           StartValue=MAX_I_VALUEDATA_AC;
           ui->lineEdit->setText(QString::number(StartValue));
       }
       else if(StartValue<MIN_I_VALUEDATA_AC)
       {
           StartValue=MIN_I_VALUEDATA_AC;
           ui->lineEdit->setText(QString::number(StartValue));
       }
       //终止值
       if(EndValue>MAX_I_VALUEDATA_AC)
       {
           EndValue=MAX_I_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(EndValue));
       }
       else if(EndValue<MIN_I_VALUEDATA_AC)
       {
           EndValue=MIN_I_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(EndValue));
       }
   }

   switch(OutputMode)
   {
   case 1:
         ui->radioButton->setChecked(true);
         ui->radioButton_2->setChecked(false);
         ui->radioButton_3->setChecked(false);
         ui->radioButton_4->setChecked(false);
         ui->radioButton_5->setChecked(false);
         ui->radioButton_6->setChecked(false);
         break;
   case 2:
         ui->radioButton->setChecked(false);
         ui->radioButton_2->setChecked(true);
         ui->radioButton_3->setChecked(false);
         ui->radioButton_4->setChecked(false);
         ui->radioButton_5->setChecked(false);
         ui->radioButton_6->setChecked(false);
         break;
   case 3:
         ui->radioButton->setChecked(false);
         ui->radioButton_2->setChecked(false);
         ui->radioButton_3->setChecked(true);
         ui->radioButton_4->setChecked(false);
         ui->radioButton_5->setChecked(false);
         ui->radioButton_6->setChecked(false);
         break;
   case 4:
         ui->radioButton->setChecked(false);
         ui->radioButton_2->setChecked(false);
         ui->radioButton_3->setChecked(false);
         ui->radioButton_4->setChecked(true);
         ui->radioButton_5->setChecked(false);
         ui->radioButton_6->setChecked(false);
         break;
   case 5:
         ui->radioButton->setChecked(false);
         ui->radioButton_2->setChecked(false);
         ui->radioButton_3->setChecked(false);
         ui->radioButton_4->setChecked(false);
         ui->radioButton_5->setChecked(true);
         ui->radioButton_6->setChecked(false);
         break;
   case 6:
         ui->radioButton->setChecked(false);
         ui->radioButton_2->setChecked(false);
         ui->radioButton_3->setChecked(false);
         ui->radioButton_4->setChecked(false);
         ui->radioButton_5->setChecked(false);
         ui->radioButton_6->setChecked(true);
         break;
   }

}

void Widget::on_radioButton_2_clicked()
{
  OutputMode=2;
  if(ACRelayType==1)//dianya jidianqi
  {
      //起始值
      if(StartValue>MAX_V_VALUEDATA_AC)
      {
          StartValue=MAX_V_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(StartValue));
      }
      else if(StartValue<MIN_V_VALUEDATA_AC)
      {
          StartValue=MIN_V_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(StartValue));
      }
      //终止值
      if(EndValue>MAX_V_VALUEDATA_AC)
      {
          EndValue=MAX_V_VALUEDATA_AC;
          ui->lineEdit_2->setText(QString::number(EndValue));
      }
      else if(EndValue<MIN_V_VALUEDATA_AC)
      {
          EndValue=MIN_V_VALUEDATA_AC;
          ui->lineEdit_2->setText(QString::number(EndValue));
      }
  }
  else if(ACRelayType==2)//dianliu jidianqi
  {
      //起始值
      if(StartValue>MAX_I_VALUEDATA_AC)
      {
          StartValue=MAX_I_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(StartValue));
      }
      else if(StartValue<MIN_I_VALUEDATA_AC)
      {
          StartValue=MIN_I_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(StartValue));
      }
      //终止值
      if(EndValue>MAX_I_VALUEDATA_AC)
      {
          EndValue=MAX_I_VALUEDATA_AC;
          ui->lineEdit_2->setText(QString::number(EndValue));
      }
      else if(EndValue<MIN_I_VALUEDATA_AC)
      {
          EndValue=MIN_I_VALUEDATA_AC;
          ui->lineEdit_2->setText(QString::number(EndValue));
      }
  }

  switch(OutputMode)
  {
  case 1:
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(false);
        break;
  case 2:
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(true);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(false);
        break;
  case 3:
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(true);
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(false);
        break;
  case 4:
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(true);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(false);
        break;
  case 5:
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(true);
        ui->radioButton_6->setChecked(false);
        break;
  case 6:
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(true);
        break;
  }

}

void Widget::on_radioButton_3_clicked()
{
OutputMode=3;
if(ACRelayType==1)//dianya jidianqi
{
    //起始值
    if(StartValue>MAX_V_VALUEDATA_AC)
    {
        StartValue=MAX_V_VALUEDATA_AC;
        ui->lineEdit->setText(QString::number(StartValue));
    }
    else if(StartValue<MIN_V_VALUEDATA_AC)
    {
        StartValue=MIN_V_VALUEDATA_AC;
        ui->lineEdit->setText(QString::number(StartValue));
    }
    //终止值
    if(EndValue>MAX_V_VALUEDATA_AC)
    {
        EndValue=MAX_V_VALUEDATA_AC;
        ui->lineEdit_2->setText(QString::number(EndValue));
    }
    else if(EndValue<MIN_V_VALUEDATA_AC)
    {
        EndValue=MIN_V_VALUEDATA_AC;
        ui->lineEdit_2->setText(QString::number(EndValue));
    }
}
else if(ACRelayType==2)//dianliu jidianqi
{
    //起始值
    if(StartValue>MAX_I_VALUEDATA_AC)
    {
        StartValue=MAX_I_VALUEDATA_AC;
        ui->lineEdit->setText(QString::number(StartValue));
    }
    else if(StartValue<MIN_I_VALUEDATA_AC)
    {
        StartValue=MIN_I_VALUEDATA_AC;
        ui->lineEdit->setText(QString::number(StartValue));
    }
    //终止值
    if(EndValue>MAX_I_VALUEDATA_AC)
    {
        EndValue=MAX_I_VALUEDATA_AC;
        ui->lineEdit_2->setText(QString::number(EndValue));
    }
    else if(EndValue<MIN_I_VALUEDATA_AC)
    {
        EndValue=MIN_I_VALUEDATA_AC;
        ui->lineEdit_2->setText(QString::number(EndValue));
    }
}
switch(OutputMode)
{
case 1:
      ui->radioButton->setChecked(true);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 2:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(true);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 3:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(true);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 4:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(true);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 5:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(true);
      ui->radioButton_6->setChecked(false);
      break;
case 6:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(true);
      break;
}

}

void Widget::on_radioButton_4_clicked()
{
 OutputMode=4;

 switch(OutputMode)
 {
 case 1:
       ui->radioButton->setChecked(true);
       ui->radioButton_2->setChecked(false);
       ui->radioButton_3->setChecked(false);
       ui->radioButton_4->setChecked(false);
       ui->radioButton_5->setChecked(false);
       ui->radioButton_6->setChecked(false);
       break;
 case 2:
       ui->radioButton->setChecked(false);
       ui->radioButton_2->setChecked(true);
       ui->radioButton_3->setChecked(false);
       ui->radioButton_4->setChecked(false);
       ui->radioButton_5->setChecked(false);
       ui->radioButton_6->setChecked(false);
       break;
 case 3:
       ui->radioButton->setChecked(false);
       ui->radioButton_2->setChecked(false);
       ui->radioButton_3->setChecked(true);
       ui->radioButton_4->setChecked(false);
       ui->radioButton_5->setChecked(false);
       ui->radioButton_6->setChecked(false);
       break;
 case 4:
       ui->radioButton->setChecked(false);
       ui->radioButton_2->setChecked(false);
       ui->radioButton_3->setChecked(false);
       ui->radioButton_4->setChecked(true);
       ui->radioButton_5->setChecked(false);
       ui->radioButton_6->setChecked(false);
       break;
 case 5:
       ui->radioButton->setChecked(false);
       ui->radioButton_2->setChecked(false);
       ui->radioButton_3->setChecked(false);
       ui->radioButton_4->setChecked(false);
       ui->radioButton_5->setChecked(true);
       ui->radioButton_6->setChecked(false);
       break;
 case 6:
       ui->radioButton->setChecked(false);
       ui->radioButton_2->setChecked(false);
       ui->radioButton_3->setChecked(false);
       ui->radioButton_4->setChecked(false);
       ui->radioButton_5->setChecked(false);
       ui->radioButton_6->setChecked(true);
       break;
 }

}

void Widget::on_radioButton_5_clicked()
{
OutputMode=5;

switch(OutputMode)
{
case 1:
      ui->radioButton->setChecked(true);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 2:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(true);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 3:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(true);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 4:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(true);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 5:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(true);
      ui->radioButton_6->setChecked(false);
      break;
case 6:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(true);
      break;
}

}

void Widget::on_radioButton_6_clicked()
{
OutputMode=6;

switch(OutputMode)
{
case 1:
      ui->radioButton->setChecked(true);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 2:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(true);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 3:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(true);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 4:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(true);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(false);
      break;
case 5:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(true);
      ui->radioButton_6->setChecked(false);
      break;
case 6:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(false);
      ui->radioButton_4->setChecked(false);
      ui->radioButton_5->setChecked(false);
      ui->radioButton_6->setChecked(true);
      break;
}

}




/*
void Widget::on_lineEdit_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit;
    currenteditpos=1;
    temprorarydata->keypadopenflag=true;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();

    }
}

void Widget::on_lineEdit_2_selectionChanged()
{
if(temprorarydata->keypadopenflag==false)
 { currentedit=ui->lineEdit_2;
    temprorarydata->keypadopenflag=true;
  currenteditpos=2;
  pad=new keypads;
  pad->setModal(true);
  connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
  connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
  pad->show();
  pad->raise();
  pad->activateWindow();
}
}

void Widget::on_lineEdit_3_selectionChanged()
{
 if(temprorarydata->keypadopenflag==false)
 {
  currentedit=ui->lineEdit_3;
  temprorarydata->keypadopenflag=true;
  currenteditpos=3;
  pad=new keypads;
  pad->setModal(true);
  connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
  connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
  pad->show();
  pad->raise();
  pad->activateWindow();
 }
}

void Widget::on_lineEdit_4_selectionChanged()
{
if(temprorarydata->keypadopenflag==false)
{currentedit=ui->lineEdit_4;
        temprorarydata->keypadopenflag=true;
currenteditpos=4;
pad=new keypads;
pad->setModal(true);
connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
pad->show();
pad->raise();
pad->activateWindow();
    }
}

void Widget::on_lineEdit_5_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_5;
     temprorarydata->keypadopenflag=true;
    currenteditpos=5;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }

}

void Widget::on_lineEdit_6_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_6;
     temprorarydata->keypadopenflag=true;
    currenteditpos=6;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_7_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_7;
     temprorarydata->keypadopenflag=true;
    currenteditpos=7;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_8_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_8;
     temprorarydata->keypadopenflag=true;
    currenteditpos=8;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}


*/


void Widget::setlineedittext(QString str)
{
    bool ok;
    float number=str.toFloat(&ok);
    if(ok==false)
        return;
    switch(currenteditpos)
    {
    case 1:
        StartValue=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StartValue,'g',6));
        break;
    case 2:
        EndValue=number;
        vi_parameter_enter(currenteditpos);

        currentedit->setText(QString::number(EndValue,'g',6));
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
        prefaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(prefaulttime,'g',6));
        break;
    case 7:
        StopValue=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StopValue,'g',6));
        break;
    case 8:
        StopValueHoldTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StopValueHoldTime,'g',6));
        break;
    }

}

void Widget::clearkeypadflag()
{
    temprorarydata->keypadopenflag=false;
}

void Widget::on_pushButton_2_clicked() //save
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


               if(ACRelayType==1)
              { strcpy(strbuf,"交流电压继电器");
                fprintf(fp,"%s\r\n",strbuf);
              }
              else
               {
                 strcpy(strbuf,"交流电流继电器");
                 fprintf(fp,"%s\r\n",strbuf);
               }
              if(ACRelayType==1)
             {
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

               if(TestItem2==1&&TestItem3!=1)
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
                   strcpy(strbuf,"动作时间(s):");
                   fprintf(fp,"%s",strbuf);
                   if(TripTime3==-1)
                     strcpy(strbuf,"动作时间(s):未动作");
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

             }
              else
              {
                  if(TestItem1==1)
                    {
                      strcpy(strbuf,"动作值(A):");
                      fprintf(fp,"%s",strbuf);
                      if(TripTime1==-1)
                        strcpy(strbuf,"动作值(A):未动作");
                      else
                        gcvt(TripValue ,7,strbuf);
                      fprintf(fp,"%s\r\n",strbuf);
                    }

                  if(TestItem2==1&&TestItem3!=1)
                    {
                      strcpy(strbuf,"返回值(A):");
                      fprintf(fp,"%s",strbuf);
                      if(TripTime2==-1)
                        strcpy(strbuf,"返回值(A):未动作");
                      else
                        gcvt(ReturnValue ,7,strbuf);
                      fprintf(fp,"%s\r\n",strbuf);
                    }

                  if(TestItem3==1)
                    {
                      strcpy(strbuf,"动作时间(s):");
                      fprintf(fp,"%s",strbuf);
                      if(TripTime3==-1)
                        strcpy(strbuf,"动作时间(s):未动作");
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


                      if(ACRelayType==1)
                     { strcpy(strbuf,"Ac Voltage Relay test result");
                       fprintf(fp,"%s\r\n",strbuf);
                     }
                     else
                      {
                        strcpy(strbuf,"Ac Current Relay test result");
                        fprintf(fp,"%s\r\n",strbuf);
                      }
                     if(ACRelayType==1)
                    {
                      if(TestItem1==1)
                        {
                          strcpy(strbuf,"pick up(V):");
                          fprintf(fp,"%s",strbuf);
                          if(TripTime1==-1)
                            strcpy(strbuf,"pick up(V):no trip");
                          else
                            gcvt(TripValue ,7,strbuf);
                          fprintf(fp,"%s\r\n",strbuf);
                        }

                      if(TestItem2==1&&TestItem3!=1)
                        {
                          strcpy(strbuf,"drop off(V):");
                          fprintf(fp,"%s",strbuf);
                          if(TripTime2==-1)
                            strcpy(strbuf,"drop off(V):no trip");
                          else
                            gcvt(ReturnValue ,7,strbuf);
                          fprintf(fp,"%s\r\n",strbuf);
                        }

                      if(TestItem3==1)
                        {
                          strcpy(strbuf,"trip time(s):");
                          fprintf(fp,"%s",strbuf);
                          if(TripTime3==-1)
                            strcpy(strbuf,"trip time(s):no trip");
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

                    }
                     else
                     {
                         if(TestItem1==1)
                           {
                             strcpy(strbuf,"pick up(A):");
                             fprintf(fp,"%s",strbuf);
                             if(TripTime1==-1)
                               strcpy(strbuf,"pick up(A):no trip");
                             else
                               gcvt(TripValue ,7,strbuf);
                             fprintf(fp,"%s\r\n",strbuf);
                           }

                         if(TestItem2==1&&TestItem3!=1)
                           {
                             strcpy(strbuf,"drop off(A):");
                             fprintf(fp,"%s",strbuf);
                             if(TripTime2==-1)
                               strcpy(strbuf,"drop off(A):no trip");
                             else
                               gcvt(ReturnValue ,7,strbuf);
                             fprintf(fp,"%s\r\n",strbuf);
                           }

                         if(TestItem3==1)
                           {
                             strcpy(strbuf,"trip time(s):");
                             fprintf(fp,"%s",strbuf);
                             if(TripTime3==-1)
                               strcpy(strbuf,"trip time(s):no trip");
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
                     }
                      fclose(fp);
                      system("sync");
   }
}

void Widget::CloseWidget()
{

}



void Widget::on_pushButton_3_clicked() //exit
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

void Widget::on_pushButton_clicked()
{
    if(starttest==1)
        return;

Helpdialog *helpdialog=new Helpdialog;
temprorarydata->helpopenflag=true;
connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
helpdialog->SetHelpfilename("Sin.txt");
helpdialog->show();
}


void Widget::moveEvent(QMoveEvent *event)
{
 QPoint oldpos=event->oldPos();
 this->move(oldpos);
}

void Widget::HelpDialogClose()
{
temprorarydata->helpopenflag=false;

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





        pad=new keypads;
        pad->setModal(true);
        connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
        connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
        pad->show();
        pad->raise();
        pad->activateWindow();

      return true;
   }
    else if(event->type()==QEvent::FocusOut)
    {
        if(obj==ui->lineEdit)
        {
               QString text=ui->lineEdit->text();
               StartValue= text.toFloat();
               currenteditpos=1;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit->setText(QString::number(StartValue,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               EndValue= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(EndValue,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                ActStep= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(ActStep,'g',6));
         }
        else if(obj==ui->lineEdit_4)
         {
                QString text=ui->lineEdit_4->text();
                StepTime= text.toFloat();
                currenteditpos=4;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_4->setText(QString::number(StepTime,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                faultduration= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(faultduration,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                prefaulttime= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(prefaulttime,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                StopValue= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(StopValue,'g',6));
         }
        else if(obj==ui->lineEdit_8)
        {
               QString text=ui->lineEdit_8->text();
               StopValueHoldTime= text.toFloat();
               currenteditpos=8;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_8->setText(QString::number(StopValueHoldTime,'g',6));
        }
        return QWidget::eventFilter(obj,event);
    }
    else
    return QWidget::eventFilter(obj,event);

}



void Widget::GetTeststate(int state)
{
    //2016-9-13  键盘输入，界面测试按钮，
    //设置软件“测试”按钮获取焦点，激活失去焦点时间，给最后的输入幅值
    if(state==1)
    ui->pushButton_4->setFocus();
}




void Widget::on_tabWidget_currentChanged(int index)
{
   if(index==0)
   {
       TestItem1=1;
     //  TestItem2=1;
       TestItem3=0;

   }
   else
   {
       TestItem1=0;
      // TestItem2=0;
       TestItem3=1;

   }


}

void Widget::on_checkBox_clicked()
{
 if(TestItem2==1)
     TestItem2=0;
 else TestItem2=1;
}

void Widget::on_radioButton_7_clicked()
{
  if(ACRelayType==1)
      return;

  ACRelayType=1;
  ui->radioButton_7->setChecked(true);
  ui->radioButton_8->setChecked(false);


  OutputMode=1;
  ui->radioButton->setChecked(true);

  ui->radioButton_4->show();
  ui->radioButton_5->show();
  ui->radioButton_6->show();

  ui->radioButton->setText(tr("VA"));
  ui->radioButton_2->setText(tr("VB"));
  ui->radioButton_3->setText(tr("VC"));
  ui->radioButton_4->setText(tr("VAB串联"));
  ui->radioButton_5->setText(tr("VBC串联"));
  ui->radioButton_6->setText(tr("VCA串联"));
  ui->label_2->setText(tr("起始值(V):"));
  ui->label_3->setText(tr("终止值(V):"));
  ui->label_4->setText(tr("步长(V):"));
  ui->label_12->setText(tr("测试电压(V):"));




  Curpara.StartValue=StartValue;
  Curpara.EndValue=EndValue;
  Curpara.Step=ActStep;
  Curpara.Steptime=StepTime;
  Curpara.StopValue=StopValue;
  Curpara.StopValueHoldtime=StopValueHoldTime;

  StartValue=Volpara.StartValue;
  EndValue=Volpara.EndValue;
  ActStep=Volpara.Step;
  StepTime=Volpara.Steptime;
  StopValue=Volpara.StopValue;
  StopValueHoldTime=Volpara.StopValueHoldtime;




  ui->lineEdit->setText(QString::number(StartValue));
  ui->lineEdit_2->setText(QString::number(EndValue));
  ui->lineEdit_3->setText(QString::number(ActStep));
  ui->lineEdit_4->setText(QString::number(StepTime));
  ui->lineEdit_5->setText(QString::number(faultduration));
  ui->lineEdit_6->setText(QString::number(prefaulttime));
  ui->lineEdit_7->setText(QString::number(StopValue));
  ui->lineEdit_8->setText(QString::number(StopValueHoldTime));

}

void Widget::on_radioButton_8_clicked()
{
  if(ACRelayType==2)//dianliu jidianqi
      return;

  ACRelayType=2;
  ui->radioButton_7->setChecked(false);
  ui->radioButton_8->setChecked(true);


  OutputMode=1;
  ui->radioButton->setChecked(true);

 // ui->radioButton_4->hide();
  ui->radioButton_5->hide();
  ui->radioButton_6->hide();

  ui->radioButton->setText(tr("IA"));
  ui->radioButton_2->setText(tr("IB"));
  ui->radioButton_3->setText(tr("IC"));
  ui->radioButton_4->setText(tr("IA、B、C并联"));

  ui->label_2->setText(tr("起始值(A):"));
  ui->label_3->setText(tr("终止值(A):"));
  ui->label_4->setText(tr("步长(A):"));
  ui->label_12->setText(tr("测试电流(A):"));

  Volpara.StartValue=StartValue;
  Volpara.EndValue=EndValue;
  Volpara.Step=ActStep;
  Volpara.Steptime=StepTime;
  Volpara.StopValue=StopValue;
  Volpara.StopValueHoldtime=StopValueHoldTime;

  StartValue=Curpara.StartValue;
  EndValue=Curpara.EndValue;
  ActStep=Curpara.Step;
  StepTime=Curpara.Steptime;
  StopValue=Curpara.StopValue;
  StopValueHoldTime=Curpara.StopValueHoldtime;


  ui->lineEdit->setText(QString::number(StartValue));
  ui->lineEdit_2->setText(QString::number(EndValue));
  ui->lineEdit_3->setText(QString::number(ActStep));
  ui->lineEdit_4->setText(QString::number(StepTime));
  ui->lineEdit_5->setText(QString::number(faultduration));
  ui->lineEdit_6->setText(QString::number(prefaulttime));
  ui->lineEdit_7->setText(QString::number(StopValue));
  ui->lineEdit_8->setText(QString::number(StopValueHoldTime));

}

void Widget::on_pushButton_4_clicked()
{

    if(SoftStartEndflag==true)
          return;
      if(starttest==1)
      {
          ui->pushButton_4->setText(tr("测试"));
          ui->pushButton_4->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
          ui->pushButton->setEnabled(true);
          ui->pushButton_2->setEnabled(true);
          ui->pushButton_3->setEnabled(true);
      }
      else
      {
          ui->pushButton_4->setText(tr("停止"));
          ui->pushButton_4->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
          ui->pushButton->setEnabled(false);
          ui->pushButton_2->setEnabled(false);
          ui->pushButton_3->setEnabled(false);
      }

      SoftStartEndflag=true;

}

void Widget::GetTestState(){
    if(starttest==1)
    {
        ui->pushButton_4->setText(tr("停止"));
        ui->pushButton_4->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
    }
    else
    {
        ui->pushButton_4->setText(tr("测试"));
        ui->pushButton_4->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }

}
