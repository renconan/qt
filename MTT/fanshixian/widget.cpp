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
long auto_i=0,IRQcnt=0,auto_count=0,tice=0;
int   input_par=0,HelpFlag=0;
int   INPAR=0,seq_count,SeqStep=0;
int   testcount=0,time_relay_testcount=0,auto_end=0;
int   cx,cy,cx_start,cx_mid,cx_end,starttest=0,low,row,page;
int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC;
float LogicResolution,SystemFrec;
float CurrentValue,VoltageValue,Ferequencer,TripTime;
int exit_par,save_f;
int sys[4];
int   TestObject=1;
float CurrentStart,CurrentEnd,CurrentStep,VoltageStart,VoltageEnd,VoltageStep,FrequencyStart,FrequencyEnd,FrequencyStep,PostTime,WaitTime;
float I1Value,I2Value,I3Value,I1Phase,I2Phase,I3Phase;
float V1Value,V2Value,V3Value,V1Phase,V2Phase,V3Phase;
float  TripTimeValue[500];
int   ResultEndCount,ResultPage,ResultFlag,ResultCount=1,DisResult,ResultEndPage;
int  CurrentPage=0;

int   FreqTriple=2;
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

float V_coarse_step=1,V_fine_step=0.01,I_coarse_step=1,I_fine_step=0.01,F_coarse_step=1,F_fine_step=0.1;
float faultduration=0;

float SystemVolt=57,VzOutput3Uo=0,VzOutputAc=0,VzOutputDc=0;

float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;

float postfaulttime=3;
float prefaulttime=3;
float Preparetime=3;
int   dcouttype=1;

int Faulttype=4;





extern TEMPRORARYDATA *temprorarydata;

QVector<QPointF> AllTripTime; //所有的触发信息


bool Threadrunstate=true;

bool SoftStartEndflag=false;

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

#ifdef ARMLINUX
  this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
#endif
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    //init_par();
    setFixedSize(800,480);
    display_variry(TestObject);
    initlineedit();
    obj=" ";
    showFullScreen();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->label_11->hide();
    ui->label_12->hide();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();
    ui->label_16->hide();

    QPixmap img(":/picture/hei.png");
    ui->label_9->setPixmap(img);
    ui->label_10->setPixmap(img);
    ui->label_11->setPixmap(img);
    ui->label_12->setPixmap(img);

    Showfaulttype(Faulttype);





        //ui->label_5->setText("<font color=blue>"+QString("反时限(2.2.1)")+"</font>");

////
        ui->lineEdit->installEventFilter(this);
        ui->lineEdit_2->installEventFilter(this);
        ui->lineEdit_3->installEventFilter(this);
        ui->lineEdit_4->installEventFilter(this);
        ui->lineEdit_5->installEventFilter(this);
        ui->lineEdit_6->installEventFilter(this);

        ui->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_5->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);


        //ui->pushButton->setFocusPolicy(Qt::NoFocus);
        //ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
        //ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
        //ui->pushButton_4->setFocusPolicy(Qt::NoFocus);

        //以下到分段之前为增加表格代码2016-5-16
        ui->tableWidget->setColumnCount(2);
        ui->tableWidget->setRowCount(10);
        int wide=ui->tableWidget->width()/2;//
        for(int i=0;i<2;i++)
        {
         ui->tableWidget->setColumnWidth(i,wide-20);
        }
        QStringList strlist;
        strlist<<tr("I(A)")<<tr("T(s)");
        ui->tableWidget->setHorizontalHeaderLabels(strlist);
        for(int i=0;i<10;i++)
        for(int j=0;j<2;j++)
        {
          ui->tableWidget->setItem(i,j,new QTableWidgetItem);
        }


        //ui->groupBox_4->setTitle(QString("输出选择"));

        Curve=new Plotter(this);
        ui->horizontalLayout_2->addWidget(Curve);

        InitTestresult();


ui->label_8->hide();
ui->lineEdit_6->hide();


    currenteditpos=0;
    uartthread =new thread1;
    connect(uartthread,SIGNAL(SIG_binarystate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_testresult(int)),SLOT(showtestresult(int)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
      connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
      connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));
    uartthread->start();
}

void Widget::updateUI()
{
    ui->label_5->setText("<font color=blue>"+QString(tr("反时限(2.2.1)"))+"</font>");
    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_2->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_4->setText(tr("测试"));

        ui->groupBox->setTitle(tr("测试曲线"));
        ui->groupBox_2->setTitle(tr("参数设置"));
        ui->groupBox_3->setTitle(tr("测试结果"));
        ui->groupBox_4->setTitle(tr("输出选择"));

        ui->label_7->setText(tr("间隔时间(s)："));
        ui->label_4->setText(tr("故障时间(s)："));

        if(TestObject==1)
        {
            ui->label->setText(tr("起始值(A):"));
            ui->label_2->setText(tr("终止值(A):"));
            ui->label_3->setText(tr("步长(A):"));
            ui->radioButton_7->setText(tr("IA,IB并联"));
            ui->radioButton_8->setText(tr("IB,IC并联"));
            ui->radioButton_9->setText(tr("IC,IA并联"));
            ui->radioButton_10->setText(tr("IA,IB,IC并联"));
        }
        else if(TestObject==2)
        {
            ui->label->setText(tr("起始值(V):"));
            ui->label_2->setText(tr("终止值(V):"));
            ui->label_3->setText(tr("步长(V):"));
            ui->radioButton_7->setText(tr("VAB串联"));
            ui->radioButton_8->setText(tr("VBC串联"));
            ui->radioButton_9->setText(tr("VCA串联"));
        }
        else if(TestObject==3)
        {
            ui->label->setText(tr("起始值(Hz):"));
            ui->label_2->setText(tr("终止值(Hz):"));
            ui->label_3->setText(tr("步长(Hz):"));
        }
}
void Widget::initlineedit()
{


    switch(TestObject)
    {
    case 1:
        ui->label->setText(tr("起始值(A):"));
        ui->label_2->setText(tr("终止值(A):"));
        ui->label_3->setText(tr("步长(A):"));


        ui->lineEdit->setText(QString::number(CurrentStart));
        ui->lineEdit_2->setText(QString::number(CurrentEnd));
        ui->lineEdit_3->setText(QString::number(CurrentStep));

        ui->lineEdit_4->setText(QString::number(WaitTime));
        ui->lineEdit_5->setText(QString::number(PostTime));
        ui->lineEdit_6->setText(QString::number(postfaulttime));

        break;
    case 2:

        ui->label->setText(tr("起始值(V):"));
        ui->label_2->setText(tr("终止值(V):"));
        ui->label_3->setText(tr("步长(V):"));

        ui->lineEdit->setText(QString::number(VoltageStart));
        ui->lineEdit_2->setText(QString::number(VoltageEnd));
        ui->lineEdit_3->setText(QString::number(VoltageStep));
        ui->lineEdit_4->setText(QString::number(WaitTime));
        ui->lineEdit_5->setText(QString::number(PostTime));
        ui->lineEdit_6->setText(QString::number(postfaulttime));
        break;

    case 3:

        ui->label->setText(tr("起始值(Hz):"));
        ui->label_2->setText(tr("终止值(Hz):"));
        ui->label_3->setText(tr("步长(Hz):"));

        ui->lineEdit->setText(QString::number(FrequencyStart));
        ui->lineEdit_2->setText(QString::number(FrequencyEnd));
        ui->lineEdit_3->setText(QString::number(FrequencyStep));
        ui->lineEdit_4->setText(QString::number(WaitTime));
        ui->lineEdit_5->setText(QString::number(PostTime));
        ui->lineEdit_6->setText(QString::number(postfaulttime));

        break;
    }
}




