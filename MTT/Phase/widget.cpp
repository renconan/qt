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
int   FreqTriple=2;
int   starttest=0;
int   seq_count,V1DC,V2DC,V3DC,I1DC,I2DC,I3DC;
int   TestItem1,TestItem2;


float V1Value,V2Value,V3Value,I1Value,I2Value,I3Value;
float TripTime1,TripTime2,TripTime3;
float ChangeValue,timeout;
int save_f;
int input_par=0;
int HelpFlag=0;
int sys[4];

float LogicResolution,SystemFrec,SystemVolt;//2009-6-20
char  V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;



float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;

float PENDSTART;
float PENDEND;
float PSTEP;
float PEND;
float ReturnStep;
float STEPTIME;
float faultduration=0;

int IACH=1;
int IBCH=0;
int ICCH=0;
int VACH=1;
int VBCH=0;
int VCCH=0;
float currentphrase=0;
float prefaulttime=3; //故障前
float postfaulttime=3; //故障后时间
float Preparetime;
float FixVoltage=57;
float FixCurrent=2;

int   TriptimeType=2; //测试时间的方式
float StopPhase=6; //测试动作时间的相位
float TripTimeHoldtime=5; //做动作时间的保持时间



bool Threadrunstate=true;

bool SoftStartEndflag=false;



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

  setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
  /* 加载语言文件 默认中文 2016-9-7*/
  init_par();
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
  setFixedSize(800,480);

      //init_par();
      showFullScreen();
      ui->pushButton_6->hide();
      ui->pushButton_7->hide();

  //ui->label_13->setText("<font color=blue>"+QString("Phase Shift Test(2.0.0)")+"</font>");


  ui->lineEdit_1->setText(QString::number(PENDSTART));
  ui->lineEdit_2->setText(QString::number(PENDEND));
  ui->lineEdit_3->setText(QString::number(PSTEP));
  ui->lineEdit_4->setText(QString::number(STEPTIME));
  ui->lineEdit_6->setText(QString::number(prefaulttime));
  ui->lineEdit_7->setText(QString::number(FixVoltage));
  ui->lineEdit_8->setText(QString::number(FixCurrent));
  ui->lineEdit_9->setText(QString::number(StopPhase));
  ui->lineEdit_10->setText(QString::number(TripTimeHoldtime));
  ui->lineEdit_11->setText(QString::number(postfaulttime));




  ui->lineEdit_1->installEventFilter(this);
  ui->lineEdit_2->installEventFilter(this);
  ui->lineEdit_3->installEventFilter(this);
  ui->lineEdit_4->installEventFilter(this);
  ui->lineEdit_6->installEventFilter(this);
  ui->lineEdit_7->installEventFilter(this);
  ui->lineEdit_8->installEventFilter(this);
  ui->lineEdit_9->installEventFilter(this);
  ui->lineEdit_10->installEventFilter(this);
  ui->lineEdit_11->installEventFilter(this);


  ui->lineEdit_1->setContextMenuPolicy(Qt::NoContextMenu);
  ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
  ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
  ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
  ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);
  ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);
  ui->lineEdit_8->setContextMenuPolicy(Qt::NoContextMenu);
  ui->lineEdit_9->setContextMenuPolicy(Qt::NoContextMenu);
  ui->lineEdit_10->setContextMenuPolicy(Qt::NoContextMenu);
  ui->lineEdit_11->setContextMenuPolicy(Qt::NoContextMenu);


  currenteditpos=1;

  ui->pushButton->setFocusPolicy(Qt::NoFocus);
  ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
  ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
  ui->pushButton_4->setFocusPolicy(Qt::NoFocus);




  if(VACH)
     ui->checkBox->setChecked(true);
  else ui->checkBox->setChecked(false);

  if(VBCH)
     ui->checkBox_2->setChecked(true);
  else ui->checkBox_2->setChecked(false);
  if(VCCH)
     ui->checkBox_3->setChecked(true);
  else ui->checkBox_3->setChecked(false);

  if(IACH)
     ui->checkBox_4->setChecked(true);
  else ui->checkBox_4->setChecked(false);

  if(IBCH)
     ui->checkBox_5->setChecked(true);
  else ui->checkBox_5->setChecked(false);

  if(ICCH)
     ui->checkBox_6->setChecked(true);
  else ui->checkBox_6->setChecked(false);



  ui->checkBox->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
  ui->checkBox_2->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
  ui->checkBox_3->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
  ui->checkBox_4->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
  ui->checkBox_5->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
  ui->checkBox_6->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");



  uartthread =new thread1;
  connect(uartthread,SIGNAL(SIG_testresult(int)),SLOT(showtestresult(int)));
  connect(uartthread,SIGNAL(SIG_DisplayChangeValue(bool,float)),SLOT(DisplayChangeValue(bool,float)));
  connect(uartthread,SIGNAL(SIG_dspstate(int)),SLOT(showdspstate(int)));
  connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
  connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));

  uartthread->start();


     ui->tabWidget->setCurrentIndex(0);
     TestItem1=1;
     TestItem2=0;



}

