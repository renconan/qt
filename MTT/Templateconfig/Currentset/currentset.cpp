#include "currentset.h"
#include "ui_currentset.h"
#include <QMessageBox>
#include <QDebug>
#include "nodeprocess.h"
#include "caculate.h"




Currentset::Currentset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Currentset)
{
    ui->setupUi(this);
    setWindowTitle(QString("Current configure"));
    setWindowIcon(QIcon(":/set.ico"));
        setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    memset(&sectmentset,0,sizeof(struct SECTMENTSET));
    memset(&currentset,0,sizeof(CURRENTSET));
    settingover=false;

    setFixedSize(this->width(),this->height());
}


void Currentset::setsectmentset(struct SECTMENTSET &set)
  {
    sectmentset=set;

  }


void Currentset::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}

//保存
void Currentset::on_pushButton_clicked()
{

    settingover=false;
    QByteArray ba;
    if(ui->lineEdit_9->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input name!"),QMessageBox::Ok);
        return;
    }

    if(sectmentset.volset.valid)
  {  if(QString(QLatin1String(sectmentset.volset.name))==ui->lineEdit_9->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with current protection!"),QMessageBox::Ok);
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
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with fruquency protection!"),QMessageBox::Ok);
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

     ba=ui->lineEdit_9->text().toLatin1();
     copytoshuzu(currentset.name,ba.data(),ba.size());


     int testitem=0;
     if(ui->checkBox_5->isChecked())
         testitem+=1;
     if(ui->checkBox_6->isChecked())
         testitem+=2;

     itoa(testitem,currentset.Testitem.text,10);
     itoa(ui->comboBox_3->currentIndex()+1,currentset.output.text,10);




     ba=ui->lineEdit_12->text().toLatin1();  //故障延时
     copytoshuzu(currentset.Faultduraion.text,ba.data(),ba.size());
     bool ok;




     if(CheckTextValid(settinglink,ui->lineEdit_19->text())==-1)
     {
         QMessageBox::warning(this,tr("Warning"),tr("Fault Voltage  is not valid!"),QMessageBox::Ok);
         return;
     }

     if(CheckTextValid(settinglink,ui->lineEdit_20->text())==-1)
     {
         QMessageBox::warning(this,tr("Warning"),tr("Angle  is not valid!"),QMessageBox::Ok);
         return;
     }



     ba=ui->lineEdit_19->text().toLatin1(); //测试项目
     copytoshuzu(currentset.Vfault.text,ba.data(),ba.size());
     ba=ui->lineEdit_20->text().toLatin1(); //测试项目
     copytoshuzu(currentset.Angle.text,ba.data(),ba.size());








    if((testitem&0x01)!=0)
    {


        if(ui->lineEdit_2->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("start current is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_3->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("End current is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_4->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("current step is empty!"),QMessageBox::Ok);
            return;
        }
        if(ui->lineEdit_16->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Step Time is empty!"),QMessageBox::Ok);
            return;
        }

        if(CheckTextValid(settinglink,ui->lineEdit_2->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("start current is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_3->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("End current is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_4->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Step current is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_16->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Step time is not valid!"),QMessageBox::Ok);
            return;
        }






        ba=ui->lineEdit_2->text().toLatin1(); //起始值
        copytoshuzu(currentset.TripValue.StartValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_3->text().toLatin1(); //终值值
        copytoshuzu(currentset.TripValue.EndValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_4->text().toLatin1(); //步长值
        copytoshuzu(currentset.TripValue.StepValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_16->text().toLatin1(); //步长时间
        copytoshuzu(currentset.TripValue.HoldTime.text,ba.data(),ba.size());

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
                QMessageBox::critical(this, "Warning", "Relative error of trip value is invalid!", QMessageBox::Yes);
                return;
            }
            currentset.TripValue.assesstripvalue.relerror.errorvalue=value.toFloat(&ok);
            currentset.TripValue.assesstripvalue.relerror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_17->text())==-1)
            {
                QMessageBox::warning(this,tr("Warning"),tr("Comparative id of Trip value  is not valid!"),QMessageBox::Ok);
                return;
            }



            ba=ui->lineEdit_17->text().toLatin1();
            copytoshuzu(currentset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());

        }
        else  currentset.TripValue.assesstripvalue.relerror.valid=false;

        if(ui->checkBox_2->isChecked())
        {
            if(ui->lineEdit_6->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input Positive absolute error of  tripvalue!"),QMessageBox::Ok);
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
                QMessageBox::critical(this, "Warning", "Positive Absolute error of trip value is invalid!", QMessageBox::Yes);
                return;
            }

            value=ui->lineEdit->text();
            value.toFloat(&ok);
            if(!ok)
            {
             QMessageBox::critical(this, "Warning", "negative Absolute error of trip value is invalid!", QMessageBox::Yes);
             return;
            }




            value=ui->lineEdit_6->text();
            currentset.TripValue.assesstripvalue.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit->text();
            currentset.TripValue.assesstripvalue.abserror.errorvalue_2=value.toFloat(&ok);

            currentset.TripValue.assesstripvalue.abserror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_17->text())==-1)
            {
                QMessageBox::warning(this,tr("Warning"),tr("Comparative id of Trip value  is not valid!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_17->text().toLatin1(); //步长时间
            copytoshuzu(currentset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());

        }

        else currentset.TripValue.assesstripvalue.abserror.valid=false;
        currentset.TripValue.assesstripvalue.assessandor=ui->comboBox->currentIndex()+1;

    }

    if((testitem&0x02)!=0)
    {


        if(ui->lineEdit_14->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("stop current of trip time is empty!"),QMessageBox::Ok);
            return;

        }
        if(ui->lineEdit_15->text().isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Hold Time of trip time is empty!"),QMessageBox::Ok);
            return;

        }

        if(CheckTextValid(settinglink,ui->lineEdit_14->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Stop current for trip time is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_15->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Hold time for trip time is not valid!"),QMessageBox::Ok);
            return;
        }

        ba=ui->lineEdit_14->text().toLatin1(); //停止电流
        copytoshuzu(currentset.TripTime.EndVaule.text,ba.data(),ba.size());
        ba=ui->lineEdit_15->text().toLatin1(); //保持时间
        copytoshuzu(currentset.TripTime.HoldTime.text,ba.data(),ba.size());
        if(ui->checkBox_3->isChecked())
        {
            if(ui->lineEdit_7->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("please input  relative error of  trip time!"),QMessageBox::Ok);
                return;
            }

            bool ok;
            QString value=ui->lineEdit_7->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Relative error of trip time is invalid!", QMessageBox::Yes);
                return;
            }
            currentset.TripTime.assesstriptime.relerror.errorvalue=value.toFloat(&ok);
            currentset.TripTime.assesstriptime.relerror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_18->text())==-1)
            {
                QMessageBox::warning(this,tr("Warning"),tr("Comparative id of Trip time  is not valid!"),QMessageBox::Ok);
                return;
            }


            ba=ui->lineEdit_18->text().toLatin1();
            copytoshuzu(currentset.TripTime.assesstriptime.CompareID,ba.data(),ba.size());

        }
        else currentset.TripTime.assesstriptime.relerror.valid=false;


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
                QMessageBox::critical(this, "Warning", "Positive Absolute error of trip time is invalid!", QMessageBox::Yes);
                return;
            }

            value=ui->lineEdit_13->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Negative Absolute error of trip time is invalid!", QMessageBox::Yes);
                return;
            }

            value=ui->lineEdit_8->text();
            currentset.TripTime.assesstriptime.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit_13->text();
            currentset.TripTime.assesstriptime.abserror.errorvalue_2=value.toFloat(&ok);

            currentset.TripTime.assesstriptime.abserror.valid=true;
        }
        else currentset.TripTime.assesstriptime.abserror.valid=false;
        currentset.TripTime.assesstriptime.assessandor=ui->comboBox_2->currentIndex()+1;

        if(CheckTextValid(settinglink,ui->lineEdit_18->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Comparative id of Trip time  is not valid!"),QMessageBox::Ok);
            return;
        }
        ba=ui->lineEdit_18->text().toLatin1();
        copytoshuzu(currentset.TripTime.assesstriptime.CompareID,ba.data(),ba.size());
    }



    if((testitem&0x02)!=0)
     currentset.TripTime.valid=true;
     if((testitem&0x01)!=0)
     currentset.TripValue.valid=true;
     settingover=true;

     if(settingover)
    {
      emit sendsettingvalue((void *)&currentset,1); //发送数据
    }

}

