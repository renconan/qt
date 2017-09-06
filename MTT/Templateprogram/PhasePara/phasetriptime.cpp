#include "phasetriptime.h"
#include "ui_phasetriptime.h"
#include <QTextCodec>
#include <QMouseEvent>
#include <QMessageBox>

PhaseTriptime::PhaseTriptime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhaseTriptime)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    setFixedSize(800,480);
        showFullScreen();
    ui->checkBox->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/Check.png);}");
    ui->checkBox_2->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/Check.png);}");
    ui->checkBox_3->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/Check.png);}");
    ui->checkBox_4->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/Check.png);}");
    ui->checkBox_5->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/Check.png);}");
    ui->checkBox_6->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/Check.png);}");
    ui->checkBox_7->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/Check.png);}");
    ui->checkBox_8->setStyleSheet("QCheckBox{font:16px;}QCheckBox::indicator {width: 13px; height: 13px;}QCheckBox::indicator:checked {image: url(:/Check.png);}");


        ui->lineEdit_2->installEventFilter(this);
        ui->lineEdit_3->installEventFilter(this);
        ui->lineEdit_4->installEventFilter(this);
        ui->lineEdit_5->installEventFilter(this);
        ui->lineEdit_6->installEventFilter(this);
        ui->lineEdit_7->installEventFilter(this);
        ui->lineEdit_8->installEventFilter(this);
        ui->lineEdit_9->installEventFilter(this);


        ui->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_5->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_8->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_9->setContextMenuPolicy(Qt::NoContextMenu);

}

PhaseTriptime::~PhaseTriptime()
{
    delete ui;
}


bool PhaseTriptime::eventFilter(QObject *obj, QEvent *event)
{


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

        if(obj==ui->lineEdit_2)
        {
             currentedit=ui->lineEdit_2;

        }
        else if(obj==ui->lineEdit_3)
         {
              currentedit=ui->lineEdit_3;

         }
        else if(obj==ui->lineEdit_4)
         {
              currentedit=ui->lineEdit_4;

         }
        else if(obj==ui->lineEdit_5)
         {
              currentedit=ui->lineEdit_5;

         }
        else if(obj==ui->lineEdit_6)
         {
              currentedit=ui->lineEdit_6;

         }
        else if(obj==ui->lineEdit_7)
         {
              currentedit=ui->lineEdit_7;

         }
        else if(obj==ui->lineEdit_8)
         {
              currentedit=ui->lineEdit_8;

         }
        else if(obj==ui->lineEdit_9)
         {
              currentedit=ui->lineEdit_9;

         }




        KeyPadDialog *pad=new KeyPadDialog;
        pad->SetText(currentedit->text());
        connect(pad,SIGNAL(SIG_sendtext(char*)),this,SLOT(setlineedittext(char *)));
        connect(pad,SIGNAL(SIG_closeflag()),this,SLOT(clearkeypadflag()));

        pad->setModal(true);
        pad->activateWindow();
        pad->raise();
        pad->show();


      return true;
   }
    else
    return QWidget::eventFilter(obj,event);

}


void PhaseTriptime::setlineedittext(char *str)
{

    currentedit->setText(str);

}

void PhaseTriptime::clearkeypadflag()
{

}



void  PhaseTriptime::Setshowpara(struct SECTMENTSET value)
{

memcpy(&Sectvalue,&value,sizeof(struct SECTMENTSET));
if(Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
   Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1)
ui->lineEdit->setText(tr("Stability"));
else
ui->lineEdit->setText(tr("Trip time"));
ui->lineEdit->setEnabled(false);

ui->lineEdit_2->setText(Sectvalue.phraseset.FixVoltage.text);
ui->lineEdit_3->setText(Sectvalue.phraseset.FixCurrent.text);

ui->checkBox->setChecked(Sectvalue.phraseset.VAch);
ui->checkBox_2->setChecked(Sectvalue.phraseset.VBch);
ui->checkBox_3->setChecked(Sectvalue.phraseset.VCch);
ui->checkBox_4->setChecked(Sectvalue.phraseset.IAch);
ui->checkBox_5->setChecked(Sectvalue.phraseset.IBch);
ui->checkBox_6->setChecked(Sectvalue.phraseset.ICch);








ui->lineEdit_4->setText(Sectvalue.phraseset.TripTime.EndVaule.text);
ui->lineEdit_5->setText(Sectvalue.phraseset.TripTime.HoldTime.text);
ui->lineEdit_6->setText(Sectvalue.phraseset.TripTime.assesstriptime.CompareID);


if(Sectvalue.phraseset.TripTime.assesstriptime.relerror.valid)
{
 ui->checkBox_7->setChecked(true);
 ui->lineEdit_7->setText(QString::number(Sectvalue.phraseset.TripTime.assesstriptime.relerror.errorvalue));

}
if(Sectvalue.phraseset.TripTime.assesstriptime.abserror.valid)
{
 ui->lineEdit_8->setText(QString::number(Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue,'g',5));
 ui->lineEdit_9->setText(QString::number(Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive,'g',5));
 ui->checkBox_8->setChecked(Sectvalue.phraseset.TripTime.assesstriptime.abserror.valid);
 }

ui->comboBox->setCurrentIndex(Sectvalue.phraseset.TripTime.assesstriptime.assessandor-1);

}

void  PhaseTriptime::SetSettingLink(struct SETTINGNODELIST *link)
{
settinglink=link;
}

void  PhaseTriptime::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}



