#include "harmonic.h"
#include "ui_harmonic.h"
#include <QMessageBox>
#include <QDebug>
#include "nodeprocess.h"
#include "caculate.h"

Harmonic::Harmonic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Harmonic)
{
    ui->setupUi(this);
    setWindowTitle(QString("Harmonic configure"));
    setWindowIcon(QIcon(":/set.ico"));
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    memset(&sectmentset,0,sizeof(struct SECTMENTSET));
    memset(&harmonicset,0,sizeof(HARMONICSET));
    settingover=false;
    setFixedSize(this->width(),this->height());
}

void Harmonic::setsectmentset(struct SECTMENTSET &set)
  {
    sectmentset=set;

  }


void Harmonic::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}

//保存
void Harmonic::on_pushButton_clicked()
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
        QMessageBox::critical(this,tr("Warning"),tr("please input Harmonic number!"),QMessageBox::Ok);
        return;
    }

    if(ui->lineEdit_16->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input Fundamental wave phase!"),QMessageBox::Ok);
        return;
    }
    if(ui->lineEdit_17->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input Fundamental wave current!"),QMessageBox::Ok);
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
    if(sectmentset.phraseset.valid)
  {  if(QString(QLatin1String(sectmentset.phraseset.name))==ui->lineEdit->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with phase!"),QMessageBox::Ok);
        return;
    }
  }


     ba=ui->lineEdit->text().toLatin1();
     copytoshuzu(harmonicset.name,ba.data(),ba.size());





//     int testitem=0;
//     if(ui->checkBox_11->isChecked())
//         testitem+=1;
//     if(ui->checkBox_12->isChecked())
//         testitem+=2;

//     itoa(testitem,harmonicset.Testitem.text,10);


     ba=ui->lineEdit_15->text().toLatin1();
     copytoshuzu(harmonicset.HarmonicCount.text,ba.data(),ba.size());


     if(CheckTextValid(settinglink,ui->lineEdit_16->text())==-1)
     {
         QMessageBox::critical(this,tr("Warning"),tr("Text of Fundamental wave phase is invalid!!"),QMessageBox::Ok);
         return;
     }

     if(CheckTextValid(settinglink,ui->lineEdit_17->text())==-1)
     {
         QMessageBox::critical(this,tr("Warning"),tr("Text of Fundamental wave current is invalid!!"),QMessageBox::Ok);
         return;
     }


     ba=ui->lineEdit_16->text().toLatin1();
     copytoshuzu(harmonicset.Phase.text,ba.data(),ba.size());
     ba=ui->lineEdit_17->text().toLatin1();
     copytoshuzu(harmonicset.Current.text,ba.data(),ba.size());


     if(ui->checkBox_4->isChecked())
         harmonicset.IAch=true;
     if(ui->checkBox_5->isChecked())
         harmonicset.IBch=true;
     if(ui->checkBox_6->isChecked())
         harmonicset.ICch=true;



        if(ui->lineEdit_2->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("start harmonic is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_3->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("End harmonic is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_4->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("harmonic step is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_5->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Hold Time is empty!"),QMessageBox::Ok);
            return;
        }


        if(CheckTextValid(settinglink,ui->lineEdit_2->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Start  Harmonic of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_3->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("End  Harmonic of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_4->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Step  Harmonic of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_5->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Hold time of trip value is not valid!"),QMessageBox::Ok);
            return;
        }




        ba=ui->lineEdit_2->text().toLatin1(); //起始值
        copytoshuzu(harmonicset.TripValue.StartValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_3->text().toLatin1(); //终值值
        copytoshuzu(harmonicset.TripValue.EndValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_4->text().toLatin1(); //步长值
        copytoshuzu(harmonicset.TripValue.StepValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_5->text().toLatin1(); //步长时间
        copytoshuzu(harmonicset.TripValue.HoldTime.text,ba.data(),ba.size());

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
            harmonicset.TripValue.assesstripvalue.relerror.errorvalue=value.toFloat(&ok);
            harmonicset.TripValue.assesstripvalue.relerror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_7->text())==-1)
            {
                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip value is empty!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_7->text().toLatin1();
            copytoshuzu(harmonicset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());

        }
        else  harmonicset.TripValue.assesstripvalue.relerror.valid=false;

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
            harmonicset.TripValue.assesstripvalue.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit_18->text();
            harmonicset.TripValue.assesstripvalue.abserror.errorvalue_2=value.toFloat(&ok);

            harmonicset.TripValue.assesstripvalue.abserror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_7->text())==-1)
            {
                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip value is empty!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_7->text().toLatin1(); //CompareID
            copytoshuzu(harmonicset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());

        }

        else harmonicset.TripValue.assesstripvalue.abserror.valid=false;
        harmonicset.TripValue.assesstripvalue.assessandor=ui->comboBox->currentIndex()+1;



     harmonicset.TripValue.valid=true;
     settingover=true;
     if(settingover)
    {
      emit sendsettingvalue((void *)&harmonicset,7); //发送数据
    }


}

//退出
void Harmonic::on_pushButton_2_clicked()
{
  // 关闭 并将值发送给主界面

    emit sendclose(7);

}



void Harmonic::Setharmonicset(HARMONICSET Harmonicset)
{

    ui->lineEdit->setText(QString(QLatin1String(Harmonicset.name)));

    ui->lineEdit_15->setText(QString(QLatin1String(Harmonicset.HarmonicCount.text)));
    ui->lineEdit_16->setText(QString(QLatin1String(Harmonicset.Phase.text)));
    ui->lineEdit_17->setText(QString(QLatin1String(Harmonicset.Current.text)));

    ui->checkBox_4->setChecked(Harmonicset.IAch);
    ui->checkBox_5->setChecked(Harmonicset.IBch);
    ui->checkBox_6->setChecked(Harmonicset.ICch);




        ui->lineEdit_2->setText(QString(QLatin1String(Harmonicset.TripValue.StartValue.text)));
        ui->lineEdit_3->setText(QString(QLatin1String(Harmonicset.TripValue.EndValue.text)));
        ui->lineEdit_4->setText(QString(QLatin1String(Harmonicset.TripValue.StepValue.text)));
        ui->lineEdit_5->setText(QString(QLatin1String(Harmonicset.TripValue.HoldTime.text)));
        if(Harmonicset.TripValue.assesstripvalue.relerror.valid)
       { ui->lineEdit_8->setText(QString::number(Harmonicset.TripValue.assesstripvalue.relerror.errorvalue,'g',3));
         ui->checkBox_7->setChecked(Harmonicset.TripValue.assesstripvalue.relerror.valid);
       }
        if(Harmonicset.TripValue.assesstripvalue.abserror.valid)
        {ui->lineEdit_9->setText(QString::number(Harmonicset.TripValue.assesstripvalue.abserror.errorvalue,'g',3));
         ui->lineEdit_18->setText(QString::number(Harmonicset.TripValue.assesstripvalue.abserror.errorvalue_2,'g',3));


         ui->checkBox_8->setChecked(Harmonicset.TripValue.assesstripvalue.abserror.valid);
        }
        ui->comboBox->setCurrentIndex(Harmonicset.TripValue.assesstripvalue.assessandor-1);
        ui->lineEdit_7->setText(QString(QLatin1String(Harmonicset.TripValue.assesstripvalue.CompareID)));
}

void Harmonic::closeEvent(QCloseEvent *event)
{
  emit sendclose(7);
}


Harmonic::~Harmonic()
{
    delete ui;
}