void Widget::showdspstate(int state)
{
    switch(state)
        {
    case 0: ui->label_6->setText(tr("运行正常")); break;
    case 1: ui->label_6->setText(tr("IA 过载")); break;
    case 2: ui->label_6->setText(tr("IABC 温度保护")); break;
    case 3: ui->label_6->setText(tr("IC 间歇")); break;
    case 4: ui->label_6->setText(tr("IB 过载")); break;
    case 5: ui->label_6->setText(tr("IC 闭锁")); break;
    case 6: ui->label_6->setText(tr("IA 间歇")); break;
    case 7: ui->label_6->setText(tr("IC 过载")); break;
    case 8: ui->label_6->setText(tr("IABC 电源保护")); break;
    case 9: ui->label_6->setText(tr("IB 间歇")); break;
    case 10: ui->label_6->setText(tr("VA 过载")); break;

    case 12: ui->label_6->setText(tr("VC 间歇")); break;
    case 13: ui->label_6->setText(tr("VB 过载")); break;
    case 14: ui->label_6->setText(tr("VABC 温度保护")); break;
    case 15: ui->label_6->setText(tr("VA 间歇")); break;
    case 16: ui->label_6->setText(tr("VC 过载")); break;
    case 17: ui->label_6->setText(tr("VABC 电源保护")); break;
    case 18: ui->label_6->setText(tr("Vz 过载")); break;
    case 19: ui->label_6->setText(tr("直流过载")); break;
    case 20: ui->label_6->setText(tr("IABC 电源保护")); break;
    case 21: ui->label_6->setText(tr("VC 间歇")); break;
        }
    QPixmap img(":/picture/hei.png");
    QPixmap pic(":/picture/hong.png");
    switch(state)
    {
    case 100:
        ui->label_9->setPixmap(img);
        ui->label_10->setPixmap(img);
        ui->label_11->setPixmap(img);
        ui->label_12->setPixmap(img);
        break;
    case 101: ui->label_9->setPixmap(pic); break;
    case 102: ui->label_10->setPixmap(pic); break;
    case 103: ui->label_11->setPixmap(pic); break;
    case 104: ui->label_12->setPixmap(pic); break;
    case 111: ui->label_9->setPixmap(img); break;
    case 112: ui->label_10->setPixmap(img); break;
    case 113: ui->label_11->setPixmap(img); break;
    case 114: ui->label_12->setPixmap(img); break;
    }


}

void Widget::Showfaulttype(int type)
{

    switch(type)
    {
    case 1:
    ui->radioButton_4->setChecked(true);
    ui->radioButton_5->setChecked(false);
    ui->radioButton_6->setChecked(false);
    ui->radioButton_7->setChecked(false);
    ui->radioButton_8->setChecked(false);
    ui->radioButton_9->setChecked(false);
    ui->radioButton_10->setChecked(false);

    break;
    case 2:
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(true);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_7->setChecked(false);
        ui->radioButton_8->setChecked(false);
        ui->radioButton_9->setChecked(false);
        ui->radioButton_10->setChecked(false);
        break;
    case 3:
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(true);
        ui->radioButton_7->setChecked(false);

        ui->radioButton_8->setChecked(false);
        ui->radioButton_9->setChecked(false);
        ui->radioButton_10->setChecked(false);
        break;
    case 4:
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_7->setChecked(true);
        ui->radioButton_8->setChecked(false);
        ui->radioButton_9->setChecked(false);
        ui->radioButton_10->setChecked(false);
        break;

    case 5:
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_7->setChecked(false);
        ui->radioButton_8->setChecked(true);
        ui->radioButton_9->setChecked(false);
        ui->radioButton_10->setChecked(false);

        break;



    case 6:
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_7->setChecked(false);
        ui->radioButton_8->setChecked(false);
        ui->radioButton_9->setChecked(true);
        ui->radioButton_10->setChecked(false);


        break;

    case 7:
        ui->radioButton_4->setChecked(false);
        ui->radioButton_5->setChecked(false);
        ui->radioButton_6->setChecked(false);
        ui->radioButton_7->setChecked(false);
        ui->radioButton_8->setChecked(false);
        ui->radioButton_9->setChecked(false);
        ui->radioButton_10->setChecked(true);
        break;
    }
}






