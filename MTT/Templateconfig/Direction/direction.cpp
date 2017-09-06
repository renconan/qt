#include "direction.h"
#include "ui_direction.h"
#include  <QMessageBox>
#include "caculate.h"

Direction::Direction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Direction)
{
    ui->setupUi(this);
    settingover=false;
    setWindowTitle(QString("Direction configure"));
    setWindowIcon(QIcon(":/set.ico"));
        setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    memset(&directionset,0,sizeof(DIRECTIONSET));
    setFixedSize(this->width(),this->height());
}

void Direction::setsectmentset(struct SECTMENTSET &set)
{
  sectmentset=set;

}


void Direction::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}



//保存
void Direction::on_pushButton_clicked()
{

    QByteArray ba;
    if(ui->lineEdit_9->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input name!"),QMessageBox::Ok);
        return;
    }


    if(sectmentset.volset.valid)
  {  if(QString(QLatin1String(sectmentset.volset.name))==ui->lineEdit_9->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with voltage protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.curset.valid)
  {  if(QString(QLatin1String(sectmentset.curset.name))==ui->lineEdit_9->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with current protection!"),QMessageBox::Ok);
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


    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Fault current  is empty!"),QMessageBox::Ok);
        return;
    }


    if(ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Fault voltage  is empty !"),QMessageBox::Ok);
        return;
    }

    if(ui->lineEdit_3->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Mta is empty !"),QMessageBox::Ok);
        return;
    }



    if(ui->lineEdit_10->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Step value is empty!"),QMessageBox::Ok);
        return;
    }
    if(ui->lineEdit_11->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Step time is empty!"),QMessageBox::Ok);
        return;
    }


    if(CheckTextValid(settinglink,ui->lineEdit->text())==-1)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Current  is not valid!"),QMessageBox::Ok);
        return;
    }
    if(CheckTextValid(settinglink,ui->lineEdit_2->text())==-1)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Voltage  is not valid!"),QMessageBox::Ok);
        return;
    }
    if(CheckTextValid(settinglink,ui->lineEdit_3->text())==-1)
    {
        QMessageBox::warning(this,tr("Warning"),tr("MTA  is not valid!"),QMessageBox::Ok);
        return;
    }

    if(CheckTextValid(settinglink,ui->lineEdit_10->text())==-1)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Step angel  is not valid!"),QMessageBox::Ok);
        return;
    }
    if(CheckTextValid(settinglink,ui->lineEdit_11->text())==-1)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Step time  is not valid!"),QMessageBox::Ok);
        return;
    }




    //name
    ba=ui->lineEdit_9->text().toLatin1();
    copytoshuzu(directionset.name,ba.data(),ba.size());
    ba=ui->lineEdit->text().toLatin1();
    copytoshuzu(directionset.TripMta.Ifault.text,ba.data(),ba.size());
    ba=ui->lineEdit_2->text().toLatin1();
    copytoshuzu(directionset.TripMta.vfault.text,ba.data(),ba.size());
    ba=ui->lineEdit_3->text().toLatin1();
    copytoshuzu(directionset.TripMta.mta.text,ba.data(),ba.size());




    itoa(ui->comboBox_2->currentIndex()+1,directionset.TripMta.faulttype.text,10);

    ba=ui->lineEdit_10->text().toLatin1();
    copytoshuzu(directionset.TripMta.step.text,ba.data(),ba.size());

    ba=ui->lineEdit_11->text().toLatin1();
    copytoshuzu(directionset.TripMta.steptime.text,ba.data(),ba.size());




    if(ui->checkBox->isChecked())
    {
        if(ui->lineEdit_5->text().isEmpty())
        {
            QMessageBox::critical(this,tr("Warning"),tr("please input relative error of mta!"),QMessageBox::Ok);
            return;
        }
        bool ok;
        QString value=ui->lineEdit_5->text();
        value.toFloat(&ok);
        if(!ok)
        {
            QMessageBox::critical(this, "Warning", "Relative error of mta is invalid!", QMessageBox::Yes);
            return;
        }
        directionset.TripMta.assessmta.relerror.errorvalue=value.toFloat(&ok);
        directionset.TripMta.assessmta.relerror.valid=true;

        if(CheckTextValid(settinglink,ui->lineEdit_7->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Comparative id  is not valid!"),QMessageBox::Ok);
            return;
        }


        ba=ui->lineEdit_7->text().toLatin1();
        copytoshuzu(directionset.TripMta.assessmta.CompareID,ba.data(),ba.size());



    }
    else directionset.TripMta.assessmta.relerror.valid=false;

    if(ui->checkBox_2->isChecked())
    {
        if(ui->lineEdit_6->text().isEmpty())
        {
            QMessageBox::critical(this,tr("Warning"),tr("Please input positive absolute error of mta!"),QMessageBox::Ok);
            return;
        }

        if(ui->lineEdit_8->text().isEmpty())
        {
            QMessageBox::critical(this,tr("Warning"),tr("Please input negative absolute error of mta!"),QMessageBox::Ok);
            return;
        }


        bool ok;
        QString value=ui->lineEdit_6->text();
        value.toFloat(&ok);
        if(!ok)
        {
            QMessageBox::critical(this, "Warning", "Positive Absolute error of mta is invalid!", QMessageBox::Yes);
            return;
        }

        value=ui->lineEdit_8->text();
        value.toFloat(&ok);
        if(!ok)
        {
            QMessageBox::critical(this, "Warning", "Negative Absolute error of mta is invalid!", QMessageBox::Yes);
            return;
        }


        value=ui->lineEdit_6->text();
        directionset.TripMta.assessmta.abserror.errorvalue=value.toFloat(&ok);
        value=ui->lineEdit_8->text();
        directionset.TripMta.assessmta.abserror.errorvalue_2=value.toFloat(&ok);


        directionset.TripMta.assessmta.abserror.valid=true;
        if(CheckTextValid(settinglink,ui->lineEdit_7->text())==-1)
        {
            QMessageBox::warning(this,tr("Warning"),tr("Comparative id  is not valid!"),QMessageBox::Ok);
            return;
        }
        ba=ui->lineEdit_7->text().toLatin1();
        copytoshuzu(directionset.TripMta.assessmta.CompareID,ba.data(),ba.size());
    }
    else directionset.TripMta.assessmta.abserror.valid=false;
    directionset.TripMta.assessmta.assessandor=ui->comboBox->currentIndex()+1;

    directionset.TripMta.valid=true;

    settingover=true;
    if(settingover)
   { emit sendsettingvalue((void *)&directionset,3); //发送数据
   }

}


