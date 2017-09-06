#include "widget.h"
#include "ui_widget.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <QTextCodec>
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
#include <unistd.h>
#include "qtranslator.h"
QTranslator *tor;
int current_language=1;  //1中文  2英文
extern TEMPRORARYDATA *temprorarydata;

float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;


unsigned char result[70],receive_data[70];
double out_time,change_timedata;

float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;

long   SeqStep=0;

int   SeqStepTwo=0;
int   cx,cy,cx_start,cx_mid,cx_end,increase,starttest=0,low,row,page;

int   I1DC,I2DC,I3DC,V1DC,V2DC,V3DC;
int   RelayType,seq_count,DCAC;
int   TestItem1,TestItem2,TestItem3;


float FixedCurrent,FixedVoltage;

float StartCurrent,EndCurrent,CurActStep,CurReturnStep;
float StartVoltage,EndVoltage,VolActStep,VolReturnStep;
float HoldCurrent,HoldVoltage;
float StepTime;
float HoldTime=3;

float ChangeValue,TripValue,ReturnValue,TripTime1,TripTime2,TripTime3;
float LogicResolution,SystemFrec,Phase;

int exit_par,save_f;
int sys[4];
char ParStr[20];
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

float TestCurrent=3;
float CurTesttime=2;

float TestVoltage=50;
float VolTesttime=2;

bool Threadrunstate=true;

float SystemVolt;
float prefaulttime;

float Postfaulttime=3;
int   dcouttype=1;
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
    /* 加载语言文件 默认中文 2016-9-9*/
        tor = new QTranslator();
        if (current_language == 1) {
            tor->load(":/trans/zh.qm");
        }
        else if (current_language == 2) {
            tor->load(":/trans/en.qm");
        }
        qApp->installTranslator(tor);
        updateUI();

    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",13);
    setFont(font);
    setFixedSize(800,480);
    showFullScreen();

    QPixmap img(":/picture/hei.png");
    ui->label_2->setPixmap(img);
    ui->label_7->setPixmap(img);
    ui->label_19->setPixmap(img);
    ui->label_20->setPixmap(img);

    init_par();
    sysinit();
    currenteditpos=1;
    ui->tabWidget->setCurrentIndex(0);
    if(TestItem2==1)
    {

     ui->checkBox->setChecked(true);
    }

     //ui->label->setText("<font color=blue>"+QString("中间继电器(2.2.1)")+"</font>");
     ui->checkBox->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");

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



       ui->label_8->setAlignment(Qt::AlignCenter);
       ui->label_9->setAlignment(Qt::AlignCenter);


       //ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
       //ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
       //ui->pushButton->setFocusPolicy(Qt::NoFocus);
       //ui->pushButton_4->setFocusPolicy(Qt::NoFocus);




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





       uartthread =new thread1;
       connect(uartthread,SIGNAL(SIG_binarystate(int)),SLOT(showdspstate(int)));
       connect(uartthread,SIGNAL(SIG_testresult(int)),this,SLOT(showtestresult(int)));
       connect(uartthread,SIGNAL(SIG_DisplayChangeValue(int,QString,int,QString)),this,SLOT(DisplayChangeValue(int,QString,int,QString)));
       connect(uartthread,SIGNAL(SIG_dspstate(int)),this,SLOT(showdspstate(int)));
       connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
       connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
       connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));

       uartthread->start();


}
void Widget::updateUI()
{
    ui->label->setText("<font color=blue>"+QString(tr("中间继电器(2.2.1)"))+"</font>");
    //this->setWindowTitle(tr("主框架"));

        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_2->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_4->setText(tr("测试"));

        ui->groupBox->setTitle(tr("继电器类型"));
        ui->groupBox_3->setTitle(tr("输出类型"));
        ui->groupBox_5->setTitle(tr("测试结果"));

        ui->radioButton->setText(tr("电压启动，电压返回"));
        ui->radioButton_2->setText(tr("电流启动，电流返回"));
        ui->radioButton_3->setText(tr("电流启动，电流保持"));
        ui->radioButton_4->setText(tr("电压启动，电流保持"));
        ui->radioButton_5->setText(tr("电流启动，电压保持"));
        ui->radioButton_6->setText(tr("直流"));
        ui->radioButton_7->setText(tr("交流"));

        ui->label_3->setText(tr("保持电压(V):"));
        ui->label_14->setText(tr("保持电流(A):"));
        ui->label_10->setText(tr("起始电压(V):"));
        ui->label_15->setText(tr("起始电流(A):"));
        ui->label_11->setText(tr("终止电压(V):"));
        ui->label_16->setText(tr("终止电流(A):"));
        ui->label_12->setText(tr("电压步长(V):"));
        ui->label_17->setText(tr("电流步长(A):"));
        ui->label_13->setText(tr("步长时间(s):"));
        ui->label_18->setText(tr("保持时间(s):"));
        ui->label_6->setText(tr("最大输出时间(s)："));
        switch(RelayType)
        {
        case 1:
        case 4:
            ui->label_5->setText(tr("测试电压(V):"));
            break;
        case 2:
        case 3:
        case 5:
            ui->label_5->setText(tr("测试电流(A):"));
            break;
        }

        ui->checkBox->setText(tr("返回值"));

        ui->tabWidget->setTabText(0,tr("动作值"));
        ui->tabWidget->setTabText(1,tr("动作时间"));
}

