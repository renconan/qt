#include "voltriptime.h"
#include "ui_voltriptime.h"
#include <QTextCodec>
#include <QMouseEvent>
#include <QMessageBox>

VolTriptime::VolTriptime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VolTriptime)
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

        ui->lineEdit_2->installEventFilter(this);
        ui->lineEdit_3->installEventFilter(this);
        ui->lineEdit_4->installEventFilter(this);
        ui->lineEdit_5->installEventFilter(this);
        ui->lineEdit_6->installEventFilter(this);
        ui->lineEdit_7->installEventFilter(this);


        ui->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_3->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_4->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_5->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_6->setContextMenuPolicy(Qt::NoContextMenu);
        ui->lineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);


}

VolTriptime::~VolTriptime()
{
    delete ui;
}

 bool VolTriptime::eventFilter(QObject *obj, QEvent *event)
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


void  VolTriptime::Setshowpara(struct SECTMENTSET value)
{

memcpy(&Sectvalue,&value,sizeof(struct SECTMENTSET));
if(Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
   Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1)
ui->lineEdit->setText(tr("Stability"));
else
ui->lineEdit->setText(tr("Trip Time"));


ui->lineEdit->setEnabled(false);
CaculateVariable(settinglink,Sectvalue.volset.FaultType);
int Faulttype=(int)(Sectvalue.volset.FaultType.value);
ui->comboBox->setCurrentIndex(Faulttype-1);
ui->comboBox_2->setCurrentIndex(Sectvalue.volset.Vpn_Vpp-1);


ui->checkBox_3->setChecked(Sectvalue.volset.TripTime.UseMeasureValue);
if(Sectvalue.volset.TripTime.UseMeasureValue==false)
ui->lineEdit_2->setText(Sectvalue.volset.TripTime.EndVaule.text);

ui->lineEdit_3->setText(Sectvalue.volset.TripTime.HoldTime.text);
ui->lineEdit_4->setText(Sectvalue.volset.TripTime.assesstriptime.CompareID);





if(Sectvalue.volset.TripTime.assesstriptime.relerror.valid)
{
 ui->checkBox->setChecked(true);
 ui->lineEdit_5->setText(QString::number(Sectvalue.volset.TripTime.assesstriptime.relerror.errorvalue));

}
if(Sectvalue.volset.TripTime.assesstriptime.abserror.valid)
{
 ui->lineEdit_6->setText(QString::number(Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue,'g',5));
 ui->lineEdit_7->setText(QString::number(Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive,'g',5));

 ui->checkBox_2->setChecked(Sectvalue.volset.TripTime.assesstriptime.abserror.valid);
 }
 ui->comboBox_3->setCurrentIndex(Sectvalue.volset.TripTime.assesstriptime.assessandor-1);

}

void  VolTriptime::SetSettingLink(struct SETTINGNODELIST *link)
{
settinglink=link;
}

void VolTriptime::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}



void VolTriptime::setlineedittext(char *str)
{

    currentedit->setText(str);

}

void VolTriptime::clearkeypadflag()
{

}





