#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDate>
#include <QTableWidgetItem>
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>
#include <QMessageBox>
#include "ComplexCaculate.h"
#include <math.h>
#include <unistd.h>

#include "qtranslator.h"
QTranslator *tor;
int current_language=2;  //1中文  2英文

float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

//extern variate define
unsigned char result[70],receive_data[70];
double out_time,change_timedata=0;

float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;

int   cursorF=0,cursor=0,cursorK=0;
int   input_par=0,Decimal=0,sige_flag=0,HelpFlag=0;



long auto_i=0,IRQcnt=0,auto_count=0,tice=0;




int   INPAR=0;
int   cx,cy,cx_start,cx_mid,cx_end,starttest=0,low,row,page;
int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC,TestResult;

int   FaultForm,FaultType;
float KeepTime,LoadCurrent,Impedance,ImpedanceAngle,FixCurrent,FixTime,KL,WaitTime,DelayTime;
float timeout,FristTripTime,CloseTripTime,SecondTripTime;
int exit_par,save_f;
//exit_par：退出程序标志。save_f：保存结果标志，为1表示有新的测试结果，可以保存，为0表示没有新的测试结果
int sys[4];
char ParStr[20];  //ParStr:为参数输入时记录的输入参数的字符串。
float  V1PerFaultValue,V2PerFaultValue,V3PerFaultValue,I1PerFaultValue,I2PerFaultValue,I3PerFaultValue;
float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase,I1PerFaultPhase,I2PerFaultPhase,I3PerFaultPhase;
float  V1FaultValue,V2FaultValue,V3FaultValue,I1FaultValue,I2FaultValue,I3FaultValue;
float  V1FaultPhase,V2FaultPhase,V3FaultPhase,I1FaultPhase,I2FaultPhase,I3FaultPhase;
float  V1PostFaultValue,V2PostFaultValue,V3PostFaultValue,I1PostFaultValue,I2PostFaultValue,I3PostFaultValue;
float  V1PostFaultPhase,V2PostFaultPhase,V3PostFaultPhase,I1PostFaultPhase,I2PostFaultPhase,I3PostFaultPhase;
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

float LogicResolution,SystemFrec,SystemVolt;
int   VzOutput3U0=0;
float VzOutputAc=0,VzOutputDc=0;

float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;



 float faultduration=0;
 extern TEMPRORARYDATA *temprorarydata;

 bool Busvolcon=true;//2016-6-8
 float VabFaultValue,VbcFaultValue,VcaFaultValue;
 float VabFaultPhase,VbcFaultPhase,VcaFaultPhase;

 float KLphase=0;
bool Threadrunstate=true;

bool SoftStartEndflag=false;



float Postfaulttime=3;
float prefaulttime=3;
float Preparetime=3;
int   dcouttype=1;

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
    //ui->label_11->setText("<font color=blue>"+QString("整组试验(2.2.1)")+"</font>");


    showFullScreen();
    ui->label_17->hide();
    ui->label_14->hide();
    ui->label_15->hide();
    ui->label_16->hide();
    ui->label_18->hide();
    ui->label_19->hide();
    ui->label_20->hide();
    ui->label_21->hide();

    QPixmap img(":/picture/hei.png");
    ui->label_17->setPixmap(img);
    ui->label_14->setPixmap(img);
    ui->label_15->setPixmap(img);
    ui->label_16->setPixmap(img);

    ui->checkBox->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
     Busvolcon=true;
     if(current_language==2)
         Busvolcon=false;
    ui->checkBox->setChecked(Busvolcon);
    ui->checkBox->setHidden(true);
    ui->pushButton_2->hide();
    ui->lineEdit_2->hide();




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


     //ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
      //ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
       //ui->pushButton->setFocusPolicy(Qt::NoFocus);

        //ui->pushButton_5->setFocusPolicy(Qt::NoFocus);


    ui->tabWidget->setCurrentIndex(0);
    uartthread =new thread1;
    connect(uartthread,SIGNAL(SIG_binarystate(int)),SLOT(showdspstate(int)));

    connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
    connect(uartthread,SIGNAL(SIG_testresult(int)),SLOT(showtestresult(int)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_sendwarning(int)),SLOT(showwarnint(int)));
    connect(uartthread,SIGNAL(SIG_changetab(int)),SLOT(changetab(int)));
    connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
    connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));
    uartthread->start();


}
void Widget::updateUI()
{
    ui->label_11->setText("<font color=blue>"+QString(tr("整组试验(2.2.1)"))+"</font>");

    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_3->setText(tr("保存"));
        ui->pushButton_4->setText(tr("退出"));
        ui->pushButton_5->setText(tr("测试"));

        ui->radioButton->setText(tr("临时性故障"));
        ui->radioButton_2->setText(tr("永久性故障"));

        ui->groupBox->setTitle(tr("故障模式"));
        ui->groupBox_2->setTitle(tr("故障类型"));
        ui->groupBox_3->setTitle(tr("故障前时间(s)"));
        ui->groupBox_4->setTitle(tr("故障参数"));
        ui->groupBox_5->setTitle(tr("零序补偿系数"));
        ui->groupBox_6->setTitle(tr("重合闸参数"));

        ui->label->setText(tr("时间(s):"));
        ui->label_3->setText(tr("故障电流(A):"));
        ui->label_4->setText(tr("故障时间(s):"));
        ui->label_5->setText(tr("短路阻抗(Ω):"));
        ui->label_6->setText(tr("灵敏角(Deg):"));
        ui->label_8->setText(tr("等待时间(s):"));
        ui->label_9->setText(tr("开关重合延时(s):"));
        ui->label_12->setText(tr("动作开入: 开关量输入1"));
        ui->label_2->setText(tr("重合闸开入: 开关量输入2"));
        ui->label_13->setText(tr("相位(度):"));

        ui->tabWidget->setTabText(0,tr("设置"));
        ui->tabWidget->setTabText(1,tr("计算结果"));
        ui->tabWidget->setTabText(2,tr("测试结果"));
}
void Widget::init_par()
{
        FaultForm=1;
        FaultType=1;
        KeepTime=25;
        LoadCurrent=0;
        FixCurrent=5;
        FixTime=2;
        Impedance=1;
        ImpedanceAngle=75;
        KL=0.67;
        WaitTime=25;
        DelayTime=0;
        V1DC=0;
        V2DC=0;
        V3DC=0;
        VzDC=0;
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
        sys[0]=1;
        sys[1]=1;
        sys[2]=1;
        sys[3]=1;
        LogicResolution=3;
        SystemFrec=25;
        SystemVolt=57.72;
        FILE *fp;
        if((fp=fopen("sysset.txt","a+"))==NULL)
        return;
        //fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&KeepTime,&Postfaulttime,&dcouttype);
        fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
               &SystemVolt,&LogicResolution,&prefaulttime,&Postfaulttime,&Preparetime,&current_language);
        fclose(fp);

}


