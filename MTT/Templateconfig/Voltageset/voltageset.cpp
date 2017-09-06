#include "voltageset.h"
#include "ui_voltageset.h"
#include <QMessageBox>
#include <QDebug>
#include "nodeprocess.h"
#include "caculate.h"

Voltageset::Voltageset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Voltageset)
{
    ui->setupUi(this);
    setWindowTitle(QString("Voltage configure"));
    setWindowIcon(QIcon(":/set.ico"));
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    memset(&sectmentset,0,sizeof(struct SECTMENTSET));
    memset(&voltageset,0,sizeof(VOLTAGESET));
    settingover=false;
    setFixedSize(this->width(),this->height());
}

void Voltageset::setsectmentset(struct SECTMENTSET &set)
{
  sectmentset=set;

}

void Voltageset::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}

void Voltageset::on_pushButton_clicked()
{
    QByteArray ba;
    settingover=false;
    if(ui->lineEdit_9->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input name!"),QMessageBox::Ok);
        return;
    }

    if(sectmentset.volset.valid)
  {  if(QString(QLatin1String(sectmentset.curset.name))==ui->lineEdit_9->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with voltage protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.dirset.valid)
  {  if(QString(QLatin1String(sectmentset.dirset.name))==ui->lineEdit_9->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with directon protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.freset.valid)
  {  if(QString(QLatin1String(sectmentset.freset.name))==ui->lineEdit_9->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with frequency protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.anytestset.valid)
  {  if(QString(QLatin1String(sectmentset.anytestset.name))==ui->lineEdit_9->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with anytest!"),QMessageBox::Ok);
        return;
    }
  }

     ba=ui->lineEdit_9->text().toLatin1(); //2aê?????
     copytoshuzu(voltageset.name,ba.data(),ba.size());

     int testitem=0;
     if(ui->checkBox_6->isChecked())
         testitem+=1;
     if(ui->checkBox_7->isChecked())
         testitem+=2;


     itoa(testitem,voltageset.Testitem.text,10);
     itoa(ui->comboBox_4->currentIndex()+1,voltageset.FaultType.text,10);



     ba=ui->lineEdit_12->text().toLatin1();  //1ê???óê±
     copytoshuzu(voltageset.Faultduraion.text,ba.data(),ba.size());

     voltageset.Vpn_Vpp=ui->comboBox_3->currentIndex()+1;





    if(testitem&0x01)
    {

        if(ui->lineEdit_2->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("start voltage is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_3->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("End voltage is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_4->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("voltage step is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_16->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Step Time is empty!"),QMessageBox::Ok);
            return;
        }

        if(CheckTextValid(settinglink,ui->lineEdit_2->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Start voltage of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_3->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("End voltage of trip value  is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_4->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Step value of trip value  is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_16->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Hold time of trip value  is not valid!"),QMessageBox::Ok);
            return;
        }




        ba=ui->lineEdit_2->text().toLatin1(); //
        copytoshuzu(voltageset.TripValue.StartValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_3->text().toLatin1(); //
        copytoshuzu(voltageset.TripValue.EndValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_4->text().toLatin1(); //
        copytoshuzu(voltageset.TripValue.StepValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_16->text().toLatin1(); //
        copytoshuzu(voltageset.TripValue.HoldTime.text,ba.data(),ba.size());

        if(ui->checkBox->isChecked())
        {
            if(ui->lineEdit_5->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("please input relative error of  tripvalue!"),QMessageBox::Ok);
                return;
            }
            bool ok;
            QString value=ui->lineEdit_5->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Relative error of trip value is invalid!", QMessageBox::Ok);
                return;
            }
            voltageset.TripValue.assesstripvalue.relerror.errorvalue=value.toFloat(&ok);
            voltageset.TripValue.assesstripvalue.relerror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_17->text())==-1)
            {
                QMessageBox::warning(this,tr("Warning"),tr("Comparative id of trip value  is not valid!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_17->text().toLatin1(); //2?3¤ê±??
            copytoshuzu(voltageset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());


        }
        else  voltageset.TripValue.assesstripvalue.relerror.valid=false;

        if(ui->checkBox_2->isChecked())
        {
            if(ui->lineEdit_6->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please  input positive absolute error of  tripvalue!"),QMessageBox::Ok);
                return;
            }

            if(ui->lineEdit->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of  tripvalue!"),QMessageBox::Ok);
                return;
            }


            bool ok;
            QString value=ui->lineEdit_6->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Positive Absolute error of trip value is invalid!", QMessageBox::Ok);
                return;
            }

            value=ui->lineEdit->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Negative Absolute error of trip value is invalid!", QMessageBox::Ok);
                return;
            }


            value=ui->lineEdit_6->text();
            voltageset.TripValue.assesstripvalue.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit->text();
            voltageset.TripValue.assesstripvalue.abserror.errorvalue_2=value.toFloat(&ok);



            voltageset.TripValue.assesstripvalue.abserror.valid=true;


            if(CheckTextValid(settinglink,ui->lineEdit_17->text())==-1)
            {
                QMessageBox::warning(this,tr("Warning"),tr("Comparative id of trip value  is not valid!"),QMessageBox::Ok);
                return;
            }
            ba=ui->lineEdit_17->text().toLatin1(); //2?3¤ê±??
            copytoshuzu(voltageset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());


        }

        else voltageset.TripValue.assesstripvalue.abserror.valid=false;
        voltageset.TripValue.assesstripvalue.assessandor=ui->comboBox->currentIndex()+1;

    }

    if(testitem&0x02)
    {
        if(ui->checkBox_5->isChecked())
        {
        voltageset.TripTime.UseMeasureValue=true;
        if((testitem&0x01)==0)
        {
            QMessageBox::warning(this,tr("Warning"),tr("pick up must be tested!"),QMessageBox::Ok);
            return;

        }
        }
        else
        {
            if(ui->lineEdit_14->text().isEmpty())
            {
                QMessageBox::warning(this,tr("Warning"),tr("stop voltage of trip time is empty!"),QMessageBox::Ok);
                return;
            }
        voltageset.TripTime.UseMeasureValue=false;
        }


        if(ui->lineEdit_15->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Hold Time of trip time is empty!"),QMessageBox::Ok);
            return;

        }

        if(CheckTextValid(settinglink,ui->lineEdit_14->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("End voltage of trip time is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_15->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Hold time of trip time is not valid!"),QMessageBox::Ok);
            return;
        }



        ba=ui->lineEdit_14->text().toLatin1(); //í￡?1μ??1
        copytoshuzu(voltageset.TripTime.EndVaule.text,ba.data(),ba.size());
        ba=ui->lineEdit_15->text().toLatin1(); //±￡3?ê±??
        copytoshuzu(voltageset.TripTime.HoldTime.text,ba.data(),ba.size());
        if(ui->checkBox_3->isChecked())
        {
            if(ui->lineEdit_7->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("please input relative error of  trip time!"),QMessageBox::Ok);
                return;
            }
            bool ok;
            QString value=ui->lineEdit_7->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Relative error of trip time is invalid!", QMessageBox::Ok);
                return;
            }
            voltageset.TripTime.assesstriptime.relerror.errorvalue=value.toFloat(&ok);
            voltageset.TripTime.assesstriptime.relerror.valid=true;
            if(CheckTextValid(settinglink,ui->lineEdit_18->text())==-1)
            {
                QMessageBox::warning(this,tr("Warning"),tr("Comparative id of trip time is not valid!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_18->text().toLatin1(); //2?3¤ê±??
            copytoshuzu(voltageset.TripTime.assesstriptime.CompareID,ba.data(),ba.size());




        }
        else voltageset.TripTime.assesstriptime.relerror.valid=false;

        if(ui->checkBox_4->isChecked())
        {
            if(ui->lineEdit_8->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input positive absolute error of  trip time!"),QMessageBox::Ok);
                return;
            }


            if(ui->lineEdit_13->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of  trip time!"),QMessageBox::Ok);
                return;
            }


            bool ok;
            QString value=ui->lineEdit_8->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Positive Absolute error of trip time is invalid!", QMessageBox::Ok);
                return;
            }

            value=ui->lineEdit_13->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Negative Absolute error of trip time is invalid!", QMessageBox::Ok);
                return;
            }


            value=ui->lineEdit_8->text();
            voltageset.TripTime.assesstriptime.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit_13->text();
            voltageset.TripTime.assesstriptime.abserror.errorvalue_2=value.toFloat(&ok);


            voltageset.TripTime.assesstriptime.abserror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_18->text())==-1)
            {
                QMessageBox::warning(this,tr("Warning"),tr("Comparative id of trip time is not valid!"),QMessageBox::Ok);
                return;
            }


            ba=ui->lineEdit_18->text().toLatin1(); //2?3¤ê±??
            copytoshuzu(voltageset.TripTime.assesstriptime.CompareID,ba.data(),ba.size());

        }
        else voltageset.TripTime.assesstriptime.abserror.valid=false;
        voltageset.TripTime.assesstriptime.assessandor=ui->comboBox_2->currentIndex()+1;

    }
    if(testitem&0x02)
     voltageset.TripTime.valid=true;
     if(testitem&0x01)
     voltageset.TripValue.valid=true;


    settingover=true;

    if(settingover==true)
    {
        emit sendsettingvalue((void *)&voltageset,2);
    }

}


