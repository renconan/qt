#include "harmonic.h"
#include "ui_harmonic.h"
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
//QTranslator *tor;
int current_language=1;  //1中文  2英文


extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;

extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision;


extern float LogicResolution;
extern int   save_f;
extern int sys[4];

extern float  TripTime,TripValue;  //谐波电流初值





extern float faultduration;

float H_Preparetime;


extern TEMPRORARYDATA *temprorarydata;

extern bool SoftStartEndflag;



Harmonic::Harmonic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Harmonic)
{
    ui->setupUi(this);
    printf("00");
    init_par();
    perror("1");
    initDesc();
    perror("2");
    /* 加载语言文件 默认中文 2016-9-9*/
//        tor = new QTranslator();
//        if (current_language == 1) {
//            tor->load(":/trans/zh.qm");
//        }
//        else if (current_language == 2) {
//            tor->load(":/trans/en.qm");
//        }
//        qApp->installTranslator(tor);
        //updateUI();
perror("3");
    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
       QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    //init_par();
    setFixedSize(640,480);
    sysinit();
    showFullScreen();
//    ui->label_13->hide();
//    ui->label_14->hide();
//    ui->label_15->hide();
//    ui->label_7->hide();
//    ui->label_16->hide();
//    ui->label_17->hide();
//    ui->label_18->hide();
//    ui->label_19->hide();

//    QPixmap img(":/picture/hei.png");
//    ui->label_13->setPixmap(img);
//    ui->label_14->setPixmap(img);
//    ui->label_15->setPixmap(img);
//    ui->label_7->setPixmap(img);

       //ui->label_11->setText("<font color=blue>"+QString("差动谐波制动(2.2.1)")+"</font>");
       ui->lineEdit->installEventFilter(this);
       ui->lineEdit_2->installEventFilter(this);
       ui->lineEdit_3->installEventFilter(this);
       ui->lineEdit_4->installEventFilter(this);
       ui->lineEdit_5->installEventFilter(this);
       ui->lineEdit_6->installEventFilter(this);
       ui->lineEdit_7->installEventFilter(this);
       ui->lineEdit_8->installEventFilter(this);
       ui->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_5->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);
       ui->lineEdit_8->setContextMenuPolicy(Qt::NoContextMenu);

       switch(H_CurrentOutputSel)
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
    perror("4");
    uartthread =new thread1;
    perror("5");
    connect(uartthread,SIGNAL(SIG_binarystate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_testresult(float)),SLOT(showtestresult(float)));
    connect(uartthread,SIGNAL(SIG_dspstate(int)),SLOT(showdspstate(int)));
    connect(uartthread,SIGNAL(SIG_DisplayChangeValue(int,float)),SLOT(displaychange(int,float)));

   connect(uartthread,SIGNAL(SIG_SendClose()),SLOT(CloseWidget()));
   connect(uartthread,SIGNAL(SIG_TestState()),SLOT(GetTestState()));
   connect(uartthread,SIGNAL(SIG_Teststate(int)),SLOT(GetTeststate(int)));

    uartthread->start();
}

void Harmonic::updateUI()
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

void Harmonic::init_par()
{
        HarmonicCount=2;
        FixHarmonic=15;
        H_Current=5;
        H_Phase=0;
        m_Ch[0].DC=0;
        m_Ch[1].DC=0;
        m_Ch[2].DC=0;
        m_Ch[3].DC=0;
        m_Ch[4].DC=0;
        m_Ch[5].DC=0;
        m_Ch[6].DC=0;
        faultduration=10;
        H_CurrentOutputSel=1;
        HarmonicStart=15;
        HarmonicEnd=5;
        HarmonicStep=0.5;
        A_StepTime=0.1;


        float SysFrec2=25;
        float mSysVolt =0;
        float mLogicResolution=0;
        float mPreFaultTime=0;
        float mPostFaultTime=0;
        float mPreparetime=0;
        FILE *fp;
        if((fp=fopen("sysset.txt","a+"))==NULL)
        return;
        //fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SysFrec,&SysVolt,&LogicResolution,&A_PreFaultTime,&A_PostFaultTime,&dcouttype);
//        fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SysFrec,\
//               &SysVolt,&LogicResolution,&A_PreFaultTime,&A_PostFaultTime,&H_Preparetime,&current_language);
        fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d",&sys[1],&sys[2],&sys[3],&SysFrec2,\
               &mSysVolt,&mLogicResolution,&mPreFaultTime,&mPostFaultTime,&mPreparetime,&current_language);
        SysFrec=SysFrec2*2;
        SysVolt=mSysVolt;
        LogicResolution=mLogicResolution;
        A_PreFaultTime=mPreFaultTime;
        A_PostFaultTime=mPostFaultTime;
        H_Preparetime=mPreparetime;
        fclose(fp);

}

void Harmonic::sysinit()
{
    ui->lineEdit->setText(QString::number(HarmonicCount,'g',6));
    ui->lineEdit_2->setText(QString::number(HarmonicStart,'g',6));
    ui->lineEdit_3->setText(QString::number(HarmonicEnd,'g',6));
    ui->lineEdit_4->setText(QString::number(HarmonicStep,'g',6));
    ui->lineEdit_5->setText(QString::number(A_StepTime,'g',6));
    ui->lineEdit_6->setText(QString::number(H_Current,'g',6));
    ui->lineEdit_7->setText(QString::number(H_Phase,'g',6));
    ui->lineEdit_8->setText(QString::number(faultduration,'g',6));

}


void Harmonic::showdspstate(int state)
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


void Harmonic::showtestresult(float num)
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