void Widget::disfaulttype(int type)
{
    switch(type)
    {
    case 1:
           ui->radioButton_3->setChecked(true);
           ui->radioButton_4->setChecked(false);
           ui->radioButton_5->setChecked(false);
           ui->radioButton_6->setChecked(false);
           ui->radioButton_7->setChecked(false);
           ui->radioButton_8->setChecked(false);
           ui->radioButton_9->setChecked(false);
           break;
    case 2:
           ui->radioButton_3->setChecked(false);
           ui->radioButton_4->setChecked(true);
           ui->radioButton_5->setChecked(false);
           ui->radioButton_6->setChecked(false);
           ui->radioButton_7->setChecked(false);
           ui->radioButton_8->setChecked(false);
           ui->radioButton_9->setChecked(false);
           break;
    case 3:
           ui->radioButton_3->setChecked(false);
           ui->radioButton_4->setChecked(false);
           ui->radioButton_5->setChecked(true);
           ui->radioButton_6->setChecked(false);
           ui->radioButton_7->setChecked(false);
           ui->radioButton_8->setChecked(false);
           ui->radioButton_9->setChecked(false);
           break;
    case 4:
           ui->radioButton_3->setChecked(false);
           ui->radioButton_4->setChecked(false);
           ui->radioButton_5->setChecked(false);
           ui->radioButton_6->setChecked(true);
           ui->radioButton_7->setChecked(false);
           ui->radioButton_8->setChecked(false);
           ui->radioButton_9->setChecked(false);
           break;
    case 5:
           ui->radioButton_3->setChecked(false);
           ui->radioButton_4->setChecked(false);
           ui->radioButton_5->setChecked(false);
           ui->radioButton_6->setChecked(false);
           ui->radioButton_7->setChecked(true);
           ui->radioButton_8->setChecked(false);
           ui->radioButton_9->setChecked(false);
           break;
    case 6:
           ui->radioButton_3->setChecked(false);
           ui->radioButton_4->setChecked(false);
           ui->radioButton_5->setChecked(false);
           ui->radioButton_6->setChecked(false);
           ui->radioButton_7->setChecked(false);
           ui->radioButton_8->setChecked(true);
           ui->radioButton_9->setChecked(false);
           break;
    case 7:
           ui->radioButton_3->setChecked(false);
           ui->radioButton_4->setChecked(false);
           ui->radioButton_5->setChecked(false);
           ui->radioButton_6->setChecked(false);
           ui->radioButton_7->setChecked(false);
           ui->radioButton_8->setChecked(false);
           ui->radioButton_9->setChecked(true);
           break;
    }
}


void Widget::sysinit()
{
    if(FaultForm==1)
        ui->radioButton->setChecked(true);
    else if(FaultForm==2)
        ui->radioButton->setChecked(true);
    disfaulttype(FaultType);
    ui->lineEdit->setText(QString::number(KeepTime,'g',3));
    ui->lineEdit_2->setText(QString::number(LoadCurrent,'g',3));
    ui->lineEdit_3->setText(QString::number(FixCurrent,'g',3));
    ui->lineEdit_4->setText(QString::number(FixTime,'g',3));
    ui->lineEdit_5->setText(QString::number(Impedance,'g',3));
    ui->lineEdit_6->setText(QString::number(ImpedanceAngle,'g',3));
    ui->lineEdit_7->setText(QString::number(KL,'g',3));
    ui->lineEdit_8->setText(QString::number(WaitTime,'g',3));
    ui->lineEdit_9->setText(QString::number(DelayTime,'g',3));
    ui->lineEdit_10->setText(QString::number(KLphase,'g',3));

}


void Widget::showwarnint(int num)
{
    if(num==0)
     {
        return;
     }
    char temp1[400];
    char temp2[100];
  if(current_language==1)
  {
    if(num==1)
        sprintf(temp1,"电压超过%.3fV,",SystemVolt);
    else   if(num==2)
        sprintf(temp1,"电压低于%.3fV,",SystemVolt/2);
    sprintf(temp2,"%s 请改变电流值.",temp1);
    QMessageBox::warning(this,"警告",temp2,QMessageBox::Ok);
  }
  else
  {
    if(num==1)
        sprintf(temp1,"The voltage is more than %.3fV,",SystemVolt);
    else   if(num==2)
        sprintf(temp1,"The voltage is else than %.3fV,",SystemVolt/2);
    sprintf(temp2,"%s please change the fixed current value.",temp1);
    QMessageBox::warning(this,"Warning",temp2,QMessageBox::Ok);
  }

}

void Widget::showdspstate(int state)
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
        ui->label_17->setPixmap(img);
        ui->label_14->setPixmap(img);
        ui->label_15->setPixmap(img);
        ui->label_16->setPixmap(img);
        break;
    case 101: ui->label_14->setPixmap(pic); break;
    case 102: ui->label_15->setPixmap(pic); break;
    case 103: ui->label_16->setPixmap(pic); break;
    case 104: ui->label_17->setPixmap(pic); break;
    case 111: ui->label_14->setPixmap(img); break;
    case 112: ui->label_15->setPixmap(img); break;
    case 113: ui->label_16->setPixmap(img); break;
    case 114: ui->label_17->setPixmap(img); break;
    }
}

void Widget::changetab(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}


void Widget::showtestresult(int num)
{
    if(num==0)
    {
     ui->textEdit->clear();
     return;
    }
     if(current_language==1)
     {
          if(TestResult)
          {
             //故障形式 FaultForm
            switch(FaultForm)
            {
            case 1:ui->textEdit->append(QString("故障模式:临时性故障\n"));break;
            case 2:ui->textEdit->append(QString("故障模式:永久性故障\n"));break;
            }

            //故障类型 FaultType
            switch(FaultType)
            {
            case 1:ui->textEdit->append(QString("故障类型:A-E"));break;
            case 2:ui->textEdit->append(QString("故障类型:B-E"));break;
            case 3:ui->textEdit->append(QString("故障类型:C-E"));break;
            case 4:ui->textEdit->append(QString("故障类型:A-B"));break;
            case 5:ui->textEdit->append(QString("故障类型:B-C"));break;
            case 6:ui->textEdit->append(QString("故障类型:C-A"));break;
            case 7:ui->textEdit->append(QString("故障类型:A-B-C"));break;
            }

            //一次性故障时间
          /* if(FristTripTime==-1)
              {
                ui->textEdit->append(QString("一次故障动作时间(s):未动作\n"));
                ui->textEdit->append(QString("重合闸时间(s):未重合\n"));

                if(FaultForm==2)
                  {
                    ui->textEdit->append(QString("二次故障动作时间(s):未动作\n"));
                  }
              }
            else if(FristTripTime!=-1)
              {


                ui->textEdit->append(QString("一次故障动作时间(s):%1\n").arg(QString::number(FristTripTime,'g',3)));
                if(CloseTripTime!=-1)
                ui->textEdit->append(QString("重合闸时间(s):%1\n").arg(QString::number(CloseTripTime,'g',3)));
                else if(CloseTripTime==-1)
                ui->textEdit->append(QString("重合闸时间(s):未重合\n"));

                if(FaultForm==2)
                  {
                     if(SecondTripTime==-1)
                         ui->textEdit->append(QString("二次故障动作时间(s):未动作\n"));
                        else
                         ui->textEdit->append(QString("二次故障动作时间(s):%1\n").arg(QString::number(SecondTripTime,'g',3)));
                  }
              }
            */
            if(FristTripTime==-1)
                ui->textEdit->append(QString("一次故障动作时间(s):未动作\n"));
            else if(FristTripTime!=-1)
                ui->textEdit->append(QString("一次故障动作时间(s):%1\n").arg(QString::number(FristTripTime,'g',3)));
            if(CloseTripTime!=-1)
                ui->textEdit->append(QString("重合闸时间(s):%1\n").arg(QString::number(CloseTripTime,'g',3)));
            else if(CloseTripTime==-1)
                ui->textEdit->append(QString("重合闸时间(s):未重合\n"));
            if(FaultForm==2)
                  {
                     if(SecondTripTime==-1)
                         ui->textEdit->append(QString("二次故障动作时间(s):未动作\n"));
                     else
                         ui->textEdit->append(QString("二次故障动作时间(s):%1\n").arg(QString::number(SecondTripTime,'g',3)));
                  }

          }
     }
     else
     {
         if(TestResult)
         {
            //故障形式 FaultForm
           switch(FaultForm)
           {
           case 1:ui->textEdit->append(QString("fault mode:Temporary fault\n"));break;
           case 2:ui->textEdit->append(QString("fault mode:permanent fault\n"));break;
           }

           //故障类型 FaultType
           switch(FaultType)
           {
           case 1:ui->textEdit->append(QString("fault type:A-E"));break;
           case 2:ui->textEdit->append(QString("fault type:B-E"));break;
           case 3:ui->textEdit->append(QString("fault type:C-E"));break;
           case 4:ui->textEdit->append(QString("fault type:A-B"));break;
           case 5:ui->textEdit->append(QString("fault type:B-C"));break;
           case 6:ui->textEdit->append(QString("fault type:C-A"));break;
           case 7:ui->textEdit->append(QString("fault type:A-B-C"));break;
           }
           if(FristTripTime==-1)
               ui->textEdit->append(QString("first fault action time(s):no trip\n"));
           else if(FristTripTime!=-1)
               ui->textEdit->append(QString("first fault action time(s):%1\n").arg(QString::number(FristTripTime,'g',3)));
           if(CloseTripTime!=-1)
               ui->textEdit->append(QString("reclosing time(s):%1\n").arg(QString::number(CloseTripTime,'g',3)));
           else if(CloseTripTime==-1)
               ui->textEdit->append(QString("reclosing time(s):no reclosing\n"));
           if(FaultForm==2)
                 {
                    if(SecondTripTime==-1)
                        ui->textEdit->append(QString("second fault action time(s):no trip\n"));
                    else
                        ui->textEdit->append(QString("second fault action time(s):%1\n").arg(QString::number(SecondTripTime,'g',3)));
                 }

         }

     }

}


