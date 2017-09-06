#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QTableWidgetItem>
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>
#include "ComplexCaculate.h"
#include "math.h"
#include <unistd.h>

#include "qtranslator.h"
QTranslator *tor;
int current_language=2;  //1中文  2英文

extern TEMPRORARYDATA *temprorarydata;

float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

unsigned char result[70],receive_data[70];
double out_time,change_timedata=0;
float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;

int   input_par=0;
long auto_i=0,IRQcnt=0,auto_count=0,tice=0;
//中断程序的变量。
int   INPAR=0,seq_count,Sequencer,ResultPage,ResultFlag,DisResult,EndSequencer;
int   testcount=0,time_relay_testcount=0,auto_end=0;
int   cx,cy,cx_start,cx_mid,cx_end,increase,starttest=0,low,row,page;
int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC,EndTest;
//一段 二段三段 四段
bool   Zone1,Zone2,Zone3,Zone4,EqualizeQuotiety;
int   FaultType1,FaultType2,FaultType3,FaultType4,FaultType5,FaultType6,FaultType7;
int   VzOutput3U0;
float VzOutputAc,VzOutputDc;
float LogicResolution,SystemFrec;
//阻抗定值
float Impedance1,Impedance2,Impedance3,Impedance4;
//灵敏角
float DelicacyAngle1,DelicacyAngle2,DelicacyAngle3,DelicacyAngle4;
float FixupCurrent1,FixupCurrent2,FixupCurrent3,FixupCurrent4;
float FixupTime1,FixupTime2,FixupTime3,FixupTime4;


float prefaulttime,postfaulttime,Preparetime;

float EqualizeValue,TestPoint1,TestPoint2,TestPoint3;
//EqualizeValue 零序补偿系数
float PerFaultTime,PostFaultTime,FaultTime;
float ChangeValue,timeout,TripTime,TripTimeSum[63]={-1};
int exit_par,save_f;
//exit_par：退出程序标志。save_f：保存结果标志，为1表示有新的测试结果，可以保存，为0表示没有新的测试结果
char report_time[30],report_load[30];
float save_time;
int sys[4];
float SystemVolt;
char ParStr[20];  //ParStr:为参数输入时记录的输入参数的字符串。
float  V1PerFaultValue,V2PerFaultValue,V3PerFaultValue,I1PerFaultValue,I2PerFaultValue,I3PerFaultValue;
float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase,I1PerFaultPhase,I2PerFaultPhase,I3PerFaultPhase;
float  V1FaultValue,V2FaultValue,V3FaultValue,I1FaultValue,I2FaultValue,I3FaultValue;
float  V1FaultPhase,V2FaultPhase,V3FaultPhase,I1FaultPhase,I2FaultPhase,I3FaultPhase;
float  V1PostFaultValue,V2PostFaultValue,V3PostFaultValue,I1PostFaultValue,I2PostFaultValue,I3PostFaultValue;
float  V1PostFaultPhase,V2PostFaultPhase,V3PostFaultPhase,I1PostFaultPhase,I2PostFaultPhase,I3PostFaultPhase;
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;

float VabFaultValue,VbcFaultValue,VcaFaultValue;
float VabFaultPhase,VbcFaultPhase,VcaFaultPhase;




float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;




//电压 电流 频率粗 细调
float V_coarse_step=1,V_fine_step=0.01,I_coarse_step=1,I_fine_step=0.01,F_coarse_step=1,F_fine_step=0.1;
float R_coarse_step=1,R_fine_step=0.1,P_coarse_step=1,P_fine_step=0.1;
int HelpFlag=0;
int showpage=1;
float faultduration=10;

//int caculateflag=0;

int   dcouttype=1;

bool Busvolcon=true;//2016-6-8

float KLphase=0;

bool istransfer=false;

bool Threadrunstate=true;
extern TEMPRORARYDATA *temprorarydata;

bool SoftStartEndflag=false;

void Widget::init_par()
{
        Zone1=1; //段1
        Zone2=0; //段2
        Zone3=0;
        Zone4=0;
        EqualizeQuotiety=1;
       ////阻抗定值
        Impedance1=1;
        Impedance2=2;
        Impedance3=3;
        Impedance4=1;
        //灵敏度
        DelicacyAngle1=75;
        DelicacyAngle2=75;
        DelicacyAngle3=75;
        DelicacyAngle4=75;
        //额定电流
        FixupCurrent1=5;
        FixupCurrent2=5;
        FixupCurrent3=5;
        FixupCurrent4=5;
        //定值时间
        FixupTime1=0.5;
        FixupTime2=1;
        FixupTime3=3;
        FixupTime4=4;
        //零序补偿系数
        EqualizeValue=0.67;
        TestPoint1=0.95;
        TestPoint2=1.05;
        TestPoint3=0.75;
        //测试点
        FaultType1=1;
        FaultType2=0;
        FaultType3=0;
        FaultType4=0;
        FaultType5=0;
        FaultType6=0;
        FaultType7=0;
        //故障前时间
        PerFaultTime=15;
            //故障后时间
        PostFaultTime=1;
        V1DC=0;
        V2DC=0;
        V3DC=0;
        VzDC=0;
        I1DC=0;
        I2DC=0;
        I3DC=0;
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
        //fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,&SystemVolt,&LogicResolution,&PerFaultTime,&PostFaultTime,&dcouttype);
        fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
               &SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&Preparetime,&current_language);
        fclose(fp);

}

void Widget::sysinit()
{
      initshowresult();
    //段选择
       ui->checkBox_8->setChecked(Zone1);
       ui->checkBox_9->setChecked(Zone2);
       ui->checkBox_10->setChecked(Zone3);
       ui->checkBox_11->setChecked(Zone4);
     //阻抗定值
       ui->lineEdit_7->setText(QString::number(Impedance1));
       ui->lineEdit_8->setText(QString::number(Impedance2));
       ui->lineEdit_9->setText(QString::number(Impedance3));
       ui->lineEdit_10->setText(QString::number(Impedance4));
     //灵敏度
       ui->lineEdit_11->setText(QString::number(DelicacyAngle1));
       ui->lineEdit_12->setText(QString::number(DelicacyAngle2));
       ui->lineEdit_13->setText(QString::number(DelicacyAngle3));
       ui->lineEdit_14->setText(QString::number(DelicacyAngle4));
     //固定电流
       ui->lineEdit_15->setText(QString::number(FixupCurrent1));
       ui->lineEdit_16->setText(QString::number(FixupCurrent2));
       ui->lineEdit_17->setText(QString::number(FixupCurrent3));
       ui->lineEdit_18->setText(QString::number(FixupCurrent4));
    //时间定值
       ui->lineEdit_19->setText(QString::number(FixupTime1));
       ui->lineEdit_20->setText(QString::number(FixupTime2));
       ui->lineEdit_21->setText(QString::number(FixupTime3));
       ui->lineEdit_22->setText(QString::number(FixupTime4));



       //零序补偿系数
       ui->lineEdit->setText(QString::number(EqualizeValue));
       ui->lineEdit_23->setText(QString::number(KLphase));

       //测试点
       ui->lineEdit_2->setText(QString::number(TestPoint1));
       ui->lineEdit_3->setText(QString::number(TestPoint2));
       ui->lineEdit_4->setText(QString::number(TestPoint3));
       ui->groupBox_2->setEnabled(false);//默认数值，不能手动改变2016-5-17
       //故障类型
       ui->checkBox->setChecked(FaultType1);
       ui->checkBox_2->setChecked(FaultType2);
       ui->checkBox_3->setChecked(FaultType3);
       ui->checkBox_4->setChecked(FaultType4);
       ui->checkBox_5->setChecked(FaultType5);
       ui->checkBox_6->setChecked(FaultType6);
       ui->checkBox_7->setChecked(FaultType7);
       //故障时间设置
       ui->lineEdit_5->setText(QString::number(PerFaultTime));
       ui->lineEdit_6->setText(QString::number(PostFaultTime));



}


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
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //单个被选中
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->groupBox_2->setHidden(true);
    ui->tableWidget->setRowCount(7);
    ui->tableWidget->setColumnCount(3);
    //ui->label_11->setText("<font color=blue>"+QString("距离定值检验(2.2.1)")+"</font>");
    showFullScreen();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();
    ui->label_16->hide();
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
    ui->label_20->hide();

    QPixmap img(":/picture/hei.png");
    ui->label_13->setPixmap(img);
    ui->label_14->setPixmap(img);
    ui->label_15->setPixmap(img);
    ui->label_16->setPixmap(img);

    QStringList strlist1;
    strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
    ui->tableWidget->setVerticalHeaderLabels(strlist1);

    setFixedSize(800,480);

    //init_par();
    sysinit();



    QStringList strlist2;
    strlist2<<QString::number(TestPoint2,'g',5)<<QString::number(TestPoint1,'g',5)<<QString::number(-TestPoint3,'g',5);
    ui->tableWidget->setHorizontalHeaderLabels(strlist2);


    for(int j=0;j<63;j++)
     TripTimeSum[j]=-1;

    ui->checkBox->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_2->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_3->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_4->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_5->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_6->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_7->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_8->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_9->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_10->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_11->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_12->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    Busvolcon=true;
    if(current_language==2)
        Busvolcon=false;
    ui->checkBox_12->setChecked(Busvolcon);
    //ui->checkBox_12->setChecked(false/*Busvolcon*/);
    ui->checkBox_12->setHidden(true);

    ui->pushButton_2->hide();

    ui->tabWidget->setCurrentIndex(0);


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
                 ui->lineEdit_21->installEventFilter(this);
                 ui->lineEdit_22->installEventFilter(this);
                 ui->lineEdit_23->installEventFilter(this);


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
                 ui->lineEdit_21->setContextMenuPolicy(Qt::NoContextMenu);
                 ui->lineEdit_22->setContextMenuPolicy(Qt::NoContextMenu);
                 ui->lineEdit_23->setContextMenuPolicy(Qt::NoContextMenu);



 //ui->pushButton->setFocusPolicy(Qt::NoFocus);
 //ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
 //ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
 //ui->pushButton_6->setFocusPolicy(Qt::NoFocus);



    ui->tabWidget->setCurrentIndex(0);
    uartthread =new thread1;
    connect(uartthread,SIGNAL(SIG_binarystate(int)),SLOT(showdspstate(int)));
   // connect(uartthread,SIGNAL(SIG_SendHeader(QString)),this,SLOT(ShowHeader(QString)));
   // connect(uartthread,SIGNAL(SIG_Sendzoneshow(int)),this,SLOT(Showzone(int)));
    connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
    connect(uartthread,SIGNAL(SIG_testresult(int)),SLOT(showtestresult(int)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_sendwarning(int,int)),SLOT(showwarnint(int,int)));
    connect(uartthread,SIGNAL(SIG_changetab(int)),SLOT(changetab(int)));
     connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
     connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));
    uartthread->start();
}
void Widget::updateUI()
{
    ui->label_11->setText("<font color=blue>"+QString(tr("距离定值检验(2.2.1)"))+"</font>");
    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_9->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_6->setText(tr("测试"));
        ui->pushButton_7->setText(tr("计算更新"));
        ui->pushButton_4->setText(tr("上一页"));
        ui->pushButton_5->setText(tr("下一页"));

        ui->checkBox_8->setText(tr("1段"));
        ui->checkBox_9->setText(tr("2段"));
        ui->checkBox_10->setText(tr("3段"));
        ui->checkBox_11->setText(tr("4段"));

        ui->groupBox->setTitle(tr("参数"));
        ui->groupBox_3->setTitle(tr("故障类型"));
        ui->groupBox_4->setTitle(tr("时间设置(s)"));
        ui->groupBox_5->setTitle(tr("零序补偿系数"));

        ui->label_3->setText(tr("故障前:"));
        ui->label_4->setText(tr("故障后:"));

        ui->tabWidget->setTabText(0,tr("设置"));
        ui->tabWidget->setTabText(1,tr("计算结果"));
        ui->tabWidget->setTabText(2,tr("测试结果"));
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
        ui->label_13->setPixmap(img);
        ui->label_14->setPixmap(img);
        ui->label_15->setPixmap(img);
        ui->label_16->setPixmap(img);
        break;
    case 101: ui->label_13->setPixmap(pic); break;
    case 102: ui->label_14->setPixmap(pic); break;
    case 103: ui->label_15->setPixmap(pic); break;
    case 104: ui->label_16->setPixmap(pic); break;
    case 111: ui->label_13->setPixmap(img); break;
    case 112: ui->label_14->setPixmap(img); break;
    case 113: ui->label_15->setPixmap(img); break;
    case 114: ui->label_16->setPixmap(img); break;
    }
}

