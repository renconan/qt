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
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision,VzPhaseRevision;
float IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;


int   FreqTriple=2;
int   input_par=0,HelpFlag=0;
long  auto_i=0,IRQcnt=0,auto_count=0,tice=0;
int   testcount=0,time_relay_testcount=0,auto_end=0,starttest1;
int   starttest=0;
int   seq_count,V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC,OUTFT;





float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
float TripTime1,TripTime2,TripTime3,TripTime4,TripTime5;
float ChangeValue,timeout;
int save_f;
int sys[4];
float LogicResolution,SystemFrec,SystemVolt;//2009-6-20
char  V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
float VzOutput3Uo=0,VzOutputAc=0,VzOutputDc=0;


float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;


int   TestItem1,TestItem2,TestItem3,TestItem4,TestItem5;




float faultduration=0;
float prefaulttime=3;
float postfaulttime=3;
float Preparetime=0;
int   dcouttype=1;
float timeTime; //2016-05-20

//鍔ㄤ綔鍊
float StartFrepuency=50,EndFrepuency=45,Freqstep=0.01,FreqStepTime=0.01;
//鍔ㄤ綔鏃堕棿
int PickupType=2;
float TriptimeEndfrec=45; //瑙﹀彂鏃堕棿鐨勫仠姝㈤鐜
float TriptimeHoldtime=5;
//dfdt闂攣鍊
float DfdtEndfrec=45;
float DfdtStart=5;
float DfdtEnd=1;
float DfdtStep=1;
//dfdt 瑙﹀彂鏃堕棿
int DfdtTriptimeType=2; //1 娴嬭瘯缁撴灉 2 鐢ㄦ埛杈撳叆
float DfdtOFTriptime=1;
float DfdtTriptimefrec=45; //dfdt瑙﹀彂鏃堕棿鐨勫仠姝㈤鐜
float Dfdtholdtime=5;
float DfdtTimefreq=48; //dfdt  鐨勮鏃堕鐜
//鐢靛帇闂攣鍊
float Dfdt_VoltageBlock=1;
float VoltageBlockEndFrec=45; //鐢靛帇闂攣鍊肩殑鍋滄棰戠巼
float VoltageBlockStart=65;
float VoltageBlockEnd=75;
float VoltageBlockStep=1;
float VoltageBlockHoldTime=0;
int   VPNVPP=2;


float TimeFrequencer;
float TripFrepuency; //瑙﹀彂棰戠巼
float currentdfdt;
float dfdttripvalue; //dfdt鐨勮Е鍙戝€
float currentvoltage=0; //褰撳墠鐨勭數鍘嬮棴閿佸€
bool Threadrunstate=true;

float LoadCurrent=0.5;

