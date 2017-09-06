#include "manualtest.h"
#include "ui_manualtest.h"
#include "qtranslator.h"
#include <QTextCodec>
#include <stdlib.h>
#include <stdio.h>
#include <QTime>
#include "Config.h"
#include <QFile>
#include <QString>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <math.h>
#include <unistd.h>

QTranslator *tor;
int current_language=1;  //1中文  2英文

extern TEMPRORARYDATA *temprorarydata;

float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
unsigned char result[70],receive_data[70];
double out_time,change_timedata;

float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision,VzPhaseRevision;
float IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;


int   cursorF=0,cursor=0,cursorK=0;
int   input_par=0,Decimal=0,sige_flag=0,HelpFlag=0,T4;

 int   VariatyV1,VariatyV2,VariatyV3,VariatyVz,VariatyVz2,VariatyI1,VariatyI2,VariatyI3;
int   VariatyP,Variable,TestMode,INPAR=0;
int   channum,blocking_num,variate_num,testcount=0,time_relay_testcount=0,auto_end=0;
int   cx,cy,cx_start,cx_mid,cx_end,increase,starttest=0,low,row,page;




int   FreqTriple=2;
int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC;
float V1Value,V1Phase,V1Frec;
float V2Value,V2Phase,V2Frec;
float V3Value,V3Phase,V3Frec;
float VzValue,VzPhase,VzFrec;
float I1Value,I1Phase,I1Frec;
float I2Value,I2Phase,I2Frec;
float I3Value,I3Phase,I3Frec;

float ChangeV1Value,ChangeV1Phase,ChangeV1Frec;
float ChangeV2Value,ChangeV2Phase,ChangeV2Frec;
float ChangeV3Value,ChangeV3Phase,ChangeV3Frec;
float ChangeVzValue,ChangeVzPhase,ChangeVzFrec;
float ChangeI1Value,ChangeI1Phase,ChangeI1Frec;
float ChangeI2Value,ChangeI2Phase,ChangeI2Frec;
float ChangeI3Value,ChangeI3Phase,ChangeI3Frec;

float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;
float CurStep,VolStep,PhaStep,FreStep,StepTime,StepNum;


float PreFaultTime,FaultTime,PostFaultTime;
float timeout;
float ChangeValue,TripValue,ReturnValue,TripTime;
float VaValueReviosn[10],VbValueReviosn[10],VcValueReviosn[10],VzValueReviosn[10];
float IaValueReviosn[10],IbValueReviosn[10],IcValueReviosn[10];
float V1OutputValue,V2OutputValue,V3OutputValue,VzOutputValue;
float I1OutputValue,I2OutputValue,I3OutputValue;

int StartTestFlag;
int exit_par,save_f;
char report_time[30],report_load[30];
float save_time;
int sys[4];
float logic_delaytime,SysFrec,SysVolt;
float  V1PerFaultValue,V2PerFaultValue,V3PerFaultValue,VzPerFaultValue,I1PerFaultValue,I2PerFaultValue,I3PerFaultValue;
float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase,VzPerFaultPhase,I1PerFaultPhase,I2PerFaultPhase,I3PerFaultPhase;

char ParStr[20];
char   V1Output,V2Output,V3Output,VzOutput,I1Output,I2Output,I3Output;


float faultduration=0;
float SumStepnum=0;

bool Phasesymmetry=false;
bool Threadrunstate=true;

bool SoftStartEndflag=false;

int   dcouttype=1;
bool IsSendcmd=false;



double V1ampPerStepError=0;  //电压VA每步变化的误差值
double V2ampPerStepError=0;  //电压VB每步变化的误差值
double V3ampPerStepError=0;  //电压VB每步变化的误差值
double VzampPerStepError=0;  //电压VZ每步变化的误差值

double I1ampPerStepError=0;
double I2ampPerStepError=0;
double I3ampPerStepError=0;




Manualtest::Manualtest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manualtest)
{
    ui->setupUi(this);
#ifdef ARMLINUX
  this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
#endif
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
    setCursor(Qt::ArrowCursor);
    //ui->label_12->setText(("<font color=blue>"+QString("任意测试(2.2.1)")+"</font>"));

    QPixmap img(":/picture/hei.png");
    ui->label_23->setPixmap(img);
    ui->label_24->setPixmap(img);
    ui->label_25->setPixmap(img);
    ui->label_26->setPixmap(img);

    ui->lineEdit_23->setEnabled(false);
    ui->lineEdit_24->setEnabled(false);
    ui->label_20->setEnabled(false);
    ui->label_21->setEnabled(false);

    showFullScreen();
    init_par();
    initalldata();
    ui->label_4->setEnabled(false);
    ui->lineEdit_28->setEnabled(false);
    display_mode(TestMode);

     VolPhaseChange=0;  //电压相位最后一次改变的位置
     CurPhaseChange=0;  //电流相位最后一次改变的位置


     if(VariatyV1==1)
      ui->checkBox->setChecked(true);
     else
     ui->checkBox->setChecked(false);
     if(VariatyV2==1)
       ui->checkBox_2->setChecked(true);
     else
       ui->checkBox_2->setChecked(false);
     if(VariatyV3==1)
     ui->checkBox_3->setChecked(true);
     else
     ui->checkBox_3->setChecked(false);



     if(VariatyI1==1)
        ui->checkBox_4->setChecked(true);
     else
        ui->checkBox_4->setChecked(false);
     if(VariatyI2==1)
      ui->checkBox_5->setChecked(true);
     else
      ui->checkBox_5->setChecked(false);
     if(VariatyI3==1)
       ui->checkBox_6->setChecked(true);
     else
       ui->checkBox_6->setChecked(false);

    ui->checkBox_7->setChecked(Phasesymmetry);

    display_variry(Variable);
    ui->checkBox->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_2->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_3->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_4->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_5->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_6->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_7->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_8->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_9->setStyleSheet("QCheckBox{font:15px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");




    ui->label_10->setText(tr("运行正常"));

    lineeditlist<<ui->lineEdit<<ui->lineEdit_2<<ui->lineEdit_3<<ui->lineEdit_4<<\
                  ui->lineEdit_5<<ui->lineEdit_6<<ui->lineEdit_7<<ui->lineEdit_8<<\
                  ui->lineEdit_9<<ui->lineEdit_10<<ui->lineEdit_11<<ui->lineEdit_12<<\
                  ui->lineEdit_13<<ui->lineEdit_14<<ui->lineEdit_15<<ui->lineEdit_16<<\
                  ui->lineEdit_17<<ui->lineEdit_18<<ui->lineEdit_19<<ui->lineEdit_20<<\
                  ui->lineEdit_21<<ui->lineEdit_22<<ui->lineEdit_23<<ui->lineEdit_24<<\
                  ui->lineEdit_25<<ui->lineEdit_26<<ui->lineEdit_27<<ui->lineEdit_28<<\
                  ui->lineEdit_29<<ui->lineEdit_30<<ui->lineEdit_31;



    for(int i=0;i<lineeditlist.size();i++)
    {
        QLineEdit *lineedit=lineeditlist.at(i);
        lineedit->installEventFilter(this);
      //  lineedit->setFocusPolicy(Qt::NoFocus);
        lineedit->setContextMenuPolicy(Qt::NoContextMenu);
        lineedit->setAlignment(Qt::AlignCenter);
    }

    //ui->pushButton_6->setFocusPolicy(Qt::NoFocus);


    uartthread=new thread1;
    connect(uartthread,SIGNAL(SIG_binarystate(int)),this,SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),this,SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_testresult(bool)),this,SLOT(showtestresult(bool)));
    connect(uartthread,SIGNAL(SIG_setVanocheck()),this,SLOT(setVanocheck()));
    connect(this,SIGNAL(SIG_Manualoutput()),uartthread,SLOT(Manualoutput()));
    connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
    connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));
    connect(uartthread,SIGNAL(SIG_SendWarning(int,int)),this,SLOT(ShowWarning(int,int)));
    connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
    connect(uartthread,SIGNAL(SIG_DisplayAutoChangeValue(int)),SLOT(DisplayAutoChangeValue(int)));


    uartthread->start();
}

void Manualtest::updateUI()
{
    //this->setWindowTitle(tr("主框架"));
    ui->pushButton->setText(tr("帮助"));
    ui->pushButton_2->setText(tr("保存"));
    ui->pushButton_3->setText(tr("退出"));
    ui->pushButton_4->setText(tr("增加"));
    ui->pushButton_5->setText(tr("减少"));
    ui->pushButton_6->setText(tr("测试"));

    ui->label->setText(tr("故障前："));
    ui->label_2->setText(tr("故障："));
    ui->label_3->setText(tr("故障后："));
    ui->label_4->setText(tr("动作保持时间(s):"));
    ui->label_5->setText(tr("幅值"));
    ui->label_6->setText(tr("相位"));
    ui->label_7->setText(tr("频率"));
    ui->label_20->setText(tr("步长时间(s):"));
    ui->label_21->setText(tr("步数:"));

    ui->checkBox_7->setText(tr("相位正序"));

    ui->radioButton->setText(tr("手动"));
    ui->radioButton_2->setText(tr("触发"));
    ui->radioButton_3->setText(tr("自动"));
    ui->radioButton_4->setText(tr("幅值"));
    ui->radioButton_5->setText(tr("相位"));
    ui->radioButton_6->setText(tr("频率"));

    ui->groupBox->setTitle(tr("步长设置"));
    ui->groupBox_2->setTitle(tr("测试模式"));
    ui->groupBox_3->setTitle(tr("变量"));
    ui->groupBox_4->setTitle(tr("故障时间设置(s)"));
    ui->groupBox_5->setTitle(tr("变化相"));
    ui->groupBox_6->setTitle(tr("测试结果"));

    ui->label_12->setText(("<font color=blue>"+QString(tr("任意测试(2.2.1)"))+"</font>"));

}

void Manualtest::init_par()
{


    float SysFrec2=25;
    FILE *fp;
    if((fp=fopen("sysset.txt","a+"))==NULL)  //009-6-19
      return;
    fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SysFrec2,&SysVolt,&logic_delaytime,&PreFaultTime,&PostFaultTime,&dcouttype);
    SysFrec=SysFrec2*2;


    fclose(fp);
    exit_par=0;
    V1Value=SysVolt;//50;
    V1Phase=0;
    V1Frec=SysFrec;
    VariatyP=1;
    Variable=1;
    TestMode=1;

    VariatyV1=1;
    VariatyV2=0;
    VariatyV3=0;
    VariatyI1=1;
    VariatyI2=0;
    VariatyI3=0;

    ChangeValue=V1Value;
    V2Value=SysVolt;//57.73;
    V2Phase=-120;
    V2Frec=SysFrec;

    V3Value=SysVolt;
    V3Phase=120;
    V3Frec=SysFrec;

    VzValue=SysVolt;
    VzPhase=0;
    VzFrec=SysFrec;

    VariatyVz=0;
    VariatyVz2=0;



       I1Value=3;
       I1Phase=0;
       I1Frec=SysFrec;

       I2Value=0;
       I2Phase=-120;
       I2Frec=SysFrec;

       I3Value=0;
       I3Phase=120;
       I3Frec=SysFrec;


       CurStep=0.5;
       VolStep=1;
       PhaStep=1;
       FreStep=1;

       FaultTime=5;

       StepTime=0.5;
       StepNum=20;
       if(SysFrec==0)
       {
       V1DC=1;
       V2DC=1;
       V3DC=1;
       VzDC=1;

       I1DC=1;
       I2DC=1;
       I3DC=1;
       }
       else
       {
           V1DC=0;
           V2DC=0;
           V3DC=0;
           VzDC=0;

           I1DC=0;
           I2DC=0;
           I3DC=0;
       }

      }


void Manualtest::initalldata()
{

  ui->lineEdit->setText(QString::number(V1Value,'g',4));
  ui->lineEdit_2->setText(QString::number(V1Phase,'g',4));
  ui->lineEdit_3->setText(QString::number(V1Frec,'g',4));

  ui->lineEdit_4->setText(QString::number(V2Value,'g',4));
  ui->lineEdit_5->setText(QString::number(V2Phase,'g',4));
  ui->lineEdit_6->setText(QString::number(V2Frec,'g',4));

  ui->lineEdit_7->setText(QString::number(V3Value,'g',4));
  ui->lineEdit_8->setText(QString::number(V3Phase,'g',4));
  ui->lineEdit_9->setText(QString::number(V3Frec,'g',4));



  ui->lineEdit_10->setText(QString::number(I1Value,'g',4));
  ui->lineEdit_11->setText(QString::number(I1Phase,'g',4));
  ui->lineEdit_12->setText(QString::number(I1Frec,'g',4));

  ui->lineEdit_13->setText(QString::number(I2Value,'g',4));
  ui->lineEdit_14->setText(QString::number(I2Phase,'g',4));
  ui->lineEdit_15->setText(QString::number(I2Frec,'g',4));

  ui->lineEdit_16->setText(QString::number(I3Value,'g',4));
  ui->lineEdit_17->setText(QString::number(I3Phase,'g',4));
  ui->lineEdit_18->setText(QString::number(I3Frec,'g',4));


  ui->lineEdit_29->setText(QString::number(VzValue,'g',4));
  ui->lineEdit_30->setText(QString::number(VzPhase,'g',4));
  ui->lineEdit_31->setText(QString::number(VzFrec,'g',4));



  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
  ui->lineEdit_20->setText(QString::number(VolStep,'g',4));
  ui->lineEdit_21->setText(QString::number(PhaStep,'g',4));
  ui->lineEdit_22->setText(QString::number(FreStep,'g',4));
  ui->lineEdit_23->setText(QString::number(StepTime,'g',4));
  ui->lineEdit_24->setText(QString::number(StepNum,'g',4));

  ui->lineEdit_25->setText(QString::number(PreFaultTime,'g',4));
  ui->lineEdit_26->setText(QString::number(FaultTime,'g',4));
  ui->lineEdit_27->setText(QString::number(PostFaultTime,'g',4));
  ui->lineEdit_28->setText(QString::number(faultduration,'g',4));

  ui->lineEdit_29->setText(QString::number(VzValue,'g',4));
  ui->lineEdit_30->setText(QString::number(VzPhase,'g',4));
  ui->lineEdit_31->setText(QString::number(VzFrec,'g',4));

}


void Manualtest::display_mode(int mode)
{
  switch(mode)
{
   case 1:
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
        ui->groupBox_4->setEnabled(false);
        break;
   case 2:
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(true);
        ui->radioButton_3->setChecked(false);
        ui->groupBox_4->setEnabled(true);
        break;

   case 3:
      ui->radioButton->setChecked(false);
      ui->radioButton_2->setChecked(false);
      ui->radioButton_3->setChecked(true);
      ui->groupBox_4->setEnabled(false);
      break;
   default:break;
}



}