void Harmonic::displaychange(int num,float value)
{
 if(num==0)
 ui->label_12->clear();
 else
 ui->label_12->setText(tr("谐波含量:")+QString::number(value,'g',4)+tr("%"));




}

void Harmonic::vi_parameter_enter(int position)
{
    switch(position)
    {
    case 1:
        if(HarmonicCount>999.99)
            HarmonicCount=999.99;
        else if(HarmonicCount<0)
            HarmonicCount=0;//参数限制
        break;
   case 2:
        if(HarmonicStart>100)
            HarmonicStart=100;
        else if(HarmonicStart<0)
            HarmonicStart=0;//参数限制
        if(HarmonicStart<=HarmonicEnd)
            HarmonicStart=HarmonicEnd+HarmonicStep;
//        if(HarmonicStart>HarmonicEnd+20)
//            HarmonicEnd=HarmonicStart-20;
        break;
   case 3:
        if(HarmonicEnd>100)
            HarmonicEnd=100;
        else if(HarmonicEnd<0)
            HarmonicEnd=0;//参数限制
        if(HarmonicEnd>=HarmonicStart)
            HarmonicEnd=HarmonicStart-HarmonicStep;
//        if(HarmonicEnd<HarmonicStart-20)
//            HarmonicStart=HarmonicEnd+20;
        break;
   case 4:
         if(HarmonicStep>100)
             HarmonicStep=100;
         else if(HarmonicStep<0)
             HarmonicStep=0;//参数限制
         if(HarmonicStep>fabs(HarmonicStart-HarmonicEnd))
             HarmonicStep=fabs(HarmonicStart-HarmonicEnd);
         break;
   case 5:
        if(A_StepTime>999.99)
            A_StepTime=999.99;
        else if(A_StepTime<0)
            A_StepTime=0;//参数限制
        break;

   case 6:
        if(H_Current>MAX_I_VALUEDATA_AC)
          H_Current=MAX_I_VALUEDATA_AC;
        else if(H_Current<0)
          H_Current=0;//参数限制
        break;
   case 7:
        if(H_Phase>180)
          H_Phase=180;
        else if(H_Phase<-180)
          H_Phase=-180;//参数限制
            break;
   case 8:
        if(faultduration>999.99)
            faultduration=999.99;
        else if(faultduration<0)
            faultduration=0;//参数限制
}
}






void Harmonic::setlineedittext(QString str)
{
    bool ok;
    float number=str.toFloat(&ok);
    if(ok==false)
        return;
    switch(currenteditpos)
    {
    case 1:
        HarmonicCount=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicCount,'g',6));
        break;

    case 2:
        HarmonicStart=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicStart,'g',6));
        ui->lineEdit_3->setText(QString::number(HarmonicEnd,'g',6));
        break;
    case 3:
        HarmonicEnd=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicEnd,'g',6));
        ui->lineEdit_2->setText(QString::number(HarmonicStart,'g',6));
        break;
    case 4:
        HarmonicStep=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(HarmonicStep,'g',6));
        break;
    case 5:
        A_StepTime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(A_StepTime,'g',6));
        break;

    case 6:
        H_Current=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(H_Current,'g',6));
        break;
    case 7:
        H_Phase=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(H_Phase,'g',6));
        break;
    case 8:
        faultduration=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(faultduration,'g',6));
        break;
    }

}

void Harmonic::clearkeypadflag()
{
    temprorarydata->keypadopenflag=false;
}

Harmonic::~Harmonic()
{
    delete ui;
}


void Harmonic::on_pushButton_clicked()
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

void Harmonic::on_pushButton_2_clicked()
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
                gcvt(H_Current,7,strbuf);
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

            strcpy(strbuf,"Baseband H_Current(A):");
            fprintf(fp,"%s",strbuf);
            gcvt(H_Current,7,strbuf);
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


void Harmonic::on_pushButton_3_clicked()
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

void Harmonic::CloseWidget()
{

}

void Harmonic::GetTestState(){
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



void Harmonic::HelpDialogClose()
{
temprorarydata->helpopenflag=false;

}


void Harmonic::moveEvent(QMoveEvent *event)
{
 QPoint oldpos=event->oldPos();
 this->move(oldpos);
}



bool Harmonic::eventFilter(QObject *obj, QEvent *event)
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
               HarmonicCount= text.toFloat();
               currenteditpos=1;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit->setText(QString::number(HarmonicCount,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               HarmonicStart= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(HarmonicStart,'g',6));
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
                A_StepTime= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(A_StepTime,'g',6));
         }
        else if(obj==ui->lineEdit_6)
         {
                QString text=ui->lineEdit_6->text();
                H_Current= text.toFloat();
                currenteditpos=6;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_6->setText(QString::number(H_Current,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                H_Phase= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(H_Phase,'g',6));
         }
        else if(obj==ui->lineEdit_8)
        {
               QString text=ui->lineEdit_8->text();
               faultduration= text.toFloat();
               currenteditpos=8;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_8->setText(QString::number(faultduration,'g',6));
        }
        return QWidget::eventFilter(obj,event);
    }
    else
    return QWidget::eventFilter(obj,event);

}

void Harmonic::GetTeststate(int state)
{
    //2016-9-13  键盘输入，界面测试按钮，
    //设置软件“测试”按钮获取焦点，激活失去焦点时间，给最后的输入幅值
    if(state==1)
    ui->pushButton_4->setFocus();
}

void Harmonic::on_radioButton_clicked()
{
H_CurrentOutputSel=1;
}

void Harmonic::on_radioButton_2_clicked()
{
H_CurrentOutputSel=2;
}

void Harmonic::on_radioButton_3_clicked()
{
H_CurrentOutputSel=3;
}

void Harmonic::on_pushButton_4_clicked()
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