bool SoftStartEndflag=false;


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
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

    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    setFixedSize(800,480);
    //init_par();
     showFullScreen();
     ui->label_23->hide();
     ui->label_29->hide();
     ui->label_30->hide();
     ui->label_31->hide();
     ui->label_32->hide();
     ui->label_33->hide();
     ui->label_34->hide();
     ui->label_35->hide();

     QPixmap img(":/picture/hei.png");
     ui->label_23->setPixmap(img);
     ui->label_29->setPixmap(img);
     ui->label_30->setPixmap(img);
     ui->label_31->setPixmap(img);

     //ui->label_26->setText("<font color=blue>"+QString("低周减载(2.2.1)")+"</font>");

      ui->tabWidget->setCurrentIndex(0);
      TestItem1=1;
      TestItem2=0;
      TestItem3=0;
      TestItem4=0;
      TestItem5=0;
      ui->textBrowser->setFocusPolicy(Qt::NoFocus);

    ui->lineEdit_2->show();//2016-5-23
    ui->label_2->show();

    ui->lineEdit_14->setText(QString::number(DfdtTriptimefrec,'g',6));
    ui->lineEdit_13->setText(QString::number(DfdtOFTriptime));



    ui->lineEdit_22->setText(QString::number(LoadCurrent,'g',5));

    ui->lineEdit_15->setText(QString::number(postfaulttime));





    ui->lineEdit->setText(QString::number(prefaulttime,'g',6));
    ui->lineEdit_2->setText(QString::number(faultduration,'g',6));
    ui->lineEdit_3->setText(QString::number(StartFrepuency,'g',6));
    ui->lineEdit_4->setText(QString::number(EndFrepuency,'g',6));
    ui->lineEdit_5->setText(QString::number(Freqstep,'g',6));
    ui->lineEdit_6->setText(QString::number(FreqStepTime,'g',6));




    ui->lineEdit_7->setText(QString::number(TriptimeEndfrec,'g',6));
    ui->lineEdit_8->setText(QString::number(TriptimeHoldtime,'g',6));

    ui->lineEdit_9->setText(QString::number(DfdtEndfrec,'g',6));
    ui->lineEdit_10->setText(QString::number(DfdtStart,'g',6));
    ui->lineEdit_11->setText(QString::number(DfdtEnd,'g',6));
    ui->lineEdit_12->setText(QString::number(DfdtStep,'g',6));


    ui->lineEdit_16->setText(QString::number(Dfdt_VoltageBlock,'g',6));
    ui->lineEdit_17->setText(QString::number(VoltageBlockEndFrec,'g',6));
    ui->lineEdit_18->setText(QString::number(VoltageBlockStart,'g',6));
    ui->lineEdit_19->setText(QString::number(VoltageBlockEnd,'g',6));
    ui->lineEdit_20->setText(QString::number(VoltageBlockStep,'g',6));
    //ui->lineEdit_21->setText(QString::number(VoltageBlockHoldTime,'g',6));
    ui->lineEdit_23->setText(QString::number(Dfdtholdtime,'g',6));
    ui->lineEdit_24->setText(QString::number(DfdtTimefreq,'g',6));





    ui->comboBox->setCurrentIndex(VPNVPP-1);

     ui->label_11->clear();
     ui->textBrowser->clear();
     currenteditpos=0;

      //ui->pushButton_4->setFocusPolicy(Qt::NoFocus);







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
             ui->lineEdit_18->installEventFilter(this);
             ui->lineEdit_19->installEventFilter(this);
             ui->lineEdit_20->installEventFilter(this);
             //ui->lineEdit_21->installEventFilter(this);
             ui->lineEdit_22->installEventFilter(this);
             ui->lineEdit_23->installEventFilter(this);
             ui->lineEdit_24->installEventFilter(this);




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
             ui->lineEdit_18->setContextMenuPolicy(Qt::NoContextMenu);
             ui->lineEdit_19->setContextMenuPolicy(Qt::NoContextMenu);
             ui->lineEdit_20->setContextMenuPolicy(Qt::NoContextMenu);
             //ui->lineEdit_21->setContextMenuPolicy(Qt::NoContextMenu);
             ui->lineEdit_22->setContextMenuPolicy(Qt::NoContextMenu);
             ui->lineEdit_23->setContextMenuPolicy(Qt::NoContextMenu);
             ui->lineEdit_24->setContextMenuPolicy(Qt::NoContextMenu);



             ui->textBrowser->setContextMenuPolicy(Qt::NoContextMenu);





             //ui->pushButton->setFocusPolicy(Qt::NoFocus);
             //ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
             //ui->pushButton_3->setFocusPolicy(Qt::NoFocus);



    uartthread =new thread1;
    connect(uartthread,SIGNAL(SIG_binarystate(int)),this,SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
    connect(uartthread,SIGNAL(SIG_testresult(int)),SLOT(showtestresult(int)));
    connect(uartthread,SIGNAL(SIG_DisplayChangeValue(bool,float)),SLOT(DisplayChangeValue(bool,float)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),SLOT(showdspstate(int)));
          connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
          connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));
    uartthread->start();
}

void Widget::updateUI()
{
    ui->label_26->setText("<font color=blue>"+QString(tr("低周减载(2.2.1)"))+"</font>");
    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_2->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_4->setText(tr("测试"));

        ui->label_3->setText(tr("初值(Hz):"));
        ui->label_4->setText(tr("终值(Hz)："));
        ui->label_5->setText(tr("步长(Hz)："));
        ui->label_6->setText(tr("步长时间(s)："));
        ui->label_8->setText(tr("测试频率(Hz):"));
        ui->label_7->setText(tr("最长时间(s):"));
        ui->label_9->setText(tr("终止频率(Hz):"));
        ui->label_10->setText(tr("滑差初值(Hz/s):"));
        ui->label_12->setText(tr("滑差终值(Hz/s):"));
        ui->label_13->setText(tr("滑差步长(Hz/s):"));
        ui->label_14->setText(tr("测试df/dt值(Hz/s):"));
        ui->label_15->setText(tr("终止频率(Hz):"));
        ui->label_16->setText(tr("终止频率持续时间(s):"));
        ui->label_25->setText(tr("计时频率(Hz):"));
        ui->label_18->setText(tr("滑差值(Hz/s):"));
        ui->label_19->setText(tr("终止频率(Hz):"));
        ui->label_20->setText(tr("起始电压(V):"));
        ui->label_21->setText(tr("终止电压(V):"));
        ui->label_22->setText(tr("电压步长(V):"));
        ui->label_24->setText(tr("相电压/线电压"));
        ui->label->setText(tr("故障前时间(s):"));
        ui->label_27->setText(tr("故障后时间(s):"));
        ui->label_28->setText(tr("负荷电流(A):"));
        ui->label_2->setText(tr("动作保持时间(s):"));

        ui->groupBox_5->setTitle(tr("测试结果"));
        ui->comboBox->addItems(QStringList()<<tr("相电压")<<tr("线电压"));

        ui->tabWidget->setTabText(0,tr("动作值"));
        ui->tabWidget->setTabText(1,tr("固定频率测试"));
        ui->tabWidget->setTabText(2,tr("滑差闭锁值"));
        ui->tabWidget->setTabText(3,tr("动作时间"));
        ui->tabWidget->setTabText(4,tr("电压闭锁值"));
}

