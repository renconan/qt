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
#include <QMessageBox>
#include <unistd.h>

#include "qtranslator.h"
QTranslator *tor;
int current_language=2;  //1中文  2英文

extern TEMPRORARYDATA *temprorarydata;

float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
float V1RelaPhase=0,V2RelaPhase=0,V3RelaPhase=0,VzRelaPhase=0,I1RelaPhase=0,I2RelaPhase=0,I3RelaPhase=0;



unsigned char result[70],receive_data[70];
double out_time,change_timedata;

float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;
float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
int   input_par=0,Decimal=0,sige_flag=0,HelpFlag=0;

int   INPAR=0;
int   starttest=0;

int   V1DC,V2DC,V3DC,VzDC,I1DC,I2DC,I3DC,SequencerCount;
float LogicResolution,SystemFrec,Angle1,Angle2;
int exit_par,save_f;
int sys[4];
char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
float prefaulttime=3;
float postfaulttime=3;
float Preparetime=3;
float FixCurrent,FixVoltage,MSAngle,StartPhase,EndPhase,OutPhase,StepPhase,StepTime=5;


float Postfaulttime=3;
int   dcouttype=1;


float  SystemVolt=57;
float  faultduration=0;
int    faulttype=1; //A-E

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
    setFixedSize(800,480);
    //init_par();


     showFullScreen();
     ui->label_2->hide();
     ui->label_7->hide();
     ui->label_8->hide();
     ui->label_14->hide();
     ui->label_16->hide();
     ui->label_17->hide();
     ui->label_18->hide();
     ui->label_19->hide();

     QPixmap img(":/picture/hei.png");
     ui->label_2->setPixmap(img);
     ui->label_7->setPixmap(img);
     ui->label_8->setPixmap(img);
     ui->label_14->setPixmap(img);

    //ui->label_5->setText("<font color=blue>"+QString("功率方向(2.2.1)")+"</font>");



    ui->lineEdit->setText(QString::number(FixCurrent));
    ui->lineEdit_2->setText(QString::number(FixVoltage));
    ui->lineEdit_3->setText(QString::number(StartPhase));

    ui->lineEdit_5->setText(QString::number(StepPhase));
    ui->lineEdit_6->setText(QString::number(StepTime));
    ui->lineEdit_7->setText(QString::number(prefaulttime));

    faulttype=1;
    ui->radioButton->setChecked(true);





        ui->lineEdit->installEventFilter(this);
        ui->lineEdit_2->installEventFilter(this);
        ui->lineEdit_3->installEventFilter(this);

        ui->lineEdit_5->installEventFilter(this);
        ui->lineEdit_6->installEventFilter(this);
        ui->lineEdit_7->installEventFilter(this);

        ui->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
       // ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_5->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);

     ui->label_11->clear();
     ui->label_4->clear();
     //ui->pushButton->setFocusPolicy(Qt::NoFocus);
      //ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
       //ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
       //ui->pushButton_4->setFocusPolicy(Qt::NoFocus);


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
    ui->label_5->setText("<font color=blue>"+QString(tr("功率方向(2.2.1)"))+"</font>");
    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("帮助"));
        ui->pushButton_2->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));
        ui->pushButton_4->setText(tr("测试"));

        ui->groupBox_4->setTitle(tr("测试结果"));

        ui->label->setText(tr("电流(A):"));
        ui->label_3->setText(tr("电压(V):"));
        ui->label_13->setText(tr("起始角度(Deg):"));
        ui->label_15->setText(tr("步长(Deg):"));
        ui->label_12->setText(tr("步长时间(s):"));
        ui->label_10->setText(tr("故障前时间(s):"));
        ui->label_9->setText(tr("故障类型:"));
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
        ui->label_2->setPixmap(img);
        ui->label_7->setPixmap(img);
        ui->label_8->setPixmap(img);
        ui->label_14->setPixmap(img);
        break;
    case 101: ui->label_2->setPixmap(pic); break;
    case 102: ui->label_7->setPixmap(pic); break;
    case 103: ui->label_8->setPixmap(pic); break;
    case 104: ui->label_14->setPixmap(pic); break;
    case 111: ui->label_2->setPixmap(img); break;
    case 112: ui->label_7->setPixmap(img); break;
    case 113: ui->label_8->setPixmap(img); break;
    case 114: ui->label_14->setPixmap(img); break;
    }


}


