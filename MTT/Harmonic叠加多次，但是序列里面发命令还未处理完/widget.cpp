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


float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

unsigned char result[70],receive_data[70];
double out_time,change_timedata;
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;

int   input_par=0,HelpFlag=0;
int   starttest=0;
int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC;
float LogicResolution,SystemFrec,Ferequencer,SystemVolt;
int   save_f;
int sys[4];

float I1Value,I2Value,I3Value,I1Phase,I2Phase,I3Phase;
float V1Value,V2Value,V3Value,V1Phase,V2Phase,V3Phase;
float  HarmonicCurrent,HarmonicVoltage,HarmonicCurrentFreq,HarmonicVoltageFreq,TripTime,TripValue;  //谐波电流初值
float  HarmonicCurrentPhase,HarmonicVoltagePhase;
float VaValueReviosn[10],VbValueReviosn[10],VcValueReviosn[10];
float IaValueReviosn[10],IbValueReviosn[10],IcValueReviosn[10];
float V1OutputValue,V2OutputValue,V3OutputValue;
float I1OutputValue,I2OutputValue,I3OutputValue;
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
float VzOutput3Uo=0,VzOutputAc=0,VzOutputDc=0;





float faultduration=10;
float prefaulttime=3;
int   CurrentOutputSel=1;
float HarmonicCurrentPercent;
float HarmonicVoltagePercent;
float HarmonicEnd=5;
float HarmonicStep=0.5;
float HarmonicStepTime=0.1;

float Postfaulttime=3;
float Preparetime;

int   dcouttype=1;

float HarmonicCurrentCount,HarmonicVoltageCount,FixHarmonic,BaseCurrent,BaseCurrentPhase,BaseVoltage,BaseVoltagePhase;
float HarmonicCurrentCount3,HarmonicVoltageCount3,HarmonicCurrent3,HarmonicVoltage3,\
      HarmonicCurrentPhase3,HarmonicVoltagePhase3,HarmonicCurrentFreq3,HarmonicVoltageFreq3,\
      HarmonicCurrentPercent3,HarmonicVoltagePercent3;
float HarmonicCurrentCount4,HarmonicVoltageCount4,HarmonicCurrent4,HarmonicVoltage4,\
      HarmonicCurrentPhase4,HarmonicVoltagePhase4,HarmonicCurrentFreq4,HarmonicVoltageFreq4,\
      HarmonicCurrentPercent4,HarmonicVoltagePercent4;
float HarmonicCurrentCount5,HarmonicVoltageCount5,HarmonicCurrent5,HarmonicVoltage5,\
      HarmonicCurrentPhase5,HarmonicVoltagePhase5,HarmonicCurrentFreq5,HarmonicVoltageFreq5,\
      HarmonicCurrentPercent5,HarmonicVoltagePercent5;
float HarmonicCurrentCount6,HarmonicVoltageCount6,HarmonicCurrent6,HarmonicVoltage6,\
      HarmonicCurrentPhase6,HarmonicVoltagePhase6,HarmonicCurrentFreq6,HarmonicVoltageFreq6,\
      HarmonicCurrentPercent6,HarmonicVoltagePercent6;
bool Threadrunstate=true;
extern TEMPRORARYDATA *temprorarydata;

bool SoftStartEndflag=false;

double V1ampPerStepError=0;
double V2ampPerStepError=0;
double V3ampPerStepError=0;
double VzampPerStepError=0;