void Widget::showdspstate(int state)
{
    switch(state)
      {
      case 0: ui->label_17->setText(tr("运行正常")); break;
      case 1: ui->label_17->setText(tr("IA 过载")); break;
      case 2: ui->label_17->setText(tr("IABC 温度保护")); break;
      case 3: ui->label_17->setText(tr("IC 间歇")); break;
      case 4: ui->label_17->setText(tr("IB 过载")); break;
      case 5: ui->label_17->setText(tr("IC 闭锁")); break;
      case 6: ui->label_17->setText(tr("IA 间歇")); break;
      case 7: ui->label_17->setText(tr("IC 过载")); break;
      case 8: ui->label_17->setText(tr("IABC 电源保护")); break;
      case 9: ui->label_17->setText(tr("IB 间歇")); break;
      case 10: ui->label_17->setText(tr("VA 过载")); break;

      case 12: ui->label_17->setText(tr("VC 间歇")); break;
      case 13: ui->label_17->setText(tr("VB 过载")); break;
      case 14: ui->label_17->setText(tr("VABC 温度保护")); break;
      case 15: ui->label_17->setText(tr("VA 间歇")); break;
      case 16: ui->label_17->setText(tr("VC 过载")); break;
      case 17: ui->label_17->setText(tr("VABC 电源保护")); break;
      case 18: ui->label_17->setText(tr("Vz 过载")); break;
      case 19: ui->label_17->setText(tr("直流过载")); break;
      case 20: ui->label_17->setText(tr("IABC 电源保护")); break;
      case 21: ui->label_17->setText(tr("VC 间歇")); break;
      }
    QPixmap img(":/picture/hei.png");
    QPixmap pic(":/picture/hong.png");
    switch(state)
    {
    case 100:
        ui->label_23->setPixmap(img);
        ui->label_29->setPixmap(img);
        ui->label_30->setPixmap(img);
        ui->label_31->setPixmap(img);
        break;
    case 101: ui->label_23->setPixmap(pic); break;
    case 102: ui->label_29->setPixmap(pic); break;
    case 103: ui->label_30->setPixmap(pic); break;
    case 104: ui->label_31->setPixmap(pic); break;
    case 111: ui->label_23->setPixmap(img); break;
    case 112: ui->label_29->setPixmap(img); break;
    case 113: ui->label_30->setPixmap(img); break;
    case 114: ui->label_31->setPixmap(img); break;
    }
}

void Widget::DisplayChangeValue(bool flag, float value)
{
    if(flag)
    ui->label_11->setText(QString::number(value,'g',6));
    else
    ui->label_11->clear();
}

