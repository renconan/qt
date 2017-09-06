#include "frequency.h"
#include "ui_frequency.h"
#include  <QMessageBox>
#include <QDebug>
#include "nodeprocess.h"
#include "caculate.h"




Frequency::Frequency(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Frequency)
{
    ui->setupUi(this);
    setWindowTitle(QString("Frequency configure"));
    setWindowIcon(QIcon(":/set.ico"));
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    memset(&freset,0,sizeof(FRECURRENCESET));
    settingover=false;
    setFixedSize(this->width(),this->height());
}


void Frequency::setsectmentset(struct SECTMENTSET &set)
{
  sectmentset=set;
}



void Frequency::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}

//保存
void Frequency::on_pushButton_clicked()
{

    QByteArray ba;
    settingover=false;
    if(ui->lineEdit_16->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("name is empty!"),QMessageBox::Ok);
        return;
    }

    if(ui->lineEdit_6->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Fault duration is empty!"),QMessageBox::Ok);
        return;
    }


    ba=ui->lineEdit_16->text().toLatin1();
    copytoshuzu(freset.name,ba.data(),ba.size());

    ba=ui->lineEdit_6->text().toLatin1();
    copytoshuzu(freset.Faultduraion.text,ba.data(),ba.size());



    int testitem=0;

    if(ui->checkBox_11->isChecked())
        testitem+=1;
    if(ui->checkBox_12->isChecked())
        testitem+=2;
    if(ui->checkBox_13->isChecked())
        testitem+=4;
    if(ui->checkBox_14->isChecked())
        testitem+=8;
    if(ui->checkBox_15->isChecked())
        testitem+=16;

    itoa(testitem,freset.TestItem.text,10);



    if(sectmentset.dirset.valid)
  {  if(QString(QLatin1String(sectmentset.dirset.name))==ui->lineEdit_6->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with direction protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.curset.valid)
  {  if(QString(QLatin1String(sectmentset.curset.name))==ui->lineEdit_6->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with current protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.volset.valid)
  {  if(QString(QLatin1String(sectmentset.volset.name))==ui->lineEdit_6->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with voltage protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.anytestset.valid)
  {  if(QString(QLatin1String(sectmentset.anytestset.name))==ui->lineEdit_6->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with anytest!"),QMessageBox::Ok);
        return;
    }
  }





    if((testitem&0x01)!=0) ///动作频率
    {

        if(ui->lineEdit_2->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Frequency start of trip value is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_3->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Frequency end of trip value is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_4->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Frequency step of trip value is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_5->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Step time of trip value is empty!"),QMessageBox::Ok);
          return;
        }

        if(CheckTextValid(settinglink,ui->lineEdit_2->text())==-1)
        {

            QMessageBox::critical(this,tr("Warning"),tr("Freq start of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_3->text())==-1)
        {

            QMessageBox::critical(this,tr("Warning"),tr("Freq end of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_4->text())==-1)
        {

            QMessageBox::critical(this,tr("Warning"),tr("Freq step of trip value is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_5->text())==-1)
        {

            QMessageBox::critical(this,tr("Warning"),tr("step time of trip value is not valid!"),QMessageBox::Ok);
            return;
        }



        ba=ui->lineEdit_2->text().toLatin1();
        copytoshuzu(freset.TripValue.FreEndValueStart.text,ba.data(),ba.size());
        ba=ui->lineEdit_3->text().toLatin1();
        copytoshuzu(freset.TripValue.FreEndValueEnd.text,ba.data(),ba.size());
        ba=ui->lineEdit_4->text().toLatin1();
        copytoshuzu(freset.TripValue.FreStep.text,ba.data(),ba.size());
        ba=ui->lineEdit_5->text().toLatin1();
        copytoshuzu(freset.TripValue.HoldTime.text,ba.data(),ba.size());

        if(ui->checkBox->isChecked())
        {
            if(ui->lineEdit_7->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("please input relative error of  trip frequency!"),QMessageBox::Ok);
                return;
            }
            bool ok;
            QString value=ui->lineEdit_7->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Relative error of trip frequency is invalid!", QMessageBox::Yes);
                return;
            }
            freset.TripValue.assesstripvalue.relerror.errorvalue=value.toFloat(&ok);
            freset.TripValue.assesstripvalue.relerror.valid=true;


            if(CheckTextValid(settinglink,ui->lineEdit_32->text())==-1)
            {

                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip value is not valid!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_32->text().toLatin1();
            copytoshuzu(freset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());



        }
        else freset.TripValue.assesstripvalue.relerror.valid=false;

        if(ui->checkBox_2->isChecked())
        {
            if(ui->lineEdit_8->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input positive absolute error of trip frequency!"),QMessageBox::Ok);
                return;
            }

            if(ui->lineEdit_29->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of trip frequency!"),QMessageBox::Ok);
                return;
            }



            bool ok;
            QString value=ui->lineEdit_8->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Positive Absolute error of trip frequency is invalid!", QMessageBox::Yes);
                return;
            }

            value=ui->lineEdit_29->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Negative Absolute error of trip frequency is invalid!", QMessageBox::Yes);
                return;
            }



            value=ui->lineEdit_8->text();
            freset.TripValue.assesstripvalue.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit_29->text();
            freset.TripValue.assesstripvalue.abserror.errorvalue_2=value.toFloat(&ok);



            freset.TripValue.assesstripvalue.abserror.valid=true;



            if(CheckTextValid(settinglink,ui->lineEdit_32->text())==-1)
            {

                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip value is not valid!"),QMessageBox::Ok);
                return;
            }


            ba=ui->lineEdit_32->text().toLatin1();
            copytoshuzu(freset.TripValue.assesstripvalue.CompareID,ba.data(),ba.size());


        }
        else  freset.TripValue.assesstripvalue.abserror.valid=false;

        freset.TripValue.assesstripvalue.assessandor=ui->comboBox->currentIndex()+1;


    }

    if((testitem&0x02)!=0) // 触发时间
    {

        if(ui->lineEdit_14->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Stop frequency of trip time is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_15->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Hold Time of trip time is empty!"),QMessageBox::Ok);
          return;
        }

        if(CheckTextValid(settinglink,ui->lineEdit_14->text())==-1)
        {

            QMessageBox::critical(this,tr("Warning"),tr("End freq of trip time is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_15->text())==-1)
        {

            QMessageBox::critical(this,tr("Warning"),tr("End freq of hold time is not valid!"),QMessageBox::Ok);
            return;
        }

        ba=ui->lineEdit_14->text().toLatin1();
        copytoshuzu(freset.TripTime.FreEndValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_15->text().toLatin1();
        copytoshuzu(freset.TripTime.HoldTime.text,ba.data(),ba.size());

        if(ui->checkBox_3->isChecked())
        {
            if(ui->lineEdit_9->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("please input relative error of  trip time!"),QMessageBox::Ok);
                return;
            }
            bool ok;
            QString value=ui->lineEdit_9->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Relative error of trip time is invalid!", QMessageBox::Yes);
                return;
            }
            freset.TripTime.assesstriptime.relerror.errorvalue=value.toFloat(&ok);
            freset.TripTime.assesstriptime.relerror.valid=true;
            if(CheckTextValid(settinglink,ui->lineEdit_35->text())==-1)
            {

                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip time is not valid!"),QMessageBox::Ok);
                return;
            }


            ba=ui->lineEdit_35->text().toLatin1();
            copytoshuzu(freset.TripTime.assesstriptime.CompareID,ba.data(),ba.size());

        }
        else  freset.TripTime.assesstriptime.relerror.valid=false;
        if(ui->checkBox_4->isChecked())
        {
            if(ui->lineEdit_10->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input positive absolute error of trip time!"),QMessageBox::Ok);
                return;
            }
            if(ui->lineEdit_38->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of trip time!"),QMessageBox::Ok);
                return;
            }


            bool ok;
            QString value=ui->lineEdit_10->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Positive Absolute error of trip time is invalid!", QMessageBox::Yes);
                return;
            }
            value=ui->lineEdit_38->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Negative Absolute error of trip time is invalid!", QMessageBox::Yes);
                return;
            }



            value=ui->lineEdit_10->text();
            freset.TripTime.assesstriptime.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit_38->text();
            freset.TripTime.assesstriptime.abserror.errorvalue_2=value.toFloat(&ok);


            freset.TripTime.assesstriptime.abserror.valid=true;


            if(CheckTextValid(settinglink,ui->lineEdit_35->text())==-1)
            {

                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of trip time is not valid!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_35->text().toLatin1();
            copytoshuzu(freset.TripTime.assesstriptime.CompareID,ba.data(),ba.size());

        }
        else freset.TripTime.assesstriptime.abserror.valid=false;
        freset.TripTime.assesstriptime.assessandor=ui->comboBox_2->currentIndex()+1;
    }


    if((testitem&0x04)!=0)  //dfdt闭锁值
    {

        if(ui->lineEdit_18->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Frequency stop of dfdt Blocking is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_19->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("dfdt start of dfdt Blocking is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_20->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("dfdt end of dfdt Blocking is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_21->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("dfdt step of dfdt Blocking is empty!"),QMessageBox::Ok);
          return;
        }




        if(CheckTextValid(settinglink,ui->lineEdit_18->text())==-1)
        {

            QMessageBox::critical(this,tr("Warning"),tr("Stop freq of df/dt blocking is not valid!"),QMessageBox::Ok);
            return;
        }


        if(CheckTextValid(settinglink,ui->lineEdit_19->text())==-1)
        {

            QMessageBox::critical(this,tr("Warning"),tr("dfdt start of df/dt blocking is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_20->text())==-1)
        {

            QMessageBox::critical(this,tr("Warning"),tr("dfdt end of df/dt blocking is not valid!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_21->text())==-1)
         {

             QMessageBox::critical(this,tr("Warning"),tr("dfdt step of df/dt blocking is not valid!"),QMessageBox::Ok);
             return;
         }



        ba=ui->lineEdit_18->text().toLatin1();
        copytoshuzu(freset.Tripdfdt.FreEndValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_19->text().toLatin1();
        copytoshuzu(freset.Tripdfdt.DfdtStart.text,ba.data(),ba.size());
        ba=ui->lineEdit_20->text().toLatin1();
        copytoshuzu(freset.Tripdfdt.DfdtEnd.text,ba.data(),ba.size());
        ba=ui->lineEdit_21->text().toLatin1();
        copytoshuzu(freset.Tripdfdt.DfdtStep.text,ba.data(),ba.size());



        if(ui->checkBox_7->isChecked())
        {
            if(ui->lineEdit_30->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("please input relative error of  dfdt blocking!"),QMessageBox::Ok);
                return;
            }
            bool ok;
            QString value=ui->lineEdit_30->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Relative error of dfdt blocking is invalid!", QMessageBox::Yes);
                return;
            }
            freset.Tripdfdt.assessdfdttrip.relerror.errorvalue=value.toFloat(&ok);
            freset.Tripdfdt.assessdfdttrip.relerror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_34->text())==-1)
             {

                 QMessageBox::critical(this,tr("Warning"),tr("comparative id of df/dt blocking is not valid!"),QMessageBox::Ok);
                 return;
             }

            ba=ui->lineEdit_34->text().toLatin1();
            copytoshuzu(freset.Tripdfdt.assessdfdttrip.CompareID,ba.data(),ba.size());


        }
        else freset.Tripdfdt.assessdfdttrip.relerror.valid=false;

        if(ui->checkBox_8->isChecked())
        {
            if(ui->lineEdit_31->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input positive absolute error of dfdt blocking!"),QMessageBox::Ok);
                return;
            }

            if(ui->lineEdit_39->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of dfdt blocking!"),QMessageBox::Ok);
                return;
            }



            bool ok;
            QString value=ui->lineEdit_31->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Positive Absolute error of dfdt blocking is invalid!", QMessageBox::Yes);
                return;
            }
            value=ui->lineEdit_39->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Negative Absolute error of dfdt blocking is invalid!", QMessageBox::Yes);
                return;
            }


            value=ui->lineEdit_31->text();
            freset.Tripdfdt.assessdfdttrip.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit_39->text();
            freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_2=value.toFloat(&ok);


            freset.Tripdfdt.assessdfdttrip.abserror.valid=true;
            if(CheckTextValid(settinglink,ui->lineEdit_34->text())==-1)
             {

                 QMessageBox::critical(this,tr("Warning"),tr("comparative id of df/dt blocking is not valid!"),QMessageBox::Ok);
                 return;
             }
            ba=ui->lineEdit_34->text().toLatin1();
            copytoshuzu(freset.Tripdfdt.assessdfdttrip.CompareID,ba.data(),ba.size());
        }
        else  freset.Tripdfdt.assessdfdttrip.abserror.valid=false;
         freset.Tripdfdt.assessdfdttrip.assessandor=ui->comboBox_4->currentIndex()+1;

    }

    if((testitem&0x08)!=0)  //dfdt闭锁时间
    {
     if(ui->lineEdit->text().isEmpty())
     {
         QMessageBox::critical(this,tr("Warning"),tr("Dfdt of dtdt trip time is empty!"),QMessageBox::Ok);
         return;
     }

     if(ui->lineEdit_13->text().isEmpty())
     {
         QMessageBox::critical(this,tr("Warning"),tr("Stop frequency of dtdt trip time is empty!"),QMessageBox::Ok);
         return;
     }

     if(ui->lineEdit_44->text().isEmpty())
      {
        QMessageBox::critical(this,tr("Warning"),tr("Duration of dfdt trip time test is empty!"),QMessageBox::Ok);
        return;
      }

     if(CheckTextValid(settinglink,ui->lineEdit->text())==-1)
     {
         QMessageBox::critical(this,tr("Warning"),tr("Test dfdt of dfdt trip time test is empty!"),QMessageBox::Ok);
         return;
     }
     if(CheckTextValid(settinglink,ui->lineEdit_13->text())==-1)
     {
         QMessageBox::critical(this,tr("Warning"),tr("Stop freq of dfdt trip time test is empty!"),QMessageBox::Ok);
         return;
     }
     if(CheckTextValid(settinglink,ui->lineEdit_44->text())==-1)
     {
         QMessageBox::critical(this,tr("Warning"),tr("Hold time of dfdt trip time test is empty!"),QMessageBox::Ok);
         return;
     }


     ba=ui->lineEdit->text().toLatin1();
     copytoshuzu(freset.Tripdfdttime.dfdt.text,ba.data(),ba.size());
     ba=ui->lineEdit_13->text().toLatin1();
     copytoshuzu(freset.Tripdfdttime.stopfreq.text,ba.data(),ba.size());
     ba=ui->lineEdit_44->text().toLatin1();
     copytoshuzu(freset.Tripdfdttime.Duration.text,ba.data(),ba.size());





     if(ui->checkBox_9->isChecked())
     {
         if(ui->lineEdit_36->text().isEmpty())
         {
             QMessageBox::critical(this,tr("Warning"),tr("please input relative error of  dfdt trip time!"),QMessageBox::Ok);
             return;
         }
         bool ok;
         QString value=ui->lineEdit_36->text();
         value.toFloat(&ok);
         if(!ok)
         {
             QMessageBox::critical(this, "Warning", "Relative error of dfdt trip time is invalid!", QMessageBox::Yes);
             return;
         }
         freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue=value.toFloat(&ok);
         freset.Tripdfdttime.assessdfdttriptime.relerror.valid=true;

         if(CheckTextValid(settinglink,ui->lineEdit_22->text())==-1)
         {
             QMessageBox::critical(this,tr("Warning"),tr("Comparative id of dfdt trip time test is empty!"),QMessageBox::Ok);
             return;
         }

         ba=ui->lineEdit_22->text().toLatin1();
         copytoshuzu(freset.Tripdfdttime.assessdfdttriptime.CompareID,ba.data(),ba.size());


     }
     else freset.Tripdfdttime.assessdfdttriptime.relerror.valid=false;

     if(ui->checkBox_10->isChecked())
     {
         if(ui->lineEdit_37->text().isEmpty())
         {
             QMessageBox::critical(this,tr("Warning"),tr("Please input positive absolute error of dfdt trip time!"),QMessageBox::Ok);
             return;
         }

         if(ui->lineEdit_41->text().isEmpty())
         {
             QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of dfdt trip time!"),QMessageBox::Ok);
             return;
         }



         bool ok;
         QString value=ui->lineEdit_37->text();
         value.toFloat(&ok);
         if(!ok)
         {
             QMessageBox::critical(this, "Warning", "Positive Absolute error of dfdt trip time is invalid!", QMessageBox::Yes);
             return;
         }
         value=ui->lineEdit_41->text();
         value.toFloat(&ok);
         if(!ok)
         {
             QMessageBox::critical(this, "Warning", "Negative Absolute error of dfdt trip time is invalid!", QMessageBox::Yes);
             return;
         }

         value=ui->lineEdit_37->text();
         freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue=value.toFloat(&ok);
         value=ui->lineEdit_41->text();
         freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_2=value.toFloat(&ok);




         freset.Tripdfdttime.assessdfdttriptime.abserror.valid=true;

         if(CheckTextValid(settinglink,ui->lineEdit_22->text())==-1)
         {
             QMessageBox::critical(this,tr("Warning"),tr("Comparative id of dfdt trip time test is empty!"),QMessageBox::Ok);
             return;
         }


         ba=ui->lineEdit_22->text().toLatin1();
         copytoshuzu(freset.Tripdfdttime.assessdfdttriptime.CompareID,ba.data(),ba.size());
     }
     else  freset.Tripdfdttime.assessdfdttriptime.abserror.valid=false;
     freset.Tripdfdttime.assessdfdttriptime.assessandor=ui->comboBox_5->currentIndex()+1;

    }

    if((testitem&0x10)!=0) // 电压闭锁值
    {
        if(ui->lineEdit_23->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Dfdt of Voltage Blocking is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_24->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Frequency stop of Voltage Blocking is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_25->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Voltage start of Voltage Blocking is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_26->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Voltage End of Voltage Blocking is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_27->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Voltage Step of Voltage Blocking is empty!"),QMessageBox::Ok);
          return;
        }
        if(ui->lineEdit_28->text().isEmpty())
        {
          QMessageBox::critical(this,tr("Warning"),tr("Hold Time of Voltage Blocking is empty!"),QMessageBox::Ok);
          return;
        }


        if(CheckTextValid(settinglink,ui->lineEdit_23->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("dfdt  of Voltage block is empty!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_24->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Stop freq of Voltage block test is empty!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_25->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Voltage start of Voltage block test is empty!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_26->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Voltage end of Voltage block test is empty!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_27->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Voltage step of Voltage block test is empty!"),QMessageBox::Ok);
            return;
        }
        if(CheckTextValid(settinglink,ui->lineEdit_28->text())==-1)
        {
            QMessageBox::critical(this,tr("Warning"),tr("Hold time of Voltage block test is empty!"),QMessageBox::Ok);
            return;
        }





        ba=ui->lineEdit_23->text().toLatin1();
        copytoshuzu(freset.TripVBlock.Dfdt.text,ba.data(),ba.size());
        ba=ui->lineEdit_24->text().toLatin1();
        copytoshuzu(freset.TripVBlock.FreEndValue.text,ba.data(),ba.size());
        ba=ui->lineEdit_25->text().toLatin1();
        copytoshuzu(freset.TripVBlock.VStart.text,ba.data(),ba.size());
        ba=ui->lineEdit_26->text().toLatin1();
        copytoshuzu(freset.TripVBlock.VEnd.text,ba.data(),ba.size());
        ba=ui->lineEdit_27->text().toLatin1();
        copytoshuzu(freset.TripVBlock.VStep.text,ba.data(),ba.size());
        ba=ui->lineEdit_28->text().toLatin1();
        copytoshuzu(freset.TripVBlock.HoldTime.text,ba.data(),ba.size());

        freset.TripVBlock.VSelect=ui->comboBox_6->currentIndex()+1;
        if(ui->checkBox_5->isChecked())
        {
            if(ui->lineEdit_11->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("please input relative error of  voltage blocking!"),QMessageBox::Ok);
                return;
            }
            bool ok;
            QString value=ui->lineEdit_11->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Relative error of voltage blocking is invalid!", QMessageBox::Yes);
                return;
            }
            freset.TripVBlock.assessvblock.relerror.errorvalue=value.toFloat(&ok);
            freset.TripVBlock.assessvblock.relerror.valid=true;

            if(CheckTextValid(settinglink,ui->lineEdit_33->text())==-1)
            {
                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of Voltage block test is empty!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_33->text().toLatin1();
            copytoshuzu(freset.TripVBlock.assessvblock.CompareID,ba.data(),ba.size());

        }
        if(ui->checkBox_6->isChecked())
        {
            if(ui->lineEdit_12->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input positive absolute error of voltage blocking!"),QMessageBox::Ok);
                return;
            }

            if(ui->lineEdit_40->text().isEmpty())
            {
                QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of voltage blocking!"),QMessageBox::Ok);
                return;
            }





            bool ok;
            QString value=ui->lineEdit_12->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Positive Absolute error of voltage blocking is invalid!", QMessageBox::Yes);
                return;
            }

            value=ui->lineEdit_40->text();
            value.toFloat(&ok);
            if(!ok)
            {
                QMessageBox::critical(this, "Warning", "Negative Absolute error of voltage blocking is invalid!", QMessageBox::Yes);
                return;
            }




            value=ui->lineEdit_12->text();
            freset.TripVBlock.assessvblock.abserror.errorvalue=value.toFloat(&ok);
            value=ui->lineEdit_40->text();
            freset.TripVBlock.assessvblock.abserror.errorvalue_2=value.toFloat(&ok);

            freset.TripVBlock.assessvblock.abserror.valid=true;


            if(CheckTextValid(settinglink,ui->lineEdit_33->text())==-1)
            {
                QMessageBox::critical(this,tr("Warning"),tr("Comparative id of Voltage block test is empty!"),QMessageBox::Ok);
                return;
            }

            ba=ui->lineEdit_33->text().toLatin1();
            copytoshuzu(freset.TripVBlock.assessvblock.CompareID,ba.data(),ba.size());

        }
        else freset.TripVBlock.assessvblock.abserror.valid=false;
        freset.TripVBlock.assessvblock.assessandor=ui->comboBox_3->currentIndex()+1;
    }







    if((testitem&0x01)!=0)
    {
     freset.TripValue.valid=true;

    }
    if((testitem&0x02)!=0)
    {
       freset.TripTime.valid=true;

    }
    if((testitem&0x04)!=0)
    {
      freset.Tripdfdt.valid=true;

    }
    if((testitem&0x08)!=0)
    {
      freset.Tripdfdttime.valid=true;

    }
    if((testitem&0x10)!=0)
    {
       freset.TripVBlock.valid=true;
    }

    settingover=true;
    if(settingover)
   {
    emit sendsettingvalue((void *)&freset,4); //发送数据
   }

}

//退出
void Frequency::on_pushButton_2_clicked()
{
  // 关闭 并将值发送给主界面

   emit sendclose(4);
}



void Frequency::Setfrequencyset(FRECURRENCESET setvalue)
{
    ui->lineEdit_16->setText(QString(QLatin1String(setvalue.name)));
    ui->lineEdit_6->setText(QString(QLatin1String(setvalue.Faultduraion.text)));


    int testitem=atoi(setvalue.TestItem.text);
    if((testitem&0x01)&&setvalue.TripValue.valid==true)
    {
        ui->checkBox_11->setChecked(true);
        ui->lineEdit_2->setText(QString(QLatin1String(setvalue.TripValue.FreEndValueStart.text)));
        ui->lineEdit_3->setText(QString(QLatin1String(setvalue.TripValue.FreEndValueEnd.text)));
        ui->lineEdit_4->setText(QString(QLatin1String(setvalue.TripValue.FreStep.text)));
        ui->lineEdit_5->setText(QString(QLatin1String(setvalue.TripValue.HoldTime.text)));

        if(setvalue.TripValue.assesstripvalue.relerror.valid)
       { ui->lineEdit_7->setText(QString::number(setvalue.TripValue.assesstripvalue.relerror.errorvalue,'g',5));
         ui->checkBox->setChecked(setvalue.TripValue.assesstripvalue.relerror.valid);
         ui->lineEdit_32->setText(QString(QLatin1String(setvalue.TripValue.assesstripvalue.CompareID)));
       }
        if(setvalue.TripValue.assesstripvalue.abserror.valid)
      {  ui->lineEdit_8->setText(QString::number(setvalue.TripValue.assesstripvalue.abserror.errorvalue,'g',5));
         ui->lineEdit_29->setText(QString::number(setvalue.TripValue.assesstripvalue.abserror.errorvalue_2,'g',5));



         ui->checkBox_2->setChecked(setvalue.TripValue.assesstripvalue.abserror.valid);
         ui->lineEdit_32->setText(QString(QLatin1String(setvalue.TripValue.assesstripvalue.CompareID)));
      }
        ui->comboBox->setCurrentIndex(setvalue.TripValue.assesstripvalue.assessandor-1);
    }

    if((testitem&0x02)&&setvalue.TripTime.valid==true)
    {
       ui->checkBox_12->setChecked(true);
       ui->lineEdit_14->setText(QString(QLatin1String(setvalue.TripTime.FreEndValue.text)));
       ui->lineEdit_15->setText(QString(QLatin1String(setvalue.TripTime.HoldTime.text)));

       if(setvalue.TripTime.assesstriptime.relerror.valid)
      {  ui->lineEdit_9->setText(QString::number(setvalue.TripTime.assesstriptime.relerror.errorvalue,'g',5));
         ui->checkBox_3->setChecked(setvalue.TripTime.assesstriptime.relerror.valid);

       ui->lineEdit_35->setText(QString(QLatin1String(setvalue.TripTime.assesstriptime.CompareID)));
       }
       if(setvalue.TripTime.assesstriptime.abserror.valid)
       { ui->lineEdit_10->setText(QString::number(setvalue.TripTime.assesstriptime.abserror.errorvalue,'g',5));
         ui->lineEdit_38->setText(QString::number(setvalue.TripTime.assesstriptime.abserror.errorvalue_2,'g',5));


         ui->checkBox_4->setChecked(setvalue.TripTime.assesstriptime.abserror.valid);
         ui->lineEdit_35->setText(QString(QLatin1String(setvalue.TripTime.assesstriptime.CompareID)));
        }
        ui->comboBox_2->setCurrentIndex(setvalue.TripTime.assesstriptime.assessandor-1);
    }



    if((testitem&0x04)&&setvalue.Tripdfdt.valid==true)
     {
          ui->checkBox_13->setChecked(true);
         ui->lineEdit_18->setText(QString(QLatin1String(setvalue.Tripdfdt.FreEndValue.text)));
         ui->lineEdit_19->setText(QString(QLatin1String(setvalue.Tripdfdt.DfdtStart.text)));
         ui->lineEdit_20->setText(QString(QLatin1String(setvalue.Tripdfdt.DfdtEnd.text)));
         ui->lineEdit_21->setText(QString(QLatin1String(setvalue.Tripdfdt.DfdtStep.text)));


         if(setvalue.Tripdfdt.assessdfdttrip.relerror.valid)
        {  ui->lineEdit_30->setText(QString::number(setvalue.Tripdfdt.assessdfdttrip.relerror.errorvalue,'g',5));
           ui->checkBox_7->setChecked(setvalue.Tripdfdt.assessdfdttrip.relerror.valid);
           ui->lineEdit_34->setText(QString(QLatin1String(setvalue.Tripdfdt.assessdfdttrip.CompareID)));
        }
         if(setvalue.Tripdfdt.assessdfdttrip.abserror.valid)
         { ui->lineEdit_31->setText(QString::number(setvalue.Tripdfdt.assessdfdttrip.abserror.errorvalue,'g',5));
           ui->lineEdit_39->setText(QString::number(setvalue.Tripdfdt.assessdfdttrip.abserror.errorvalue_2,'g',5));



           ui->checkBox_8->setChecked(setvalue.Tripdfdt.assessdfdttrip.abserror.valid);
           ui->lineEdit_34->setText(QString(QLatin1String(setvalue.Tripdfdt.assessdfdttrip.CompareID)));
         }
          ui->comboBox_4->setCurrentIndex(setvalue.Tripdfdt.assessdfdttrip.assessandor-1);




     }

     if((testitem&0x08)&&setvalue.Tripdfdttime.valid==true)
     {
         ui->checkBox_14->setChecked(true);
         ui->lineEdit->setText(QString(QLatin1String(setvalue.Tripdfdttime.dfdt.text)));
         ui->lineEdit_13->setText(QString(QLatin1String(setvalue.Tripdfdttime.stopfreq.text)));
         ui->lineEdit_44->setText(setvalue.Tripdfdttime.Duration.text);

         if(setvalue.Tripdfdttime.assessdfdttriptime.relerror.valid==true)
         {
             ui->checkBox_9->setChecked(setvalue.Tripdfdttime.assessdfdttriptime.relerror.valid);
             ui->lineEdit_36->setText(QString::number(setvalue.Tripdfdttime.assessdfdttriptime.relerror.errorvalue));
             ui->lineEdit_22->setText(QString(setvalue.Tripdfdttime.assessdfdttriptime.CompareID));
         }
         if(setvalue.Tripdfdttime.assessdfdttriptime.abserror.valid==true)
         {
             ui->checkBox_10->setChecked(setvalue.Tripdfdttime.assessdfdttriptime.abserror.valid);
             ui->lineEdit_37->setText(QString::number(setvalue.Tripdfdttime.assessdfdttriptime.abserror.errorvalue));
             ui->lineEdit_41->setText(QString::number(setvalue.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_2));
             ui->lineEdit_22->setText(QString(setvalue.Tripdfdttime.assessdfdttriptime.CompareID));
         }
         ui->comboBox_5->setCurrentIndex(setvalue.Tripdfdttime.assessdfdttriptime.assessandor-1);
}







    if((testitem&0x10)&&setvalue.TripVBlock.valid==true)
    {
        ui->checkBox_15->setChecked(true);
        ui->lineEdit_23->setText(QString(QLatin1String(setvalue.TripVBlock.Dfdt.text)));
        ui->lineEdit_24->setText(QString(QLatin1String(setvalue.TripVBlock.FreEndValue.text)));
        ui->lineEdit_25->setText(QString(QLatin1String(setvalue.TripVBlock.VStart.text)));
        ui->lineEdit_26->setText(QString(QLatin1String(setvalue.TripVBlock.VEnd.text)));
        ui->lineEdit_27->setText(QString(QLatin1String(setvalue.TripVBlock.VStep.text)));
        ui->lineEdit_28->setText(QString(QLatin1String(setvalue.TripVBlock.HoldTime.text)));
        ui->comboBox_6->setCurrentIndex(setvalue.TripVBlock.VSelect-1);

        if(setvalue.TripVBlock.assessvblock.relerror.valid)
      {  ui->lineEdit_11->setText(QString::number(setvalue.TripVBlock.assessvblock.relerror.errorvalue,'g',5));
         ui->checkBox_5->setChecked(setvalue.TripVBlock.assessvblock.relerror.valid);
         ui->lineEdit_33->setText(QString(QLatin1String(setvalue.TripVBlock.assessvblock.CompareID)));
      }
        if(setvalue.TripVBlock.assessvblock.abserror.valid)
      {  ui->lineEdit_12->setText(QString::number(setvalue.TripVBlock.assessvblock.abserror.errorvalue,'g',5));
         ui->lineEdit_40->setText(QString::number(setvalue.TripVBlock.assessvblock.abserror.errorvalue_2,'g',5));



         ui->checkBox_6->setChecked(setvalue.TripVBlock.assessvblock.abserror.valid);
         ui->lineEdit_33->setText(QString(QLatin1String(setvalue.TripVBlock.assessvblock.CompareID)));
      }
        ui->comboBox_3->setCurrentIndex(setvalue.TripVBlock.assessvblock.assessandor-1);
    }



}
void Frequency::closeEvent(QCloseEvent *event)
{
  emit sendclose(4);
}






Frequency::~Frequency()
{
    delete ui;
}