void Widget::updateUI()
{
    ui->label_13->setText("<font color=blue>"+QString(tr("相位测试(2.1.0)"))+"</font>");
    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_2->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_4->setText(tr("测试"));

        ui->label->setText(tr("测试相位(°)："));
        ui->label_15->setText(tr("最长时间(s):"));

        ui->label_5->setText(tr("相位初值(°)："));
        ui->label_6->setText(tr("相位终值(°)："));
        ui->label_7->setText(tr("相位步长(°)："));
        ui->label_8->setText(tr("故障时间(s):"));

        ui->label_12->setText(tr("故障前时间(s):"));
        ui->label_14->setText(tr("故障后时间(s):"));
        ui->label_4->setText(tr("电压(V):"));
        ui->label_11->setText(tr("电流(A):"));


        ui->groupBox->setTitle(tr("变化相"));
        ui->groupBox_4->setTitle(tr("测试结果"));

        ui->tabWidget->setTabText(0,tr("动作值"));
        ui->tabWidget->setTabText(1,tr("动作时间"));
}

void Widget::moveEvent(QMoveEvent *event)
{
 QPoint oldpos=event->oldPos();
 this->move(oldpos);
}


void Widget::init_par(void)
{
    FILE *fp;
    SystemVolt=57.735;
    if((fp=fopen(SYSSET,"a+"))==NULL)
      return;
    fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
           &SystemVolt,&LogicResolution,&prefaulttime,&postfaulttime,&Preparetime,&current_language);
    SystemFrec=SystemFrec*FreqTriple;
    fclose(fp);
    V1Value=SystemVolt;
    V2Value=SystemVolt;
    V3Value=SystemVolt;
    I1Value=0.5;
    I2Value=0.5;
    I3Value=0.5;
    TestItem1=1;
    TestItem2=1;
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


    PENDSTART=6;
    PENDEND=9;
    PSTEP=0.1;
    ReturnStep=0.1;
    STEPTIME=1;
    faultduration=5;
    IACH=1;
    IBCH=0;
    ICCH=0;
    VACH=1;
    VBCH=0;
    VCCH=0;
    currentphrase=PENDSTART;
}

void Widget::DisplayChangeValue(bool flag,float value)
{
    if(flag)
    {
        ui->label_3->setText(QString::number(value,'g',6));
    }
    else ui->label_3->clear();
}


void Widget::showtestresult(int num)
{
    switch(num)
      {
        case 0:
             ui->label_9->clear();
             ui->label_10->clear();
             break;
        case 1://显示动作值
          if(TripTime1==-1)
            {
               ui->label_9->setText(tr("trip value(Deg):no trip"));
            }
          else
            {
               if(PENDSTART>=PENDEND)
               ui->label_9->setText(tr("trip value(Deg):")+QString::number(currentphrase+PSTEP,'g',5));
               else
               ui->label_9->setText(tr("trip value(Deg):")+QString::number(currentphrase-PSTEP,'g',3));


            }
          break;
        case 2://显示动作时间
          if(TestItem1==1)
            {   //如果测试了动作值、返回值，则动作时间的显示位置如下。
              if(TripTime2==-1)
              ui->label_10->setText(tr("trip time(s):no trip"));

              else
               {
              ui->label_10->setText(tr("trip time(s):")+QString::number(TripTime2,'g',4));
               }

           }
          else if(TestItem1==0)
          {
              if(TripTime2==-1)
              ui->label_9->setText(tr("trip time(s):no trip"));

              else
               {
              ui->label_9->setText(tr("trip time(s):")+QString::number(TripTime2,'g',4));
               }
          }
    }
}