double I1ampPerStepError=0;
double I2ampPerStepError=0;
double I3ampPerStepError=0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init_par();
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
    QFont font("wenquanyi",16);
    setFont(font);
    //init_par();
    setFixedSize(800,480);
    sysinit();
    showFullScreen();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();
    ui->label_7->hide();
    ui->label_16->hide();
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
    ui->lineEdit_11->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_27->setEnabled(false);
    ui->lineEdit_28->setEnabled(false);
    ui->lineEdit_29->setEnabled(false);
    ui->lineEdit_30->setEnabled(false);
    QPixmap img(":/picture/hei.png");
    ui->label_13->setPixmap(img);
    ui->label_14->setPixmap(img);
    ui->label_15->setPixmap(img);
    ui->label_7->setPixmap(img);

       //ui->label_11->setText("<font color=blue>"+QString("差动谐波制动(2.2.1)")+"</font>");
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
       ui->lineEdit_17->installEventFilter(this);
       ui->lineEdit_18->installEventFilter(this);
       ui->lineEdit_19->installEventFilter(this);
       ui->lineEdit_20->installEventFilter(this);
       ui->lineEdit_21->installEventFilter(this);
       ui->lineEdit_22->installEventFilter(this);
       ui->lineEdit_23->installEventFilter(this);
       ui->lineEdit_24->installEventFilter(this);
       ui->lineEdit_25->installEventFilter(this);
       ui->lineEdit_26->installEventFilter(this);
       ui->lineEdit_31->installEventFilter(this);
       ui->lineEdit_32->installEventFilter(this);
       ui->lineEdit_33->installEventFilter(this);
       ui->lineEdit_34->installEventFilter(this);
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
       ui->lineEdit_17->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_18->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_19->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_20->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_21->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_22->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_23->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_24->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_25->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_26->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_31->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_32->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_33->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_34->setContextMenuPolicy(Qt::NoContextMenu);

       switch(CurrentOutputSel)
       {
       case 1:
           ui->radioButton->setChecked(true);
           ui->radioButton_2->setChecked(false);
           ui->radioButton_3->setChecked(false);
           break;
       case 2:
           ui->radioButton->setChecked(false);
           ui->radioButton_2->setChecked(true);
           ui->radioButton_3->setChecked(false);
           break;
       case 3:
           ui->radioButton->setChecked(false);
           ui->radioButton_2->setChecked(false);
           ui->radioButton_3->setChecked(true);
           break;
       }


       //ui->pushButton_4->setFocusPolicy(Qt::NoFocus);


       //ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
        //ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
         //ui->pushButton->setFocusPolicy(Qt::NoFocus);


    ui->lineEdit_8->hide();
    currenteditpos=0;
    uartthread =new thread1;
    connect(uartthread,SIGNAL(SIG_binarystate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_testresult(float)),SLOT(showtestresult(float)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_DisplayChangeValue(int,float)),SLOT(displaychange(int,float)));

   connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
   connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
   connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));

    uartthread->start();
}

void Widget::updateUI()
{
    ui->label_11->setText("<font color=blue>"+QString(tr("差动谐波制动(2.2.1)"))+"</font>");
    //this->setWindowTitle(tr("主框架"));

        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_2->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_4->setText(tr("测试"));

        ui->groupBox->setTitle(tr("参数"));
        ui->groupBox_2->setTitle(tr("测试结果"));
        ui->groupBox_3->setTitle(tr("选择基波类型"));

        ui->label->setText(tr("谐波次数:"));
        ui->label_2->setText(tr("谐波含量起始值(%):"));
        ui->label_8->setText(tr("谐波含量终止值(%):"));
        ui->label_9->setText(tr("谐波含量步长(%):"));
        ui->label_10->setText(tr("步长时间(s):"));
        ui->label_3->setText(tr("基波电流(A):"));
        ui->label_4->setText(tr("基波相位(°):"));

}

