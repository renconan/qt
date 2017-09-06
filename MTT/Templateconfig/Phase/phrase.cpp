#include "phrase.h"
#include "ui_phrase.h"
#include <QMessageBox>
#include <QDebug>
#include "nodeprocess.h"
#include "caculate.h"

Phrase::Phrase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Phrase)
{
    ui->setupUi(this);
    setWindowTitle(QString("Phase configure"));
    setWindowIcon(QIcon(":/set.ico"));
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    memset(&sectmentset,0,sizeof(struct SECTMENTSET));
    memset(&phraseset,0,sizeof(PHRASESET));
    settingover=false;
    setFixedSize(this->width(),this->height());
}

void Phrase::setsectmentset(struct SECTMENTSET &set)
  {
    sectmentset=set;

  }


void Phrase::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}

//保存
void Phrase::on_pushButton_clicked()
{

    settingover=false;
    QByteArray ba;
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input name!"),QMessageBox::Ok);
        return;
    }



    if(ui->lineEdit_15->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input fault duration!"),QMessageBox::Ok);
        return;
    }

    if(ui->lineEdit_16->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input Fix Voltage!"),QMessageBox::Ok);
        return;
    }
    if(ui->lineEdit_17->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input Fix Current!"),QMessageBox::Ok);
        return;
    }




    if(sectmentset.curset.valid)
  {  if(QString(QLatin1String(sectmentset.curset.name))==ui->lineEdit->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with current protection!"),QMessageBox::Ok);
        return;
    }
  }

    if(sectmentset.volset.valid)
  {  if(QString(QLatin1String(sectmentset.volset.name))==ui->lineEdit->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with voltage protection!"),QMessageBox::Ok);
        return;
    }
  }

    if(sectmentset.dirset.valid)
  {  if(QString(QLatin1String(sectmentset.dirset.name))==ui->lineEdit->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with directon protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.freset.valid)
  {  if(QString(QLatin1String(sectmentset.freset.name))==ui->lineEdit->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with fruquency protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.anytestset.valid)
  {  if(QString(QLatin1String(sectmentset.anytestset.name))==ui->lineEdit->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with anytest!"),QMessageBox::Ok);
        return;
    }
  }



     ba=ui->lineEdit->text().toLatin1();
     copytoshuzu(phraseset.name,ba.data(),ba.size());





     int testitem=0;
     if(ui->checkBox_11->isChecked())
         testitem+=1;
     if(ui->checkBox_12->isChecked())
         testitem+=2;

     itoa(testitem,phraseset.Testitem.text,10);


     ba=ui->lineEdit_15->text().toLatin1();
     copytoshuzu(phraseset.Faultduraion.text,ba.data(),ba.size());


     if(CheckTextValid(settinglink,ui->lineEdit_16->text())==-1)
     {
         QMessageBox::critical(this,tr("Warning"),tr("Voltage of Phase shift test is empty!"),QMessageBox::Ok);
         return;
     }

     if(CheckTextValid(settinglink,ui->lineEdit_17->text())==-1)
     {
         QMessageBox::critical(this,tr("Warning"),tr("Current of Phase shift test is empty!"),QMessageBox::Ok);
         return;
     }


     ba=ui->lineEdit_16->text().toLatin1();
     copytoshuzu(phraseset.FixVoltage.text,ba.data(),ba.size());
     ba=ui->lineEdit_17->text().toLatin1();
     copytoshuzu(phraseset.FixCurrent.text,ba.data(),ba.size());







     if(ui->checkBox->isChecked())
         phraseset.VAch=true;
     if(ui->checkBox_2->isChecked())
         phraseset.VBch=true;
     if(ui->checkBox_3->isChecked())
         phraseset.VCch=true;
     if(ui->checkBox_4->isChecked())
         phraseset.IAch=true;
     if(ui->checkBox_5->isChecked())
         phraseset.IBch=true;
     if(ui->checkBox_6->isChecked())
         phraseset.ICch=true;


    if((testitem&0x01)!=0)
    {

        if(ui->lineEdit_2->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("start phrase is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_3->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("End phrase is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_4->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("phrase step is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_5->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Hold Time is empty!"),QMessageBox::Ok);
            return;
        }


        if(CheckTextValid(settinglink,ui->lineEdit_2->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Start  phase of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_3->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("End  phase of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_4->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Step  phase of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_5->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Hold time of trip value is not valid!"),QMessageBox::Ok);
            return;
        }




        ba=ui->lineEdit_2->text().toLatin1(); //起始值
        copytoshuzu(phraseset.TripValue.StartValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_3->text().toLatin1(); //终值值
        copytoshuzu(phraseset.TripValue.EndValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_4->text().toLatin1(); //步长值
        copytoshuzu(phraseset.TripValue.StepValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_5->text().toLatin1(); //步长时间
        copytoshuzu(phraseset.TripValue.HoldTime.text,ba.data(),ba.size());

        if(ui->checkBox_7->isChecked())
        {
            if(ui->lineEdit_8->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("please input relative error of  tripvalue!"),QMessageBox::Ok);
                return;
            }
            bool ok;
            QString value=ui->lineEdit_8->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Relative error of trip value is invalid!", QMessageBox::Yes);
                return;
            }
            phraseset.TripValue.assesstripvalue.relerror.errorvalue=value.toFloat(&ok);
            phraseset.TripValue.assesstripvalue.relerror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_7->text())==-1)
            {
                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip value is empty!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_7->text().toLatin1();
            copytoshuzu(phraseset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());

        }
        else  phraseset.TripValue.assesstripvalue.relerror.valid=false;

        if(ui->checkBox_8->isChecked())
        {
            if(ui->lineEdit_9->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input Positive absolute error of  tripvalue!"),QMessageBox::Ok);
                return;
            }

            if(ui->lineEdit_18->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of  tripvalue!"),QMessageBox::Ok);
                return;
            }



            bool ok;
            QString value=ui->lineEdit_9->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Positive Absolute error of trip value is invalid!", QMessageBox::Yes);
                return;
            }

            value=ui->lineEdit_18->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Negative Absolute error of trip value is invalid!", QMessageBox::Yes);
                return;
            }


            value=ui->lineEdit_9->text();
            phraseset.TripValue.assesstripvalue.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit_18->text();
            phraseset.TripValue.assesstripvalue.abserror.errorvalue_2=value.toFloat(&ok);

            phraseset.TripValue.assesstripvalue.abserror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_7->text())==-1)
            {
                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip value is empty!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_7->text().toLatin1(); //步长时间
            copytoshuzu(phraseset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());

        }

        else phraseset.TripValue.assesstripvalue.abserror.valid=false;
        phraseset.TripValue.assesstripvalue.assessandor=ui->comboBox->currentIndex()+1;

    }

    if((testitem&0x02)!=0)
    {


        if(ui->lineEdit_10->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("stop phrase of trip time is empty!"),QMessageBox::Ok);
            return;

        }
        if(ui->lineEdit_11->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Hold Time of trip time is empty!"),QMessageBox::Ok);
            return;

        }

        if(CheckTextValid(settinglink,ui->lineEdit_10->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("End Phase of trip time is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_11->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Hold time of trip time is not valid!"),QMessageBox::Ok);
            return;
        }


        ba=ui->lineEdit_10->text().toLatin1(); //停止电流
        copytoshuzu(phraseset.TripTime.EndVaule.text,ba.data(),ba.size());
        ba=ui->lineEdit_11->text().toLatin1(); //保持时间
        copytoshuzu(phraseset.TripTime.HoldTime.text,ba.data(),ba.size());
        if(ui->checkBox_9->isChecked())
        {
            if(ui->lineEdit_13->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("please input relative error of  trip time!"),QMessageBox::Ok);
                return;
            }
            bool ok;
            QString value=ui->lineEdit_13->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Relative error of trip time is invalid!", QMessageBox::Yes);
                return;
            }
            phraseset.TripTime.assesstriptime.relerror.errorvalue=value.toFloat(&ok);
            phraseset.TripTime.assesstriptime.relerror.valid=true;


            if(CheckTextValid(settinglink,ui->lineEdit_12->text())==-1)
            {
                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip time is not valid!"),QMessageBox::Ok);
                return;
            }
            ba=ui->lineEdit_12->text().toLatin1();
            copytoshuzu(phraseset.TripTime.assesstriptime.CompareID,ba.data(),ba.size());

        }
        else phraseset.TripTime.assesstriptime.relerror.valid=false;

        if(ui->checkBox_10->isChecked())
        {
            if(ui->lineEdit_14->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input positive absolute error of  trip time!"),QMessageBox::Ok);
                return;
            }
            if(ui->lineEdit_19->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of  trip time!"),QMessageBox::Ok);
                return;
            }



            bool ok;
            QString value=ui->lineEdit_14->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Positive Absolute error of trip time is invalid!", QMessageBox::Yes);
                return;
            }
            value=ui->lineEdit_19->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Negative Absolute error of trip time is invalid!", QMessageBox::Yes);
                return;
            }


            value=ui->lineEdit_14->text();
            phraseset.TripTime.assesstriptime.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit_19->text();
            phraseset.TripTime.assesstriptime.abserror.errorvalue_2=value.toFloat(&ok);


            phraseset.TripTime.assesstriptime.abserror.valid=true;
        }
        else phraseset.TripTime.assesstriptime.abserror.valid=false;
        phraseset.TripTime.assesstriptime.assessandor=ui->comboBox_2->currentIndex()+1;

        if(CheckTextValid(settinglink,ui->lineEdit_12->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip time is not valid!"),QMessageBox::Ok);
            return;
        }
        ba=ui->lineEdit_12->text().toLatin1();
        copytoshuzu(phraseset.TripTime.assesstriptime.CompareID,ba.data(),ba.size());
    }



    if((testitem&0x02)!=0)
     phraseset.TripTime.valid=true;
     if((testitem&0x01)!=0)
     phraseset.TripValue.valid=true;
     settingover=true;
     if(settingover)
    {
      emit sendsettingvalue((void *)&phraseset,6); //发送数据
    }


}