void Widget::vi_parameter_enter(int position)
{
       float Maxfactor=1;
     if(FaultType>=1&&FaultType<=3)
        Maxfactor=1;
     else  if(FaultType>=4&&FaultType<=6)
        Maxfactor=2;
     else  if(FaultType==7)
        Maxfactor=3;




        switch(position)
        {
        case 1:
            if(KeepTime>999.99)
              KeepTime=999.99;
            else if(KeepTime<0)
              KeepTime=0;//参数限制


            break;
        case 2:
            if(LoadCurrent>MAX_I_VALUEDATA_AC*Maxfactor)
              LoadCurrent=MAX_I_VALUEDATA_AC*Maxfactor;
            else if(LoadCurrent<0)
              LoadCurrent=0;//参数限制

            break;
        case 3:
            if(FixCurrent>MAX_I_VALUEDATA_AC*Maxfactor)
              FixCurrent=MAX_I_VALUEDATA_AC*Maxfactor;
            else if(FixCurrent<0)
              FixCurrent=0;//参数限制

            break;
        case 4:
            if(FixTime>999.99)
               FixTime=999.99;
             else if(FixTime<0)
               FixTime=0;//参数限制

            break;
        case 5:
            if(Impedance>999.99)
              Impedance=999.99;
            else if(Impedance<0)
              Impedance=0;//参数限制

            break;
        case 6:
            if(ImpedanceAngle>360)
              ImpedanceAngle=360;
            else if(ImpedanceAngle<0)
              ImpedanceAngle=0;//参数限制

            break;
        case 7:
            if(KL>999.99)
              KL=999.99;
            else if(KL<0)
              KL=0;//参数限制

            break;
        case 8:
            if(WaitTime>999.99)
              WaitTime=999.99;
            else if(WaitTime<0)
              WaitTime=0;//参数限制

            break;
        case 9:
            if(DelayTime>999.99)
              DelayTime=999.99;
            else if(DelayTime<0)
              DelayTime=0;//参数限制
            break;
        case 10:
            if(KLphase>360)
              KLphase=0;
            else if(KLphase<-360)
              KLphase=0;//参数限制
            break;

        }

}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_3_clicked()//save
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

                    strcpy(strbuf,"整组测试");
                    fprintf(fp,"%s\r\n",strbuf);

                    strcpy(strbuf,"故障模式:");
                    fprintf(fp,"%s",strbuf);
                    if(FaultForm==1)
                      strcpy(strbuf,"临时性故障");
                    else if(FaultForm==2)
                      strcpy(strbuf,"永久性故障");
                    fprintf(fp,"%s\r\n",strbuf);

                    strcpy(strbuf,"故障类型: ");
        fprintf(fp,"%s",strbuf);
        switch(FaultType)
          {
            case 1:
              strcpy(strbuf,"A-E");
              break;
            case 2:
              strcpy(strbuf,"B-E");
              break;
            case 3:
              strcpy(strbuf,"C-E");
              break;
            case 4:
              strcpy(strbuf,"A-B");
              break;
            case 5:
              strcpy(strbuf,"B-C");
              break;
            case 6:
              strcpy(strbuf,"C-A");
              break;
            case 7:
              strcpy(strbuf,"A-B-C");
              break;
          }
        fprintf(fp,"%s\r\n",strbuf);

        if(FaultForm==1)
          {
            strcpy(strbuf,"一次故障动作时间(s):");
            fprintf(fp,"%s",strbuf);
            if(FristTripTime==-1)
              strcpy(strbuf,"未动作");
            else
              gcvt(FristTripTime,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

            strcpy(strbuf,"重合闸时间(s):");
            fprintf(fp,"%s",strbuf);
            if(CloseTripTime==-1)
              strcpy(strbuf,"未重合");
            else
              gcvt(CloseTripTime,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

          }

          else if(FaultForm==2)
          {
            strcpy(strbuf,"一次故障动作时间(s):");
            fprintf(fp,"%s",strbuf);
            if(FristTripTime==-1)
              strcpy(strbuf,"未动作");
            else
              gcvt(FristTripTime,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

            strcpy(strbuf,"重合闸时间(s):");
            fprintf(fp,"%s",strbuf);
            if(CloseTripTime==-1)
              strcpy(strbuf,"未重合");
            else
              gcvt(CloseTripTime ,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

            strcpy(strbuf,"二次故障动作时间(s):");
            fprintf(fp,"%s",strbuf);
            if(SecondTripTime==-1)
              strcpy(strbuf,"未动作");
            else
              gcvt(SecondTripTime ,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);
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

                strcpy(strbuf,"Auto-Reclosing test result");
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"Fault mode:");
                fprintf(fp,"%s",strbuf);
                if(FaultForm==1)
                  strcpy(strbuf,"Temporary fault");
                else if(FaultForm==2)
                  strcpy(strbuf,"Permanent fault");
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"Fault type: ");
    fprintf(fp,"%s",strbuf);
    switch(FaultType)
      {
        case 1:
          strcpy(strbuf,"A-E");
          break;
        case 2:
          strcpy(strbuf,"B-E");
          break;
        case 3:
          strcpy(strbuf,"C-E");
          break;
        case 4:
          strcpy(strbuf,"A-B");
          break;
        case 5:
          strcpy(strbuf,"B-C");
          break;
        case 6:
          strcpy(strbuf,"C-A");
          break;
        case 7:
          strcpy(strbuf,"A-B-C");
          break;
      }
    fprintf(fp,"%s\r\n",strbuf);

    if(FaultForm==1)
      {
        strcpy(strbuf,"1st Fault Trip Time(s):");
        fprintf(fp,"%s",strbuf);
        if(FristTripTime==-1)
          strcpy(strbuf,"No Trip");
        else
          gcvt(FristTripTime,7,strbuf);
        fprintf(fp,"%s\r\n",strbuf);

        strcpy(strbuf,"Reclosing time(s):");
        fprintf(fp,"%s",strbuf);
        if(CloseTripTime==-1)
          strcpy(strbuf,"No Reclose");
        else
          gcvt(CloseTripTime,7,strbuf);
        fprintf(fp,"%s\r\n",strbuf);

      }

      else if(FaultForm==2)
      {
        strcpy(strbuf,"1st Fault Trip Time(s):");
        fprintf(fp,"%s",strbuf);
        if(FristTripTime==-1)
          strcpy(strbuf,"No Trip");
        else
          gcvt(FristTripTime,7,strbuf);
        fprintf(fp,"%s\r\n",strbuf);

        strcpy(strbuf,"Reclosing time(s):");
        fprintf(fp,"%s",strbuf);
        if(CloseTripTime==-1)
          strcpy(strbuf,"No Reclose");
        else
          gcvt(CloseTripTime ,7,strbuf);
        fprintf(fp,"%s\r\n",strbuf);

        strcpy(strbuf,"2nd Fault Trip Time(s):");
        fprintf(fp,"%s",strbuf);
        if(SecondTripTime==-1)
          strcpy(strbuf,"No Trip");
        else
          gcvt(SecondTripTime ,7,strbuf);
        fprintf(fp,"%s\r\n",strbuf);
      }
    fclose(fp);
    system("sync");
        }
}

void Widget::on_pushButton_4_clicked()
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

void Widget::on_radioButton_clicked()
{
   FaultForm=1;
}

void Widget::on_radioButton_2_clicked()
{
  FaultForm=2;
}

void Widget::on_radioButton_3_clicked()
{
    FaultType=1;
}

void Widget::on_radioButton_4_clicked()
{
  FaultType=2;
}

void Widget::on_radioButton_5_clicked()
{
  FaultType=3;
}

void Widget::on_radioButton_6_clicked()
{
  FaultType=4;
}

void Widget::on_radioButton_7_clicked()
{
   FaultType=5;
}

void Widget::on_radioButton_8_clicked()
{
   FaultType=6;
}

void Widget::on_radioButton_9_clicked()
{
  FaultType=7;
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
        KeepTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(KeepTime,'g',6));
        break;
    case 2:
        LoadCurrent=number;//参数限制
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(LoadCurrent,'g',6));

        break;
    case 3:
        FixCurrent=number;//参数限制
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixCurrent,'g',6));
        break;
    case 4:
        FixTime=number;//参数限制
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixTime,'g',6));
        break;
    case 5:
        Impedance=number;//参数限制
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Impedance,'g',6));
        break;
    case 6:
        ImpedanceAngle=number;//参数限制
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(ImpedanceAngle,'g',6));
        break;
    case 7:
        KL=number;//参数限制

        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(KL,'g',6));
        break;
    case 8:
        WaitTime=number;//参数限制
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(WaitTime,'g',6));
        break;
    case 9:
        DelayTime=number;//参数限制
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DelayTime,'g',6));
        break;
    case 10:
        KLphase=number;//参数限制
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(KLphase,'g',6));
        break;
    }


  }