void Widget::changetab(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

void Widget::ShowPage(int page)
{
     ui->tableWidget->clear();
     switch(page)
     {
     case 1:
        {
         char temp[200];
         ui->tableWidget->setRowCount(7);
         ui->tableWidget->setColumnCount(3);
         if(current_language==1)
             sprintf(temp,"1段 z=%.3f",Impedance1);
         else
             sprintf(temp,"Zone1 z=%.3f",Impedance1);
         ui->label_9->setText(QString((temp)));
         QStringList strlist1;
         strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
         ui->tableWidget->setVerticalHeaderLabels(strlist1);
         QStringList strlist2;
         strlist2<<QString::number(TestPoint1,'g',5)<<QString::number(TestPoint2,'g',5)<<QString::number(-TestPoint3,'g',5);

         ui->tableWidget->setHorizontalHeaderLabels(strlist2);

         ui->tableWidget->setFixedWidth(550+60);
         for(int i=0;i<7;i++)
         for(int j=0;j<3;j++)
         ui->tableWidget->setItem(i,j,new QTableWidgetItem);

         int row;
         int col;
         for(int i=1;i<=21;i++)
         {
             row=(i-1)/3;
             col=(i-1)%3;
             if(TripTimeSum[i-1]==0)
                 ui->tableWidget->item(row,col)->setText(tr("未动作"));
             else if(TripTimeSum[i-1]==-1)
                 ui->tableWidget->item(row,col)->setText(tr("未测试"));
             else ui->tableWidget->item(row,col)->setText(QString::number(TripTimeSum[i-1],'g',6));

         }
         break;
      }
     case 2:
       {
         char temp[200];
         ui->tableWidget->setRowCount(7);
         ui->tableWidget->setColumnCount(2);
         if(current_language==1)
             sprintf(temp,"2段 z=%.3f",Impedance2);
         else
             sprintf(temp,"Zone2 z=%.3f",Impedance2);
         ui->label_9->setText(QString((temp)));
         QStringList strlist1;
         strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
         ui->tableWidget->setVerticalHeaderLabels(strlist1);
         QStringList strlist2;
         strlist2<<QString::number(TestPoint1,'g',5)<<QString::number(TestPoint2,'g',5);
         ui->tableWidget->setHorizontalHeaderLabels(strlist2);


         ui->tableWidget->setFixedWidth(300+50);
         for(int i=0;i<7;i++)
         for(int j=0;j<2;j++)
         ui->tableWidget->setItem(i,j,new QTableWidgetItem);

         int row;
         int col;
         for(int i=22;i<=(22+13);i++)
         {
             row=(i-22)/2;
             col=(i-22)%2;
             if(TripTimeSum[i-1]==0)
                 ui->tableWidget->item(row,col)->setText(tr("未动作"));
             else if(TripTimeSum[i-1]==-1)
                 ui->tableWidget->item(row,col)->setText(tr("未测试"));
             else ui->tableWidget->item(row,col)->setText(QString::number(TripTimeSum[i-1],'g',6));
         }
         break;
     }

     case 3:
      {   char temp[200];
         ui->tableWidget->setRowCount(7);
         ui->tableWidget->setColumnCount(2);
         if(current_language==1)
             sprintf(temp,"3段 z=%.3f",Impedance3);
         else
             sprintf(temp,"Zone3 z=%.3f",Impedance3);
         ui->label_9->setText(QString((temp)));
         QStringList strlist1;
         strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
         ui->tableWidget->setVerticalHeaderLabels(strlist1);
         QStringList strlist2;
         strlist2<<QString::number(TestPoint1,'g',5)<<QString::number(TestPoint2,'g',5);
         ui->tableWidget->setHorizontalHeaderLabels(strlist2);
         ui->tableWidget->setFixedWidth(300+50);
         for(int i=0;i<7;i++)
         for(int j=0;j<2;j++)
         ui->tableWidget->setItem(i,j,new QTableWidgetItem);
         int row;
         int col;
         for(int i=36;i<=(36+13);i++)
         {
             row=(i-36)/2;
             col=(i-36)%2;
             if(TripTimeSum[i-1]==0)
                 ui->tableWidget->item(row,col)->setText(tr("未动作"));
             else if(TripTimeSum[i-1]==-1)
                 ui->tableWidget->item(row,col)->setText(tr("未测试"));
             else ui->tableWidget->item(row,col)->setText(QString::number(TripTimeSum[i-1],'g',6));
         }
      break;
     }
case 4:
        {
         char temp[200];
         ui->tableWidget->setRowCount(7);
         ui->tableWidget->setColumnCount(2);
         if(current_language==1)
             sprintf(temp,"4段 z=%.3f",Impedance4);
         else
             sprintf(temp,"Zone4 z=%.3f",Impedance4);
         ui->label_9->setText(QString((temp)));
         QStringList strlist1;
         strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
         ui->tableWidget->setVerticalHeaderLabels(strlist1);
         QStringList strlist2;
         strlist2<<QString::number(TestPoint1,'g',5)<<QString::number(TestPoint2,'g',5);
         ui->tableWidget->setHorizontalHeaderLabels(strlist2);
         ui->tableWidget->setFixedWidth(300+50);

         for(int i=0;i<7;i++)
         for(int j=0;j<2;j++)
         ui->tableWidget->setItem(i,j,new QTableWidgetItem);
         int row;
         int col;
         for(int i=50;i<=(50+13);i++)
         {
             row=(i-50)/2;
             col=(i-50)%2;
             if(TripTimeSum[i-1]==0)
                 ui->tableWidget->item(row,col)->setText(tr("未动作"));
             else if(TripTimeSum[i-1]==-1)
                 ui->tableWidget->item(row,col)->setText(tr("未测试"));
            else ui->tableWidget->item(row,col)->setText(QString::number(TripTimeSum[i-1],'g',6));
         }
        break;
        }
     }
}

void Widget::initshowresult(void)
{

    if(Zone1==1)
    {
        ui->tableWidget->setRowCount(7);
        ui->tableWidget->setColumnCount(3);
        QString msg2 = QString("%1").arg(Impedance1);
        if(current_language==1)
        {
            std::string zhongwen = "1段 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        else if(current_language==2)
        {
            std::string zhongwen = "Zone1 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        QStringList strlist1;
        strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
        ui->tableWidget->setVerticalHeaderLabels(strlist1);
        QStringList strlist2;
        strlist2<<QString::number(TestPoint2,'g',5)<<QString::number(TestPoint1,'g',5)<<QString::number(-TestPoint3,'g',5);
        ui->tableWidget->setHorizontalHeaderLabels(strlist2);


        ui->tableWidget->setColumnWidth(0,150);
        ui->tableWidget->setColumnWidth(1,150);
        ui->tableWidget->setColumnWidth(2,250);

        ui->tableWidget->setFixedWidth(550+50);


    }
    else if(Zone1==0&&Zone2==1)
    {
        ui->tableWidget->setRowCount(7);
        ui->tableWidget->setColumnCount(2);
        QString msg2 = QString("%1").arg(Impedance2);
        if(current_language==1)
        {
            std::string zhongwen = "2段 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        else if(current_language==2)
        {
            std::string zhongwen = "Zone2 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        QStringList strlist1;
        strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
        ui->tableWidget->setVerticalHeaderLabels(strlist1);
        QStringList strlist2;
        strlist2<<QString::number(TestPoint2,'g',5)<<QString::number(TestPoint1,'g',5);
        ui->tableWidget->setHorizontalHeaderLabels(strlist2);

        for(int i=0;i<2;i++)
            ui->tableWidget->setColumnWidth(i,150);
         ui->tableWidget->setFixedWidth(300+50);

    }
    else if(Zone1==0&&Zone2==0&&Zone3==1)
    {
        ui->tableWidget->setRowCount(7);
        ui->tableWidget->setColumnCount(2);
        QString msg2 = QString("%1").arg(Impedance3);
        if(current_language==1)
        {
            std::string zhongwen = "3段 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        else if(current_language==2)
        {
            std::string zhongwen = "Zone3 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        QStringList strlist1;
        strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
        ui->tableWidget->setVerticalHeaderLabels(strlist1);
        QStringList strlist2;
        strlist2<<QString::number(TestPoint2,'g',5)<<QString::number(TestPoint1,'g',5);
        ui->tableWidget->setHorizontalHeaderLabels(strlist2);

        for(int i=0;i<2;i++)
            ui->tableWidget->setColumnWidth(i,150);
        ui->tableWidget->setFixedWidth(300+50);
    }
    else if(Zone1==0&&Zone2==0&&Zone3==0&&Zone4==1)
    {
        ui->tableWidget->setRowCount(7);
        ui->tableWidget->setColumnCount(2);
        QString msg2 = QString("%1").arg(Impedance4);
        if(current_language==1)
        {
            std::string zhongwen = "4段 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        else if(current_language==2)
        {
            std::string zhongwen = "Zone4 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        QStringList strlist1;
        strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
        ui->tableWidget->setVerticalHeaderLabels(strlist1);
        QStringList strlist2;
        strlist2<<QString::number(TestPoint2,'g',5)<<QString::number(TestPoint1,'g',5);
        ui->tableWidget->setHorizontalHeaderLabels(strlist2);

        for(int i=0;i<2;i++)
            ui->tableWidget->setColumnWidth(i,150);
        ui->tableWidget->setFixedWidth(300+50);
    }
}

void Widget::showtestresult(int num)
{

    if(num==0)
   { ui->tableWidget->clear();
    initshowresult();
    return;
   }

    ui->tableWidget->clear();
    if(num>=1&&num<=21)
   {
        showpage=1;
        ui->tableWidget->setRowCount(7);
        ui->tableWidget->setColumnCount(3);
        QString msg2 = QString("%1").arg(Impedance1);
        if(current_language==1)
        {
            std::string zhongwen = "1段 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        else if(current_language==2)
        {
            std::string zhongwen = "Zone1 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        QStringList strlist1;
        strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
        ui->tableWidget->setVerticalHeaderLabels(strlist1);
        QStringList strlist2;
        strlist2<<QString::number(TestPoint2,'g',5)<<QString::number(TestPoint1,'g',5)<<QString::number(-TestPoint3,'g',5);

        ui->tableWidget->setHorizontalHeaderLabels(strlist2);

        for(int i=0;i<7;i++)
        for(int j=0;j<3;j++)
        ui->tableWidget->setItem(i,j,new QTableWidgetItem);

        ui->tableWidget->setColumnWidth(0,150);
        ui->tableWidget->setColumnWidth(1,150);
        ui->tableWidget->setColumnWidth(2,250);

        ui->tableWidget->setFixedWidth(550+50);



        int row;
        int col;
        for(int i=1;i<=num;i++)
        {
            row=(i-1)/3;
            col=(i-1)%3;
            if(TripTimeSum[i-1]==0)
                ui->tableWidget->item(row,col)->setText(tr("未动作"));
            else if(TripTimeSum[i-1]==-1)
            ui->tableWidget->item(row,col)->setText(tr("未测试"));
            else
            ui->tableWidget->item(row,col)->setText(QString::number(TripTimeSum[i-1],'g',6));

        }
   }
    else if(num>=22&&num<=35)
    {
        showpage=2;
        ui->tableWidget->setRowCount(7);
        ui->tableWidget->setColumnCount(2);
        QString msg2 = QString("%1").arg(Impedance2);
        if(current_language==1)
        {
            std::string zhongwen = "2段 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        else if(current_language==2)
        {
            std::string zhongwen = "Zone2 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        QStringList strlist1;
        strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
        ui->tableWidget->setVerticalHeaderLabels(strlist1);
        QStringList strlist2;
        strlist2<<QString::number(TestPoint2,'g',5)<<QString::number(TestPoint1,'g',5);
        ui->tableWidget->setHorizontalHeaderLabels(strlist2);
        ui->tableWidget->setRowCount(7);
        ui->tableWidget->setColumnCount(2);

      for(int i=0;i<2;i++)
      ui->tableWidget->setColumnWidth(i,150);

      ui->tableWidget->setFixedWidth(300+50);


        for(int i=0;i<7;i++)
        for(int j=0;j<2;j++)
        ui->tableWidget->setItem(i,j,new QTableWidgetItem);

        int row;
        int col;
        for(int i=22;i<=num;i++)
        {
            row=(i-22)/2;
            col=(i-22)%2;
            if(TripTimeSum[i-1]==0)
                ui->tableWidget->item(row,col)->setText(tr("未动作"));
            else   if(TripTimeSum[i-1]==-1)
            ui->tableWidget->item(row,col)->setText(tr("未测试"));
            else
            ui->tableWidget->item(row,col)->setText(QString::number(TripTimeSum[i-1],'g',6));


        }
    }
    else if(num>=36&&num<=49)
    {
        showpage=3;
        ui->tableWidget->setRowCount(7);
        ui->tableWidget->setColumnCount(2);
        QString msg2 = QString("%1").arg(Impedance3);
        if(current_language==1)
        {
            std::string zhongwen = "3段 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        else if(current_language==2)
        {
            std::string zhongwen = "Zone3 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        QStringList strlist1;
        strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
        ui->tableWidget->setVerticalHeaderLabels(strlist1);
        QStringList strlist2;
        strlist2<<QString::number(TestPoint2,'g',5)<<QString::number(TestPoint1,'g',5);
        ui->tableWidget->setHorizontalHeaderLabels(strlist2);
        for(int i=0;i<2;i++)
        ui->tableWidget->setColumnWidth(i,150);

        ui->tableWidget->setFixedWidth(300+50);

        for(int i=0;i<7;i++)
        for(int j=0;j<2;j++)
        ui->tableWidget->setItem(i,j,new QTableWidgetItem);

        int row;
        int col;
        for(int i=36;i<=num;i++)
        {
            row=(i-36)/2;
            col=(i-36)%2;
            if(TripTimeSum[i-1]==0)
                ui->tableWidget->item(row,col)->setText(tr("未动作"));
            else  if(TripTimeSum[i-1]==-1)
                ui->tableWidget->item(row,col)->setText(tr("未测试"));
            else
                ui->tableWidget->item(row,col)->setText(QString::number(TripTimeSum[i-1],'g',6));
        }
    }
    else if(num>=50&&num<=63)
    {
        showpage=4;
        ui->tableWidget->setRowCount(7);
        ui->tableWidget->setColumnCount(2);
        QString msg2 = QString("%1").arg(Impedance4);
        if(current_language==1)
        {
            std::string zhongwen = "4段 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        else if(current_language==2)
        {
            std::string zhongwen = "Zone4 z=";
            QString msg1 = QString::fromLocal8Bit(zhongwen.c_str());
            QString msg = msg1 + msg2;
            ui->label_9->setText(msg);
        }
        QStringList strlist1;
        strlist1<<tr("A-E")<<tr("B-E")<<tr("C-E")<<tr("A-B")<<tr("B-C")<<tr("C-A")<<tr("A-B-C");
        ui->tableWidget->setVerticalHeaderLabels(strlist1);
        QStringList strlist2;
        strlist2<<QString::number(TestPoint2,'g',5)<<QString::number(TestPoint1,'g',5);
        ui->tableWidget->setHorizontalHeaderLabels(strlist2);

        for(int i=0;i<2;i++)
           ui->tableWidget->setColumnWidth(i,150);

           ui->tableWidget->setFixedWidth(300+50);

        for(int i=0;i<7;i++)
        for(int j=0;j<2;j++)
        ui->tableWidget->setItem(i,j,new QTableWidgetItem);

        int row;
        int col;
        for(int i=50;i<=num;i++)
        {
            row=(i-50)/2;
            col=(i-50)%2;
            if(TripTimeSum[i-1]==0)
                 ui->tableWidget->item(row,col)->setText(tr("未动作"));
            else if(TripTimeSum[i-1]==-1)
                 ui->tableWidget->item(row,col)->setText(tr("未测试"));
            else ui->tableWidget->item(row,col)->setText(QString::number(TripTimeSum[i-1],'g',6));
        }
    }
}

void Widget::showwarnint(int sectnum,int num)
{

//    if(sectnum==0&&num==0)
//    {
//       return;
//    }
//   char *str[4]={"zone 1","zone 2","zone 3","zone 4"};
//   char temp1[400];
//   char temp2[100];
//   if(num==1)
//       sprintf(temp1,"The %s zone of the voltage is more than %.3fV,",str[sectnum-1],SystemVolt);
//   else   if(num==2)
//       sprintf(temp1,"zone of the voltage is else than %.3fV,",SystemVolt/2);
//   sprintf(temp2,"%s please change the fixed current value.",temp1);

//   QMessageBox::warning(this,"Warning",temp2,QMessageBox::Ok);
  if(sectnum==0&&num==0)
  {
        return;
  }
  if(current_language==1)
  {
    char *str[4]={"1段","2段","3段","4段"};
    char temp1[400];
    char temp2[100];
    QString msg;
    if(num==1)
        sprintf(temp1,"%s故障电压超过系统电压,",str[sectnum-1]);
    else   if(num==2)
        sprintf(temp1,"%s故障电压低于系统电压的一半,",str[sectnum-1]);
    sprintf(temp2,"%s 请改变电流值.",temp1);
    msg=QString(temp2);
    //QMessageBox::warning(this,QString::fromUtf8("警告"),QString::fromUtf8(temp2),QMessageBox::Ok);

    Warning *warning=new Warning;
    warning->raise();
    warning->activateWindow();
    warning->setModal(true);
    warning->Setwarning(1,msg);
    warning->show();
  }
  else if(current_language==2)
  {
      char *str[4]={"zone 1","zone 2","zone 3","zone 4"};
      char temp1[400];
      char temp2[100];
      QString msg;
      if(num==1)
          sprintf(temp1,"The %s zone of the voltage is more than %.3fV,",str[sectnum-1],SystemVolt);
      else   if(num==2)
          sprintf(temp1,"zone of the voltage is else than %.3fV,",SystemVolt/2);
      sprintf(temp2,"%s please change the fixed current value.",temp1);
      msg=QString(temp2);
      Warning *warning=new Warning;
      warning->raise();
      warning->activateWindow();
      warning->setModal(true);
      warning->Setwarning(1,msg);
      warning->show();
  }
}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_9_clicked() //保存
{
if(save_f==0)
  return;
  save_f=0;
   if(current_language==1)
   {
                FILE *fp;
                char str[512],str1[512];
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

                strcpy(strbuf,"距离定值检验");
                fprintf(fp,"%s\r\n",strbuf);
        if(Zone1==1)
          {
            strcpy(strbuf,"1段: Z=");
            fprintf(fp,"%s",strbuf);
            gcvt(Impedance1 ,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType1==1)
              {
                strcpy(strbuf,"故障类型: A-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点: ");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[0]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[0] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[1]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[1] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:-");
                gcvt(TestPoint3 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[2]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[2] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType1==0)
              {
                strcpy(strbuf,"故障类型:A-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType2==1)
              {
                strcpy(strbuf,"故障类型:B-E");
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[3]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[3] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[4]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[4] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点: - ");
                gcvt(TestPoint3 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[5]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[5] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType2==0)
              {
                strcpy(strbuf,"故障类型:B-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType3==1)
              {
                strcpy(strbuf,"故障类型:C-E");
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[6]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[6] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[7]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[7] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点: - ");
                gcvt(TestPoint3,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[8]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[8] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType3==0)
              {
                strcpy(strbuf,"故障类型:C-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType4==1)
              {
                strcpy(strbuf,"故障类型:A-B");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[9]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[9] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[10]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[10] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点: - ");
                gcvt(TestPoint3,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[11]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[11] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType4==0)
              {
                strcpy(strbuf,"故障类型:A-B，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType5==1)
              {
                strcpy(strbuf,"故障类型:B-C");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[12]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[12] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[13]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[13] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点: - ");
                gcvt(TestPoint3 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[14]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[14] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType5==0)
              {
                strcpy(strbuf,"故障类型:B-C，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType6==1)
              {
                strcpy(strbuf,"故障类型:C-A");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[15]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[15] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[16]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[16] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点: - ");
                gcvt(TestPoint3 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[17]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[17] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType6==0)
              {
                strcpy(strbuf,"故障类型:C-A，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType7==1)
              {
                strcpy(strbuf,"故障类型:A-B-C");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[18]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[18] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[19]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[19] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点: - ");
                gcvt(TestPoint3 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[20]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[20] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType7==0)
              {
                strcpy(strbuf,"故障类型:A-B-C，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }
          }
        else if(Zone1==0)
          {
            strcpy(strbuf,"1段: 未测试");
            fprintf(fp,"%s\r\n",strbuf);
          }

        strcpy(strbuf,"/************************************************/");
        fprintf(fp,"%s\r\n",strbuf);

        if(Zone2==1)
          {
            strcpy(strbuf,"2段: Z=");
            fprintf(fp,"%s",strbuf);
            gcvt(Impedance2 ,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType1==1)
              {
                strcpy(strbuf,"故障类型:A-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[21]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[21] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[22]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[22] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType1==0)
              {
                strcpy(strbuf,"故障类型:A-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType2==1)
              {
                strcpy(strbuf,"故障类型:B-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[23]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[23] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[24]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[24] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType2==0)
              {
                strcpy(strbuf,"故障类型:B-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType3==1)
              {
                strcpy(strbuf,"故障类型:C-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[25]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[25] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[26]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[26] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType3==0)
              {
                strcpy(strbuf,"故障类型:C-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType4==1)
              {
                strcpy(strbuf,"故障类型:A-B");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[27]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[27] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[28]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[28] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType4==0)
              {
                strcpy(strbuf,"故障类型:A-B，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType5==1)
              {
                strcpy(strbuf,"故障类型:B-C");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[29]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[29] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[30]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[30] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType5==0)
              {
                strcpy(strbuf,"故障类型:B-C，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType6==1)
              {
                strcpy(strbuf,"故障类型:C-A");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[31]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[31] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[32]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[32] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType6==0)
              {
                strcpy(strbuf,"故障类型:C-A，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType7==1)
              {
                strcpy(strbuf,"故障类型:A-B-C");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[33]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[33] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[34]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[34] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType7==0)
              {
                strcpy(strbuf,"故障类型:A-B-C，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }
          }
        else if(Zone2==0)
          {
            strcpy(strbuf,"2段: 未测试");
            fprintf(fp,"%s\r\n",strbuf);
          }
        strcpy(strbuf,"/************************************************/");
        fprintf(fp,"%s\r\n",strbuf);

        if(Zone3==1)
          {
            strcpy(strbuf,"3段: Z=");
            fprintf(fp,"%s",strbuf);
            gcvt(Impedance3 ,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType1==1)
              {
                strcpy(strbuf,"故障类型:A-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[35]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[35] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[36]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[36] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType1==0)
              {
                strcpy(strbuf,"故障类型:A-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType2==1)
              {
                strcpy(strbuf,"故障类型:B-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[37]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[37] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[38]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[38] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType2==0)
              {
                strcpy(strbuf,"故障类型:B-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType3==1)
              {
                strcpy(strbuf,"故障类型:C-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[39]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[39] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[40]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[40] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType3==0)
              {
                strcpy(strbuf,"故障类型:C-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType4==1)
              {
                strcpy(strbuf,"故障类型:A-B");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[41]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[41] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[42]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[42] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType4==0)
              {
                strcpy(strbuf,"故障类型:A-B，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType5==1)
              {
                strcpy(strbuf,"故障类型:B-C");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[43]==0)


                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[43] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[44]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[44] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType5==0)
              {
                strcpy(strbuf,"故障类型:B-C，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType6==1)
              {
                strcpy(strbuf,"故障类型:C-A");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[45]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[45] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[46]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[46] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType6==0)
              {
                strcpy(strbuf,"故障类型:C-A，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType7==1)
              {
                strcpy(strbuf,"故障类型:A-B-C");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[47]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[47] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[48]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[48] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType7==0)
              {
                strcpy(strbuf,"故障类型:A-B-C，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }
          }
        else if(Zone3==0)
          {
            strcpy(strbuf,"3段: 未测试");
            fprintf(fp,"%s\r\n",strbuf);
          }
        strcpy(strbuf,"/************************************************/");
        fprintf(fp,"%s\r\n",strbuf);

        if(Zone4==1)
          {
            strcpy(strbuf,"4段:Z=");
            fprintf(fp,"%s",strbuf);
            gcvt(Impedance4 ,7,strbuf);
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType1==1)
              {
                strcpy(strbuf,"故障类型:A-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[49]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[49] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[50]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[50] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType1==0)
              {
                strcpy(strbuf,"故障类型:A-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType2==1)
              {
                strcpy(strbuf,"故障类型:B-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[51]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[51] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[52]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[52] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType2==0)
              {
                strcpy(strbuf,"故障类型:B-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType3==1)
              {
                strcpy(strbuf,"故障类型:C-E");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[53]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[53] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[54]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[54] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType3==0)
              {
                strcpy(strbuf,"故障类型:C-E，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType4==1)
              {
                strcpy(strbuf,"故障类型:A-B");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[55]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[55] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[56]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[56] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType4==0)
              {
                strcpy(strbuf,"故障类型:A-B，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType5==1)
              {
                strcpy(strbuf,"故障类型:B-C");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[57]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[57] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[58]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[58] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType5==0)
              {
                strcpy(strbuf,"故障类型:B-C，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType6==1)
              {
                strcpy(strbuf,"故障类型:C-A");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[59]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[59] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[60]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[60] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType6==0)
              {
                strcpy(strbuf,"故障类型:C-A，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }

            strcpy(strbuf,"-----------------------------------");
            fprintf(fp,"%s\r\n",strbuf);

            if(FaultType7==1)
              {
                strcpy(strbuf,"故障类型:A-B-C");
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"测试点:");
                gcvt(TestPoint2 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[61]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[61] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);

                strcpy(strbuf,"测试点:");
                gcvt(TestPoint1 ,7,str);
                strcat(strbuf,str);
                strcpy(str1,"Z");
                strcat(strbuf,str1);
                fprintf(fp,"%s\r\n",strbuf);
                strcpy(strbuf,"动作时间(s):");
                fprintf(fp,"%s",strbuf);
                if(TripTimeSum[62]==0)
                  strcpy(strbuf,"未动作");
                else
                  gcvt(TripTimeSum[62] ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
              }
            else if(FaultType7==0)
              {
                strcpy(strbuf,"故障类型:A-B-C，未测试");
                fprintf(fp,"%s\r\n",strbuf);
              }
          }
         else if(Zone4==0)
          {
            strcpy(strbuf,"4段: 未测试");
            fprintf(fp,"%s\r\n",strbuf);
          }
        fclose(fp);
        system("sync");
   }
   else if(current_language==2)
   {
       FILE *fp;
       char str[512],str1[512];
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
       strcpy(strbuf,"Distance test result");
       fprintf(fp,"%s\r\n",strbuf);

       if(Zone1==1)
    {
     strcpy(strbuf,"zone 1: Z=");
     fprintf(fp,"%s",strbuf);
     gcvt(Impedance1 ,7,strbuf);
     fprintf(fp,"%s\r\n",strbuf);

     if(FaultType1==1)
     {
       strcpy(strbuf,"Fault type: A-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point: ");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[0]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[0] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[1]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[1] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:-");
       gcvt(TestPoint3 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[2]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[2] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
     else if(FaultType1==0)
     {
       strcpy(strbuf,"Fault type:A-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

     strcpy(strbuf,"-----------------------------------");
     fprintf(fp,"%s\r\n",strbuf);

     if(FaultType2==1)
     {
       strcpy(strbuf,"Fault type:B-E");
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[3]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[3] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[4]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[4] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point: - ");
       gcvt(TestPoint3 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[5]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[5] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
     else if(FaultType2==0)
     {
       strcpy(strbuf,"Fault type:B-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

     strcpy(strbuf,"-----------------------------------");
     fprintf(fp,"%s\r\n",strbuf);

     if(FaultType3==1)
     {
       strcpy(strbuf,"Fault type:C-E");
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[6]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[6] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[7]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[7] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point: - ");
       gcvt(TestPoint3,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[8]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[8] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
     else if(FaultType3==0)
     {
       strcpy(strbuf,"Fault type:C-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

     strcpy(strbuf,"-----------------------------------");
     fprintf(fp,"%s\r\n",strbuf);

     if(FaultType4==1)
     {
       strcpy(strbuf,"Fault type:A-B");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[9]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[9] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[10]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[10] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point: - ");
       gcvt(TestPoint3,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[11]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[11] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
     else if(FaultType4==0)
     {
       strcpy(strbuf,"Fault type:A-B，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

     strcpy(strbuf,"-----------------------------------");
     fprintf(fp,"%s\r\n",strbuf);

     if(FaultType5==1)
     {
       strcpy(strbuf,"Fault type:B-C");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[12]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[12] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[13]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[13] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point: - ");
       gcvt(TestPoint3 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[14]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[14] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
     else if(FaultType5==0)
     {
       strcpy(strbuf,"Fault type:B-C，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

     strcpy(strbuf,"-----------------------------------");
     fprintf(fp,"%s\r\n",strbuf);

     if(FaultType6==1)
     {
       strcpy(strbuf,"Fault type:C-A");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[15]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[15] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[16]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[16] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point: - ");
       gcvt(TestPoint3 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[17]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[17] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
     else if(FaultType6==0)
     {
       strcpy(strbuf,"Fault type:C-A，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

     strcpy(strbuf,"-----------------------------------");
     fprintf(fp,"%s\r\n",strbuf);

     if(FaultType7==1)
     {
       strcpy(strbuf,"Fault type:A-B-C");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[18]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[18] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[19]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[19] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point: - ");
       gcvt(TestPoint3 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[20]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[20] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
     else if(FaultType7==0)
     {
       strcpy(strbuf,"Fault type:A-B-C，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }
 }
       else if(Zone1==0)
    {
     strcpy(strbuf,"Zone 1: No Test");
     fprintf(fp,"%s\r\n",strbuf);
    }

       strcpy(strbuf,"/************************************************/");
       fprintf(fp,"%s\r\n",strbuf);

       if(Zone2==1)
 {
   strcpy(strbuf,"Zone 2: Z=");
   fprintf(fp,"%s",strbuf);
   gcvt(Impedance2 ,7,strbuf);
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType1==1)
     {
       strcpy(strbuf,"Fault type:A-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[21]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[21] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[22]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[22] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType1==0)
     {
       strcpy(strbuf,"Fault type:A-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType2==1)
     {
       strcpy(strbuf,"Fault type:B-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[23]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[23] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[24]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[24] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType2==0)
     {
       strcpy(strbuf,"Fault type:B-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType3==1)
     {
       strcpy(strbuf,"Fault type:C-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[25]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[25] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[26]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[26] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType3==0)
     {
       strcpy(strbuf,"Fault type:C-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType4==1)
     {
       strcpy(strbuf,"Fault type:A-B");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[27]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[27] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[28]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[28] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType4==0)
     {
       strcpy(strbuf,"Fault type:A-B，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType5==1)
     {
       strcpy(strbuf,"Fault type:B-C");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[29]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[29] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[30]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[30] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType5==0)
     {
       strcpy(strbuf,"Fault type:B-C，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType6==1)
     {
       strcpy(strbuf,"Fault type:C-A");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[31]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[31] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[32]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[32] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType6==0)
     {
       strcpy(strbuf,"Fault type:C-A，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType7==1)
     {
       strcpy(strbuf,"Fault type:A-B-C");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[33]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[33] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[34]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[34] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType7==0)
     {
       strcpy(strbuf,"Fault type:A-B-C，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }
 }
       else if(Zone2==0)
 {
   strcpy(strbuf,"Zone 2: No Test");
   fprintf(fp,"%s\r\n",strbuf);
 }
       strcpy(strbuf,"/************************************************/");
       fprintf(fp,"%s\r\n",strbuf);

        if(Zone3==1)
 {
   strcpy(strbuf,"Zone 3: Z=");
   fprintf(fp,"%s",strbuf);
   gcvt(Impedance3 ,7,strbuf);
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType1==1)
     {
       strcpy(strbuf,"Fault type:A-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[35]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[35] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[36]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[36] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType1==0)
     {
       strcpy(strbuf,"Fault type:A-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType2==1)
     {
       strcpy(strbuf,"Fault type:B-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[37]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[37] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[38]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[38] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType2==0)
     {
       strcpy(strbuf,"Fault type:B-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType3==1)
     {
       strcpy(strbuf,"Fault type:C-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[39]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[39] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[40]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[40] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType3==0)
     {
       strcpy(strbuf,"Fault type:C-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType4==1)
     {
       strcpy(strbuf,"Fault type:A-B");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[41]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[41] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[42]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[42] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType4==0)
     {
       strcpy(strbuf,"Fault type:A-B，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType5==1)
     {
       strcpy(strbuf,"Fault type:B-C");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[43]==0)


         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[43] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[44]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[44] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType5==0)
     {
       strcpy(strbuf,"Fault type:B-C，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType6==1)
     {
       strcpy(strbuf,"Fault type:C-A");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[45]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[45] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[46]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[46] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType6==0)
     {
       strcpy(strbuf,"Fault type:C-A，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType7==1)
     {
       strcpy(strbuf,"Fault type:A-B-C");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[47]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[47] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[48]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[48] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType7==0)
     {
       strcpy(strbuf,"Fault type:A-B-C，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }
 }
        else if(Zone3==0)
 {
   strcpy(strbuf,"Zone 3: No Test");
   fprintf(fp,"%s\r\n",strbuf);
 }
        strcpy(strbuf,"/************************************************/");
        fprintf(fp,"%s\r\n",strbuf);

        if(Zone4==1)
 {
   strcpy(strbuf,"Zone 4:Z=");
   fprintf(fp,"%s",strbuf);
   gcvt(Impedance4 ,7,strbuf);
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType1==1)
     {
       strcpy(strbuf,"Fault type:A-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[49]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[49] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[50]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[50] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType1==0)
     {
       strcpy(strbuf,"Fault type:A-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType2==1)
     {
       strcpy(strbuf,"Fault type:B-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[51]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[51] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[52]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[52] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType2==0)
     {
       strcpy(strbuf,"Fault type:B-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType3==1)
     {
       strcpy(strbuf,"Fault type:C-E");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[53]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[53] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[54]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[54] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType3==0)
     {
       strcpy(strbuf,"Fault type:C-E，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType4==1)
     {
       strcpy(strbuf,"Fault type:A-B");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[55]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[55] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[56]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[56] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType4==0)
     {
       strcpy(strbuf,"Fault type:A-B，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType5==1)
     {
       strcpy(strbuf,"Fault type:B-C");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[57]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[57] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[58]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[58] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType5==0)
     {
       strcpy(strbuf,"Fault type:B-C，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType6==1)
     {
       strcpy(strbuf,"Fault type:C-A");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[59]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[59] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[60]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[60] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType6==0)
     {
       strcpy(strbuf,"Fault type:C-A，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }

   strcpy(strbuf,"-----------------------------------");
   fprintf(fp,"%s\r\n",strbuf);

   if(FaultType7==1)
     {
       strcpy(strbuf,"Fault type:A-B-C");
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Test point:");
       gcvt(TestPoint2 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[61]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[61] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);

       strcpy(strbuf,"Test point:");
       gcvt(TestPoint1 ,7,str);
       strcat(strbuf,str);
       strcpy(str1,"Z");
       strcat(strbuf,str1);
       fprintf(fp,"%s\r\n",strbuf);
       strcpy(strbuf,"Trip time(s):");
       fprintf(fp,"%s",strbuf);
       if(TripTimeSum[62]==0)
         strcpy(strbuf,"No Trip");
       else
         gcvt(TripTimeSum[62] ,7,strbuf);
       fprintf(fp,"%s\r\n",strbuf);
     }
   else if(FaultType7==0)
     {
       strcpy(strbuf,"Fault type:A-B-C，No Test");
       fprintf(fp,"%s\r\n",strbuf);
     }
 }
        else if(Zone4==0)
 {
   strcpy(strbuf,"Zone 4: No Test");
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

void Widget::on_pushButton_4_clicked() //上一页
{
    if(showpage>1)
       showpage-=1;
    ShowPage(showpage);
}

void Widget::on_pushButton_5_clicked() //下一页
{
    if(showpage<4)
        showpage+=1;
     ShowPage(showpage);
}


void Widget::on_checkBox_8_clicked()
{
   if(Zone1==1) //段1
       Zone1=0;
   else Zone1=1;
   if(Zone1==1)
       ui->lineEdit_4->setEnabled(true);
   else
       ui->lineEdit_4->setEnabled(false);

}

void Widget::on_checkBox_9_clicked()
{
    if(Zone2==1) //段1
        Zone2=0;
    else Zone2=1;
}


void Widget::on_checkBox_10_clicked()
{
    if(Zone3==1) //段1
        Zone3=0;
    else Zone3=1;
}

void Widget::on_checkBox_11_clicked()
{
    if(Zone4==1) //段1
        Zone4=0;
    else Zone4=1;
}

void Widget::on_checkBox_clicked()
{
    if(FaultType1==1)
        FaultType1=0;
    else FaultType1=1;
}

void Widget::on_checkBox_2_clicked()
{
    if(FaultType2==1)
        FaultType2=0;
    else FaultType2=1;
}

void Widget::on_checkBox_3_clicked()
{
    if(FaultType3==1)
        FaultType3=0;
    else FaultType3=1;
}

void Widget::on_checkBox_4_clicked()
{
    if(FaultType4==1)
        FaultType4=0;
    else FaultType4=1;
}

void Widget::on_checkBox_5_clicked()
{
    if(FaultType5==1)
        FaultType5=0;
    else FaultType5=1;
}

void Widget::on_checkBox_6_clicked()
{
    if(FaultType6==1)
        FaultType6=0;
    else FaultType6=1;
}

void Widget::on_checkBox_7_clicked()
{
    if(FaultType7==1)
        FaultType7=0;
    else FaultType7=1;
}


/*
void Widget::on_lineEdit_7_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_7;
     temprorarydata->keypadopenflag=true;
    currenteditpos=1;
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

void Widget::on_lineEdit_9_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_9;
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

void Widget::on_lineEdit_10_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_10;
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

void Widget::on_lineEdit_11_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_11;
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

void Widget::on_lineEdit_12_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_12;
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

void Widget::on_lineEdit_13_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_13;
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

void Widget::on_lineEdit_14_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_14;
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

void Widget::on_lineEdit_15_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_15;
     temprorarydata->keypadopenflag=true;
    currenteditpos=9;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_16_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_16;
     temprorarydata->keypadopenflag=true;
    currenteditpos=10;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_17_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_17;
     temprorarydata->keypadopenflag=true;
    currenteditpos=11;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_18_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_18;
     temprorarydata->keypadopenflag=true;
    currenteditpos=12;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_19_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_19;
     temprorarydata->keypadopenflag=true;
    currenteditpos=13;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_20_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_20;
     temprorarydata->keypadopenflag=true;
    currenteditpos=14;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_21_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_21;
     temprorarydata->keypadopenflag=true;
    currenteditpos=15;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_22_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit_22;
     temprorarydata->keypadopenflag=true;
    currenteditpos=16;
    pad=new keypads;
    pad->setModal(true);
    connect(pad,SIGNAL(sendvalue(QString)),this,SLOT(setlineedittext(QString)));
    connect(pad,SIGNAL(sendflag()),this,SLOT(clearkeypadflag()));
    pad->show();
    pad->raise();
    pad->activateWindow();
    }
}

void Widget::on_lineEdit_selectionChanged()
{
    if(temprorarydata->keypadopenflag==false)
   { currentedit=ui->lineEdit;
     temprorarydata->keypadopenflag=true;
    currenteditpos=17;
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
    currenteditpos=18;
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
   { currentedit=ui->lineEdit_3;
     temprorarydata->keypadopenflag=true;
    currenteditpos=19;
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
   { currentedit=ui->lineEdit_4;
     temprorarydata->keypadopenflag=true;
    currenteditpos=20;
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
    currenteditpos=21;
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
    currenteditpos=22;
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
void Widget::vi_parameter_enter(int position)
{


   switch(position)
    {
    case 1:
        if(Impedance1>999)
            Impedance1=999;
        else if(Impedance1<0)
            Impedance1=0;

        break;
    case 2:
        if(Impedance2>999)
            Impedance2=999;
        else if(Impedance2<0)
            Impedance2=0;

        break;
    case 3:
        if(Impedance3>999)
            Impedance3=999;
        else if(Impedance3<0)
            Impedance3=0;

        break;
    case 4:
        if(Impedance4>999)
            Impedance4=999;
        else if(Impedance4<0)
            Impedance4=0;

        break;
    case 5:
        if(DelicacyAngle1>999)
            DelicacyAngle1=999;
        else if(DelicacyAngle1<0)
            DelicacyAngle1=0;

        break;
    case 6:
        if(DelicacyAngle2>999)
            DelicacyAngle2=999;
        else if(DelicacyAngle2<0)
            DelicacyAngle2=0;

        break;
    case 7:
        if(DelicacyAngle3>999)
            DelicacyAngle3=999;
        else if(DelicacyAngle3<0)
            DelicacyAngle3=0;

        break;
    case 8:
        if(DelicacyAngle4>999)
            DelicacyAngle4=999;
        else if(DelicacyAngle4<0)
            DelicacyAngle4=0;

        break;

    case 9:
        if(FixupCurrent1>MAX_I_VALUEDATA_AC)
            FixupCurrent1=MAX_I_VALUEDATA_AC;
        else if(FixupCurrent1<0)
            FixupCurrent1=0;

        break;
    case 10:
        if(FixupCurrent2>MAX_I_VALUEDATA_AC)
            FixupCurrent2=MAX_I_VALUEDATA_AC;
        else if(FixupCurrent2<0)
            FixupCurrent2=0;

        break;
    case 11:
        if(FixupCurrent3>MAX_I_VALUEDATA_AC)
            FixupCurrent3=MAX_I_VALUEDATA_AC;
        else if(FixupCurrent3<0)
            FixupCurrent3=0;

        break;
    case 12:
        if(FixupCurrent4>MAX_I_VALUEDATA_AC)
            FixupCurrent4=MAX_I_VALUEDATA_AC;
        else if(FixupCurrent4<0)
            FixupCurrent4=0;

        break;

    case 13:
        if(FixupTime1>999.9)
            FixupTime1=999.9;
        else if(FixupTime1<0)
            FixupTime1=0;

        break;
    case 14:
        if(FixupTime2>999.9)
            FixupTime2=999.9;
        else if(FixupTime2<0)
            FixupTime2=0;

        break;
    case 15:
        if(FixupTime3>999.9)
            FixupTime3=999.9;
        else if(FixupTime3<0)
            FixupTime3=0;

        break;
    case 16:
        if(FixupTime4>999.9)
            FixupTime4=999.9;
        else if(FixupTime4<0)
            FixupTime4=0;

        break;
    case 17:
        if(EqualizeValue>999.9)
            EqualizeValue=999.9;
        else if(EqualizeValue<0)
            EqualizeValue=0;

        break;
    case 18:
        if(TestPoint1>999.9)
            TestPoint1=999.9;
        else if(TestPoint1<0)
             TestPoint1=0;

        break;
    case 19:
        if(TestPoint2>999.9)
            TestPoint2=999.9;
        else if(TestPoint2<0)
             TestPoint2=0;

        break;
    case 20:
        if(TestPoint3>999.9)
            TestPoint3=999.9;
        else if(TestPoint3<0)
             TestPoint3=0;

        break;

    case 21:
        if(PerFaultTime>999.9)
            PerFaultTime=999.9;
        else if(PerFaultTime<0)
             PerFaultTime=0;

        break;

    case 22:
        if(PostFaultTime>999.9)
            PostFaultTime=999.9;
        else if(PostFaultTime<0)
             PostFaultTime=0;

        break;
    case 23:
        if(KLphase>=180)
            KLphase=180;
        else if(KLphase<=-180)
             KLphase=-180;

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
        Impedance1=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Impedance1,'g',6));
        break;
    case 2:
        Impedance2=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Impedance2,'g',6));
        break;
    case 3:
        Impedance3=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Impedance3,'g',6));
        break;
    case 4:
        Impedance4=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Impedance4,'g',6));
        break;
    case 5:
        DelicacyAngle1=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DelicacyAngle1,'g',6));
        break;
    case 6:
        DelicacyAngle2=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DelicacyAngle2,'g',6));
        break;
    case 7:
        DelicacyAngle3=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DelicacyAngle3,'g',6));
        break;
    case 8:
        DelicacyAngle4=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(DelicacyAngle4,'g',6));
        break;
    case 9:
        FixupCurrent1=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixupCurrent1,'g',6));
        break;
    case 10:
        FixupCurrent2=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixupCurrent2,'g',6));
        break;
    case 11:
        FixupCurrent3=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixupCurrent3,'g',6));
        break;
    case 12:
        FixupCurrent4=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixupCurrent4,'g',6));
        break;
    case 13:
        FixupTime1=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixupTime1,'g',6));
        break;
    case 14:
        FixupTime2=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixupTime2,'g',6));
        break;
    case 15:
        FixupTime3=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixupTime3,'g',6));
        break;
    case 16:
        FixupTime4=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixupTime4,'g',6));
        break;

    case 17:
        EqualizeValue=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(EqualizeValue,'g',6));
        break;
    case 18:
        TestPoint1=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(TestPoint1,'g',6));
        break;
    case 19:
        TestPoint2=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(TestPoint2,'g',6));
        break;
    case 20:
        TestPoint3=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(TestPoint3,'g',6));
        break;
    case 21:
        PerFaultTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PerFaultTime,'g',6));
        break;
    case 22:
        PostFaultTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PostFaultTime,'g',6));
        break;

    case 23:
        KLphase=number;
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
helpdialog->SetHelpfilename("zone.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("zone1.txt");
//helpdialog->SetHelpfilename("zone.txt");
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

        if(obj==ui->lineEdit_7)
        {     temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_7;
             currenteditpos=1;
        }
       else if(obj==ui->lineEdit_8)
        {     temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_8;
             currenteditpos=2;
        }
        else if(obj==ui->lineEdit_9)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_9;
              currenteditpos=3;
         }
        else if(obj==ui->lineEdit_10)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_10;
              currenteditpos=4;
         }
        else if(obj==ui->lineEdit_11)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_11;
              currenteditpos=5;
         }
        else if(obj==ui->lineEdit_12)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_12;
              currenteditpos=6;
         }
        else if(obj==ui->lineEdit_13)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_13;
              currenteditpos=7;
         }
        else if(obj==ui->lineEdit_14)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_14;
              currenteditpos=8;
         }
        else if(obj==ui->lineEdit_15)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_15;
              currenteditpos=9;
         }
        else if(obj==ui->lineEdit_16)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_16;
              currenteditpos=10;
         }
        else if(obj==ui->lineEdit_17)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_17;
              currenteditpos=11;
         }
        else if(obj==ui->lineEdit_18)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_18;
              currenteditpos=12;
         }
        else if(obj==ui->lineEdit_19)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_19;
              currenteditpos=13;
         }
        else if(obj==ui->lineEdit_20)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_20;
              currenteditpos=14;
         }
        else if(obj==ui->lineEdit_21)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_21;
              currenteditpos=15;
         }
        else if(obj==ui->lineEdit_22)
         {     temprorarydata->keypadopenflag=true;
               currentedit=ui->lineEdit_22;
              currenteditpos=16;
         }

       else if(obj==ui->lineEdit)
        {     temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit;
             currenteditpos=17;
        }
       else if(obj==ui->lineEdit_2)
        {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_2;
             currenteditpos=18;
        }
        else if(obj==ui->lineEdit_3)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_3;
              currenteditpos=19;
         }
        else if(obj==ui->lineEdit_4)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_4;
              currenteditpos=20;
         }
        else if(obj==ui->lineEdit_5)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_5;
              currenteditpos=21;
         }
        else if(obj==ui->lineEdit_6)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_6;
              currenteditpos=22;
         }
        else if(obj==ui->lineEdit_23)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_23;
              currenteditpos=23;
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
              EqualizeValue= text.toFloat();
              currenteditpos=17;
              vi_parameter_enter(currenteditpos);
              ui->lineEdit->setText(QString::number(EqualizeValue,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               TestPoint1= text.toFloat();
               currenteditpos=18;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(TestPoint1,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                TestPoint2= text.toFloat();
                currenteditpos=19;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(TestPoint2,'g',6));
         }
        else if(obj==ui->lineEdit_4)
         {
                QString text=ui->lineEdit_4->text();
                TestPoint3= text.toFloat();
                currenteditpos=20;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_4->setText(QString::number(TestPoint3,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                PerFaultTime= text.toFloat();
                currenteditpos=21;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(PerFaultTime,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                PostFaultTime= text.toFloat();
                currenteditpos=22;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(PostFaultTime,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                Impedance1= text.toFloat();
                currenteditpos=1;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(Impedance1,'g',6));
         }
        else if(obj==ui->lineEdit_8)
         {
                QString text=ui->lineEdit_8->text();
                Impedance2= text.toFloat();
                currenteditpos=2;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_8->setText(QString::number(Impedance2,'g',6));
         }
        else if(obj==ui->lineEdit_9)
         {
                QString text=ui->lineEdit_9->text();
                Impedance3= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_9->setText(QString::number(Impedance3,'g',6));
         }
        else if(obj==ui->lineEdit_10)
         {
                QString text=ui->lineEdit_10->text();
                Impedance4= text.toFloat();
                currenteditpos=4;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_10->setText(QString::number(Impedance4,'g',6));
         }
        else if(obj==ui->lineEdit_11)
         {
                QString text=ui->lineEdit_11->text();
                DelicacyAngle1= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_11->setText(QString::number(DelicacyAngle1,'g',6));
         }
        else if(obj==ui->lineEdit_12)
         {
                QString text=ui->lineEdit_12->text();
                DelicacyAngle2= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_12->setText(QString::number(DelicacyAngle2,'g',6));
         }
        else if(obj==ui->lineEdit_13)
         {
                QString text=ui->lineEdit_13->text();
                DelicacyAngle3= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_13->setText(QString::number(DelicacyAngle3,'g',6));
         }
        else if(obj==ui->lineEdit_14)
         {
                QString text=ui->lineEdit_14->text();
                DelicacyAngle4= text.toFloat();
                currenteditpos=8;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_14->setText(QString::number(DelicacyAngle4,'g',6));
         }
        else if(obj==ui->lineEdit_15)
         {
                QString text=ui->lineEdit_15->text();
                FixupCurrent1= text.toFloat();
                currenteditpos=9;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_15->setText(QString::number(FixupCurrent1,'g',6));
         }
        else if(obj==ui->lineEdit_16)
         {
                QString text=ui->lineEdit_16->text();
                FixupCurrent2= text.toFloat();
                currenteditpos=10;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_16->setText(QString::number(FixupCurrent2,'g',6));
         }
        else if(obj==ui->lineEdit_17)
         {
                QString text=ui->lineEdit_17->text();
                FixupCurrent3= text.toFloat();
                currenteditpos=11;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_17->setText(QString::number(FixupCurrent3,'g',6));
         }
        else if(obj==ui->lineEdit_18)
         {
                QString text=ui->lineEdit_18->text();
                FixupCurrent4= text.toFloat();
                currenteditpos=12;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_18->setText(QString::number(FixupCurrent4,'g',6));
         }
        else if(obj==ui->lineEdit_19)
         {
                QString text=ui->lineEdit_19->text();
                FixupTime1= text.toFloat();
                currenteditpos=13;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_19->setText(QString::number(FixupTime1,'g',6));
         }
        else if(obj==ui->lineEdit_20)
         {
                QString text=ui->lineEdit_20->text();
                FixupTime2= text.toFloat();
                currenteditpos=14;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_20->setText(QString::number(FixupTime2,'g',6));
         }
        else if(obj==ui->lineEdit_21)
         {
                QString text=ui->lineEdit_21->text();
                FixupTime3= text.toFloat();
                currenteditpos=15;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_21->setText(QString::number(FixupTime3,'g',6));
         }
        else if(obj==ui->lineEdit_22)
         {
                QString text=ui->lineEdit_22->text();
                FixupTime4= text.toFloat();
                currenteditpos=16;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_22->setText(QString::number(FixupTime4,'g',6));
         }
        else if(obj==ui->lineEdit_23)
         {
                QString text=ui->lineEdit_23->text();
                KLphase= text.toFloat();
                currenteditpos=23;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_23->setText(QString::number(KLphase,'g',6));
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
    ui->pushButton_6->setFocus();
}


void Widget::on_pushButton_2_clicked()
{



}





void Widget::LLLNormalFaultPar()
  {//三相相间故障时，故障前和故障后参数
    V1PerFaultValue=SystemVolt;//SystemVolt;  //2009-6-20
    V2PerFaultValue=SystemVolt;//SystemVolt;
    V3PerFaultValue=SystemVolt;//SystemVolt;
    I1PerFaultValue=0;
    I2PerFaultValue=0;
    I3PerFaultValue=0;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //故障前电压每相输出SystemVoltV，相位对称，电流输出为0。

    V1PostFaultValue=SystemVolt;//SystemVolt;
    V2PostFaultValue=SystemVolt;//SystemVolt;
    V3PostFaultValue=SystemVolt;//SystemVolt;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;
    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //故障后电压每相输出SystemVoltV，相位对称，电流输出为0。
  }


void Widget::LLNormalFaultPar()
  {//两相相间故障时，故障前和故障后参数
    V1PerFaultValue=SystemVolt;//SystemVolt;
    V2PerFaultValue=SystemVolt;//SystemVolt;
    V3PerFaultValue=SystemVolt;//SystemVolt;
    I1PerFaultValue=0;
    I2PerFaultValue=0;
    I3PerFaultValue=0;
    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;

    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //故障前电压每相输出SystemVoltV，相位对称，电流输出为0。

    V1PostFaultValue=SystemVolt;//SystemVolt;
    V2PostFaultValue=SystemVolt;//SystemVolt;
    V3PostFaultValue=SystemVolt;//SystemVolt;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;

    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //故障后电压每相输出SystemVoltV，相位对称，电流输出为0。
  }


void Widget::LNNormalFaultPar()
  {//单相接地故障时，故障前和故障后参数
    V1PerFaultValue=SystemVolt;//SystemVolt;
    V2PerFaultValue=SystemVolt;//SystemVolt;
    V3PerFaultValue=SystemVolt;//SystemVolt;
    I1PerFaultValue=0;
    I2PerFaultValue=0;
    I3PerFaultValue=0;

    V1PerFaultPhase=0;
    V2PerFaultPhase=-120;
    V3PerFaultPhase=120;
    I1PerFaultPhase=75;
    I2PerFaultPhase=-45;
    I3PerFaultPhase=-165;
    //故障前电压每相输出SystemVoltV，相位对称，电流输出为0。





    V1PostFaultValue=SystemVolt;//SystemVolt;
    V2PostFaultValue=SystemVolt;//SystemVolt;
    V3PostFaultValue=SystemVolt;//SystemVolt;
    I1PostFaultValue=0;
    I2PostFaultValue=0;
    I3PostFaultValue=0;

    V1PostFaultPhase=0;
    V2PostFaultPhase=-120;
    V3PostFaultPhase=120;
    I1PostFaultPhase=75;
    I2PostFaultPhase=-45;
    I3PostFaultPhase=-165;
    //故障后电压每相输出SystemVoltV，相位对称，电流输出为0。
  }







void Widget::ShowCaculateValue( int FaultType)
{

    switch(FaultType)
    {
    case 1:
        ui->textEdit->append(tr("故障类型:A-E"));
        break;
    case 2:
        ui->textEdit->append(tr("故障类型:B-E"));
        break;

    case 3:
        ui->textEdit->append(tr("故障类型:C-E"));
        break;

    case 4:
        ui->textEdit->append(tr("故障类型:A-B"));
        break;

    case 5:
        ui->textEdit->append(tr("故障类型:B-C"));

        break;

    case 6:
         ui->textEdit->append(tr("故障类型:C-A"));

        break;

    case 7:
        ui->textEdit->append(tr("故障类型:A-B-C"));
        break;
    }

    ui->textEdit->append(tr("故障前电压:"));
    ui->textEdit->append(tr("VA(V):")+QString::number(V1PerFaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(V1PerFaultPhase,'g',6));
    ui->textEdit->append(tr("VB(V):")+QString::number(V2PerFaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(V2PerFaultPhase,'g',6));
    ui->textEdit->append(tr("VC(V):")+QString::number(V3PerFaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(V3PerFaultPhase,'g',6));

    ui->textEdit->append(tr("故障前电流:"));
    ui->textEdit->append(tr("IA(A):")+QString::number(I1PerFaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(I1PerFaultPhase,'g',6));
    ui->textEdit->append(tr("IB(A):")+QString::number(I2PerFaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(I2PerFaultPhase,'g',6));
    ui->textEdit->append(tr("IC(A):")+QString::number(I3PerFaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(I3PerFaultPhase,'g',6));



    ui->textEdit->append(tr("故障电压:"));
    ui->textEdit->append(tr("VA(V):")+QString::number(V1FaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(V1FaultPhase,'g',6));
    ui->textEdit->append(tr("VB(V):")+QString::number(V2FaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(V2FaultPhase,'g',6));
    ui->textEdit->append(tr("VC(V):")+QString::number(V3FaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(V3FaultPhase,'g',6));
    switch(FaultType)
    {
    case 4:
        ui->textEdit->append(tr("VAB(V):")+QString::number(VabFaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(VabFaultPhase,'g',6));
        break;
    case 5:
        ui->textEdit->append(tr("VBC(V):")+QString::number(VbcFaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(VbcFaultPhase,'g',6));
        break;
    case 6:
        ui->textEdit->append(tr("VCA(V):")+QString::number(VbcFaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(VcaFaultPhase,'g',6));
        break;


    }






    ui->textEdit->append(tr("故障电流:"));
    ui->textEdit->append(tr("IA(A):")+QString::number(I1FaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(I1FaultPhase,'g',6));
    ui->textEdit->append(tr("IB(A):")+QString::number(I2FaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(I2FaultPhase,'g',6));
    ui->textEdit->append(tr("IC(A):")+QString::number(I3FaultValue,'g',6)+tr("")+tr("相位(Deg):")+QString::number(I3FaultPhase,'g',6));



}




void Widget::on_checkBox_12_clicked()
{
    Busvolcon=!Busvolcon;
}

void Widget::on_textBrowser_textChanged()
{
    ui->textEdit->moveCursor(QTextCursor::Start);
}
int Widget::OutputSignalCompute(int flag,int k)
  {
    float Impedance; //Impedance 闃绘姉
    int   ErrorFlag;

    COMPLEX C;
    C.x=3*EqualizeValue*cos(KLphase*3.1415/180)+2;
    C.y=3*EqualizeValue*sin(KLphase*3.1415/180);
    float Angel_C=CaculatePhase(C.y,C.x);
    if(Angel_C>=180)
        Angel_C-=360;
    else if(Angel_C<=-180)
      Angel_C+=360;





    switch(flag)
     {

     case 1:  //1段的A-E故障
       LNNormalFaultPar();//故障前和故障后参数电压都为系统电压，电流为0
       V1Output=1;
       V2Output=1;
       V3Output=1;
       I1Output=1;
       I2Output=0;
       I3Output=0;

          //不同测试点阻抗不一样
       if(k==1)
         Impedance=Impedance1*TestPoint1;
       else if(k==0)
         Impedance=Impedance1*TestPoint2;
       else if(k==-1)
         Impedance=Impedance1*TestPoint3;

       I1FaultValue=FixupCurrent1;
       if(Busvolcon==true)
     {  //调用出错处理程序，如果返回1，说明参数设置不对。

       COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
       V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
       V2FaultValue=SystemVolt;//SystemVolt;
       V3FaultValue=SystemVolt;//SystemVolt;
       I2FaultValue=0;
       I3FaultValue=0;
       //单相接地故障时，未发生故障的电压为Vnom（SystemVolt），未发生故障的电流为0。
       V1FaultPhase=CaculatePhase(Va.y,Va.x);
       V2FaultPhase=-120+V1FaultPhase;
       V3FaultPhase=120+V1FaultPhase;
       //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
     }
     else
       {

        I2FaultValue=0;
        I3FaultValue=0;

        COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
        COMPLEX Vb=VbCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
        COMPLEX Vc=VcCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);


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

       ErrorFlag=ErrorWarning(1,1);
       if(ErrorFlag==1)
       return 1;


       I1FaultPhase=0-DelicacyAngle1-Angel_C;
       if(k==-1)
       I1FaultPhase=I1FaultPhase-180;


       I2FaultPhase=I1FaultPhase-120;
       I3FaultPhase=I1FaultPhase+120;
       LNNormalFaultPar();//故障前和故障后参数
       //return 0;
      break;
    case 2://1段的B-E故障
      LNNormalFaultPar();//故障前和故障后参数
      V1Output=1;
      V2Output=1;
      V3Output=1;
      I1Output=0;
      I2Output=1;
      I3Output=0;

      if(k==1)
        Impedance=Impedance1*TestPoint1;
      else if(k==0)
        Impedance=Impedance1*TestPoint2;
      else if(k==-1)
        Impedance=Impedance1*TestPoint3;
      I2FaultValue=FixupCurrent1;
      //单相接地故障时，电流故障相I=I(固定电流)
      if(Busvolcon==true)
     {
      COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
      V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
      V1FaultValue=SystemVolt;//SystemVolt;
      V3FaultValue=SystemVolt;//SystemVolt;
      I1FaultValue=0;
      I3FaultValue=0;

      V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
      V1FaultPhase=V2FaultPhase+120;
      V3FaultPhase=V2FaultPhase-120;
     }
     else
      {
          I1FaultValue=0;
          I3FaultValue=0;
          COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
          COMPLEX Vc=VbCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
          COMPLEX Va=VcCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);

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

      ErrorFlag=ErrorWarning(1,2);
      if(ErrorFlag==1)
      return 1;

      I2FaultPhase=0-DelicacyAngle1-Angel_C;
      if(k==-1)
        I2FaultPhase=I2FaultPhase-180;
      I1FaultPhase=I2FaultPhase+120;
      I3FaultPhase=I2FaultPhase-120;
      LNNormalFaultPar();//故障前和故障后参数
      //return 0;
      break;
    case 3://1段的C-E故障
      LNNormalFaultPar();//故障前和故障后参数
      V1Output=1;
      V2Output=1;
      V3Output=1;
      I1Output=0;
      I2Output=0;
      I3Output=1;

      if(k==1)
        Impedance=Impedance1*TestPoint1;
      else if(k==0)
        Impedance=Impedance1*TestPoint2;
      else if(k==-1)
        Impedance=Impedance1*TestPoint3;
      I3FaultValue=FixupCurrent1;
      //单相接地故障时，电流故障相I=I(固定电流)


      //调用出错处理程序，如果返回1，说明参数设置不对。
      if(Busvolcon==true)
     {
      COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
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
          COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
          COMPLEX Va=VbCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);
          COMPLEX Vb=VcCaculate(EqualizeValue,KLphase,FixupCurrent1,Impedance,DelicacyAngle1);


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

      ErrorFlag=ErrorWarning(1,3);
      if(ErrorFlag==1)
      return 1;



      //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
      I3FaultPhase=0-DelicacyAngle1-Angel_C;
      if(k==-1)
        I3FaultPhase=I3FaultPhase-180;

      I1FaultPhase=I3FaultPhase-120;
      I2FaultPhase=I3FaultPhase+120;
      //单相接地故障时，故障电流的相位=故障电压的相位-灵敏角。
      LNNormalFaultPar();//故障前和故障后参数

         // return 0;
          break;
        case 4://1娈电殑A-B鏁呴殰
        {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=0;

          if(k==1)
            Impedance=Impedance1*TestPoint1;
          else if(k==0)
            Impedance=Impedance1*TestPoint2;
          else if(k==-1)
            Impedance=Impedance1*TestPoint3;

          I1FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I2FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I3FaultValue=I3PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊


          COMPLEX Va1=V1LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          COMPLEX Vb1=V2LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
          V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);



          //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
          V3FaultValue=V3PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
          V3FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅
          V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
          V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);

          V1FaultPhase=V1FaultPhase;
          V2FaultPhase=V2FaultPhase;

          ErrorFlag=ErrorWarning(1,4);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
           if(ErrorFlag==1)
          return 1;







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



          I3FaultPhase=0;
          I1FaultPhase=-90-DelicacyAngle1;
          I2FaultPhase=90-DelicacyAngle1;
          if(k==-1)
            I1FaultPhase=I1FaultPhase-180;
          if(I1FaultPhase>180)
            I1FaultPhase=I1FaultPhase-360;
          else if(I1FaultPhase<-180)
            I1FaultPhase=I1FaultPhase+360;

          if(k==-1)
            I2FaultPhase=I2FaultPhase-180;
          if(I2FaultPhase>180)
            I2FaultPhase=I2FaultPhase-360;
          else if(I2FaultPhase<-180)
            I2FaultPhase=I2FaultPhase+360;

          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
          //return 0;
      }
          break;
        case 5://1娈电殑B-C鏁呴殰
        {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=0;
          I2Output=1;
          I3Output=1;

          if(k==1)
            Impedance=Impedance1*TestPoint1;
          else if(k==0)
            Impedance=Impedance1*TestPoint2;
          else if(k==-1)
            Impedance=Impedance1*TestPoint3;

          I2FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I3FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I1FaultValue=I1PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

          V1FaultValue=V1PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
          V1FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅

          COMPLEX Vb2=V1LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          COMPLEX Vc2=V2LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
          V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);



          ErrorFlag=ErrorWarning(1,5);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
           if(ErrorFlag==1)
          return 1;


          //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

          V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
          V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);
          V3FaultPhase=V3FaultPhase;
          V2FaultPhase=V2FaultPhase;




          //鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅數鍘嬬殑鐩镐綅涓嶅彉锛
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


          I1FaultPhase=0;
          I2FaultPhase=-90-DelicacyAngle1;
          I3FaultPhase=90-DelicacyAngle1;
          if(k==-1)
            I2FaultPhase=I2FaultPhase-180;
          if(I2FaultPhase>180)
            I2FaultPhase=I2FaultPhase-360;
          else if(I2FaultPhase<-180)
            I2FaultPhase=I2FaultPhase+360;

          if(k==-1)
            I3FaultPhase=I3FaultPhase-180;
          if(I3FaultPhase>180)
            I3FaultPhase=I3FaultPhase-360;
          else if(I3FaultPhase<-180)
            I3FaultPhase=I3FaultPhase+360;
          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
         // return 0;
          }
          break;
        case 6://1娈电殑C-A鏁呴殰
        {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=0;
          I3Output=1;

          if(k==1)
            Impedance=Impedance1*TestPoint1;
          else if(k==0)
            Impedance=Impedance1*TestPoint2;
          else if(k==-1)
            Impedance=Impedance1*TestPoint3;

          I1FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I3FaultValue=FixupCurrent1;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I2FaultValue=I2PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

          V2FaultValue=V2PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
          V2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅


          COMPLEX Vc3=V1LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          COMPLEX Va3=V2LLCaculate(FixupCurrent1,Impedance,SystemVolt);
          V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
          V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);



          ErrorFlag=ErrorWarning(1,6);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
           if(ErrorFlag==1)
          return 1;


          //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
          V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
          V1FaultPhase=CaculatePhase(Va3.y,Va3.x);
          V3FaultPhase=V3FaultPhase;
          V1FaultPhase=V1FaultPhase;


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

          I2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢垫祦鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佺殑鐩镐綅
          I3FaultPhase=-90-DelicacyAngle1;
          I1FaultPhase=90-DelicacyAngle1;
          if(k==-1)
            I3FaultPhase=I3FaultPhase-180;
          if(I3FaultPhase>180)
            I3FaultPhase=I3FaultPhase-360;
          else if(I3FaultPhase<-180)
            I3FaultPhase=I3FaultPhase+360;

          if(k==-1)
            I1FaultPhase=I1FaultPhase-180;
          if(I1FaultPhase>180)
            I1FaultPhase=I1FaultPhase-360;
          else if(I1FaultPhase<-180)
            I1FaultPhase=I1FaultPhase+360;
          LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
          //return 0;
      }
          break;
        case 7://1娈电殑A-B-C鏁呴殰

       {   LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

          V1Output=1;
          V2Output=1;
          V3Output=1;
          I1Output=1;
          I2Output=1;
          I3Output=1;

          if(k==1)
            Impedance=Impedance1*TestPoint1;
          else if(k==0)
            Impedance=Impedance1*TestPoint2;
          else if(k==-1)
            Impedance=Impedance1*TestPoint3;

          I1FaultValue=FixupCurrent1;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I2FaultValue=FixupCurrent1;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
          I3FaultValue=FixupCurrent1;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦

          V1FaultValue=FixupCurrent1*Impedance;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝U=I*Z锛堢浉鐢靛帇=鍥哄畾鐢垫祦*闃绘姉瀹氬€硷級
          V2FaultValue=V1FaultValue;
          V3FaultValue=V1FaultValue;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝V1銆乂2銆乂3骞呭€煎ぇ灏忕浉绛


          ErrorFlag=ErrorWarning(1,7);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
           if(ErrorFlag==1)
          return 1;



          //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

          V1FaultPhase=V1PerFaultPhase;
          V2FaultPhase=V2PerFaultPhase;
          V3FaultPhase=V3PerFaultPhase;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐢靛帇鐩哥殑鐩镐綅绛変簬鏁呴殰鍓嶇浉搴旂殑鐩哥殑鐩镐綅銆
          I1FaultPhase=V1FaultPhase-DelicacyAngle1;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐏垫晱瑙掔瓑浜嶪1涓嶸1涔嬮棿鐨勫す瑙掞紝I1鐨勭浉浣嶇瓑浜嶸1鐨勭浉浣嶅噺鐏垫晱瑙
          I2FaultPhase=I1FaultPhase-120;
          I3FaultPhase=I1FaultPhase+120;
          if(k==-1)
            I1FaultPhase=I1FaultPhase-180;
          if(I1FaultPhase>180)
            I1FaultPhase=I1FaultPhase-360;
          else if(I1FaultPhase<-180)
            I1FaultPhase=I1FaultPhase+360;

          if(k==-1)
            I2FaultPhase=I2FaultPhase-180;
          if(I2FaultPhase>180)
            I2FaultPhase=I2FaultPhase-360;
          else if(I2FaultPhase<-180)
            I2FaultPhase=I2FaultPhase+360;

          if(k==-1)
            I3FaultPhase=I3FaultPhase-180;
          if(I3FaultPhase>180)
            I3FaultPhase=I3FaultPhase-360;
          else if(I3FaultPhase<-180)
            I3FaultPhase=I3FaultPhase+360;
          //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐩镐綅鎴20搴﹀绉板叧绯汇€
          LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
           }

          break;

 case 8:
    LNNormalFaultPar();//故障前和故障后参数电压都为系统电压，电流为0
    V1Output=1;
    V2Output=1;
    V3Output=1;
    I1Output=1;
    I2Output=0;
    I3Output=0;

       //不同测试点阻抗不一样
    if(k==1)
      Impedance=Impedance2*TestPoint1;
    else if(k==0)
      Impedance=Impedance2*TestPoint2;
    else if(k==-1)
      Impedance=Impedance2*TestPoint3;

    I1FaultValue=FixupCurrent2;
    if(Busvolcon==true)
  {  //调用出错处理程序，如果返回1，说明参数设置不对。
    COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
    V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
    V2FaultValue=SystemVolt;//SystemVolt;
    V3FaultValue=SystemVolt;//SystemVolt;
    I2FaultValue=0;
    I3FaultValue=0;
    //单相接地故障时，未发生故障的电压为Vnom（SystemVolt），未发生故障的电流为0。
    V1FaultPhase=CaculatePhase(Va.y,Va.x);
    V2FaultPhase=-120+V1FaultPhase;
    V3FaultPhase=120+V1FaultPhase;
    //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
  }
  else
    {

     I2FaultValue=0;
     I3FaultValue=0;

     COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
     COMPLEX Vb=VbCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
     COMPLEX Vc=VcCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);


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

    ErrorFlag=ErrorWarning(2,1);
    if(ErrorFlag==1)
    return 1;

    I1FaultPhase=0-DelicacyAngle2-Angel_C;
    if(k==-1)
    I1FaultPhase=I1FaultPhase-180;
    I2FaultPhase=I1FaultPhase-120;
    I3FaultPhase=I1FaultPhase+120;
    LNNormalFaultPar();//故障前和故障后参数
    break;

   case 9:
         LNNormalFaultPar();//故障前和故障后参数
         V1Output=1;
         V2Output=1;
         V3Output=1;
         I1Output=0;
         I2Output=1;
         I3Output=0;

         if(k==1)
           Impedance=Impedance2*TestPoint1;
         else if(k==0)
           Impedance=Impedance2*TestPoint2;
         else if(k==-1)
           Impedance=Impedance2*TestPoint3;
         I2FaultValue=FixupCurrent2;
         //单相接地故障时，电流故障相I=I(固定电流)
         if(Busvolcon==true)
        {
         COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
         V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
         V1FaultValue=SystemVolt;//SystemVolt;
         V3FaultValue=SystemVolt;//SystemVolt;
         I1FaultValue=0;
         I3FaultValue=0;

         V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
         V1FaultPhase=V2FaultPhase+120;
         V3FaultPhase=V2FaultPhase-120;
        }
        else
         {
             I1FaultValue=0;
             I3FaultValue=0;
             COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
             COMPLEX Vc=VbCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
             COMPLEX Va=VcCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);

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

         ErrorFlag=ErrorWarning(2,2);
         if(ErrorFlag==1)
         return 1;

         I2FaultPhase=0-DelicacyAngle2-Angel_C;
         if(k==-1)
           I2FaultPhase=I2FaultPhase-180;
         I1FaultPhase=I2FaultPhase+120;
         I3FaultPhase=I2FaultPhase-120;
         LNNormalFaultPar();//故障前和故障后参数

        break;
      case 10://2娈电殑C-E鏁呴殰
               LNNormalFaultPar();//故障前和故障后参数
               V1Output=1;
               V2Output=1;
               V3Output=1;
               I1Output=0;
               I2Output=0;
               I3Output=1;

               if(k==1)
                 Impedance=Impedance2*TestPoint1;
               else if(k==0)
                 Impedance=Impedance2*TestPoint2;
               else if(k==-1)
                 Impedance=Impedance2*TestPoint3;
               I3FaultValue=FixupCurrent2;
               //单相接地故障时，电流故障相I=I(固定电流)

               //调用出错处理程序，如果返回1，说明参数设置不对。
               if(Busvolcon==true)
              {
               COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
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
                   COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
                   COMPLEX Va=VbCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);
                   COMPLEX Vb=VcCaculate(EqualizeValue,KLphase,FixupCurrent2,Impedance,DelicacyAngle2);


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

               ErrorFlag=ErrorWarning(2,3);
               if(ErrorFlag==1)
               return 1;

               //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
               I3FaultPhase=0-DelicacyAngle2-Angel_C;
               if(k==-1)
                 I3FaultPhase=I3FaultPhase-180;

               I1FaultPhase=I3FaultPhase-120;
               I2FaultPhase=I3FaultPhase+120;
               //单相接地故障时，故障电流的相位=故障电压的相位-灵敏角。
               LNNormalFaultPar();//故障前和故障后参数


        break;
      case 11://2娈电殑A-B鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=0;

        if(k==1)
          Impedance=Impedance2*TestPoint1;
        else if(k==0)
          Impedance=Impedance2*TestPoint2;
        else if(k==-1)
          Impedance=Impedance2*TestPoint3;

        I1FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=I3PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊


        COMPLEX Va1=V1LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        COMPLEX Vb1=V2LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
        V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);



        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultValue=V3PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V3FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅
        V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
        V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);
        V1FaultPhase=V1FaultPhase;
        V2FaultPhase=V2FaultPhase;


        ErrorFlag=ErrorWarning(2,4);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;





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



        I3FaultPhase=0;
        I1FaultPhase=-90-DelicacyAngle2;
        I2FaultPhase=90-DelicacyAngle2;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 12://2娈电殑B-C鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=0;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance2*TestPoint1;
        else if(k==0)
          Impedance=Impedance2*TestPoint2;
        else if(k==-1)
          Impedance=Impedance2*TestPoint3;

        I2FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I1FaultValue=I1PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V1FaultValue=V1PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V1FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅

        COMPLEX Vb2=V1LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        COMPLEX Vc2=V2LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
        V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€


        ErrorFlag=ErrorWarning(2,5);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;




        V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
        V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);
        V3FaultPhase=V3FaultPhase;
        V2FaultPhase=V2FaultPhase;



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


        I1FaultPhase=0;
        I2FaultPhase=-90-DelicacyAngle2;
        I3FaultPhase=90-DelicacyAngle2;
        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
       // return 0;
        }
        break;
      case 13://2娈电殑C-A鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=0;
        I3Output=1;

        if(k==1)
          Impedance=Impedance2*TestPoint1;
        else if(k==0)
          Impedance=Impedance2*TestPoint2;
        else if(k==-1)
          Impedance=Impedance2*TestPoint3;

        I1FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent2;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=I2PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V2FaultValue=V2PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅


        COMPLEX Vc3=V1LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        COMPLEX Va3=V2LLCaculate(FixupCurrent2,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
        V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);


        ErrorFlag=ErrorWarning(2,6);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;




        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
        V1FaultPhase=CaculatePhase(Va3.y,Va3.x);
        V3FaultPhase=V3FaultPhase;
        V1FaultPhase=V1FaultPhase;


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

        I2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢垫祦鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佺殑鐩镐綅
        I3FaultPhase=-90-DelicacyAngle2;
        I1FaultPhase=90-DelicacyAngle2;
        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;

        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 14://2娈电殑A-B-C鏁呴殰
        {
        LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance2*TestPoint1;
        else if(k==0)
          Impedance=Impedance2*TestPoint2;
        else if(k==-1)
          Impedance=Impedance2*TestPoint3;

        I1FaultValue=FixupCurrent2;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent2;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent2;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦

        V1FaultValue=FixupCurrent2*Impedance;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝U=I*Z锛堢浉鐢靛帇=鍥哄畾鐢垫祦*闃绘姉瀹氬€硷級
        V2FaultValue=V1FaultValue;
        V3FaultValue=V1FaultValue;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝V1銆乂2銆乂3骞呭€煎ぇ灏忕浉绛


        ErrorFlag=ErrorWarning(2,7);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V1FaultPhase=V1PerFaultPhase;
        V2FaultPhase=V2PerFaultPhase;
        V3FaultPhase=V3PerFaultPhase;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐢靛帇鐩哥殑鐩镐綅绛変簬鏁呴殰鍓嶇浉搴旂殑鐩哥殑鐩镐綅銆
        I1FaultPhase=V1FaultPhase-DelicacyAngle2;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐏垫晱瑙掔瓑浜嶪1涓嶸1涔嬮棿鐨勫す瑙掞紝I1鐨勭浉浣嶇瓑浜嶸1鐨勭浉浣嶅噺鐏垫晱瑙
        I2FaultPhase=I1FaultPhase-120;
        I3FaultPhase=I1FaultPhase+120;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐩镐綅鎴20搴﹀绉板叧绯汇€
        LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
         }
        break;


  case 15:
    LNNormalFaultPar();//故障前和故障后参数电压都为系统电压，电流为0
    V1Output=1;
    V2Output=1;
    V3Output=1;
    I1Output=1;
    I2Output=0;
    I3Output=0;

       //不同测试点阻抗不一样
    if(k==1)
      Impedance=Impedance3*TestPoint1;
    else if(k==0)
      Impedance=Impedance3*TestPoint2;
    else if(k==-1)
      Impedance=Impedance3*TestPoint3;

    I1FaultValue=FixupCurrent3;
    if(Busvolcon==true)
  {  //调用出错处理程序，如果返回1，说明参数设置不对。

    COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
    V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
    V2FaultValue=SystemVolt;//SystemVolt;
    V3FaultValue=SystemVolt;//SystemVolt;
    I2FaultValue=0;
    I3FaultValue=0;
    //单相接地故障时，未发生故障的电压为Vnom（SystemVolt），未发生故障的电流为0。
    V1FaultPhase=CaculatePhase(Va.y,Va.x);
    V2FaultPhase=-120+V1FaultPhase;
    V3FaultPhase=120+V1FaultPhase;
    //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
  }
  else
    {

     I2FaultValue=0;
     I3FaultValue=0;

     COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
     COMPLEX Vb=VbCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
     COMPLEX Vc=VcCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);


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

    ErrorFlag=ErrorWarning(3,1);
    if(ErrorFlag==1)
    return 1;



    I1FaultPhase=0-DelicacyAngle3-Angel_C;
    if(k==-1)
    I1FaultPhase=I1FaultPhase-180;

    I2FaultPhase=I1FaultPhase-120;
    I3FaultPhase=I1FaultPhase+120;
    LNNormalFaultPar();//故障前和故障后参数
 break;
      case 16://3娈电殑B-E鏁呴殰
         LNNormalFaultPar();//故障前和故障后参数
         V1Output=1;
         V2Output=1;
         V3Output=1;
         I1Output=0;
         I2Output=1;
         I3Output=0;
         if(k==1)
           Impedance=Impedance3*TestPoint1;
         else if(k==0)
           Impedance=Impedance3*TestPoint2;
         else if(k==-1)
           Impedance=Impedance3*TestPoint3;
         I2FaultValue=FixupCurrent3;
         if(Busvolcon==true)
        {
         COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
         V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
         V1FaultValue=SystemVolt;//SystemVolt;
         V3FaultValue=SystemVolt;//SystemVolt;
         I1FaultValue=0;
         I3FaultValue=0;

         V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
         V1FaultPhase=V2FaultPhase+120;
         V3FaultPhase=V2FaultPhase-120;
        }
        else
         {
             I1FaultValue=0;
             I3FaultValue=0;
             COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
             COMPLEX Vc=VbCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
             COMPLEX Va=VcCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);

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

         ErrorFlag=ErrorWarning(3,2);
         if(ErrorFlag==1)
         return 1;

         I2FaultPhase=0-DelicacyAngle3-Angel_C;
         if(k==-1)
           I2FaultPhase=I2FaultPhase-180;
         I1FaultPhase=I2FaultPhase+120;
         I3FaultPhase=I2FaultPhase-120;
         LNNormalFaultPar();//故障前和故障后参数
         //return 0;
        break;
      case 17://1娈电殑C-E鏁呴殰
         LNNormalFaultPar();//故障前和故障后参数
               V1Output=1;
               V2Output=1;
               V3Output=1;
               I1Output=0;
               I2Output=0;
               I3Output=1;

               if(k==1)
                 Impedance=Impedance3*TestPoint1;
               else if(k==0)
                 Impedance=Impedance3*TestPoint2;
               else if(k==-1)
                 Impedance=Impedance3*TestPoint3;
               I3FaultValue=FixupCurrent3;


               if(Busvolcon==true)
              {
               COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
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
                   COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
                   COMPLEX Va=VbCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);
                   COMPLEX Vb=VcCaculate(EqualizeValue,KLphase,FixupCurrent3,Impedance,DelicacyAngle3);

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
               ErrorFlag=ErrorWarning(3,3);
               if(ErrorFlag==1)
               return 1;

               I3FaultPhase=0-DelicacyAngle3-Angel_C;
               if(k==-1)
                 I3FaultPhase=I3FaultPhase-180;

               I1FaultPhase=I3FaultPhase-120;
               I2FaultPhase=I3FaultPhase+120;
               //单相接地故障时，故障电流的相位=故障电压的相位-灵敏角。
               LNNormalFaultPar();//故障前和故障后参数
        break;
      case 18://1娈电殑A-B鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=0;

        if(k==1)
          Impedance=Impedance3*TestPoint1;
        else if(k==0)
          Impedance=Impedance3*TestPoint2;
        else if(k==-1)
          Impedance=Impedance3*TestPoint3;

        I1FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=I3PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊


        COMPLEX Va1=V1LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        COMPLEX Vb1=V2LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
        V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);



        ErrorFlag=ErrorWarning(3,4);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultValue=V3PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V3FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅
        V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
        V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);


        V2FaultPhase=V2FaultPhase;
        V1FaultPhase=V1FaultPhase;





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



        I3FaultPhase=0;
        I1FaultPhase=-90-DelicacyAngle3;
        I2FaultPhase=90-DelicacyAngle3;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 19://1娈电殑B-C鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=0;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance3*TestPoint1;
        else if(k==0)
          Impedance=Impedance3*TestPoint2;
        else if(k==-1)
          Impedance=Impedance3*TestPoint3;

        I2FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I1FaultValue=I1PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V1FaultValue=V1PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V1FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅

        COMPLEX Vb2=V1LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        COMPLEX Vc2=V2LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
        V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);



        ErrorFlag=ErrorWarning(3,5);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
        V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);


        V2FaultPhase=V2FaultPhase;
        V3FaultPhase=V3FaultPhase;




        //鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅數鍘嬬殑鐩镐綅涓嶅彉锛
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


        I1FaultPhase=0;
        I2FaultPhase=-90-DelicacyAngle3;
        I3FaultPhase=90-DelicacyAngle3;
        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
       // return 0;
        }
        break;
      case 20://1娈电殑C-A鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=0;
        I3Output=1;

        if(k==1)
          Impedance=Impedance3*TestPoint1;
        else if(k==0)
          Impedance=Impedance3*TestPoint2;
        else if(k==-1)
          Impedance=Impedance3*TestPoint3;

        I1FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent3;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=I2PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V2FaultValue=V2PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅


        COMPLEX Vc3=V1LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        COMPLEX Va3=V2LLCaculate(FixupCurrent3,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
        V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);


        ErrorFlag=ErrorWarning(3,6);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
        V1FaultPhase=CaculatePhase(Va3.y,Va3.x);
        V1FaultPhase=V1FaultPhase;
        V3FaultPhase=V3FaultPhase;


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

        I2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢垫祦鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佺殑鐩镐綅
        I3FaultPhase=-90-DelicacyAngle3;
        I1FaultPhase=90-DelicacyAngle3;
        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;

        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 21://1娈电殑A-B-C鏁呴殰
     {   LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance3*TestPoint1;
        else if(k==0)
          Impedance=Impedance3*TestPoint2;
        else if(k==-1)
          Impedance=Impedance3*TestPoint3;

        I1FaultValue=FixupCurrent3;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent3;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent3;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦

        V1FaultValue=FixupCurrent3*Impedance;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝U=I*Z锛堢浉鐢靛帇=鍥哄畾鐢垫祦*闃绘姉瀹氬€硷級
        V2FaultValue=V1FaultValue;
        V3FaultValue=V1FaultValue;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝V1銆乂2銆乂3骞呭€煎ぇ灏忕浉绛


        ErrorFlag=ErrorWarning(3,7);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V1FaultPhase=V1PerFaultPhase;
        V2FaultPhase=V2PerFaultPhase;
        V3FaultPhase=V3PerFaultPhase;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐢靛帇鐩哥殑鐩镐綅绛変簬鏁呴殰鍓嶇浉搴旂殑鐩哥殑鐩镐綅銆
        I1FaultPhase=V1FaultPhase-DelicacyAngle3;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐏垫晱瑙掔瓑浜嶪1涓嶸1涔嬮棿鐨勫す瑙掞紝I1鐨勭浉浣嶇瓑浜嶸1鐨勭浉浣嶅噺鐏垫晱瑙
        I2FaultPhase=I1FaultPhase-120;
        I3FaultPhase=I1FaultPhase+120;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐩镐綅鎴20搴﹀绉板叧绯汇€
        LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
         }

        break;

      case 22:  //4娈电殑A-E鏁呴殰

    LNNormalFaultPar();//故障前和故障后参数电压都为系统电压，电流为0
    V1Output=1;
    V2Output=1;
    V3Output=1;
    I1Output=1;
    I2Output=0;
    I3Output=0;

       //不同测试点阻抗不一样
    if(k==1)
      Impedance=Impedance4*TestPoint1;
    else if(k==0)
      Impedance=Impedance4*TestPoint2;
    else if(k==-1)
      Impedance=Impedance4*TestPoint3;

    I1FaultValue=FixupCurrent4;
    if(Busvolcon==true)
  {

    COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
    V1FaultValue=sqrt(Va.x*Va.x+Va.y*Va.y);
    V2FaultValue=SystemVolt;//SystemVolt;
    V3FaultValue=SystemVolt;//SystemVolt;
    I2FaultValue=0;
    I3FaultValue=0;
    //单相接地故障时，未发生故障的电压为Vnom（SystemVolt），未发生故障的电流为0。
    V1FaultPhase=CaculatePhase(Va.y,Va.x);
    V2FaultPhase=-120+V1FaultPhase;
    V3FaultPhase=120+V1FaultPhase;
    //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
  }
  else
    {

     I2FaultValue=0;
     I3FaultValue=0;

     COMPLEX Va=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
     COMPLEX Vb=VbCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
     COMPLEX Vc=VcCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);


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

    ErrorFlag=ErrorWarning(4,1);
    if(ErrorFlag==1)
    return 1;



    I1FaultPhase=0-DelicacyAngle4-Angel_C;
    if(k==-1)
    I1FaultPhase=I1FaultPhase-180;

    I2FaultPhase=I1FaultPhase-120;
    I3FaultPhase=I1FaultPhase+120;
    LNNormalFaultPar();//故障前和故障后参数
        break;
      case 23://4娈电殑B-E鏁呴殰
         LNNormalFaultPar();//故障前和故障后参数
         V1Output=1;
         V2Output=1;
         V3Output=1;
         I1Output=0;
         I2Output=1;
         I3Output=0;
         if(k==1)
           Impedance=Impedance4*TestPoint1;
         else if(k==0)
           Impedance=Impedance4*TestPoint2;
         else if(k==-1)
           Impedance=Impedance4*TestPoint3;
         I2FaultValue=FixupCurrent4;
         if(Busvolcon==true)
        {
         COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
         V2FaultValue=sqrt(Vb.x*Vb.x+Vb.y*Vb.y);
         V1FaultValue=SystemVolt;//SystemVolt;
         V3FaultValue=SystemVolt;//SystemVolt;
         I1FaultValue=0;
         I3FaultValue=0;

         V2FaultPhase=CaculatePhase(Vb.y,Vb.x);
         V1FaultPhase=V2FaultPhase+120;
         V3FaultPhase=V2FaultPhase-120;
        }
        else
         {
             I1FaultValue=0;
             I3FaultValue=0;
             COMPLEX Vb=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
             COMPLEX Vc=VbCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
             COMPLEX Va=VcCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);

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

         ErrorFlag=ErrorWarning(4,2);
         if(ErrorFlag==1)
         return 1;

         I2FaultPhase=0-DelicacyAngle4-Angel_C;
         if(k==-1)
           I2FaultPhase=I2FaultPhase-180;
         I1FaultPhase=I2FaultPhase+120;
         I3FaultPhase=I2FaultPhase-120;
         LNNormalFaultPar();//故障前和故障后参数

        break;
      case 24://4娈电殑C-E鏁呴殰
               LNNormalFaultPar();//故障前和故障后参数
               V1Output=1;
               V2Output=1;
               V3Output=1;
               I1Output=0;
               I2Output=0;
               I3Output=1;

               if(k==1)
                 Impedance=Impedance4*TestPoint1;
               else if(k==0)
                 Impedance=Impedance4*TestPoint2;
               else if(k==-1)
                 Impedance=Impedance4*TestPoint3;
               I3FaultValue=FixupCurrent4;
               //单相接地故障时，电流故障相I=I(固定电流)


               //调用出错处理程序，如果返回1，说明参数设置不对。
               if(Busvolcon==true)
              {
               COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
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
                   COMPLEX Vc=VaCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
                   COMPLEX Va=VbCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);
                   COMPLEX Vb=VcCaculate(EqualizeValue,KLphase,FixupCurrent4,Impedance,DelicacyAngle4);


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

               ErrorFlag=ErrorWarning(4,3);
               if(ErrorFlag==1)
               return 1;



               //单相接地故障时，φV1=φV2+120°;φV2=φV3+120°
               I3FaultPhase=0-DelicacyAngle4-Angel_C;
               if(k==-1)
                 I3FaultPhase=I3FaultPhase-180;

               I1FaultPhase=I3FaultPhase-120;
               I2FaultPhase=I3FaultPhase+120;
               //单相接地故障时，故障电流的相位=故障电压的相位-灵敏角。
               LNNormalFaultPar();//故障前和故障后参数

                  // return 0;
        break;
      case 25://4娈电殑A-B鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=0;

        if(k==1)
          Impedance=Impedance4*TestPoint1;
        else if(k==0)
          Impedance=Impedance4*TestPoint2;
        else if(k==-1)
          Impedance=Impedance4*TestPoint3;

        I1FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=I3PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊


        COMPLEX Va1=V1LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        COMPLEX Vb1=V2LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va1.x*Va1.x+Va1.y*Va1.y);
        V2FaultValue=sqrt(Vb1.x*Vb1.x+Vb1.y*Vb1.y);



        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultValue=V3PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V3FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅
        V1FaultPhase=CaculatePhase(Va1.y,Va1.x);
        V2FaultPhase=CaculatePhase(Vb1.y,Vb1.x);

        V2FaultPhase=V2FaultPhase;
        V1FaultPhase=V1FaultPhase;




        ErrorFlag=ErrorWarning(4,4);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;





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



        I3FaultPhase=0;
        I1FaultPhase=-90-DelicacyAngle4;
        I2FaultPhase=90-DelicacyAngle4;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 26://4娈电殑B-C鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=0;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance4*TestPoint1;
        else if(k==0)
          Impedance=Impedance4*TestPoint2;
        else if(k==-1)
          Impedance=Impedance4*TestPoint3;

        I2FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I1FaultValue=I1PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V1FaultValue=V1PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V1FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅

        COMPLEX Vb2=V1LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        COMPLEX Vc2=V2LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        V2FaultValue=sqrt(Vb2.x*Vb2.x+Vb2.y*Vb2.y);
        V3FaultValue=sqrt(Vc2.x*Vc2.x+Vc2.y*Vc2.y);


        ErrorFlag=ErrorWarning(4,5);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V2FaultPhase=CaculatePhase(Vb2.y,Vb2.x);
        V3FaultPhase=CaculatePhase(Vc2.y,Vc2.x);
        V2FaultPhase=V2FaultPhase;
        V3FaultPhase=V3FaultPhase;




        //鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅數鍘嬬殑鐩镐綅涓嶅彉锛
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


        I1FaultPhase=0;
        I2FaultPhase=-90-DelicacyAngle4;
        I3FaultPhase=90-DelicacyAngle4;
        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
       // return 0;
        }
        break;
      case 27://4娈电殑C-A鏁呴殰
      {  LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=0;
        I3Output=1;

        if(k==1)
          Impedance=Impedance4*TestPoint1;
        else if(k==0)
          Impedance=Impedance4*TestPoint2;
        else if(k==-1)
          Impedance=Impedance4*TestPoint3;

        I1FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent4;//鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=I2PerFaultValue;//鐩搁棿鏁呴殰鏃讹紝闈炴晠闅滅浉鐢垫祦鐨勫箙鍊鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佸箙鍊

        V2FaultValue=V2PerFaultValue;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇骞呭€鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬪箙鍊
        V2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢靛帇鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數鍘嬬殑鐩镐綅


        COMPLEX Vc3=V1LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        COMPLEX Va3=V2LLCaculate(FixupCurrent4,Impedance,SystemVolt);
        V1FaultValue=sqrt(Va3.x*Va3.x+Va3.y*Va3.y);
        V3FaultValue=sqrt(Vc3.x*Vc3.x+Vc3.y*Vc3.y);


        ErrorFlag=ErrorWarning(4,6);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€
        V3FaultPhase=CaculatePhase(Vc3.y,Vc3.x);
        V1FaultPhase=CaculatePhase(Va3.y,Va3.x);
        V1FaultPhase=V1FaultPhase;
        V3FaultPhase=V3FaultPhase;



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

        I2FaultPhase=0;//鐩搁棿鏁呴殰鏃闈炴晠闅滅浉鐢垫祦鐩镐綅=鏁呴殰鍓嶇浉搴旂殑鐩哥數娴佺殑鐩镐綅
        I3FaultPhase=-90-DelicacyAngle4;
        I1FaultPhase=90-DelicacyAngle4;
        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;

        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;
        LLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
        //return 0;
    }
        break;
      case 28://4娈电殑A-B-C鏁呴殰
     {   LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁

        V1Output=1;
        V2Output=1;
        V3Output=1;
        I1Output=1;
        I2Output=1;
        I3Output=1;

        if(k==1)
          Impedance=Impedance4*TestPoint1;
        else if(k==0)
          Impedance=Impedance4*TestPoint2;
        else if(k==-1)
          Impedance=Impedance4*TestPoint3;

        I1FaultValue=FixupCurrent4;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I2FaultValue=FixupCurrent4;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦
        I3FaultValue=FixupCurrent4;//涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐩哥數娴佺殑骞呭€鍥哄畾鐢垫祦

        V1FaultValue=FixupCurrent4*Impedance;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝U=I*Z锛堢浉鐢靛帇=鍥哄畾鐢垫祦*闃绘姉瀹氬€硷級
        V2FaultValue=V1FaultValue;
        V3FaultValue=V1FaultValue;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝V1銆乂2銆乂3骞呭€煎ぇ灏忕浉绛


        ErrorFlag=ErrorWarning(4,7);//鍒ゆ柇鍙傛暟鏄惁璁剧疆姝ｇ‘锛岀涓€涓弬鏁拌〃绀烘锛岀浜屼釜鍙傛暟琛ㄧず鏁呴殰绫诲瀷
         if(ErrorFlag==1)
        return 1;


        //璋冪敤鍑洪敊澶勭悊绋嬪簭锛屽鏋滆繑鍥锛岃鏄庡弬鏁拌缃笉瀵广€

        V1FaultPhase=V1PerFaultPhase;
        V2FaultPhase=V2PerFaultPhase;
        V3FaultPhase=V3PerFaultPhase;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鏁呴殰鐢靛帇鐩哥殑鐩镐綅绛変簬鏁呴殰鍓嶇浉搴旂殑鐩哥殑鐩镐綅銆
        I1FaultPhase=V1FaultPhase-DelicacyAngle4;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐏垫晱瑙掔瓑浜嶪1涓嶸1涔嬮棿鐨勫す瑙掞紝I1鐨勭浉浣嶇瓑浜嶸1鐨勭浉浣嶅噺鐏垫晱瑙
        I2FaultPhase=I1FaultPhase-120;
        I3FaultPhase=I1FaultPhase+120;
        if(k==-1)
          I1FaultPhase=I1FaultPhase-180;
        if(I1FaultPhase>180)
          I1FaultPhase=I1FaultPhase-360;
        else if(I1FaultPhase<-180)
          I1FaultPhase=I1FaultPhase+360;

        if(k==-1)
          I2FaultPhase=I2FaultPhase-180;
        if(I2FaultPhase>180)
          I2FaultPhase=I2FaultPhase-360;
        else if(I2FaultPhase<-180)
          I2FaultPhase=I2FaultPhase+360;

        if(k==-1)
          I3FaultPhase=I3FaultPhase-180;
        if(I3FaultPhase>180)
          I3FaultPhase=I3FaultPhase-360;
        else if(I3FaultPhase<-180)
          I3FaultPhase=I3FaultPhase+360;
        //涓夌浉鐩搁棿鏁呴殰鏃讹紝鐩镐綅鎴20搴﹀绉板叧绯汇€
        LLLNormalFaultPar();//鏁呴殰鍓嶅拰鏁呴殰鍚庡弬鏁
         }

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

    V1PerFaultPhase=V1FaultPhase-360*SystemFrec*2*PerFaultTime;
    V2PerFaultPhase=V2FaultPhase-360*SystemFrec*2*PerFaultTime;
    V3PerFaultPhase=V3FaultPhase-360*SystemFrec*2*PerFaultTime;
    V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
    V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
    V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;

    I1PerFaultPhase=I1FaultPhase-360*SystemFrec*2*PerFaultTime;
    I2PerFaultPhase=I2FaultPhase-360*SystemFrec*2*PerFaultTime;
    I3PerFaultPhase=I3FaultPhase-360*SystemFrec*2*PerFaultTime;
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

int Widget::ErrorWarning(int segment,int fault)
      {
        switch(segment)
          {
            case 1://1娈
              switch(fault)
                {
                  case 1://鏁呴殰绫诲瀷涓猴細A-E
                    if(V1FaultValue>SystemVolt)
                      {
                       //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇澶т簬SystemVolt 璇锋敼鍙樼數娴佸€
                        showwarnint(segment,1);
                        return 1;
                       }
                    break;
                  case 2://鏁呴殰绫诲瀷涓猴細B-E
                    if(V2FaultValue>SystemVolt)
                      {
                        //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇澶т簬SystemVolt 璇锋敼鍙樼數娴佸€
                        showwarnint(segment,1);
                        return 1;
                      }
                    break;
                  case 3: //鏁呴殰绫诲瀷涓猴細C-E
                    if(V3FaultValue>SystemVolt)
                      {
                         //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇澶т簬SystemVolt 璇锋敼鍙樼數娴佸€
                         showwarnint(segment,1);
                        return 1;
                      }

                    break;
                  case 4://鏁呴殰绫诲瀷涓猴細A-B
                    if(V1FaultValue>SystemVolt)
                      {
                         showwarnint(segment,1);
                        return 1;
                      }
                    if(V1FaultValue<SystemVolt/2)
                      {
                        //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇灏忎簬SystemVolt/2 璇锋敼鍙樼數娴佸€
                         showwarnint(segment,2);
                        return 1;
                      }
                    break;
                  case 5://鏁呴殰绫诲瀷涓猴細B-C
                    if(V2FaultValue>SystemVolt)
                      {//鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇澶т簬SystemVolt 璇锋敼鍙樼數娴佸€
                         showwarnint(segment,1);
                        return 1;
                      }
                    if(V2FaultValue<SystemVolt/2)
                      {
                        //鍙戦€佹彁绀轰俊鎭鐢ㄦ埛鏀瑰彉杈撳叆鍊1娈鐢靛帇灏忎簬SystemVolt/2 璇锋敼鍙樼數娴佸€
                         showwarnint(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 6://鏁呴殰绫诲瀷涓猴細C-A
                    if(V3FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                    if(V3FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         showwarnint(segment,2);
                        return 1;
                      }

                    break;
                  case 7://鏁呴殰绫诲瀷涓猴細A-B-C
                    if(V1FaultValue>SystemVolt)
                      {//涓夌浉鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                       showwarnint(segment,1);
                        return 1;
                      }
                    break;
                }
              break;
            case 2://2娈
              switch(fault)
                {
                  case 1://鏁呴殰绫诲瀷涓猴細A-E
                    if(V1FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }

                    break;
                  case 2://鏁呴殰绫诲瀷涓猴細B-E
                    if(V2FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 3://鏁呴殰绫诲瀷涓猴細C-E
                    if(V3FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 4://鏁呴殰绫诲瀷涓猴細A-B
                    if(V1FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                       showwarnint(segment,1);
                        return 1;
                      }
                    if(V1FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        showwarnint(segment,2);
                        return 1;
                      }
                    break;
                  case 5://鏁呴殰绫诲瀷涓猴細B-C
                    if(V2FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                       showwarnint(segment,1);
                        return 1;
                      }
                    if(V2FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        showwarnint(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 6://鏁呴殰绫诲瀷涓猴細C-A
                    if(V3FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                       showwarnint(segment,1);
                        return 1;
                      }
                    if(V3FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        showwarnint(segment,2);
                        return 1;
                      }
                   /* else
                      return 0;*/
                    break;
                  case 7://鏁呴殰绫诲瀷涓猴細A-B-C
                    if(V1FaultValue>SystemVolt)
                      {//涓夌浉鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                       showwarnint(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                }
              break;
            case 3://3娈
              switch(fault)
                {
                  case 1://鏁呴殰绫诲瀷涓猴細A-E
                    if(V1FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                   /* else
                      return 0;*/
                    break;
                  case 2://鏁呴殰绫诲瀷涓猴細B-E
                    if(V2FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 3://鏁呴殰绫诲瀷涓猴細C-E
                    if(V3FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                   /* else
                      return 0;*/
                    break;
                  case 4://鏁呴殰绫诲瀷涓猴細A-B
                    if(V1FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                    if(V1FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         showwarnint(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 5://鏁呴殰绫诲瀷涓猴細B-C
                    if(V2FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                    if(V2FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         showwarnint(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 6://鏁呴殰绫诲瀷涓猴細C-A
                    if(V3FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         showwarnint(segment,1);
                        return 1;
                      }
                    if(V3FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         showwarnint(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 7://鏁呴殰绫诲瀷涓猴細A-B-C
                    if(V1FaultValue>SystemVolt)
                      {//涓夌浉鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                         showwarnint(segment,1);
                        return 1;
                      }
                    break;
                }
              break;
            case 4://4娈
              switch(fault)
                {
                  case 1://鏁呴殰绫诲瀷涓猴細A-E
                    if(V1FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 2://鏁呴殰绫诲瀷涓猴細B-E
                    if(V2FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                         showwarnint(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 3://鏁呴殰绫诲瀷涓猴細C-E
                    if(V3FaultValue>SystemVolt)
                      {//鍗曠浉鎺ュ湴鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                         showwarnint(segment,1);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 4://鏁呴殰绫诲瀷涓猴細A-B
                    if(V1FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         showwarnint(segment,1);
                        return 1;
                      }
                    if(V1FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        showwarnint(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 5://鏁呴殰绫诲瀷涓猴細B-C
                    if(V2FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                    if(V2FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         showwarnint(segment,2);
                        return 1;
                      }
                    /*else
                      return 0;*/
                    break;
                  case 6://鏁呴殰绫诲瀷涓猴細C-A
                    if(V3FaultValue>SystemVolt)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                        showwarnint(segment,1);
                        return 1;
                      }
                    if(V3FaultValue<SystemVolt/2)
                      {//鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0.5*Vnom~Vnom闂
                         showwarnint(segment,2);
                        return 1;
                      }
                    break;
                  case 7://鏁呴殰绫诲瀷涓猴細A-B-C
                    if(V1FaultValue>SystemVolt)
                      {//涓夌浉鐩搁棿鏁呴殰鏃讹紝鍙戠敓鏁呴殰鏃舵晠闅滅浉鐨勫箙鍊艰寖鍥村湪0~Vnom闂
                         showwarnint(segment,1);
                        return 1;
                      }
                    break;
                }
              break;
          }
        return 0;
      }

void Widget::on_tabWidget_currentChanged(int index)
{

 if(index!=1)
     return;




 ui->tabWidget->setCurrentIndex(1);

 if(starttest==1)
     ui->pushButton_7->setEnabled(false);
 else
     ui->pushButton_7->setEnabled(true);



}

void Widget::ShowHeader(QString str)
{
 ui->textEdit->append(str);
}
void Widget::Showzone(int i)
{
    ui->textEdit->setReadOnly(false);
ShowCaculateValue(i);
ui->textEdit->moveCursor(QTextCursor::Start);//光标跳转到开始？？？
ui->textEdit->setReadOnly(true);
}

void Widget::on_pushButton_6_clicked()
{
    if(SoftStartEndflag==true)
            return;
        if(starttest==1)
        {
            ui->pushButton_6->setText(tr("测试"));
            ui->pushButton_6->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
            ui->pushButton->setEnabled(true);
            ui->pushButton_9->setEnabled(true);
            ui->pushButton_3->setEnabled(true);
        }
        else
        {
            ui->pushButton_6->setText(tr("停止"));
            ui->pushButton_6->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
            ui->pushButton->setEnabled(false);
            ui->pushButton_9->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            on_pushButton_7_clicked();
        }

        SoftStartEndflag=true;

}

void Widget::GetTestState(){
    if(starttest==1)
    {
        ui->pushButton_6->setText(tr("停止"));
        ui->pushButton_6->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;background-color:rgb(150,255,255);color:black;border-radius:2px;border:2px groove gray;border-style:outset;}");
        ui->pushButton->setEnabled(false);
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
    }
    else
    {
        ui->pushButton_6->setText(tr("测试"));
        ui->pushButton_6->setStyleSheet("QPushButton{font-family:'wenquanyi';font-size:16px;}");
        ui->pushButton->setEnabled(true);
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }

}
//update
void Widget::on_pushButton_7_clicked()
{
    ui->textEdit->clear();




    if(Zone1==1)
    {
     int testpint[3]={1,0,-1};
     for(int j=0;j<3;j++)
    {


      switch(j)
      {
      case 0:

          ShowHeader(tr("1段 ")+tr("测试点 1:  ")+QString::number(TestPoint1));
          break;
      case 1:
          ShowHeader(tr("1段 ")+tr("测试点 2:  ")+QString::number(TestPoint2));
          break;
      case 2:
          ShowHeader(tr("1段 ")+tr("测试点 3:  ")+QString::number(TestPoint3));
          break;
      }


      if(FaultType1==1)
     {//A-E
      usleep(100);
      OutputSignalCompute(1,testpint[j]);
      CaculatePerfalultPhase();

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

      Showzone(1);
     }
      if(FaultType2==1)
     {//B-E
       usleep(100);
      OutputSignalCompute(2,testpint[j]);
      CaculatePerfalultPhase();

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

      Showzone(2);
     }
      if(FaultType3==1)
     {//C-E
      usleep(100);
      OutputSignalCompute(3,testpint[j]);
      CaculatePerfalultPhase();

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

     Showzone(3);
     }
      if(FaultType4==1)
     {//A-B-E
      usleep(100);
      OutputSignalCompute(4,testpint[j]);
      CaculatePerfalultPhase();

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

      Showzone(4);
     }
      if(FaultType5==1)
     {//B-C-E
      usleep(100);
      OutputSignalCompute(5,testpint[j]);
      CaculatePerfalultPhase();

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

     Showzone(5);
     }
      if(FaultType6==1)
     {//C-A-E
      usleep(100);
      OutputSignalCompute(6,testpint[j]);
      CaculatePerfalultPhase();

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

      Showzone(6);
     }
      if(FaultType7==1)
     {//A-B-C-E
      usleep(100);
      OutputSignalCompute(7,testpint[j]);
      CaculatePerfalultPhase();
      Showzone(7);
     }

      ShowHeader(tr("\n"));





    }

    }

    if(Zone2==1)
    {
        int testpint[3]={1,0,-1};
        for(int j=0;j<3;j++)
       {


         switch(j)
         {
         case 0:

             ShowHeader(tr("2段 ")+tr("测试点 1:  ")+QString::number(TestPoint1));
             break;
         case 1:
             ShowHeader(tr("2段 ")+tr("测试点 2:  ")+QString::number(TestPoint2));
             break;
         case 2:
             ShowHeader(tr("2段 ")+tr("测试点 3:  ")+QString::number(TestPoint3));
             break;
         }

         if(FaultType1==1)
        {//A-E
          usleep(100);
         OutputSignalCompute(1+7,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(1);
        }
         if(FaultType2==1)
        {//B-E
          usleep(100);
         OutputSignalCompute(2+7,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(2);
        }
         if(FaultType3==1)
        {//C-E
          usleep(100);
         OutputSignalCompute(3+7,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(3);
        }
         if(FaultType4==1)//A-B-E
        { usleep(100);
         OutputSignalCompute(4+7,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(4);
        }
         if(FaultType5==1)
        {//B-C-E
          usleep(100);
         OutputSignalCompute(5+7,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(5);
        }
         if(FaultType6==1)
        {//C-A-E
          usleep(100);
         OutputSignalCompute(6+7,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(6);
        }
         if(FaultType7==1)
        {
          usleep(100);
         OutputSignalCompute(7+7,testpint[j]);
         CaculatePerfalultPhase();
         Showzone(7);
        }

         ShowHeader(tr("\n"));

       }




    }

    if(Zone3==1)
    {
         int testpint[3]={1,0,-1};
        for(int j=0;j<3;j++)
       {


         switch(j)
         {
         case 0:

             ShowHeader(tr("3段 ")+tr("测试点 1:  ")+QString::number(TestPoint1));
             break;
         case 1:
             ShowHeader(tr("3段 ")+tr("测试点 2:  ")+QString::number(TestPoint2));
             break;
         case 2:
             ShowHeader(tr("3段 ")+tr("测试点 3:  ")+QString::number(TestPoint3));
             break;
         }

         if(FaultType1==1)
        {
          usleep(100);
         OutputSignalCompute(1+14,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(1);
        }
         if(FaultType2==1)
        {
          usleep(100);
         OutputSignalCompute(2+14,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(2);
        }
         if(FaultType3==1)
        {
          usleep(100);
         OutputSignalCompute(3+14,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(3);
        }
         if(FaultType4==1)
        {
          usleep(100);
         OutputSignalCompute(4+14,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(4);
        }
         if(FaultType5==1)
        {
          usleep(100);
         OutputSignalCompute(5+14,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(5);
        }
         if(FaultType6==1)
        {
          usleep(100);
         OutputSignalCompute(6+14,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(6);
        }
         if(FaultType7==1)
        {
          usleep(100);
         OutputSignalCompute(7+14,testpint[j]);
         CaculatePerfalultPhase();
         Showzone(7);
        }
         ShowHeader(tr("\n"));

       }



    }

    if(Zone4==1)
    {

        int testpint[3]={1,0,-1};
        for(int j=0;j<3;j++)
       {


         switch(j)
         {
         case 0:

             ShowHeader(tr("4段 ")+tr("测试点 1:  ")+QString::number(TestPoint1));
             break;
         case 1:
             ShowHeader(tr("4段 ")+tr("测试点 2:  ")+QString::number(TestPoint2));
             break;
         case 2:
             ShowHeader(tr("4段 ")+tr("测试点 3:  ")+QString::number(TestPoint3));
             break;
         }

         if(FaultType1==1)
        {
          usleep(100);
         OutputSignalCompute(1+21,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(1);
        }
         if(FaultType2==1)
        {
          usleep(100);
         OutputSignalCompute(2+21,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(2);
        }
         if(FaultType3==1)
        {
          usleep(100);
         OutputSignalCompute(3+21,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(3);
        }
         if(FaultType4==1)
        {
           usleep(100);
         OutputSignalCompute(4+21,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(4);
        }
         if(FaultType5==1)
        {
          usleep(100);
         OutputSignalCompute(5+21,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(5);
        }
         if(FaultType6==1)
        {
          usleep(100);
         OutputSignalCompute(6+21,testpint[j]);
         CaculatePerfalultPhase();

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

         Showzone(6);
        }
         if(FaultType7==1)
        {
          usleep(100);
         OutputSignalCompute(7+21,testpint[j]);
         CaculatePerfalultPhase();
         Showzone(7);
        }

         ShowHeader(tr("\n"));

       }

    }

}