void Widget::showdspstate(int state)
{
    switch(state)
    {
       case 0: ui->label_2->setText(tr("run normally")); break;
       case 1: ui->label_2->setText(tr("IA overload")); break;
       case 2: ui->label_2->setText(tr("IABC temperature protect")); break;
       case 3: ui->label_2->setText(tr("IC resume")); break;
       case 4: ui->label_2->setText(tr("IB overload")); break;
       case 5: ui->label_2->setText(tr("IC closedown")); break;
       case 6: ui->label_2->setText(tr("IA resume")); break;
       case 7: ui->label_2->setText(tr("IC overload")); break;
       case 8: ui->label_2->setText(tr("IABC power protection")); break;
       case 9: ui->label_2->setText(tr("IB resume")); break;
       case 10: ui->label_2->setText(tr("VA overload")); break;

       case 12: ui->label_2->setText(tr("VC resume ")); break;
       case 13: ui->label_2->setText(tr("VB overload ")); break;
       case 14: ui->label_2->setText(tr("VABC temperature protection")); break;
       case 15: ui->label_2->setText(tr("VA resume")); break;
       case 16: ui->label_2->setText(tr("VC overload")); break;
       case 17: ui->label_2->setText(tr("VABC power protection")); break;
       case 18: ui->label_2->setText(tr("Vz overload")); break;
       case 19: ui->label_2->setText(tr("direct current overload")); break;
       case 20: ui->label_2->setText(tr("IABC power protection")); break;
       case 21: ui->label_2->setText(tr("VC resume")); break;
    }


}


Widget::~Widget()
{
    delete ui;
}

void Widget::vi_parameter_enter(int position)
{

    switch(position)
    {
    case 1:
      if(PENDSTART>360)
         PENDSTART=360;
      //相位小于0 不做处理

      if((PENDSTART>0&&PENDEND<0)||(PENDSTART<0&&PENDEND>0))
      {
         if(PENDSTART>0)
             PENDEND=PENDSTART+PSTEP;
         else
             PENDEND=PENDSTART-PSTEP;
      }

      break;
   case 2:
        if(PENDEND>360)
          PENDEND=360;

        if((PENDSTART>0&&PENDEND<0)||(PENDSTART<0&&PENDEND>0))
        {
           if(PENDEND>0)
               PENDSTART=PENDEND-PSTEP;
           else
               PENDSTART=PENDEND+PSTEP;
        }


      break;
    case 3:
            if(PSTEP>180)
              PSTEP=180;
            else if(PSTEP<0)
              PSTEP=1;
            break;
    case 4:
            if(STEPTIME>999.99)
               STEPTIME=999.99;
            else if(STEPTIME<0)
               STEPTIME=0;
            break;

   case 6:
        if(prefaulttime>999.99)
          prefaulttime=999.99;
        else if(prefaulttime<0)
          prefaulttime=0;
        break;
   case 7:
        if(FixVoltage>MAX_V_VALUEDATA_AC)
          FixVoltage=MAX_V_VALUEDATA_AC;
           else if(FixVoltage<0)
          FixVoltage=0;
          break;
   case 8:
         if(FixCurrent>MAX_I_VALUEDATA_AC)
           FixCurrent=MAX_I_VALUEDATA_AC;
            else if(FixCurrent<0)
           FixCurrent=0;
           break;
   case 9:
         if(StopPhase>999.99)
           StopPhase=999.99;
         else if(StopPhase<0)
           StopPhase=0;
         break;
    case 10:
          if(TripTimeHoldtime>999.99)
            TripTimeHoldtime=999.99;
          else if(TripTimeHoldtime<0)
            TripTimeHoldtime=0;
          break;
     case 11:
        if(postfaulttime>999.99)
          postfaulttime=999.99;
        else if(postfaulttime<0)
          postfaulttime=0;
        break;
}
}





void Widget::on_checkBox_clicked()
{
if(VACH==1)
    VACH=0;
else VACH=1;
}

void Widget::on_checkBox_2_clicked()
{
    if(VBCH==1)
        VBCH=0;
    else VBCH=1;
}

void Widget::on_checkBox_3_clicked()
{
    if(VCCH==1)
        VCCH=0;
    else VCCH=1;
}

void Widget::on_checkBox_4_clicked()
{
    if(IACH==1)
        IACH=0;
    else IACH=1;
}

