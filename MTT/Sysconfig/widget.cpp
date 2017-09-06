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
#include <QMouseEvent>
#include <math.h>
#include "qtranslator.h"
//ME3000_MPF
QTranslator *tor;
int current_language=1;  //1中文  2英文
int   WarningInfo1,WarningInfo2,WarningInfo3;
float SysFrec,LogicResolution;
float SysVol;
int year,month,date,hour,minute,second;
bool keypadopenflag=false;
float prefaulttime=3;
float Postfaulttime=3;
float Preparetime=3;
float password=0;
QString softwarenumber;
int softnumber;
float softenter;
int dcouttype=1; //
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    init_par();
    /* 加载语言文件 默认中文 2016-9-7*/
    tor = new QTranslator();
    if (current_language == 1) {
        tor->load(":/trans/zh.qm");
        ui->radioButton_2->setChecked(true);
        ui->radioButton->setChecked(false);
    }
    else if (current_language == 2) {
        tor->load(":/trans/en.qm");
        ui->radioButton_2->setChecked(false);
        ui->radioButton->setChecked(true);
    }
    qApp->installTranslator(tor);
     updateUI();
     QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
     QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
     QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFixedSize(800,480);
    setFont(font);
    //init_par();
    initalldata();
    ui->checkBox->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_2->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
    ui->checkBox_3->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/dagou.png);}");
   ui->label_7->hide();
   ui->lineEdit_6->hide();
   showFullScreen();
    //ui->label_5->setText(("<font color=blue>"+QString("System Configure")+"</font>"));

   switch(dcouttype)
   {
   case 1:
       ui->radioButton_3->setChecked(true);

       break;
   case 2:

       ui->radioButton_4->setChecked(true);
       break;
   case 3:
        ui->radioButton_5->setChecked(true);
       break;
   }

   ui->pushButton_6->hide();
   ui->pushButton_7->hide();
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
    ui->lineEdit_5->installEventFilter(this);
    //ui->lineEdit_6->installEventFilter(this);

    ui->lineEdit_7->installEventFilter(this);
    ui->lineEdit_8->installEventFilter(this);
    ui->lineEdit_9->installEventFilter(this);
    ui->lineEdit_10->installEventFilter(this);
    ui->lineEdit_11->installEventFilter(this);
    ui->lineEdit_12->installEventFilter(this);

    ui->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_5->setContextMenuPolicy(Qt::NoContextMenu);
    //ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);

    ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_8->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_9->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_10->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_11->setContextMenuPolicy(Qt::NoContextMenu);
    ui->lineEdit_12->setContextMenuPolicy(Qt::NoContextMenu);

    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
    ui->groupBox->hide();
    //ui->groupBox_2->hide();
    ui->groupBox_4->hide();

}
void Widget::updateUI()
{
    ui->label_5->setText("<font color=blue>"+QString(tr("系统设置"))+"</font>");
    //this->setWindowTitle(tr("主框架"));
        ui->pushButton->setText(tr("保存"));
        ui->pushButton_3->setText(tr("退出"));

        ui->label->setText(tr("系统频率(Hz)："));
        ui->label_13->setText(tr("系统电压(V)："));
        ui->label_2->setText(tr("开关量分辨率(ms)："));
        ui->label_7->setText(tr("准备时间(s)："));
        ui->label_3->setText(tr("故障前时间(s)："));
        ui->label_6->setText(tr("故障后时间(s)："));
        ui->label_8->setText(tr("年"));
        ui->label_9->setText(tr("月"));
        ui->label_10->setText(tr("日"));
        ui->label_11->setText(tr("时"));
        ui->label_12->setText(tr("分"));
        ui->label_14->setText(tr("秒"));

        ui->groupBox->setTitle(tr("语言设置"));
        ui->groupBox_2->setTitle(tr("系统时间"));
        ui->groupBox_3->setTitle(tr("报警设置"));

        ui->radioButton->setText(tr("英文"));
        ui->radioButton_2->setText(tr("中文"));

        ui->radioButton_3->setText(tr("No "));
        ui->radioButton_4->setText(tr("110 V"));
        ui->radioButton_5->setText(tr("220 V"));

        ui->checkBox->setText(tr("过载"));
        ui->checkBox_2->setText(tr("间隙"));
        ui->checkBox_3->setText(tr("闭锁"));
}
void Widget::moveEvent(QMoveEvent *event)
{
 QPoint oldpos=event->oldPos();
 this->move(oldpos);
}