void Voltageset::on_pushButton_2_clicked()
{

    emit sendclose(2);
}

void Voltageset::Setvoltageset(VOLTAGESET voltset)
{


    ui->lineEdit_9->setText(QString(QLatin1String(voltset.name)));
    int testitem=atoi(voltset.Testitem.text);

    int faulttype=atoi(voltset.FaultType.text);

    ui->comboBox_4->setCurrentIndex(faulttype-1);


    ui->lineEdit_12->setText(QString(QLatin1String(voltset.Faultduraion.text)));

    ui->comboBox_3->setCurrentIndex(voltset.Vpn_Vpp-1);


    if(testitem&0x01)
    {
        ui->checkBox_6->setChecked(true);

        ui->lineEdit_2->setText(QString(QLatin1String(voltset.TripValue.StartValue.text)));
        ui->lineEdit_3->setText(QString(QLatin1String(voltset.TripValue.EndValue.text)));
        ui->lineEdit_4->setText(QString(QLatin1String(voltset.TripValue.StepValue.text)));
        ui->lineEdit_16->setText(QString(QLatin1String(voltset.TripValue.HoldTime.text)));
        if(voltset.TripValue.assesstripvalue.relerror.valid)
       { ui->lineEdit_5->setText(QString::number(voltset.TripValue.assesstripvalue.relerror.errorvalue,'g',3));
         ui->checkBox->setChecked(voltset.TripValue.assesstripvalue.relerror.valid);
       }
        if(voltset.TripValue.assesstripvalue.abserror.valid)
        {ui->lineEdit_6->setText(QString::number(voltset.TripValue.assesstripvalue.abserror.errorvalue,'g',3));
         ui->lineEdit->setText(QString::number(voltset.TripValue.assesstripvalue.abserror.errorvalue_2,'g',3));



          ui->checkBox_2->setChecked(voltset.TripValue.assesstripvalue.abserror.valid);
        }
        ui->comboBox->setCurrentIndex(voltset.TripValue.assesstripvalue.assessandor-1);

        ui->lineEdit_17->setText(QString(QLatin1String(voltset.TripValue.assesstripvalue.CompareID)));


    }


    if(testitem&0x02)
    {

        ui->checkBox_7->setChecked(true);
        if(voltset.TripTime.UseMeasureValue==false)
        ui->lineEdit_14->setText(QString(QLatin1String(voltset.TripTime.EndVaule.text)));
        ui->checkBox_5->setChecked(voltset.TripTime.UseMeasureValue);


        ui->lineEdit_15->setText(QString(QLatin1String(voltset.TripTime.HoldTime.text)));
        if(voltset.TripTime.assesstriptime.relerror.valid)
        { ui->lineEdit_7->setText(QString::number(voltset.TripTime.assesstriptime.relerror.errorvalue,'g',3));
          ui->checkBox_3->setChecked(voltset.TripTime.assesstriptime.relerror.valid);
        }
        if(voltset.TripTime.assesstriptime.abserror.valid)
        { ui->lineEdit_8->setText(QString::number(voltset.TripTime.assesstriptime.abserror.errorvalue,'g',3));
          ui->lineEdit_13->setText(QString::number(voltset.TripTime.assesstriptime.abserror.errorvalue_2,'g',3));


            ui->checkBox_4->setChecked(voltset.TripTime.assesstriptime.abserror.valid);
        }
        ui->comboBox_2->setCurrentIndex(voltset.TripTime.assesstriptime.assessandor-1);
        ui->lineEdit_18->setText(QString(QLatin1String(voltset.TripTime.assesstriptime.CompareID)));

    }
}



void Voltageset::closeEvent(QCloseEvent *event)
{
  emit sendclose(2);
}

Voltageset::~Voltageset()
{
    delete ui;
}

void Voltageset::on_checkBox_5_clicked()
{

}