void Widget::clearkeypadflag()
{
temprorarydata->keypadopenflag=false;

}




void Widget::on_pushButton_clicked()
{
    if(starttest==1)
        return;

Helpdialog *helpdialog=new Helpdialog;
temprorarydata->helpopenflag=true;
connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
if(current_language==2)
helpdialog->SetHelpfilename("Systemtest.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("Systemtest1.txt");
//helpdialog->SetHelpfilename("Systemtest.txt");
helpdialog->show();
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
    else if(event->type()==QEvent::FocusOut)
    {
        if(obj==ui->lineEdit)
        {
               QString text=ui->lineEdit->text();
               KeepTime= text.toFloat();
               currenteditpos=1;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit->setText(QString::number(KeepTime,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               LoadCurrent= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(LoadCurrent,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                FixCurrent= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(FixCurrent,'g',6));
         }
        else if(obj==ui->lineEdit_4)
         {
                QString text=ui->lineEdit_4->text();
                FixTime= text.toFloat();
                currenteditpos=4;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_4->setText(QString::number(FixTime,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                Impedance= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(Impedance,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                ImpedanceAngle= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(ImpedanceAngle,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                KL= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(KL,'g',6));
         }
        else if(obj==ui->lineEdit_8)
        {
               QString text=ui->lineEdit_8->text();
               WaitTime= text.toFloat();
               currenteditpos=8;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_8->setText(QString::number(WaitTime,'g',6));
        }
        else if(obj==ui->lineEdit_9)
         {
                QString text=ui->lineEdit_9->text();
                DelayTime= text.toFloat();
                currenteditpos=9;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_9->setText(QString::number(DelayTime,'g',6));
         }
        else if(obj==ui->lineEdit_10)
        {
               QString text=ui->lineEdit_10->text();
               KLphase= text.toFloat();
               currenteditpos=10;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_10->setText(QString::number(KLphase,'g',6));
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
    ui->pushButton_5->setFocus();
}


void Widget::LLLNormalFaultPar() //三相 相间故障
  { V1PerFaultValue=SystemVolt;//57.73;
    V2PerFaultValue=SystemVolt;//57.73;
    V3PerFaultValue=SystemVolt;//57.73;
    I1PerFaultValue=LoadCurrent;
    I2PerFaultValue=LoadCurrent;
    I3PerFaultValue=LoadCurrent;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //故障前电压每相输出57.73V，相位对称，电流输出为0。

    V1PostFaultValue=SystemVolt;//SystemVolt;//57.73;
    V2PostFaultValue=SystemVolt;//57.73;
    V3PostFaultValue=SystemVolt;//57.73;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;
    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
  }

void Widget::LLNormalFaultPar()
  { //两相相间故障 ，故障前和故障后参数
    V1PerFaultValue=SystemVolt;//57.73;
    V2PerFaultValue=SystemVolt;//57.73;
    V3PerFaultValue=SystemVolt;//57.73;
    I1PerFaultValue=LoadCurrent;
    I2PerFaultValue=LoadCurrent;
    I3PerFaultValue=LoadCurrent;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //故障前电压每相输出57.73V，相位对称，电流输出为0。

    V1PostFaultValue=SystemVolt;//57.73;
    V2PostFaultValue=SystemVolt;//57.73;
    V3PostFaultValue=SystemVolt;//57.73;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;
    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //故障后电压每相输出57.73V，相位对称，电流输出为0。
  }

void Widget::LNNormalFaultPar()
  {//单相接地故障 ，故障前和故障后参数
    V1PerFaultValue=SystemVolt;//57.73;
    V2PerFaultValue=SystemVolt;//57.73;
    V3PerFaultValue=SystemVolt;//57.73;
    I1PerFaultValue=LoadCurrent;
    I2PerFaultValue=LoadCurrent;
    I3PerFaultValue=LoadCurrent;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //故障前电压每相输出57.73V，相位对称，电流输出为0。

    V1PostFaultValue=SystemVolt;//57.73;
    V2PostFaultValue=SystemVolt;//57.73;
    V3PostFaultValue=SystemVolt;//57.73;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;
    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //故障后电压每相输出57.73V，相位对称，电流输出为0。
  }



int Widget::OutputSignalCompute(int Fault)
{
      //flag:表示某段的故障类型。
    COMPLEX C;
    C.x=3*KL*cos(KLphase*3.1415/180)+2;
    C.y=3*KL*sin(KLphase*3.1415/180);
    float Angel_C=CaculatePhase(C.y,C.x);
    if(Angel_C>=180)
        Angel_C-=360;
    else if(Angel_C<=-180)
      Angel_C+=360;

    switch(Fault)
      {
        case 1:
          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=0;
          I3Output=0;
          LNNormalFaultPar();
          I1FaultValue=FixCurrent;
          if(Busvolcon==true)
        {
          COMPLEX Va=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
          V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
          V2FaultValue=SystemVolt;//SystemVolt;
          V3FaultValue=SystemVolt;//SystemVolt;
          I2FaultValue=0;
          I3FaultValue=0;
          //单相接地故障时，未发生故障的电压为Vnom（SystemVolt），未发生故障的电流为0。
          V1FaultPhase=CaculatePhase(Va.y,Va.x);
          V2FaultPhase=-120+V1FaultPhase;
          V3FaultPhase=120+V1FaultPhase;
        }
        else
          {
              I2FaultValue=0;
              I3FaultValue=0;

              COMPLEX Va=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
              COMPLEX Vb=VbCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
              COMPLEX Vc=VcCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);

              V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
              V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
              V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
              V1FaultPhase=CaculatePhase(Va.y,Va.x);
              V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
              V3FaultPhase=CaculatePhase(Vc.y,Vc.x);

              V1FaultPhase=fmod(V1FaultPhase,360);
              V2FaultPhase=fmod(V2FaultPhase,360);
              V3FaultPhase=fmod(V3FaultPhase,360);
          }



          I1FaultPhase=0-ImpedanceAngle-Angel_C;
          I2FaultPhase=I1FaultPhase-120;
          I3FaultPhase=I1FaultPhase+120;
          LNNormalFaultPar();
          break;
        case 2://B-E鏁呴殰
          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=1;
          LNNormalFaultPar();
          I2FaultValue=FixCurrent;

          if(Busvolcon==true)
        {
          COMPLEX Vb=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
          V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
          V1FaultValue=SystemVolt;//57.73;
          V3FaultValue=SystemVolt;//57.73;
          I1FaultValue=LoadCurrent;
          I3FaultValue=LoadCurrent;
          //鍗曠浉鎺ュ湴鏁呴殰 锛屾湭鍙戠敓鏁呴殰鐨勭數鍘嬩负Vnom锛7.73锛夛紝鏈彂鐢熸晠闅滅殑鐢垫祦涓銆
          V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
          V1FaultPhase=V2FaultPhase+120;
          V3FaultPhase=V2FaultPhase-120;
        }
        else
        {
            I1FaultValue=0;
            I3FaultValue=0;
            COMPLEX Vb=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
            COMPLEX Vc=VbCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
            COMPLEX Va=VcCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);

            V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
            V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
            V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
            V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
            V1FaultPhase=CaculatePhase(Va.y,Va.x);
            V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
            V1FaultPhase=fmod(V1FaultPhase,360);
            V2FaultPhase=fmod(V2FaultPhase,360);
            V3FaultPhase=fmod(V3FaultPhase,360);

        }



          I2FaultPhase=0-ImpedanceAngle-Angel_C;
          I1FaultPhase=I2FaultPhase+120;
          I3FaultPhase=I2FaultPhase-120;
          LNNormalFaultPar();
          break;
        case 3://C-E鏁呴殰
          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=1;
          LNNormalFaultPar();
          I3FaultValue=FixCurrent;

          if(Busvolcon==true)
         {
          COMPLEX Vc=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
          V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
          V1FaultValue=SystemVolt;//SystemVolt;
          V2FaultValue=SystemVolt;//SystemVolt;
          I1FaultValue=0;
          I2FaultValue=0;
          V3FaultPhase=CaculatePhase(Vc.y,Vc.x);
          V2FaultPhase=120+V3FaultPhase;
          V1FaultPhase=V3FaultPhase-120;
         }

          else
          {

              I1FaultValue=0;
              I2FaultValue=0;
              COMPLEX Vc=VaCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
              COMPLEX Va=VbCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);
              COMPLEX Vb=VcCaculate(KL,KLphase,FixCurrent,Impedance,ImpedanceAngle);


              V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
              V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
              V3FaultValue=sqrt(Vc.x*Vc.x+Vc.y*Vc.y);
              V1FaultPhase=CaculatePhase(Va.y,Va.x);
              V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
              V3FaultPhase=CaculatePhase(Vc.y,Vc.x);

              //相故障时，非故障电压的相位不变，
              V1FaultPhase=fmod(V1FaultPhase,360);
              V2FaultPhase=fmod(V2FaultPhase,360);
              V3FaultPhase=fmod(V3FaultPhase,360);

          }



          I3FaultPhase=0-ImpedanceAngle-Angel_C;
          I1FaultPhase=I3FaultPhase-120;
          I2FaultPhase=I3FaultPhase+120;
          LNNormalFaultPar();
          break;
        case 4://A-B鏁呴殰
        {  V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=1;
          LLNormalFaultPar();
          I1FaultValue=FixCurrent;
          I2FaultValue=FixCurrent;
          I3FaultValue=I3PerFaultValue;

          COMPLEX Va1=V1LLCaculate(FixCurrent,Impedance,SystemVolt);
          COMPLEX Vb1=V2LLCaculate(FixCurrent,Impedance,SystemVolt);
          V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
          V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);

          V3FaultValue=V3PerFaultValue;
          V3FaultPhase=0;
          V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
          V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);





          V1FaultPhase=fmod(V1FaultPhase,360);
          V2FaultPhase=fmod(V2FaultPhase,360);
          if(V1FaultPhase>180)
            V1FaultPhase=V1FaultPhase-360;
          else if(V1FaultPhase<-180)
            V1FaultPhase=V1FaultPhase+360;
          if(V2FaultPhase>180)
            V2FaultPhase=V2FaultPhase-360;
          else if(V2FaultPhase<-180)
            V2FaultPhase=V2FaultPhase+360;





          I3FaultPhase=V3FaultPhase-ImpedanceAngle;
          I1FaultPhase=I3FaultPhase-90;
          I2FaultPhase=I3FaultPhase+90;
          if(I1FaultPhase>180)
            I1FaultPhase=I1FaultPhase-360;
          else if(I1FaultPhase<-180)
            I1FaultPhase=I1FaultPhase+360;
          if(I2FaultPhase>180)
            I2FaultPhase=I2FaultPhase-360;
          else if(I2FaultPhase<-180)
            I2FaultPhase=I2FaultPhase+360;

          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
      }

          break;
        case 5://B-C鏁呴殰
        {  V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=1;

          LLNormalFaultPar();
          I2FaultValue=FixCurrent;
          I3FaultValue=FixCurrent;
          I1FaultValue=I1PerFaultValue;

          V1FaultValue=V1PerFaultValue;
          V1FaultPhase=0;
          COMPLEX Vb2=V1LLCaculate(FixCurrent,Impedance,SystemVolt);
          COMPLEX Vc2=V2LLCaculate(FixCurrent,Impedance,SystemVolt);
          V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
          V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);

          V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
          V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);


          V2FaultPhase=fmod(V2FaultPhase,360);
          V3FaultPhase=fmod(V3FaultPhase,360);
          if(V2FaultPhase>180)
            V2FaultPhase=V2FaultPhase-360;
          else if(V2FaultPhase<-180)
            V2FaultPhase=V2FaultPhase+360;
          if(V3FaultPhase>180)
            V3FaultPhase=V3FaultPhase-360;
          else if(V3FaultPhase<-180)
            V3FaultPhase=V3FaultPhase+360;




          I1FaultPhase=V1FaultPhase-ImpedanceAngle;
          I2FaultPhase=I1FaultPhase-90;
          I3FaultPhase=I1FaultPhase+90;


          if(I2FaultPhase>180)
            I2FaultPhase=I2FaultPhase-360;
          else if(I2FaultPhase<-180)
            I2FaultPhase=I2FaultPhase+360;
          if(I3FaultPhase>180)
            I3FaultPhase=I3FaultPhase-360;
          else if(I3FaultPhase<-180)
            I3FaultPhase=I3FaultPhase+360;

          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
      }

          break;
        case 6://C-A鏁呴殰
        {  V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=1;
          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
          I1FaultValue=FixCurrent;//鐩搁棿鏁呴殰 锛屾晠闅滅浉鐢垫祦鐨勫箙鍊鍥哄畾鐢垫祦
          I3FaultValue=FixCurrent;//鐩搁棿鏁呴殰 锛屾晠闅滅浉鐢垫祦鐨勫箙鍊鍥哄畾鐢垫祦
          I2FaultValue=I2PerFaultValue;//鐩搁棿鏁呴殰 锛岄潪鏁呴殰鐩哥數娴佺殑骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

          V2FaultValue=V2PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
          V2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅
          COMPLEX Vc3=V1LLCaculate(FixCurrent,Impedance,SystemVolt);
          COMPLEX Va3=V2LLCaculate(FixCurrent,Impedance,SystemVolt);
          V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
          V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);

          V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
          V1FaultPhase=CaculatePhase(Va3.y,Va3.x);

          V1FaultPhase=fmod(V1FaultPhase,360);
          V3FaultPhase=fmod(V3FaultPhase,360);
          if(V3FaultPhase>180)
            V3FaultPhase=V3FaultPhase-360;
          else if(V3FaultPhase<-180)
            V3FaultPhase=V3FaultPhase+360;
          if(V1FaultPhase>180)
            V1FaultPhase=V1FaultPhase-360;
          else if(V1FaultPhase<-180)
            V1FaultPhase=V1FaultPhase+360;





          I2FaultPhase=0-ImpedanceAngle;//鐩搁棿鏁呴殰 ,闈炴晠闅滅浉鐢垫祦鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佺殑鐩镐綅
          I3FaultPhase=I2FaultPhase-90;
          I1FaultPhase=I2FaultPhase+90;
          if(I3FaultPhase>180)
            I3FaultPhase=I3FaultPhase-360;
          else if(I3FaultPhase<-180)
            I3FaultPhase=I3FaultPhase+360;

          if(I1FaultPhase>180)
            I1FaultPhase=I1FaultPhase-360;
          else if(I1FaultPhase<-180)
            I1FaultPhase=I1FaultPhase+360;

          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
      }

          break;
        case 7://A-B-C鏁呴殰
          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=1;

          LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

          I1FaultValue=FixCurrent;//涓夌浉鐩搁棿鏁呴殰 锛屾晠闅滅浉鐢垫祦鐨勫箙鍊鍥哄畾鐢垫祦
          I2FaultValue=FixCurrent;//涓夌浉鐩搁棿鏁呴殰 锛屾晠闅滅浉鐢垫祦鐨勫箙鍊鍥哄畾鐢垫祦
          I3FaultValue=FixCurrent;//涓夌浉鐩搁棿鏁呴殰 锛屾晠闅滅浉鐢垫祦鐨勫箙鍊鍥哄畾鐢垫祦

          V1FaultValue=FixCurrent*Impedance;
          //涓夌浉鐩搁棿鏁呴殰 锛孶=I*Z锛堢浉鐢靛帇=鍥哄畾鐢垫祦*闃绘姉瀹氬€硷級
          V2FaultValue=V1FaultValue;
          V3FaultValue=V1FaultValue;
          //涓夌浉鐩搁棿鏁呴殰 锛孷1銆乂2銆乂3骞呭€煎ぇ灏忕浉绛

          //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
          V1FaultPhase=V1PerFaultPhase;
          V2FaultPhase=V2PerFaultPhase;
          V3FaultPhase=V3PerFaultPhase;







          I1FaultPhase=V1FaultPhase-ImpedanceAngle;

          I2FaultPhase=I1FaultPhase-120;
          I3FaultPhase=I1FaultPhase+120;

          if(I1FaultPhase>180)
            I1FaultPhase=I1FaultPhase-360;
          else if(I1FaultPhase<-180)
            I1FaultPhase=I1FaultPhase+360;
          if(I2FaultPhase>180)
            I2FaultPhase=I2FaultPhase-360;
          else if(I2FaultPhase<-180)
            I2FaultPhase=I2FaultPhase+360;


          if(I3FaultPhase>180)
            I3FaultPhase=I3FaultPhase-360;
          else if(I3FaultPhase<-180)
            I3FaultPhase=I3FaultPhase+360;
          //涓夌浉鐩搁棿鏁呴殰 锛岀浉浣嶆垚120搴﹀绉板叧绯汇€
          LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
          break;
      }

  return 0;
}