void Widget::DisplayChangeValue(bool flag, float value)
{
    if(flag)
    {
        int temp;
       if(value>360)
       {
           temp=(int)(value/360);
           value=value-360*temp;
       }
       else if(value<-360)
       {
           temp=(int)(value/360);
           value=value+360*temp;
       }
        ui->label_11->setText(tr("当前相位(Deg):"));
        ui->label_4->setText(QString::number(value));
    }
    else
      {  ui->label_11->clear();
         ui->label_4->clear();

      }

}

void Widget::showtestresult(int num)
{

 ui->textBrowser->clear();
 if(num==1)
  ui->textBrowser->append(tr("边界角1(Deg):")+QString::number(Angle1,'g',6));

 else  if(num==2)
 {
     if(Angle1>360)
       Angle1=Angle1-360;
     else if(Angle1<-360)
       Angle1+=360;
     if(Angle2>360)
       Angle2=Angle2-360;
     else if(Angle2<-360)
       Angle2+=360;



  ui->textBrowser->append(tr("边界角1(Deg):")+QString::number(Angle1,'g',6));
  ui->textBrowser->append(tr("边界角2(Deg):")+QString::number(Angle2,'g',6));

  MSAngle=(Angle1+Angle2)/2;
  if(MSAngle>360)
    MSAngle=MSAngle-360;
  else if(MSAngle<-360)
    MSAngle+=360;

   MSAngle=-MSAngle;
  if(MSAngle<-180)
      MSAngle=MSAngle+360;
  else if(MSAngle>180)
      MSAngle=MSAngle-360;
  ui->textBrowser->append(tr("灵敏角(Deg):")+QString::number(MSAngle,'g',6));

 }

}


void Widget::init_par()
{

    FILE *fp;
    if((fp=fopen("sysset.txt","a+"))==NULL)
    return;
    fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SystemFrec,\
           &SystemVolt,&LogicResolution,&prefaulttime,&Postfaulttime,&Preparetime,&current_language);
    exit_par=0;
    fclose(fp);
    FixCurrent=5;
    FixVoltage=40;
    MSAngle=30;
    StartPhase=30;
    //EndPhase=180;
    StepPhase=5;
    StepTime=2;

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

    float MaxFactor=1;
    if(faulttype==4)
       MaxFactor=3;
    else
       MaxFactor=1;




    switch(position)
    {
    case 1:
                           if(FixCurrent>MAX_I_VALUEDATA_AC*MaxFactor)
                               FixCurrent=MAX_I_VALUEDATA_AC*MaxFactor;
                             else if(FixCurrent<0)
                              FixCurrent=0;//閸欏倹鏆熼梽鎰煑
                             break;

   case 2:
                          if(FixVoltage>MAX_V_VALUEDATA_AC)
                                FixVoltage=MAX_V_VALUEDATA_AC;
                          else if(FixVoltage<0)
                                FixVoltage=0;//閸欏倹鏆熼梽鎰煑
                          break;
  case 3:
        if(StartPhase<-360)
            StartPhase+=360;
        if(StartPhase>360)
            StartPhase=0;
        break;
        /*
  case 4:
        if(EndPhase>360)
        EndPhase=360;
        if(EndPhase<=StartPhase)
        EndPhase=StartPhase+StepPhase;
        break;
        */
   case 5:
        if(StepPhase>360)
            StepPhase=360;
    /*    if(StepPhase>(EndPhase-StartPhase))
        {
          StepPhase=1;
          QMessageBox::warning(this,tr("警告"),tr("相位步长过大!"),QMessageBox::Ok);
          return;
        }*/
    case 6:
        if(StepTime>999.9)
            StepTime=999;
        else if(StepTime<0)
            StepTime=0;
        break;   
    case 7:
         if(prefaulttime>999.9)
             prefaulttime=999;
         else if(prefaulttime<0)
             prefaulttime=0;
         break;
   }
}