bool Widget::eventFilter(QObject *obj, QEvent *event)
{



    if(event->type()==QEvent::MouseButtonPress)
    {

        QMouseEvent *mouseevent=static_cast<QMouseEvent *>(event);
        if(mouseevent->button()==Qt::RightButton)
        {

            return true;
        }
        else
        return false;

    }
    else if(event->type()==QEvent::MouseButtonDblClick)
   {


        QMouseEvent *mouseevent=static_cast<QMouseEvent *>(event);
        if(mouseevent->button()==Qt::RightButton)
        {
            return true;
        }
        if(obj==ui->lineEdit)
        {    keypadopenflag=true;
              currentedit=ui->lineEdit;
             currenteditpos=1;
        }
       else if(obj==ui->lineEdit_2)
        {    keypadopenflag=true;
              currentedit=ui->lineEdit_2;
             currenteditpos=2;
        }
        else if(obj==ui->lineEdit_3)
         {    keypadopenflag=true;
               currentedit=ui->lineEdit_3;
              currenteditpos=3;
         }
        else if(obj==ui->lineEdit_4)
         {    keypadopenflag=true;
              currentedit=ui->lineEdit_4;
              currenteditpos=4;
         }
        else if(obj==ui->lineEdit_5)
         {    keypadopenflag=true;
              currentedit=ui->lineEdit_5;
              currenteditpos=5;
         }
//        else if(obj==ui->lineEdit_6)
//         {    keypadopenflag=true;
//              currentedit=ui->lineEdit_6;
//              currenteditpos=6;
//         }

        else if(obj==ui->lineEdit_7)
         {    keypadopenflag=true;
              currentedit=ui->lineEdit_7;
              currenteditpos=7;
         }
        else if(obj==ui->lineEdit_8)
         {    keypadopenflag=true;
              currentedit=ui->lineEdit_8;
              currenteditpos=8;
         }
        else if(obj==ui->lineEdit_9)
         {    keypadopenflag=true;
              currentedit=ui->lineEdit_9;
              currenteditpos=9;
         }
        else if(obj==ui->lineEdit_10)
         {    keypadopenflag=true;
              currentedit=ui->lineEdit_10;
              currenteditpos=10;
         }
        else if(obj==ui->lineEdit_11)
         {    keypadopenflag=true;
              currentedit=ui->lineEdit_11;
              currenteditpos=11;
         }
        else if(obj==ui->lineEdit_12)
         {    keypadopenflag=true;
              currentedit=ui->lineEdit_12;
              currenteditpos=12;
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
              SysFrec= text.toFloat();
              currenteditpos=1;
              vi_parameter_enter(currenteditpos);
              ui->lineEdit->setText(QString::number(SysFrec,'g',6));
        }
        else if(obj==ui->lineEdit_2)
        {
               QString text=ui->lineEdit_2->text();
               SysVol= text.toFloat();
               currenteditpos=2;
               vi_parameter_enter(currenteditpos);
               ui->lineEdit_2->setText(QString::number(SysVol,'g',6));
        }
        else if(obj==ui->lineEdit_3)
         {
                QString text=ui->lineEdit_3->text();
                LogicResolution= text.toFloat();
                currenteditpos=3;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_3->setText(QString::number(LogicResolution,'g',6));
         }
        else if(obj==ui->lineEdit_4)
         {
                QString text=ui->lineEdit_4->text();
                prefaulttime= text.toFloat();
                currenteditpos=4;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_4->setText(QString::number(prefaulttime,'g',6));
         }
        else if(obj==ui->lineEdit_5)
         {
                QString text=ui->lineEdit_5->text();
                Postfaulttime= text.toFloat();
                currenteditpos=5;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_5->setText(QString::number(Postfaulttime,'g',6));
         }
        else if(obj==ui->lineEdit_7)
         {
                QString text=ui->lineEdit_7->text();
                year= text.toFloat();
                currenteditpos=7;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_7->setText(QString::number(year,'g',6));
         }
        else if(obj==ui->lineEdit_8)
         {
                QString text=ui->lineEdit_8->text();
                month= text.toFloat();
                currenteditpos=8;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_8->setText(QString::number(month,'g',6));
         }
        else if(obj==ui->lineEdit_9)
         {
                QString text=ui->lineEdit_9->text();
                date= text.toFloat();
                currenteditpos=9;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_9->setText(QString::number(date,'g',6));
         }
        else if(obj==ui->lineEdit_10)
         {
                QString text=ui->lineEdit_10->text();
                hour= text.toFloat();
                currenteditpos=10;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_10->setText(QString::number(hour,'g',6));
         }
        else if(obj==ui->lineEdit_11)
         {
                QString text=ui->lineEdit_11->text();
                minute= text.toFloat();
                currenteditpos=11;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_11->setText(QString::number(minute,'g',6));
         }
        else if(obj==ui->lineEdit_12)
         {
                QString text=ui->lineEdit_12->text();
                second= text.toFloat();
                currenteditpos=12;
                vi_parameter_enter(currenteditpos);
                ui->lineEdit_12->setText(QString::number(second,'g',6));
         }
        return QWidget::eventFilter(obj,event);
    }

    else
    return QWidget::eventFilter(obj,event);

}