void Widget::on_checkBox_5_clicked()
{
    if(IBCH==1)
        IBCH=0;
    else IBCH=1;
}

void Widget::on_checkBox_6_clicked()
{
    if(ICCH==1)
        ICCH=0;
    else ICCH=1;
}




void Widget::on_pushButton_2_clicked()
{
   if(save_f==0)
       return ;
   save_f=0;
   if(current_language==2)
    {
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
    fprintf(fp,"Date:%d.%d.%d\r\n",mon,day,year);

    hour=QTime::currentTime().hour();
    minute=QTime::currentTime().minute();
    second=QTime::currentTime().second();
    fprintf(fp,"Time:%d.%d.%d\r\n",hour,minute,second);
    strcpy(strbuf,"Phase test result");
    fprintf(fp,"%s\r\n",strbuf);

    if(TestItem1==1)
    {
      strcpy(strbuf,"Trip value(Deg):");
      fprintf(fp,"%s",strbuf);
      if(TripTime1==-1)
        strcpy(strbuf,"No Trip");
      else
        {
        if(PENDSTART>=PENDEND)
        gcvt(currentphrase+PSTEP,7,strbuf);
        else
        gcvt(currentphrase-PSTEP,7,strbuf);

        }
      fprintf(fp,"%s\r\n",strbuf);
    }
    if(TestItem2==1)
    {
      strcpy(strbuf,"Trip time(s):");
      fprintf(fp,"%s",strbuf);
      if(TripTime2==-1)
        strcpy(strbuf,"No trip");
      else
        gcvt(TripTime2 ,7,strbuf);
      fprintf(fp,"%s\r\n",strbuf);
    }
    fclose(fp);
   }
   else if(current_language==1)
   {
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
       fprintf(fp,"Date:%d.%d.%d\r\n",mon,day,year);

       hour=QTime::currentTime().hour();
       minute=QTime::currentTime().minute();
       second=QTime::currentTime().second();
       fprintf(fp,"Time:%d.%d.%d\r\n",hour,minute,second);
       strcpy(strbuf,"相位测试结果:");
       fprintf(fp,"%s\r\n",strbuf);

       if(TestItem1==1)
       {
         strcpy(strbuf,"动作值(度):");
         fprintf(fp,"%s",strbuf);
         if(TripTime1==-1)
           strcpy(strbuf,"未动作");
         else
           {
           if(PENDSTART>=PENDEND)
           gcvt(currentphrase+PSTEP,7,strbuf);
           else
           gcvt(currentphrase-PSTEP,7,strbuf);

           }
         fprintf(fp,"%s\r\n",strbuf);
       }
       if(TestItem2==1)
       {
         strcpy(strbuf,"动作时间(s):");
         fprintf(fp,"%s",strbuf);
         if(TripTime2==-1)
           strcpy(strbuf,"未动作");
         else
           gcvt(TripTime2 ,7,strbuf);
         fprintf(fp,"%s\r\n",strbuf);
       }
       fclose(fp);
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
        PENDSTART=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PENDSTART,'g',6));
        ui->lineEdit_2->setText(QString::number(PENDEND,'g',6));
        break;
    case 2:
        PENDEND=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PENDEND,'g',6));
        ui->lineEdit_1->setText(QString::number(PENDSTART,'g',6));
        break;
    case 3:
        PSTEP=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(PSTEP,'g',6));
        break;
    case 4:
        STEPTIME=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(STEPTIME,'g',6));
        break;

    case 6:
        prefaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(prefaulttime,'g',6));
        break;
    case 7:
        FixVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixVoltage,'g',6));
        break;
    case 8:
        FixCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixCurrent,'g',6));
        break;
    case 9:
        StopPhase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StopPhase,'g',6));
        break;
    case 10:
        TripTimeHoldtime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(TripTimeHoldtime,'g',6));
        break;
    case 11:
        postfaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(postfaulttime,'g',6));
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
helpdialog->SetHelpfilename("Phase.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("Phase1.txt");
//helpdialog->SetHelpfilename("Phase.txt");
helpdialog->show();
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
        if(obj==ui->lineEdit_1)
        {     temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_1;
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



void Widget::on_tabWidget_currentChanged(int index)
{

    if(index==0)
    {
        TestItem1=1;
        TestItem2=0;
    }
    else
    {
        TestItem1=0;
        TestItem2=1;
    }
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