void Widget::showtestresult(int k)
{

    switch(TestObject)
    {
    case 1:
        obj="I(A)";
        break;
    case 2:
        obj="U(V)";
        break;
    case 3:
        obj="F(Hz)";
        break;
    }

    //显示表格
    if(AllTripTime.size()==0)//ceshijieguomeiyou
    {
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(2);
        ui->tableWidget->setRowCount(10);

        int wide=ui->tableWidget->width()/2;
        for(int i=0;i<2;i++)
        {
         ui->tableWidget->setColumnWidth(i,wide-20);
        }
        QStringList strlist;
        strlist<<obj<<tr("T(s)");
        ui->tableWidget->setHorizontalHeaderLabels(strlist);


        for(int i=0;i<10;i++)
        for(int j=0;j<2;j++)
        {
          ui->tableWidget->setItem(i,j,new QTableWidgetItem);
        }
    }
    else
    {
        ui->tableWidget->clear();
        int rowcount=AllTripTime.size();
        ui->tableWidget->setColumnCount(2);
        ui->tableWidget->setRowCount(rowcount);
        int wide=ui->tableWidget->width()/2;
        for(int i=0;i<2;i++)
        {
         ui->tableWidget->setColumnWidth(i,wide-20);
        }
        QStringList strlist;
        strlist<<obj<<tr("T(s)");
        ui->tableWidget->setHorizontalHeaderLabels(strlist);


        for(int i=0;i<rowcount;i++)
        for(int j=0;j<2;j++)
        {
         QTableWidgetItem  *item=new QTableWidgetItem;
         ui->tableWidget->setItem(i,j,item);
         if(j==0)
            item->setText(QString::number(AllTripTime.at(i).x(),'g',5));
         else
            item->setText(QString::number(AllTripTime.at(i).y(),'g',5));
        }
        ui->tableWidget->scrollToBottom();
    }
      ShowExcitCurve();//显示曲线

}

void Widget::InitTestresult()
{


    //X 轴为时间 Y轴为电流 电压 频率值
    switch(TestObject)
    {

    case 1:
    if(CurrentStart>CurrentEnd)
   {
       CurveSetting.maxX=CurrentStart+CurrentStart*0.2;
       CurveSetting.minX=CurrentEnd-CurrentEnd*0.2;
   }
    else
    {
        CurveSetting.minX=CurrentStart-CurrentStart*0.2;
        CurveSetting.maxX=CurrentEnd+CurrentEnd*0.2;
    }
    Curve->CurveType=1;
    CurveSetting.minY=0;
    CurveSetting.maxY=PostTime+PostTime*0.2;
    Curve->setPlotSettings(CurveSetting);
    break;


   case 2:

        if(VoltageStart>VoltageEnd)
       {
           CurveSetting.maxX=VoltageStart+VoltageStart*0.2;
           CurveSetting.minX=VoltageEnd-VoltageEnd*0.2;
       }
        else
        {
            CurveSetting.minX=VoltageStart-VoltageStart*0.2;
            CurveSetting.maxX=VoltageEnd+VoltageEnd*0.2;
        }
        Curve->CurveType=2;
        CurveSetting.minY=0;
        CurveSetting.maxY=PostTime+PostTime*0.2;

        Curve->setPlotSettings(CurveSetting);
        break;

    case 3:

        if(FrequencyStart>FrequencyEnd)
       {
           CurveSetting.maxX=FrequencyStart+FrequencyStart*0.2;
           CurveSetting.minX=FrequencyEnd-FrequencyEnd*0.2;
       }
        else
        {
           CurveSetting.minX=FrequencyStart-FrequencyStart*0.2;
           CurveSetting.maxX=FrequencyEnd+FrequencyEnd*0.2;
        }
        Curve->CurveType=3;
        CurveSetting.minY=0;
        CurveSetting.maxY=PostTime+PostTime*0.2;
        Curve->setPlotSettings(CurveSetting);
        break;

    }



}

void Widget::ShowExcitCurve()//显示曲线
{


    if(AllTripTime.size()==0)
      {
          Curve->CleatAll();
          return;
      }

   //X 轴为时间 Y轴为电流 电压 频率值
   switch(TestObject)
   {

   case 1:
   if(CurrentStart>CurrentEnd)
  {
      CurveSetting.maxX=CurrentStart+CurrentStart*0.2;
      CurveSetting.minX=CurrentEnd-CurrentEnd*0.2;
  }
   else
   {
       CurveSetting.minX=CurrentStart-CurrentStart*0.2;
       CurveSetting.maxX=CurrentEnd+CurrentEnd*0.2;
   }
   Curve->CurveType=1;
   CurveSetting.minY=0;
   CurveSetting.maxY=PostTime+PostTime*0.2;
   Curve->setCurveData(0,AllTripTime);
   Curve->setPlotSettings(CurveSetting);
   break;


  case 2:

       if(VoltageStart>VoltageEnd)
      {
          CurveSetting.maxX=VoltageStart+VoltageStart*0.2;
          CurveSetting.minX=VoltageEnd-VoltageEnd*0.2;
      }
       else
       {
           CurveSetting.minX=VoltageStart-VoltageStart*0.2;
           CurveSetting.maxX=VoltageEnd+VoltageEnd*0.2;
       }
       Curve->CurveType=2;
       CurveSetting.minY=0;
       CurveSetting.maxY=PostTime+PostTime*0.2;
       Curve->setCurveData(0,AllTripTime);
       Curve->setPlotSettings(CurveSetting);
       break;

   case 3:

       if(FrequencyStart>FrequencyEnd)
      {
          CurveSetting.maxX=FrequencyStart+FrequencyStart*0.2;
          CurveSetting.minX=FrequencyEnd-FrequencyEnd*0.2;
      }
       else
       {
           CurveSetting.minX=FrequencyStart-FrequencyStart*0.2;
           CurveSetting.maxX=FrequencyEnd+FrequencyEnd*0.2;
       }
        Curve->CurveType=3;
       CurveSetting.minY=0;
       CurveSetting.maxY=PostTime+PostTime*0.2;
       Curve->setCurveData(0,AllTripTime);
       Curve->setPlotSettings(CurveSetting);
        break;

   }


}