//退出
void Direction::on_pushButton_2_clicked()
{


  emit sendclose(3);
}



void Direction::SetDirectionset(DIRECTIONSET dirset)
{

    ui->lineEdit_9->setText(QString(QLatin1String(dirset.name)));
    ui->lineEdit->setText(QString(QLatin1String(dirset.TripMta.Ifault.text)));
    ui->lineEdit_2->setText(QString(QLatin1String(dirset.TripMta.vfault.text)));
    ui->lineEdit_3->setText(QString(QLatin1String(dirset.TripMta.mta.text)));


    int faulttype=atoi(dirset.TripMta.faulttype.text);
    ui->comboBox_2->setCurrentIndex(faulttype-1);

    ui->lineEdit_10->setText(QString(QLatin1String(dirset.TripMta.step.text)));
    ui->lineEdit_11->setText(QString(QLatin1String(dirset.TripMta.steptime.text)));






    if(dirset.TripMta.valid)
 {
    if(dirset.TripMta.assessmta.relerror.valid)
    { ui->lineEdit_5->setText(QString::number(dirset.TripMta.assessmta.relerror.errorvalue,'g',3));
    ui->checkBox->setChecked(dirset.TripMta.assessmta.relerror.valid);
    ui->lineEdit_7->setText(dirset.TripMta.assessmta.CompareID);
    }
    if(dirset.TripMta.assessmta.abserror.valid)
    { ui->lineEdit_6->setText(QString::number(dirset.TripMta.assessmta.abserror.errorvalue,'g',3));
      ui->lineEdit_8->setText(QString::number(dirset.TripMta.assessmta.abserror.errorvalue_2,'g',3));



        ui->checkBox_2->setChecked(dirset.TripMta.assessmta.abserror.valid);
      ui->lineEdit_7->setText(dirset.TripMta.assessmta.CompareID);
    }
    ui->comboBox->setCurrentIndex(dirset.TripMta.assessmta.assessandor-1);


}


}


void Direction::closeEvent(QCloseEvent *event)
{
  emit sendclose(3);
}

Direction::~Direction()
{
    delete ui;
}