void Widget::init_par()
{
        FILE *fp;
        if((fp=fopen("sysset.txt","a+"))==NULL)
        return;
        SysVol=57.735;
        LogicResolution=3;
        SysFrec=25;
        prefaulttime=15;
        Postfaulttime=1;
        WarningInfo1=1;
        WarningInfo2=1;
        WarningInfo3=1;
        current_language=1;
        fscanf(fp,"%d,%d,%d,%f,%f,%f,%f,%f,%f,%d,%d",&WarningInfo1,&WarningInfo2,&WarningInfo3,&SysFrec,&SysVol,&LogicResolution,&prefaulttime,&Postfaulttime,&dcouttype,&current_language,&softnumber);
        dcouttype=1;//2016-8-9直流选择有输出，重启机器，下次要恢复为0
        SysFrec=SysFrec*2;
        current_language=1;//2017-6-28  强制中文版
        fclose(fp);
}



void Widget::initalldata()
{

     ui->lineEdit->setText(QString::number(SysFrec));
     ui->lineEdit_2->setText(QString::number(SysVol));
     ui->lineEdit_3->setText(QString::number(LogicResolution));
     ui->lineEdit_4->setText(QString::number(prefaulttime));
     ui->lineEdit_5->setText(QString::number(Postfaulttime));
     //ui->lineEdit_6->setText(QString::number(Preparetime));


     ui->checkBox->setChecked((bool)WarningInfo1);
     ui->checkBox_2->setChecked((bool)WarningInfo2);
     ui->checkBox_3->setChecked((bool)WarningInfo3);





     year=QDate::currentDate().year();
     month=QDate::currentDate().month();
     date=QDate::currentDate().day();
     hour=QTime::currentTime().hour();
     minute=QTime::currentTime().minute();
     second=QTime::currentTime().second();

     ui->lineEdit_7->setText(QString::number(year));
     ui->lineEdit_8->setText(QString::number(month));
     ui->lineEdit_9->setText(QString::number(date));
     ui->lineEdit_10->setText(QString::number(hour));
     ui->lineEdit_11->setText(QString::number(minute));
     ui->lineEdit_12->setText(QString::number(second));


}