void Widget::init_par()
{
    FixHarmonic=15;
    BaseCurrent=5;
    BaseCurrentPhase=0;
    BaseVoltage=30;
    BaseVoltagePhase=0;
        //2
        HarmonicCurrentCount=2;
        HarmonicCurrent=0;
        HarmonicCurrentPercent=0;
        HarmonicCurrentPhase=0;
        HarmonicVoltageCount=2;
        HarmonicVoltage=0;
        HarmonicVoltagePercent=0;
        HarmonicVoltagePhase=0;

        //3
        HarmonicCurrentCount3=3;
        HarmonicCurrent3=0;
        HarmonicCurrentPercent3=0;
        HarmonicCurrentPhase3=0;
        HarmonicVoltageCount3=3;
        HarmonicVoltage3=0;
        HarmonicVoltagePercent3=0;
        HarmonicVoltagePhase3=0;
        //4
        HarmonicCurrentCount4=4;
        HarmonicCurrent4=0;
        HarmonicCurrentPercent4=0;
        HarmonicCurrentPhase4=0;
        HarmonicVoltageCount4=4;
        HarmonicVoltage4=0;
        HarmonicVoltagePercent4=0;
        HarmonicVoltagePhase4=0;
        //5
        HarmonicCurrentCount5=5;
        HarmonicCurrent5=0;
        HarmonicCurrentPercent5=0;
        HarmonicCurrentPhase5=0;
        HarmonicVoltageCoun5t=5;
        HarmonicVoltage5=0;
        HarmonicVoltagePercent5=0;
        HarmonicVoltagePhase5=0;
        //6
        HarmonicCurrentCount6=6;
        HarmonicCurrent6=0;
        HarmonicCurrentPercent6=0;
        HarmonicCurrentPhase6=0;
        HarmonicVoltageCoun6t=6;
        HarmonicVoltage6=0;
        HarmonicVoltagePercent6=0;
        HarmonicVoltagePhase6=0;

        V1DC=0;
        V2DC=0;
        V3DC=0;
        VzDC=0;
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

void Widget::sysinit()
{
    ui->lineEdit->setText(QString::number(HarmonicCurrentCount,'g',6));
    ui->lineEdit_2->setText(QString::number(HarmonicCurrentPercent,'g',6));
    ui->lineEdit_3->setText(QString::number(HarmonicEnd,'g',6));
    ui->lineEdit_4->setText(QString::number(HarmonicStep,'g',6));
    ui->lineEdit_5->setText(QString::number(HarmonicStepTime,'g',6));
    ui->lineEdit_6->setText(QString::number(BaseCurrent,'g',6));
    ui->lineEdit_7->setText(QString::number(BaseCurrentPhase,'g',6));
    ui->lineEdit_8->setText(QString::number(faultduration,'g',6));
    ui->lineEdit_9->setText(QString::number(HarmonicCurrent,'g',6));
    ui->lineEdit_10->setText(QString::number(HarmonicCurrentPhase,'g',6));
    ui->lineEdit_11->setText(QString::number(HarmonicVoltagePercent,'g',6));
    ui->lineEdit_12->setText(QString::number(HarmonicVoltageCount,'g',6));
    ui->lineEdit_13->setText(QString::number(HarmonicVoltage,'g',6));
    ui->lineEdit_14->setText(QString::number(HarmonicVoltagePhase,'g',6));
    ui->lineEdit_17->setText(QString::number(BaseVoltagePhase,'g',6));
    ui->lineEdit_18->setText(QString::number(BaseVoltage,'g',6));

    ui->lineEdit_19->setText(QString::number(HarmonicCurrentCount3,'g',6));
    ui->lineEdit_20->setText(QString::number(HarmonicCurrentCount4,'g',6));
    ui->lineEdit_21->setText(QString::number(HarmonicCurrentCount5,'g',6));
    ui->lineEdit_22->setText(QString::number(HarmonicCurrentCount6,'g',6));
    ui->lineEdit_23->setText(QString::number(HarmonicCurrent3,'g',6));
    ui->lineEdit_24->setText(QString::number(HarmonicCurrent4,'g',6));
    ui->lineEdit_25->setText(QString::number(HarmonicCurrent5,'g',6));
    ui->lineEdit_26->setText(QString::number(HarmonicCurrent6,'g',6));
    ui->lineEdit_31->setText(QString::number(HarmonicCurrentPhase3,'g',6));
    ui->lineEdit_32->setText(QString::number(HarmonicCurrentPhase4,'g',6));
    ui->lineEdit_33->setText(QString::number(HarmonicCurrentPhase5,'g',6));
    ui->lineEdit_34->setText(QString::number(HarmonicCurrentPhase6,'g',6));


}


void Widget::showdspstate(int state)
{
    switch(state)
    {
    case 0: ui->label_5->setText(tr("运行正常")); break;
    case 1: ui->label_5->setText(tr("IA 过载")); break;
    case 2: ui->label_5->setText(tr("IABC 温度保护")); break;
    case 3: ui->label_5->setText(tr("IC 间歇")); break;
    case 4: ui->label_5->setText(tr("IB 过载")); break;
    case 5: ui->label_5->setText(tr("IC 闭锁")); break;
    case 6: ui->label_5->setText(tr("IA 间歇")); break;
    case 7: ui->label_5->setText(tr("IC 过载")); break;
    case 8: ui->label_5->setText(tr("IABC 电源保护")); break;
    case 9: ui->label_5->setText(tr("IB 间歇")); break;
    case 10: ui->label_5->setText(tr("VA 过载")); break;

    case 12: ui->label_5->setText(tr("VC 间歇")); break;
    case 13: ui->label_5->setText(tr("VB 过载")); break;
    case 14: ui->label_5->setText(tr("VABC 温度保护")); break;
    case 15: ui->label_5->setText(tr("VA 间歇")); break;
    case 16: ui->label_5->setText(tr("VC 过载")); break;
    case 17: ui->label_5->setText(tr("VABC 电源保护")); break;
    case 18: ui->label_5->setText(tr("Vz 过载")); break;
    case 19: ui->label_5->setText(tr("直流过载")); break;
    case 20: ui->label_5->setText(tr("IABC 电源保护")); break;
    case 21: ui->label_5->setText(tr("VC 间歇")); break;


    }
    QPixmap img(":/picture/hei.png");
    QPixmap pic(":/picture/hong.png");
    switch(state)
    {
    case 100:
        ui->label_13->setPixmap(img);
        ui->label_14->setPixmap(img);
        ui->label_15->setPixmap(img);
        ui->label_7->setPixmap(img);
        break;
    case 101: ui->label_7->setPixmap(pic); break;
    case 102: ui->label_13->setPixmap(pic); break;
    case 103: ui->label_14->setPixmap(pic); break;
    case 104: ui->label_15->setPixmap(pic); break;
    case 111: ui->label_7->setPixmap(img); break;
    case 112: ui->label_13->setPixmap(img); break;
    case 113: ui->label_14->setPixmap(img); break;
    case 114: ui->label_15->setPixmap(img); break;
    }
}


void Widget::showtestresult(float num)
{
    if(num==-2)
    {
    ui->label_6->setText(tr(""));
    return;
    }
    if(num==-1)
    {
    ui->label_6->setText(tr("动作值:未动作"));
    }
    else
    {
    ui->label_6->setText(tr("动作值:")+QString::number(num,'g',6)+tr("%"));
    }


}



void Widget::displaychange(int num,float value)
{
 if(num==0)
 ui->label_12->clear();
 else
 ui->label_12->setText(tr("谐波含量:")+QString::number(value,'g',4)+tr("%"));




}

void Widget::vi_parameter_enter(int position)
{
    switch(position)
    {
    case 1:
        if(HarmonicCurrentCount>50)
            HarmonicCurrentCount=50;
        else if(HarmonicCurrentCount<0)
            HarmonicCurrentCount=0;//参数限制
        break;
   case 2:
        if(HarmonicCurrentPercent>100)
            HarmonicCurrentPercent=100;
        else if(HarmonicCurrentPercent<0)
            HarmonicCurrentPercent=0;//参数限制
        if(HarmonicCurrentPercent<=HarmonicEnd)
            HarmonicCurrentPercent=HarmonicEnd+HarmonicStep;
//        if(HarmonicCurrentPercent>HarmonicEnd+20)
//            HarmonicEnd=HarmonicCurrentPercent-20;
        break;
   case 3:
        if(HarmonicEnd>100)
            HarmonicEnd=100;
        else if(HarmonicEnd<0)
            HarmonicEnd=0;//参数限制
        if(HarmonicEnd>=HarmonicCurrentPercent)
            HarmonicEnd=HarmonicCurrentPercent-HarmonicStep;
//        if(HarmonicEnd<HarmonicCurrentPercent-20)
//            HarmonicCurrentPercent=HarmonicEnd+20;
        break;
   case 4:
         if(HarmonicStep>100)
             HarmonicStep=100;
         else if(HarmonicStep<0)
             HarmonicStep=0;//参数限制
         if(HarmonicStep>fabs(HarmonicCurrentPercent-HarmonicEnd))
             HarmonicStep=fabs(HarmonicCurrentPercent-HarmonicEnd);
         break;
   case 5:
        if(HarmonicStepTime>999.99)
            HarmonicStepTime=999.99;
        else if(HarmonicStepTime<0)
            HarmonicStepTime=0;//参数限制
        break;

   case 6:
        if(BaseCurrent>MAX_I_VALUEDATA_AC)
          BaseCurrent=MAX_I_VALUEDATA_AC;
        else if(BaseCurrent<0)
          BaseCurrent=0;//参数限制
        break;
   case 7:
        if(BaseCurrentPhase>180)
          BaseCurrentPhase=180;
        else if(BaseCurrentPhase<-180)
          BaseCurrentPhase=-180;//参数限制
            break;
   case 8:
        if(faultduration>999.99)
            faultduration=999.99;
        else if(faultduration<0)
            faultduration=0;//参数限制
              break;
    case 9:
         if(HarmonicCurrent>MAX_I_VALUEDATA_AC)
           HarmonicCurrent=MAX_I_VALUEDATA_AC;
         else if(HarmonicCurrent<0)
           HarmonicCurrent=0;//参数限制
         HarmonicCurrentPercent = 100*HarmonicCurrent/BaseCurrent;
         ui->lineEdit_2->setText(QString::number(HarmonicCurrentPercent,'g',6));
             break;
    case 10:
         if(HarmonicCurrentPhase>180)
             HarmonicCurrentPhase=180;
         else if(HarmonicCurrentPhase<0)
             HarmonicCurrentPhase=0;//参数限制
              break;
    case 11:
         if(HarmonicVoltagePercent>100)
             HarmonicVoltagePercent=100;
         else if(HarmonicVoltagePercent<0)
             HarmonicVoltagePercent=0;//参数限制
              break;
    case 12:
        if(HarmonicVoltageCount>50)
            HarmonicVoltageCount=50;
        else if(HarmonicVoltageCount<0)
            HarmonicVoltageCount=0;//参数限制
        break;
    case 13:
         if(HarmonicVoltage>MAX_V_VALUEDATA_AC)
           HarmonicVoltage=MAX_V_VALUEDATA_AC;
         else if(HarmonicVoltage<0)
           HarmonicVoltage=0;//参数限制
         HarmonicVoltagePercent = 100*HarmonicVoltage/BaseVoltage;
         ui->lineEdit_11->setText(QString::number(HarmonicVoltagePercent,'g',6));
             break;
    case 14:
         if(HarmonicVoltagePhase>180)
             HarmonicVoltagePhase=180;
         else if(HarmonicVoltagePhase<0)
             HarmonicVoltagePhase=0;//参数限制
              break;
    case 17:
         if(BaseVoltagePhase>180)
             BaseVoltagePhase=180;
         else if(BaseVoltagePhase<0)
             BaseVoltagePhase=0;//参
              break;
    case 18:
         if(BaseVoltage>MAX_V_VALUEDATA_AC)
           BaseVoltage=MAX_V_VALUEDATA_AC;
         else if(BaseVoltage<0)
           BaseVoltage=0;//参数限制
         break;
    case 19:
        if(HarmonicCurrentCount3>50)
            HarmonicCurrentCount3=50;
        else if(HarmonicCurrentCount3<0)
            HarmonicCurrentCount3=0;//参数限制
        break;
    case 20:
        if(HarmonicCurrentCount4>50)
            HarmonicCurrentCount4=50;
        else if(HarmonicCurrentCount4<0)
            HarmonicCurrentCount4=0;//参数限制
        break;
    case 21:
        if(HarmonicCurrentCount5>50)
            HarmonicCurrentCount5=50;
        else if(HarmonicCurrentCount5<0)
            HarmonicCurrentCount5=0;//参数限制
        break;
    case 22:
        if(HarmonicCurrentCount6>50)
            HarmonicCurrentCount6=50;
        else if(HarmonicCurrentCount6<0)
            HarmonicCurrentCount6=0;//参数限制
        break;
    case 23:
         if(HarmonicCurrent3>MAX_I_VALUEDATA_AC)
           HarmonicCurrent3=MAX_I_VALUEDATA_AC;
         else if(HarmonicCurrent3<0)
           HarmonicCurrent3=0;//参数限制
         HarmonicCurrentPercent3 = 100*HarmonicCurrent3/BaseCurrent;
         ui->lineEdit_27->setText(QString::number(HarmonicCurrentPercent3,'g',6));
             break;
    case 24:
         if(HarmonicCurrent4>MAX_I_VALUEDATA_AC)
           HarmonicCurrent4=MAX_I_VALUEDATA_AC;
         else if(HarmonicCurrent4<0)
           HarmonicCurrent4=0;//参数限制
         HarmonicCurrentPercent4 = 100*HarmonicCurrent4/BaseCurrent;
         ui->lineEdit_28->setText(QString::number(HarmonicCurrentPercent4,'g',6));
             break;
    case 25:
         if(HarmonicCurrent5>MAX_I_VALUEDATA_AC)
           HarmonicCurrent5=MAX_I_VALUEDATA_AC;
         else if(HarmonicCurrent5<0)
           HarmonicCurrent5=0;//参数限制
         HarmonicCurrentPercent5 = 100*HarmonicCurrent5/BaseCurrent;
         ui->lineEdit_29->setText(QString::number(HarmonicCurrentPercent5,'g',6));
             break;
    case 26:
         if(HarmonicCurrent6>MAX_I_VALUEDATA_AC)
           HarmonicCurrent6=MAX_I_VALUEDATA_AC;
         else if(HarmonicCurrent6<0)
           HarmonicCurrent6=0;//参数限制
         HarmonicCurrentPercent6 = 100*HarmonicCurrent6/BaseCurrent;
         ui->lineEdit_30->setText(QString::number(HarmonicCurrentPercent6,'g',6));
             break;
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
    case 1:
        HarmonicCurrentCount=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentCount,'g',6));
        break;

    case 2:
        HarmonicCurrentPercent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentPercent,'g',6));
        ui->lineEdit_3->setText(QString::number(HarmonicEnd,'g',6));
        break;
    case 3:
        HarmonicEnd=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicEnd,'g',6));
        ui->lineEdit_2->setText(QString::number(HarmonicCurrentPercent,'g',6));
        break;
    case 4:
        HarmonicStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicStep,'g',6));
        break;
    case 5:
        HarmonicStepTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicStepTime,'g',6));
        break;

    case 6:
        BaseCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(BaseCurrent,'g',6));
        break;
    case 7:
        BaseCurrentPhase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(BaseCurrentPhase,'g',6));
        break;
    case 8:
        faultduration=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(faultduration,'g',6));
        break;
    case 9:
        HarmonicCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrent,'g',6));
        break;
    case 10:
        HarmonicCurrentPhase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentPhase,'g',6));
        break;
    case 11:
        HarmonicVoltagePercent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicVoltagePercent,'g',6));
        break;
    case 12:
        HarmonicVoltageCount=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicVoltageCount,'g',6));
        break;
    case 13:
        HarmonicVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicVoltage,'g',6));
        break;
    case 14:
        HarmonicVoltagePhase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicVoltagePhase,'g',6));
        break;
    case 17:
        BaseVoltagePhase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(BaseVoltagePhase,'g',6));
        break;
    case 18:
        BaseVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(BaseVoltage,'g',6));
    case 19:
        HarmonicCurrentCount3=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentCount3,'g',6));
    case 20:
        HarmonicCurrentCount4=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentCount4,'g',6));
    case 21:
        HarmonicCurrentCount5=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentCount5,'g',6));
    case 22:
        HarmonicCurrentCount6=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentCount6,'g',6));
    case 23:
        HarmonicCurrent3=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrent3,'g',6));
    case 24:
        HarmonicCurrent4=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrent4,'g',6));
    case 25:
        HarmonicCurrent5=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrent5,'g',6));
    case 26:
        HarmonicCurrent6=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrent6,'g',6));
    case 31:
        HarmonicCurrentPhase3=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentPhase3,'g',6));
    case 32:
        HarmonicCurrentPhase4=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentPhase4,'g',6));
    case 33:
        HarmonicCurrentPhase5=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentPhase5,'g',6));
    case 34:
        HarmonicCurrentPhase6=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCurrentPhase6,'g',6));
        break;
    }

}