void Widget::display_variry(int Outputmode)// U-T  I-T  F-T
{

    switch(Outputmode)
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

}

void Widget::init_par()
{
    FILE *fp;
    if((fp=fopen("sysset.txt","a+"))==NULL)
    return;
    fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
           &SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&Preparetime,&current_language);
    SystemFrec=2*SystemFrec;
    exit_par=0;

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

        TestObject=1;
        CurrentStart=5;
        CurrentEnd=1;
        CurrentStep=0.5;
        VoltageStart=20;
        VoltageEnd=100;
        VoltageStep=1;
        FrequencyStart=50;
        FrequencyEnd=40;
        FrequencyStep=0.2;
        PostTime=5;
        WaitTime=2;

}

void Widget::vi_parameter_enter(int position)
{

    float Maxfactor=1;
    if(TestObject==1)
    {
        if(Faulttype>=1&&Faulttype<=3)
        Maxfactor=1;
        else if(Faulttype>=4&&Faulttype<=6)
        Maxfactor=2;
        else if(Faulttype==7)
        Maxfactor=3;
    }
    else if(TestObject==2)
    {       if(Faulttype>=1&&Faulttype<=3)
            Maxfactor=1;
            else if(Faulttype>=4&&Faulttype<=6)
            Maxfactor=2;

    }
    else if(TestObject==3)
    {
    Maxfactor=1;
    }





    switch(position)
    {
    case 1:
                             if(TestObject==1)
                               {
                                 if(CurrentStart>MAX_I_VALUEDATA_AC*Maxfactor)
                                   CurrentStart=MAX_I_VALUEDATA_AC*Maxfactor;
                                 else if(CurrentStart<0)
                                   CurrentStart=0;//鍙傛暟闄愬埗
                               }
                             else if(TestObject==2)
                               {
                                 if(VoltageStart>MAX_V_VALUEDATA_AC*Maxfactor)
                                   VoltageStart=MAX_V_VALUEDATA_AC*Maxfactor;
                                 else if(VoltageStart<0)
                                   VoltageStart=0;//鍙傛暟闄愬埗
                               }
                             else if(TestObject==3)
                               {
                                 if(FrequencyStart>999.99)
                                   FrequencyStart=999.99;
                                 else if(FrequencyStart<0)
                                   FrequencyStart=0;//鍙傛暟闄愬埗
                               }
                             break;
   case 2:
                             if(TestObject==1)
                                {
                                  if(CurrentEnd>MAX_I_VALUEDATA_AC*Maxfactor)
                                    CurrentEnd=MAX_I_VALUEDATA_AC*Maxfactor;
                                  else if(CurrentEnd<0)
                                    CurrentEnd=0;//鍙傛暟闄愬埗
                                }
                              else if(TestObject==2)
                                {
                                  if(VoltageEnd>MAX_V_VALUEDATA_AC*Maxfactor)
                                    VoltageEnd=MAX_V_VALUEDATA_AC*Maxfactor;
                                  else if(VoltageEnd<0)
                                    VoltageEnd=0;//鍙傛暟闄愬埗
                                }
                              else if(TestObject==3)
                                {
                                  if(FrequencyEnd>999.99)
                                    FrequencyEnd=999.99;
                                  else if(FrequencyEnd<0)
                                    FrequencyEnd=0;//鍙傛暟闄愬埗
                                }
                             break;
  case 3:
                              if(TestObject==1)
                                {
                                  if(CurrentStep>fabs(CurrentStart-CurrentEnd))
                                    CurrentStep=fabs(CurrentStart-CurrentEnd);
                                  else if(CurrentStep<0)
                                    CurrentStep=0;//鍙傛暟闄愬埗


                                }
                              else if(TestObject==2)
                                {
                                  if(VoltageStep>fabs(VoltageStart-VoltageEnd))
                                    VoltageStep=fabs(VoltageStart-VoltageEnd);
                                  else if(VoltageStep<0)
                                    VoltageStep=0;//鍙傛暟闄愬埗
                                }
                              else if(TestObject==3)
                                {
                                  if(FrequencyStep>fabs(FrequencyStart-FrequencyEnd))
                                    FrequencyStep=fabs(FrequencyStart-FrequencyEnd);
                                  else if(FrequencyStep<0)
                                    FrequencyStep=0;//鍙傛暟闄愬埗
                                }
                              break;


    case 4:
                            if(WaitTime>100)
                                WaitTime=100;
                           else if(WaitTime<0)
                                WaitTime=0;//鍙傛暟闄愬埗
                               break;
    case 5:
                                if(PostTime>999.99)
                                    PostTime=999.99;
                                  else if(PostTime<0)
                                    PostTime=0;//鍙傛暟闄愬埗
                                 break;
    case 6:
        if(postfaulttime>999.99)
            postfaulttime=999.99;
          else if(postfaulttime<0)
            postfaulttime=0;//鍙傛暟闄愬埗
         break;

}
}