//退出
void Phrase::on_pushButton_2_clicked()
{
  // 关闭 并将值发送给主界面

    emit sendclose(6);

}



void Phrase::Setphraseset(PHRASESET Phraseset)
{

    ui->lineEdit->setText(QString(QLatin1String(Phraseset.name)));

    int testitem=atoi(Phraseset.Testitem.text);

    ui->lineEdit_15->setText(QString(QLatin1String(Phraseset.Faultduraion.text)));


    ui->lineEdit_16->setText(QString(QLatin1String(Phraseset.FixVoltage.text)));
    ui->lineEdit_17->setText(QString(QLatin1String(Phraseset.FixCurrent.text)));




    ui->checkBox->setChecked(Phraseset.VAch);
    ui->checkBox_2->setChecked(Phraseset.VBch);
    ui->checkBox_3->setChecked(Phraseset.VCch);
    ui->checkBox_4->setChecked(Phraseset.IAch);
    ui->checkBox_5->setChecked(Phraseset.IBch);
    ui->checkBox_6->setChecked(Phraseset.ICch);



    if((testitem&0x01)!=0)
    {
        ui->checkBox_11->setChecked(true);
        ui->lineEdit_2->setText(QString(QLatin1String(Phraseset.TripValue.StartValue.text)));
        ui->lineEdit_3->setText(QString(QLatin1String(Phraseset.TripValue.EndValue.text)));
        ui->lineEdit_4->setText(QString(QLatin1String(Phraseset.TripValue.StepValue.text)));
        ui->lineEdit_5->setText(QString(QLatin1String(Phraseset.TripValue.HoldTime.text)));
        if(Phraseset.TripValue.assesstripvalue.relerror.valid)
       { ui->lineEdit_8->setText(QString::number(Phraseset.TripValue.assesstripvalue.relerror.errorvalue,'g',3));
         ui->checkBox_7->setChecked(Phraseset.TripValue.assesstripvalue.relerror.valid);
       }
        if(Phraseset.TripValue.assesstripvalue.abserror.valid)
        {ui->lineEdit_9->setText(QString::number(Phraseset.TripValue.assesstripvalue.abserror.errorvalue,'g',3));
         ui->lineEdit_18->setText(QString::number(Phraseset.TripValue.assesstripvalue.abserror.errorvalue_2,'g',3));


         ui->checkBox_8->setChecked(Phraseset.TripValue.assesstripvalue.abserror.valid);
        }
        ui->comboBox->setCurrentIndex(Phraseset.TripValue.assesstripvalue.assessandor-1);
        ui->lineEdit_7->setText(QString(QLatin1String(Phraseset.TripValue.assesstripvalue.CompareID)));

    }
    if((testitem&0x02)!=0)
    {

        ui->checkBox_12->setChecked(true);
        ui->lineEdit_10->setText(QString(QLatin1String(Phraseset.TripTime.EndVaule.text)));
        ui->lineEdit_11->setText(QString(QLatin1String(Phraseset.TripTime.HoldTime.text)));

        if(Phraseset.TripTime.assesstriptime.relerror.valid)
        { ui->lineEdit_13->setText(QString::number(Phraseset.TripTime.assesstriptime.relerror.errorvalue,'g',3));
          ui->checkBox_9->setChecked(Phraseset.TripTime.assesstriptime.relerror.valid);
        }
        if(Phraseset.TripTime.assesstriptime.abserror.valid)
        { ui->lineEdit_14->setText(QString::number(Phraseset.TripTime.assesstriptime.abserror.errorvalue,'g',3));
          ui->lineEdit_19->setText(QString::number(Phraseset.TripTime.assesstriptime.abserror.errorvalue_2,'g',3));


          ui->checkBox_10->setChecked(Phraseset.TripTime.assesstriptime.abserror.valid);
        }
        ui->comboBox_2->setCurrentIndex(Phraseset.TripTime.assesstriptime.assessandor-1);
        ui->lineEdit_12->setText(QString(QLatin1String(Phraseset.TripTime.assesstriptime.CompareID)));
    }




}

void Phrase::closeEvent(QCloseEvent *event)
{
  emit sendclose(6);
}


Phrase::~Phrase()
{
    delete ui;
}

void Phrase::on_checkBox_clicked()
{

}