void VolTriptime::on_pushButton_clicked()
{
    int Faulttype=ui->comboBox->currentIndex()+1;
    int VpnVpp=ui->comboBox_2->currentIndex()+1;


    if(ui->checkBox_3->isChecked()==false)
    {
    if(ui->lineEdit_2->text().isEmpty())
    {
     QMessageBox::warning(this,tr("Warning"),tr("Please input End voltage!"),QMessageBox::Ok);
     return;
    }

    }


    if(ui->lineEdit_3->text().isEmpty())
    {
     QMessageBox::warning(this,tr("Warning"),tr("Please input hold time!"),QMessageBox::Ok);
     return;
    }

    if(ui->lineEdit_4->text().isEmpty())
    {
     QMessageBox::warning(this,tr("Warning"),tr("Please input comparative id!"),QMessageBox::Ok);
     return;
    }




    if(ui->checkBox_3->isChecked()==false)
   {
       if(CheckTextValid(settinglink,ui->lineEdit_2->text())==-1)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Text of end voltage is invalid!"),QMessageBox::Ok);
        return;
    }

   }

    if(CheckTextValid(settinglink,ui->lineEdit_3->text())==-1)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Text of hold time is invalid!"),QMessageBox::Ok);
        return;
    }

    if(CheckTextValid(settinglink,ui->lineEdit_4->text())==-1)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Text of comparative id is invalid!"),QMessageBox::Ok);
        return;
    }


    if(ui->checkBox->isChecked())
    {
        if(ui->lineEdit_5->text().isEmpty())
        {
         QMessageBox::warning(this,tr("Warning"),tr("Please input relative error!"),QMessageBox::Ok);
         return;
        }

        if(CheckTextValid(settinglink,ui->lineEdit_5->text())!=0)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Text of relative error is invalid!"),QMessageBox::Ok);
            return;
        }

    }

    if(ui->checkBox_2->isChecked())
    {
        if(ui->lineEdit_6->text().isEmpty())
        {
         QMessageBox::warning(this,tr("Warning"),tr("Please input positive absolute error!"),QMessageBox::Ok);
         return;
        }

        if(CheckTextValid(settinglink,ui->lineEdit_6->text())!=0)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Text of positive absolute error is invalid!"),QMessageBox::Ok);
            return;
        }

        if(ui->lineEdit_7->text().isEmpty())
        {
         QMessageBox::warning(this,tr("Warning"),tr("Please input negative absolute error!"),QMessageBox::Ok);
         return;
        }

        if(CheckTextValid(settinglink,ui->lineEdit_7->text())!=0)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Text of negative absolute error is invalid!"),QMessageBox::Ok);
            return;
        }



    }

    sprintf(Sectvalue.volset.FaultType.text,"%d",Faulttype);
    Sectvalue.volset.Vpn_Vpp=VpnVpp;

    if(ui->checkBox_3->isChecked())
      Sectvalue.volset.TripTime.UseMeasureValue=true;
    else
      Sectvalue.volset.TripTime.UseMeasureValue=false;




    if(Sectvalue.volset.TripTime.UseMeasureValue==false)
    copytoshuzu(Sectvalue.volset.TripTime.EndVaule.text,\
                ui->lineEdit_2->text(),\
                ui->lineEdit_2->text().size());

    copytoshuzu(Sectvalue.volset.TripTime.HoldTime.text,\
                ui->lineEdit_3->text(),\
                ui->lineEdit_3->text().size());


    copytoshuzu(Sectvalue.volset.TripTime.assesstriptime.CompareID,\
                ui->lineEdit_4->text(),\
                ui->lineEdit_4->text().size());

    if(ui->checkBox->isChecked())
    {
                    QString value=ui->lineEdit_5->text();
                    bool ok;
                    Sectvalue.volset.TripTime.assesstriptime.relerror.errorvalue=value.toFloat(&ok);
                    Sectvalue.volset.TripTime.assesstriptime.relerror.valid=true;
    }
    else
    Sectvalue.volset.TripTime.assesstriptime.relerror.valid=false;

    if(ui->checkBox_2->isChecked())
    {

                bool ok;
                QString value;
                value=ui->lineEdit_6->text();
                Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue=value.toFloat(&ok);
                value.clear();
                value=ui->lineEdit_7->text();
                Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive=value.toFloat(&ok);
                Sectvalue.volset.TripTime.assesstriptime.abserror.valid=true;

    }

    else
    Sectvalue.volset.TripTime.assesstriptime.abserror.valid=false;

    Sectvalue.volset.TripTime.assesstriptime.assessandor=ui->comboBox_3->currentIndex()+1;


  emit Sendpara((void *)&Sectvalue);
  emit sendflag();
  close();
}

void VolTriptime::on_pushButton_2_clicked()
{
    emit sendflag();
    close();
}