extern bool checkiszero(float x);

float Widget::CaculatePhase(float y,float r)
{
    if(checkiszero(r)==true&&checkiszero(y)==true)
        return 0;
    else if(r>0&&checkiszero(y)==true)
        return 0;
    else if(r<0&&checkiszero(y)==true)
        return 180;
    else if(checkiszero(r)==true&&y>0)
        return 90;
    else if(checkiszero(r)==true&&y<0)
        return -90;

    float angle=atan(y/r)*180/3.1415;
    if(r<0)
     angle=angle+180;

    if(angle>360)
        angle=angle-360;
    else  if(angle<0)
        angle=angle+360;

     return angle;
}

void Widget::CaculatePerfalultPhase()
{

    V1PerFaultPhase=V1FaultPhase-360*SystemFrec*2*KeepTime;
    V2PerFaultPhase=V2FaultPhase-360*SystemFrec*2*KeepTime;
    V3PerFaultPhase=V3FaultPhase-360*SystemFrec*2*KeepTime;
    V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
    V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
    V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;

    I1PerFaultPhase=I1FaultPhase-360*SystemFrec*2*KeepTime;
    I2PerFaultPhase=I2FaultPhase-360*SystemFrec*2*KeepTime;
    I3PerFaultPhase=I3FaultPhase-360*SystemFrec*2*KeepTime;
    I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
    I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
    I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;

    if(V1PerFaultPhase>180)
        V1PerFaultPhase=V1PerFaultPhase-360;
    else if(V1PerFaultPhase<-180)
        V1PerFaultPhase=V1PerFaultPhase+360;

    if(V2PerFaultPhase>180)
        V2PerFaultPhase=V2PerFaultPhase-360;
    else if(V2PerFaultPhase<-180)
        V2PerFaultPhase=V2PerFaultPhase+360;

    if(V3PerFaultPhase>180)
        V3PerFaultPhase=V3PerFaultPhase-360;
    else if(V3PerFaultPhase<-180)
        V3PerFaultPhase=V3PerFaultPhase+360;

    if(I1PerFaultPhase>180)
        I1PerFaultPhase=I1PerFaultPhase-360;
    else if(I1PerFaultPhase<-180)
        I1PerFaultPhase=I1PerFaultPhase+360;

    if(I2PerFaultPhase>180)
        I2PerFaultPhase=I2PerFaultPhase-360;
    else if(I2PerFaultPhase<-180)
        I2PerFaultPhase=I2PerFaultPhase+360;

    if(I3PerFaultPhase>180)
        I3PerFaultPhase=I3PerFaultPhase-360;
    else if(I3PerFaultPhase<-180)
        I3PerFaultPhase=I3PerFaultPhase+360;

    if(I1FaultPhase>180)
        I1FaultPhase=I1FaultPhase-360;
    else if(I1FaultPhase<-180)
        I1FaultPhase=I1FaultPhase+360;

    if(I2FaultPhase>180)
        I2FaultPhase=I2FaultPhase-360;
    else if(I2FaultPhase<-180)
        I2FaultPhase=I2FaultPhase+360;

    if(I3FaultPhase>180)
        I3FaultPhase=I3FaultPhase-360;
    else if(I3FaultPhase<-180)
        I3FaultPhase=I3FaultPhase+360;
}