void Widget::init_par()
  {

    FILE *fp;
    if((fp=fopen("sysset.txt","a+"))==NULL)
    return;
    fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&prefaulttime,&Postfaulttime,&dcouttype);
    fclose(fp);




    HoldVoltage=220;
    HoldCurrent=5;

    StepTime=0.5;
    TestItem1=1;
    TestItem2=1;
    TestItem3=1;

    RelayType=1;
    StartVoltage=70;
    EndVoltage=160;
    VolActStep=1;
    VolReturnStep=1;
    StepTime=0.1;

    StartCurrent=2;
    EndCurrent=10;
    CurActStep=0.1;
    CurReturnStep=0.1;


    HoldTime=3;





    Phase=0;
    DCAC=1;

    V1DC=1;
    V2DC=1;
    V3DC=1;
    I1DC=1;
    I2DC=1;
    I3DC=1;//1拢潞脰卤脕梅拢禄0拢潞陆禄脕梅

    MAX_V_VALUEDATA_DC=176;
    MIN_V_VALUEDATA_DC=-176;
    MAX_V_VALUEDATA_AC=125;
    MIN_V_VALUEDATA_AC=0;
    MAX_I_VALUEDATA_DC=70;
    MIN_I_VALUEDATA_DC=-70;
    MAX_I_VALUEDATA_AC=50;
    MIN_I_VALUEDATA_AC=0;
  }

Widget::~Widget()
{
    delete ui;
}


void  Widget::sysinit()
{

ui->radioButton->setChecked(true);

if(DCAC==1)
    ui->radioButton_6->setChecked(true);
else
    ui->radioButton_7->setChecked(true);

if(TestItem2==1)
    ui->checkBox->setChecked(true);





ui->lineEdit_3->setText(QString::number(HoldVoltage));


ui->lineEdit_6->setText(QString::number(StartVoltage));
ui->lineEdit_7->setText(QString::number(EndVoltage));
ui->lineEdit_8->setText(QString::number(VolActStep));
ui->lineEdit_9->setText(QString::number(StepTime));


ui->lineEdit_10->setText(QString::number(HoldCurrent));
ui->lineEdit_11->setText(QString::number(StartCurrent));
ui->lineEdit_12->setText(QString::number(EndCurrent));
ui->lineEdit_13->setText(QString::number(CurActStep));
ui->lineEdit_14->setText(QString::number(HoldTime));


ui->lineEdit_3->setEnabled(false);

ui->lineEdit_10->setEnabled(false);
ui->lineEdit_11->setEnabled(false);
ui->lineEdit_12->setEnabled(false);
ui->lineEdit_13->setEnabled(false);
ui->lineEdit_14->setEnabled(false);






ShowRelayType();

}

