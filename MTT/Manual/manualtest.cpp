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
#include "Config.h"

//struct ST_KeyValue
//{
//    QString Key;
//    QString Value;
//    float GetValue()
//    {
//        //return atof(Value.c_str());
//        return Value.toFloat();
//    }
//};


//QTranslator *tor;
extern int current_language;  //1中文  2英文
extern TEMPRORARYDATA *temprorarydata;

extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
extern unsigned char result[70],receive_data[70];

extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision,VzPhaseRevision;
extern float IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
extern int VariatyVz2;
extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,VzRelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase;

extern float faultduration;
extern float TripValue,TripTime;

extern int save_f;
extern int sys[4];

float Preparetime;
float LogicResolution;

//char ParStr[20];
float A_SumStepnum=0;
bool A_Phasesymmetry=false;//任意测试独有

extern bool SoftStartEndflag;
extern bool ManulIsSendcmd;

Manualtest::Manualtest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manualtest)
{
    ui->setupUi(this);
#ifdef ARMLINUX
  this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
#endif

#ifdef ARMTEST
  printf("2");
#endif
    init_par();
    initDesc();

    //getQMap();
    /* 加载语言文件 默认中文 2016-9-7*/
//    tor = new QTranslator();
//    if (current_language == 1) {
//        tor->load(":/trans/zh.qm");
//    }
//    else if (current_language == 2) {
//        tor->load(":/trans/en.qm");
//    }
//    qApp->installTranslator(tor);
//    updateUI();

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    setFixedSize(640,480);
    setCursor(Qt::ArrowCursor);
    //ui->label_23->hide();
    //ui->label_24->hide();
    ui->label_25->hide();
    ui->label_26->hide();
    //ui->label_27->hide();
    //ui->label_28->hide();
    ui->label_29->hide();
    ui->label_30->hide();
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
    //init_par();
    initalldata();
    //无VZ显示
    ui->checkBox_8->hide();
    ui->checkBox_9->hide();
    ui->label_22->hide();
    ui->lineEdit_29->hide();
    ui->lineEdit_30->hide();
    ui->lineEdit_31->hide();

    ui->label_4->setEnabled(false);
    ui->lineEdit_28->setEnabled(false);
    display_mode(A_TestMode);

     VolPhaseChange=0;  //电压相位最后一次改变的位置
     CurPhaseChange=0;  //电流相位最后一次改变的位置


     if(m_Ch[0].checked==1)
      ui->checkBox->setChecked(true);
     else
     ui->checkBox->setChecked(false);
     if(m_Ch[1].checked==1)
       ui->checkBox_2->setChecked(true);
     else
       ui->checkBox_2->setChecked(false);
     if(m_Ch[2].checked==1)
     ui->checkBox_3->setChecked(true);
     else
     ui->checkBox_3->setChecked(false);



     if(m_Ch[4].checked==1)
        ui->checkBox_4->setChecked(true);
     else
        ui->checkBox_4->setChecked(false);
     if(m_Ch[5].checked==1)
      ui->checkBox_5->setChecked(true);
     else
      ui->checkBox_5->setChecked(false);
     if(m_Ch[6].checked==1)
       ui->checkBox_6->setChecked(true);
     else
       ui->checkBox_6->setChecked(false);

    ui->checkBox_7->setChecked(A_Phasesymmetry);

    display_variry(A_Variable);
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
void Manualtest::getQMap()
{
    QFile file("widgetdate.txt");

                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    return;
                }

                QTextStream in(&file);
                QString anytype = in.readLine().section("=", -1);//anytest

                while (1)
                {
                    QString line = in.readLine();
                    qDebug()<<"linetext"<<line;
                    if(line=="end")
                        break;
                    QString key = line.section("=",0,0);
                    float value = line.section("=", -1).toFloat();
                    Anydate.insert(key,value);
                }
                widgetdate.insert(0,Anydate);

                //ui->textBrowser->setText(QString::number(widgetdate.value(0).value("m_Ch[0].Amp"),'g',4));

                file.close();

}
void Manualtest::init_par()
{


    float SysFrec2=25;
    float mSysVolt =0;
    float mLogicResolution=0;
    float mPreFaultTime=0;
    float mPostFaultTime=0;
    float mPreparetime=0;
    FILE *fp;
    if((fp=fopen("sysset.txt","a+"))==NULL)  //009-6-19
      return;
    fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SysFrec2,\
           &mSysVolt,&mLogicResolution,&mPreFaultTime,&mPostFaultTime,&mPreparetime,&current_language);

    SysFrec=SysFrec2*2;
    SysVolt=mSysVolt;
    LogicResolution=mLogicResolution;
    A_PreFaultTime=mPreFaultTime;
    A_PostFaultTime=mPostFaultTime;
    Preparetime=mPreparetime;


    fclose(fp);
    //exit_par=0;
    m_Ch[0].Amp=SysVolt;//50;
    m_Ch[0].Phase=0;
    m_Ch[0].Freq=SysFrec;
    A_Variable=1;
    A_TestMode=1;
    m_Ch[0].checked=1;
    m_Ch[1].checked=0;
    m_Ch[2].checked=0;
    m_Ch[4].checked=1;
    m_Ch[5].checked=0;
    m_Ch[6].checked=0;

    m_Ch[1].Amp=SysVolt;//57.73;
    m_Ch[1].Phase=-120;
    m_Ch[1].Freq=SysFrec;

    m_Ch[2].Amp=SysVolt;
    m_Ch[2].Phase=120;
    m_Ch[2].Freq=SysFrec;

    m_Ch[3].Amp=SysVolt;
    m_Ch[3].Phase=0;
    m_Ch[3].Freq=SysFrec;

    m_Ch[3].checked=0;
    VariatyVz2=0;



       m_Ch[4].Amp=3;
       m_Ch[4].Phase=0;
       m_Ch[4].Freq=SysFrec;

       m_Ch[5].Amp=0;
       m_Ch[5].Phase=-120;
       m_Ch[5].Freq=SysFrec;

       m_Ch[6].Amp=0;
       m_Ch[6].Phase=120;
       m_Ch[6].Freq=SysFrec;


       A_CurStep=0.5;
       A_VolStep=1;
       A_PhaStep=1;
       A_FreStep=1;

       A_FaultTime=5;

       A_StepTime=0.5;
       A_StepNum=20;       
       if(SysFrec==0)
       {
       m_Ch[0].DC=1;
       m_Ch[1].DC=1;
       m_Ch[2].DC=1;
       m_Ch[3].DC=1;

       m_Ch[4].DC=1;
       m_Ch[5].DC=1;
       m_Ch[6].DC=1;
       }
       else
       {
           m_Ch[0].DC=0;
           m_Ch[1].DC=0;
           m_Ch[2].DC=0;
           m_Ch[3].DC=0;

           m_Ch[4].DC=0;
           m_Ch[5].DC=0;
           m_Ch[6].DC=0;
       }

#ifdef ARMTEST
  printf("6");
#endif
}