void Widget::clearkeypadflag()
{
    temprorarydata->keypadopenflag=false;
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
temprorarydata->helpopenflag=true;
connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
if(current_language==2)
helpdialog->SetHelpfilename("Harmonic.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("Harmonic1.txt");
//helpdialog->SetHelpfilename("Harmonic.txt");
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

                strcpy(strbuf,"谐波测试");
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"基波电流(A):");
                fprintf(fp,"%s",strbuf);
                gcvt(BaseCurrent,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"动作值(%):");
                fprintf(fp,"%s",strbuf);
                if(TripValue==-1)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripValue,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                fclose(fp);
                system("sync");
        }
        else
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

            strcpy(strbuf,"Harmonic test result");
            fprintf(fp,"%s\r\n",strbuf);

            strcpy(strbuf,"Baseband BaseCurrent(A):");
            fprintf(fp,"%s",strbuf);
            gcvt(BaseCurrent,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

            strcpy(strbuf,"Trip value(%):");
            fprintf(fp,"%s",strbuf);
            if(TripValue==-1)
              strcpy(strbuf,"No Trip");
            else
              gcvt(TripValue,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

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

void Widget::CloseWidget()
{

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



void Widget::HelpDialogClose()
{
temprorarydata->helpopenflag=false;

}


void Widget::moveEvent(QMoveEvent *event)
{
 QPoint oldpos=event->oldPos();
 this->move(oldpos);
}



bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if(starttest==1)
         return QWidget::eventFilter(obj,event);


    if(event->type()==QEvent::MouseButtonPress)
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
        else if(obj==ui->lineEdit_31)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_31;
              currenteditpos=31;
         }
        else if(obj==ui->lineEdit_32)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_32;
              currenteditpos=32;
         }
        else if(obj==ui->lineEdit_33)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_33;
              currenteditpos=33;
         }
        else if(obj==ui->lineEdit_34)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_34;
              currenteditpos=34;
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
  /*  else if(event->type()==QEvent::FocusOut)
    {
        if(obj==ui->lineEdit)
        {
               QString text=ui->lineEdit->text();
               HarmonicCurrentCount= text.toFloat();
               currenteditpos=1;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit->setText(QString::number(HarmonicCurrentCount,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               HarmonicCurrentPercent= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(HarmonicCurrentPercent,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                HarmonicEnd= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(HarmonicEnd,'g',6));
         }
        else if(obj==ui->lineEdit_4)
         {
                QString text=ui->lineEdit_4->text();
                HarmonicStep= text.toFloat();
                currenteditpos=4;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_4->setText(QString::number(HarmonicStep,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                HarmonicStepTime= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(HarmonicStepTime,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                BaseCurrent= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(BaseCurrent,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                BaseCurrentPhase= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(BaseCurrentPhase,'g',6));
         }
        else if(obj==ui->lineEdit_8)
        {
               QString text=ui->lineEdit_8->text();
               faultduration= text.toFloat();
               currenteditpos=8;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_8->setText(QString::number(faultduration,'g',6));
        }
        else if(obj==ui->lineEdit_9)
         {
                QString text=ui->lineEdit_9->text();
                HarmonicCurrent= text.toFloat();
                currenteditpos=9;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_9->setText(QString::number(HarmonicCurrent,'g',6));
         }
        else if(obj==ui->lineEdit_10)
        {
               QString text=ui->lineEdit_10->text();
               HarmonicCurrentPhase= text.toFloat();
               currenteditpos=10;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_10->setText(QString::number(HarmonicCurrentPhase,'g',6));
        }
        else if(obj==ui->lineEdit_11)
        {
               QString text=ui->lineEdit_11->text();
               HarmonicVoltagePercent= text.toFloat();
               currenteditpos=11;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_11->setText(QString::number(HarmonicVoltagePercent,'g',6));
        }
        else if(obj==ui->lineEdit_12)
        {
               QString text=ui->lineEdit_12->text();
               HarmonicVoltageCount= text.toFloat();
               currenteditpos=12;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_12->setText(QString::number(HarmonicVoltageCount,'g',6));
        }
        else if(obj==ui->lineEdit_13)
        {
               QString text=ui->lineEdit_13->text();
               HarmonicVoltage= text.toFloat();
               currenteditpos=13;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_13->setText(QString::number(HarmonicVoltage,'g',6));
        }
        else if(obj==ui->lineEdit_14)
        {
               QString text=ui->lineEdit_14->text();
               HarmonicVoltagePhase= text.toFloat();
               currenteditpos=14;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_14->setText(QString::number(HarmonicVoltagePhase,'g',6));
        }
        else if(obj==ui->lineEdit_17)
        {
               QString text=ui->lineEdit_17->text();
               BaseVoltagePhase= text.toFloat();
               currenteditpos=17;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_17->setText(QString::number(BaseVoltagePhase,'g',6));
        }
        else if(obj==ui->lineEdit_18)
        {
               QString text=ui->lineEdit_18->text();
               BaseVoltage= text.toFloat();
               currenteditpos=18;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_18->setText(QString::number(BaseVoltage,'g',6));
        }
        return QWidget::eventFilter(obj,event);
    }
    */
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

void Widget::on_radioButton_clicked()
{
CurrentOutputSel=1;
}

void Widget::on_radioButton_2_clicked()
{
CurrentOutputSel=2;
}

void Widget::on_radioButton_3_clicked()
{
CurrentOutputSel=3;
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