void Widget::on_checkBox_clicked()
{
    Busvolcon=!Busvolcon;
}

void Widget::on_pushButton_2_clicked()
{


}

void Widget::on_textBrowser_textChanged()
{
  ui->textBrowser->moveCursor(QTextCursor::Start);
}

void Widget::on_tabWidget_currentChanged(int index)
{

    if(index!=1)
        return;


    ui->textBrowser->clear();
    ui->tabWidget->setCurrentIndex(1);
    OutputSignalCompute(FaultType);

    CaculatePerfalultPhase();


    switch(FaultType)
    {
    case 1:
        ui->textBrowser->append(tr("故障类型:A-E"));

        V1PerFaultPhase=V2PerFaultPhase+120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V3PerFaultPhase=V2PerFaultPhase-120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
        if(V1PerFaultPhase>180)
            V1PerFaultPhase=V1PerFaultPhase-360;
        else if(V1PerFaultPhase<-180)
            V1PerFaultPhase=V1PerFaultPhase+360;
        if(V2PerFaultPhase>180)
            V2PerFaultPhase=V2PerFaultPhase-360;
        else if(V2PerFaultPhase<-180)
            V2PerFaultPhase=V2PerFaultPhase+360;
        if(V3PerFaultPhase>180)
            V3PerFaultPhase=V3PerFaultPhase-360;
        else if(V3PerFaultPhase<-180)
            V3PerFaultPhase=V3PerFaultPhase+360;
        I1PerFaultPhase=I2PerFaultPhase+120;
        I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
        I3PerFaultPhase=I2PerFaultPhase-120;
        I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        if(I1PerFaultPhase>180)
            I1PerFaultPhase=I1PerFaultPhase-360;
        else if(I1PerFaultPhase<-180)
            I1PerFaultPhase=I1PerFaultPhase+360;
        if(I2PerFaultPhase>180)
            I2PerFaultPhase=I2PerFaultPhase-360;
        else if(I2PerFaultPhase<-180)
            I2PerFaultPhase=I2PerFaultPhase+360;
        if(I3PerFaultPhase>180)
            I3PerFaultPhase=I3PerFaultPhase-360;
        else if(I3PerFaultPhase<-180)
            I3PerFaultPhase=I3PerFaultPhase+360;

        break;

    case 2:
        ui->textBrowser->append(tr("故障类型:B-E"));

        V1PerFaultPhase=V3PerFaultPhase-120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V2PerFaultPhase=V3PerFaultPhase+120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        if(V1PerFaultPhase>180)
            V1PerFaultPhase=V1PerFaultPhase-360;
        else if(V1PerFaultPhase<-180)
            V1PerFaultPhase=V1PerFaultPhase+360;
        if(V2PerFaultPhase>180)
            V2PerFaultPhase=V2PerFaultPhase-360;
        else if(V2PerFaultPhase<-180)
            V2PerFaultPhase=V2PerFaultPhase+360;
        if(V3PerFaultPhase>180)
            V3PerFaultPhase=V3PerFaultPhase-360;
        else if(V3PerFaultPhase<-180)
            V3PerFaultPhase=V3PerFaultPhase+360;
        I1PerFaultPhase=I3PerFaultPhase-120;
        I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
        I2PerFaultPhase=I3PerFaultPhase+120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        if(I1PerFaultPhase>180)
            I1PerFaultPhase=I1PerFaultPhase-360;
        else if(I1PerFaultPhase<-180)
            I1PerFaultPhase=I1PerFaultPhase+360;
        if(I2PerFaultPhase>180)
            I2PerFaultPhase=I2PerFaultPhase-360;
        else if(I2PerFaultPhase<-180)
            I2PerFaultPhase=I2PerFaultPhase+360;
        if(I3PerFaultPhase>180)
            I3PerFaultPhase=I3PerFaultPhase-360;
        else if(I3PerFaultPhase<-180)
            I3PerFaultPhase=I3PerFaultPhase+360;

        break;

    case 3:
        ui->textBrowser->append(tr("故障类型:C-E"));

        V2PerFaultPhase=V1PerFaultPhase-120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        V3PerFaultPhase=V1PerFaultPhase+120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
        if(V1PerFaultPhase>180)
            V1PerFaultPhase=V1PerFaultPhase-360;
        else if(V1PerFaultPhase<-180)
            V1PerFaultPhase=V1PerFaultPhase+360;
        if(V2PerFaultPhase>180)
            V2PerFaultPhase=V2PerFaultPhase-360;
        else if(V2PerFaultPhase<-180)
            V2PerFaultPhase=V2PerFaultPhase+360;
        if(V3PerFaultPhase>180)
            V3PerFaultPhase=V3PerFaultPhase-360;
        else if(V3PerFaultPhase<-180)
            V3PerFaultPhase=V3PerFaultPhase+360;
        I2PerFaultPhase=I1PerFaultPhase-120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        I3PerFaultPhase=I1PerFaultPhase+120;
        I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        if(I1PerFaultPhase>180)
            I1PerFaultPhase=I1PerFaultPhase-360;
        else if(I1PerFaultPhase<-180)
            I1PerFaultPhase=I1PerFaultPhase+360;
        if(I2PerFaultPhase>180)
            I2PerFaultPhase=I2PerFaultPhase-360;
        else if(I2PerFaultPhase<-180)
            I2PerFaultPhase=I2PerFaultPhase+360;
        if(I3PerFaultPhase>180)
            I3PerFaultPhase=I3PerFaultPhase-360;
        else if(I3PerFaultPhase<-180)
            I3PerFaultPhase=I3PerFaultPhase+360;

        break;

    case 4:
        ui->textBrowser->append(tr("故障类型:A-B"));

        V1PerFaultPhase=V3PerFaultPhase-120;
        V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
        V2PerFaultPhase=V3PerFaultPhase+120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        if(V1PerFaultPhase>180)
            V1PerFaultPhase=V1PerFaultPhase-360;
        else if(V1PerFaultPhase<-180)
            V1PerFaultPhase=V1PerFaultPhase+360;
        if(V2PerFaultPhase>180)
            V2PerFaultPhase=V2PerFaultPhase-360;
        else if(V2PerFaultPhase<-180)
            V2PerFaultPhase=V2PerFaultPhase+360;
        if(V3PerFaultPhase>180)
            V3PerFaultPhase=V3PerFaultPhase-360;
        else if(V3PerFaultPhase<-180)
            V3PerFaultPhase=V3PerFaultPhase+360;
        I1PerFaultPhase=I3PerFaultPhase-120;
        I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
        I2PerFaultPhase=I3PerFaultPhase+120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        if(I1PerFaultPhase>180)
            I1PerFaultPhase=I1PerFaultPhase-360;
        else if(I1PerFaultPhase<-180)
            I1PerFaultPhase=I1PerFaultPhase+360;
        if(I2PerFaultPhase>180)
            I2PerFaultPhase=I2PerFaultPhase-360;
        else if(I2PerFaultPhase<-180)
            I2PerFaultPhase=I2PerFaultPhase+360;
        if(I3PerFaultPhase>180)
            I3PerFaultPhase=I3PerFaultPhase-360;
        else if(I3PerFaultPhase<-180)
            I3PerFaultPhase=I3PerFaultPhase+360;

        break;

    case 5:
        ui->textBrowser->append(tr("故障类型:B-C"));

        V2PerFaultPhase=V1PerFaultPhase-120;
        V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
        V3PerFaultPhase=V1PerFaultPhase+120;
        V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
        if(V1PerFaultPhase>180)
            V1PerFaultPhase=V1PerFaultPhase-360;
        else if(V1PerFaultPhase<-180)
            V1PerFaultPhase=V1PerFaultPhase+360;
        if(V2PerFaultPhase>180)
            V2PerFaultPhase=V2PerFaultPhase-360;
        else if(V2PerFaultPhase<-180)
            V2PerFaultPhase=V2PerFaultPhase+360;
        if(V3PerFaultPhase>180)
            V3PerFaultPhase=V3PerFaultPhase-360;
        else if(V3PerFaultPhase<-180)
            V3PerFaultPhase=V3PerFaultPhase+360;
        I2PerFaultPhase=I1PerFaultPhase-120;
        I2PerFaultPhase=I2PerFaultPhase-((int)(I2PerFaultPhase/360))*360;
        I3PerFaultPhase=I1PerFaultPhase+120;
        I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
        if(I1PerFaultPhase>180)
            I1PerFaultPhase=I1PerFaultPhase-360;
        else if(I1PerFaultPhase<-180)
            I1PerFaultPhase=I1PerFaultPhase+360;
        if(I2PerFaultPhase>180)
            I2PerFaultPhase=I2PerFaultPhase-360;
        else if(I2PerFaultPhase<-180)
            I2PerFaultPhase=I2PerFaultPhase+360;
        if(I3PerFaultPhase>180)
            I3PerFaultPhase=I3PerFaultPhase-360;
        else if(I3PerFaultPhase<-180)
            I3PerFaultPhase=I3PerFaultPhase+360;

        break;

    case 6:
         ui->textBrowser->append(tr("故障类型:C-A"));

         V1PerFaultPhase=V2PerFaultPhase+120;
         V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
         V3PerFaultPhase=V2PerFaultPhase-120;
         V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
         if(V1PerFaultPhase>180)
             V1PerFaultPhase=V1PerFaultPhase-360;
         else if(V1PerFaultPhase<-180)
             V1PerFaultPhase=V1PerFaultPhase+360;
         if(V2PerFaultPhase>180)
             V2PerFaultPhase=V2PerFaultPhase-360;
         else if(V2PerFaultPhase<-180)
             V2PerFaultPhase=V2PerFaultPhase+360;
         if(V3PerFaultPhase>180)
             V3PerFaultPhase=V3PerFaultPhase-360;
         else if(V3PerFaultPhase<-180)
             V3PerFaultPhase=V3PerFaultPhase+360;
         I1PerFaultPhase=I2PerFaultPhase+120;
         I1PerFaultPhase=I1PerFaultPhase-((int)(I1PerFaultPhase/360))*360;
         I3PerFaultPhase=I2PerFaultPhase-120;
         I3PerFaultPhase=I3PerFaultPhase-((int)(I3PerFaultPhase/360))*360;
         if(I1PerFaultPhase>180)
             I1PerFaultPhase=I1PerFaultPhase-360;
         else if(I1PerFaultPhase<-180)
             I1PerFaultPhase=I1PerFaultPhase+360;
         if(I2PerFaultPhase>180)
             I2PerFaultPhase=I2PerFaultPhase-360;
         else if(I2PerFaultPhase<-180)
             I2PerFaultPhase=I2PerFaultPhase+360;
         if(I3PerFaultPhase>180)
             I3PerFaultPhase=I3PerFaultPhase-360;
         else if(I3PerFaultPhase<-180)
             I3PerFaultPhase=I3PerFaultPhase+360;

        break;

    case 7:
        ui->textBrowser->append(tr("故障类型:A-B-C"));
        break;
    }

    ui->textBrowser->append(tr("故障前电压:"));
    ui->textBrowser->append(tr("VA(V):")+QString::number(V1PerFaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(V1PerFaultPhase,'g',6));
    ui->textBrowser->append(tr("VB(V):")+QString::number(V2PerFaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(V2PerFaultPhase,'g',6));
    ui->textBrowser->append(tr("VC(V):")+QString::number(V3PerFaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(V3PerFaultPhase,'g',6));

    ui->textBrowser->append(tr("故障前电流:"));
    ui->textBrowser->append(tr("IA(A):")+QString::number(I1PerFaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(I1PerFaultPhase,'g',6));
    ui->textBrowser->append(tr("IB(A):")+QString::number(I2PerFaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(I2PerFaultPhase,'g',6));
    ui->textBrowser->append(tr("IC(A):")+QString::number(I3PerFaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(I3PerFaultPhase,'g',6));



    ui->textBrowser->append(tr("故障电压:"));
    ui->textBrowser->append(tr("VA(V):")+QString::number(V1FaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(V1FaultPhase,'g',6));
    ui->textBrowser->append(tr("VB(V):")+QString::number(V2FaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(V2FaultPhase,'g',6));
    ui->textBrowser->append(tr("VC(V):")+QString::number(V3FaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(V3FaultPhase,'g',6));
    switch(FaultType)
    {
    case 4:
        ui->textBrowser->append(tr("VAB(V):")+QString::number(VabFaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(VabFaultPhase,'g',6));
        break;
    case 5:
        ui->textBrowser->append(tr("VBC(V):")+QString::number(VbcFaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(VbcFaultPhase,'g',6));
        break;
    case 6:
        ui->textBrowser->append(tr("VCA(V):")+QString::number(VbcFaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(VcaFaultPhase,'g',6));
        break;
    }




    ui->textBrowser->append(tr("故障电流:"));
    ui->textBrowser->append(tr("IA(A):")+QString::number(I1FaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(I1FaultPhase,'g',6));
    ui->textBrowser->append(tr("IB(A):")+QString::number(I2FaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(I2FaultPhase,'g',6));
    ui->textBrowser->append(tr("IC(A):")+QString::number(I3FaultValue,'g',6)+tr("  ")+tr("相位(Deg):")+QString::number(I3FaultPhase,'g',6));

    ui->textBrowser->moveCursor(QTextCursor::Start);

}

void Widget::on_pushButton_5_clicked()
{
    if(SoftStartEndflag==true)
            return;
        if(starttest==1)
        {
            ui->pushButton_5->setText(tr("测试"));
            ui->pushButton_5->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
            ui->pushButton->setEnabled(true);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_4->setEnabled(true);
        }
        else
        {
            ui->pushButton_5->setText(tr("停止"));
            ui->pushButton_5->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
            ui->pushButton->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
        }

        SoftStartEndflag=true;
}

void Widget::GetTestState(){
    if(starttest==1)
    {
        ui->pushButton_5->setText(tr("停止"));
        ui->pushButton_5->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
        ui->pushButton->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
    }
    else
    {
        ui->pushButton_5->setText(tr("测试"));
        ui->pushButton_5->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
        ui->pushButton->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
    }

}