void Manualtest::initalldata()
{


  ui->lineEdit->setText(QString::number(m_Ch[0].Amp,'g',4));
  ui->lineEdit_2->setText(QString::number(m_Ch[0].Phase,'g',4));
  ui->lineEdit_3->setText(QString::number(m_Ch[0].Freq,'g',4));

  ui->lineEdit_4->setText(QString::number(m_Ch[1].Amp,'g',4));
  ui->lineEdit_5->setText(QString::number(m_Ch[1].Phase,'g',4));
  ui->lineEdit_6->setText(QString::number(m_Ch[1].Freq,'g',4));

  ui->lineEdit_7->setText(QString::number(m_Ch[2].Amp,'g',4));
  ui->lineEdit_8->setText(QString::number(m_Ch[2].Phase,'g',4));
  ui->lineEdit_9->setText(QString::number(m_Ch[2].Freq,'g',4));



  ui->lineEdit_10->setText(QString::number(m_Ch[4].Amp,'g',4));
  ui->lineEdit_11->setText(QString::number(m_Ch[4].Phase,'g',4));
  ui->lineEdit_12->setText(QString::number(m_Ch[4].Freq,'g',4));

  ui->lineEdit_13->setText(QString::number(m_Ch[5].Amp,'g',4));
  ui->lineEdit_14->setText(QString::number(m_Ch[5].Phase,'g',4));
  ui->lineEdit_15->setText(QString::number(m_Ch[5].Freq,'g',4));

  ui->lineEdit_16->setText(QString::number(m_Ch[6].Amp,'g',4));
  ui->lineEdit_17->setText(QString::number(m_Ch[6].Phase,'g',4));
  ui->lineEdit_18->setText(QString::number(m_Ch[6].Freq,'g',4));


  ui->lineEdit_29->setText(QString::number(m_Ch[3].Amp,'g',4));
  ui->lineEdit_30->setText(QString::number(m_Ch[3].Phase,'g',4));
  ui->lineEdit_31->setText(QString::number(m_Ch[3].Freq,'g',4));



  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
  ui->lineEdit_20->setText(QString::number(A_VolStep,'g',4));
  ui->lineEdit_21->setText(QString::number(A_PhaStep,'g',4));
  ui->lineEdit_22->setText(QString::number(A_FreStep,'g',4));
  ui->lineEdit_23->setText(QString::number(A_StepTime,'g',4));
  ui->lineEdit_24->setText(QString::number(A_StepNum,'g',4));

  ui->lineEdit_25->setText(QString::number(A_PreFaultTime,'g',4));
  ui->lineEdit_26->setText(QString::number(A_FaultTime,'g',4));
  ui->lineEdit_27->setText(QString::number(A_PostFaultTime,'g',4));
  ui->lineEdit_28->setText(QString::number(faultduration,'g',4));

  ui->lineEdit_29->setText(QString::number(m_Ch[3].Amp,'g',4));
  ui->lineEdit_30->setText(QString::number(m_Ch[3].Phase,'g',4));
  ui->lineEdit_31->setText(QString::number(m_Ch[3].Freq,'g',4));

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
    ftemp=value+A_SumStepnum*A_PhaStep;
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

        if(A_TestMode==2)
       {
          if(TripTime==-1)
          ui->textBrowser->setText(tr("动作时间(s:):未动作"));
          else
         {
           ui->textBrowser->setText(tr("动作时间(s:):")+QString::number(TripTime));
         }
       }

        else if(A_TestMode==3)
        {
            if(TripTime==-1)
             ui->textBrowser->setText(tr("动作值:未动作"));
            else
          {

            ui->textBrowser->setText(tr("动作值:"));
            switch(A_Variable)
            {
            case 1:  //幅值
                if(m_Ch[0].checked==1)
                {
                 ui->textBrowser->append(tr("VA:")+QString::number(m_Ch[0].Amp+A_SumStepnum*A_VolStep,'g',6)+tr(" V"));
                }
                if(m_Ch[1].checked==1)
                {
                 ui->textBrowser->append(tr("VB:")+QString::number(m_Ch[1].Amp+A_SumStepnum*A_VolStep,'g',6)+tr(" V"));
                }
                if(m_Ch[2].checked==1)
                {
                 ui->textBrowser->append(tr("VC:")+QString::number(m_Ch[2].Amp+A_SumStepnum*A_VolStep,'g',6)+tr(" V"));
                }

                if(VariatyVz2==0&&m_Ch[3].checked==1)
                  ui->textBrowser->append(tr("Vz:")+QString::number(m_Ch[3].Amp+A_SumStepnum*A_VolStep,'g',6)+tr(" V"));




                if(m_Ch[4].checked==1)
                {
                 ui->textBrowser->append(tr("IA:")+QString::number(m_Ch[4].Amp+A_SumStepnum*A_CurStep,'g',6)+tr(" A"));
                }
                if(m_Ch[5].checked==1)
                {
                 ui->textBrowser->append(tr("IB:")+QString::number(m_Ch[5].Amp+A_SumStepnum*A_CurStep,'g',6)+tr(" A"));
                }
                if(m_Ch[6].checked==1)
                {
                 ui->textBrowser->append(tr("IC:")+QString::number(m_Ch[6].Amp+A_SumStepnum*A_CurStep,'g',6)+tr(" A"));
                }

                break;
            case 2: // 相位
                float value;
                if(m_Ch[0].checked==1)
                {
                    value=showPhase(m_Ch[0].Phase);
                 ui->textBrowser->append(tr("VA:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                if(m_Ch[1].checked==1)
                {
                    value=showPhase(m_Ch[1].Phase);
                 ui->textBrowser->append(tr("VB:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                if(m_Ch[2].checked==1)
                {    value=showPhase(m_Ch[2].Phase);
                 ui->textBrowser->append(tr("VC:")+QString::number(value,'g',6)+tr(" Deg"));
                }


                if(VariatyVz2==0&&m_Ch[3].checked==1)
                {    value=showPhase(m_Ch[3].Phase);
                  ui->textBrowser->append(tr("Vz:")+QString::number(value,'g',6)+tr(" Deg"));
                }

                if(m_Ch[4].checked==1)
                {    value=showPhase(m_Ch[4].Phase);
                 ui->textBrowser->append(tr("IA:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                if(m_Ch[5].checked==1)
                {value=showPhase(m_Ch[5].Phase);
                 ui->textBrowser->append(tr("IB:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                if(m_Ch[6].checked==1)
                {value=showPhase(m_Ch[6].Phase);
                 ui->textBrowser->append(tr("IC:")+QString::number(value,'g',6)+tr(" Deg"));
                }
                break;


            case 3: //频率
                if(m_Ch[0].checked==1)
                {
                 ui->textBrowser->append(tr("VA:")+QString::number(m_Ch[0].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                }
                if(m_Ch[1].checked==1)
                {
                 ui->textBrowser->append(tr("VB:")+QString::number(m_Ch[1].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                }
                if(m_Ch[2].checked==1)
                {
                 ui->textBrowser->append(tr("VC:")+QString::number(m_Ch[2].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                }

                if(VariatyVz2==0&&m_Ch[3].checked==1)
                  ui->textBrowser->append(tr("Vz:")+QString::number(m_Ch[3].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));



                if(m_Ch[4].checked==1)
                {
                 ui->textBrowser->append(tr("IA:")+QString::number(m_Ch[4].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                }
                if(m_Ch[5].checked==1)
                {
                 ui->textBrowser->append(tr("IB:")+QString::number(m_Ch[5].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                }
                if(m_Ch[6].checked==1)
                {
                 ui->textBrowser->append(tr("IC:")+QString::number(m_Ch[6].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                }
                break;
            }


          }


        }

        else if(A_TestMode==1)
        {

          ui->textBrowser->setText(tr("动作值:"));
          switch(A_Variable)
          {
          case 1:  //幅值
              if(m_Ch[0].checked==1)
              {
               ui->textBrowser->append(tr("VA:")+QString::number(m_Ch[0].Amp,'g',6)+tr(" V"));
              }
              if(m_Ch[1].checked==1)
              {
               ui->textBrowser->append(tr("VB:")+QString::number(m_Ch[1].Amp,'g',6)+tr(" V"));
              }
              if(m_Ch[2].checked==1)
              {
               ui->textBrowser->append(tr("VC:")+QString::number(m_Ch[2].Amp,'g',6)+tr(" V"));
              }
              if(m_Ch[3].checked==1)
               ui->textBrowser->append(tr("Vz:")+QString::number(m_Ch[3].Amp,'g',6)+tr(" V"));





              if(m_Ch[4].checked==1)
              {
               ui->textBrowser->append(tr("IA:")+QString::number(m_Ch[4].Amp,'g',6)+tr(" A"));
              }
              if(m_Ch[5].checked==1)
              {
               ui->textBrowser->append(tr("IB:")+QString::number(m_Ch[5].Amp,'g',6)+tr(" A"));
              }
              if(m_Ch[6].checked==1)
              {
               ui->textBrowser->append(tr("IC:")+QString::number(m_Ch[6].Amp,'g',6)+tr(" A"));
              }

              break;
          case 2: // 相位
              if(m_Ch[0].checked==1)
              {
               ui->textBrowser->append(tr("VA:")+QString::number(m_Ch[0].Phase,'g',6)+tr(" Deg"));
              }
              if(m_Ch[1].checked==1)
              {
               ui->textBrowser->append(tr("VB:")+QString::number(m_Ch[1].Phase,'g',6)+tr(" Deg"));
              }
              if(m_Ch[2].checked==1)
              {
               ui->textBrowser->append(tr("VC:")+QString::number(m_Ch[2].Phase,'g',6)+tr(" Deg"));
              }

              if(m_Ch[3].checked==1)
               ui->textBrowser->append(tr("Vz:")+QString::number(m_Ch[3].Phase,'g',6)+tr(" Deg"));


              if(m_Ch[4].checked==1)
              {
               ui->textBrowser->append(tr("IA:")+QString::number(m_Ch[4].Phase,'g',6)+tr(" Deg"));
              }
              if(m_Ch[5].checked==1)
              {
               ui->textBrowser->append(tr("IB:")+QString::number(m_Ch[5].Phase,'g',6)+tr(" Deg"));
              }
              if(m_Ch[6].checked==1)
              {
               ui->textBrowser->append(tr("IC:")+QString::number(m_Ch[6].Phase,'g',6)+tr(" Deg"));
              }
              break;


          case 3: //频率
              if(m_Ch[0].checked==1)
              {
               ui->textBrowser->append(tr("VA:")+QString::number(m_Ch[0].Freq,'g',6)+tr(" Hz"));
              }
              if(m_Ch[1].checked==1)
              {
               ui->textBrowser->append(tr("VB:")+QString::number(m_Ch[1].Freq,'g',6)+tr(" Hz"));
              }
              if(m_Ch[2].checked==1)
              {
               ui->textBrowser->append(tr("VC:")+QString::number(m_Ch[2].Freq,'g',6)+tr(" Hz"));
              }

              if(m_Ch[3].checked==1)
               ui->textBrowser->append(tr("Vz:")+QString::number(m_Ch[3].Freq,'g',6)+tr(" Hz"));


              if(m_Ch[4].checked==1)
              {
               ui->textBrowser->append(tr("IA:")+QString::number(m_Ch[4].Freq,'g',6)+tr(" Hz"));
              }
              if(m_Ch[5].checked==1)
              {
               ui->textBrowser->append(tr("IB:")+QString::number(m_Ch[5].Freq,'g',6)+tr(" Hz"));
              }
              if(m_Ch[6].checked==1)
              {
               ui->textBrowser->append(tr("IC:")+QString::number(m_Ch[6].Freq,'g',6)+tr(" Hz"));
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
           switch(m_Ch[0].DC)
           {
             case 0:  //VA  交流
               if(m_Ch[0].Amp>MAX_V_VALUEDATA_AC)
                 m_Ch[0].Amp=MAX_V_VALUEDATA_AC;
               else if(m_Ch[0].Amp<0)
                 m_Ch[0].Amp=0;
               break;
             case 1:  //VA  直流
               if(m_Ch[0].Amp>MAX_V_VALUEDATA_DC)
                 m_Ch[0].Amp=MAX_V_VALUEDATA_DC;
               else if(m_Ch[0].Amp<MIN_V_VALUEDATA_DC)
                 m_Ch[0].Amp=MIN_V_VALUEDATA_DC;
               break;
           }


         break;
        case 2:
           if(m_Ch[0].Phase>180)
               m_Ch[0].Phase=m_Ch[0].Phase-360;
           else if(m_Ch[0].Phase<-180)
              m_Ch[0].Phase=(360+m_Ch[0].Phase);
           if(A_Phasesymmetry)
           {
            m_Ch[1].Phase=m_Ch[0].Phase-120;
            m_Ch[2].Phase=m_Ch[0].Phase+120;
            if(m_Ch[1].Phase>180)
                m_Ch[1].Phase=m_Ch[1].Phase-360;
            else if(m_Ch[1].Phase<-180)
                m_Ch[1].Phase+=360;
            if(m_Ch[2].Phase>180)
                m_Ch[2].Phase=m_Ch[2].Phase-360;
            else if(m_Ch[2].Phase<-180)
                m_Ch[2].Phase+=360;

           }
           break;
        case 3:
           if(m_Ch[0].Freq>1000)
             m_Ch[0].Freq=1000;
           else if(m_Ch[0].Freq<0)
             m_Ch[0].Freq=0;
           if(m_Ch[0].Freq==0)
             {
               m_Ch[0].DC=1;
               if(m_Ch[0].Amp<MIN_V_VALUEDATA_DC)
               {
                   m_Ch[0].Amp=MIN_V_VALUEDATA_DC;
                   ui->lineEdit->setText(QString::number(m_Ch[0].Amp,'g',4));
               }
               else if(m_Ch[0].Amp>MAX_V_VALUEDATA_DC)
               {
                   m_Ch[0].Amp=MAX_V_VALUEDATA_DC;
                   ui->lineEdit->setText(QString::number(m_Ch[0].Amp,'g',4));
               }
             }
           else
             {
               m_Ch[0].DC=0;
               if(m_Ch[0].Amp<MIN_V_VALUEDATA_AC)
               {
                   m_Ch[0].Amp=MIN_V_VALUEDATA_AC;
                   ui->lineEdit->setText(QString::number(m_Ch[0].Amp,'g',4));
               }
               else if(m_Ch[0].Amp>MAX_V_VALUEDATA_AC)
               {
                   m_Ch[0].Amp=MAX_V_VALUEDATA_AC;
                   ui->lineEdit->setText(QString::number(m_Ch[0].Amp,'g',4));
               }
             }
            break;
      case 4:       //VB
           switch(m_Ch[1].DC)
           {
             case 0:  //VB  交流
               if(m_Ch[1].Amp>MAX_V_VALUEDATA_AC)
                 m_Ch[1].Amp=MAX_V_VALUEDATA_AC;
               else if(m_Ch[1].Amp<0)
                 m_Ch[1].Amp=0;
               break;
             case 1:  //VB  直流
               if(m_Ch[1].Amp>MAX_V_VALUEDATA_DC)
                 m_Ch[1].Amp=MAX_V_VALUEDATA_DC;
               else if(m_Ch[1].Amp<MIN_V_VALUEDATA_DC)
                 m_Ch[1].Amp=MIN_V_VALUEDATA_DC;
               break;
           }
           //鏇存柊鏄剧ず骞呭€?
          break;
       case 5:
           //V2鐩镐綅
          if(m_Ch[1].Phase>180)
          m_Ch[1].Phase=m_Ch[1].Phase-360;
          else if(m_Ch[1].Phase<-180)
          m_Ch[1].Phase+=360;

          if(A_Phasesymmetry)
          {
              m_Ch[0].Phase=m_Ch[1].Phase+120;
              if(m_Ch[0].Phase>180)
                  m_Ch[0].Phase-=360;
              else if(m_Ch[0].Phase<-180)
                  m_Ch[0].Phase+=360;

              m_Ch[2].Phase=m_Ch[0].Phase+120;
              if(m_Ch[2].Phase>180)
                  m_Ch[2].Phase-=360;
              else if(m_Ch[2].Phase<-180)
                  m_Ch[2].Phase+=360;
          }
           break;
       case 6:   //
             if(m_Ch[1].Freq>1000)
              m_Ch[1].Freq=1000;
            else if(m_Ch[1].Freq<0)
              m_Ch[1].Freq=0;
            if(m_Ch[1].Freq==0)   //濡傛灉棰戠巼涓?锛屽垯璇ヤ俊鍙蜂负鐩存祦锛屽惁鍒欎负浜ゆ祦銆?
              {
                m_Ch[1].DC=1;
                if(m_Ch[1].Amp<MIN_V_VALUEDATA_DC)
                {
                    m_Ch[1].Amp=MIN_V_VALUEDATA_DC;
                    ui->lineEdit_4->setText(QString::number(m_Ch[1].Amp,'g',4));
                }
                else if(m_Ch[1].Amp>MAX_V_VALUEDATA_DC)
                {
                    m_Ch[1].Amp=MAX_V_VALUEDATA_DC;
                    ui->lineEdit_4->setText(QString::number(m_Ch[1].Amp,'g',4));
                }
              }
            else
              {
                m_Ch[1].DC=0;
                if(m_Ch[1].Amp<MIN_V_VALUEDATA_AC)
                {
                    m_Ch[1].Amp=MIN_V_VALUEDATA_AC;
                    ui->lineEdit_4->setText(QString::number(m_Ch[1].Amp,'g',4));
                }
                else if(m_Ch[1].Amp>MAX_V_VALUEDATA_AC)
                {
                    m_Ch[1].Amp=MAX_V_VALUEDATA_AC;
                    ui->lineEdit_4->setText(QString::number(m_Ch[1].Amp,'g',4));
                }
              }
            break;
        case 7:      //V3骞呭€?
            switch(m_Ch[2].DC)
              {
                case 0:  //V3涓轰氦娴?
                  if(m_Ch[2].Amp>MAX_V_VALUEDATA_AC)
                    m_Ch[2].Amp=MAX_V_VALUEDATA_AC;
                  else if(m_Ch[2].Amp<0)
                    m_Ch[2].Amp=0;
                  break;
                case 1:  //V3涓虹洿娴?
                  if(m_Ch[2].Amp>MAX_V_VALUEDATA_DC)
                    m_Ch[2].Amp=MAX_V_VALUEDATA_DC;
                  else if(m_Ch[2].Amp<MIN_V_VALUEDATA_DC)
                    m_Ch[2].Amp=MIN_V_VALUEDATA_DC;
                  break;
              }
             //鏇存柊鏄剧ず V3骞呭€?
            break;
        case 8:   //V3鐩镐綅
            if(m_Ch[2].Phase>180)
              m_Ch[2].Phase-=360;
            else if(m_Ch[2].Phase<-180)
              m_Ch[2].Phase+=360;
            if(A_Phasesymmetry)
            {
                m_Ch[0].Phase=m_Ch[2].Phase-120;
                if(m_Ch[0].Phase>180)
                    m_Ch[0].Phase-=360;
                else if(m_Ch[0].Phase<-180)
                    m_Ch[0].Phase+=360;
                m_Ch[1].Phase=m_Ch[0].Phase-120;
                if(m_Ch[1].Phase>180)
                    m_Ch[1].Phase-=360;
                else if(m_Ch[1].Phase<-180)
                    m_Ch[1].Phase+=360;
            }


            break;

       case 9: //V3棰戠巼
            if(m_Ch[2].Freq>1000)
                 m_Ch[2].Freq=1000;
            else if(m_Ch[2].Freq<0)
                 m_Ch[2].Freq=0;
            if(m_Ch[2].Freq==0)   //濡傛灉棰戠巼涓?锛屽垯璇ヤ俊鍙蜂负鐩存祦锛屽惁鍒欎负浜ゆ祦銆?
            {

                m_Ch[2].DC=1;
                if(m_Ch[2].Amp<MIN_V_VALUEDATA_DC)
                {
                    m_Ch[2].Amp=MIN_V_VALUEDATA_DC;
                    ui->lineEdit_7->setText(QString::number(m_Ch[2].Amp,'g',4));
                }
                else if(m_Ch[2].Amp>MAX_V_VALUEDATA_DC)
                {
                    m_Ch[2].Amp=MAX_V_VALUEDATA_DC;
                    ui->lineEdit_7->setText(QString::number(m_Ch[2].Amp,'g',4));
                }
             }
           else
             {
                 m_Ch[2].DC=0;
                 if(m_Ch[2].Amp<MIN_V_VALUEDATA_AC)
                 {
                     m_Ch[2].Amp=MIN_V_VALUEDATA_AC;
                     ui->lineEdit_7->setText(QString::number(m_Ch[2].Amp,'g',4));
                 }
                 else if(m_Ch[2].Amp>MAX_V_VALUEDATA_AC)
                 {
                     m_Ch[2].Amp=MAX_V_VALUEDATA_AC;
                     ui->lineEdit_7->setText(QString::number(m_Ch[2].Amp,'g',4));
                 }
             }



              break;

case 10:           //I1骞呭€?
            switch(m_Ch[4].DC)
              {
                case 0:  //I1交流
                  if(m_Ch[4].Amp>MAX_I_VALUEDATA_AC)
                    m_Ch[4].Amp=MAX_I_VALUEDATA_AC;
                  else if(m_Ch[4].Amp<0)
                    m_Ch[4].Amp=0;
                  break;
                case 1:  //V1直流
                  if(m_Ch[4].Amp>MAX_I_VALUEDATA_DC)
                    m_Ch[4].Amp=MAX_I_VALUEDATA_DC;
                  else if(m_Ch[4].Amp<MIN_I_VALUEDATA_DC)
                    m_Ch[4].Amp=MIN_I_VALUEDATA_DC;
                  break;
              }

            break;

case 11:  //I1鐩镐綅
          if(m_Ch[4].Phase>180)
            m_Ch[4].Phase-=360;
          else if(m_Ch[4].Phase<-180)
            m_Ch[4].Phase+=360;


          if(A_Phasesymmetry)
          {
           m_Ch[5].Phase=m_Ch[4].Phase-120;
           m_Ch[6].Phase=m_Ch[4].Phase+120;
           if(m_Ch[5].Phase>180)
               m_Ch[5].Phase=m_Ch[5].Phase-360;
           else if(m_Ch[5].Phase<-180)
               m_Ch[5].Phase+=360;
           if(m_Ch[6].Phase>180)
               m_Ch[6].Phase=m_Ch[6].Phase-360;
           else if(m_Ch[6].Phase<-180)
               m_Ch[6].Phase+=360;

          }





          break;
case 12:  //I1棰戠巼
              if(m_Ch[4].Freq>1000)
                m_Ch[4].Freq=1000;
              else if(m_Ch[4].Freq<-1000)
               m_Ch[4].Freq=-1000;
           if(m_Ch[4].Freq==0)   //是否直流
            {
              m_Ch[4].DC=1;//直流
              if(m_Ch[4].Amp<MIN_I_VALUEDATA_DC)
              {
                  m_Ch[4].Amp=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_10->setText(QString::number(m_Ch[4].Amp,'g',4));
              }
              else if(m_Ch[4].Amp>MAX_I_VALUEDATA_DC)
              {
                  m_Ch[4].Amp=MAX_I_VALUEDATA_DC;
                  ui->lineEdit_10->setText(QString::number(m_Ch[4].Amp,'g',4));
              }
              if(A_CurStep<MIN_I_VALUEDATA_DC)
              {
                  A_CurStep=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
              else if(A_CurStep>30/*MAX_I_VALUEDATA_DC*/)
              {
                  A_CurStep=30/*MAX_I_VALUEDATA_DC*/;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
            }
          else
            {
              m_Ch[4].DC=0;//交流
              if(m_Ch[4].Amp<MIN_I_VALUEDATA_AC)
              {
                  m_Ch[4].Amp=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_10->setText(QString::number(m_Ch[4].Amp,'g',4));
              }
              else if(m_Ch[4].Amp>MAX_I_VALUEDATA_AC)
              {
                  m_Ch[4].Amp=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_10->setText(QString::number(m_Ch[4].Amp,'g',4));
              }
              if(A_CurStep<MIN_I_VALUEDATA_AC)
              {
                  A_CurStep=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
              else if(A_CurStep>MAX_I_VALUEDATA_AC)
              {
                  A_CurStep=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
            }
          break;


 case 13:    //I2骞呭€?
          switch(m_Ch[5].DC)
            {
              case 0:  //I2涓轰氦娴?
                if(m_Ch[5].Amp>MAX_I_VALUEDATA_AC)
                  m_Ch[5].Amp=MAX_I_VALUEDATA_AC;
                else if(m_Ch[5].Amp<0)
                  m_Ch[5].Amp=0;
                break;
              case 1:  //V1涓虹洿娴?
                if(m_Ch[5].Amp>MAX_I_VALUEDATA_DC)
                  m_Ch[5].Amp=MAX_I_VALUEDATA_DC;
                else if(m_Ch[5].Amp<MIN_I_VALUEDATA_DC)
                  m_Ch[5].Amp=MIN_I_VALUEDATA_DC;
                break;
            }


          break;
 case 14:   //I2鐩镐綅
          if(m_Ch[5].Phase>180)
            m_Ch[5].Phase-=360;
          else if(m_Ch[5].Phase<-180)
            m_Ch[5].Phase+=360;

          if(A_Phasesymmetry)
          {
              m_Ch[4].Phase=m_Ch[5].Phase+120;
              if(m_Ch[4].Phase>180)
                  m_Ch[4].Phase-=360;
              else if(m_Ch[4].Phase<-180)
                  m_Ch[4].Phase+=360;

              m_Ch[6].Phase=m_Ch[4].Phase+120;
              if(m_Ch[6].Phase>180)
                  m_Ch[6].Phase-=360;
              else if(m_Ch[6].Phase<-180)
                  m_Ch[6].Phase+=360;
          }



          //鏇存柊鏄剧ずI2鐩镐綅
          break;
case 15:  //I2棰戠巼
          if(m_Ch[5].Freq>1000)
            m_Ch[5].Freq=1000;
          else if(m_Ch[5].Freq<0)
           m_Ch[5].Freq=0;
          if(m_Ch[5].Freq==0)   //濡傛灉棰戠巼涓?锛屽垯璇ヤ俊鍙蜂负鐩存祦锛屽惁鍒欎负浜ゆ祦銆?
            {
              m_Ch[5].DC=1;
              if(m_Ch[5].Amp<MIN_I_VALUEDATA_DC)
              {
                  m_Ch[5].Amp=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_13->setText(QString::number(m_Ch[5].Amp,'g',4));
              }
              else if(m_Ch[5].Amp>MAX_I_VALUEDATA_DC)
              {
                  m_Ch[5].Amp=MAX_I_VALUEDATA_DC;
                  ui->lineEdit_13->setText(QString::number(m_Ch[5].Amp,'g',4));
              }
              if(A_CurStep<MIN_I_VALUEDATA_DC)
              {
                  A_CurStep=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
              else if(A_CurStep>30/*MAX_I_VALUEDATA_DC*/)
              {
                  A_CurStep=30/*MAX_I_VALUEDATA_DC*/;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
            }
          else
            {
              m_Ch[5].DC=0;
              if(m_Ch[5].Amp<MIN_I_VALUEDATA_AC)
              {
                  m_Ch[5].Amp=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_13->setText(QString::number(m_Ch[5].Amp,'g',4));
              }
              else if(m_Ch[5].Amp>MAX_I_VALUEDATA_AC)
              {
                  m_Ch[5].Amp=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_13->setText(QString::number(m_Ch[5].Amp,'g',4));
              }
              if(A_CurStep<MIN_I_VALUEDATA_AC)
              {
                  A_CurStep=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
              else if(A_CurStep>MAX_I_VALUEDATA_AC)
              {
                  A_CurStep=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
            }

          break;
  case 16:   //I3骞呭€?
          switch(m_Ch[6].DC)
            {
              case 0:  //I3涓轰氦娴?
                if(m_Ch[6].Amp>MAX_I_VALUEDATA_AC)
                  m_Ch[6].Amp=MAX_I_VALUEDATA_AC;
                else if(m_Ch[6].Amp<0)
                  m_Ch[6].Amp=0;
                break;
              case 1:  //V1涓虹洿娴?
                if(m_Ch[6].Amp>MAX_I_VALUEDATA_DC)
                  m_Ch[6].Amp=MAX_I_VALUEDATA_DC;
                else if(m_Ch[6].Amp<MIN_I_VALUEDATA_DC)
                  m_Ch[6].Amp=MIN_I_VALUEDATA_DC;
                break;
            }
          //鏇存柊鏄剧ずI3骞呭€?
          break;
 case 17:  //I3鐩镐綅
          if(m_Ch[6].Phase>180)
            m_Ch[6].Phase-=360;
          else if(m_Ch[6].Phase<-180)
            m_Ch[6].Phase+=360;
          if(A_Phasesymmetry)
          {
              m_Ch[4].Phase=m_Ch[6].Phase-120;
              if(m_Ch[4].Phase>180)
                  m_Ch[4].Phase-=360;
              else if(m_Ch[4].Phase<-180)
                  m_Ch[4].Phase+=360;
              m_Ch[5].Phase=m_Ch[4].Phase-120;
              if(m_Ch[5].Phase>180)
                  m_Ch[5].Phase-=360;
              else if(m_Ch[5].Phase<-180)
                  m_Ch[5].Phase+=360;
          }

          //鏇存柊鏄剧ず
          break;
 case 18:  //I3棰戠巼
          if(m_Ch[6].Freq>1000)
            m_Ch[6].Freq=1000;
          else if(m_Ch[6].Freq<0)
            m_Ch[6].Freq=0;
          if(m_Ch[6].Freq==0)   //濡傛灉棰戠巼涓?锛屽垯璇ヤ俊鍙蜂负鐩存祦锛屽惁鍒欎负浜ゆ祦銆?
            {

              m_Ch[6].DC=1;
              if(m_Ch[6].Amp<MIN_I_VALUEDATA_DC)
              {
                  m_Ch[6].Amp=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_16->setText(QString::number(m_Ch[6].Amp,'g',4));
              }
              else if(m_Ch[6].Amp>MAX_I_VALUEDATA_DC)
              {
                  m_Ch[6].Amp=MAX_I_VALUEDATA_DC;
                  ui->lineEdit_16->setText(QString::number(m_Ch[6].Amp,'g',4));
              }
              if(A_CurStep<MIN_I_VALUEDATA_DC)
              {
                  A_CurStep=MIN_I_VALUEDATA_DC;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
              else if(A_CurStep>30/*MAX_I_VALUEDATA_DC*/)
              {
                  A_CurStep=30/*MAX_I_VALUEDATA_DC*/;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }

            }
          else
            {

              m_Ch[6].DC=0;
              if(m_Ch[6].Amp<MIN_I_VALUEDATA_AC)
              {
                  m_Ch[6].Amp=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_16->setText(QString::number(m_Ch[6].Amp,'g',4));
              }
              else if(m_Ch[6].Amp>MAX_I_VALUEDATA_AC)
              {
                  m_Ch[6].Amp=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_16->setText(QString::number(m_Ch[6].Amp,'g',4));
              }
              if(A_CurStep<MIN_I_VALUEDATA_AC)
              {
                  A_CurStep=MIN_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }
              else if(A_CurStep>MAX_I_VALUEDATA_AC)
              {
                  A_CurStep=MAX_I_VALUEDATA_AC;
                  ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
              }

            }
          break;

    case 19:
           if(A_TestMode==1)
           {
           switch(m_Ch[4].DC)
             {
               case 0:  //交流
                 if(A_CurStep>MAX_I_VALUEDATA_AC)
                   A_CurStep=MAX_I_VALUEDATA_AC;
                 else if(A_CurStep<0)
                   A_CurStep=0;
                 break;
               case 1:  //鐩存祦鏃剁數娴佹闀块檺瀹氬€?
                 if(A_CurStep>30/*MAX_I_VALUEDATA_DC*/)
                   A_CurStep=30/*MAX_I_VALUEDATA_DC*/;
                 else if(A_CurStep<0)
                   A_CurStep=0;
                 break;
             }
           }
           else if(A_TestMode==3)
           {
               switch(m_Ch[4].DC)
                 {
                   case 0:  //交流
                     if(A_CurStep>MAX_I_VALUEDATA_AC)
                       A_CurStep=MAX_I_VALUEDATA_AC;
                     else if(A_CurStep<0-MAX_I_VALUEDATA_AC)
                       A_CurStep=0-MAX_I_VALUEDATA_AC;
                     break;
                   case 1:  //鐩存祦鏃剁數娴佹闀块檺瀹氬€?
                     if(A_CurStep>MAX_I_VALUEDATA_DC)
                       A_CurStep=MAX_I_VALUEDATA_DC;
                     else if(A_CurStep<0-MAX_I_VALUEDATA_DC)
                       A_CurStep=0-MAX_I_VALUEDATA_DC;
                     break;
                 }
           }
            break;

    case 20:
           if(A_TestMode==1)
           {
            switch(m_Ch[4].DC)
            {
            case 0:  //浜ゆ祦鏃剁數娴佹闀块檺瀹氬€?
              if(A_VolStep>MAX_V_VALUEDATA_AC)
                A_VolStep=MAX_V_VALUEDATA_AC;
              else if(A_VolStep<0)
                A_VolStep=0;
              break;
            case 1:  //鐩存祦鏃剁數娴佹闀块檺瀹氬€?
              if(A_VolStep>MAX_V_VALUEDATA_DC)
                A_VolStep=MAX_V_VALUEDATA_DC;
              else if(A_VolStep<0)
                A_VolStep=0;
              break;
            }
           }
           if(A_TestMode==3)
           {
            switch(m_Ch[4].DC)
            {
            case 0:  //浜ゆ祦鏃剁數娴佹闀块檺瀹氬€?
              if(A_VolStep>MAX_V_VALUEDATA_AC)
                A_VolStep=MAX_V_VALUEDATA_AC;
              else if(A_VolStep<0-MAX_V_VALUEDATA_AC)
                A_VolStep=0-MAX_V_VALUEDATA_AC;
              break;
            case 1:  //鐩存祦鏃剁數娴佹闀块檺瀹氬€?
              if(A_VolStep>MAX_V_VALUEDATA_DC)
                A_VolStep=MAX_V_VALUEDATA_DC;
              else if(A_VolStep<0-MAX_V_VALUEDATA_DC)
                A_VolStep=0-MAX_V_VALUEDATA_DC;
              break;
            }
           }
            //鏇存柊鏄剧ず鐢靛帇姝ラ暱
            break;
    case 21: //鐩镐綅姝ラ暱
           if(A_TestMode==1)
           {
                    if(A_PhaStep>180)
                      A_PhaStep=180;
                    else if(A_PhaStep<0)
                      A_PhaStep=0;
           }
           if(A_TestMode==3)
           {
                    if(A_PhaStep>180)
                      A_PhaStep=180;
                    else if(A_PhaStep<-180)
                      A_PhaStep=-180;
           }

                    //鏇存柊鏄剧ず姝ラ暱
                    break;

    case 22:   //棰戠巼姝ラ暱
           if(A_TestMode==1)
           {
                  if(A_FreStep>100)
                    A_FreStep=1;
                  else if(A_FreStep<0)
                    A_FreStep=0;
           }
           if(A_TestMode==3)
           {
                  if(A_FreStep>100)
                    A_FreStep=1;
                  else if(A_FreStep<-100)
                    A_FreStep=-1;
           }
                  //鏇存柊鏄剧ず棰戠巼姝ラ暱
                  break;

    case 23:
                             //姝ラ暱鏃堕棿
                             if(A_StepTime>99.99)
                               A_StepTime=99.99;
                             else if(A_StepTime<0)
                               A_StepTime=0.5;
                             //鏇存柊鐣岄潰鏄剧ず
                             break;
    case 24:
                             if(A_StepNum>1000)
                               A_StepNum=1000;
                             else if(A_StepNum<1)
                               A_StepNum=1;
                             //鏇存柊鐣岄潰鏄剧ず
                             break;
    case 25:
                             if(A_PreFaultTime>999.9)
                              A_PreFaultTime=999.9;
                             else if(A_PreFaultTime<0)
                             A_PreFaultTime=0;
                            //鏇存柊鐣岄潰鏄剧ず
                              break;
    case 26:
             if(A_FaultTime>999.9)
             A_FaultTime=999.9;
             else if(A_FaultTime<0)
             A_FaultTime=0;
             //鏇存柊鐣岄潰鏄剧ず
              break;

    case 27:
            if(A_PostFaultTime>999.9)
                A_PostFaultTime=999.9;
            else if(A_PostFaultTime<0)
                A_PostFaultTime=0;
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
            switch(m_Ch[3].DC)
              {
                case 0:  //Vz为交流
                  if(m_Ch[3].Amp>MAX_V_VALUEDATA_AC)
                    m_Ch[3].Amp=MAX_V_VALUEDATA_AC;
                  else if(m_Ch[3].Amp<0)
                    m_Ch[3].Amp=0;
                  break;
                case 1:  //Vz为直流
                  if(m_Ch[3].Amp>MAX_V_VALUEDATA_DC)
                    m_Ch[3].Amp=MAX_V_VALUEDATA_DC;
                  else if(m_Ch[3].Amp<MIN_V_VALUEDATA_DC)
                    m_Ch[3].Amp=MIN_V_VALUEDATA_DC;
                  break;
              }
             break;
     case 30:  //Vz相位
          if(m_Ch[3].Phase>180)
            m_Ch[3].Phase=180;
          else if(m_Ch[3].Phase<-180)
            m_Ch[3].Phase=-180;
          break;
     case 31:  //Vz频率
          if(m_Ch[3].Freq>1000)
            m_Ch[3].Freq=1000;
          else if(m_Ch[3].Freq<0)
            m_Ch[3].Freq=0;
          if(m_Ch[3].Freq==0)
          {//如果频率为0，则该信号为直流，否则为交流。
              m_Ch[3].DC=1;
              if(m_Ch[3].Amp<MIN_V_VALUEDATA_DC)
              {
                m_Ch[3].Amp=MIN_V_VALUEDATA_DC;
                ui->lineEdit_29->setText(QString::number(m_Ch[3].Amp,'g',4));
              }
              else if(m_Ch[3].Amp>MAX_V_VALUEDATA_DC)
              {
                m_Ch[3].Amp=MAX_V_VALUEDATA_DC;
                ui->lineEdit_29->setText(QString::number(m_Ch[3].Amp,'g',4));
              }
           }
          else
           {
              m_Ch[3].DC=0;
              if(m_Ch[3].Amp<MIN_V_VALUEDATA_AC)
              {
                m_Ch[3].Amp=MIN_V_VALUEDATA_AC;
                ui->lineEdit_29->setText(QString::number(m_Ch[3].Amp,'g',4));
              }
              else if(m_Ch[3].Amp>MAX_V_VALUEDATA_AC)
              {
                m_Ch[3].Amp=MAX_V_VALUEDATA_AC;
                ui->lineEdit_29->setText(QString::number(m_Ch[3].Amp,'g',4));
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
        case 1:ui->lineEdit->setText(QString::number(m_Ch[0].Amp,'g',6)); break;
        case 2:ui->lineEdit_2->setText(QString::number(m_Ch[0].Phase,'g',6)); break;
        case 3:ui->lineEdit_3->setText(QString::number(m_Ch[0].Freq,'g',6)); break;
        case 4:ui->lineEdit_4->setText(QString::number(m_Ch[1].Amp,'g',6)); break;
        case 5:ui->lineEdit_5->setText(QString::number(m_Ch[1].Phase,'g',6)); break;
        case 6:ui->lineEdit_6->setText(QString::number(m_Ch[1].Freq,'g',6)); break;
        case 7:lineedit->setText(QString::number(m_Ch[2].Amp,'g',6)); break;
        case 8:lineedit->setText(QString::number(m_Ch[2].Phase,'g',6)); break;
        case 9:lineedit->setText(QString::number(m_Ch[2].Freq,'g',6)); break;

        case 10:lineedit->setText(QString::number(m_Ch[4].Amp,'g',6)); break;
        case 11:lineedit->setText(QString::number(m_Ch[4].Phase,'g',6)); break;
        case 12:lineedit->setText(QString::number(m_Ch[4].Freq,'g',6)); break;
        case 13:lineedit->setText(QString::number(m_Ch[5].Amp,'g',6)); break;
        case 14:lineedit->setText(QString::number(m_Ch[5].Phase,'g',6)); break;
        case 15:lineedit->setText(QString::number(m_Ch[5].Freq,'g',6)); break;
        case 16:lineedit->setText(QString::number(m_Ch[6].Amp,'g',6)); break;
        case 17:lineedit->setText(QString::number(m_Ch[6].Phase,'g',6)); break;
        case 18:lineedit->setText(QString::number(m_Ch[6].Freq,'g',6)); break;


        case 19:lineedit->setText(QString::number(A_CurStep,'g',6)); break;
        case 20:lineedit->setText(QString::number(A_VolStep,'g',6)); break;
        case 21:lineedit->setText(QString::number(A_PhaStep,'g',6)); break;
        case 22:lineedit->setText(QString::number(A_FreStep,'g',6)); break;
        case 23:lineedit->setText(QString::number(A_StepTime,'g',6)); break;
        case 24:lineedit->setText(QString::number(A_StepNum,'g',6)); break;
        case 25:lineedit->setText(QString::number(A_PreFaultTime,'g',6)); break;
        case 26:lineedit->setText(QString::number(A_FaultTime,'g',6)); break;
        case 27:lineedit->setText(QString::number(A_PostFaultTime,'g',6)); break;
        case 28:lineedit->setText(QString::number(faultduration,'g',6)); break;

        case 29:lineedit->setText(QString::number(m_Ch[3].Amp,'g',6)); break;
        case 30:lineedit->setText(QString::number(m_Ch[3].Phase,'g',6)); break;
        case 31:lineedit->setText(QString::number(m_Ch[3].Freq,'g',6)); break;

        }

        return;
    }



}

 void  Manualtest::updateVz()
{

     if(m_Ch[0].Freq==SysFrec&&m_Ch[1].Freq==SysFrec&&m_Ch[2].Freq==SysFrec&&VariatyVz2==1)
     {
     float VzReal,VzImaginary;
     //实部
     VzReal=m_Ch[0].Amp*cos(m_Ch[0].Phase*3.14159/180)+m_Ch[1].Amp*cos(m_Ch[1].Phase*3.14159/180)+m_Ch[2].Amp*cos(m_Ch[2].Phase*3.14159/180);
     //虚部
     VzImaginary=m_Ch[0].Amp*sin(m_Ch[0].Phase*3.14159/180)+m_Ch[1].Amp*sin(m_Ch[1].Phase*3.14159/180)+m_Ch[2].Amp*sin(m_Ch[2].Phase*3.14159/180);
     m_Ch[3].Amp=sqrt(VzReal*VzReal+VzImaginary*VzImaginary);
     if(VzReal>=-0.0002&&VzReal<=0.0002)
     {
         if(VzImaginary>=-0.0002&&VzImaginary<=0.0002)
             m_Ch[3].Phase=0;
         else if(VzImaginary>0)
             m_Ch[3].Phase=90;
         else
             m_Ch[3].Phase=-90;
     }
     else if(VzReal<0)
     {
         m_Ch[3].Phase=atan(VzImaginary/VzReal)*180/3.14159;
         m_Ch[3].Phase=m_Ch[3].Phase+180;//2016-4-28修改，VZPhase的改变
     }
     else
     {
        m_Ch[3].Phase=atan(VzImaginary/VzReal)*180/3.14159;
     }
     m_Ch[3].Phase=fmod(m_Ch[3].Phase,360);//m_Ch[3].Phase%360
     m_Ch[3].Freq=SysFrec;

     if(m_Ch[3].Amp>=-0.0002&&m_Ch[3].Amp<=0.0002)
        m_Ch[3].Amp=0;
     if(m_Ch[3].Phase>=-0.0002&&m_Ch[3].Phase<=0.0002)
        m_Ch[3].Phase=0;




     ui->lineEdit_29->setText(QString::number(m_Ch[3].Amp,'g',4));
     ui->lineEdit_30->setText(QString::number(m_Ch[3].Phase,'g',4));
     ui->lineEdit_31->setText(QString::number(m_Ch[3].Freq,'g',4));
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
      switch(A_Variable)
        {
          case 1:
            if(m_Ch[0].checked==1&&flag)
                ui->lineEdit->setText(QString::number(ChangeV1Value,'g',4));
            if(m_Ch[1].checked==1&&flag)
                ui->lineEdit_4->setText(QString::number(ChangeV2Value,'g',4));
            if(m_Ch[2].checked==1&&flag)
              ui->lineEdit_7->setText(QString::number(ChangeV3Value,'g',4));
            //if(m_Ch[3].checked==1&&flag)
              //ui->lineEdit_29->setText(QString::number(ChangeVzValue,'g',4));

            if(m_Ch[4].checked==1&&flag)
              ui->lineEdit_10->setText(QString::number(ChangeI1Value,'g',4));
            if(m_Ch[5].checked==1&&flag)
              ui->lineEdit_13->setText(QString::number(ChangeI2Value,'g',4));
            if(m_Ch[6].checked==1&&flag)
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

            if(m_Ch[0].checked==1&&m_Ch[0].DC==0&&flag)
              ui->lineEdit_2->setText(QString::number(ChangeV1Phase,'g',4));
            if(m_Ch[1].checked==1&&m_Ch[1].DC==0&&flag)
              ui->lineEdit_5->setText(QString::number(ChangeV2Phase,'g',4));
            if(m_Ch[2].checked==1&&m_Ch[2].DC==0&&flag)
              ui->lineEdit_8->setText(QString::number(ChangeV3Phase,'g',4));

            //if(m_Ch[3].checked==1&&m_Ch[3].DC==0&&flag)
              //ui->lineEdit_30->setText(QString::number(ChangeVzPhase,'g',4));

            if(m_Ch[4].checked==1&&m_Ch[4].DC==0&&flag)
             ui->lineEdit_11->setText(QString::number(ChangeI1Phase,'g',4));
            if(m_Ch[5].checked==1&&m_Ch[5].DC==0&&flag)
              ui->lineEdit_14->setText(QString::number(ChangeI2Phase,'g',4));
            if(m_Ch[6].checked==1&&m_Ch[6].DC==0&&flag)
              ui->lineEdit_17->setText(QString::number(ChangeI3Phase,'g',4));
          break;

          case 3:
            if(m_Ch[0].checked==1&&m_Ch[0].DC==0&&flag)
              ui->lineEdit_3->setText(QString::number(ChangeV1Frec,'g',4));
            if(m_Ch[1].checked==1&&m_Ch[1].DC==0&&flag)
              ui->lineEdit_6->setText(QString::number(ChangeV2Frec,'g',4));
            if(m_Ch[2].checked==1&&m_Ch[2].DC==0&&flag)
              ui->lineEdit_9->setText(QString::number(ChangeV3Frec,'g',4));
            //if(m_Ch[3].checked==1&&m_Ch[3].DC==0&&flag)
              //ui->lineEdit_31->setText(QString::number(ChangeVzFrec,'g',4));

            if(m_Ch[4].checked==1&&m_Ch[4].DC==0&&flag)
              ui->lineEdit_12->setText(QString::number(ChangeI1Frec,'g',4));
            if(m_Ch[5].checked==1&&m_Ch[5].DC==0&&flag)
              ui->lineEdit_15->setText(QString::number(ChangeI2Frec,'g',4));
            if(m_Ch[6].checked==1&&m_Ch[6].DC==0&&flag)
              ui->lineEdit_18->setText(QString::number(ChangeI3Frec,'g',4));
            break;
        }
     }
     else
     {
         switch(A_Variable)
           {
             case 1:
               if(m_Ch[0].checked==1)
                   ui->lineEdit->setText(QString::number(m_Ch[0].Amp,'g',4));
               if(m_Ch[1].checked==1)
                   ui->lineEdit_4->setText(QString::number(m_Ch[1].Amp,'g',4));
               if(m_Ch[2].checked==1)
                   ui->lineEdit_7->setText(QString::number(m_Ch[2].Amp,'g',4));
               //if(m_Ch[3].checked==1)
                   //ui->lineEdit_29->setText(QString::number(m_Ch[3].Amp,'g',4));

               if(m_Ch[4].checked==1)
                   ui->lineEdit_10->setText(QString::number(m_Ch[4].Amp,'g',4));
               if(m_Ch[5].checked==1)
                   ui->lineEdit_13->setText(QString::number(m_Ch[5].Amp,'g',4));
               if(m_Ch[6].checked==1)
                   ui->lineEdit_16->setText(QString::number(m_Ch[6].Amp,'g',4));
               break;

             case 2:

               V1RelaPhase=0;
               V2RelaPhase=0;
               V3RelaPhase=0;
               VzRelaPhase=0;
               I1RelaPhase=0;
               I2RelaPhase=0;
               I3RelaPhase=0;

               if(m_Ch[0].checked==1&&m_Ch[0].DC==0)
                 ui->lineEdit_2->setText(QString::number(m_Ch[0].Phase,'g',4));
               if(m_Ch[1].checked==1&&m_Ch[1].DC==0)
                ui->lineEdit_5->setText(QString::number(m_Ch[1].Phase,'g',4));
               if(m_Ch[2].checked==1&&m_Ch[2].DC==0)
                ui->lineEdit_8->setText(QString::number(m_Ch[2].Phase,'g',4));

               //if(m_Ch[3].checked==1&&m_Ch[3].DC==0)
                //ui->lineEdit_30->setText(QString::number(m_Ch[3].Phase,'g',4));

               if(m_Ch[4].checked==1&&m_Ch[4].DC==0)
                ui->lineEdit_11->setText(QString::number(m_Ch[4].Phase,'g',4));
               if(m_Ch[5].checked==1&&m_Ch[5].DC==0)
                ui->lineEdit_14->setText(QString::number(m_Ch[5].Phase,'g',4));
               if(m_Ch[6].checked==1&&m_Ch[6].DC==0)
                ui->lineEdit_17->setText(QString::number(m_Ch[6].Phase,'g',4));
             break;

             case 3:
               if(m_Ch[0].checked==1&&m_Ch[0].DC==0)
                 ui->lineEdit_3->setText(QString::number(m_Ch[0].Freq,'g',4));
               if(m_Ch[1].checked==1&&m_Ch[1].DC==0)
                   ui->lineEdit_6->setText(QString::number(m_Ch[1].Freq,'g',4));
               if(m_Ch[2].checked==1&&m_Ch[2].DC==0)
                   ui->lineEdit_9->setText(QString::number(m_Ch[2].Freq,'g',4));
               //if(m_Ch[3].checked==1&&m_Ch[3].DC==0)
                   //ui->lineEdit_31->setText(QString::number(m_Ch[3].Freq,'g',4));

               if(m_Ch[4].checked==1&&m_Ch[4].DC==0)
                   ui->lineEdit_12->setText(QString::number(m_Ch[4].Freq,'g',4));
               if(m_Ch[5].checked==1&&m_Ch[5].DC==0)
                   ui->lineEdit_15->setText(QString::number(m_Ch[5].Freq,'g',4));
               if(m_Ch[6].checked==1&&m_Ch[6].DC==0)
                   ui->lineEdit_18->setText(QString::number(m_Ch[6].Freq,'g',4));
               break;
           }

     }

 }

int Manualtest::vi_change_step(int add_or_dec,float devision)
  {
    if(ManulIsSendcmd==true)
        return 0;

    ManulIsSendcmd=true;
    float max_v_valuedata,min_v_valuedata;
    float max_i_valuedata,min_i_valuedata;
    int flag;
    if(m_Ch[0].DC==0||m_Ch[1].DC==0||m_Ch[2].DC==0||m_Ch[3].DC==0||m_Ch[4].DC==0||m_Ch[5].DC==0||m_Ch[6].DC==0)
      {//浜ゆ祦
        max_v_valuedata=MAX_V_VALUEDATA_AC;
        min_v_valuedata=0;
        max_i_valuedata=MAX_I_VALUEDATA_AC;
        min_i_valuedata=0;
      }
    else if(m_Ch[0].DC==1||m_Ch[1].DC==1||m_Ch[2].DC==1||m_Ch[3].DC==1||m_Ch[4].DC==1||m_Ch[5].DC==1||m_Ch[6].DC==1)
      {//鐩存祦
        max_v_valuedata=MAX_V_VALUEDATA_DC;
        min_v_valuedata=MIN_V_VALUEDATA_DC;
        max_i_valuedata=MAX_I_VALUEDATA_DC;
        min_i_valuedata=MIN_I_VALUEDATA_DC;
      }
    flag=0;
    if(add_or_dec==1)
      {
        switch(A_Variable)
          {
            case 1://幅值
              if(m_Ch[0].checked==1)//VA选择
                {
                  m_Ch[0].Amp=m_Ch[0].Amp+A_VolStep*devision;
                  switch(m_Ch[0].DC)
                  {
                    case 0:  //VA  交流
                      if(m_Ch[0].Amp>MAX_V_VALUEDATA_AC)
                      {
                        m_Ch[0].Amp=MAX_V_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(m_Ch[0].Amp>MAX_V_VALUEDATA_DC)
                      {
                        m_Ch[0].Amp=MAX_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(1);
                }
              if(m_Ch[1].checked==1)
                {
                  m_Ch[1].Amp=m_Ch[1].Amp+A_VolStep*devision;
                  switch(m_Ch[1].DC)
                  {
                    case 0:  //VA  交流
                      if(m_Ch[1].Amp>MAX_V_VALUEDATA_AC)
                      {
                        m_Ch[1].Amp=MAX_V_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(m_Ch[1].Amp>MAX_V_VALUEDATA_DC)
                      {
                        m_Ch[1].Amp=MAX_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                   displaydataandstate(4);
                }
              if(m_Ch[2].checked==1)
                {//V3瑕佸彉鍖?                  
                  m_Ch[2].Amp=m_Ch[2].Amp+A_VolStep*devision;
                  switch(m_Ch[2].DC)
                  {
                    case 0:  //VA  交流
                      if(m_Ch[2].Amp>MAX_V_VALUEDATA_AC)
                      {
                        m_Ch[2].Amp=MAX_V_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(m_Ch[2].Amp>MAX_V_VALUEDATA_DC)
                      {
                        m_Ch[2].Amp=MAX_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                   displaydataandstate(7); //V3鐨勫箙鍊?
                }

              if(m_Ch[3].checked==1)
                {//V3瑕佸彉鍖?
                  m_Ch[3].Amp=m_Ch[3].Amp+A_VolStep*devision;
                  switch(m_Ch[3].DC)
                  {
                    case 0:  //VA  交流
                      if(m_Ch[3].Amp>MAX_V_VALUEDATA_AC)
                      {
                        m_Ch[3].Amp=MAX_V_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(m_Ch[3].Amp>MAX_V_VALUEDATA_DC)
                      {
                        m_Ch[3].Amp=MAX_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                   displaydataandstate(29);
                }



              if(m_Ch[4].checked==1)
                {//I1瑕佸彉鍖?
                  m_Ch[4].Amp=m_Ch[4].Amp+A_CurStep*devision;
                  switch(m_Ch[4].DC)
                  {
                    case 0:  //IA  交流
                      if(m_Ch[4].Amp>MAX_I_VALUEDATA_AC)
                      {
                        m_Ch[4].Amp=MAX_I_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //IA  直流
                      if(m_Ch[4].Amp>MAX_I_VALUEDATA_DC)
                      {
                        m_Ch[4].Amp=MAX_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(10);  //I1鐨勫箙鍊?
                }
              if(m_Ch[5].checked==1)
                {//I2瑕佸彉鍖?
                  m_Ch[5].Amp=m_Ch[5].Amp+A_CurStep*devision;
                  switch(m_Ch[5].DC)
                  {
                    case 0:  //IA  交流
                      if(m_Ch[5].Amp>MAX_I_VALUEDATA_AC)
                      {
                        m_Ch[5].Amp=MAX_I_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //IA  直流
                      if(m_Ch[5].Amp>MAX_I_VALUEDATA_DC)
                      {
                        m_Ch[5].Amp=MAX_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                 displaydataandstate(13);  //I1鐨勫箙鍊?

                }
              if(m_Ch[6].checked==1)
                {//I3瑕佸彉鍖?
                  m_Ch[6].Amp=m_Ch[6].Amp+A_CurStep*devision;
                  switch(m_Ch[6].DC)
                  {
                    case 0:  //IA  交流
                      if(m_Ch[6].Amp>MAX_I_VALUEDATA_AC)
                      {
                        m_Ch[6].Amp=MAX_I_VALUEDATA_AC;
                        flag=1;
                      }
                      break;
                    case 1:  //IA  直流
                      if(m_Ch[6].Amp>MAX_I_VALUEDATA_DC)
                      {
                        m_Ch[6].Amp=MAX_I_VALUEDATA_DC;
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

              if(m_Ch[0].checked==1&&m_Ch[0].DC==0)
                {
                  m_Ch[0].Phase=m_Ch[0].Phase+A_PhaStep*devision;
                  if(m_Ch[0].Phase>180)
                    {
                      m_Ch[0].Phase=m_Ch[0].Phase-A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        V1RelaPhase=A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        V1RelaPhase=360+A_PhaStep*devision;
                  }
                  displaydataandstate(2);
                }
              else if(m_Ch[0].checked!=1&&m_Ch[0].DC==0)
                {//V1涓嶅彉鍖?
                  V1RelaPhase=0;
                }

              if(m_Ch[1].checked==1&&m_Ch[1].DC==0)
                {//V2瑕佸彉鍖?
                  m_Ch[1].Phase=m_Ch[1].Phase+A_PhaStep*devision;
                  if(m_Ch[1].Phase>180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[1].Phase=m_Ch[1].Phase-A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        V2RelaPhase=A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        V2RelaPhase=360+A_PhaStep*devision;
                  }
                  displaydataandstate(5);
                }

              else if(m_Ch[1].checked!=1&&m_Ch[1].DC==0)
                {//V2涓嶅彉鍖?
                  V2RelaPhase=0;
                }




              if(m_Ch[2].checked==1&&m_Ch[2].DC==0)
                {//V3瑕佸彉鍖?
                  m_Ch[2].Phase=m_Ch[2].Phase+A_PhaStep*devision;
                  if(m_Ch[2].Phase>180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[2].Phase=m_Ch[2].Phase-A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        V3RelaPhase=A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        V3RelaPhase=360+A_PhaStep*devision;
                  }
                 displaydataandstate(8);
                }
              else if(m_Ch[2].checked!=1&&m_Ch[2].DC==0)
                {
                  V3RelaPhase=0;
                }


              if(m_Ch[3].checked==1&&m_Ch[3].DC==0)
                {
                  m_Ch[3].Phase=m_Ch[3].Phase+A_PhaStep*devision;
                  if(m_Ch[3].Phase>180)
                    {
                      m_Ch[3].Phase=m_Ch[3].Phase-A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        VzRelaPhase=A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        VzRelaPhase=360+A_PhaStep*devision;
                  }
                 displaydataandstate(30);
                }
              else if(m_Ch[3].checked!=1&&m_Ch[3].DC==0)
                {
                  VzRelaPhase=0;
                }

              if(m_Ch[4].checked==1&&m_Ch[4].DC==0)
                {//I1瑕佸彉鍖?
                  m_Ch[4].Phase=m_Ch[4].Phase+A_PhaStep*devision;
                  if(m_Ch[4].Phase>180)
                    {
                      m_Ch[4].Phase=m_Ch[4].Phase-A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        I1RelaPhase=A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        I1RelaPhase=360+A_PhaStep*devision;
                  }
                  displaydataandstate(11);
                }
              else if(m_Ch[4].checked!=1&&m_Ch[4].DC==0)
                {
                  I1RelaPhase=0;
                }
              if(m_Ch[5].checked==1&&m_Ch[5].DC==0)
                {//I2瑕佸彉鍖?
                  m_Ch[5].Phase=m_Ch[5].Phase+A_PhaStep*devision;
                  if(m_Ch[5].Phase>180)
                    {
                      m_Ch[5].Phase=m_Ch[5].Phase-A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        I2RelaPhase=A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        I2RelaPhase=360+A_PhaStep*devision;
                  }

                  displaydataandstate(14);
                }
              else if(m_Ch[5].checked!=1&&m_Ch[5].DC==0)
                {//I2涓嶅彉鍖?
                  I2RelaPhase=0;
                }
              if(m_Ch[6].checked==1&&m_Ch[6].DC==0)
                {//I3瑕佸彉鍖?
                  m_Ch[6].Phase=m_Ch[6].Phase+A_PhaStep*devision;

                  if(m_Ch[6].Phase>180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[6].Phase=m_Ch[6].Phase-A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        I3RelaPhase=A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        I3RelaPhase=360+A_PhaStep*devision;
                  }
                  displaydataandstate(17);
                }
              else if(m_Ch[6].checked!=1&&m_Ch[6].DC==0)
                {
                  I3RelaPhase=0;
                }
              break;




            case 3:
              if(m_Ch[0].checked==1&&m_Ch[0].DC==0)
                {//V1瑕佸彉鍖?
                  m_Ch[0].Freq=m_Ch[0].Freq+A_FreStep*devision;
                  if(m_Ch[0].Freq>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[0].Freq=m_Ch[0].Freq-A_FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(3);
                }
              if(m_Ch[1].checked==1&&m_Ch[1].DC==0)
                {//V2瑕佸彉鍖?
                  m_Ch[1].Freq=m_Ch[1].Freq+A_FreStep*devision;
                  if(m_Ch[1].Freq>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[1].Freq=m_Ch[1].Freq-A_FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(6);
                }
              if(m_Ch[2].checked==1&&m_Ch[2].DC==0)
                {//V3瑕佸彉鍖?
                  m_Ch[2].Freq=m_Ch[2].Freq+A_FreStep*devision;
                  if(m_Ch[2].Freq>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[2].Freq=m_Ch[2].Freq-A_FreStep*devision;
                      flag=1;
                    }

                  displaydataandstate(9);
                }

              if(m_Ch[3].checked==1&&m_Ch[3].DC==0)
                {//V3瑕佸彉鍖?
                  m_Ch[3].Freq=m_Ch[3].Freq+A_FreStep*devision;
                  if(m_Ch[3].Freq>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[3].Freq=m_Ch[3].Freq-A_FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(31);
                }




              if(m_Ch[4].checked==1&&m_Ch[4].DC==0)
                {//I1瑕佸彉鍖?
                  m_Ch[4].Freq=m_Ch[4].Freq+A_FreStep*devision;
                  if(m_Ch[4].Freq>1000)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[4].Freq=m_Ch[4].Freq-A_FreStep*devision;
                      flag=1;
                    }

                  displaydataandstate(12);
                }
              if(m_Ch[5].checked==1&&m_Ch[5].DC==0)
                {//I2瑕佸彉鍖?
                  m_Ch[5].Freq=m_Ch[5].Freq+A_FreStep*devision;
                  if(m_Ch[5].Freq>1000)

                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[5].Freq=m_Ch[5].Freq-A_FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(15);
                }
              if(m_Ch[6].checked==1&&m_Ch[6].DC==0)
                {//I3瑕佸彉鍖?
                  m_Ch[6].Freq=m_Ch[6].Freq+A_FreStep*devision;
                  if(m_Ch[6].Freq>1000)

                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[6].Freq=m_Ch[6].Freq-A_FreStep*devision;

                      flag=1;
                    }
                   displaydataandstate(18);
                }

              break;
          }
      }

    else /*if(increase==0)*/
      {
        switch(A_Variable)
          {
            case 1://鍙橀噺鏄箙鍊?
              if(m_Ch[0].checked==1)
                {//V1瑕佸彉鍖?
                  m_Ch[0].Amp=m_Ch[0].Amp-A_VolStep*devision;
                  switch(m_Ch[0].DC)
                  {
                    case 0:  //VA  交流
                      if(m_Ch[0].Amp<0)
                      {
                        m_Ch[0].Amp=0;
                        flag=1;
                      }
                      break;
                    case 1:  //VA  直流
                      if(m_Ch[0].Amp<MIN_V_VALUEDATA_DC)
                      {
                        m_Ch[0].Amp=MIN_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(1);
                }
              if(m_Ch[1].checked==1)
                {//V2瑕佸彉鍖?
                  m_Ch[1].Amp=m_Ch[1].Amp-A_VolStep*devision;
                  switch(m_Ch[1].DC)
                  {
                    case 0:  //VB  交流
                      if(m_Ch[1].Amp<0)
                      {
                        m_Ch[1].Amp=0;
                        flag=1;
                      }
                      break;
                    case 1:  //VB  直流
                      if(m_Ch[1].Amp<MIN_V_VALUEDATA_DC)
                      {
                        m_Ch[1].Amp=MIN_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(4);
                }
              if(m_Ch[2].checked==1)
                {//V3瑕佸彉鍖?
                  m_Ch[2].Amp=m_Ch[2].Amp-A_VolStep*devision;
                  switch(m_Ch[2].DC)
                  {
                    case 0:  //VC  交流
                      if(m_Ch[2].Amp<0)
                      {
                        m_Ch[2].Amp=0;
                        flag=1;
                      }
                      break;
                    case 1:  //VC  直流
                      if(m_Ch[2].Amp<MIN_V_VALUEDATA_DC)
                      {
                        m_Ch[2].Amp=MIN_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                 displaydataandstate(7);
                }

              if(m_Ch[3].checked==1)
                {//V3瑕佸彉鍖?
                  m_Ch[3].Amp=m_Ch[3].Amp-A_VolStep*devision;
                  switch(m_Ch[3].DC)
                  {
                    case 0:  //VZ  交流
                      if(m_Ch[3].Amp<0)
                      {
                        m_Ch[3].Amp=0;
                        flag=1;
                      }
                      break;
                    case 1:  //VZ  直流
                      if(m_Ch[3].Amp<MIN_V_VALUEDATA_DC)
                      {
                        m_Ch[3].Amp=MIN_V_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                 displaydataandstate(29);
                }


              if(m_Ch[4].checked==1)
                {//I1瑕佸彉鍖?
                  m_Ch[4].Amp=m_Ch[4].Amp-A_CurStep*devision;
                  switch(m_Ch[4].DC)
                  {
                    case 0:  //IA  交流
                      if(m_Ch[4].Amp<0)
                      {
                        m_Ch[4].Amp=0;
                        flag=1;
                      }
                      break;
                    case 1:  //IA  直流
                      if(m_Ch[4].Amp<MIN_I_VALUEDATA_DC)
                      {
                        m_Ch[4].Amp=MIN_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                     displaydataandstate(10);
                }
              if(m_Ch[5].checked==1)
                {//I2瑕佸彉鍖?
                  m_Ch[5].Amp=m_Ch[5].Amp-A_CurStep*devision;
                  switch(m_Ch[5].DC)
                  {
                    case 0:  //IB  交流
                      if(m_Ch[5].Amp<0)
                      {
                        m_Ch[5].Amp=0;
                        flag=1;
                      }
                      break;
                    case 1:  //IB  直流
                      if(m_Ch[5].Amp<MIN_I_VALUEDATA_DC)
                      {
                        m_Ch[5].Amp=MIN_I_VALUEDATA_DC;
                        flag=1;
                      }
                      break;
                  }
                  displaydataandstate(13);
                }
              if(m_Ch[6].checked==1)
                {//I3瑕佸彉鍖?
                  m_Ch[6].Amp=m_Ch[6].Amp-A_CurStep*devision;
                  switch(m_Ch[6].DC)
                  {
                    case 0:  //IC  交流
                      if(m_Ch[6].Amp<0)
                      {
                        m_Ch[6].Amp=0;
                        flag=1;
                      }
                      break;
                    case 1:  //IC  直流
                      if(m_Ch[6].Amp<MIN_I_VALUEDATA_DC)
                      {
                        m_Ch[6].Amp=MIN_I_VALUEDATA_DC;
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



              if(m_Ch[0].checked==1&&m_Ch[0].DC==0)
                {//V1瑕佸彉鍖?
                  m_Ch[0].Phase=m_Ch[0].Phase-A_PhaStep*devision;
                  if(m_Ch[0].Phase<-180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[0].Phase=m_Ch[0].Phase+A_PhaStep*devision;
                      flag=1;
                    }
                   else
                  {
                      if(A_PhaStep>=0)
                        V1RelaPhase=360-A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        V1RelaPhase=0-A_PhaStep*devision;
                  }
                    displaydataandstate(2);
                }
              else if(m_Ch[0].checked!=1&&m_Ch[0].DC==0)
                {//V1涓嶅彉鍖?
                  V1RelaPhase=0;
                }
              if(m_Ch[1].checked==1&&m_Ch[1].DC==0)
                {//V2瑕佸彉鍖?
                  m_Ch[1].Phase=m_Ch[1].Phase-A_PhaStep*devision;
                  if(m_Ch[1].Phase<-180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[1].Phase=m_Ch[1].Phase+A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        V2RelaPhase=360-A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        V2RelaPhase=0-A_PhaStep*devision;
                  }
                   displaydataandstate(5);
                }
              else if(m_Ch[1].checked!=1&&m_Ch[1].DC==0)
                {//V2涓嶅彉鍖?
                  V2RelaPhase=0;
                }

              if(m_Ch[2].checked==1&&m_Ch[2].DC==0)
                {//V3瑕佸彉鍖?
                  m_Ch[2].Phase=m_Ch[2].Phase-A_PhaStep*devision;

                  if(m_Ch[2].Phase<-180)
                    {
                      m_Ch[2].Phase=m_Ch[2].Phase+A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        V3RelaPhase=360-A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        V3RelaPhase=0-A_PhaStep*devision;
                  }
                  displaydataandstate(8);
                }
              else if(m_Ch[2].checked!=1&&m_Ch[2].DC==0)
                {//V3涓嶅彉鍖?
                  V3RelaPhase=0;
                }

              if(m_Ch[3].checked==1&&m_Ch[3].DC==0)
                {//V3瑕佸彉鍖?
                  m_Ch[3].Phase=m_Ch[3].Phase-A_PhaStep*devision;
                  if(m_Ch[3].Phase<-180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[3].Phase=m_Ch[3].Phase+A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        VzRelaPhase=360-A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        VzRelaPhase=0-A_PhaStep*devision;
                  }
                  displaydataandstate(30);
                }
              else if(m_Ch[3].checked!=1&&m_Ch[3].DC==0)
                {//V3涓嶅彉鍖?
                  VzRelaPhase=0;
                }

              if(m_Ch[4].checked==1&&m_Ch[4].DC==0)
                {//I1瑕佸彉鍖?
                  m_Ch[4].Phase=m_Ch[4].Phase-A_PhaStep*devision;
                  if(m_Ch[4].Phase<-180)
                    {
                      m_Ch[4].Phase=m_Ch[4].Phase+A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        I1RelaPhase=360-A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        I1RelaPhase=0-A_PhaStep*devision;
                  }
                 displaydataandstate(11);
                }
              else if(m_Ch[4].checked!=1&&m_Ch[4].DC==0)
                {//I1涓嶅彉鍖?
                  I1RelaPhase=0;
                }
              if(m_Ch[5].checked==1&&m_Ch[5].DC==0)
                {//I2瑕佸彉鍖?
                  m_Ch[5].Phase=m_Ch[5].Phase-A_PhaStep*devision;
                  if(m_Ch[5].Phase<-180)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[5].Phase=m_Ch[5].Phase+A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        I2RelaPhase=360-A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        I2RelaPhase=0-A_PhaStep*devision;
                  }
                  displaydataandstate(14);
                }

              else if(m_Ch[5].checked!=1&&m_Ch[5].DC==0)
                {//I1涓嶅彉鍖?
                  I2RelaPhase=0;
                }
              if(m_Ch[6].checked==1&&m_Ch[6].DC==0)
                {//I3瑕佸彉鍖?
                  m_Ch[6].Phase=m_Ch[6].Phase-A_PhaStep*devision;
                  if(m_Ch[6].Phase<-180)
                    {
                      m_Ch[6].Phase=m_Ch[6].Phase+A_PhaStep*devision;
                      flag=1;
                    }
                  else
                  {
                      if(A_PhaStep>=0)
                        I3RelaPhase=360-A_PhaStep*devision;
                      else if(A_PhaStep<0)
                        I3RelaPhase=0-A_PhaStep*devision;
                  }
                   displaydataandstate(17);
                }
              else if(m_Ch[6].checked!=1&&m_Ch[6].DC==0)
                {//I3涓嶅彉鍖?
                  I3RelaPhase=0;
                }
              break;


            case 3:
              if(m_Ch[0].checked==1&&m_Ch[0].DC==0)
                {//V1瑕佸彉鍖?
                  m_Ch[0].Freq=m_Ch[0].Freq-A_FreStep*devision;
                  if(m_Ch[0].Freq<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[0].Freq=m_Ch[0].Freq+A_FreStep*devision;
                      flag=1;
                    }

                   displaydataandstate(3);
                }
              if(m_Ch[1].checked==1&&m_Ch[1].DC==0)
                {//V2瑕佸彉鍖?
                  m_Ch[1].Freq=m_Ch[1].Freq-A_FreStep*devision;
                  if(m_Ch[1].Freq<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[1].Freq=m_Ch[1].Freq+A_FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(6);
                }
              if(m_Ch[2].checked==1&&m_Ch[2].DC==0)
                {//V3瑕佸彉鍖?
                  m_Ch[2].Freq=m_Ch[2].Freq-A_FreStep*devision;
                  if(m_Ch[2].Freq<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[2].Freq=m_Ch[2].Freq+A_FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(9);
                }


              if(m_Ch[3].checked==1&&m_Ch[3].DC==0)
                {//V3瑕佸彉鍖?
                  m_Ch[3].Freq=m_Ch[3].Freq-A_FreStep*devision;


                  if(m_Ch[3].Freq<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[3].Freq=m_Ch[3].Freq+A_FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(31);
                }




              if(m_Ch[4].checked==1&&m_Ch[4].DC==0)
                {//I1瑕佸彉鍖?
                  m_Ch[4].Freq=m_Ch[4].Freq-A_FreStep*devision;
                  if(m_Ch[4].Freq<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[4].Freq=m_Ch[4].Freq+A_FreStep*devision;
                      flag=1;
                    }
                   displaydataandstate(12);
                }
              if(m_Ch[5].checked==1&&m_Ch[5].DC==0)
                {//I2瑕佸彉鍖?
                  m_Ch[5].Freq=m_Ch[5].Freq-A_FreStep*devision;
                  if(m_Ch[5].Freq<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[5].Freq=m_Ch[5].Freq+A_FreStep*devision;
                      flag=1;
                    }
                  displaydataandstate(15);
                }
              if(m_Ch[6].checked==1&&m_Ch[6].DC==0)
                {//I3瑕佸彉鍖?
                  m_Ch[6].Freq=m_Ch[6].Freq-A_FreStep*devision;
                  if(m_Ch[6].Freq<0)
                    {//濡傛灉澧炲姞鍒版渶澶у€煎垯涓嶅彉
                      m_Ch[6].Freq=m_Ch[6].Freq+A_FreStep*devision;
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


     if(A_TestMode==1)
     {
         strcpy(strbuf,"手动模式:");
         fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"动作值:");
       fprintf(fp,"%s\r\n",strbuf);
       switch(A_Variable)
       {
       case 1:  //幅值
           if(m_Ch[0].checked==1)
           {
            QString str;
            QByteArray ba;
            char *buf;
            str=QString(tr("VA:")+QString::number(m_Ch[0].Amp,'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);

           }
           if(m_Ch[1].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VB:")+QString::number(m_Ch[1].Amp,'g',6)+tr(" V"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);

           }
           if(m_Ch[2].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(m_Ch[2].Amp,'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(m_Ch[3].checked==1)
           {
             QString str;
             QByteArray ba;
             char *buf;
            str=QString(tr("Vz:")+QString::number(m_Ch[3].Amp,'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }





           if(m_Ch[4].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("IA:")+QString::number(m_Ch[4].Amp,'g',6)+tr(" A"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[5].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("IB:")+QString::number(m_Ch[5].Amp,'g',6)+tr(" A"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[6].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IC:")+QString::number(m_Ch[6].Amp,'g',6)+tr(" A"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           break;
       case 2: // 相位
           if(m_Ch[0].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VA:")+QString::number(m_Ch[0].Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[1].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VB:")+QString::number(m_Ch[1].Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[2].checked==1)
           {   QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(m_Ch[2].Phase,'g',6)+tr(" Deg"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[3].checked==1)
           {   QString str;
               QByteArray ba;
               char *buf;
              str=QString(tr("Vz:")+QString::number(m_Ch[3].Phase,'g',6)+tr(" Deg"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }





           if(m_Ch[4].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IA:")+QString::number(m_Ch[4].Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);

           }
           if(m_Ch[5].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IB:")+QString::number(m_Ch[5].Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[6].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
                str=QString(tr("IC:")+QString::number(m_Ch[6].Phase,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           break;


       case 3: //频率
           if(m_Ch[0].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VA:")+QString::number(m_Ch[0].Freq,'g',6)+tr(" Hz"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[1].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VB:")+QString::number(m_Ch[1].Freq,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[2].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(m_Ch[2].Freq,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(m_Ch[3].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("Vz:")+QString::number(m_Ch[3].Freq,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(m_Ch[4].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IA:")+QString::number(m_Ch[4].Freq,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[5].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IB:")+QString::number(m_Ch[5].Freq,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[6].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IC:")+QString::number(m_Ch[6].Freq,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           break;
       }

     }

     else if(A_TestMode==2)
    {
         strcpy(strbuf,"触发模式:");
         fprintf(fp,"%s\r\n",strbuf);
       if(TripTime==-1)
       strcpy(strbuf,"动作时间(s):未动作");
       else
       sprintf(strbuf,"动作时间(s):%.3f",TripTime);
       fprintf(fp,"%s\r\n",strbuf);
    }

     else if(A_TestMode==3)
     {
         strcpy(strbuf,"自动模式:");
         fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"动作值:");
       fprintf(fp,"%s\r\n",strbuf);
       switch(A_Variable)
       {
       case 1:  //幅值
           if(m_Ch[0].checked==1)
           {
            QString str;
            QByteArray ba;
            char *buf;
            str=QString(tr("VA:")+QString::number(m_Ch[0].Amp+A_SumStepnum*(A_VolStep/*-A_V1ampPerStepError*/),'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);

           }
           if(m_Ch[1].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VB:")+QString::number(m_Ch[1].Amp+A_SumStepnum*(A_VolStep/*-A_V2ampPerStepError*/),'g',6)+tr(" V"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);

           }
           if(m_Ch[2].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(m_Ch[2].Amp+A_SumStepnum*(A_VolStep/*-A_V3ampPerStepError*/),'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(VariatyVz2==0&&m_Ch[3].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("Vz:")+QString::number(m_Ch[3].Amp+A_SumStepnum*(A_VolStep/*-A_VzampPerStepError*/),'g',6)+tr(" V"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }





           if(m_Ch[4].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("IA:")+QString::number(m_Ch[4].Amp+A_SumStepnum*(A_CurStep/*-A_I1ampPerStepError*/),'g',6)+tr(" A"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[5].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("IB:")+QString::number(m_Ch[5].Amp+A_SumStepnum*(A_CurStep/*-A_I2ampPerStepError*/),'g',6)+tr(" A"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[6].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IC:")+QString::number(m_Ch[6].Amp+A_SumStepnum*(A_CurStep/*-A_I3ampPerStepError*/),'g',6)+tr(" A"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           break;
       case 2: // 相位
           if(m_Ch[0].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VA:")+QString::number(m_Ch[0].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[1].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VB:")+QString::number(m_Ch[1].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[2].checked==1)
           {   QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(m_Ch[2].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }

           if(VariatyVz2==0&&m_Ch[3].checked==1)
           {   QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("Vz:")+QString::number(m_Ch[3].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }




           if(m_Ch[4].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IA:")+QString::number(m_Ch[4].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);

           }
           if(m_Ch[5].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IB:")+QString::number(m_Ch[5].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[6].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
                str=QString(tr("IC:")+QString::number(m_Ch[6].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           break;


       case 3: //频率
           if(m_Ch[0].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
               str=QString(tr("VA:")+QString::number(m_Ch[0].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
               ba=str.toLatin1();
               buf=ba.data();
               fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[1].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VB:")+QString::number(m_Ch[1].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[2].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("VC:")+QString::number(m_Ch[2].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }

           if(VariatyVz2==0&&m_Ch[3].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("Vz:")+QString::number(m_Ch[3].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }



           if(m_Ch[4].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IA:")+QString::number(m_Ch[4].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[5].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IB:")+QString::number(m_Ch[5].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
            ba=str.toLatin1();
            buf=ba.data();
            fprintf(fp,"%s\r\n",buf);
           }
           if(m_Ch[6].checked==1)
           {
               QString str;
               QByteArray ba;
               char *buf;
            str=QString(tr("IC:")+QString::number(m_Ch[6].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
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


           if(A_TestMode==1)
           {
               strcpy(strbuf,"Manual:");
               fprintf(fp,"%s\r\n",strbuf);

             strcpy(strbuf,"Trip value:");
             fprintf(fp,"%s\r\n",strbuf);
             switch(A_Variable)
             {
             case 1:  //幅值
                 if(m_Ch[0].checked==1)
                 {
                  QString str;
                  QByteArray ba;
                  char *buf;
                  str=QString(tr("VA:")+QString::number(m_Ch[0].Amp,'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);

                 }
                 if(m_Ch[1].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VB:")+QString::number(m_Ch[1].Amp,'g',6)+tr(" V"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);

                 }
                 if(m_Ch[2].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(m_Ch[2].Amp,'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(m_Ch[3].checked==1)
                 {
                   QString str;
                   QByteArray ba;
                   char *buf;
                  str=QString(tr("Vz:")+QString::number(m_Ch[3].Amp,'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }





                 if(m_Ch[4].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("IA:")+QString::number(m_Ch[4].Amp,'g',6)+tr(" A"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[5].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("IB:")+QString::number(m_Ch[5].Amp,'g',6)+tr(" A"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[6].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IC:")+QString::number(m_Ch[6].Amp,'g',6)+tr(" A"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 break;
             case 2: // 相位
                 if(m_Ch[0].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VA:")+QString::number(m_Ch[0].Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[1].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VB:")+QString::number(m_Ch[1].Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[2].checked==1)
                 {   QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(m_Ch[2].Phase,'g',6)+tr(" Deg"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[3].checked==1)
                 {   QString str;
                     QByteArray ba;
                     char *buf;
                    str=QString(tr("Vz:")+QString::number(m_Ch[3].Phase,'g',6)+tr(" Deg"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }





                 if(m_Ch[4].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IA:")+QString::number(m_Ch[4].Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);

                 }
                 if(m_Ch[5].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IB:")+QString::number(m_Ch[5].Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[6].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                      str=QString(tr("IC:")+QString::number(m_Ch[6].Phase,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 break;


             case 3: //频率
                 if(m_Ch[0].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VA:")+QString::number(m_Ch[0].Freq,'g',6)+tr(" Hz"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[1].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VB:")+QString::number(m_Ch[1].Freq,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[2].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(m_Ch[2].Freq,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(m_Ch[3].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("Vz:")+QString::number(m_Ch[3].Freq,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(m_Ch[4].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IA:")+QString::number(m_Ch[4].Freq,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[5].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IB:")+QString::number(m_Ch[5].Freq,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[6].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IC:")+QString::number(m_Ch[6].Freq,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 break;
             }

           }

           else if(A_TestMode==2)
          {
               strcpy(strbuf,"Shot:");
               fprintf(fp,"%s\r\n",strbuf);
             if(TripTime==-1)
             strcpy(strbuf,"Trip time(s):no trip");
             else
             sprintf(strbuf,"Trip time(s):%.3f",TripTime);
             fprintf(fp,"%s\r\n",strbuf);
          }

           else if(A_TestMode==3)
           {
               strcpy(strbuf,"Auto:");
               fprintf(fp,"%s\r\n",strbuf);

             strcpy(strbuf,"Trip value:");
             fprintf(fp,"%s\r\n",strbuf);
             switch(A_Variable)
             {
             case 1:  //幅值
                 if(m_Ch[0].checked==1)
                 {
                  QString str;
                  QByteArray ba;
                  char *buf;
                  str=QString(tr("VA:")+QString::number(m_Ch[0].Amp+A_SumStepnum*(A_VolStep/*-A_V1ampPerStepError*/),'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);

                 }
                 if(m_Ch[1].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VB:")+QString::number(m_Ch[1].Amp+A_SumStepnum*(A_VolStep/*-A_V2ampPerStepError*/),'g',6)+tr(" V"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);

                 }
                 if(m_Ch[2].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(m_Ch[2].Amp+A_SumStepnum*(A_VolStep/*-A_V3ampPerStepError*/),'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(VariatyVz2==0&&m_Ch[3].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("Vz:")+QString::number(m_Ch[3].Amp+A_SumStepnum*(A_VolStep/*-A_VzampPerStepError*/),'g',6)+tr(" V"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }





                 if(m_Ch[4].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("IA:")+QString::number(m_Ch[4].Amp+A_SumStepnum*(A_CurStep/*-A_I1ampPerStepError*/),'g',6)+tr(" A"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[5].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("IB:")+QString::number(m_Ch[5].Amp+A_SumStepnum*(A_CurStep/*-A_I2ampPerStepError*/),'g',6)+tr(" A"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[6].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IC:")+QString::number(m_Ch[6].Amp+A_SumStepnum*(A_CurStep/*-A_I3ampPerStepError*/),'g',6)+tr(" A"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 break;
             case 2: // 相位
                 if(m_Ch[0].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VA:")+QString::number(m_Ch[0].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[1].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VB:")+QString::number(m_Ch[1].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[2].checked==1)
                 {   QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(m_Ch[2].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }

                 if(VariatyVz2==0&&m_Ch[3].checked==1)
                 {   QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("Vz:")+QString::number(m_Ch[3].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }




                 if(m_Ch[4].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IA:")+QString::number(m_Ch[4].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);

                 }
                 if(m_Ch[5].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IB:")+QString::number(m_Ch[5].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[6].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                      str=QString(tr("IC:")+QString::number(m_Ch[6].Phase+A_SumStepnum*A_PhaStep,'g',6)+tr(" Deg"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 break;


             case 3: //频率
                 if(m_Ch[0].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                     str=QString(tr("VA:")+QString::number(m_Ch[0].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                     ba=str.toLatin1();
                     buf=ba.data();
                     fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[1].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VB:")+QString::number(m_Ch[1].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[2].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("VC:")+QString::number(m_Ch[2].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }

                 if(VariatyVz2==0&&m_Ch[3].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("Vz:")+QString::number(m_Ch[3].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }



                 if(m_Ch[4].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IA:")+QString::number(m_Ch[4].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[5].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IB:")+QString::number(m_Ch[5].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
                  ba=str.toLatin1();
                  buf=ba.data();
                  fprintf(fp,"%s\r\n",buf);
                 }
                 if(m_Ch[6].checked==1)
                 {
                     QString str;
                     QByteArray ba;
                     char *buf;
                  str=QString(tr("IC:")+QString::number(m_Ch[6].Freq+A_SumStepnum*A_FreStep,'g',6)+tr(" Hz"));
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

    if(m_Ch[0].DC==0||m_Ch[1].DC==0||m_Ch[2].DC==0||m_Ch[3].DC==0||m_Ch[4].DC==0||m_Ch[5].DC==0||m_Ch[6].DC==0)
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
    else if(m_Ch[0].DC==1||m_Ch[1].DC==1||m_Ch[2].DC==1||m_Ch[3].DC==1||m_Ch[4].DC==1||m_Ch[5].DC==1||m_Ch[6].DC==1)
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
    if(A_Variable==1)//幅值
    {
       if(type==1)//超过
       {
          if(Outtype==1)
          {
           switch(m_Ch[0].DC)
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
           switch(m_Ch[1].DC)
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
           switch(m_Ch[2].DC)
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
           switch(m_Ch[3].DC)
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
           switch(m_Ch[4].DC)
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
           switch(m_Ch[5].DC)
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
           switch(m_Ch[6].DC)
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
            switch(m_Ch[0].DC)
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
            switch(m_Ch[1].DC)
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
            switch(m_Ch[2].DC)
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
            switch(m_Ch[3].DC)
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
            switch(m_Ch[4].DC)
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
            switch(m_Ch[5].DC)
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
            switch(m_Ch[6].DC)
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
    else  if(A_Variable==3)//频率
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
    if(A_TestMode==1||A_TestMode==3)
    {
        if(A_Variable==2) //
        {
        m_Ch[0].checked=0;
        ui->checkBox->setChecked(false);
        return;
        }
    }

    if(m_Ch[0].checked==1)
       { m_Ch[0].checked=0;

        ui->checkBox->setChecked(false);
       }
    else {
        m_Ch[0].checked=1;
        ui->checkBox->setChecked(true);
    }
}

void Manualtest::on_checkBox_2_clicked()
{
    if(m_Ch[1].checked==1)
        m_Ch[1].checked=0;
    else m_Ch[1].checked=1;
}

void Manualtest::on_checkBox_3_clicked()
{
    if(m_Ch[2].checked==1)
        m_Ch[2].checked=0;
    else m_Ch[2].checked=1;
}

void Manualtest::on_checkBox_4_clicked()
{
    if(m_Ch[4].checked==1)
        m_Ch[4].checked=0;
    else m_Ch[4].checked=1;
}

void Manualtest::on_checkBox_5_clicked()
{
    if(m_Ch[5].checked==1)
        m_Ch[5].checked=0;
    else m_Ch[5].checked=1;

}

void Manualtest::on_checkBox_6_clicked()
{
    if(m_Ch[6].checked==1)
        m_Ch[6].checked=0;
    else m_Ch[6].checked=1;
}


void Manualtest::on_radioButton_clicked()
{
   A_TestMode=1;
   ui->groupBox_5->setEnabled(true);
   ui->groupBox_7->setEnabled(true);
   ui->groupBox_3->setEnabled(true);
   ui->lineEdit_28->setEnabled(false);
   ui->groupBox_4->setEnabled(false);
   ui->label_4->setEnabled(false);
   ui->checkBox_8->setEnabled(true);
   //ui->checkBox_9->setHidden(false);
   ui->groupBox->setEnabled(true);

   ui->lineEdit_23->setEnabled(false);
   ui->lineEdit_24->setEnabled(false);
   ui->label_20->setEnabled(false);
   ui->label_21->setEnabled(false);

   if(A_CurStep<0)
   {
       A_CurStep=0;
       ui->lineEdit_19->setText(QString::number(A_CurStep,'g',4));
   }
   if(A_VolStep<0)
   {
       A_VolStep=0;
       ui->lineEdit_20->setText(QString::number(A_VolStep,'g',4));
   }
   if(A_PhaStep<0)
   {
       A_PhaStep=0;
       ui->lineEdit_21->setText(QString::number(A_PhaStep,'g',4));
   }
   if(A_FreStep<0)
   {
       A_FreStep=0;
       ui->lineEdit_22->setText(QString::number(A_FreStep,'g',4));
   }
}

void Manualtest::on_radioButton_2_clicked()
{
  A_TestMode=2;
   ui->groupBox_5->setEnabled(false);
   ui->groupBox_7->setEnabled(false);
   ui->groupBox_3->setEnabled(false);
   ui->lineEdit_28->setEnabled(false);
   ui->groupBox_4->setEnabled(true);
   ui->label_4->setEnabled(false);
   ui->groupBox->setEnabled(false);

   ui->checkBox_8->setEnabled(true);
   //ui->checkBox_9->setHidden(false);

   ui->lineEdit_23->setEnabled(false);
   ui->lineEdit_24->setEnabled(false);
   ui->label_20->setEnabled(false);
   ui->label_21->setEnabled(false);

}

void Manualtest::on_radioButton_3_clicked()
{
   A_TestMode=3;
   ui->groupBox_5->setEnabled(true);
   ui->groupBox_7->setEnabled(false);
   ui->groupBox_3->setEnabled(true);
   ui->lineEdit_28->setEnabled(true);
   ui->groupBox_4->setEnabled(false);
   ui->groupBox->setEnabled(true);
   ui->label_4->setEnabled(true);
   ui->checkBox_8->setEnabled(false);
   //ui->checkBox_9->setChecked(false);
   //ui->checkBox_9->setHidden(true);

   ui->lineEdit_23->setEnabled(true);
   ui->lineEdit_24->setEnabled(true);
   ui->label_20->setEnabled(true);
   ui->label_21->setEnabled(true);
}

void Manualtest::on_radioButton_4_clicked()
{
  A_Variable=1;


}

void Manualtest::on_radioButton_5_clicked()
{
  A_Variable=2;


}

void Manualtest::on_radioButton_6_clicked()
{
  A_Variable=3;

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
        m_Ch[0].Amp=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[0].Amp,'g',6));
        break;
    case 2:
        m_Ch[0].Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[0].Phase,'g',6));
        ui->lineEdit_2->setText(QString::number(m_Ch[0].Phase,'g',6));
        ui->lineEdit_5->setText(QString::number(m_Ch[1].Phase,'g',6));
        ui->lineEdit_8->setText(QString::number(m_Ch[2].Phase,'g',6));
        VolPhaseChange=1;
        break;
    case 3:
        m_Ch[0].Freq=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[0].Freq,'g',6));
        break;

    case 4:
        m_Ch[1].Amp=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[1].Amp,'g',6));
        break;
    case 5:
        m_Ch[1].Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[1].Phase,'g',6));
        ui->lineEdit_2->setText(QString::number(m_Ch[0].Phase,'g',6));
        ui->lineEdit_5->setText(QString::number(m_Ch[1].Phase,'g',6));
        ui->lineEdit_8->setText(QString::number(m_Ch[2].Phase,'g',6));
         VolPhaseChange=2;
        break;
    case 6:
        m_Ch[1].Freq=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[1].Freq,'g',6));
        break;


    case 7:
        m_Ch[2].Amp=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[2].Amp,'g',6));
        break;
    case 8:
        m_Ch[2].Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[2].Phase,'g',6));
        ui->lineEdit_2->setText(QString::number(m_Ch[0].Phase,'g',6));
        ui->lineEdit_5->setText(QString::number(m_Ch[1].Phase,'g',6));
        ui->lineEdit_8->setText(QString::number(m_Ch[2].Phase,'g',6));
        VolPhaseChange=3;
        break;
    case 9:
        m_Ch[2].Freq=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[2].Freq,'g',6));
        break;




    case 10:
        m_Ch[4].Amp=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[4].Amp,'g',6));
        break;
    case 11:
        m_Ch[4].Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[4].Phase,'g',6));

        ui->lineEdit_11->setText(QString::number(m_Ch[4].Phase,'g',6));
        ui->lineEdit_14->setText(QString::number(m_Ch[5].Phase,'g',6));
        ui->lineEdit_17->setText(QString::number(m_Ch[6].Phase,'g',6));
        CurPhaseChange=1;
        break;
    case 12:
        m_Ch[4].Freq=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[4].Freq,'g',6));
        break;



    case 13:
        m_Ch[5].Amp=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[5].Amp,'g',6));
        break;
    case 14:
        m_Ch[5].Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[5].Phase,'g',6));
        ui->lineEdit_11->setText(QString::number(m_Ch[4].Phase,'g',6));
        ui->lineEdit_14->setText(QString::number(m_Ch[5].Phase,'g',6));
        ui->lineEdit_17->setText(QString::number(m_Ch[6].Phase,'g',6));
        CurPhaseChange=2;
        break;
    case 15:
        m_Ch[5].Freq=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[5].Freq,'g',6));
        break;



    case 16:
        m_Ch[6].Amp=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[6].Amp,'g',6));
        break;
    case 17:
        m_Ch[6].Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[6].Phase,'g',6));
        ui->lineEdit_11->setText(QString::number(m_Ch[4].Phase,'g',6));
        ui->lineEdit_14->setText(QString::number(m_Ch[5].Phase,'g',6));
        ui->lineEdit_17->setText(QString::number(m_Ch[6].Phase,'g',6));
        CurPhaseChange=3;
        break;
    case 18:
        m_Ch[6].Freq=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[6].Freq,'g',6));
        break;

    case 19:
        A_CurStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(A_CurStep,'g',6));
        break;
    case 20:
        A_VolStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(A_VolStep,'g',6));
        break;
    case 21:
        A_PhaStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(A_PhaStep,'g',6));
        break;
    case 22:
        A_FreStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(A_FreStep,'g',6));
        break;
    case 23:
        A_StepTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(A_StepTime,'g',6));
        break;
    case 24:
        A_StepNum=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number( A_StepNum,'g',6));
        break;
    case 25:
        A_PreFaultTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number( A_PreFaultTime,'g',6));
        break;
    case 26:
        A_FaultTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(A_FaultTime,'g',6));
        break;
    case 27:
        A_PostFaultTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(A_PostFaultTime,'g',6));
        break;
    case 28:
        faultduration=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(faultduration,'g',6));
        break;
    case 29:
        if(VariatyVz2==0)
       { m_Ch[3].Amp=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[3].Amp,'g',6));
       }
        break;
    case 30:
         if(VariatyVz2==0)
       { m_Ch[3].Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[3].Phase,'g',6));
       }
        break;
    case 31:
        if(VariatyVz2==0)
       { m_Ch[3].Freq=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(m_Ch[3].Freq,'g',6));
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
    if(starttest==1&&A_TestMode==1)
     vi_change_step(1,1);
    updateVz();
}

void Manualtest::on_pushButton_5_clicked()
{
 if(starttest==1&&A_TestMode==1)
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
if(current_language==2)
helpdialog->SetHelpfilename("Manual.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("Manual1.txt");
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

//2017-7-6触摸屏按压弹出keypads
//    if(event->type()==QEvent::MouseButtonPress)
//    {

//        /*
//        QMouseEvent *mouseevent=static_cast<QMouseEvent *>(event);
//        if(mouseevent->button()==Qt::RightButton)
//        {

//            return true;
//        }
//        else
//        return false;
//        */
//        return QWidget::eventFilter(obj,event);

//    }

    //else if(event->type()==QEvent::MouseButtonDblClick)
    else if(event->type()==QEvent::MouseButtonPress)
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
              m_Ch[0].Amp= text.toFloat();
              currenteditpos=1;
              vi_parameter_enter(currenteditpos);
              ui->lineEdit->setText(QString::number(m_Ch[0].Amp,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               m_Ch[0].Phase= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(m_Ch[0].Phase,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                m_Ch[0].Freq= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(m_Ch[0].Freq,'g',6));
         }
        else if(obj==ui->lineEdit_4)
         {
                QString text=ui->lineEdit_4->text();
                m_Ch[1].Amp= text.toFloat();
                currenteditpos=4;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_4->setText(QString::number(m_Ch[1].Amp,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                m_Ch[1].Phase= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(m_Ch[1].Phase,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                m_Ch[1].Freq= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(m_Ch[1].Freq,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                m_Ch[2].Amp= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(m_Ch[2].Amp,'g',6));
         }
        else if(obj==ui->lineEdit_8)
         {
                QString text=ui->lineEdit_8->text();
                m_Ch[2].Phase= text.toFloat();
                currenteditpos=8;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_8->setText(QString::number(m_Ch[2].Phase,'g',6));
         }
        else if(obj==ui->lineEdit_9)
         {
                QString text=ui->lineEdit_9->text();
                m_Ch[2].Freq= text.toFloat();
                currenteditpos=9;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_9->setText(QString::number(m_Ch[2].Freq,'g',6));
         }
        else if(obj==ui->lineEdit_10)
         {
                QString text=ui->lineEdit_10->text();
                m_Ch[4].Amp= text.toFloat();
                currenteditpos=10;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_10->setText(QString::number(m_Ch[4].Amp,'g',6));
         }
        else if(obj==ui->lineEdit_11)
         {
                QString text=ui->lineEdit_11->text();
                m_Ch[4].Phase= text.toFloat();
                currenteditpos=11;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_11->setText(QString::number(m_Ch[4].Phase,'g',6));
         }
        else if(obj==ui->lineEdit_12)
         {
                QString text=ui->lineEdit_12->text();
                m_Ch[4].Freq= text.toFloat();
                currenteditpos=12;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_12->setText(QString::number(m_Ch[4].Freq,'g',6));
         }
        else if(obj==ui->lineEdit_13)
         {
                QString text=ui->lineEdit_13->text();
                m_Ch[5].Amp= text.toFloat();
                currenteditpos=13;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_13->setText(QString::number(m_Ch[5].Amp,'g',6));
         }
        else if(obj==ui->lineEdit_14)
         {
                QString text=ui->lineEdit_14->text();
                m_Ch[5].Phase= text.toFloat();
                currenteditpos=14;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_14->setText(QString::number(m_Ch[5].Phase,'g',6));
         }
        else if(obj==ui->lineEdit_15)
         {
                QString text=ui->lineEdit_15->text();
                m_Ch[5].Freq= text.toFloat();
                currenteditpos=15;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_15->setText(QString::number(m_Ch[5].Freq,'g',6));
         }
        else if(obj==ui->lineEdit_16)
         {
                QString text=ui->lineEdit_16->text();
                m_Ch[6].Amp= text.toFloat();
                currenteditpos=16;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_16->setText(QString::number(m_Ch[6].Amp,'g',6));
         }
        else if(obj==ui->lineEdit_17)
         {
                QString text=ui->lineEdit_17->text();
                m_Ch[6].Phase= text.toFloat();
                currenteditpos=17;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_17->setText(QString::number(m_Ch[6].Phase,'g',6));
         }
        else if(obj==ui->lineEdit_18)
         {
                QString text=ui->lineEdit_18->text();
                m_Ch[6].Freq= text.toFloat();
                currenteditpos=18;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_18->setText(QString::number(m_Ch[6].Freq,'g',6));
         }
        else if(obj==ui->lineEdit_19)
         {
                QString text=ui->lineEdit_19->text();
                A_CurStep= text.toFloat();
                currenteditpos=19;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_19->setText(QString::number(A_CurStep,'g',6));
         }
        else if(obj==ui->lineEdit_20)
         {
                QString text=ui->lineEdit_20->text();
                A_VolStep= text.toFloat();
                currenteditpos=20;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_20->setText(QString::number(A_VolStep,'g',6));
         }
        else if(obj==ui->lineEdit_21)
         {
                QString text=ui->lineEdit_21->text();
                A_PhaStep= text.toFloat();
                currenteditpos=21;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_21->setText(QString::number(A_PhaStep,'g',6));
         }
        else if(obj==ui->lineEdit_22)
         {
                QString text=ui->lineEdit_22->text();
                A_FreStep= text.toFloat();
                currenteditpos=22;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_22->setText(QString::number(A_FreStep,'g',6));
         }
        else if(obj==ui->lineEdit_23)
         {
                QString text=ui->lineEdit_23->text();
                A_StepTime= text.toFloat();
                currenteditpos=23;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_23->setText(QString::number(A_StepTime,'g',6));
         }
        else if(obj==ui->lineEdit_24)
         {
                QString text=ui->lineEdit_24->text();
                A_StepNum= text.toFloat();
                currenteditpos=24;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_24->setText(QString::number(A_StepNum,'g',6));
         }
        else if(obj==ui->lineEdit_25)
         {
                QString text=ui->lineEdit_25->text();
                A_PreFaultTime= text.toFloat();
                currenteditpos=25;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_25->setText(QString::number(A_PreFaultTime,'g',6));
         }
        else if(obj==ui->lineEdit_26)
         {
                QString text=ui->lineEdit_26->text();
                A_FaultTime= text.toFloat();
                currenteditpos=26;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_26->setText(QString::number(A_FaultTime,'g',6));
         }
        else if(obj==ui->lineEdit_27)
         {
                QString text=ui->lineEdit_27->text();
                A_PostFaultTime= text.toFloat();
                currenteditpos=27;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_27->setText(QString::number(A_PostFaultTime,'g',6));
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
                m_Ch[3].Amp= text.toFloat();
                currenteditpos=29;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_29->setText(QString::number(m_Ch[3].Amp,'g',6));
         }
        else if(obj==ui->lineEdit_30)
         {
                QString text=ui->lineEdit_30->text();
                m_Ch[3].Phase= text.toFloat();
                currenteditpos=30;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_30->setText(QString::number(m_Ch[3].Phase,'g',6));
         }
        else if(obj==ui->lineEdit_31)
         {
                QString text=ui->lineEdit_31->text();
                m_Ch[3].Freq= text.toFloat();
                currenteditpos=31;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_31->setText(QString::number(m_Ch[3].Freq,'g',6));
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
    if(A_Phasesymmetry)
        A_Phasesymmetry=false;
    else
        A_Phasesymmetry=true;

    if(A_Phasesymmetry)
    {
       if(VolPhaseChange!=0)
       {
           switch(VolPhaseChange)
           {
           case 1:
                        m_Ch[1].Phase=m_Ch[0].Phase-120;
                           m_Ch[2].Phase=m_Ch[0].Phase+120;
                           if(m_Ch[1].Phase>180)
                               m_Ch[1].Phase=m_Ch[1].Phase-360;
                           else if(m_Ch[1].Phase<-180)
                               m_Ch[1].Phase+=360;
                           if(m_Ch[2].Phase>180)
                               m_Ch[2].Phase=m_Ch[2].Phase-360;
                           else if(m_Ch[2].Phase<-180)
                               m_Ch[2].Phase+=360;
               break;
           case 2:
                            m_Ch[0].Phase=m_Ch[1].Phase+120;
                             if(m_Ch[0].Phase>180)
                                 m_Ch[0].Phase-=360;
                             else if(m_Ch[0].Phase<-180)
                                 m_Ch[0].Phase+=360;

                             m_Ch[2].Phase=m_Ch[0].Phase+120;
                             if(m_Ch[2].Phase>180)
                                 m_Ch[2].Phase-=360;
                             else if(m_Ch[2].Phase<-180)
                                 m_Ch[2].Phase+=360;
               break;

           case 3:
                               m_Ch[0].Phase=m_Ch[2].Phase-120;
                               if(m_Ch[0].Phase>180)
                                   m_Ch[0].Phase-=360;
                               else if(m_Ch[0].Phase<-180)
                                   m_Ch[0].Phase+=360;
                               m_Ch[1].Phase=m_Ch[0].Phase-120;
                               if(m_Ch[1].Phase>180)
                                   m_Ch[1].Phase-=360;
                               else if(m_Ch[1].Phase<-180)
                                   m_Ch[1].Phase+=360;
               break;

           }


           ui->lineEdit_2->setText(QString::number(m_Ch[0].Phase,'g',6));
           ui->lineEdit_5->setText(QString::number(m_Ch[1].Phase,'g',6));
           ui->lineEdit_8->setText(QString::number(m_Ch[2].Phase,'g',6));


       }

       if(CurPhaseChange!=0)
       {
        switch(CurPhaseChange)
        {
        case 1:
                        m_Ch[5].Phase=m_Ch[4].Phase-120;
                       m_Ch[6].Phase=m_Ch[4].Phase+120;
                       if(m_Ch[5].Phase>180)
                           m_Ch[5].Phase=m_Ch[5].Phase-360;
                       else if(m_Ch[5].Phase<-180)
                           m_Ch[5].Phase+=360;
                       if(m_Ch[6].Phase>180)
                           m_Ch[6].Phase=m_Ch[6].Phase-360;
                       else if(m_Ch[6].Phase<-180)
                           m_Ch[6].Phase+=360;
            break;
        case 2:
                        m_Ch[4].Phase=m_Ch[5].Phase+120;
                          if(m_Ch[4].Phase>180)
                              m_Ch[4].Phase-=360;
                          else if(m_Ch[4].Phase<-180)
                              m_Ch[4].Phase+=360;

                          m_Ch[6].Phase=m_Ch[4].Phase+120;
                          if(m_Ch[6].Phase>180)
                              m_Ch[6].Phase-=360;
                          else if(m_Ch[6].Phase<-180)
                              m_Ch[6].Phase+=360;
            break;

        case 3:
                         m_Ch[4].Phase=m_Ch[6].Phase-120;
                          if(m_Ch[4].Phase>180)
                              m_Ch[4].Phase-=360;
                          else if(m_Ch[4].Phase<-180)
                              m_Ch[4].Phase+=360;
                          m_Ch[5].Phase=m_Ch[4].Phase-120;
                          if(m_Ch[5].Phase>180)
                              m_Ch[5].Phase-=360;
                          else if(m_Ch[5].Phase<-180)
                              m_Ch[5].Phase+=360;
            break;
        }


        ui->lineEdit_11->setText(QString::number(m_Ch[4].Phase,'g',6));
        ui->lineEdit_14->setText(QString::number(m_Ch[5].Phase,'g',6));
        ui->lineEdit_17->setText(QString::number(m_Ch[6].Phase,'g',6));
       }
    }
}

void Manualtest::on_checkBox_8_clicked()
{
    if(VariatyVz2!=1)  //
    {
        if(m_Ch[0].Freq==SysFrec&&m_Ch[1].Freq==SysFrec&&m_Ch[2].Freq==SysFrec)
      {
         ui->checkBox_8->setChecked(true);
         VariatyVz2=1;
        if(m_Ch[3].checked==1) //如果Vz=3U0 则取消变Vz
        {
           m_Ch[3].checked=0;
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
    if(m_Ch[3].checked==1)
    {
        m_Ch[3].checked=0;
        ui->checkBox_9->setChecked(false);
    }
    else
    {
        m_Ch[3].checked=1;
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