void Widget::showtestresult(int num)
{
       ui->textBrowser->clear();
       if(current_language==1)
       {
          switch(num)
          {
            case 0:
                ui->textBrowser->clear();
                break;

            case 1:

              if(TripTime1==-1)
                ui->textBrowser->append(QString("频率动作值(Hz):未动作"));
              else
                ui->textBrowser->append(QString("频率动作值(Hz):")+QString::number(TripFrepuency,'g',6));
              break;

            case 2:
            if(TripTime2==-1)
              ui->textBrowser->append(QString("固定频率动作时间(s):未动作"));
            else
              ui->textBrowser->append(tr("固定频率动作时间(s):")+QString::number(TripTime2,'g',6));
              break;

            case 3:
           if(TripTime3==-1)
             ui->textBrowser->append(QString("滑差闭锁值(Hz/s):未动作"));
           else
            {
                char temp[512];
                if(DfdtStart>DfdtEnd)
              {
                  sprintf(temp,"%.3f 滑差闭锁值(Hz/s):动作",currentdfdt+DfdtStep);



              }
                else
                sprintf(temp,"%.3f 滑差闭锁值(Hz/s):动作",currentdfdt-DfdtStep);
                ui->textBrowser->append(QString(temp));

            }
             break;
          case 4:
              if(TripTime4==-1)
                ui->textBrowser->append(QString("动作时间(s):未动作"));
              else
                ui->textBrowser->append(QString("动作时间(s):")+QString::number(TripTime4,'g',6));
                  break;
          case 5://显示电压闭锁值
               if(TripTime5==-1)
               ui->textBrowser->append("电压闭锁值(V):未动作");
               else
              {
                  if(VoltageBlockStart>VoltageBlockEnd)
                  ui->textBrowser->append(tr("电压闭锁值(V):")+QString::number(currentvoltage+VoltageBlockStep+VoltageBlockStep,'g',6));
                  else
                  ui->textBrowser->append(tr("电压闭锁值(V):")+QString::number(currentvoltage-VoltageBlockStep-VoltageBlockStep,'g',6));

              }
                  break;

}
       }
       else if(current_language==2)
       {
           switch(num)
              {
                case 0:
                    ui->textBrowser->clear();
                    break;

                case 1:

                  if(TripTime1==-1)
                    ui->textBrowser->append(QString("pick up(Hz):no trip"));
                  else
                    ui->textBrowser->append(QString("pick up(Hz):")+QString::number(TripFrepuency,'g',6));
                  break;

                case 2:
                if(TripTime2==-1)
                  ui->textBrowser->append(QString("Fixed frequency(s):no trip"));
                else
                  ui->textBrowser->append(tr("Fixed frequency(s):")+QString::number(TripTime2,'g',6));
                  break;

                case 3:
               if(TripTime3==-1)
                 ui->textBrowser->append(QString("dfdt(Hz/s):no trip"));
               else
                {
                    char temp[512];
                    if(DfdtStart>DfdtEnd)
                  {
                      sprintf(temp,"%.3f dfdt(Hz/s):trip",currentdfdt+DfdtStep);



                  }
                    else
                    sprintf(temp,"%.3f dfdt(Hz/s):trip",currentdfdt-DfdtStep);
                    ui->textBrowser->append(QString(temp));

                }
                 break;
              case 4:
                  if(TripTime4==-1)
                    ui->textBrowser->append(QString("Trip time(s):no trip"));
                  else
                    ui->textBrowser->append(QString("Trip time(s):")+QString::number(TripTime4,'g',6));
                      break;
              case 5://显示电压闭锁值
                   if(TripTime5==-1)
                   ui->textBrowser->append("voltage blocking(V):no trip");
                   else
                  {
                      if(VoltageBlockStart>VoltageBlockEnd)
                      ui->textBrowser->append(tr("voltage blocking(V):")+QString::number(currentvoltage+VoltageBlockStep+VoltageBlockStep,'g',6));
                      else
                      ui->textBrowser->append(tr("voltage blocking(V):")+QString::number(currentvoltage-VoltageBlockStep-VoltageBlockStep,'g',6));

                  }
                      break;

              }

       }
}


void Widget::init_par()
{

    FILE *fp;
    SystemVolt=57.735;
    if((fp=fopen("sysset.txt","a+"))==NULL)
      return;
    fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
           &SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&Preparetime,&current_language);
    SystemFrec=SystemFrec*FreqTriple;
    fclose(fp);


    TestItem1=1;
    TestItem2=0;
    TestItem3=0;
    TestItem4=0;
    TestItem5=0;


    V1Value=SystemVolt;
    V2Value=SystemVolt;
    V3Value=SystemVolt;
    if(SystemFrec==0)
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
    MAX_V_VALUEDATA_DC=176;
    MIN_V_VALUEDATA_DC=-176;
    MAX_V_VALUEDATA_AC=125;
    MIN_V_VALUEDATA_AC=0;
    MAX_I_VALUEDATA_DC=70;
    MIN_I_VALUEDATA_DC=-70;
    MAX_I_VALUEDATA_AC=50;
    MIN_I_VALUEDATA_AC=0;
}