void Widget::on_pushButton_2_clicked()
{
    if(starttest==1)
        return;
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

             strcpy(strbuf,"反时限");
             fprintf(fp,"%s\r\n",strbuf);

             if(TestObject==1)
               strcpy(strbuf,"I-T曲线:");
             else if(TestObject==2)
               strcpy(strbuf,"U-T曲线:");
             else if(TestObject==3)
               strcpy(strbuf,"F-T曲线:");
             fprintf(fp,"%s\r\n",strbuf);

             for(int i=0;i<AllTripTime.size();i++)
               {
                 if(TestObject==1)
                   {
                     sprintf(strbuf,"I=%.4f A",AllTripTime.at(i).x());
                   }
                 else if(TestObject==2)
                   {
                     sprintf(strbuf,"U=%.3f V",AllTripTime.at(i).x());
                   }
                 else if(TestObject==3)
                   {
                     sprintf(strbuf,"F=%.3f Hz",AllTripTime.at(i).x());
                   }
                 fprintf(fp,"%s\r\n",strbuf);


                 if(AllTripTime.at(i).y()==-1)
                   sprintf(strbuf,"触发时间(s):未动作");
                 else
                   sprintf(strbuf,"触发时间(s):%.4f",AllTripTime.at(i).y());
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

         strcpy(strbuf,"Inversed-Time test result");
         fprintf(fp,"%s\r\n",strbuf);

         if(TestObject==1)
           strcpy(strbuf,"I-T Curve:");
         else if(TestObject==2)
           strcpy(strbuf,"V-T Curve:");
         else if(TestObject==3)
           strcpy(strbuf,"F-T Curve:");
         fprintf(fp,"%s\r\n",strbuf);

         for(int i=0;i<AllTripTime.size();i++)
           {
             if(TestObject==1)
               {
                 sprintf(strbuf,"I=%.4f A",AllTripTime.at(i).x());
               }
             else if(TestObject==2)
               {
                 sprintf(strbuf,"V=%.3f V",AllTripTime.at(i).x());
               }
             else if(TestObject==3)
               {
                 sprintf(strbuf,"F=%.3f Hz",AllTripTime.at(i).x());
               }
                fprintf(fp,"%s\r\n",strbuf);


                if(AllTripTime.at(i).y()==-1)
                 sprintf(strbuf,"Trip time(s):No Trip");
                else
                 sprintf(strbuf,"Trip time(s):%.4f",AllTripTime.at(i).y());
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


void Widget::on_radioButton_clicked()
{
   TestObject=1;
   showtestresult(TestObject);
   ui->groupBox_4->show();
   ui->groupBox_4->setTitle(QString(tr("输出选择")));

   ui->radioButton_10->show();
   initlineedit();

   ui->radioButton_4->setText("IA");
   ui->radioButton_5->setText("IB");
   ui->radioButton_6->setText("IC");
   ui->radioButton_7->setText(tr("IA,IB并联"));
   ui->radioButton_8->setText(tr("IB,IC并联"));
   ui->radioButton_9->setText(tr("IC,IA并联"));
   ui->radioButton_10->setText(tr("IA,IB,IC并联"));

   InitTestresult();

}

void Widget::on_radioButton_2_clicked()
{
   TestObject=2;
   showtestresult(TestObject);
   ui->groupBox_4->setTitle(QString(tr("输出选择")));

   if(Faulttype==7)
   {
      Faulttype=1;
      Showfaulttype(Faulttype);
   }

   ui->groupBox_4->show();
   ui->radioButton_4->setText("VA");
   ui->radioButton_5->setText("VB");
   ui->radioButton_6->setText("VC");
   ui->radioButton_7->setText(tr("VAB串联"));
   ui->radioButton_8->setText(tr("VBC串联"));
   ui->radioButton_9->setText(tr("VCA串联"));
   ui->radioButton_10->hide();


   initlineedit();
   InitTestresult();
}

void Widget::on_radioButton_3_clicked()
{
  TestObject=3;
  showtestresult(TestObject);
  ui->groupBox_4->hide();
  initlineedit();
  InitTestresult();

}





void Widget::setlineedittext(QString str)
{
    bool ok;
    float number=str.toFloat(&ok);
    if(ok==false)
        return;
    if(TestObject==1)
    {


    switch(currenteditpos)
    {
    case 1:

        CurrentStart=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(CurrentStart,'g',6));
        break;
    case 2:
        CurrentEnd=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(CurrentEnd,'g',6));
        break;
    case 3:
        CurrentStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(CurrentStep,'g',6));
        break;
    case 4:
        WaitTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(WaitTime,'g',6));
        break;
    case 5:
        PostTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PostTime,'g',6));
        break;
    case 6:
        postfaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(postfaulttime,'g',6));
        break;

    }


    }

    else if(TestObject==2)
    {
    switch(currenteditpos)
    {
    case 1:
        VoltageStart=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VoltageStart,'g',6));
        break;
    case 2:
        VoltageEnd=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VoltageEnd,'g',6));
        break;
    case 3:
        VoltageStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(VoltageStep,'g',6));
        break;
    case 4:
        WaitTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(WaitTime,'g',6));
        break;
    case 5:
        PostTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PostTime,'g',6));
        break;
    case 6:
        postfaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(postfaulttime,'g',6));
        break;
    }

    }
    else if(TestObject==3)
    {
    switch(currenteditpos)
    {
    case 1:
        FrequencyStart=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FrequencyStart,'g',6));
        break;
    case 2:
        FrequencyEnd=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FrequencyEnd,'g',6));
        break;
    case 3:
        FrequencyStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FrequencyStep,'g',6));
        break;
    case 4:
        WaitTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(WaitTime,'g',6));
        break;
    case 5:
        PostTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PostTime,'g',6));
        break;
    case 6:
        postfaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(postfaulttime,'g',6));
        break;
    }

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
helpdialog->SetHelpfilename("fanshixian.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("fanshixian1.txt");
//helpdialog->SetHelpfilename("fanshixian.txt");
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
        switch(TestObject)
        {
        case 1:
            if(obj==ui->lineEdit)
            {
                   QString text=ui->lineEdit->text();
                   CurrentStart= text.toFloat();
                   currenteditpos=1;
                   vi_parameter_enter(currenteditpos);
                   ui->lineEdit->setText(QString::number(CurrentStart,'g',6));
            }
            else if(obj==ui->lineEdit_2)
            {
                   QString text=ui->lineEdit_2->text();
                   CurrentEnd= text.toFloat();
                   currenteditpos=2;
                   vi_parameter_enter(currenteditpos);
                   ui->lineEdit_2->setText(QString::number(CurrentEnd,'g',6));
            }
            else if(obj==ui->lineEdit_3)
             {
                    QString text=ui->lineEdit_3->text();
                    CurrentStep= text.toFloat();
                    currenteditpos=3;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_3->setText(QString::number(CurrentStep,'g',6));
             }
            else if(obj==ui->lineEdit_4)
             {
                    QString text=ui->lineEdit_4->text();
                    WaitTime= text.toFloat();
                    currenteditpos=4;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_4->setText(QString::number(WaitTime,'g',6));
             }
            else if(obj==ui->lineEdit_5)
             {
                    QString text=ui->lineEdit_5->text();
                    PostTime= text.toFloat();
                    currenteditpos=5;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_5->setText(QString::number(PostTime,'g',6));
             }
            else if(obj==ui->lineEdit_6)
             {
                    QString text=ui->lineEdit_6->text();
                    postfaulttime= text.toFloat();
                    currenteditpos=6;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_6->setText(QString::number(postfaulttime,'g',6));
             }
            break;
        case 2:
            if(obj==ui->lineEdit)
            {
                   QString text=ui->lineEdit->text();
                   VoltageStart= text.toFloat();
                   currenteditpos=1;
                   vi_parameter_enter(currenteditpos);
                   ui->lineEdit->setText(QString::number(VoltageStart,'g',6));
            }
            else if(obj==ui->lineEdit_2)
            {
                   QString text=ui->lineEdit_2->text();
                   VoltageEnd= text.toFloat();
                   currenteditpos=2;
                   vi_parameter_enter(currenteditpos);
                   ui->lineEdit_2->setText(QString::number(VoltageEnd,'g',6));
            }
            else if(obj==ui->lineEdit_3)
             {
                    QString text=ui->lineEdit_3->text();
                    VoltageStep= text.toFloat();
                    currenteditpos=3;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_3->setText(QString::number(VoltageStep,'g',6));
             }
            else if(obj==ui->lineEdit_4)
             {
                    QString text=ui->lineEdit_4->text();
                    WaitTime= text.toFloat();
                    currenteditpos=4;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_4->setText(QString::number(WaitTime,'g',6));
             }
            else if(obj==ui->lineEdit_5)
             {
                    QString text=ui->lineEdit_5->text();
                    PostTime= text.toFloat();
                    currenteditpos=5;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_5->setText(QString::number(PostTime,'g',6));
             }
            else if(obj==ui->lineEdit_6)
             {
                    QString text=ui->lineEdit_6->text();
                    postfaulttime= text.toFloat();
                    currenteditpos=6;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_6->setText(QString::number(postfaulttime,'g',6));
             }
            break;
        case 3:
            if(obj==ui->lineEdit)
            {
                   QString text=ui->lineEdit->text();
                   FrequencyStart= text.toFloat();
                   currenteditpos=1;
                   vi_parameter_enter(currenteditpos);
                   ui->lineEdit->setText(QString::number(FrequencyStart,'g',6));
            }
            else if(obj==ui->lineEdit_2)
            {
                   QString text=ui->lineEdit_2->text();
                   FrequencyEnd= text.toFloat();
                   currenteditpos=2;
                   vi_parameter_enter(currenteditpos);
                   ui->lineEdit_2->setText(QString::number(FrequencyEnd,'g',6));
            }
            else if(obj==ui->lineEdit_3)
             {
                    QString text=ui->lineEdit_3->text();
                    FrequencyStep= text.toFloat();
                    currenteditpos=3;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_3->setText(QString::number(FrequencyStep,'g',6));
             }
            else if(obj==ui->lineEdit_4)
             {
                    QString text=ui->lineEdit_4->text();
                    WaitTime= text.toFloat();
                    currenteditpos=4;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_4->setText(QString::number(WaitTime,'g',6));
             }
            else if(obj==ui->lineEdit_5)
             {
                    QString text=ui->lineEdit_5->text();
                    PostTime= text.toFloat();
                    currenteditpos=5;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_5->setText(QString::number(PostTime,'g',6));
             }
            else if(obj==ui->lineEdit_6)
             {
                    QString text=ui->lineEdit_6->text();
                    postfaulttime= text.toFloat();
                    currenteditpos=6;
                    vi_parameter_enter(currenteditpos);
                    ui->lineEdit_6->setText(QString::number(postfaulttime,'g',6));
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





void Widget::on_radioButton_4_clicked()
{
    Faulttype=1;
    if(TestObject==1)
      {
        if(CurrentStart>MAX_I_VALUEDATA_AC)
        {
          CurrentStart=MAX_I_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
        else if(CurrentStart<0)
        {
          CurrentStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
      }
    else if(TestObject==2)
      {
        if(VoltageStart>MAX_V_VALUEDATA_AC)
        {
          VoltageStart=MAX_V_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
        else if(VoltageStart<0)
        {
          VoltageStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
      }
    if(TestObject==1)
       {
         if(CurrentEnd>MAX_I_VALUEDATA_AC)
         {
           CurrentEnd=MAX_I_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
         else if(CurrentEnd<0)
         {
           CurrentEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
       }
     else if(TestObject==2)
       {
         if(VoltageEnd>MAX_V_VALUEDATA_AC)
         {
           VoltageEnd=MAX_V_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
         else if(VoltageEnd<0)
         {
           VoltageEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
       }
    if(TestObject==1)
       {
        if(CurrentStep>fabs(CurrentStart-CurrentEnd))
        {
          CurrentStep=fabs(CurrentStart-CurrentEnd);
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
        else if(CurrentStep<0)
        {
          CurrentStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
       }
     else if(TestObject==2)
       {
        if(VoltageStep>fabs(VoltageStart-VoltageEnd))
        {
          VoltageStep=fabs(VoltageStart-VoltageEnd);
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
        else if(VoltageStep<0)
        {
          VoltageStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
       }
//     else if(TestObject==3)
//       {
//        if(FrequencyStep>fabs(FrequencyStart-FrequencyEnd))
//        {
//          FrequencyStep=fabs(FrequencyStart-FrequencyEnd);
//          ui->lineEdit_3->setText(QString::number(FrequencyStep));
//        }
//        else if(FrequencyStep<0)
//        {
//          FrequencyStep=0;//鍙傛暟闄愬埗
//          ui->lineEdit_3->setText(QString::number(FrequencyStep));
//        }
//       }


    Showfaulttype(Faulttype);
}

void Widget::on_radioButton_5_clicked()
{
    Faulttype=2;
    if(TestObject==1)
      {
        if(CurrentStart>MAX_I_VALUEDATA_AC)
        {
          CurrentStart=MAX_I_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
        else if(CurrentStart<0)
        {
          CurrentStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
      }
    else if(TestObject==2)
      {
        if(VoltageStart>MAX_V_VALUEDATA_AC)
        {
          VoltageStart=MAX_V_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
        else if(VoltageStart<0)
        {
          VoltageStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
      }
    if(TestObject==1)
       {
         if(CurrentEnd>MAX_I_VALUEDATA_AC)
         {
           CurrentEnd=MAX_I_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
         else if(CurrentEnd<0)
         {
           CurrentEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
       }
     else if(TestObject==2)
       {
         if(VoltageEnd>MAX_V_VALUEDATA_AC)
         {
           VoltageEnd=MAX_V_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
         else if(VoltageEnd<0)
         {
           VoltageEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
       }
    if(TestObject==1)
       {
        if(CurrentStep>fabs(CurrentStart-CurrentEnd))
        {
          CurrentStep=fabs(CurrentStart-CurrentEnd);
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
        else if(CurrentStep<0)
        {
          CurrentStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
       }
     else if(TestObject==2)
       {
        if(VoltageStep>fabs(VoltageStart-VoltageEnd))
        {
          VoltageStep=fabs(VoltageStart-VoltageEnd);
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
        else if(VoltageStep<0)
        {
          VoltageStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
       }
//     else if(TestObject==3)
//       {
//        if(FrequencyStep>fabs(FrequencyStart-FrequencyEnd))
//        {
//          FrequencyStep=fabs(FrequencyStart-FrequencyEnd);
//          ui->lineEdit_3->setText(QString::number(FrequencyStep));
//        }
//        else if(FrequencyStep<0)
//        {
//          FrequencyStep=0;//鍙傛暟闄愬埗
//          ui->lineEdit_3->setText(QString::number(FrequencyStep));
//        }
//       }

    Showfaulttype(Faulttype);
}

void Widget::on_radioButton_6_clicked()
{
    Faulttype=3;
    if(TestObject==1)
      {
        if(CurrentStart>MAX_I_VALUEDATA_AC)
        {
          CurrentStart=MAX_I_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
        else if(CurrentStart<0)
        {
          CurrentStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
      }
    else if(TestObject==2)
      {
        if(VoltageStart>MAX_V_VALUEDATA_AC)
        {
          VoltageStart=MAX_V_VALUEDATA_AC;
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
        else if(VoltageStart<0)
        {
          VoltageStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
      }
    if(TestObject==1)
       {
         if(CurrentEnd>MAX_I_VALUEDATA_AC)
         {
           CurrentEnd=MAX_I_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
         else if(CurrentEnd<0)
         {
           CurrentEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
       }
     else if(TestObject==2)
       {
         if(VoltageEnd>MAX_V_VALUEDATA_AC)
         {
           VoltageEnd=MAX_V_VALUEDATA_AC;
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
         else if(VoltageEnd<0)
         {
           VoltageEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
       }
    if(TestObject==1)
       {
        if(CurrentStep>fabs(CurrentStart-CurrentEnd))
        {
          CurrentStep=fabs(CurrentStart-CurrentEnd);
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
        else if(CurrentStep<0)
        {
          CurrentStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
       }
     else if(TestObject==2)
       {
        if(VoltageStep>fabs(VoltageStart-VoltageEnd))
        {
          VoltageStep=fabs(VoltageStart-VoltageEnd);
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
        else if(VoltageStep<0)
        {
          VoltageStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
       }
//     else if(TestObject==3)
//       {
//        if(FrequencyStep>fabs(FrequencyStart-FrequencyEnd))
//        {
//          FrequencyStep=fabs(FrequencyStart-FrequencyEnd);
//          ui->lineEdit_3->setText(QString::number(FrequencyStep));
//        }
//        else if(FrequencyStep<0)
//        {
//          FrequencyStep=0;//鍙傛暟闄愬埗
//          ui->lineEdit_3->setText(QString::number(FrequencyStep));
//        }
//       }
    Showfaulttype(Faulttype);
}

void Widget::on_radioButton_7_clicked()
{
    Faulttype=4;
    if(TestObject==1)
      {
        if(CurrentStart>MAX_I_VALUEDATA_AC*2)
        {
          CurrentStart=MAX_I_VALUEDATA_AC*2;
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
        else if(CurrentStart<0)
        {
          CurrentStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
      }
    else if(TestObject==2)
      {
        if(VoltageStart>MAX_V_VALUEDATA_AC*2)
        {
          VoltageStart=MAX_V_VALUEDATA_AC*2;
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
        else if(VoltageStart<0)
        {
          VoltageStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
      }
    if(TestObject==1)
       {
         if(CurrentEnd>MAX_I_VALUEDATA_AC*2)
         {
           CurrentEnd=MAX_I_VALUEDATA_AC*2;
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
         else if(CurrentEnd<0)
         {
           CurrentEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
       }
     else if(TestObject==2)
       {
         if(VoltageEnd>MAX_V_VALUEDATA_AC*2)
         {
           VoltageEnd=MAX_V_VALUEDATA_AC*2;
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
         else if(VoltageEnd<0)
         {
           VoltageEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
       }
    if(TestObject==1)
       {
        if(CurrentStep>fabs(CurrentStart-CurrentEnd))
        {
          CurrentStep=fabs(CurrentStart-CurrentEnd);
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
        else if(CurrentStep<0)
        {
          CurrentStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
       }
     else if(TestObject==2)
       {
        if(VoltageStep>fabs(VoltageStart-VoltageEnd))
        {
          VoltageStep=fabs(VoltageStart-VoltageEnd);
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
        else if(VoltageStep<0)
        {
          VoltageStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
       }
    Showfaulttype(Faulttype);
}

void Widget::on_radioButton_8_clicked()
{
    Faulttype=5;
    if(TestObject==1)
      {
        if(CurrentStart>MAX_I_VALUEDATA_AC*2)
        {
          CurrentStart=MAX_I_VALUEDATA_AC*2;
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
        else if(CurrentStart<0)
        {
          CurrentStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
      }
    else if(TestObject==2)
      {
        if(VoltageStart>MAX_V_VALUEDATA_AC*2)
        {
          VoltageStart=MAX_V_VALUEDATA_AC*2;
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
        else if(VoltageStart<0)
        {
          VoltageStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
      }
    if(TestObject==1)
       {
         if(CurrentEnd>MAX_I_VALUEDATA_AC*2)
         {
           CurrentEnd=MAX_I_VALUEDATA_AC*2;
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
         else if(CurrentEnd<0)
         {
           CurrentEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
       }
     else if(TestObject==2)
       {
         if(VoltageEnd>MAX_V_VALUEDATA_AC*2)
         {
           VoltageEnd=MAX_V_VALUEDATA_AC*2;
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
         else if(VoltageEnd<0)
         {
           VoltageEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
       }
    if(TestObject==1)
       {
        if(CurrentStep>fabs(CurrentStart-CurrentEnd))
        {
          CurrentStep=fabs(CurrentStart-CurrentEnd);
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
        else if(CurrentStep<0)
        {
          CurrentStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
       }
     else if(TestObject==2)
       {
        if(VoltageStep>fabs(VoltageStart-VoltageEnd))
        {
          VoltageStep=fabs(VoltageStart-VoltageEnd);
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
        else if(VoltageStep<0)
        {
          VoltageStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
       }
    Showfaulttype(Faulttype);
}

void Widget::on_radioButton_9_clicked()
{
    Faulttype=6;
    if(TestObject==1)
      {
        if(CurrentStart>MAX_I_VALUEDATA_AC*2)
        {
          CurrentStart=MAX_I_VALUEDATA_AC*2;
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
        else if(CurrentStart<0)
        {
          CurrentStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
      }
    else if(TestObject==2)
      {
        if(VoltageStart>MAX_V_VALUEDATA_AC*2)
        {
          VoltageStart=MAX_V_VALUEDATA_AC*2;
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
        else if(VoltageStart<0)
        {
          VoltageStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
      }
    if(TestObject==1)
       {
         if(CurrentEnd>MAX_I_VALUEDATA_AC*2)
         {
           CurrentEnd=MAX_I_VALUEDATA_AC*2;
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
         else if(CurrentEnd<0)
         {
           CurrentEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
       }
     else if(TestObject==2)
       {
         if(VoltageEnd>MAX_V_VALUEDATA_AC*2)
         {
           VoltageEnd=MAX_V_VALUEDATA_AC*2;
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
         else if(VoltageEnd<0)
         {
           VoltageEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
       }
    if(TestObject==1)
       {
        if(CurrentStep>fabs(CurrentStart-CurrentEnd))
        {
          CurrentStep=fabs(CurrentStart-CurrentEnd);
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
        else if(CurrentStep<0)
        {
          CurrentStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
       }
     else if(TestObject==2)
       {
        if(VoltageStep>fabs(VoltageStart-VoltageEnd))
        {
          VoltageStep=fabs(VoltageStart-VoltageEnd);
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
        else if(VoltageStep<0)
        {
          VoltageStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
       }
    Showfaulttype(Faulttype);
}



void Widget::on_radioButton_10_clicked()
{
    Faulttype=7;
    if(TestObject==1)
      {
        if(CurrentStart>MAX_I_VALUEDATA_AC*3)
        {
          CurrentStart=MAX_I_VALUEDATA_AC*3;
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
        else if(CurrentStart<0)
        {
          CurrentStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(CurrentStart));
        }
      }
    else if(TestObject==2)
      {
        if(VoltageStart>MAX_V_VALUEDATA_AC*3)
        {
          VoltageStart=MAX_V_VALUEDATA_AC*3;
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
        else if(VoltageStart<0)
        {
          VoltageStart=0;//鍙傛暟闄愬埗
          ui->lineEdit->setText(QString::number(VoltageStart));
        }
      }
    if(TestObject==1)
       {
         if(CurrentEnd>MAX_I_VALUEDATA_AC*3)
         {
           CurrentEnd=MAX_I_VALUEDATA_AC*3;
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
         else if(CurrentEnd<0)
         {
           CurrentEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(CurrentEnd));
         }
       }
     else if(TestObject==2)
       {
         if(VoltageEnd>MAX_V_VALUEDATA_AC*3)
         {
           VoltageEnd=MAX_V_VALUEDATA_AC*3;
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
         else if(VoltageEnd<0)
         {
           VoltageEnd=0;//鍙傛暟闄愬埗
           ui->lineEdit_2->setText(QString::number(VoltageEnd));
         }
       }
    if(TestObject==1)
       {
        if(CurrentStep>fabs(CurrentStart-CurrentEnd))
        {
          CurrentStep=fabs(CurrentStart-CurrentEnd);
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
        else if(CurrentStep<0)
        {
          CurrentStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(CurrentStep));
        }
       }
     else if(TestObject==2)
       {
        if(VoltageStep>fabs(VoltageStart-VoltageEnd))
        {
          VoltageStep=fabs(VoltageStart-VoltageEnd);
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
        else if(VoltageStep<0)
        {
          VoltageStep=0;//鍙傛暟闄愬埗
          ui->lineEdit_3->setText(QString::number(VoltageStep));
        }
       }
    Showfaulttype(Faulttype);
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