void Manualtest::display_variry(int variry)
{

    switch(variry)
  { case 1:
        ui->radioButton_4->setChecked(true);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(false);
        break;
   case 2:
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(true);
        ui->radioButton_6->setChecked(false);
        break;
   case 3:
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(true);
        break;
   default:break;
  }

}


void Manualtest::showdspstate(int state)
{




    switch(state)
    {
    case 0: ui->label_10->setText(tr("运行正常")); break;
    case 1: ui->label_10->setText(tr("IA 过载")); break;
    case 2: ui->label_10->setText(tr("IABC 温度保护")); break;
    case 3: ui->label_10->setText(tr("IC 间歇")); break;
    case 4: ui->label_10->setText(tr("IB 过载")); break;
    case 5: ui->label_10->setText(tr("IC 闭锁")); break;
    case 6: ui->label_10->setText(tr("IA 间歇")); break;
    case 7: ui->label_10->setText(tr("IC 过载")); break;
    case 8: ui->label_10->setText(tr("IABC 电源保护")); break;
    case 9: ui->label_10->setText(tr("IB 间歇")); break;
    case 10: ui->label_10->setText(tr("VA 过载")); break;

    case 12: ui->label_10->setText(tr("VC 间歇")); break;
    case 13: ui->label_10->setText(tr("VB 过载")); break;
    case 14: ui->label_10->setText(tr("VABC 温度保护")); break;
    case 15: ui->label_10->setText(tr("VA 间歇")); break;
    case 16: ui->label_10->setText(tr("VC 过载")); break;
    case 17: ui->label_10->setText(tr("VABC 电源保护")); break;
    case 18: ui->label_10->setText(tr("Vz 过载")); break;
    case 19: ui->label_10->setText(tr("直流过载")); break;
    case 20: ui->label_10->setText(tr("IABC 电源保护")); break;
    case 21: ui->label_10->setText(tr("VC 间歇")); break;
    }
    QPixmap img(":/picture/hei.png");
    QPixmap pic(":/picture/hong.png");
    switch(state)
    {
    case 100:
        ui->label_23->setPixmap(img);
        ui->label_24->setPixmap(img);
        ui->label_25->setPixmap(img);
        ui->label_26->setPixmap(img);
        break;
    case 101: ui->label_23->setPixmap(pic); break;
    case 102: ui->label_24->setPixmap(pic); break;
    case 103: ui->label_25->setPixmap(pic); break;
    case 104: ui->label_26->setPixmap(pic); break;
    case 111: ui->label_23->setPixmap(img); break;
    case 112: ui->label_24->setPixmap(img); break;
    case 113: ui->label_25->setPixmap(img); break;
    case 114: ui->label_26->setPixmap(img); break;
    }

}


float Manualtest::showPhase(float value)//显示结果，相位值的处理
{
    int temp;
    static float ftemp=0;
    ftemp=value+SumStepnum*PhaStep;
    if(ftemp>360)
    {
        temp=(int)(ftemp/360);
        ftemp=ftemp-360*temp;
    }
    else if(value<-360)
    {
        temp=(int)(ftemp/360);
        ftemp=ftemp+360*temp;
    }
    return ftemp;
}

