#include "dfdtblocking.h"
#include "ui_dfdtblocking.h"
#include <QTextCodec>
#include <QMouseEvent>
#include <QMessageBox>

DfdtBlocking::DfdtBlocking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DfdtBlocking)
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
}

DfdtBlocking::~DfdtBlocking()
{
    delete ui;
}


bool DfdtBlocking::eventFilter(QObject *obj, QEvent *event)
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
        else if(obj==ui->lineEdit_10)
         {
              currentedit=ui->lineEdit_10;

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


void DfdtBlocking::setlineedittext(char *str)
{

    currentedit->setText(str);

}

void DfdtBlocking::clearkeypadflag()
{

}



void  DfdtBlocking::Setshowpara(struct SECTMENTSET value)
{

memcpy(&Sectvalue,&value,sizeof(struct SECTMENTSET));
ui->lineEdit->setText(tr("df/dt blocking"));
ui->lineEdit->setEnabled(false);


ui->lineEdit_2->setText(Sectvalue.freset.Tripdfdt.FreEndValue.text);
ui->lineEdit_3->setText(Sectvalue.freset.Tripdfdt.DfdtStart.text);
ui->lineEdit_4->setText(Sectvalue.freset.Tripdfdt.DfdtEnd.text);
ui->lineEdit_5->setText(Sectvalue.freset.Tripdfdt.DfdtStep.text);
ui->lineEdit_6->setText(Sectvalue.freset.Faultduraion.text);


ui->lineEdit_7->setText(Sectvalue.freset.Tripdfdt.assessdfdttrip.CompareID);

if(Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.valid)
{
 ui->checkBox->setChecked(true);
 ui->lineEdit_8->setText(QString::number(Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue));

}
if(Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.valid)
{
 ui->lineEdit_9->setText(QString::number(Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,'g',5));
 ui->lineEdit_10->setText(QString::number(Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_Negtive,'g',5));

 ui->checkBox_2->setChecked(Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.valid);
 }
 ui->comboBox_3->setCurrentIndex(Sectvalue.freset.Tripdfdt.assessdfdttrip.assessandor-1);

}

void  DfdtBlocking::SetSettingLink(struct SETTINGNODELIST *link)
{
settinglink=link;
}

void DfdtBlocking::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}



void DfdtBlocking::on_pushButton_clicked()
{


   if(ui->lineEdit_2->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input stop frequence!"),QMessageBox::Ok);
    return;
   }

   if(ui->lineEdit_3->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input df/dt start!"),QMessageBox::Ok);
    return;
   }

   if(ui->lineEdit_4->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input df/dt end!"),QMessageBox::Ok);
    return;
   }

   if(ui->lineEdit_5->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input df/dt step!"),QMessageBox::Ok);
    return;
   }

   if(ui->lineEdit_6->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input tripping value keeping!"),QMessageBox::Ok);
    return;
   }

   if(ui->lineEdit_7->text().isEmpty())
   {
    QMessageBox::warning(this,tr("Warning"),tr("Please input comparative id!"),QMessageBox::Ok);
    return;
   }



   if(CheckTextValid(settinglink,ui->lineEdit_2->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of stop frequence is invalid!"),QMessageBox::Ok);
       return;
   }
   if(CheckTextValid(settinglink,ui->lineEdit_3->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of df/dt start is invalid!"),QMessageBox::Ok);
       return;
   }
   if(CheckTextValid(settinglink,ui->lineEdit_4->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of df/dt end is invalid!"),QMessageBox::Ok);
       return;
   }
   if(CheckTextValid(settinglink,ui->lineEdit_5->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of df/dt step is invalid!"),QMessageBox::Ok);
       return;
   }
   if(CheckTextValid(settinglink,ui->lineEdit_6->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of tripping value keeping is invalid!"),QMessageBox::Ok);
       return;
   }

   if(CheckTextValid(settinglink,ui->lineEdit_7->text())==-1)
   {
       QMessageBox::warning(this,tr("Warning"),tr("Text of comparative id is invalid!"),QMessageBox::Ok);
       return;
   }


   if(ui->checkBox->isChecked())
   {
       if(ui->lineEdit_8->text().isEmpty())
       {
        QMessageBox::warning(this,tr("Warning"),tr("Please input relative error!"),QMessageBox::Ok);
        return;
       }

       if(CheckTextValid(settinglink,ui->lineEdit_8->text())!=0)
       {
           QMessageBox::warning(this,tr("Warning"),tr("Text of relative error is invalid!"),QMessageBox::Ok);
           return;
       }

   }

   if(ui->checkBox_2->isChecked())
   {
       if(ui->lineEdit_9->text().isEmpty())
       {
        QMessageBox::warning(this,tr("Warning"),tr("Please input positive absolute error!"),QMessageBox::Ok);
        return;
       }

       if(CheckTextValid(settinglink,ui->lineEdit_9->text())!=0)
       {
           QMessageBox::warning(this,tr("Warning"),tr("Text of positive absolute error is invalid!"),QMessageBox::Ok);
           return;
       }

       if(ui->lineEdit_10->text().isEmpty())
       {
        QMessageBox::warning(this,tr("Warning"),tr("Please input negative absolute error!"),QMessageBox::Ok);
        return;
       }

       if(CheckTextValid(settinglink,ui->lineEdit_10->text())!=0)
       {
           QMessageBox::warning(this,tr("Warning"),tr("Text of negative absolute error is invalid!"),QMessageBox::Ok);
           return;
       }

   }


   copytoshuzu(Sectvalue.freset.Tripdfdt.FreEndValue.text,\
               ui->lineEdit_2->text(),\
               ui->lineEdit_2->text().size());
   copytoshuzu(Sectvalue.freset.Tripdfdt.DfdtStart.text,\
               ui->lineEdit_3->text(),\
               ui->lineEdit_3->text().size());
   copytoshuzu(Sectvalue.freset.Tripdfdt.DfdtEnd.text,\
               ui->lineEdit_4->text(),\
               ui->lineEdit_4->text().size());
   copytoshuzu(Sectvalue.freset.Tripdfdt.DfdtStep.text,\
               ui->lineEdit_5->text(),\
               ui->lineEdit_5->text().size());
   copytoshuzu(Sectvalue.freset.Faultduraion.text,\
               ui->lineEdit_6->text(),\
               ui->lineEdit_6->text().size());
   copytoshuzu(Sectvalue.freset.Tripdfdt.assessdfdttrip.CompareID,\
               ui->lineEdit_7->text(),\
               ui->lineEdit_7->text().size());

   if(ui->checkBox->isChecked())
   {
                   QString value=ui->lineEdit_8->text();
                   bool ok;
                   Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue=value.toFloat(&ok);
                   Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.valid=true;
   }
   else
   Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.valid=false;

   if(ui->checkBox_2->isChecked())
   {

               bool ok;
               QString value;
               value=ui->lineEdit_9->text();
               Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue=value.toFloat(&ok);
               value.clear();
               value=ui->lineEdit_10->text();
               Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_Negtive=value.toFloat(&ok);
               Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.valid=true;

   }

   else
   Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.valid=false;

   Sectvalue.freset.Tripdfdt.assessdfdttrip.assessandor=ui->comboBox_3->currentIndex()+1;


 emit Sendpara((void *)&Sectvalue);
 emit sendflag();
 close();

}

void DfdtBlocking::on_pushButton_2_clicked()
{
 emit sendflag();
 close();
}