void Widget::vi_parameter_enter(int position)
{

    switch(position)
    {
    case 1:
                            if(SysFrec>999.99)
                              SysFrec=999.99;
                              else if(SysFrec<0)
                              SysFrec=0;
                             break;
    case 2:
                             if(SysVol>999.99)
                               SysVol=999.99;
                             else if(SysVol<0)
                               SysVol=0;
                              break;
    case 3:
                             if(LogicResolution>999.99)
                               LogicResolution=999.99;
                             else if(LogicResolution<0)
                               LogicResolution=0;
                              break;
    case 4:
        if(prefaulttime>999.99)
          prefaulttime=999.99;
        else if(prefaulttime<0)
          prefaulttime=0;
         break;

    case 5:
        if(Postfaulttime>999.99)
          Postfaulttime=999.99;
        else if(Postfaulttime<0)
          Postfaulttime=0;
         break;
    case 6:
        if(Preparetime>999.99)
          Preparetime=999.99;
        else if(Preparetime<0)
          Preparetime=0;
         break;
    case 7:
            if(year>2100)
                year=2100;
            else if(year<2014)
                year=2014;
            break;
        case 8:
            if(month>12)
                month=12;
            else if(month<0)
                month=1;
            break;
        case 9:
            if(date>31)
             date=31;
            else if(date<0)
             date=0;
            break;
        case 10:
            if(hour>24)
                hour=24;
            else if(hour<0)
                hour=0;
            break;
        case 11:
            if(minute>59)
                minute=59;
            else if(minute<0)
                minute=0;
            break;
        case 12:
        if(second>59)
            second=59;
        else if(second<0)
            second=0;
        break;

}
}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    FILE *fp;
    if((fp=fopen("sysset.txt","w+"))==NULL)
      return;
    fprintf(fp,"%d,",WarningInfo1);
    fprintf(fp,"%d,",WarningInfo2);
    fprintf(fp,"%d,",WarningInfo3);
    fprintf(fp,"%.2f,",SysFrec/2);
    fprintf(fp,"%.2f,",SysVol);
    fprintf(fp,"%.2f,",LogicResolution);
    fprintf(fp,"%.3f,",prefaulttime);
    fprintf(fp,"%.3f,",Postfaulttime);
    fprintf(fp,"%d,",dcouttype);
    fprintf(fp,"%d,",current_language);
    fprintf(fp,"%d,",softnumber);
    fclose(fp);

    time_t  t;
    struct  tm nowtime;
    nowtime.tm_sec=second;
    nowtime.tm_min=minute;
    nowtime.tm_hour=hour;
    nowtime.tm_mday=date;
    nowtime.tm_mon=month-1;
    nowtime.tm_year=year-1900;

    nowtime.tm_isdst=-1;
    t=mktime(&nowtime);
    stime(&t);
    system("hwclock -w");

    close();

}


void Widget::on_pushButton_3_clicked()
{
    close();
}

void Widget::on_checkBox_clicked()
{
    if(WarningInfo1==1)
        WarningInfo1=0;
    else WarningInfo1=1;

}

void Widget::on_checkBox_2_clicked()
{
    if(WarningInfo2==1)
        WarningInfo2=0;
    else WarningInfo2=1;
}

void Widget::on_checkBox_3_clicked()
{
    if(WarningInfo3==1)
        WarningInfo3=0;
    else WarningInfo3=1;
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
        SysFrec=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(SysFrec,'g',6));
        break;
    case 2:
        SysVol=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(SysVol,'g',6));
        break;
    case 3:
        LogicResolution=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(LogicResolution,'g',6));
        break;
    case 4:
        prefaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(prefaulttime,'g',6));
        break;
    case 5:
        Postfaulttime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Postfaulttime,'g',6));
        break;
    case 6:
        Preparetime=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(Preparetime,'g',6));
        break;
    case 7:
        year=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(year,'g',6));
        break;
    case 8:
        month=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(month,'g',6));
        break;
    case 9:
        date=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(date,'g',6));
        break;
    case 10:
        hour=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(hour,'g',6));
        break;
    case 11:
        minute=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(minute,'g',6));
        break;
    case 12:
        second=number;
        vi_parameter_enter(currenteditpos);
        currentedit->setText(QString::number(second,'g',6));
        break;

    }

}

void Widget::clearkeypadflag()
{
    keypadopenflag=false;
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

void Widget::on_radioButton_clicked()//英
{
    ui->radioButton->setChecked(true);
    ui->radioButton_2->setChecked(false);
    on_pushButton_7_clicked();
}

void Widget::on_radioButton_2_clicked()//中
{
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(true);
    on_pushButton_6_clicked();
}




void Widget::on_radioButton_3_clicked()
{
    dcouttype=1;
}

void Widget::on_radioButton_4_clicked()
{
    dcouttype=2;
}

void Widget::on_radioButton_5_clicked()
{
    dcouttype=3;
}