void Manualtest::showtestresult(bool k)
    {
       if(k==false)
       {

        if(TestMode==2)
       {
          if(TripTime==-1)
          ui->textBrowser->setText(tr("动作时间(s:):未动作"));
          else
         {
           ui->textBrowser->setText(tr("动作时间(s:):")+QString::number(TripTime));
         }
       }

        else if(TestMode==3)
        {
            if(TripTime==-1)
             ui->textBrowser->setText(tr("动作值:未动作"));
            else
          {

            ui->textBrowser->setText(tr("动作值:"));
            switch(Variable)
            {
            case 1:  //幅值
                if(VariatyV1==1)
                {
                 ui->textBrowser->append(tr("VA:")+QString::number(V1Value+SumStepnum*VolStep,'g',6)+tr(" V"));
                }
                if(VariatyV2==1)
                {
                 ui->textBrowser->append(tr("VB:")+QString::number(V2Value+SumStepnum*VolStep,'g',6)+tr(" V"));
                }
                if(VariatyV3==1)
                {
                 ui->textBrowser->append(tr("VC:")+QString::number(V3Value+SumStepnum*VolStep,'g',6)+tr(" V"));
                }

                if(VariatyVz2==0&&VariatyVz==1)
                  ui->textBrowser->append(tr("Vz:")+QString::number(VzValue+SumStepnum*VolStep,'g',6)+tr(" V"));




                if(VariatyI1==1)
                {
                 ui->textBrowser->append(tr("IA:")+QString::number(I1Value+SumStepnum*CurStep,'g',6)+tr(" A"));
                }
                if(VariatyI2==1)
                {
                 ui->textBrowser->append(tr("IB:")+QString::number(I2Value+SumStepnum*CurStep,'g',6)+tr(" A"));
                }
                if(VariatyI3==1)
                {
                 ui->textBrowser->append(tr("IC:")+QString::number(I3Value+SumStepnum*CurStep,'g',6)+tr(" A"));
                }

                break;
            case 2: // 相位
                float value;
                if(VariatyV1==1)
                {
                    value=showPhase(V1Phase);
                 ui->textBrowser->append(tr("VA:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                if(VariatyV2==1)
                {
                    value=showPhase(V2Phase);
                 ui->textBrowser->append(tr("VB:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                if(VariatyV3==1)
                {    value=showPhase(V3Phase);
                 ui->textBrowser->append(tr("VC:")+QString::number(value,'g',6)+tr(" Deg"));
                }


                if(VariatyVz2==0&&VariatyVz==1)
                {    value=showPhase(VzPhase);
                  ui->textBrowser->append(tr("Vz:")+QString::number(value,'g',6)+tr(" Deg"));
                }

                if(VariatyI1==1)
                {    value=showPhase(I1Phase);
                 ui->textBrowser->append(tr("IA:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                if(VariatyI2==1)
                {value=showPhase(I2Phase);
                 ui->textBrowser->append(tr("IB:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                if(VariatyI3==1)
                {value=showPhase(I3Phase);
                 ui->textBrowser->append(tr("IC:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                break;


            case 3: //频率
                if(VariatyV1==1)
                {
                 ui->textBrowser->append(tr("VA:")+QString::number(V1Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                }
                if(VariatyV2==1)
                {
                 ui->textBrowser->append(tr("VB:")+QString::number(V2Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                }
                if(VariatyV3==1)
                {
                 ui->textBrowser->append(tr("VC:")+QString::number(V3Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                }

                if(VariatyVz2==0&&VariatyVz==1)
                  ui->textBrowser->append(tr("Vz:")+QString::number(VzFrec+SumStepnum*FreStep,'g',6)+tr(" Hz"));



                if(VariatyI1==1)
                {
                 ui->textBrowser->append(tr("IA:")+QString::number(I1Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                }
                if(VariatyI2==1)
                {
                 ui->textBrowser->append(tr("IB:")+QString::number(I2Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                }
                if(VariatyI3==1)
                {
                 ui->textBrowser->append(tr("IC:")+QString::number(I3Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                }
                break;
            }


          }


        }

        else if(TestMode==1)
        {

          ui->textBrowser->setText(tr("动作值:"));
          switch(Variable)
          {
          case 1:  //幅值
              if(VariatyV1==1)
              {
               ui->textBrowser->append(tr("VA:")+QString::number(V1Value,'g',6)+tr(" V"));
              }
              if(VariatyV2==1)
              {
               ui->textBrowser->append(tr("VB:")+QString::number(V2Value,'g',6)+tr(" V"));
              }
              if(VariatyV3==1)
              {
               ui->textBrowser->append(tr("VC:")+QString::number(V3Value,'g',6)+tr(" V"));
              }
              if(VariatyVz==1)
               ui->textBrowser->append(tr("Vz:")+QString::number(VzValue,'g',6)+tr(" V"));





              if(VariatyI1==1)
              {
               ui->textBrowser->append(tr("IA:")+QString::number(I1Value,'g',6)+tr(" A"));
              }
              if(VariatyI2==1)
              {
               ui->textBrowser->append(tr("IB:")+QString::number(I2Value,'g',6)+tr(" A"));
              }
              if(VariatyI3==1)
              {
               ui->textBrowser->append(tr("IC:")+QString::number(I3Value,'g',6)+tr(" A"));
              }

              break;
          case 2: // 相位
              if(VariatyV1==1)
              {
               ui->textBrowser->append(tr("VA:")+QString::number(V1Phase,'g',6)+tr(" Deg"));
              }
              if(VariatyV2==1)
              {
               ui->textBrowser->append(tr("VB:")+QString::number(V2Phase,'g',6)+tr(" Deg"));
              }
              if(VariatyV3==1)
              {
               ui->textBrowser->append(tr("VC:")+QString::number(V3Phase,'g',6)+tr(" Deg"));
              }

              if(VariatyVz==1)
               ui->textBrowser->append(tr("Vz:")+QString::number(VzPhase,'g',6)+tr(" Deg"));


              if(VariatyI1==1)
              {
               ui->textBrowser->append(tr("IA:")+QString::number(I1Phase,'g',6)+tr(" Deg"));
              }
              if(VariatyI2==1)
              {
               ui->textBrowser->append(tr("IB:")+QString::number(I2Phase,'g',6)+tr(" Deg"));
              }
              if(VariatyI3==1)
              {
               ui->textBrowser->append(tr("IC:")+QString::number(I3Phase,'g',6)+tr(" Deg"));
              }
              break;


          case 3: //频率
              if(VariatyV1==1)
              {
               ui->textBrowser->append(tr("VA:")+QString::number(V1Frec,'g',6)+tr(" Hz"));
              }
              if(VariatyV2==1)
              {
               ui->textBrowser->append(tr("VB:")+QString::number(V2Frec,'g',6)+tr(" Hz"));
              }
              if(VariatyV3==1)
              {
               ui->textBrowser->append(tr("VC:")+QString::number(V3Frec,'g',6)+tr(" Hz"));
              }

              if(VariatyVz==1)
               ui->textBrowser->append(tr("Vz:")+QString::number(VzFrec,'g',6)+tr(" Hz"));


              if(VariatyI1==1)
              {
               ui->textBrowser->append(tr("IA:")+QString::number(I1Frec,'g',6)+tr(" Hz"));
              }
              if(VariatyI2==1)
              {
               ui->textBrowser->append(tr("IB:")+QString::number(I2Frec,'g',6)+tr(" Hz"));
              }
              if(VariatyI3==1)
              {
               ui->textBrowser->append(tr("IC:")+QString::number(I3Frec,'g',6)+tr(" Hz"));
              }
              break;
          }

        }



    }

       else ui->textBrowser->clear();
    }


void Manualtest::setVanocheck()
{
ui->checkBox->setChecked(false);

}


void Manualtest::vi_parameter_enter(int position)
{

       switch(position)
        {
        case 1: //V1鍙傛暟
           switch(V1DC)
           {
             case 0:  //VA  交流
               if(V1Value>MAX_V_VALUEDATA_AC)
                 V1Value=MAX_V_VALUEDATA_AC;
               else if(V1Value<0)
                 V1Value=0;
               break;
             case 1:  //VA  直流
               if(V1Value>MAX_V_VALUEDATA_DC)
                 V1Value=MAX_V_VALUEDATA_DC;
               else if(V1Value<MIN_V_VALUEDATA_DC)
                 V1Value=MIN_V_VALUEDATA_DC;
               break;
           }


         break;
        case 2:
           if(V1Phase>180)
               V1Phase=V1Phase-360;
           else if(V1Phase<-180)
              V1Phase=(360+V1Phase);
           if(Phasesymmetry)
           {
            V2Phase=V1Phase-120;
            V3Phase=V1Phase+120;
            if(V2Phase>180)
                V2Phase=V2Phase-360;
            else if(V2Phase<-180)
                V2Phase+=360;
            if(V3Phase>180)
                V3Phase=V3Phase-360;
            else if(V3Phase<-180)
                V3Phase+=360;

           }
           break;
        case 3:
           if(V1Frec>1000)
             V1Frec=1000;
           else if(V1Frec<0)
             V1Frec=0;
           if(V1Frec==0)
             {
               V1DC=1;
               if(V1Value<MIN_V_VALUEDATA_DC)
               {
                   V1Value=MIN_V_VALUEDATA_DC;
                   ui->lineEdit->setText(QString::number(V1Value,'g',4));
               }
               else if(V1Value>MAX_V_VALUEDATA_DC)
               {
                   V1Value=MAX_V_VALUEDATA_DC;
                   ui->lineEdit->setText(QString::number(V1Value,'g',4));
               }
             }
           else
             {
               V1DC=0;
               if(V1Value<MIN_V_VALUEDATA_AC)
               {
                   V1Value=MIN_V_VALUEDATA_AC;
                   ui->lineEdit->setText(QString::number(V1Value,'g',4));
               }
               else if(V1Value>MAX_V_VALUEDATA_AC)
               {
                   V1Value=MAX_V_VALUEDATA_AC;
                   ui->lineEdit->setText(QString::number(V1Value,'g',4));
               }
             }
            break;
      case 4:       //VB
           switch(V2DC)
           {
             case 0:  //VB  交流
               if(V2Value>MAX_V_VALUEDATA_AC)
                 V2Value=MAX_V_VALUEDATA_AC;
               else if(V2Value<0)
                 V2Value=0;
               break;
             case 1:  //VB  直流
               if(V2Value>MAX_V_VALUEDATA_DC)
                 V2Value=MAX_V_VALUEDATA_DC;
               else if(V2Value<MIN_V_VALUEDATA_DC)
                 V2Value=MIN_V_VALUEDATA_DC;
               break;
           }
           //鏇存柊鏄剧ず骞呭€?
          break;
       case 5:
           //V2鐩镐綅
          if(V2Phase>180)
          V2Phase=V2Phase-360;
          else if(V2Phase<-180)
          V2Phase+=360;

          if(Phasesymmetry)
          {
              V1Phase=V2Phase+120;
              if(V1Phase>180)
                  V1Phase-=360;
              else if(V1Phase<-180)
                  V1Phase+=360;

              V3Phase=V1Phase+120;
              if(V3Phase>180)
                  V3Phase-=360;
              else if(V3Phase<-180)
                  V3Phase+=360;
          }
           break;
       case 6:   //
             if(V2Frec>1000)
              V2Frec=1000;
            else if(V2Frec<0)
              V2Frec=0;
            if(V2Frec==0)   //濡傛灉棰戠巼涓?锛屽垯璇ヤ俊鍙蜂负鐩存祦锛屽惁鍒欎负浜ゆ祦銆?
              {
                V2DC=1;
                if(V2Value<MIN_V_VALUEDATA_DC)
                {
                    V2Value=MIN_V_VALUEDATA_DC;
                    ui->lineEdit_4->setText(QString::number(V2Value,'g',4));
                }
                else if(V2Value>MAX_V_VALUEDATA_DC)
                {
                    V2Value=MAX_V_VALUEDATA_DC;
                    ui->lineEdit_4->setText(QString::number(V2Value,'g',4));
                }
              }
            else
              {
                V2DC=0;
                if(V2Value<MIN_V_VALUEDATA_AC)
                {
                    V2Value=MIN_V_VALUEDATA_AC;
                    ui->lineEdit_4->setText(QString::number(V2Value,'g',4));
                }
                else if(V2Value>MAX_V_VALUEDATA_AC)
                {
                    V2Value=MAX_V_VALUEDATA_AC;
                    ui->lineEdit_4->setText(QString::number(V2Value,'g',4));
                }
              }
            break;
        case 7:      //V3骞呭€?
            switch(V3DC)
              {
                case 0:  //V3涓轰氦娴?
                  if(V3Value>MAX_V_VALUEDATA_AC)
                    V3Value=MAX_V_VALUEDATA_AC;
                  else if(V3Value<0)
                    V3Value=0;
                  break;
                case 1:  //V3涓虹洿娴?
                  if(V3Value>MAX_V_VALUEDATA_DC)
                    V3Value=MAX_V_VALUEDATA_DC;
                  else if(V3Value<MIN_V_VALUEDATA_DC)
                    V3Value=MIN_V_VALUEDATA_DC;
                  break;
              }
             //鏇存柊鏄剧ず V3骞呭€?
            break;
        case 8:   //V3鐩镐綅
            if(V3Phase>180)
              V3Phase-=360;
            else if(V3Phase<-180)
              V3Phase+=360;
            if(Phasesymmetry)
            {
                V1Phase=V3Phase-120;
                if(V1Phase>180)
                    V1Phase-=360;
                else if(V1Phase<-180)
                    V1Phase+=360;
                V2Phase=V1Phase-120;
                if(V2Phase>180)
                    V2Phase-=360;
                else if(V2Phase<-180)
                    V2Phase+=360;
            }


            break;

       case 9: //V3棰戠巼
            if(V3Frec>1000)
                 V3Frec=1000;
            else if(V3Frec<0)
                 V3Frec=0;
            if(V3Frec==0)   //濡傛灉棰戠巼涓?锛屽垯璇ヤ俊鍙蜂负鐩存祦锛屽惁鍒欎负浜ゆ祦銆?
            {

                V3DC=1;
                if(V3Value<MIN_V_VALUEDATA_DC)
                {
                    V3Value=MIN_V_VALUEDATA_DC;
                    ui->lineEdit_7->setText(QString::number(V3Value,'g',4));
                }
                else if(V3Value>MAX_V_VALUEDATA_DC)
                {
                    V3Value=MAX_V_VALUEDATA_DC;
                    ui->lineEdit_7->setText(QString::number(V3Value,'g',4));
                }
             }
           else
             {
                 V3DC=0;
                 if(V3Value<MIN_V_VALUEDATA_AC)
                 {
                     V3Value=MIN_V_VALUEDATA_AC;
                     ui->lineEdit_7->setText(QString::number(V3Value,'g',4));
                 }
                 else if(V3Value>MAX_V_VALUEDATA_AC)
                 {
                     V3Value=MAX_V_VALUEDATA_AC;
                     ui->lineEdit_7->setText(QString::number(V3Value,'g',4));
                 }
             }



              break;

case 10:           //I1骞呭€?
            switch(I1DC)
              {
                case 0:  //I1交流
                  if(I1Value>MAX_I_VALUEDATA_AC)
                    I1Value=MAX_I_VALUEDATA_AC;
                  else if(I1Value<0)
                    I1Value=0;
                  break;
                case 1:  //V1直流
                  if(I1Value>MAX_I_VALUEDATA_DC)
                    I1Value=MAX_I_VALUEDATA_DC;
                  else if(I1Value<MIN_I_VALUEDATA_DC)
                    I1Value=MIN_I_VALUEDATA_DC;
                  break;
              }

            break;

case 11:  //I1鐩镐綅
          if(I1Phase>180)
            I1Phase-=360;
          else if(I1Phase<-180)
            I1Phase+=360;


          if(Phasesymmetry)
          {
           I2Phase=I1Phase-120;
           I3Phase=I1Phase+120;
           if(I2Phase>180)
               I2Phase=I2Phase-360;
           else if(I2Phase<-180)
               I2Phase+=360;
           if(I3Phase>180)
               I3Phase=I3Phase-360;
           else if(I3Phase<-180)
               I3Phase+=360;

          }





          break;
case 12:  //I1棰戠巼
              if(I1Frec>1000)
                I1Frec=1000;
              else if(I1Frec<-1000)
               I1Frec=-1000;
           if(I1Frec==0)   //是否直流
            {
              I1DC=1;//直流
              if(I1Value<MIN_I_VALUEDATA_DC)
              {
                  I1Value=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_10->setText(QString::number(I1Value,'g',4));
              }
              else if(I1Value>MAX_I_VALUEDATA_DC)
              {
                  I1Value=MAX_I_VALUEDATA_DC;
                  ui->lineEdit_10->setText(QString::number(I1Value,'g',4));
              }
              if(CurStep<MIN_I_VALUEDATA_DC)
              {
                  CurStep=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
              else if(CurStep>30/*MAX_I_VALUEDATA_DC*/)
              {
                  CurStep=30/*MAX_I_VALUEDATA_DC*/;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
            }
          else
            {
              I1DC=0;//交流
              if(I1Value<MIN_I_VALUEDATA_AC)
              {
                  I1Value=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_10->setText(QString::number(I1Value,'g',4));
              }
              else if(I1Value>MAX_I_VALUEDATA_AC)
              {
                  I1Value=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_10->setText(QString::number(I1Value,'g',4));
              }
              if(CurStep<MIN_I_VALUEDATA_AC)
              {
                  CurStep=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
              else if(CurStep>MAX_I_VALUEDATA_AC)
              {
                  CurStep=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
            }
          break;


 case 13:    //I2骞呭€?
          switch(I2DC)
            {
              case 0:  //I2涓轰氦娴?
                if(I2Value>MAX_I_VALUEDATA_AC)
                  I2Value=MAX_I_VALUEDATA_AC;
                else if(I2Value<0)
                  I2Value=0;
                break;
              case 1:  //V1涓虹洿娴?
                if(I2Value>MAX_I_VALUEDATA_DC)
                  I2Value=MAX_I_VALUEDATA_DC;
                else if(I2Value<MIN_I_VALUEDATA_DC)
                  I2Value=MIN_I_VALUEDATA_DC;
                break;
            }


          break;
 case 14:   //I2鐩镐綅
          if(I2Phase>180)
            I2Phase-=360;
          else if(I2Phase<-180)
            I2Phase+=360;

          if(Phasesymmetry)
          {
              I1Phase=I2Phase+120;
              if(I1Phase>180)
                  I1Phase-=360;
              else if(I1Phase<-180)
                  I1Phase+=360;

              I3Phase=I1Phase+120;
              if(I3Phase>180)
                  I3Phase-=360;
              else if(I3Phase<-180)
                  I3Phase+=360;
          }



          //鏇存柊鏄剧ずI2鐩镐綅
          break;
case 15:  //I2棰戠巼
          if(I2Frec>1000)
            I2Frec=1000;
          else if(I2Frec<0)
           I2Frec=0;
          if(I2Frec==0)   //濡傛灉棰戠巼涓?锛屽垯璇ヤ俊鍙蜂负鐩存祦锛屽惁鍒欎负浜ゆ祦銆?
            {
              I2DC=1;
              if(I2Value<MIN_I_VALUEDATA_DC)
              {
                  I2Value=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_13->setText(QString::number(I2Value,'g',4));
              }
              else if(I2Value>MAX_I_VALUEDATA_DC)
              {
                  I2Value=MAX_I_VALUEDATA_DC;
                  ui->lineEdit_13->setText(QString::number(I2Value,'g',4));
              }
              if(CurStep<MIN_I_VALUEDATA_DC)
              {
                  CurStep=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
              else if(CurStep>30/*MAX_I_VALUEDATA_DC*/)
              {
                  CurStep=30/*MAX_I_VALUEDATA_DC*/;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
            }
          else
            {
              I2DC=0;
              if(I2Value<MIN_I_VALUEDATA_AC)
              {
                  I2Value=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_13->setText(QString::number(I2Value,'g',4));
              }
              else if(I2Value>MAX_I_VALUEDATA_AC)
              {
                  I2Value=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_13->setText(QString::number(I2Value,'g',4));
              }
              if(CurStep<MIN_I_VALUEDATA_AC)
              {
                  CurStep=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
              else if(CurStep>MAX_I_VALUEDATA_AC)
              {
                  CurStep=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
            }

          break;
  case 16:   //I3骞呭€?
          switch(I3DC)
            {
              case 0:  //I3涓轰氦娴?
                if(I3Value>MAX_I_VALUEDATA_AC)
                  I3Value=MAX_I_VALUEDATA_AC;
                else if(I3Value<0)
                  I3Value=0;
                break;
              case 1:  //V1涓虹洿娴?
                if(I3Value>MAX_I_VALUEDATA_DC)
                  I3Value=MAX_I_VALUEDATA_DC;
                else if(I3Value<MIN_I_VALUEDATA_DC)
                  I3Value=MIN_I_VALUEDATA_DC;
                break;
            }
          //鏇存柊鏄剧ずI3骞呭€?
          break;
 case 17:  //I3鐩镐綅
          if(I3Phase>180)
            I3Phase-=360;
          else if(I3Phase<-180)
            I3Phase+=360;
          if(Phasesymmetry)
          {
              I1Phase=I3Phase-120;
              if(I1Phase>180)
                  I1Phase-=360;
              else if(I1Phase<-180)
                  I1Phase+=360;
              I2Phase=I1Phase-120;
              if(I2Phase>180)
                  I2Phase-=360;
              else if(I2Phase<-180)
                  I2Phase+=360;
          }

          //鏇存柊鏄剧ず
          break;
 case 18:  //I3棰戠巼
          if(I3Frec>1000)
            I3Frec=1000;
          else if(I3Frec<0)
            I3Frec=0;
          if(I3Frec==0)   //濡傛灉棰戠巼涓?锛屽垯璇ヤ俊鍙蜂负鐩存祦锛屽惁鍒欎负浜ゆ祦銆?
            {

              I3DC=1;
              if(I3Value<MIN_I_VALUEDATA_DC)
              {
                  I3Value=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_16->setText(QString::number(I3Value,'g',4));
              }
              else if(I3Value>MAX_I_VALUEDATA_DC)
              {
                  I3Value=MAX_I_VALUEDATA_DC;
                  ui->lineEdit_16->setText(QString::number(I3Value,'g',4));
              }
              if(CurStep<MIN_I_VALUEDATA_DC)
              {
                  CurStep=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
              else if(CurStep>30/*MAX_I_VALUEDATA_DC*/)
              {
                  CurStep=30/*MAX_I_VALUEDATA_DC*/;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }

            }
          else
            {

              I3DC=0;
              if(I3Value<MIN_I_VALUEDATA_AC)
              {
                  I3Value=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_16->setText(QString::number(I3Value,'g',4));
              }
              else if(I3Value>MAX_I_VALUEDATA_AC)
              {
                  I3Value=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_16->setText(QString::number(I3Value,'g',4));
              }
              if(CurStep<MIN_I_VALUEDATA_AC)
              {
                  CurStep=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }
              else if(CurStep>MAX_I_VALUEDATA_AC)
              {
                  CurStep=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
              }

            }
          break;

    case 19:
           if(TestMode==1)
           {
           switch(I1DC)
             {
               case 0:  //交流
                 if(CurStep>MAX_I_VALUEDATA_AC)
                   CurStep=MAX_I_VALUEDATA_AC;
                 else if(CurStep<0)
                   CurStep=0;
                 break;
               case 1:  //鐩存祦鏃剁數娴佹闀块檺瀹氬€?
                 if(CurStep>30/*MAX_I_VALUEDATA_DC*/)
                   CurStep=30/*MAX_I_VALUEDATA_DC*/;
                 else if(CurStep<0)
                   CurStep=0;
                 break;
             }
           }
           else if(TestMode==3)
           {
               switch(I1DC)
                 {
                   case 0:  //交流
                     if(CurStep>MAX_I_VALUEDATA_AC)
                       CurStep=MAX_I_VALUEDATA_AC;
                     else if(CurStep<0-MAX_I_VALUEDATA_AC)
                       CurStep=0-MAX_I_VALUEDATA_AC;
                     break;
                   case 1:  //鐩存祦鏃剁數娴佹闀块檺瀹氬€?
                     if(CurStep>MAX_I_VALUEDATA_DC)
                       CurStep=MAX_I_VALUEDATA_DC;
                     else if(CurStep<0-MAX_I_VALUEDATA_DC)
                       CurStep=0-MAX_I_VALUEDATA_DC;
                     break;
                 }
           }
            break;

    case 20:
           if(TestMode==1)
           {
            switch(I1DC)
            {
            case 0:  //浜ゆ祦鏃剁數娴佹闀块檺瀹氬€?
              if(VolStep>MAX_V_VALUEDATA_AC)
                VolStep=MAX_V_VALUEDATA_AC;
              else if(VolStep<0)
                VolStep=0;
              break;
            case 1:  //鐩存祦鏃剁數娴佹闀块檺瀹氬€?
              if(VolStep>MAX_V_VALUEDATA_DC)
                VolStep=MAX_V_VALUEDATA_DC;
              else if(VolStep<0)
                VolStep=0;
              break;
            }
           }
           if(TestMode==3)
           {
            switch(I1DC)
            {
            case 0:  //浜ゆ祦鏃剁數娴佹闀块檺瀹氬€?
              if(VolStep>MAX_V_VALUEDATA_AC)
                VolStep=MAX_V_VALUEDATA_AC;
              else if(VolStep<0-MAX_V_VALUEDATA_AC)
                VolStep=0-MAX_V_VALUEDATA_AC;
              break;
            case 1:  //鐩存祦鏃剁數娴佹闀块檺瀹氬€?
              if(VolStep>MAX_V_VALUEDATA_DC)
                VolStep=MAX_V_VALUEDATA_DC;
              else if(VolStep<0-MAX_V_VALUEDATA_DC)
                VolStep=0-MAX_V_VALUEDATA_DC;
              break;
            }
           }
            //鏇存柊鏄剧ず鐢靛帇姝ラ暱
            break;
    case 21: //鐩镐綅姝ラ暱
           if(TestMode==1)
           {
                    if(PhaStep>180)
                      PhaStep=180;
                    else if(PhaStep<0)
                      PhaStep=0;
           }
           if(TestMode==3)
           {
                    if(PhaStep>180)
                      PhaStep=180;
                    else if(PhaStep<-180)
                      PhaStep=-180;
           }

                    //鏇存柊鏄剧ず姝ラ暱
                    break;

    case 22:   //棰戠巼姝ラ暱
           if(TestMode==1)
           {
                  if(FreStep>100)
                    FreStep=1;
                  else if(FreStep<0)
                    FreStep=0;
           }
           if(TestMode==3)
           {
                  if(FreStep>100)
                    FreStep=1;
                  else if(FreStep<-100)
                    FreStep=-1;
           }
                  //鏇存柊鏄剧ず棰戠巼姝ラ暱
                  break;

    case 23:
                             //姝ラ暱鏃堕棿
                             if(StepTime>99.99)
                               StepTime=99.99;
                             else if(StepTime<0)
                               StepTime=0.5;
                             //鏇存柊鐣岄潰鏄剧ず
                             break;
    case 24:
                             if(StepNum>1000)
                               StepNum=1000;
                             else if(StepNum<1)
                               StepNum=1;
                             //鏇存柊鐣岄潰鏄剧ず
                             break;
    case 25:
                             if(PreFaultTime>999.9)
                              PreFaultTime=999.9;
                             else if(PreFaultTime<0)
                             PreFaultTime=0;
                            //鏇存柊鐣岄潰鏄剧ず
                              break;
    case 26:
             if(FaultTime>999.9)
             FaultTime=999.9;
             else if(FaultTime<0)
             FaultTime=0;
             //鏇存柊鐣岄潰鏄剧ず
              break;

    case 27:
            if(PostFaultTime>999.9)
                PostFaultTime=999.9;
            else if(PostFaultTime<0)
                PostFaultTime=0;
                //鏇存柊鐣岄潰鏄剧ず
               break;
    case 28:
               if(faultduration>999.9)
                   faultduration=999.9;
               else if(faultduration<0)
                   faultduration=0;
                   //鏇存柊鐣岄潰鏄剧ず
                  break;

    case 29: //Vz参数
            switch(VzDC)
              {
                case 0:  //Vz为交流
                  if(VzValue>MAX_V_VALUEDATA_AC)
                    VzValue=MAX_V_VALUEDATA_AC;
                  else if(VzValue<0)
                    VzValue=0;
                  break;
                case 1:  //Vz为直流
                  if(VzValue>MAX_V_VALUEDATA_DC)
                    VzValue=MAX_V_VALUEDATA_DC;
                  else if(VzValue<MIN_V_VALUEDATA_DC)
                    VzValue=MIN_V_VALUEDATA_DC;
                  break;
              }
             break;
     case 30:  //Vz相位
          if(VzPhase>180)
            VzPhase=180;
          else if(VzPhase<-180)
            VzPhase=-180;
          break;
     case 31:  //Vz频率
          if(VzFrec>1000)
            VzFrec=1000;
          else if(VzFrec<0)
            VzFrec=0;
          if(VzFrec==0)
          {//如果频率为0，则该信号为直流，否则为交流。
              VzDC=1;
              if(VzValue<MIN_V_VALUEDATA_DC)
              {
                VzValue=MIN_V_VALUEDATA_DC;
                ui->lineEdit_29->setText(QString::number(VzValue,'g',4));
              }
              else if(VzValue>MAX_V_VALUEDATA_DC)
              {
                VzValue=MAX_V_VALUEDATA_DC;
                ui->lineEdit_29->setText(QString::number(VzValue,'g',4));
              }
           }
          else
           {
              VzDC=0;
              if(VzValue<MIN_V_VALUEDATA_AC)
              {
                VzValue=MIN_V_VALUEDATA_AC;
                ui->lineEdit_29->setText(QString::number(VzValue,'g',4));
              }
              else if(VzValue>MAX_V_VALUEDATA_AC)
              {
                VzValue=MAX_V_VALUEDATA_AC;
                ui->lineEdit_29->setText(QString::number(VzValue,'g',4));
              }
           }
          break;
          }

}




void Manualtest::displaydataandstate(int position)
{


    if(position>=1&&position<=31)
    {
        QLineEdit *lineedit=(QLineEdit *)lineeditlist.at(position-1);

        switch(position)
        {
        case 1:ui->lineEdit->setText(QString::number(V1Value,'g',6)); break;
        case 2:ui->lineEdit_2->setText(QString::number(V1Phase,'g',6)); break;
        case 3:ui->lineEdit_3->setText(QString::number(V1Frec,'g',6)); break;
        case 4:ui->lineEdit_4->setText(QString::number(V2Value,'g',6)); break;
        case 5:ui->lineEdit_5->setText(QString::number(V2Phase,'g',6)); break;
        case 6:ui->lineEdit_6->setText(QString::number(V2Frec,'g',6)); break;
        case 7:lineedit->setText(QString::number(V3Value,'g',6)); break;
        case 8:lineedit->setText(QString::number(V3Phase,'g',6)); break;
        case 9:lineedit->setText(QString::number(V3Frec,'g',6)); break;

        case 10:lineedit->setText(QString::number(I1Value,'g',6)); break;
        case 11:lineedit->setText(QString::number(I1Phase,'g',6)); break;
        case 12:lineedit->setText(QString::number(I1Frec,'g',6)); break;
        case 13:lineedit->setText(QString::number(I2Value,'g',6)); break;
        case 14:lineedit->setText(QString::number(I2Phase,'g',6)); break;
        case 15:lineedit->setText(QString::number(I2Frec,'g',6)); break;
        case 16:lineedit->setText(QString::number(I3Value,'g',6)); break;
        case 17:lineedit->setText(QString::number(I3Phase,'g',6)); break;
        case 18:lineedit->setText(QString::number(I3Frec,'g',6)); break;


        case 19:lineedit->setText(QString::number(CurStep,'g',6)); break;
        case 20:lineedit->setText(QString::number(VolStep,'g',6)); break;
        case 21:lineedit->setText(QString::number(PhaStep,'g',6)); break;
        case 22:lineedit->setText(QString::number(FreStep,'g',6)); break;
        case 23:lineedit->setText(QString::number(StepTime,'g',6)); break;
        case 24:lineedit->setText(QString::number(StepNum,'g',6)); break;
        case 25:lineedit->setText(QString::number(PreFaultTime,'g',6)); break;
        case 26:lineedit->setText(QString::number(FaultTime,'g',6)); break;
        case 27:lineedit->setText(QString::number(PostFaultTime,'g',6)); break;
        case 28:lineedit->setText(QString::number(faultduration,'g',6)); break;

        case 29:lineedit->setText(QString::number(VzValue,'g',6)); break;
        case 30:lineedit->setText(QString::number(VzPhase,'g',6)); break;
        case 31:lineedit->setText(QString::number(VzFrec,'g',6)); break;

        }

        return;
    }



}

 void  Manualtest::updateVz()
{

     if(V1Frec==SysFrec&&V2Frec==SysFrec&&V3Frec==SysFrec&&VariatyVz2==1)
     {
     float VzReal,VzImaginary;
     //实部
     VzReal=V1Value*cos(V1Phase*3.14159/180)+V2Value*cos(V2Phase*3.14159/180)+V3Value*cos(V3Phase*3.14159/180);
     //虚部
     VzImaginary=V1Value*sin(V1Phase*3.14159/180)+V2Value*sin(V2Phase*3.14159/180)+V3Value*sin(V3Phase*3.14159/180);
     VzValue=sqrt(VzReal*VzReal+VzImaginary*VzImaginary);
     if(VzReal>=-0.0002&&VzReal<=0.0002)
     {
         if(VzImaginary>=-0.0002&&VzImaginary<=0.0002)
             VzPhase=0;
         else if(VzImaginary>0)
             VzPhase=90;
         else
             VzPhase=-90;
     }
     else if(VzReal<0)
     {
         VzPhase=atan(VzImaginary/VzReal)*180/3.14159;
         VzPhase=VzPhase+180;//2016-4-28修改，VZPhase的改变
     }
     else
     {
        VzPhase=atan(VzImaginary/VzReal)*180/3.14159;
     }
     VzPhase=fmod(VzPhase,360);//VzPhase%360
     VzFrec=SysFrec;  

     if(VzValue>=-0.0002&&VzValue<=0.0002)
        VzValue=0;
     if(VzPhase>=-0.0002&&VzPhase<=0.0002)
        VzPhase=0;




     ui->lineEdit_29->setText(QString::number(VzValue,'g',4));
     ui->lineEdit_30->setText(QString::number(VzPhase,'g',4));
     ui->lineEdit_31->setText(QString::number(VzFrec,'g',4));
     }
     else
     {
         ui->checkBox_8->setChecked(false);
         VariatyVz2=0;
     }

 }

 void Manualtest::DisplayAutoChangeValue(int flag)
 {
     if(flag){
      switch(Variable)
        {
          case 1:
            if(VariatyV1==1&&flag)
                ui->lineEdit->setText(QString::number(ChangeV1Value,'g',4));
            if(VariatyV2==1&&flag)
                ui->lineEdit_4->setText(QString::number(ChangeV2Value,'g',4));
            if(VariatyV3==1&&flag)
              ui->lineEdit_7->setText(QString::number(ChangeV3Value,'g',4));
            //if(VariatyVz==1&&flag)
              //ui->lineEdit_29->setText(QString::number(ChangeVzValue,'g',4));

            if(VariatyI1==1&&flag)
              ui->lineEdit_10->setText(QString::number(ChangeI1Value,'g',4));
            if(VariatyI2==1&&flag)
              ui->lineEdit_13->setText(QString::number(ChangeI2Value,'g',4));
            if(VariatyI3==1&&flag)
              ui->lineEdit_16->setText(QString::number(ChangeI3Value,'g',4));
            break;

          case 2:

            V1RelaPhase=0;
            V2RelaPhase=0;
            V3RelaPhase=0;
            VzRelaPhase=0;
            I1RelaPhase=0;
            I2RelaPhase=0;
            I3RelaPhase=0;

            if(VariatyV1==1&&V1DC==0&&flag)
              ui->lineEdit_2->setText(QString::number(ChangeV1Phase,'g',4));
            if(VariatyV2==1&&V2DC==0&&flag)
              ui->lineEdit_5->setText(QString::number(ChangeV2Phase,'g',4));
            if(VariatyV3==1&&V3DC==0&&flag)
              ui->lineEdit_8->setText(QString::number(ChangeV3Phase,'g',4));

            //if(VariatyVz==1&&VzDC==0&&flag)
              //ui->lineEdit_30->setText(QString::number(ChangeVzPhase,'g',4));

            if(VariatyI1==1&&I1DC==0&&flag)
             ui->lineEdit_11->setText(QString::number(ChangeI1Phase,'g',4));
            if(VariatyI2==1&&I2DC==0&&flag)
              ui->lineEdit_14->setText(QString::number(ChangeI2Phase,'g',4));
            if(VariatyI3==1&&I3DC==0&&flag)
              ui->lineEdit_17->setText(QString::number(ChangeI3Phase,'g',4));
          break;

          case 3:
            if(VariatyV1==1&&V1DC==0&&flag)
              ui->lineEdit_3->setText(QString::number(ChangeV1Frec,'g',4));
            if(VariatyV2==1&&V2DC==0&&flag)
              ui->lineEdit_6->setText(QString::number(ChangeV2Frec,'g',4));
            if(VariatyV3==1&&V3DC==0&&flag)
              ui->lineEdit_9->setText(QString::number(ChangeV3Frec,'g',4));
            //if(VariatyVz==1&&VzDC==0&&flag)
              //ui->lineEdit_31->setText(QString::number(ChangeVzFrec,'g',4));

            if(VariatyI1==1&&I1DC==0&&flag)
              ui->lineEdit_12->setText(QString::number(ChangeI1Frec,'g',4));
            if(VariatyI2==1&&I2DC==0&&flag)
              ui->lineEdit_15->setText(QString::number(ChangeI2Frec,'g',4));
            if(VariatyI3==1&&I3DC==0&&flag)
              ui->lineEdit_18->setText(QString::number(ChangeI3Frec,'g',4));
            break;
        }
     }
     else
     {
         switch(Variable)
           {
             case 1:
               if(VariatyV1==1)
                   ui->lineEdit->setText(QString::number(V1Value,'g',4));
               if(VariatyV2==1)
                   ui->lineEdit_4->setText(QString::number(V2Value,'g',4));
               if(VariatyV3==1)
                   ui->lineEdit_7->setText(QString::number(V3Value,'g',4));
               //if(VariatyVz==1)
                   //ui->lineEdit_29->setText(QString::number(VzValue,'g',4));

               if(VariatyI1==1)
                   ui->lineEdit_10->setText(QString::number(I1Value,'g',4));
               if(VariatyI2==1)
                   ui->lineEdit_13->setText(QString::number(I2Value,'g',4));
               if(VariatyI3==1)
                   ui->lineEdit_16->setText(QString::number(I3Value,'g',4));
               break;

             case 2:

               V1RelaPhase=0;
               V2RelaPhase=0;
               V3RelaPhase=0;
               VzRelaPhase=0;
               I1RelaPhase=0;
               I2RelaPhase=0;
               I3RelaPhase=0;

               if(VariatyV1==1&&V1DC==0)
                 ui->lineEdit_2->setText(QString::number(V1Phase,'g',4));
               if(VariatyV2==1&&V2DC==0)
                ui->lineEdit_5->setText(QString::number(V2Phase,'g',4));
               if(VariatyV3==1&&V3DC==0)
                ui->lineEdit_8->setText(QString::number(V3Phase,'g',4));

               //if(VariatyVz==1&&VzDC==0)
                //ui->lineEdit_30->setText(QString::number(VzPhase,'g',4));

               if(VariatyI1==1&&I1DC==0)
                ui->lineEdit_11->setText(QString::number(I1Phase,'g',4));
               if(VariatyI2==1&&I2DC==0)
                ui->lineEdit_14->setText(QString::number(I2Phase,'g',4));
               if(VariatyI3==1&&I3DC==0)
                ui->lineEdit_17->setText(QString::number(I3Phase,'g',4));
             break;

             case 3:
               if(VariatyV1==1&&V1DC==0)
                 ui->lineEdit_3->setText(QString::number(V1Frec,'g',4));
               if(VariatyV2==1&&V2DC==0)
                   ui->lineEdit_6->setText(QString::number(V2Frec,'g',4));
               if(VariatyV3==1&&V3DC==0)
                   ui->lineEdit_9->setText(QString::number(V3Frec,'g',4));
               //if(VariatyVz==1&&VzDC==0)
                   //ui->lineEdit_31->setText(QString::number(VzFrec,'g',4));

               if(VariatyI1==1&&I1DC==0)
                   ui->lineEdit_12->setText(QString::number(I1Frec,'g',4));
               if(VariatyI2==1&&I2DC==0)
                   ui->lineEdit_15->setText(QString::number(I2Frec,'g',4));
               if(VariatyI3==1&&I3DC==0)
                   ui->lineEdit_18->setText(QString::number(I3Frec,'g',4));
               break;
           }

     }

 }

int Manualtest::vi_change_step(int add_or_dec,float devision)
  {
    if(IsSendcmd==true)
        return 0;

    IsSendcmd=true;
    float max_v_valuedata,min_v_valuedata;
    float max_i_valuedata,min_i_valuedata;
    int flag;
    if(V1DC==0||V2DC==0||V3DC==0||VzDC==0||I1DC==0||I2DC==0||I3DC==0)
      {//浜ゆ祦
        max_v_valuedata=MAX_V_VALUEDATA_AC;
        min_v_valuedata=0;
        max_i_valuedata=MAX_I_VALUEDATA_AC;
        min_i_valuedata=0;
      }
    else if(V1DC==1||V2DC==1||V3DC==1||VzDC==1||I1DC==1||I2DC==1||I3DC==1)
      {//鐩存祦
        max_v_valuedata=MAX_V_VALUEDATA_DC;
        min_v_valuedata=MIN_V_VALUEDATA_DC;
        max_i_valuedata=MAX_I_VALUEDATA_DC;
        min_i_valuedata=MIN_I_VALUEDATA_DC;
      }
    flag=0;
    if(add_or_dec==1)
      {
        switch(Variable)
          {
            case 1://幅值
              if(VariatyV1==1)//VA选择
                {
                  V1Value=V1Value+VolStep*devision;
                  switch(V1DC)
                  {
                    case 0:  //VA  交流
                      if(V1Value>MAX_V_VALUEDATA_AC)
                      {
                        V1Value=MAX_V_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(V1Value>MAX_V_VALUEDATA_DC)
                      {
                        V1Value=MAX_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(1);
                }
              if(VariatyV2==1)
                {
                  V2Value=V2Value+VolStep*devision;
                  switch(V2DC)
                  {
                    case 0:  //VA  交流
                      if(V2Value>MAX_V_VALUEDATA_AC)
                      {
                        V2Value=MAX_V_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(V2Value>MAX_V_VALUEDATA_DC)
                      {
                        V2Value=MAX_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                   displaydataandstate(4);
                }
              if(VariatyV3==1)
                {//V3瑕佸彉鍖?                  
                  V3Value=V3Value+VolStep*devision;
                  switch(V3DC)
                  {
                    case 0:  //VA  交流
                      if(V3Value>MAX_V_VALUEDATA_AC)
                      {
                        V3Value=MAX_V_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(V3Value>MAX_V_VALUEDATA_DC)
                      {
                        V3Value=MAX_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                   displaydataandstate(7); //V3鐨勫箙鍊?
                }

              if(VariatyVz==1)
                {//V3瑕佸彉鍖?
                  VzValue=VzValue+VolStep*devision;
                  switch(VzDC)
                  {
                    case 0:  //VA  交流
                      if(VzValue>MAX_V_VALUEDATA_AC)
                      {
                        VzValue=MAX_V_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(VzValue>MAX_V_VALUEDATA_DC)
                      {
                        VzValue=MAX_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                   displaydataandstate(29);
                }



              if(VariatyI1==1)
                {//I1瑕佸彉鍖?
                  I1Value=I1Value+CurStep*devision;
                  switch(I1DC)
                  {
                    case 0:  //IA  交流
                      if(I1Value>MAX_I_VALUEDATA_AC)
                      {
                        I1Value=MAX_I_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //IA  直流
                      if(I1Value>MAX_I_VALUEDATA_DC)
                      {
                        I1Value=MAX_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(10);  //I1鐨勫箙鍊?
                }
              if(VariatyI2==1)
                {//I2瑕佸彉鍖?
                  I2Value=I2Value+CurStep*devision;
                  switch(I2DC)
                  {
                    case 0:  //IA  交流
                      if(I2Value>MAX_I_VALUEDATA_AC)
                      {
                        I2Value=MAX_I_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //IA  直流
                      if(I2Value>MAX_I_VALUEDATA_DC)
                      {
                        I2Value=MAX_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                 displaydataandstate(13);  //I1鐨勫箙鍊?

                }
              if(VariatyI3==1)
                {//I3瑕佸彉鍖?
                  I3Value=I3Value+CurStep*devision;
                  switch(I3DC)
                  {
                    case 0:  //IA  交流
                      if(I3Value>MAX_I_VALUEDATA_AC)
                      {
                        I3Value=MAX_I_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //IA  直流
                      if(I3Value>MAX_I_VALUEDATA_DC)
                      {
                        I3Value=MAX_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                   displaydataandstate(16);  //I1鐨勫箙鍊?
                }
              break;
            case 2:

              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;

              if(VariatyV1==1&&V1DC==0)
                {
                  V1Phase=V1Phase+PhaStep*devision;
                  if(V1Phase>180)
                    {
                      V1Phase=V1Phase-PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        V1RelaPhase=PhaStep*devision;
                      else if(PhaStep<0)
                        V1RelaPhase=360+PhaStep*devision;
                  }
                  displaydataandstate(2);
                }
              else if(VariatyV1!=1&&V1DC==0)
                {//V1涓嶅彉鍖?
                  V1RelaPhase=0;
                }

              if(VariatyV2==1&&V2DC==0)
                {//V2瑕佸彉鍖?
                  V2Phase=V2Phase+PhaStep*devision;
                  if(V2Phase>180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V2Phase=V2Phase-PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        V2RelaPhase=PhaStep*devision;
                      else if(PhaStep<0)
                        V2RelaPhase=360+PhaStep*devision;
                  }
                  displaydataandstate(5);
                }

              else if(VariatyV2!=1&&V2DC==0)
                {//V2涓嶅彉鍖?
                  V2RelaPhase=0;
                }




              if(VariatyV3==1&&V3DC==0)
                {//V3瑕佸彉鍖?
                  V3Phase=V3Phase+PhaStep*devision;
                  if(V3Phase>180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V3Phase=V3Phase-PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        V3RelaPhase=PhaStep*devision;
                      else if(PhaStep<0)
                        V3RelaPhase=360+PhaStep*devision;
                  }
                 displaydataandstate(8);
                }
              else if(VariatyV3!=1&&V3DC==0)
                {
                  V3RelaPhase=0;
                }


              if(VariatyVz==1&&VzDC==0)
                {
                  VzPhase=VzPhase+PhaStep*devision;
                  if(VzPhase>180)
                    {
                      VzPhase=VzPhase-PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        VzRelaPhase=PhaStep*devision;
                      else if(PhaStep<0)
                        VzRelaPhase=360+PhaStep*devision;
                  }
                 displaydataandstate(30);
                }
              else if(VariatyVz!=1&&VzDC==0)
                {
                  VzRelaPhase=0;
                }

              if(VariatyI1==1&&I1DC==0)
                {//I1瑕佸彉鍖?
                  I1Phase=I1Phase+PhaStep*devision;
                  if(I1Phase>180)
                    {
                      I1Phase=I1Phase-PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        I1RelaPhase=PhaStep*devision;
                      else if(PhaStep<0)
                        I1RelaPhase=360+PhaStep*devision;
                  }
                  displaydataandstate(11);
                }
              else if(VariatyI1!=1&&I1DC==0)
                {
                  I1RelaPhase=0;
                }
              if(VariatyI2==1&&I2DC==0)
                {//I2瑕佸彉鍖?
                  I2Phase=I2Phase+PhaStep*devision;
                  if(I2Phase>180)
                    {
                      I2Phase=I2Phase-PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        I2RelaPhase=PhaStep*devision;
                      else if(PhaStep<0)
                        I2RelaPhase=360+PhaStep*devision;
                  }

                  displaydataandstate(14);
                }
              else if(VariatyI2!=1&&I2DC==0)
                {//I2涓嶅彉鍖?
                  I2RelaPhase=0;
                }
              if(VariatyI3==1&&I3DC==0)
                {//I3瑕佸彉鍖?
                  I3Phase=I3Phase+PhaStep*devision;

                  if(I3Phase>180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      I3Phase=I3Phase-PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        I3RelaPhase=PhaStep*devision;
                      else if(PhaStep<0)
                        I3RelaPhase=360+PhaStep*devision;
                  }
                  displaydataandstate(17);
                }
              else if(VariatyI3!=1&&I3DC==0)
                {
                  I3RelaPhase=0;
                }
              break;




            case 3:
              if(VariatyV1==1&&V1DC==0)
                {//V1瑕佸彉鍖?
                  V1Frec=V1Frec+FreStep*devision;
                  if(V1Frec>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V1Frec=V1Frec-FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(3);
                }
              if(VariatyV2==1&&V2DC==0)
                {//V2瑕佸彉鍖?
                  V2Frec=V2Frec+FreStep*devision;
                  if(V2Frec>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V2Frec=V2Frec-FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(6);
                }
              if(VariatyV3==1&&V3DC==0)
                {//V3瑕佸彉鍖?
                  V3Frec=V3Frec+FreStep*devision;
                  if(V3Frec>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V3Frec=V3Frec-FreStep*devision;
                      flag=1;
                    }

                  displaydataandstate(9);
                }

              if(VariatyVz==1&&VzDC==0)
                {//V3瑕佸彉鍖?
                  VzFrec=VzFrec+FreStep*devision;
                  if(VzFrec>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      VzFrec=VzFrec-FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(31);
                }




              if(VariatyI1==1&&I1DC==0)
                {//I1瑕佸彉鍖?
                  I1Frec=I1Frec+FreStep*devision;
                  if(I1Frec>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      I1Frec=I1Frec-FreStep*devision;
                      flag=1;
                    }

                  displaydataandstate(12);
                }
              if(VariatyI2==1&&I2DC==0)
                {//I2瑕佸彉鍖?
                  I2Frec=I2Frec+FreStep*devision;
                  if(I2Frec>1000)

                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      I2Frec=I2Frec-FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(15);
                }
              if(VariatyI3==1&&I3DC==0)
                {//I3瑕佸彉鍖?
                  I3Frec=I3Frec+FreStep*devision;
                  if(I3Frec>1000)

                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      I3Frec=I3Frec-FreStep*devision;

                      flag=1;
                    }
                   displaydataandstate(18);
                }

              break;
          }
      }

    else if(increase==0)
      {
        switch(Variable)
          {
            case 1://鍙橀噺鏄箙鍊?
              if(VariatyV1==1)
                {//V1瑕佸彉鍖?
                  V1Value=V1Value-VolStep*devision;
                  switch(V1DC)
                  {
                    case 0:  //VA  交流
                      if(V1Value<0)
                      {
                        V1Value=0;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(V1Value<MIN_V_VALUEDATA_DC)
                      {
                        V1Value=MIN_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(1);
                }
              if(VariatyV2==1)
                {//V2瑕佸彉鍖?
                  V2Value=V2Value-VolStep*devision;
                  switch(V2DC)
                  {
                    case 0:  //VB  交流
                      if(V2Value<0)
                      {
                        V2Value=0;
                        flag=1;
                      }
                      break;
                    case 1:  //VB  直流
                      if(V2Value<MIN_V_VALUEDATA_DC)
                      {
                        V2Value=MIN_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(4);
                }
              if(VariatyV3==1)
                {//V3瑕佸彉鍖?
                  V3Value=V3Value-VolStep*devision;
                  switch(V3DC)
                  {
                    case 0:  //VC  交流
                      if(V3Value<0)
                      {
                        V3Value=0;
                        flag=1;
                      }
                      break;
                    case 1:  //VC  直流
                      if(V3Value<MIN_V_VALUEDATA_DC)
                      {
                        V3Value=MIN_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                 displaydataandstate(7);
                }

              if(VariatyVz==1)
                {//V3瑕佸彉鍖?
                  VzValue=VzValue-VolStep*devision;
                  switch(VzDC)
                  {
                    case 0:  //VZ  交流
                      if(VzValue<0)
                      {
                        VzValue=0;
                        flag=1;
                      }
                      break;
                    case 1:  //VZ  直流
                      if(VzValue<MIN_V_VALUEDATA_DC)
                      {
                        VzValue=MIN_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                 displaydataandstate(29);
                }


              if(VariatyI1==1)
                {//I1瑕佸彉鍖?
                  I1Value=I1Value-CurStep*devision;
                  switch(I1DC)
                  {
                    case 0:  //IA  交流
                      if(I1Value<0)
                      {
                        I1Value=0;
                        flag=1;
                      }
                      break;
                    case 1:  //IA  直流
                      if(I1Value<MIN_I_VALUEDATA_DC)
                      {
                        I1Value=MIN_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                     displaydataandstate(10);
                }
              if(VariatyI2==1)
                {//I2瑕佸彉鍖?
                  I2Value=I2Value-CurStep*devision;
                  switch(I2DC)
                  {
                    case 0:  //IB  交流
                      if(I2Value<0)
                      {
                        I2Value=0;
                        flag=1;
                      }
                      break;
                    case 1:  //IB  直流
                      if(I2Value<MIN_I_VALUEDATA_DC)
                      {
                        I2Value=MIN_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(13);
                }
              if(VariatyI3==1)
                {//I3瑕佸彉鍖?
                  I3Value=I3Value-CurStep*devision;
                  switch(I3DC)
                  {
                    case 0:  //IC  交流
                      if(I3Value<0)
                      {
                        I3Value=0;
                        flag=1;
                      }
                      break;
                    case 1:  //IC  直流
                      if(I3Value<MIN_I_VALUEDATA_DC)
                      {
                        I3Value=MIN_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                 displaydataandstate(16);
                }
              break;
            case 2://

              V1RelaPhase=0;
              V2RelaPhase=0;
              V3RelaPhase=0;
              VzRelaPhase=0;
              I1RelaPhase=0;
              I2RelaPhase=0;
              I3RelaPhase=0;



              if(VariatyV1==1&&V1DC==0)
                {//V1瑕佸彉鍖?
                  V1Phase=V1Phase-PhaStep*devision;
                  if(V1Phase<-180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V1Phase=V1Phase+PhaStep*devision;
                      flag=1;
                    }
                   else
                  {
                      if(PhaStep>=0)
                        V1RelaPhase=360-PhaStep*devision;
                      else if(PhaStep<0)
                        V1RelaPhase=0-PhaStep*devision;
                  }
                    displaydataandstate(2);
                }
              else if(VariatyV1!=1&&V1DC==0)
                {//V1涓嶅彉鍖?
                  V1RelaPhase=0;
                }
              if(VariatyV2==1&&V2DC==0)
                {//V2瑕佸彉鍖?
                  V2Phase=V2Phase-PhaStep*devision;
                  if(V2Phase<-180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V2Phase=V2Phase+PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        V2RelaPhase=360-PhaStep*devision;
                      else if(PhaStep<0)
                        V2RelaPhase=0-PhaStep*devision;
                  }
                   displaydataandstate(5);
                }
              else if(VariatyV2!=1&&V2DC==0)
                {//V2涓嶅彉鍖?
                  V2RelaPhase=0;
                }

              if(VariatyV3==1&&V3DC==0)
                {//V3瑕佸彉鍖?
                  V3Phase=V3Phase-PhaStep*devision;

                  if(V3Phase<-180)
                    {
                      V3Phase=V3Phase+PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        V3RelaPhase=360-PhaStep*devision;
                      else if(PhaStep<0)
                        V3RelaPhase=0-PhaStep*devision;
                  }
                  displaydataandstate(8);
                }
              else if(VariatyV3!=1&&V3DC==0)
                {//V3涓嶅彉鍖?
                  V3RelaPhase=0;
                }

              if(VariatyVz==1&&VzDC==0)
                {//V3瑕佸彉鍖?
                  VzPhase=VzPhase-PhaStep*devision;
                  if(VzPhase<-180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      VzPhase=VzPhase+PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        VzRelaPhase=360-PhaStep*devision;
                      else if(PhaStep<0)
                        VzRelaPhase=0-PhaStep*devision;
                  }
                  displaydataandstate(30);
                }
              else if(VariatyVz!=1&&VzDC==0)
                {//V3涓嶅彉鍖?
                  VzRelaPhase=0;
                }

              if(VariatyI1==1&&I1DC==0)
                {//I1瑕佸彉鍖?
                  I1Phase=I1Phase-PhaStep*devision;
                  if(I1Phase<-180)
                    {
                      I1Phase=I1Phase+PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        I1RelaPhase=360-PhaStep*devision;
                      else if(PhaStep<0)
                        I1RelaPhase=0-PhaStep*devision;
                  }
                 displaydataandstate(11);
                }
              else if(VariatyI1!=1&&I1DC==0)
                {//I1涓嶅彉鍖?
                  I1RelaPhase=0;
                }
              if(VariatyI2==1&&I2DC==0)
                {//I2瑕佸彉鍖?
                  I2Phase=I2Phase-PhaStep*devision;
                  if(I2Phase<-180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      I2Phase=I2Phase+PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        I2RelaPhase=360-PhaStep*devision;
                      else if(PhaStep<0)
                        I2RelaPhase=0-PhaStep*devision;
                  }
                  displaydataandstate(14);
                }

              else if(VariatyI2!=1&&I2DC==0)
                {//I1涓嶅彉鍖?
                  I2RelaPhase=0;
                }
              if(VariatyI3==1&&I3DC==0)
                {//I3瑕佸彉鍖?
                  I3Phase=I3Phase-PhaStep*devision;
                  if(I3Phase<-180)
                    {
                      I3Phase=I3Phase+PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(PhaStep>=0)
                        I3RelaPhase=360-PhaStep*devision;
                      else if(PhaStep<0)
                        I3RelaPhase=0-PhaStep*devision;
                  }
                   displaydataandstate(17);
                }
              else if(VariatyI3!=1&&I3DC==0)
                {//I3涓嶅彉鍖?
                  I3RelaPhase=0;
                }
              break;


            case 3:
              if(VariatyV1==1&&V1DC==0)
                {//V1瑕佸彉鍖?
                  V1Frec=V1Frec-FreStep*devision;
                  if(V1Frec<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V1Frec=V1Frec+FreStep*devision;
                      flag=1;
                    }

                   displaydataandstate(3);
                }
              if(VariatyV2==1&&V2DC==0)
                {//V2瑕佸彉鍖?
                  V2Frec=V2Frec-FreStep*devision;
                  if(V2Frec<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V2Frec=V2Frec+FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(6);
                }
              if(VariatyV3==1&&V3DC==0)
                {//V3瑕佸彉鍖?
                  V3Frec=V3Frec-FreStep*devision;
                  if(V3Frec<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      V3Frec=V3Frec+FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(9);
                }


              if(VariatyVz==1&&VzDC==0)
                {//V3瑕佸彉鍖?
                  VzFrec=VzFrec-FreStep*devision;


                  if(VzFrec<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      VzFrec=VzFrec+FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(31);
                }




              if(VariatyI1==1&&I1DC==0)
                {//I1瑕佸彉鍖?
                  I1Frec=I1Frec-FreStep*devision;
                  if(I1Frec<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      I1Frec=I1Frec+FreStep*devision;
                      flag=1;
                    }
                   displaydataandstate(12);
                }
              if(VariatyI2==1&&I2DC==0)
                {//I2瑕佸彉鍖?
                  I2Frec=I2Frec-FreStep*devision;
                  if(I2Frec<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      I2Frec=I2Frec+FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(15);
                }
              if(VariatyI3==1&&I3DC==0)
                {//I3瑕佸彉鍖?
                  I3Frec=I3Frec-FreStep*devision;
                  if(I3Frec<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      I3Frec=I3Frec+FreStep*devision;
                      flag=1;
                    }
                   displaydataandstate(18);
                }
              break;
          }
      }
    // fasong xinhao gei xiancheng
   emit SIG_Manualoutput();
  return flag;
  }




Manualtest::~Manualtest()
{
    delete ui;
}

void Manualtest::on_pushButton_2_clicked()
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


                 strcpy(strbuf,"任意测试结果：");
                 fprintf(fp,"%s\r\n",strbuf);


     if(TestMode==1)
     {
         strcpy(strbuf,"手动模式:");
         fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"动作值:");
       fprintf(fp,"%s\r\n",strbuf);
       switch(Variable)
       {
       case 1:  //幅值
           if(VariatyV1==1)
           {
            QString str;
            QByteArray ba;
            char *buf;
            str=QString(tr("VA:")+QString::number(V1Value,'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);

           }
           if(VariatyV2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VB:")+QString::number(V2Value,'g',6)+tr(" V"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);

           }
           if(VariatyV3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(V3Value,'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(VariatyVz==1)
           {
             QString str;
             QByteArray ba;
             char *buf;
            str=QString(tr("Vz:")+QString::number(VzValue,'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }





           if(VariatyI1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("IA:")+QString::number(I1Value,'g',6)+tr(" A"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("IB:")+QString::number(I2Value,'g',6)+tr(" A"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IC:")+QString::number(I3Value,'g',6)+tr(" A"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           break;
       case 2: // 相位
           if(VariatyV1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VA:")+QString::number(V1Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyV2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VB:")+QString::number(V2Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyV3==1)
           {   QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(V3Phase,'g',6)+tr(" Deg"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyVz==1)
           {   QString str;
               QByteArray ba;
               char *buf;
              str=QString(tr("Vz:")+QString::number(VzPhase,'g',6)+tr(" Deg"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }





           if(VariatyI1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IA:")+QString::number(I1Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);

           }
           if(VariatyI2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IB:")+QString::number(I2Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
                str=QString(tr("IC:")+QString::number(I3Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           break;


       case 3: //频率
           if(VariatyV1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VA:")+QString::number(V1Frec,'g',6)+tr(" Hz"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyV2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VB:")+QString::number(V2Frec,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyV3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(V3Frec,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(VariatyVz==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("Vz:")+QString::number(VzFrec,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(VariatyI1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IA:")+QString::number(I1Frec,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IB:")+QString::number(I2Frec,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IC:")+QString::number(I3Frec,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           break;
       }

     }

     else if(TestMode==2)
    {
         strcpy(strbuf,"触发模式:");
         fprintf(fp,"%s\r\n",strbuf);
       if(TripTime==-1)
       strcpy(strbuf,"动作时间(s):未动作");
       else
       sprintf(strbuf,"动作时间(s):%.3f",TripTime);
       fprintf(fp,"%s\r\n",strbuf);
    }

     else if(TestMode==3)
     {
         strcpy(strbuf,"自动模式:");
         fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"动作值:");
       fprintf(fp,"%s\r\n",strbuf);
       switch(Variable)
       {
       case 1:  //幅值
           if(VariatyV1==1)
           {
            QString str;
            QByteArray ba;
            char *buf;
            str=QString(tr("VA:")+QString::number(V1Value+SumStepnum*(VolStep/*-V1ampPerStepError*/),'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);

           }
           if(VariatyV2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VB:")+QString::number(V2Value+SumStepnum*(VolStep/*-V2ampPerStepError*/),'g',6)+tr(" V"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);

           }
           if(VariatyV3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(V3Value+SumStepnum*(VolStep/*-V3ampPerStepError*/),'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(VariatyVz2==0&&VariatyVz==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("Vz:")+QString::number(VzValue+SumStepnum*(VolStep/*-VzampPerStepError*/),'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }





           if(VariatyI1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("IA:")+QString::number(I1Value+SumStepnum*(CurStep/*-I1ampPerStepError*/),'g',6)+tr(" A"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("IB:")+QString::number(I2Value+SumStepnum*(CurStep/*-I2ampPerStepError*/),'g',6)+tr(" A"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IC:")+QString::number(I3Value+SumStepnum*(CurStep/*-I3ampPerStepError*/),'g',6)+tr(" A"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           break;
       case 2: // 相位
           if(VariatyV1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VA:")+QString::number(V1Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyV2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VB:")+QString::number(V2Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyV3==1)
           {   QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(V3Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }

           if(VariatyVz2==0&&VariatyVz==1)
           {   QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("Vz:")+QString::number(VzPhase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }




           if(VariatyI1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IA:")+QString::number(I1Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);

           }
           if(VariatyI2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IB:")+QString::number(I2Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
                str=QString(tr("IC:")+QString::number(I3Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           break;


       case 3: //频率
           if(VariatyV1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VA:")+QString::number(V1Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyV2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VB:")+QString::number(V2Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyV3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(V3Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(VariatyVz2==0&&VariatyVz==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("Vz:")+QString::number(VzFrec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }



           if(VariatyI1==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IA:")+QString::number(I1Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI2==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IB:")+QString::number(I2Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(VariatyI3==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IC:")+QString::number(I3Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           break;
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

                       strcpy(strbuf,"Anytest  result");
                       fprintf(fp,"%s\r\n",strbuf);


           if(TestMode==1)
           {
               strcpy(strbuf,"Manual:");
               fprintf(fp,"%s\r\n",strbuf);

             strcpy(strbuf,"Trip value:");
             fprintf(fp,"%s\r\n",strbuf);
             switch(Variable)
             {
             case 1:  //幅值
                 if(VariatyV1==1)
                 {
                  QString str;
                  QByteArray ba;
                  char *buf;
                  str=QString(tr("VA:")+QString::number(V1Value,'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);

                 }
                 if(VariatyV2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VB:")+QString::number(V2Value,'g',6)+tr(" V"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);

                 }
                 if(VariatyV3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(V3Value,'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(VariatyVz==1)
                 {
                   QString str;
                   QByteArray ba;
                   char *buf;
                  str=QString(tr("Vz:")+QString::number(VzValue,'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }





                 if(VariatyI1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("IA:")+QString::number(I1Value,'g',6)+tr(" A"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("IB:")+QString::number(I2Value,'g',6)+tr(" A"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IC:")+QString::number(I3Value,'g',6)+tr(" A"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 break;
             case 2: // 相位
                 if(VariatyV1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VA:")+QString::number(V1Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyV2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VB:")+QString::number(V2Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyV3==1)
                 {   QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(V3Phase,'g',6)+tr(" Deg"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyVz==1)
                 {   QString str;
                     QByteArray ba;
                     char *buf;
                    str=QString(tr("Vz:")+QString::number(VzPhase,'g',6)+tr(" Deg"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }





                 if(VariatyI1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IA:")+QString::number(I1Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);

                 }
                 if(VariatyI2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IB:")+QString::number(I2Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                      str=QString(tr("IC:")+QString::number(I3Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 break;


             case 3: //频率
                 if(VariatyV1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VA:")+QString::number(V1Frec,'g',6)+tr(" Hz"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyV2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VB:")+QString::number(V2Frec,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyV3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(V3Frec,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(VariatyVz==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("Vz:")+QString::number(VzFrec,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(VariatyI1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IA:")+QString::number(I1Frec,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IB:")+QString::number(I2Frec,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IC:")+QString::number(I3Frec,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 break;
             }

           }

           else if(TestMode==2)
          {
               strcpy(strbuf,"Shot:");
               fprintf(fp,"%s\r\n",strbuf);
             if(TripTime==-1)
             strcpy(strbuf,"Trip time(s):no trip");
             else
             sprintf(strbuf,"Trip time(s):%.3f",TripTime);
             fprintf(fp,"%s\r\n",strbuf);
          }

           else if(TestMode==3)
           {
               strcpy(strbuf,"Auto:");
               fprintf(fp,"%s\r\n",strbuf);

             strcpy(strbuf,"Trip value:");
             fprintf(fp,"%s\r\n",strbuf);
             switch(Variable)
             {
             case 1:  //幅值
                 if(VariatyV1==1)
                 {
                  QString str;
                  QByteArray ba;
                  char *buf;
                  str=QString(tr("VA:")+QString::number(V1Value+SumStepnum*(VolStep/*-V1ampPerStepError*/),'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);

                 }
                 if(VariatyV2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VB:")+QString::number(V2Value+SumStepnum*(VolStep/*-V2ampPerStepError*/),'g',6)+tr(" V"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);

                 }
                 if(VariatyV3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(V3Value+SumStepnum*(VolStep/*-V3ampPerStepError*/),'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(VariatyVz2==0&&VariatyVz==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("Vz:")+QString::number(VzValue+SumStepnum*(VolStep/*-VzampPerStepError*/),'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }





                 if(VariatyI1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("IA:")+QString::number(I1Value+SumStepnum*(CurStep/*-I1ampPerStepError*/),'g',6)+tr(" A"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("IB:")+QString::number(I2Value+SumStepnum*(CurStep/*-I2ampPerStepError*/),'g',6)+tr(" A"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IC:")+QString::number(I3Value+SumStepnum*(CurStep/*-I3ampPerStepError*/),'g',6)+tr(" A"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 break;
             case 2: // 相位
                 if(VariatyV1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VA:")+QString::number(V1Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyV2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VB:")+QString::number(V2Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyV3==1)
                 {   QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(V3Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }

                 if(VariatyVz2==0&&VariatyVz==1)
                 {   QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("Vz:")+QString::number(VzPhase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }




                 if(VariatyI1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IA:")+QString::number(I1Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);

                 }
                 if(VariatyI2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IB:")+QString::number(I2Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                      str=QString(tr("IC:")+QString::number(I3Phase+SumStepnum*PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 break;


             case 3: //频率
                 if(VariatyV1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VA:")+QString::number(V1Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyV2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VB:")+QString::number(V2Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyV3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(V3Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(VariatyVz2==0&&VariatyVz==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("Vz:")+QString::number(VzFrec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }



                 if(VariatyI1==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IA:")+QString::number(I1Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI2==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IB:")+QString::number(I2Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(VariatyI3==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IC:")+QString::number(I3Frec+SumStepnum*FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 break;
             }

           }
           fclose(fp);
           system("sync");
  }


}

void Manualtest::on_pushButton_3_clicked()
{
    if(starttest==1)
        return;
  uartthread->stoprun();
  while(uartthread->isRunning())//2016-4-29修改，EXIT退出线程死循环卡死
  {
      uartthread->stoprun();
      sleep(1);
      break;
  }
    free(temprorarydata);
    temprorarydata=NULL;
    close();
//    if(uartthread->isRunning())
//    {
//        uartthread->stoprun();
//        free(temprorarydata);
//        temprorarydata=NULL;
//        close();
//    }

}


void  Manualtest::ShowWarning(int Outtype,int type)
{



    char *buff[7]={"VA","VB","VC","Vz","IA","IB","IC"};
    float MaxValue,MinValue;

    if(V1DC==0||V2DC==0||V3DC==0||VzDC==0||I1DC==0||I2DC==0||I3DC==0)
      {//浜ゆ祦
        if(Outtype>=1&&Outtype<=4)
        {
            MaxValue=MAX_V_VALUEDATA_AC;
            MinValue=MIN_V_VALUEDATA_AC;
        }
        else
        {
            MaxValue=MAX_I_VALUEDATA_AC;
            MinValue=MIN_I_VALUEDATA_AC;
        }
      }
    else if(V1DC==1||V2DC==1||V3DC==1||VzDC==1||I1DC==1||I2DC==1||I3DC==1)
      {//鐩存祦
        if(Outtype>=1&&Outtype<=4)
        {
            MaxValue=MAX_V_VALUEDATA_DC;
            MinValue=MIN_V_VALUEDATA_DC;
        }
        else
        {
            MaxValue=MAX_I_VALUEDATA_DC;
            MinValue=MIN_I_VALUEDATA_DC;
        }
      }

    QString msg;
    if(Variable==1)//幅值
    {
       if(type==1)//超过
       {
          if(Outtype==1)
          {
           switch(V1DC)
           {
             case 0:  //VA  交流
               MaxValue=MAX_V_VALUEDATA_AC;
               break;
             case 1:  //VA  直流
               MaxValue=MAX_V_VALUEDATA_DC;
               break;
           }
          }
          else if(Outtype==2)
          {
           switch(V2DC)
           {
             case 0:  //VB  交流
               MaxValue=MAX_V_VALUEDATA_AC;
               break;
             case 1:  //VB  直流
               MaxValue=MAX_V_VALUEDATA_DC;
               break;
           }
          }
          else if(Outtype==3)
          {
           switch(V3DC)
           {
             case 0:  //VC  交流
               MaxValue=MAX_V_VALUEDATA_AC;
               break;
             case 1:  //VC  直流
               MaxValue=MAX_V_VALUEDATA_DC;
               break;
           }
          }
          else if(Outtype==4)
          {
           switch(VzDC)
           {
             case 0:  //VZ  交流
               MaxValue=MAX_V_VALUEDATA_AC;
               break;
             case 1:  //VZ  直流
               MaxValue=MAX_V_VALUEDATA_DC;
               break;
           }
          }
          else if(Outtype==5)
          {
           switch(I1DC)
           {
             case 0:  //IA  交流
               MaxValue=MAX_I_VALUEDATA_AC;
               break;
             case 1:  //IA  直流
               MaxValue=MAX_I_VALUEDATA_DC;
               break;
           }
          }
          else if(Outtype==6)
          {
           switch(I2DC)
           {
             case 0:  //IB  交流
               MaxValue=MAX_I_VALUEDATA_AC;
               break;
             case 1:  //IB  直流
               MaxValue=MAX_I_VALUEDATA_DC;
               break;
           }
          }
          else if(Outtype==7)
          {
           switch(I3DC)
           {
             case 0:  //IC  交流
               MaxValue=MAX_I_VALUEDATA_AC;
               break;
             case 1:  //IC  直流
               MaxValue=MAX_I_VALUEDATA_DC;
               break;
           }
          }
          if(current_language==1)
           msg=QString(buff[Outtype-1])+QString(tr(" 幅值超过最大值:"))+QString::number(MaxValue,'g',6);
          else
           msg=QString("Amplitude of ")+QString(buff[Outtype-1])+QString(" is bigger than:")+QString::number(MaxValue,'g',6);
       }
   else if(type==2)//小于
       {
           if(Outtype==1)
           {
            switch(V1DC)
            {
              case 0:  //VA  交流
                MinValue=MIN_V_VALUEDATA_AC;
                break;
              case 1:  //VA  直流
                MinValue=MIN_V_VALUEDATA_DC;
                break;
            }
           }
           else if(Outtype==2)
           {
            switch(V2DC)
            {
              case 0:  //VB  交流
                MinValue=MIN_V_VALUEDATA_AC;
                break;
              case 1:  //VB  直流
                MinValue=MIN_V_VALUEDATA_DC;
                break;
            }
           }
           else if(Outtype==3)
           {
            switch(V3DC)
            {
              case 0:  //Vc  交流
                MinValue=MIN_V_VALUEDATA_AC;
                break;
              case 1:  //Vc  直流
                MinValue=MIN_V_VALUEDATA_DC;
                break;
            }
           }
           else if(Outtype==4)
           {
            switch(VzDC)
            {
              case 0:  //Vz  交流
                MinValue=MIN_V_VALUEDATA_AC;
                break;
              case 1:  //Vz  直流
                MinValue=MIN_V_VALUEDATA_DC;
                break;
            }
           }
           else if(Outtype==5)
           {
            switch(I1DC)
            {
              case 0:  //IA  交流
                MinValue=MIN_I_VALUEDATA_AC;
                break;
              case 1:  //IA  直流
                MinValue=MIN_I_VALUEDATA_DC;
                break;
            }
           }
           else if(Outtype==6)
           {
            switch(I2DC)
            {
              case 0:  //Ib  交流
                MinValue=MIN_I_VALUEDATA_AC;
                break;
              case 1:  //Ib  直流
                MinValue=MIN_I_VALUEDATA_DC;
                break;
            }
           }
           else if(Outtype==7)
           {
            switch(I3DC)
            {
              case 0:  //Ic  交流
                MinValue=MIN_I_VALUEDATA_AC;
                break;
              case 1:  //Ic  直流
                MinValue=MIN_I_VALUEDATA_DC;
                break;
            }
           }
      if(current_language==1)
       msg=QString(buff[Outtype-1])+QString(tr(" 幅值低于最小值:"))+QString::number(0,'g',6);
      else
       msg=QString("Amplitude of ")+QString(buff[Outtype-1])+QString(" is smaller than:")+QString::number(0,'g',6);
       }
    }
    else  if(Variable==3)//频率
    {
       if(current_language==1)
       {
        if(type==1)//超过
        msg=QString(buff[Outtype-1])+QString(tr(" 频率超过最大值:"))+QString::number(1000,'g',6);
        else if(type==2)//小于
        msg=QString(buff[Outtype-1])+QString(tr(" 频率低于最小值:"))+QString::number(0,'g',6);
       }
       else
       {
           if(type==1)//超过
           msg=QString("Frequency of ")+QString(buff[Outtype-1])+QString(" is bigger than:1000");
           else if(type==2)//小于
           msg=QString("Frequency of ")+QString(buff[Outtype-1])+QString(" is smaller than:0");
       }
    }

    Warning *warning=new Warning;

    connect(warning,SIGNAL(WarningWindowClose()),this,SLOT(WarningWinclose()));
    warning->raise();
    warning->activateWindow();
    warning->setModal(true);
    warning->Setwarning(1,msg);
    warning->show();


}

 void Manualtest::WarningWinclose()
 {
   temprorarydata->Warningopenflag=false;
 }



void Manualtest::CloseWidget()
{
      while(uartthread->isRunning());
      free(temprorarydata);
      temprorarydata=NULL;
      close();
}

void Manualtest::GetTeststate(int state)
{
    //2016-9-13  键盘输入，界面测试按钮，
    //设置软件“测试”按钮获取焦点，激活失去焦点时间，给最后的输入幅值
    if(state==1)
    ui->pushButton_6->setFocus();


        /*for(int i=0;i<lineeditlist.size();i++)
           {
             QLineEdit *lineedit=lineeditlist.at(i);
             if(state==1)
             lineedit->setFocusPolicy(Qt::NoFocus);
             else
             lineedit->setFocusPolicy(Qt::ClickFocus);

           }

*/



}



void Manualtest::on_checkBox_clicked()
{
    if(TestMode==1||TestMode==3)
    {
        if(Variable==2) //
        {
        VariatyV1=0;
        ui->checkBox->setChecked(false);
        return;
        }
    }

    if(VariatyV1==1)
       { VariatyV1=0;

        ui->checkBox->setChecked(false);
       }
    else {
        VariatyV1=1;
        ui->checkBox->setChecked(true);
    }
}

void Manualtest::on_checkBox_2_clicked()
{
    if(VariatyV2==1)
        VariatyV2=0;
    else VariatyV2=1;
}

void Manualtest::on_checkBox_3_clicked()
{
    if(VariatyV3==1)
        VariatyV3=0;
    else VariatyV3=1;
}

void Manualtest::on_checkBox_4_clicked()
{
    if(VariatyI1==1)
        VariatyI1=0;
    else VariatyI1=1;
}

void Manualtest::on_checkBox_5_clicked()
{
    if(VariatyI2==1)
        VariatyI2=0;
    else VariatyI2=1;

}

void Manualtest::on_checkBox_6_clicked()
{
    if(VariatyI3==1)
        VariatyI3=0;
    else VariatyI3=1;
}


void Manualtest::on_radioButton_clicked()
{
   TestMode=1;
   ui->groupBox_5->setEnabled(true);
   ui->groupBox_7->setEnabled(true);
   ui->groupBox_3->setEnabled(true);
   ui->lineEdit_28->setEnabled(false);
   ui->groupBox_4->setEnabled(false);
   ui->label_4->setEnabled(false);
   ui->checkBox_8->setEnabled(true);
   ui->checkBox_9->setHidden(false);
   ui->groupBox->setEnabled(true);

   ui->lineEdit_23->setEnabled(false);
   ui->lineEdit_24->setEnabled(false);
   ui->label_20->setEnabled(false);
   ui->label_21->setEnabled(false);

   if(CurStep<0)
   {
       CurStep=0;
       ui->lineEdit_19->setText(QString::number(CurStep,'g',4));
   }
   if(VolStep<0)
   {
       VolStep=0;
       ui->lineEdit_20->setText(QString::number(VolStep,'g',4));
   }
   if(PhaStep<0)
   {
       PhaStep=0;
       ui->lineEdit_21->setText(QString::number(PhaStep,'g',4));
   }
   if(FreStep<0)
   {
       FreStep=0;
       ui->lineEdit_22->setText(QString::number(FreStep,'g',4));
   }
}

void Manualtest::on_radioButton_2_clicked()
{
  TestMode=2;
   ui->groupBox_5->setEnabled(false);
   ui->groupBox_7->setEnabled(false);
   ui->groupBox_3->setEnabled(false);
   ui->lineEdit_28->setEnabled(false);
   ui->groupBox_4->setEnabled(true);
   ui->label_4->setEnabled(false);
   ui->groupBox->setEnabled(false);

   ui->checkBox_8->setEnabled(true);
   ui->checkBox_9->setHidden(false);

   ui->lineEdit_23->setEnabled(false);
   ui->lineEdit_24->setEnabled(false);
   ui->label_20->setEnabled(false);
   ui->label_21->setEnabled(false);

}

void Manualtest::on_radioButton_3_clicked()
{
   TestMode=3;
   ui->groupBox_5->setEnabled(true);
   ui->groupBox_7->setEnabled(false);
   ui->groupBox_3->setEnabled(true);
   ui->lineEdit_28->setEnabled(true);
   ui->groupBox_4->setEnabled(false);
   ui->groupBox->setEnabled(true);
   ui->label_4->setEnabled(true);
   ui->checkBox_8->setEnabled(false);
   ui->checkBox_9->setChecked(false);
   ui->checkBox_9->setHidden(true);

   ui->lineEdit_23->setEnabled(true);
   ui->lineEdit_24->setEnabled(true);
   ui->label_20->setEnabled(true);
   ui->label_21->setEnabled(true);
}

void Manualtest::on_radioButton_4_clicked()
{
  Variable=1;


}

void Manualtest::on_radioButton_5_clicked()
{
  Variable=2;


}

void Manualtest::on_radioButton_6_clicked()
{
  Variable=3;

}




void Manualtest::setlineedittext(QString str)
{
    bool ok;
    float number=str.toFloat(&ok);
    if(ok==false)
        return;
    switch(currenteditpos)
    {
    case 1:
        V1Value=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(V1Value,'g',6));
        break;
    case 2:
        V1Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(V1Phase,'g',6));
        ui->lineEdit_2->setText(QString::number(V1Phase,'g',6));
        ui->lineEdit_5->setText(QString::number(V2Phase,'g',6));
        ui->lineEdit_8->setText(QString::number(V3Phase,'g',6));
        VolPhaseChange=1;
        break;
    case 3:
        V1Frec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(V1Frec,'g',6));
        break;

    case 4:
        V2Value=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(V2Value,'g',6));
        break;
    case 5:
        V2Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(V2Phase,'g',6));
        ui->lineEdit_2->setText(QString::number(V1Phase,'g',6));
        ui->lineEdit_5->setText(QString::number(V2Phase,'g',6));
        ui->lineEdit_8->setText(QString::number(V3Phase,'g',6));
         VolPhaseChange=2;
        break;
    case 6:
        V2Frec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(V2Frec,'g',6));
        break;


    case 7:
        V3Value=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(V3Value,'g',6));
        break;
    case 8:
        V3Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(V3Phase,'g',6));
        ui->lineEdit_2->setText(QString::number(V1Phase,'g',6));
        ui->lineEdit_5->setText(QString::number(V2Phase,'g',6));
        ui->lineEdit_8->setText(QString::number(V3Phase,'g',6));
        VolPhaseChange=3;
        break;
    case 9:
        V3Frec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(V3Frec,'g',6));
        break;




    case 10:
        I1Value=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(I1Value,'g',6));
        break;
    case 11:
        I1Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(I1Phase,'g',6));

        ui->lineEdit_11->setText(QString::number(I1Phase,'g',6));
        ui->lineEdit_14->setText(QString::number(I2Phase,'g',6));
        ui->lineEdit_17->setText(QString::number(I3Phase,'g',6));
        CurPhaseChange=1;
        break;
    case 12:
        I1Frec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(I1Frec,'g',6));
        break;



    case 13:
        I2Value=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(I2Value,'g',6));
        break;
    case 14:
        I2Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(I2Phase,'g',6));
        ui->lineEdit_11->setText(QString::number(I1Phase,'g',6));
        ui->lineEdit_14->setText(QString::number(I2Phase,'g',6));
        ui->lineEdit_17->setText(QString::number(I3Phase,'g',6));
        CurPhaseChange=2;
        break;
    case 15:
        I2Frec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(I2Frec,'g',6));
        break;



    case 16:
        I3Value=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(I3Value,'g',6));
        break;
    case 17:
        I3Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(I3Phase,'g',6));
        ui->lineEdit_11->setText(QString::number(I1Phase,'g',6));
        ui->lineEdit_14->setText(QString::number(I2Phase,'g',6));
        ui->lineEdit_17->setText(QString::number(I3Phase,'g',6));
        CurPhaseChange=3;
        break;
    case 18:
        I3Frec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(I3Frec,'g',6));
        break;

    case 19:
        CurStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(CurStep,'g',6));
        break;
    case 20:
        VolStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VolStep,'g',6));
        break;
    case 21:
        PhaStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PhaStep,'g',6));
        break;
    case 22:
        FreStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FreStep,'g',6));
        break;
    case 23:
        StepTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StepTime,'g',6));
        break;
    case 24:
        StepNum=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number( StepNum,'g',6));
        break;
    case 25:
        PreFaultTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number( PreFaultTime,'g',6));
        break;
    case 26:
        FaultTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FaultTime,'g',6));
        break;
    case 27:
        PostFaultTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PostFaultTime,'g',6));
        break;
    case 28:
        faultduration=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(faultduration,'g',6));
        break;
    case 29:
        if(VariatyVz2==0)
       { VzValue=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VzValue,'g',6));
       }
        break;
    case 30:
         if(VariatyVz2==0)
       { VzPhase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VzPhase,'g',6));
       }
        break;
    case 31:
        if(VariatyVz2==0)
       { VzFrec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VzFrec,'g',6));
       }
        break;
}
    if(currenteditpos>=1&&currenteditpos<=9)
    {
        updateVz();
    }




}

void Manualtest::clearkeypadflag()
{
 temprorarydata->keypadopenflag=false;
 if(pad)
 { pad->close();
   pad=NULL;
 }


}

void Manualtest::on_pushButton_4_clicked()  //鎵嬪姩 澧炲姞
{
    if(starttest==1&&TestMode==1)
     vi_change_step(1,1);
    updateVz();
}

void Manualtest::on_pushButton_5_clicked()
{
 if(starttest==1&&TestMode==1)
    vi_change_step(0,1);
 updateVz();
}

void Manualtest::on_pushButton_clicked()
{
    if(starttest==1)
        return;

Helpdialog *helpdialog=new Helpdialog;
temprorarydata->helpopenflag=true;
connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
helpdialog->SetHelpfilename("Manual.txt");
helpdialog->show();
}
void Manualtest::HelpDialogClose()
{
temprorarydata->helpopenflag=false;

}

bool Manualtest::eventFilter(QObject *obj, QEvent *event)
{
    if(starttest==1)
       {
        return QWidget::eventFilter(obj,event);

       }


    if(event->type()==QEvent::MouseButtonPress)
    {

        /*
        QMouseEvent *mouseevent=static_cast<QMouseEvent *>(event);
        if(mouseevent->button()==Qt::RightButton)
        {

            return true;
        }
        else
        return false;
        */
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
        {
            temprorarydata->keypadopenflag=true;
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
        else if(obj==ui->lineEdit_18)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_18;
              currenteditpos=18;
         }
        else if(obj==ui->lineEdit_19)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_19;
              currenteditpos=19;
         }
        else if(obj==ui->lineEdit_20)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_20;
              currenteditpos=20;
         }
        else if(obj==ui->lineEdit_21)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_21;
              currenteditpos=21;
         }
        else if(obj==ui->lineEdit_22)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_22;
              currenteditpos=22;
         }
        else if(obj==ui->lineEdit_23)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_23;
              currenteditpos=23;
         }
        else if(obj==ui->lineEdit_24)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_24;
              currenteditpos=24;
         }
        else if(obj==ui->lineEdit_25)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_25;
              currenteditpos=25;
         }
        else if(obj==ui->lineEdit_26)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_26;
              currenteditpos=26;
         }
        else if(obj==ui->lineEdit_27)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_27;
              currenteditpos=27;
         }
        else if(obj==ui->lineEdit_28)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_28;
              currenteditpos=28;
         }
        else if(obj==ui->lineEdit_29)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_29;
              currenteditpos=29;
         }
        else if(obj==ui->lineEdit_30)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_30;
              currenteditpos=30;
         }
        else if(obj==ui->lineEdit_31)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_31;
              currenteditpos=31;
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
              V1Value= text.toFloat();
              currenteditpos=1;
              vi_parameter_enter(currenteditpos);
              ui->lineEdit->setText(QString::number(V1Value,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               V1Phase= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(V1Phase,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                V1Frec= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(V1Frec,'g',6));
         }
        else if(obj==ui->lineEdit_4)
         {
                QString text=ui->lineEdit_4->text();
                V2Value= text.toFloat();
                currenteditpos=4;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_4->setText(QString::number(V2Value,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                V2Phase= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(V2Phase,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                V2Frec= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(V2Frec,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                V3Value= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(V3Value,'g',6));
         }
        else if(obj==ui->lineEdit_8)
         {
                QString text=ui->lineEdit_8->text();
                V3Phase= text.toFloat();
                currenteditpos=8;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_8->setText(QString::number(V3Phase,'g',6));
         }
        else if(obj==ui->lineEdit_9)
         {
                QString text=ui->lineEdit_9->text();
                V3Frec= text.toFloat();
                currenteditpos=9;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_9->setText(QString::number(V3Frec,'g',6));
         }
        else if(obj==ui->lineEdit_10)
         {
                QString text=ui->lineEdit_10->text();
                I1Value= text.toFloat();
                currenteditpos=10;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_10->setText(QString::number(I1Value,'g',6));
         }
        else if(obj==ui->lineEdit_11)
         {
                QString text=ui->lineEdit_11->text();
                I1Phase= text.toFloat();
                currenteditpos=11;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_11->setText(QString::number(I1Phase,'g',6));
         }
        else if(obj==ui->lineEdit_12)
         {
                QString text=ui->lineEdit_12->text();
                I1Frec= text.toFloat();
                currenteditpos=12;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_12->setText(QString::number(I1Frec,'g',6));
         }
        else if(obj==ui->lineEdit_13)
         {
                QString text=ui->lineEdit_13->text();
                I2Value= text.toFloat();
                currenteditpos=13;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_13->setText(QString::number(I2Value,'g',6));
         }
        else if(obj==ui->lineEdit_14)
         {
                QString text=ui->lineEdit_14->text();
                I2Phase= text.toFloat();
                currenteditpos=14;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_14->setText(QString::number(I2Phase,'g',6));
         }
        else if(obj==ui->lineEdit_15)
         {
                QString text=ui->lineEdit_15->text();
                I2Frec= text.toFloat();
                currenteditpos=15;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_15->setText(QString::number(I2Frec,'g',6));
         }
        else if(obj==ui->lineEdit_16)
         {
                QString text=ui->lineEdit_16->text();
                I3Value= text.toFloat();
                currenteditpos=16;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_16->setText(QString::number(I3Value,'g',6));
         }
        else if(obj==ui->lineEdit_17)
         {
                QString text=ui->lineEdit_17->text();
                I3Phase= text.toFloat();
                currenteditpos=17;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_17->setText(QString::number(I3Phase,'g',6));
         }
        else if(obj==ui->lineEdit_18)
         {
                QString text=ui->lineEdit_18->text();
                I3Frec= text.toFloat();
                currenteditpos=18;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_18->setText(QString::number(I3Frec,'g',6));
         }
        else if(obj==ui->lineEdit_19)
         {
                QString text=ui->lineEdit_19->text();
                CurStep= text.toFloat();
                currenteditpos=19;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_19->setText(QString::number(CurStep,'g',6));
         }
        else if(obj==ui->lineEdit_20)
         {
                QString text=ui->lineEdit_20->text();
                VolStep= text.toFloat();
                currenteditpos=20;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_20->setText(QString::number(VolStep,'g',6));
         }
        else if(obj==ui->lineEdit_21)
         {
                QString text=ui->lineEdit_21->text();
                PhaStep= text.toFloat();
                currenteditpos=21;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_21->setText(QString::number(PhaStep,'g',6));
         }
        else if(obj==ui->lineEdit_22)
         {
                QString text=ui->lineEdit_22->text();
                FreStep= text.toFloat();
                currenteditpos=22;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_22->setText(QString::number(FreStep,'g',6));
         }
        else if(obj==ui->lineEdit_23)
         {
                QString text=ui->lineEdit_23->text();
                StepTime= text.toFloat();
                currenteditpos=23;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_23->setText(QString::number(StepTime,'g',6));
         }
        else if(obj==ui->lineEdit_24)
         {
                QString text=ui->lineEdit_24->text();
                StepNum= text.toFloat();
                currenteditpos=24;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_24->setText(QString::number(StepNum,'g',6));
         }
        else if(obj==ui->lineEdit_25)
         {
                QString text=ui->lineEdit_25->text();
                PreFaultTime= text.toFloat();
                currenteditpos=25;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_25->setText(QString::number(PreFaultTime,'g',6));
         }
        else if(obj==ui->lineEdit_26)
         {
                QString text=ui->lineEdit_26->text();
                FaultTime= text.toFloat();
                currenteditpos=26;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_26->setText(QString::number(FaultTime,'g',6));
         }
        else if(obj==ui->lineEdit_27)
         {
                QString text=ui->lineEdit_27->text();
                PostFaultTime= text.toFloat();
                currenteditpos=27;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_27->setText(QString::number(PostFaultTime,'g',6));
         }
        else if(obj==ui->lineEdit_28)
         {
                QString text=ui->lineEdit_28->text();
                faultduration= text.toFloat();
                currenteditpos=28;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_28->setText(QString::number(faultduration,'g',6));
         }
        else if(obj==ui->lineEdit_29)
         {
                QString text=ui->lineEdit_29->text();
                VzValue= text.toFloat();
                currenteditpos=29;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_29->setText(QString::number(VzValue,'g',6));
         }
        else if(obj==ui->lineEdit_30)
         {
                QString text=ui->lineEdit_30->text();
                VzPhase= text.toFloat();
                currenteditpos=30;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_30->setText(QString::number(VzPhase,'g',6));
         }
        else if(obj==ui->lineEdit_31)
         {
                QString text=ui->lineEdit_31->text();
                VzFrec= text.toFloat();
                currenteditpos=31;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_31->setText(QString::number(VzFrec,'g',6));
         }
        return QWidget::eventFilter(obj,event);
    }
    else
    return QWidget::eventFilter(obj,event);

}

 void Manualtest::moveEvent(QMoveEvent *event)
 {
     QPoint oldpos=event->oldPos();
     this->move(oldpos);

 }

void Manualtest::on_checkBox_7_clicked()
{
    if(Phasesymmetry)
        Phasesymmetry=false;
    else
        Phasesymmetry=true;

    if(Phasesymmetry)
    {
       if(VolPhaseChange!=0)
       {
           switch(VolPhaseChange)
           {
           case 1:
                        V2Phase=V1Phase-120;
                           V3Phase=V1Phase+120;
                           if(V2Phase>180)
                               V2Phase=V2Phase-360;
                           else if(V2Phase<-180)
                               V2Phase+=360;
                           if(V3Phase>180)
                               V3Phase=V3Phase-360;
                           else if(V3Phase<-180)
                               V3Phase+=360;
               break;
           case 2:
                            V1Phase=V2Phase+120;
                             if(V1Phase>180)
                                 V1Phase-=360;
                             else if(V1Phase<-180)
                                 V1Phase+=360;

                             V3Phase=V1Phase+120;
                             if(V3Phase>180)
                                 V3Phase-=360;
                             else if(V3Phase<-180)
                                 V3Phase+=360;
               break;

           case 3:
                               V1Phase=V3Phase-120;
                               if(V1Phase>180)
                                   V1Phase-=360;
                               else if(V1Phase<-180)
                                   V1Phase+=360;
                               V2Phase=V1Phase-120;
                               if(V2Phase>180)
                                   V2Phase-=360;
                               else if(V2Phase<-180)
                                   V2Phase+=360;
               break;

           }


           ui->lineEdit_2->setText(QString::number(V1Phase,'g',6));
           ui->lineEdit_5->setText(QString::number(V2Phase,'g',6));
           ui->lineEdit_8->setText(QString::number(V3Phase,'g',6));


       }

       if(CurPhaseChange!=0)
       {
        switch(CurPhaseChange)
        {
        case 1:
                        I2Phase=I1Phase-120;
                       I3Phase=I1Phase+120;
                       if(I2Phase>180)
                           I2Phase=I2Phase-360;
                       else if(I2Phase<-180)
                           I2Phase+=360;
                       if(I3Phase>180)
                           I3Phase=I3Phase-360;
                       else if(I3Phase<-180)
                           I3Phase+=360;
            break;
        case 2:
                        I1Phase=I2Phase+120;
                          if(I1Phase>180)
                              I1Phase-=360;
                          else if(I1Phase<-180)
                              I1Phase+=360;

                          I3Phase=I1Phase+120;
                          if(I3Phase>180)
                              I3Phase-=360;
                          else if(I3Phase<-180)
                              I3Phase+=360;
            break;

        case 3:
                         I1Phase=I3Phase-120;
                          if(I1Phase>180)
                              I1Phase-=360;
                          else if(I1Phase<-180)
                              I1Phase+=360;
                          I2Phase=I1Phase-120;
                          if(I2Phase>180)
                              I2Phase-=360;
                          else if(I2Phase<-180)
                              I2Phase+=360;
            break;
        }


        ui->lineEdit_11->setText(QString::number(I1Phase,'g',6));
        ui->lineEdit_14->setText(QString::number(I2Phase,'g',6));
        ui->lineEdit_17->setText(QString::number(I3Phase,'g',6));
       }
    }
}

void Manualtest::on_checkBox_8_clicked()
{
    if(VariatyVz2!=1)  //
    {
        if(V1Frec==SysFrec&&V2Frec==SysFrec&&V3Frec==SysFrec)
      {
         ui->checkBox_8->setChecked(true);
         VariatyVz2=1;
        if(VariatyVz==1) //如果Vz=3U0 则取消变Vz
        {
           VariatyVz=0;
           ui->checkBox_9->setChecked(false);
        }
        updateVz();

      }
        else
        {
         ui->checkBox_8->setChecked(false);
         temprorarydata->Warningopenflag;
         Warning *warning=new Warning;
         connect(warning,SIGNAL(WarningWindowClose()),this,SLOT(WarningWinclose()));
         warning->raise();
         warning->activateWindow();
         warning->setModal(true);
         warning->Setwarning(2,"");
         warning->show();

        }
    }
    else
    {
        ui->checkBox_8->setChecked(false);
        VariatyVz2=0;
    }
}

void Manualtest::on_checkBox_9_clicked()
{
    if(VariatyVz==1)
    {
        VariatyVz=0;
        ui->checkBox_9->setChecked(false);
    }
    else
    {
        VariatyVz=1;
        ui->checkBox_9->setChecked(true);

        if(VariatyVz2==1)
        {
            VariatyVz2=0;
            ui->checkBox_8->setChecked(false);

        }

    }


}



void Manualtest::on_pushButton_6_clicked()
{
    if(SoftStartEndflag==true)
            return;
    if(starttest==1)
    {
        ui->pushButton_6->setText(tr("测试"));
        ui->pushButton_6->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }
    else
    {
        ui->pushButton_6->setText(tr("停止"));
        ui->pushButton_6->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
    }

        SoftStartEndflag=true;
}

void Manualtest::GetTestState(){
    if(starttest==1)
    {
        ui->pushButton_6->setText(tr("停止"));
        ui->pushButton_6->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
    }
    else
    {
        ui->pushButton_6->setText(tr("测试"));
        ui->pushButton_6->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }

}
