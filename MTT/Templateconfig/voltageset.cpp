#include "voltageset.h"
#include "ui_voltageset.h"
#include <QMessageBox>
#include <QDebug>

Voltageset::Voltageset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Voltageset)
{
    ui->setupUi(this);
     voltageset=new VOLTAGESET;
     settingover=false;
}

Voltageset::~Voltageset()
{
    delete ui;
}

void Voltageset::on_pushButton_clicked()
{


    QByteArray ba;
    QString str;
    bool ok;
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()||ui->lineEdit_4->text().isEmpty()\
    ||ui->lineEdit_5->text().isEmpty()||ui->lineEdit_6->text().isEmpty()||ui->lineEdit_7->text().isEmpty()||ui->lineEdit_8->text().isEmpty()\
    ||ui->lineEdit_9->text().isEmpty()||ui->lineEdit_10->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("The input box is empty, please input someting"),QMessageBox::Ok);
        return;
    }
    ba=ui->lineEdit->text().toLatin1();
    memcpy(voltageset->Vset.name,ba.data(),ba.size());
    ba=ui->lineEdit_2->text().toLatin1();
    memcpy(voltageset->Vset.ID,ba.data(),ba.size());
    str=ui->lineEdit_3->text();
    str.toFloat(&ok);
    if(!ok)
    {  QMessageBox::critical(NULL,"Warning",tr("The input \"value\" must be digital"));
        qDebug()<<__func__<<__LINE__;
       return ;
    }
    voltageset->Vset.value=ui->lineEdit_3->text().toFloat();

    //Tset
    ba=ui->lineEdit_4->text().toLatin1();
    memcpy(voltageset->Tset.name,ba.data(),ba.size());
    ba=ui->lineEdit_5->text().toLatin1();
    memcpy(voltageset->Tset.ID,ba.data(),ba.size());
    str=ui->lineEdit_6->text();
    str.toFloat(&ok);
    if(!ok)
    {  QMessageBox::critical(NULL,"Warning",tr("The input \"value\" must be digital"));

       return ;
    }
   voltageset->Tset.value=ui->lineEdit_6->text().toFloat();
    // testitems
    ba=ui->lineEdit_7->text().toLatin1();
    memcpy(voltageset->Testitem.name,ba.data(),ba.size());
    ba=ui->lineEdit_8->text().toLatin1();
    memcpy(voltageset->Testitem.ID,ba.data(),ba.size());
    voltageset->Testitem.value=(float)(ui->comboBox->currentIndex());
    //output selecttion
    ba=ui->lineEdit_9->text().toLatin1();
    memcpy(voltageset->faulttype.name,ba.data(),ba.size());
    ba=ui->lineEdit_10->text().toLatin1();
    memcpy(voltageset->faulttype.ID,ba.data(),ba.size());
    voltageset->faulttype.value=(float)(ui->comboBox_2->currentIndex());
    settingover=true;
}

void Voltageset::on_pushButton_2_clicked()
{
    if(settingover==true)
    {
        emit sendsettingvalue((void *)voltageset,2);
    }
    close();
}

void Voltageset::Setvoltageset(VOLTAGESET *voltset)
{
  voltageset=voltset;
  ui->lineEdit->setText(QString(QLatin1String(voltageset->Vset.name)));
  ui->lineEdit_2->setText(QString(QLatin1String(voltageset->Vset.ID)));
  ui->lineEdit_3->setText(QString::number(voltageset->Vset.value));

  ui->lineEdit_4->setText(QString(QLatin1String(voltageset->Tset.name)));
  ui->lineEdit_5->setText(QString(QLatin1String(voltageset->Tset.ID)));
  ui->lineEdit_6->setText(QString::number(voltageset->Tset.value));

  ui->lineEdit_7->setText(QString(QLatin1String(voltageset->Testitem.name)));
  ui->lineEdit_8->setText(QString(QLatin1String(voltageset->Testitem.ID)));
  ui->comboBox->setCurrentIndex((int)voltageset->Testitem.value);

  ui->lineEdit_9->setText(QString(QLatin1String(voltageset->faulttype.name)));
  ui->lineEdit_10->setText(QString(QLatin1String(voltageset->faulttype.ID)));
  ui->comboBox_2->setCurrentIndex((int)voltageset->faulttype.value);

}