void  Widget::vi_parameter_enter(int position)
{

   float VoltageMax=1;
   float VoltageMin=0;
   float CurrentMax=1;
   float CurrentMin=1;

   if(DCAC==0)//交流
     {  VoltageMax=MAX_V_VALUEDATA_AC;
        VoltageMin=MIN_V_VALUEDATA_AC;
     }
   else  if(DCAC==1)//直流
     {   VoltageMax=MAX_V_VALUEDATA_DC;
         VoltageMin=MIN_V_VALUEDATA_DC;
     }

   if(DCAC==0)//交流
      { CurrentMax=MAX_I_VALUEDATA_AC;
        CurrentMin=MIN_I_VALUEDATA_AC;
      }
   else  if(DCAC==1)//直流
      {  CurrentMax=MAX_I_VALUEDATA_DC;
         CurrentMin=MIN_I_VALUEDATA_DC;
      }




   switch(position)
   {



  case 3:
       if(HoldVoltage>VoltageMax*2)
           HoldVoltage=VoltageMax*2;
       else if(HoldVoltage<VoltageMin)
           HoldVoltage=VoltageMin;
       break;

  case 4:
        if(RelayType==1||RelayType==4)
        {
            if(TestVoltage>VoltageMax*2)
              TestVoltage=VoltageMax*2;
            else if(TestVoltage<VoltageMin)
              TestVoltage=VoltageMin;
        }
        else if(RelayType==2||RelayType==3||RelayType==5)
        {
            if(TestCurrent>CurrentMax)
                TestCurrent=CurrentMax;
              else if(TestCurrent<CurrentMin)
                TestCurrent=CurrentMin;
        }
         break;
  case 5:
         if(RelayType==1||RelayType==4)
         {
             if(VolTesttime>999.99)
               VolTesttime=999.99;
             else if(VolTesttime<0)
               VolTesttime=0;
         }
          else if(RelayType==2||RelayType==3||RelayType==5)
         {
             if(CurTesttime>999.99)
               CurTesttime=999.99;
             else if(CurTesttime<0)
               CurTesttime=0;
         }
         break;
   case 6:
       if(StartVoltage>VoltageMax*2)
           StartVoltage=VoltageMax*2;
       else if(StartVoltage<VoltageMin)
           StartVoltage=VoltageMin;
       break;
   case 7:
       if(EndVoltage>VoltageMax*2)
           EndVoltage=VoltageMax*2;
       else if(EndVoltage<VoltageMin)
           EndVoltage=VoltageMin;
       break;
   case 8:
       if(RelayType==5)
       {
           if(VolActStep>=fabs(HoldVoltage))
               VolActStep=fabs(HoldVoltage);
       }
       else
       {
       if(VolActStep>fabs(StartVoltage-EndVoltage))
           VolActStep=fabs(StartVoltage-EndVoltage);
       else if(VolActStep<VoltageMin)
           VolActStep=VoltageMin;
       }
       VolReturnStep=VolActStep;
       break;
   case 9:
       if(StepTime>999.99)
         StepTime=999.99;
       else if(StepTime<0)
         StepTime=0;
       break;
   case 10:
         if(HoldCurrent>CurrentMax)
             HoldCurrent=CurrentMax;
         else if(HoldCurrent<CurrentMin)
             HoldCurrent=CurrentMin;
         break;
   case 11:
        if(StartCurrent>CurrentMax)
            StartCurrent=CurrentMax;
          else if(StartCurrent<CurrentMin)
            StartCurrent=CurrentMin;
          break;
   case 12:
       if(EndCurrent>CurrentMax)
           EndCurrent=CurrentMax;
         else if(EndCurrent<CurrentMin)
           EndCurrent=CurrentMin;
         break;
   case 13:
       if(RelayType==4)
       {
           if(CurActStep>=fabs(HoldCurrent))
               CurActStep=fabs(HoldCurrent);
       }
       else
       {
       if(CurActStep>fabs(StartCurrent-EndCurrent))
           CurActStep=fabs(StartCurrent-EndCurrent);
         else if(CurActStep<CurrentMin)
           CurActStep=CurrentMin;
       }
       CurReturnStep=CurActStep;
       break;
   case 14:
       if(HoldTime>999.99)
         HoldTime=999.99;
       else if(HoldTime<0)
         HoldTime=0;
       break;
     }
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
        if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                HoldVoltage= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(HoldVoltage,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                StartVoltage= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(StartVoltage,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                EndVoltage= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(EndVoltage,'g',6));
         }
        else if(obj==ui->lineEdit_8)
        {
               QString text=ui->lineEdit_8->text();
               VolActStep= text.toFloat();
               currenteditpos=8;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_8->setText(QString::number(VolActStep,'g',6));
        }
        else if(obj==ui->lineEdit_9)
        {
               QString text=ui->lineEdit_9->text();
               StepTime= text.toFloat();
               currenteditpos=9;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_9->setText(QString::number(StepTime,'g',6));
        }
        else if(obj==ui->lineEdit_10)
        {
               QString text=ui->lineEdit_10->text();
               HoldCurrent= text.toFloat();
               currenteditpos=10;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_10->setText(QString::number(HoldCurrent,'g',6));
        }
        else if(obj==ui->lineEdit_11)
        {
               QString text=ui->lineEdit_11->text();
               StartCurrent= text.toFloat();
               currenteditpos=11;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_11->setText(QString::number(StartCurrent,'g',6));
        }
        else if(obj==ui->lineEdit_12)
        {
               QString text=ui->lineEdit_12->text();
               EndCurrent= text.toFloat();
               currenteditpos=12;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_12->setText(QString::number(EndCurrent,'g',6));
        }
        else if(obj==ui->lineEdit_13)
        {
               QString text=ui->lineEdit_13->text();
               CurActStep= text.toFloat();
               currenteditpos=13;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_13->setText(QString::number(CurActStep,'g',6));
        }
        else if(obj==ui->lineEdit_14)
        {
               QString text=ui->lineEdit_14->text();
               HoldTime= text.toFloat();
               currenteditpos=14;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_14->setText(QString::number(HoldTime,'g',6));
        }
        switch(RelayType)
        {
        case 1:
        case 4:
            if(obj==ui->lineEdit_4)
             {
                    QString text=ui->lineEdit_4->text();
                    TestVoltage= text.toFloat();
                    currenteditpos=4;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_4->setText(QString::number(TestVoltage,'g',6));
             }
            else if(obj==ui->lineEdit_5)
             {
                    QString text=ui->lineEdit_5->text();
                    VolTesttime= text.toFloat();
                    currenteditpos=5;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_5->setText(QString::number(VolTesttime,'g',6));
             }
            break;
        case 2:
        case 3:
        case 5:
            if(obj==ui->lineEdit_4)
             {
                    QString text=ui->lineEdit_4->text();
                    TestCurrent= text.toFloat();
                    currenteditpos=4;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_4->setText(QString::number(TestCurrent,'g',6));
             }
            else if(obj==ui->lineEdit_5)
             {
                    QString text=ui->lineEdit_5->text();
                    CurTesttime= text.toFloat();
                    currenteditpos=5;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_5->setText(QString::number(CurTesttime,'g',6));
             }
            break;
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

 void Widget::ShowRelayType()
{
    switch(RelayType)
    {
    case 1:
    case 4:
        ui->label_5->setText(tr("测试电压(V):"));
        ui->label_6->setText(tr("最大输出时间(s):"));
         ui->lineEdit_4->setText(QString::number(TestVoltage));
         ui->lineEdit_5->setText(QString::number(VolTesttime));
        break;

    case 2:
    case 3:
    case 5:

        ui->label_5->setText(tr("测试电流(A):"));
        ui->label_6->setText(tr("最大输出时间(s):"));
        ui->lineEdit_4->setText(QString::number(TestCurrent));
        ui->lineEdit_5->setText(QString::number(CurTesttime));
        break;
    }



}



void Widget::showtestresult(int ResultSum)
{

    if(ResultSum==0)
    {
     ui->textBrowser->clear();
     return;

    }



        char bufstr[20];
        float ReturnQuotiety,Temp;
        switch(ResultSum)
          {
            case 1:
              if(RelayType==1||RelayType==4)
                {
                  if(TripTime1==-1)
                      ui->textBrowser->append(tr("动作值(V):未动作"));
                  else
                      ui->textBrowser->append(QString(tr("动作值(V):"))+QString::number(TripValue,'g',5));
                }
              else
              {
                  if(TripTime1==-1)
                      ui->textBrowser->append(tr("动作值(A):未动作"));
                  else
                      ui->textBrowser->append(QString(tr("动作值(A):"))+QString::number(TripValue,'g',5));
              }

              break;
            case 2:
              if(RelayType==1||RelayType==5)
                {
                  if(TripTime2==-1)
                      ui->textBrowser->append(tr("返回值(V):未动作"));
                  else
                      ui->textBrowser->append(QString(tr("返回值(V):"))+QString::number(ReturnValue,'g',5));
                }
              else
              {
                  if(TripTime2==-1)
                      ui->textBrowser->append(tr("返回值(A):未动作"));
                  else
                      ui->textBrowser->append(QString(tr("返回值(A):"))+QString::number(ReturnValue,'g',5));
              }

              if(RelayType==1||RelayType==2)
              {
                ReturnQuotiety=ReturnValue/TripValue;
                Temp=ReturnQuotiety*10000;
                Temp=ceil(Temp);
                ReturnQuotiety=Temp/10000;//将返回系数的限制在小数点后最多只有四位
                ui->textBrowser->append(QString(tr("返回系数:"))+QString::number(ReturnQuotiety,'g',5));
              }
              break;
            case 3:
              if(TripTime3==-1)
                  ui->textBrowser->append(tr("动作时间(s):未动作"));
             else
                  ui->textBrowser->append(QString(tr("动作时间(s):"))+QString::number(TripTime3,'g',5));

              break;
          }



}

void Widget::showdspstate(int state)
{
    switch(state)
    {
     case 0: ui->label_4->setText(tr("运行正常")); break;
     case 1: ui->label_4->setText(tr("IA 过载")); break;
     case 2: ui->label_4->setText(tr("IABC 温度保护")); break;
     case 3: ui->label_4->setText(tr("IC 间歇")); break;
     case 4: ui->label_4->setText(tr("IB 过载")); break;
     case 5: ui->label_4->setText(tr("IC 闭锁")); break;
     case 6: ui->label_4->setText(tr("IA 间歇")); break;
     case 7: ui->label_4->setText(tr("IC 过载")); break;
     case 8: ui->label_4->setText(tr("IABC 电源保护")); break;
     case 9: ui->label_4->setText(tr("IB 间歇")); break;
     case 10: ui->label_4->setText(tr("VA 过载")); break;

     case 12: ui->label_4->setText(tr("VC 间歇")); break;
     case 13: ui->label_4->setText(tr("VB 过载")); break;
     case 14: ui->label_4->setText(tr("VABC 温度保护")); break;
     case 15: ui->label_4->setText(tr("VA 间歇")); break;
     case 16: ui->label_4->setText(tr("VC 过载")); break;
     case 17: ui->label_4->setText(tr("VABC 电源保护")); break;
     case 18: ui->label_4->setText(tr("Vz 过载")); break;
     case 19: ui->label_4->setText(tr("直流过载")); break;
     case 20: ui->label_4->setText(tr("IABC 电源保护")); break;
     case 21: ui->label_4->setText(tr("VC 间歇")); break;
    }
   QPixmap img(":/picture/hei.png");
   QPixmap pic(":/picture/hong.png");
   switch(state)
   {
   case 100:
       ui->label_2->setPixmap(img);
       ui->label_7->setPixmap(img);
       ui->label_19->setPixmap(img);
       ui->label_20->setPixmap(img);
       break;
   case 101: ui->label_2->setPixmap(pic); break;
   case 102: ui->label_7->setPixmap(pic); break;
   case 103: ui->label_19->setPixmap(pic); break;
   case 104: ui->label_20->setPixmap(pic); break;
   case 111: ui->label_2->setPixmap(img); break;
   case 112: ui->label_7->setPixmap(img); break;
   case 113: ui->label_19->setPixmap(img); break;
   case 114: ui->label_20->setPixmap(img); break;
   }

}

void Widget::DisplayChangeValue(int valid, QString  value,int valid2,QString value2)
{
if(valid==0)
    ui->label_8->clear();
else
    ui->label_8->setText(value);
if(valid2==0)
    ui->label_9->clear();
else
    ui->label_9->setText(value2);




}

void Widget::HelpDialogClose()
{
temprorarydata->helpopenflag=false;

}

void Widget::moveEvent(QMoveEvent *event)
{
    QPoint oldpos=event->oldPos();
    this->move(oldpos);
}

 void Widget::CloseWidget()
 {


 }






void Widget::on_pushButton_clicked()
{
    if(starttest==1)
        return;

Helpdialog *helpdialog=new Helpdialog;
temprorarydata->helpopenflag=true;
connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
helpdialog->SetHelpfilename("Middle.txt");
helpdialog->show();

}

void Widget::on_pushButton_2_clicked()
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
                float ReturnQuotiety;
                float Temp;
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


                if(DCAC==1)
                strcpy(strbuf,"直流中间继电器");
                else
                strcpy(strbuf,"交流中间继电器");
                fprintf(fp,"%s\r\n",strbuf);

                if(RelayType==1)
                  strcpy(strbuf,"电压启动，电压返回");
                else if(RelayType==2)
                  strcpy(strbuf,"电流启动，电流返回");
                else if(RelayType==3)
                  strcpy(strbuf,"电流启动,电流保持");
                else if(RelayType==4)
                  strcpy(strbuf,"电压启动，电流保持");
                else if(RelayType==5)
                  strcpy(strbuf,"电压启动，电压保持");
                fprintf(fp,"%s\r\n",strbuf);

                if(TestItem1==1)
                  {
                    if(RelayType==1)
                      {
                        strcpy(strbuf,"动作电压(V):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime1==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(TripValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                    else if(RelayType==2)
                      {
                        strcpy(strbuf,"动作电流(A):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime1==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(TripValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                    else if(RelayType==3)
                      {
                        strcpy(strbuf,"动作电流(A):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime1==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(TripValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                    else if(RelayType==4)
                      {
                        strcpy(strbuf,"动作电压(V):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime1==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(TripValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                    else if(RelayType==5)
                      {
                        strcpy(strbuf,"动作电流(A):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime1==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(TripValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                  }

                if(TestItem2==1&&TestItem3!=1)
                  {
                    if(RelayType==1)
                      {
                        strcpy(strbuf,"电压返回值(V):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime2==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(ReturnValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);

                        if(TestItem1==1&&TestItem2==1&&TripTime2!=-1)
                          {
                            strcpy(strbuf,"返回系数:");
                            fprintf(fp,"%s",strbuf);
                            ReturnQuotiety=ReturnValue/TripValue;
                            Temp=ReturnQuotiety*10000;
                            Temp=ceil(Temp);
                            ReturnQuotiety=Temp/10000;
                            gcvt(ReturnQuotiety,7,strbuf);
                            fprintf(fp,"%s\r\n",strbuf);
                          }




                      }
                    else if(RelayType==2)
                      {
                        strcpy(strbuf,"电流返回值(A):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime2==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(ReturnValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);

                        if(TestItem1==1&&TestItem2==1&&TripTime2!=-1)
                          {
                            strcpy(strbuf,"返回系数:");
                            fprintf(fp,"%s",strbuf);
                            ReturnQuotiety=ReturnValue/TripValue;
                            Temp=ReturnQuotiety*10000;
                            Temp=ceil(Temp);
                            ReturnQuotiety=Temp/10000;
                            gcvt(ReturnQuotiety,7,strbuf);
                            fprintf(fp,"%s\r\n",strbuf);
                          }
                      }
                    else if(RelayType==3)
                      {
                        strcpy(strbuf,"返回电流(A):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime2==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(ReturnValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                    else if(RelayType==4)
                      {
                        strcpy(strbuf,"返回电流(A):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime2==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(ReturnValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                    else if(RelayType==5)
                      {
                        strcpy(strbuf,"返回电压(V):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime2==-1)
                          strcpy(strbuf,"未动作");
                        else
                          gcvt(ReturnValue ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                  }

                if(TestItem3==1&&TestItem1!=1)
                  {
                    if(RelayType==1)
                      {
                        strcpy(strbuf,"动作时间(s):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime3==-1)
                          strcpy(strbuf,"动作时间(s):未动作");
                        else
                          gcvt(TripTime3 ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);

                      }
                    else if(RelayType==2)
                      {
                        strcpy(strbuf,"动作时间(s):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime3==-1)
                          strcpy(strbuf,"动作时间(s):未动作");
                        else
                          gcvt(TripTime3 ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);

                      }
                    else if(RelayType==3)
                      {
                        strcpy(strbuf,"动作时间(s):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime3==-1)
                          strcpy(strbuf,"动作时间(s):未动作");
                        else
                          gcvt(TripTime3 ,7,strbuf);
                        fprintf(fp,"%s\r\n",strbuf);
                      }
                    else if(RelayType==4)
                      {
                        strcpy(strbuf,"动作时间(s):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime3==-1)
                          strcpy(strbuf,"动作时间(s):未动作");
                        else
                          gcvt(TripTime3 ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                    else if(RelayType==5)
                      {
                        strcpy(strbuf,"动作时间(s):");
                        fprintf(fp,"%s",strbuf);
                        if(TripTime3==-1)
                          strcpy(strbuf,"动作时间(s):未动作");
                        else
                          gcvt(TripTime3 ,7,strbuf);
                        fprintf(fp," %s\r\n",strbuf);
                      }
                  }

                 fclose(fp);
                 system("sync");
        }
        else
        {
            FILE *fp;
            char strbuf[80];
            int day,mon,year;
            int hour,minute,second;
            float ReturnQuotiety;
            float Temp;
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


            if(DCAC==1)
            strcpy(strbuf,"DC Intermediate relay test result");
            else
            strcpy(strbuf,"AC Intermediate relay test result");
            fprintf(fp,"%s\r\n",strbuf);

            if(RelayType==1)
              strcpy(strbuf,"V Start,V Return");
            else if(RelayType==2)
              strcpy(strbuf,"I Start,I Return");
            else if(RelayType==3)
              strcpy(strbuf,"I Start,I Keep");
            else if(RelayType==4)
              strcpy(strbuf,"V Start,I Keep");
            else if(RelayType==5)
              strcpy(strbuf,"V Start,V Keep");
            fprintf(fp,"%s\r\n",strbuf);

            if(TestItem1==1)
              {
                if(RelayType==1)
                  {
                    strcpy(strbuf,"Trip Voltage(V):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime1==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(TripValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
                else if(RelayType==2)
                  {
                    strcpy(strbuf,"Trip Current(A):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime1==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(TripValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
                else if(RelayType==3)
                  {
                    strcpy(strbuf,"Trip Current(A):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime1==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(TripValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
                else if(RelayType==4)
                  {
                    strcpy(strbuf,"Trip Voltage(V):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime1==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(TripValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
                else if(RelayType==5)
                  {
                    strcpy(strbuf,"Trip Current(A):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime1==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(TripValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
              }

            if(TestItem2==1&&TestItem3!=1)
              {
                if(RelayType==1)
                  {
                    strcpy(strbuf,"Drop Off Voltage(V):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime2==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(ReturnValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);

                    if(TestItem1==1&&TestItem2==1&&TripTime2!=-1)
                      {
                        strcpy(strbuf,"Drop Off Ratio:");
                        fprintf(fp,"%s",strbuf);
                        ReturnQuotiety=ReturnValue/TripValue;
                        Temp=ReturnQuotiety*10000;
                        Temp=ceil(Temp);
                        ReturnQuotiety=Temp/10000;
                        gcvt(ReturnQuotiety,7,strbuf);
                        fprintf(fp,"%s\r\n",strbuf);
                      }




                  }
                else if(RelayType==2)
                  {
                    strcpy(strbuf,"Drop Off Currrent(A):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime2==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(ReturnValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);

                    if(TestItem1==1&&TestItem2==1&&TripTime2!=-1)
                      {
                        strcpy(strbuf,"Drop Off Ratio:");
                        fprintf(fp,"%s",strbuf);
                        ReturnQuotiety=ReturnValue/TripValue;
                        Temp=ReturnQuotiety*10000;
                        Temp=ceil(Temp);
                        ReturnQuotiety=Temp/10000;
                        gcvt(ReturnQuotiety,7,strbuf);
                        fprintf(fp,"%s\r\n",strbuf);
                      }
                  }
                else if(RelayType==3)
                  {
                    strcpy(strbuf,"Drop Off Currrent(A):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime2==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(ReturnValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
                else if(RelayType==4)
                  {
                    strcpy(strbuf,"Drop Off Currrent(A):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime2==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(ReturnValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
                else if(RelayType==5)
                  {
                    strcpy(strbuf,"Drop Off Voltage(V):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime2==-1)
                      strcpy(strbuf,"No Trip");
                    else
                      gcvt(ReturnValue ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
              }

            if(TestItem3==1&&TestItem1!=1)
              {
                if(RelayType==1)
                  {
                    strcpy(strbuf,"Trip Time(s):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime3==-1)
                      strcpy(strbuf,"Trip Time(s):No Trip");
                    else
                      gcvt(TripTime3 ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);

                  }
                else if(RelayType==2)
                  {
                    strcpy(strbuf,"Trip Time(s):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime3==-1)
                      strcpy(strbuf,"Trip Time(s):No Trip");
                    else
                      gcvt(TripTime3 ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);

                  }
                else if(RelayType==3)
                  {
                    strcpy(strbuf,"Trip Time(s):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime3==-1)
                      strcpy(strbuf,"Trip Time(s):No Trip");
                    else
                      gcvt(TripTime3 ,7,strbuf);
                    fprintf(fp,"%s\r\n",strbuf);
                  }
                else if(RelayType==4)
                  {
                    strcpy(strbuf,"Trip Time(s):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime3==-1)
                      strcpy(strbuf,"Trip Time(s):No Trip");
                    else
                      gcvt(TripTime3 ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
                else if(RelayType==5)
                  {
                    strcpy(strbuf,"Trip Time(s):");
                    fprintf(fp,"%s",strbuf);
                    if(TripTime3==-1)
                      strcpy(strbuf,"Trip Time(s):No Trip");
                    else
                      gcvt(TripTime3 ,7,strbuf);
                    fprintf(fp," %s\r\n",strbuf);
                  }
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
    RelayType=1;

    ui->lineEdit_3->setEnabled(true);
    ui->lineEdit_6->setEnabled(true);
    ui->lineEdit_7->setEnabled(true);
    ui->lineEdit_8->setEnabled(true);
    ui->lineEdit_9->setEnabled(true);
    ui->lineEdit_10->setEnabled(true);
    ui->lineEdit_11->setEnabled(true);
    ui->lineEdit_12->setEnabled(true);
    ui->lineEdit_13->setEnabled(true);
    ui->lineEdit_14->setEnabled(true);

    ui->lineEdit_3->setEnabled(false);

    ui->lineEdit_10->setEnabled(false);
    ui->lineEdit_11->setEnabled(false);
    ui->lineEdit_12->setEnabled(false);
    ui->lineEdit_13->setEnabled(false);
    ui->lineEdit_14->setEnabled(false);





    ShowRelayType();
}


void Widget::on_radioButton_2_clicked()
{
    RelayType=2;

    ui->lineEdit_3->setEnabled(true);
    ui->lineEdit_6->setEnabled(true);
    ui->lineEdit_7->setEnabled(true);
    ui->lineEdit_8->setEnabled(true);
    ui->lineEdit_9->setEnabled(true);
    ui->lineEdit_10->setEnabled(true);
    ui->lineEdit_11->setEnabled(true);
    ui->lineEdit_12->setEnabled(true);
    ui->lineEdit_13->setEnabled(true);
    ui->lineEdit_14->setEnabled(true);

    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);
    ui->lineEdit_8->setEnabled(false);
    ui->lineEdit_10->setEnabled(false);
    ui->lineEdit_14->setEnabled(false);


    ShowRelayType();
}


void Widget::on_radioButton_3_clicked()
{
     RelayType=3;


     ui->lineEdit_3->setEnabled(true);
     ui->lineEdit_6->setEnabled(true);
     ui->lineEdit_7->setEnabled(true);
     ui->lineEdit_8->setEnabled(true);
     ui->lineEdit_9->setEnabled(true);
     ui->lineEdit_10->setEnabled(true);
     ui->lineEdit_11->setEnabled(true);
     ui->lineEdit_12->setEnabled(true);
     ui->lineEdit_13->setEnabled(true);
     ui->lineEdit_14->setEnabled(true);



     ui->lineEdit_3->setEnabled(false);
     ui->lineEdit_6->setEnabled(false);
     ui->lineEdit_7->setEnabled(false);
     ui->lineEdit_8->setEnabled(false);
      ShowRelayType();
}


void Widget::on_radioButton_4_clicked()
{
    RelayType=4;

    ui->lineEdit_3->setEnabled(true);
    ui->lineEdit_6->setEnabled(true);
    ui->lineEdit_7->setEnabled(true);
    ui->lineEdit_8->setEnabled(true);
    ui->lineEdit_9->setEnabled(true);
    ui->lineEdit_10->setEnabled(true);
    ui->lineEdit_11->setEnabled(true);
    ui->lineEdit_12->setEnabled(true);
    ui->lineEdit_13->setEnabled(true);
    ui->lineEdit_14->setEnabled(true);

    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_11->setEnabled(false);
    ui->lineEdit_12->setEnabled(false);


    ShowRelayType();
}


void Widget::on_radioButton_5_clicked()
{
     RelayType=5;


     ui->lineEdit_3->setEnabled(true);
     ui->lineEdit_6->setEnabled(true);
     ui->lineEdit_7->setEnabled(true);
     ui->lineEdit_8->setEnabled(true);
     ui->lineEdit_9->setEnabled(true);
     ui->lineEdit_10->setEnabled(true);
     ui->lineEdit_11->setEnabled(true);
     ui->lineEdit_12->setEnabled(true);
     ui->lineEdit_13->setEnabled(true);
     ui->lineEdit_14->setEnabled(true);



    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);
    ui->lineEdit_10->setEnabled(false);

    ShowRelayType();
}



void Widget::on_radioButton_6_clicked()//直流
{
   DCAC=1;
   float VoltageMax=1;
   float VoltageMin=0;
   float CurrentMax=1;
   float CurrentMin=1;

   if(DCAC==0)//交流
     {  VoltageMax=MAX_V_VALUEDATA_AC;
        VoltageMin=MIN_V_VALUEDATA_AC;
     }
   else  if(DCAC==1)//直流
     {   VoltageMax=MAX_V_VALUEDATA_DC;
         VoltageMin=MIN_V_VALUEDATA_DC;
     }

   if(DCAC==0)//交流
      { CurrentMax=MAX_I_VALUEDATA_AC;
        CurrentMin=MIN_I_VALUEDATA_AC;
      }
   else  if(DCAC==1)//直流
      {  CurrentMax=MAX_I_VALUEDATA_DC;
         CurrentMin=MIN_I_VALUEDATA_DC;
      }




       if(HoldVoltage>VoltageMax*2)
       {
           HoldVoltage=VoltageMax*2;
           ui->lineEdit_3->setText(QString::number(HoldVoltage));
       }

       if(RelayType==1||RelayType==4)
        {
            if(TestVoltage>VoltageMax*2)
            {
              TestVoltage=VoltageMax*2;
              ui->lineEdit_4->setText(QString::number(TestVoltage));
            }

        }
        else if(RelayType==2||RelayType==3||RelayType==5)
        {
            if(TestCurrent>CurrentMax)
            {
                TestCurrent=CurrentMax;
                ui->lineEdit_4->setText(QString::number(TestCurrent));
            }
        }

       if(StartVoltage>VoltageMax*2)
       {
           StartVoltage=VoltageMax*2;
           ui->lineEdit_6->setText(QString::number(StartVoltage));
       }

       if(EndVoltage>VoltageMax*2)
       {
           EndVoltage=VoltageMax*2;
           ui->lineEdit_7->setText(QString::number(EndVoltage));
       }
       if(VolActStep>fabs(StartVoltage-EndVoltage))
       {
           VolActStep=fabs(StartVoltage-EndVoltage);
               VolReturnStep=VolActStep;
               ui->lineEdit_8->setText(QString::number(VolActStep));
       }
         if(HoldCurrent>CurrentMax)
         {
             HoldCurrent=CurrentMax;
             ui->lineEdit_10->setText(QString::number(HoldCurrent));
         }
        if(StartCurrent>CurrentMax)
        {
            StartCurrent=CurrentMax;
            ui->lineEdit_11->setText(QString::number(StartCurrent));
        }

       if(EndCurrent>CurrentMax)
       {
           EndCurrent=CurrentMax;
           ui->lineEdit_12->setText(QString::number(EndCurrent));
       }
       if(CurActStep>fabs(StartCurrent-EndCurrent))
       {
           CurActStep=fabs(StartCurrent-EndCurrent);
           CurReturnStep=CurActStep;
              ui->lineEdit_13->setText(QString::number(CurActStep));
       }
}

void Widget::on_radioButton_7_clicked()//交流
{
     DCAC=0;
     float VoltageMax=1;
     float VoltageMin=0;
     float CurrentMax=1;
     float CurrentMin=1;

     if(DCAC==0)//交流
       {  VoltageMax=MAX_V_VALUEDATA_AC;
          VoltageMin=MIN_V_VALUEDATA_AC;
       }
     else  if(DCAC==1)//直流
       {   VoltageMax=MAX_V_VALUEDATA_DC;
           VoltageMin=MIN_V_VALUEDATA_DC;
       }

     if(DCAC==0)//交流
        { CurrentMax=MAX_I_VALUEDATA_AC;
          CurrentMin=MIN_I_VALUEDATA_AC;
        }
     else  if(DCAC==1)//直流
        {  CurrentMax=MAX_I_VALUEDATA_DC;
           CurrentMin=MIN_I_VALUEDATA_DC;
        }




         if(HoldVoltage>VoltageMax*2)
         {
             HoldVoltage=VoltageMax*2;
             ui->lineEdit_3->setText(QString::number(HoldVoltage));
         }

         if(RelayType==1||RelayType==4)
          {
              if(TestVoltage>VoltageMax*2)
              {
                TestVoltage=VoltageMax*2;
                ui->lineEdit_4->setText(QString::number(TestVoltage));
              }

          }
          else if(RelayType==2||RelayType==3||RelayType==5)
          {
              if(TestCurrent>CurrentMax)
              {
                  TestCurrent=CurrentMax;
                  ui->lineEdit_4->setText(QString::number(TestCurrent));
              }
          }

         if(StartVoltage>VoltageMax*2)
         {
             StartVoltage=VoltageMax*2;
             ui->lineEdit_6->setText(QString::number(StartVoltage));
         }

         if(EndVoltage>VoltageMax*2)
         {
             EndVoltage=VoltageMax*2;
             ui->lineEdit_7->setText(QString::number(EndVoltage));
         }
         if(VolActStep>fabs(StartVoltage-EndVoltage))
         {
             VolActStep=fabs(StartVoltage-EndVoltage);
                 VolReturnStep=VolActStep;
                 ui->lineEdit_8->setText(QString::number(VolActStep));
         }
           if(HoldCurrent>CurrentMax)
           {
               HoldCurrent=CurrentMax;
               ui->lineEdit_10->setText(QString::number(HoldCurrent));
           }
          if(StartCurrent>CurrentMax)
          {
              StartCurrent=CurrentMax;
              ui->lineEdit_11->setText(QString::number(StartCurrent));
          }

         if(EndCurrent>CurrentMax)
         {
             EndCurrent=CurrentMax;
             ui->lineEdit_12->setText(QString::number(EndCurrent));
         }
         if(CurActStep>fabs(StartCurrent-EndCurrent))
         {
             CurActStep=fabs(StartCurrent-EndCurrent);
             CurReturnStep=CurActStep;
                ui->lineEdit_13->setText(QString::number(CurActStep));
         }

}





void Widget::on_checkBox_clicked()
{
    if(TestItem2==1)
        TestItem2=0;
    else
        TestItem2=1;
}


void Widget::on_tabWidget_currentChanged(int index)
{
    if(index==0)
     {   TestItem1=1;
         TestItem3=0;
     }
    else
    {
        TestItem1=0;
        TestItem3=1;
    }
}


void Widget::setlineedittext(QString str)
{
    bool ok;
    float number=str.toFloat(&ok);
    if(ok==false)
        return;
    switch(currenteditpos)
    {

    case 3:
        HoldVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HoldVoltage,'g',6));
        break;
    case 4:
        if(RelayType==1||RelayType==4)
       {
        TestVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(TestVoltage,'g',6));
       }
        else
        {
            TestCurrent=number;
            vi_parameter_enter(currenteditpos);
            currentedit->setText(QString::number(TestCurrent,'g',6));
        }


        break;
    case 5:
        if(RelayType==1||RelayType==4)
       {
        VolTesttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VolTesttime,'g',6));
       }
        else
        {
            CurTesttime=number;
            vi_parameter_enter(currenteditpos);
            currentedit->setText(QString::number(CurTesttime,'g',6));
        }
        break;

    case 6:
        StartVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StartVoltage,'g',6));

        break;

    case 7:
        EndVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(EndVoltage,'g',6));

        break;

    case 8:
        VolActStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VolActStep,'g',6));

        break;

    case 9:
        StepTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StepTime,'g',6));

        break;

    case 10:
        HoldCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HoldCurrent,'g',6));

        break;


    case 11:
        StartCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StartCurrent,'g',6));
        break;

    case 12:
        EndCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(EndCurrent,'g',6));
        break;


    case 13:
        CurActStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(CurActStep,'g',6));

        break;

    case 14:
        HoldTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HoldTime,'g',6));
        break;

    }

}
void Widget::clearkeypadflag()
{
 temprorarydata->keypadopenflag=false;
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