void Widget::vi_parameter_enter(int position)
{
    switch(position)
    {
    case 1:
                            if(prefaulttime>999.99)
                               prefaulttime=999.99;
                             else if(prefaulttime<0)
                               prefaulttime=0;//鍙傛暟闄愬埗
                             break;
    case 2:
                              if(faultduration>999.99)
                                faultduration=999.99;
                              else if(faultduration<0)
                                faultduration=0;//鍙傛暟闄愬埗
                              break;
    case 3:
                              if(StartFrepuency>999.99)
                                StartFrepuency=999.99;
                              else if(StartFrepuency<0)
                                StartFrepuency=0;//鍙傛暟闄愬埗
                                break;
    case 4:
                              if(EndFrepuency>999.99)
                                EndFrepuency=999.99;
                              else if(EndFrepuency<0)
                                EndFrepuency=0;//鍙傛暟闄愬埗
                                break;
   case 5:
                            if(Freqstep>999.99)
                              Freqstep=999.99;
                            if(Freqstep<0)
                              Freqstep=0;
                              break;
   case 6:
                            if(FreqStepTime>999.99)
                              FreqStepTime=999.99;
                            else if(FreqStepTime<0)
                              FreqStepTime=0;//鍙傛暟闄愬埗
                              break;

  //瑙﹀彂鏃堕棿
   case 7:
                             if(TriptimeEndfrec>999.99)
                               TriptimeEndfrec=999.99;
                             else if(TriptimeEndfrec<0)
                               TriptimeEndfrec=0;//鍙傛暟闄愬埗
                               break;
   case 8:
                              if(TriptimeHoldtime>999.99)
                                TriptimeHoldtime=999.99;
                              else if(TriptimeHoldtime<0)
                                TriptimeHoldtime=0;//鍙傛暟闄愬埗
                                break;

   //Dfdt闂攣鍊
   case 9:
                               if(DfdtEndfrec>999.99)
                                 DfdtEndfrec=999.99;
                               else if(DfdtEndfrec<0)
                                  DfdtEndfrec=0;
                                 break;
   case 10:
                                if(DfdtStart>999.99)
                                  DfdtStart=999.99;
                                else if(DfdtStart<0)
                                  DfdtStart=0;//鍙傛暟闄愬埗
                                if(DfdtStart<DfdtEnd)
                                 DfdtStart=DfdtEnd+DfdtStep;

                                  break;
   case 11:
                                 if(DfdtEnd>999.99)
                                   DfdtEnd=999.99;
                                 else if(DfdtEnd<0)
                                   DfdtEnd=0;//鍙傛暟闄愬埗
                                  if(DfdtStart<DfdtEnd)
                                      DfdtEnd=DfdtStart-DfdtStep;
                                   break;
    case 12:
                                  if(DfdtStep>999.99)
                                    DfdtStep=999.99;
                                  else if(DfdtStep<0)
                                    DfdtStep=0;//鍙傛暟闄愬埗
                                    break;
    case 13:
                                  if(DfdtOFTriptime>999.99)
                                    DfdtOFTriptime=999.99;
                                  else if(DfdtOFTriptime<0)
                                    DfdtOFTriptime=0;//鍙傛暟闄愬埗
                                    break;
    case 14:
                                  if(DfdtTriptimefrec>999.99)
                                    DfdtTriptimefrec=999.99;
                                  else if(DfdtTriptimefrec<0)
                                    DfdtTriptimefrec=0;//鍙傛暟闄愬埗
                                    break;
    case 15:
                                  if(postfaulttime>999.99)
                                    postfaulttime=999.99;
                                  else if(postfaulttime<0)
                                    postfaulttime=0;//鍙傛暟闄愬埗
                                    break;
    case 16:
                                  if(Dfdt_VoltageBlock>999.99)
                                    Dfdt_VoltageBlock=999.99;
                                  else if(Dfdt_VoltageBlock<0)
                                    Dfdt_VoltageBlock=0;//鍙傛暟闄愬埗
                                    break;
    case 17:
                                  if(VoltageBlockEndFrec>999.99)
                                    VoltageBlockStart=999.99;
                                  else if(VoltageBlockStart<0)
                                    VoltageBlockStart=0;//鍙傛暟闄愬埗
                                    break;
    case 18:
                                  if(VoltageBlockStart>MAX_V_VALUEDATA_AC)
                                    VoltageBlockStart=MAX_V_VALUEDATA_AC;
                                  else if(VoltageBlockStart<0)
                                    VoltageBlockStart=0;//鍙傛暟闄愬埗
                                  if(VoltageBlockStart>VoltageBlockEnd)
                                      VoltageBlockStart=VoltageBlockEnd-VoltageBlockStep;



                                    break;
    case 19:
                                  if(VoltageBlockEnd>MAX_V_VALUEDATA_AC)
                                    VoltageBlockEnd=MAX_V_VALUEDATA_AC;
                                  else if(VoltageBlockEnd<0)
                                    VoltageBlockEnd=0;//鍙傛暟闄愬埗
                                  if(VoltageBlockStart>VoltageBlockEnd)
                                      VoltageBlockEnd=VoltageBlockStart+VoltageBlockStep;


                                    break;
    case 20:
                                  if(VoltageBlockStep>MAX_V_VALUEDATA_AC)
                                    VoltageBlockStep=MAX_V_VALUEDATA_AC;
                                  else if(VoltageBlockStep<0)
                                    VoltageBlockStep=0;//鍙傛暟闄愬埗
                                    break;
    case 21:
                                  if(VoltageBlockHoldTime>999.99)
                                    VoltageBlockHoldTime=999.99;
                                  else if(VoltageBlockHoldTime<0)
                                    VoltageBlockHoldTime=0;//鍙傛暟闄愬埗
                                    break;
    case 22:
                                  if(LoadCurrent>MAX_I_VALUEDATA_AC)
                                    LoadCurrent=MAX_I_VALUEDATA_AC;
                                  else if(LoadCurrent<MIN_I_VALUEDATA_AC)
                                    LoadCurrent=MIN_I_VALUEDATA_AC;//鍙傛暟闄愬埗
                                    break;


   case 23:

                                if(Dfdtholdtime>999.99)
                                  Dfdtholdtime=999.99;
                                else if(Dfdtholdtime<0)
                                  Dfdtholdtime=0;//鍙傛暟闄愬埗
                                  break;
  case 24:
                                if(DfdtTimefreq>999.99)
                                  DfdtTimefreq=999.99;
                                else if(DfdtTimefreq<0)
                                  DfdtTimefreq=0;//鍙傛暟闄愬埗
                                  break;

}
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_2_clicked() //保存
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
                strcpy(strbuf,"低周减载:");
                fprintf(fp,"%s\r\n",strbuf);


         if(TestItem1==1)
           {
             strcpy(strbuf,"频率动作值(Hz):");
             fprintf(fp,"%s",strbuf);
             if(TripTime1==-1)
               strcpy(strbuf,"频率动作值(Hz):未动作");
             else
               gcvt(TripFrepuency,7,strbuf);
             fprintf(fp,"%s\r\n",strbuf);
           }

         if(TestItem2==1)
           {
             strcpy(strbuf,"固定频率动作时间(s):");
             fprintf(fp,"%s",strbuf);
             if(TripTime2==-1)
               strcpy(strbuf,"固定频率动作时间(s):未动作");
             else
               gcvt(TripTime2,7,strbuf);
             fprintf(fp,"%s\r\n",strbuf);
           }
         if(TestItem3==1)
           {

             if(TripTime3==-1)
               sprintf(strbuf,"滑差闭锁值(Hz/s):未动作");
             else
              {

               if(DfdtStart>DfdtEnd)
                   currentdfdt=currentdfdt+DfdtStep;
               else
                   currentdfdt=currentdfdt-DfdtStep;
               sprintf(strbuf,"滑差闭锁值(Hz/s):%3.4f",currentdfdt);
              }
             fprintf(fp,"%s\r\n",strbuf);

           }
         if(TestItem4==1)
           {
             if(TripTime4==-1)
                sprintf(strbuf,"动作时间(s):未动作");
             else
                sprintf(strbuf,"动作时间(s):%4.4f",TripTime4);
             fprintf(fp,"%s\r\n",strbuf);
           }

         if(TestItem5==1)
           {
             strcpy(strbuf,"电压闭锁值(V):");
             fprintf(fp,"%s",strbuf);
             if(TripTime5==-1)
               strcpy(strbuf,"电压闭锁值(V):未动作");
            else
             {
                 if(VoltageBlockStart>VoltageBlockEnd)
                  gcvt(currentvoltage+VoltageBlockStep+VoltageBlockStep,7,strbuf);
                 else gcvt(currentvoltage-VoltageBlockStep-VoltageBlockStep,7,strbuf);

             }
            fprintf(fp,"%s\r\n",strbuf);


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
        strcpy(strbuf,"Df/dt test result\n");
        fprintf(fp,"%s\r\n",strbuf);


 if(TestItem1==1)
   {
     strcpy(strbuf,"pick up(Hz):");
     fprintf(fp,"%s",strbuf);
     if(TripTime1==-1)
       strcpy(strbuf,"pick up(Hz):No Trip");
     else
       gcvt(TripFrepuency,7,strbuf);
     fprintf(fp,"%s\r\n",strbuf);
   }

 if(TestItem2==1)
   {
     strcpy(strbuf,"Fixed frequency Trip time(s):");
     fprintf(fp,"%s",strbuf);
     if(TripTime2==-1)
       strcpy(strbuf,"Fixed frequency(s):No Trip");
     else
       gcvt(TripTime2,7,strbuf);
     fprintf(fp,"%s\r\n",strbuf);
   }
 if(TestItem3==1)
   {

     if(TripTime3==-1)
       sprintf(strbuf,"dfdt(Hz/s):No Trip");
     else
      {

       if(DfdtStart>DfdtEnd)
           currentdfdt=currentdfdt+DfdtStep;
       else
           currentdfdt=currentdfdt-DfdtStep;
       sprintf(strbuf,"%3.4f dfdt(Hz/s):Trip",currentdfdt);
      }
     fprintf(fp,"%s\r\n",strbuf);

   }
 if(TestItem4==1)
   {
     if(TripTime4==-1)
        sprintf(strbuf,"dfdt trip time(s):No Trip");
     else
        sprintf(strbuf,"dfdt trip time(s):%4.4f",TripTime4);
     fprintf(fp,"%s\r\n",strbuf);
   }

 if(TestItem5==1)
   {
     strcpy(strbuf,"Voltage blocking(V):");
     fprintf(fp,"%s",strbuf);
     if(TripTime5==-1)
       strcpy(strbuf,"Voltage blocking(V):No Trip");
    else
     {
         if(VoltageBlockStart>VoltageBlockEnd)
          gcvt(currentvoltage+VoltageBlockStep+VoltageBlockStep,7,strbuf);
         else gcvt(currentvoltage-VoltageBlockStep-VoltageBlockStep,7,strbuf);

     }
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

void Widget::CloseWidget()
{

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
        prefaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(prefaulttime,'g',6));
        break;
    case 2:
        faultduration=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(faultduration,'g',6));
        break;
    case 3:
        StartFrepuency=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StartFrepuency,'g',6));
        break;
    case 4:
        EndFrepuency=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(EndFrepuency,'g',6));
        break;
    case 5:
        Freqstep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Freqstep,'g',6));
        break;
    case 6:
        FreqStepTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FreqStepTime,'g',6));
        break;

   //瑙﹀彂鏃堕棿
    case 7:
        TriptimeEndfrec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(TriptimeEndfrec,'g',6));
        break;
    case 8:
        TriptimeHoldtime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(TriptimeHoldtime,'g',6));
        break;

   //dfdt 闂攣鍊
    case 9:
        DfdtEndfrec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DfdtEndfrec,'g',6));
        break;
    case 10:
        DfdtStart=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DfdtStart,'g',6));
        break;
    case 11:
        DfdtEnd=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DfdtEnd,'g',6));
        break;
    case 12:
        DfdtStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DfdtStep,'g',6));
        break;


    //dfdt 瑙﹀彂鏃堕棿
    case 13:
        DfdtOFTriptime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DfdtOFTriptime,'g',6));

        break;
    case 14:
        DfdtTriptimefrec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DfdtTriptimefrec,'g',6));

        break;
    case 15:
        postfaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(postfaulttime,'g',6));
        break;

    case 16:  //鐢靛帇闂攣鍊
        Dfdt_VoltageBlock=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Dfdt_VoltageBlock,'g',6));
        break ;
    case 17:  //鐢靛帇闂攣鍊
        VoltageBlockEndFrec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VoltageBlockEndFrec,'g',6));
        break ;
    case 18:  //鐢靛帇闂攣鍊
        VoltageBlockStart=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VoltageBlockStart,'g',6));
        break ;
    case 19:  //鐢靛帇闂攣鍊
        VoltageBlockEnd=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VoltageBlockEnd,'g',6));
        break ;
    case 20:  //鐢靛帇闂攣鍊
        VoltageBlockStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VoltageBlockStep,'g',6));
        break ;
    case 21:  //鐢靛帇闂攣鍊
        VoltageBlockHoldTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VoltageBlockHoldTime,'g',6));
        break ;
    case 22:  //鐢靛帇闂攣鍊
        LoadCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(LoadCurrent,'g',6));
        break ;
    case 23:
        Dfdtholdtime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Dfdtholdtime,'g',6));
        break ;
    case 24:
        DfdtTimefreq=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DfdtTimefreq,'g',6));
        break ;

    }

}