//退出
void Currentset::on_pushButton_2_clicked()
{
  // 关闭 并将值发送给主界面
  emit sendclose(1);

}


void Currentset::Setcurrentset(CURRENTSET curset)
{

    ui->lineEdit_9->setText(QString(QLatin1String(curset.name)));
  // ui->lineEdit_10->setText(QString(QLatin1String(curset.Testitem.text)));
 //   ui->lineEdit_11->setText(QString(QLatin1String(curset.output.text)));

    int outputtype=atoi(curset.output.text);
    ui->comboBox_3->setCurrentIndex(outputtype-1);

    ui->lineEdit_12->setText(QString(QLatin1String(curset.Faultduraion.text)));

    ui->lineEdit_19->setText(QString(curset.Vfault.text));
    ui->lineEdit_20->setText(QString(curset.Angle.text));

    int testitem=atoi(curset.Testitem.text);
    if((testitem&0x01)!=0)
    {

        ui->checkBox_5->setChecked(true);
        ui->lineEdit_2->setText(QString(QLatin1String(curset.TripValue.StartValue.text)));
        ui->lineEdit_3->setText(QString(QLatin1String(curset.TripValue.EndValue.text)));
        ui->lineEdit_4->setText(QString(QLatin1String(curset.TripValue.StepValue.text)));
        ui->lineEdit_16->setText(QString(QLatin1String(curset.TripValue.HoldTime.text)));
        if(curset.TripValue.assesstripvalue.relerror.valid)
       { ui->lineEdit_5->setText(QString::number(curset.TripValue.assesstripvalue.relerror.errorvalue,'g',5));
         ui->checkBox->setChecked(curset.TripValue.assesstripvalue.relerror.valid);
       }
        if(curset.TripValue.assesstripvalue.abserror.valid)
        {ui->lineEdit_6->setText(QString::number(curset.TripValue.assesstripvalue.abserror.errorvalue,'g',5));
         ui->lineEdit->setText(QString::number(curset.TripValue.assesstripvalue.abserror.errorvalue_2,'g',5));
         ui->checkBox_2->setChecked(curset.TripValue.assesstripvalue.abserror.valid);



        }
        ui->comboBox->setCurrentIndex(curset.TripValue.assesstripvalue.assessandor-1);
        ui->lineEdit_17->setText(QString(QLatin1String(curset.TripValue.assesstripvalue.CompareID)));

    }
    if((testitem&0x02)!=0)
    {

        ui->checkBox_6->setChecked(true);
        ui->lineEdit_14->setText(QString(QLatin1String(curset.TripTime.EndVaule.text)));
        ui->lineEdit_15->setText(QString(QLatin1String(curset.TripTime.HoldTime.text)));

        if(curset.TripTime.assesstriptime.relerror.valid)
        { ui->lineEdit_7->setText(QString::number(curset.TripTime.assesstriptime.relerror.errorvalue,'g',3));
          ui->checkBox_3->setChecked(curset.TripTime.assesstriptime.relerror.valid);
        }
        if(curset.TripTime.assesstriptime.abserror.valid)
        { ui->lineEdit_8->setText(QString::number(curset.TripTime.assesstriptime.abserror.errorvalue,'g',3));
          ui->lineEdit_13->setText(QString::number(curset.TripTime.assesstriptime.abserror.errorvalue_2,'g',3));
          ui->checkBox_4->setChecked(curset.TripTime.assesstriptime.abserror.valid);

        }
        ui->comboBox_2->setCurrentIndex(curset.TripTime.assesstriptime.assessandor-1);
        ui->lineEdit_18->setText(QString(QLatin1String(curset.TripTime.assesstriptime.CompareID)));
    }




}

void Currentset::closeEvent(QCloseEvent *event)
{
  emit sendclose(1);
}




Currentset::~Currentset()
{
    delete ui;
}