Widget::~Widget()
{
    delete ui;
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



                strcpy(strbuf,"功率方向");
                fprintf(fp,"%s\r\n",strbuf);

                if(faulttype==1)
                     strcpy(strbuf,"故障类型:A-E");
                 else if(faulttype==2)
                         strcpy(strbuf,"故障类型:B-E");
                 else if(faulttype==3)
                        strcpy(strbuf,"故障类型:C-E");
                 else if(faulttype==4)
                          strcpy(strbuf,"故障类型:A-B-C");
                     fprintf(fp,"%s\r\n",strbuf);

                 strcpy(strbuf,"边界角1(Deg):");
                 fprintf(fp,"%s",strbuf);
                gcvt(Angle1,7,strbuf);
                fprintf(fp," %s\r\n",strbuf);

                strcpy(strbuf,"边界角2(Deg):");
                fprintf(fp,"%s",strbuf);
                gcvt(Angle2 ,7,strbuf);
                fprintf(fp," %s\r\n",strbuf);

                strcpy(strbuf,"灵敏角(Deg):");
                fprintf(fp,"%s",strbuf);
                gcvt(MSAngle ,7,strbuf);
                fprintf(fp,"%s\r\n",strbuf);
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
            fprintf(fp,"Time:%d.%d.%d\n",hour,minute,second);



            strcpy(strbuf,"Power directional test result");
            fprintf(fp,"%s\r\n",strbuf);

            if(faulttype==1)
                     strcpy(strbuf,"Fault Type:A-E");
             else if(faulttype==2)
                     strcpy(strbuf,"Fault Type:B-E");
             else if(faulttype==3)
                     strcpy(strbuf,"Fault Type:C-E");
             else if(faulttype==4)
                     strcpy(strbuf,"Fault Type:A-B-C");
                 fprintf(fp,"%s\r\n",strbuf);

            strcpy(strbuf,"Boundary angle 1(Deg):");
            fprintf(fp,"%s",strbuf);
            gcvt(Angle1,7,strbuf);
            fprintf(fp," %s\r\n",strbuf);

            strcpy(strbuf,"Boundary angle 2(Deg):");
            fprintf(fp,"%s",strbuf);
            gcvt(Angle2 ,7,strbuf);
            fprintf(fp," %s\r\n",strbuf);

            strcpy(strbuf,"MTA(Deg):");
            fprintf(fp,"%s",strbuf);
            gcvt(MSAngle ,7,strbuf);
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


void Widget::setlineedittext(QString str)
{
    bool ok;
    float number=str.toFloat(&ok);
    if(ok==false)
        return;
    switch(currenteditpos)
    {
    case 1:
        FixCurrent=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixCurrent,'g',6));
        break;
    case 2:
        FixVoltage=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(FixVoltage,'g',6));
        break;

    case 3:
        StartPhase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StartPhase,'g',6));
        break;
        /*
    case 4:
        EndPhase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(EndPhase,'g',6));
        break;
        */
    case 5:
        StepPhase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StepPhase,'g',6));
        break;
    case 6:
        StepTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(StepTime,'g',6));
        break;
    case 7:
        prefaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(prefaulttime,'g',6));
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
helpdialog->SetHelpfilename("Power.txt");
else if(current_language==1)
    helpdialog->SetHelpfilename("Power1.txt");
//helpdialog->SetHelpfilename("Power.txt");
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

        /*
        else if(obj==ui->lineEdit_4)
         {    temprorarydata->keypadopenflag=true;
              currentedit=ui->lineEdit_4;
              currenteditpos=4;
         }
        */

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
               FixCurrent= text.toFloat();
               currenteditpos=1;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit->setText(QString::number(FixCurrent,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               FixVoltage= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(FixVoltage,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                StartPhase= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(StartPhase,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                StepPhase= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(StepPhase,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                StepTime= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(StepTime,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                prefaulttime= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(prefaulttime,'g',6));
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




void Widget::on_radioButton_clicked()
{
    faulttype=1;
}

void Widget::on_radioButton_2_clicked()
{
   faulttype=2;
}

void Widget::on_radioButton_3_clicked()
{
   faulttype=3;
}

void Widget::on_radioButton_4_clicked()
{
   faulttype=4;
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