void Widget::clearkeypadflag()
{
 temprorarydata->keypadopenflag=false;
}



void Widget::on_comboBox_currentIndexChanged(int index)
{
   VPNVPP=index+1;
}







void Widget::on_pushButton_clicked()
{
    if(starttest==1)
        return;

Helpdialog *helpdialog=new Helpdialog;
temprorarydata->helponflag=true;
connect(helpdialog,SIGNAL(HelpWindowClose()),SLOT(HelpDialogClose()));
if(current_language==2)
helpdialog->SetHelpfilename("dizhou.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("dizhou1.txt");
//helpdialog->SetHelpfilename("dizhou.txt");
helpdialog->show();
}
void Widget::HelpDialogClose()
{
temprorarydata->helponflag=false;

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
        /*else if(obj==ui->lineEdit_21)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_21;
              currenteditpos=21;
         }*/
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
               prefaulttime= text.toFloat();
               currenteditpos=1;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit->setText(QString::number(prefaulttime,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               faultduration= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(faultduration,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                StartFrepuency= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(StartFrepuency,'g',6));
         }
        else if(obj==ui->lineEdit_4)
         {
                QString text=ui->lineEdit_4->text();
                EndFrepuency= text.toFloat();
                currenteditpos=4;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_4->setText(QString::number(EndFrepuency,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                Freqstep= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(Freqstep,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                FreqStepTime= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(FreqStepTime,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                TriptimeEndfrec= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(TriptimeEndfrec,'g',6));
         }
        else if(obj==ui->lineEdit_8)
         {
                QString text=ui->lineEdit_8->text();
                TriptimeHoldtime= text.toFloat();
                currenteditpos=8;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_8->setText(QString::number(TriptimeHoldtime,'g',6));
         }
        else if(obj==ui->lineEdit_9)
         {
                QString text=ui->lineEdit_9->text();
                DfdtEndfrec= text.toFloat();
                currenteditpos=9;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_9->setText(QString::number(DfdtEndfrec,'g',6));
         }
        else if(obj==ui->lineEdit_10)
         {
                QString text=ui->lineEdit_10->text();
                DfdtStart= text.toFloat();
                currenteditpos=10;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_10->setText(QString::number(DfdtStart,'g',6));
         }
        else if(obj==ui->lineEdit_11)
         {
                QString text=ui->lineEdit_11->text();
                DfdtEnd= text.toFloat();
                currenteditpos=11;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_11->setText(QString::number(DfdtEnd,'g',6));
         }
        else if(obj==ui->lineEdit_12)
         {
                QString text=ui->lineEdit_12->text();
                DfdtStep= text.toFloat();
                currenteditpos=12;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_12->setText(QString::number(DfdtStep,'g',6));
         }
        else if(obj==ui->lineEdit_13)
         {
                QString text=ui->lineEdit_13->text();
                DfdtOFTriptime= text.toFloat();
                currenteditpos=13;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_13->setText(QString::number(DfdtOFTriptime,'g',6));
         }
        else if(obj==ui->lineEdit_14)
         {
                QString text=ui->lineEdit_14->text();
                DfdtTriptimefrec= text.toFloat();
                currenteditpos=14;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_14->setText(QString::number(DfdtTriptimefrec,'g',6));
         }
        else if(obj==ui->lineEdit_15)
         {
                QString text=ui->lineEdit_15->text();
                postfaulttime= text.toFloat();
                currenteditpos=15;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_15->setText(QString::number(postfaulttime,'g',6));
         }
        else if(obj==ui->lineEdit_16)
         {
                QString text=ui->lineEdit_16->text();
                Dfdt_VoltageBlock= text.toFloat();
                currenteditpos=16;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_16->setText(QString::number(Dfdt_VoltageBlock,'g',6));
         }
        else if(obj==ui->lineEdit_17)
         {
                QString text=ui->lineEdit_17->text();
                VoltageBlockEndFrec= text.toFloat();
                currenteditpos=17;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_17->setText(QString::number(VoltageBlockEndFrec,'g',6));
         }
        else if(obj==ui->lineEdit_18)
         {
                QString text=ui->lineEdit_18->text();
                VoltageBlockStart= text.toFloat();
                currenteditpos=18;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_18->setText(QString::number(VoltageBlockStart,'g',6));
         }
        else if(obj==ui->lineEdit_19)
         {
                QString text=ui->lineEdit_19->text();
                VoltageBlockEnd= text.toFloat();
                currenteditpos=19;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_19->setText(QString::number(VoltageBlockEnd,'g',6));
         }
        else if(obj==ui->lineEdit_20)
         {
                QString text=ui->lineEdit_20->text();
                VoltageBlockStep= text.toFloat();
                currenteditpos=20;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_20->setText(QString::number(VoltageBlockStep,'g',6));
         }
//        else if(obj==ui->lineEdit_21)
//         {
//                QString text=ui->lineEdit_21->text();
//                VoltageBlockHoldTime= text.toFloat();
//         }
        else if(obj==ui->lineEdit_22)
         {
                QString text=ui->lineEdit_22->text();
                LoadCurrent= text.toFloat();
                currenteditpos=22;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_22->setText(QString::number(LoadCurrent,'g',6));
         }
        else if(obj==ui->lineEdit_23)
         {
                QString text=ui->lineEdit_23->text();
                Dfdtholdtime= text.toFloat();
                currenteditpos=23;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_23->setText(QString::number(Dfdtholdtime,'g',6));
         }
        else if(obj==ui->lineEdit_24)
         {
                QString text=ui->lineEdit_24->text();
                DfdtTimefreq= text.toFloat();
                currenteditpos=24;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_24->setText(QString::number(DfdtTimefreq,'g',6));
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

 if(index==1)
 {
   ui->label_2->hide();
   ui->lineEdit_2->hide();
 }
 else
 {
     ui->label_2->show();
     ui->lineEdit_2->show();
 }

 switch(index)
 {
 case 0:
     TestItem1=1;
     TestItem2=0;
     TestItem3=0;
     TestItem4=0;
     TestItem5=0;


     break;

 case 1:
     TestItem1=0;
     TestItem2=1;
     TestItem3=0;
     TestItem4=0;
     TestItem5=0;

     break;

 case 2:
     TestItem1=0;
     TestItem2=0;
     TestItem3=1;
     TestItem4=0;
     TestItem5=0;

     break;


 case 3:
     TestItem1=0;
     TestItem2=0;
     TestItem3=0;
     TestItem4=1;
     TestItem5=0;
     break;

  case 4:
     TestItem1=0;
     TestItem2=0;
     TestItem3=0;
     TestItem4=0;
     TestItem5=1;
     break;

 }
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