void  PhaseTriptime::on_pushButton_clicked()
{


   if(ui->lineEdit_2->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input fix voltage!"),QMessageBox::Ok);
    return;
   }

   if(ui->lineEdit_3->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input fix current!"),QMessageBox::Ok);
    return;
   }

   if(ui->lineEdit_4->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input End phase!"),QMessageBox::Ok);
    return;
   }

   if(ui->lineEdit_5->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input hold time!"),QMessageBox::Ok);
    return;
   }


   if(ui->lineEdit_6->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input comparative id!"),QMessageBox::Ok);
    return;
   }



   if(CheckTextValid(settinglink,ui->lineEdit_2->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of fix voltage is invalid!"),QMessageBox::Ok);
       return;
   }
   if(CheckTextValid(settinglink,ui->lineEdit_3->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of fix current is invalid!"),QMessageBox::Ok);
       return;
   }

   if(CheckTextValid(settinglink,ui->lineEdit_4->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of end phase is invalid!"),QMessageBox::Ok);
       return;
   }
   if(CheckTextValid(settinglink,ui->lineEdit_5->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of hold time is invalid!"),QMessageBox::Ok);
       return;
   }

   if(CheckTextValid(settinglink,ui->lineEdit_6->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of comparative id is invalid!"),QMessageBox::Ok);
       return;
   }



   if(ui->checkBox_7->isChecked())
   {
       if(ui->lineEdit_7->text().isEmpty())
       {
        QMessageBox::warning(this,tr("Warning"),tr("Please input relative error!"),QMessageBox::Ok);
        return;
       }

       if(CheckTextValid(settinglink,ui->lineEdit_7->text())!=0)
       {
           QMessageBox::warning(this,tr("Warning"),tr("Text of relative error is invalid!"),QMessageBox::Ok);
           return;
       }

   }

   if(ui->checkBox_8->isChecked())
   {
       if(ui->lineEdit_8->text().isEmpty())
       {
        QMessageBox::warning(this,tr("Warning"),tr("Please input positive absolute error!"),QMessageBox::Ok);
        return;
       }

       if(CheckTextValid(settinglink,ui->lineEdit_8->text())!=0)
       {
           QMessageBox::warning(this,tr("Warning"),tr("Text of positive absolute error is invalid!"),QMessageBox::Ok);
           return;
       }

       if(ui->lineEdit_9->text().isEmpty())
       {
        QMessageBox::warning(this,tr("Warning"),tr("Please input negative absolute error!"),QMessageBox::Ok);
        return;
       }

       if(CheckTextValid(settinglink,ui->lineEdit_9->text())!=0)
       {
           QMessageBox::warning(this,tr("Warning"),tr("Text of negative absolute error is invalid!"),QMessageBox::Ok);
           return;
       }

   }



   copytoshuzu(Sectvalue.phraseset.FixVoltage.text,\
               ui->lineEdit_2->text(),\
               ui->lineEdit_2->text().size());
   copytoshuzu(Sectvalue.phraseset.FixCurrent.text,\
               ui->lineEdit_3->text(),\
               ui->lineEdit_3->text().size());

   if(ui->checkBox->isChecked())
     Sectvalue.phraseset.VAch=1;
   else
     Sectvalue.phraseset.VAch=0;

   if(ui->checkBox_2->isChecked())
     Sectvalue.phraseset.VBch=1;
   else
     Sectvalue.phraseset.VBch=0;

   if(ui->checkBox_3->isChecked())
     Sectvalue.phraseset.VCch=1;
   else
     Sectvalue.phraseset.VCch=0;

   if(ui->checkBox_4->isChecked())
     Sectvalue.phraseset.IAch=1;
   else
     Sectvalue.phraseset.IAch=0;

   if(ui->checkBox_5->isChecked())
     Sectvalue.phraseset.IBch=1;
   else
     Sectvalue.phraseset.IBch=0;

   if(ui->checkBox_6->isChecked())
     Sectvalue.phraseset.ICch=1;
   else
     Sectvalue.phraseset.ICch=0;


   copytoshuzu(Sectvalue.phraseset.TripTime.EndVaule.text,\
               ui->lineEdit_4->text(),\
               ui->lineEdit_4->text().size());
   copytoshuzu(Sectvalue.phraseset.TripTime.HoldTime.text,\
               ui->lineEdit_5->text(),\
               ui->lineEdit_5->text().size());

   copytoshuzu(Sectvalue.phraseset.TripTime.assesstriptime.CompareID,\
               ui->lineEdit_6->text(),\
               ui->lineEdit_6->text().size());

   if(ui->checkBox_7->isChecked())
   {
         QString value=ui->lineEdit_7->text();
         bool ok;
         Sectvalue.phraseset.TripTime.assesstriptime.relerror.errorvalue=value.toFloat(&ok);
         Sectvalue.phraseset.TripTime.assesstriptime.relerror.valid=true;
   }
   else
   Sectvalue.phraseset.TripTime.assesstriptime.relerror.valid=false;

   if(ui->checkBox_8->isChecked())
   {
       bool ok;
       QString value;
       value=ui->lineEdit_8->text();
       Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue=value.toFloat(&ok);
       value.clear();
       value=ui->lineEdit_9->text();
       Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive=value.toFloat(&ok);
       Sectvalue.phraseset.TripTime.assesstriptime.abserror.valid=true;

   }

   else
   Sectvalue.phraseset.TripTime.assesstriptime.abserror.valid=false;

   Sectvalue.phraseset.TripTime.assesstriptime.assessandor=ui->comboBox->currentIndex()+1;


 emit Sendpara((void *)&Sectvalue);
 emit sendflag();
 close();

}

void  PhaseTriptime::on_pushButton_2_clicked()
{
 emit sendflag();
 close();
}



