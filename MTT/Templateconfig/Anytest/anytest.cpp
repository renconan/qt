#include "anytest.h"
#include "ui_anytest.h"
#include <QMessageBox>

Anytest::Anytest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Anytest)
{
    ui->setupUi(this);
    memset(&anytestset,0,sizeof(ANYTESTSET));
    setWindowTitle(QString("Anytest config"));
    setWindowIcon(QIcon(":/set.ico"));
        setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    settingover=false;
}

 void Anytest::setsectmentset(struct SECTMENTSET &set)
 {
   sectmentset=set;

 }

void Anytest::copytoshuzu(char shuzu[], QString str, int len)
{
    if(len>=512)
        return;
    for(int i=0;i<len;i++)
        shuzu[i]=str.at(i).toAscii();
    shuzu[len]='\0';
}

//需要更改
void Anytest::on_pushButton_clicked()
{
     QString str;
    if(ui->lineEdit_40->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("please input name!"),QMessageBox::Ok);
        return;
    }

    if(sectmentset.dirset.valid)
  {  if(QString(QLatin1String(sectmentset.dirset.name))==ui->lineEdit_40->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with direction protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.curset.valid)
  {  if(QString(QLatin1String(sectmentset.curset.name))==ui->lineEdit_40->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with current protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.volset.valid)
  {  if(QString(QLatin1String(sectmentset.volset.name))==ui->lineEdit_40->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with voltage protection!"),QMessageBox::Ok);
        return;
    }
  }
    if(sectmentset.freset.valid)
  {  if(QString(QLatin1String(sectmentset.freset.name))==ui->lineEdit_40->text())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Name is the same with frequency protection!"),QMessageBox::Ok);
        return;
    }
  }

    //请输入幅值的ID
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_4->text().isEmpty()||ui->lineEdit_7->text().isEmpty()||ui->lineEdit_10->text().isEmpty()||\
       ui->lineEdit_13->text().isEmpty()||ui->lineEdit_16->text().isEmpty()||ui->lineEdit_19->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Amplitude is empty!"),QMessageBox::Ok);
        return;
    }

    if(ui->lineEdit_2->text().isEmpty()||ui->lineEdit_5->text().isEmpty()||ui->lineEdit_8->text().isEmpty()||ui->lineEdit_11->text().isEmpty()||\
       ui->lineEdit_14->text().isEmpty()||ui->lineEdit_17->text().isEmpty()||ui->lineEdit_20->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("Phrase is emtpy!"),QMessageBox::Ok);
        return;
    }
    //请输入频率的ID
    if(ui->lineEdit_3->text().isEmpty()||ui->lineEdit_6->text().isEmpty()||ui->lineEdit_9->text().isEmpty()||ui->lineEdit_12->text().isEmpty()||\
       ui->lineEdit_15->text().isEmpty()||ui->lineEdit_18->text().isEmpty()||ui->lineEdit_21->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("frequency is empty!"),QMessageBox::Ok);
        return;
    }

    if(ui->lineEdit_22->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("current step is empty!"),QMessageBox::Ok);
        return;
    }
    if(ui->lineEdit_23->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("voltage step is empty!"),QMessageBox::Ok);
        return;
    }
    if(ui->lineEdit_24->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("phrase step is empty!"),QMessageBox::Ok);
        return;
    }
    if(ui->lineEdit_25->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("frequency step is is empty!"),QMessageBox::Ok);
        return;
    }
    if(ui->lineEdit_26->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("step time is empty!"),QMessageBox::Ok);
        return;
    }
    if(ui->lineEdit_27->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("step number is empty!"),QMessageBox::Ok);
        return;
    }

    if(ui->lineEdit_31->text().isEmpty()||ui->lineEdit_32->text().isEmpty()||ui->lineEdit_33->text().isEmpty()||ui->lineEdit_34->text().isEmpty()||\
       ui->lineEdit_35->text().isEmpty()||ui->lineEdit_36->text().isEmpty()||ui->lineEdit_37->text().isEmpty()||ui->lineEdit_3U0->text().isEmpty())
   {
        QMessageBox::critical(this,tr("Warning"),tr("variable is empty!"),QMessageBox::Ok);
        return;
   }
   if(ui->lineEdit_28->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("pre fault time is empty!"),QMessageBox::Ok);
        return;
    }
   if(ui->lineEdit_29->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("fault time is empty!"),QMessageBox::Ok);
        return;
    }
   if(ui->lineEdit_30->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("post fault time is empty!"),QMessageBox::Ok);
        return;
    }

   if(ui->lineEdit_41->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Warning"),tr("fault duration is empty!"),QMessageBox::Ok);
        return;
    }




   if(ui->lineEdit_38->text().isEmpty())
   {
       QMessageBox::critical(this,tr("Warning"),tr("test mode is empty!"),QMessageBox::Ok);
       return;
   }
   if(ui->lineEdit_39->text().isEmpty())
   {
       QMessageBox::critical(this,tr("Warning"),tr("Change is empty!"),QMessageBox::Ok);
       return;
   }



   //VA
    QByteArray ba;

    ba=ui->lineEdit_40->text().toLatin1();
    copytoshuzu(anytestset.name,ba.data(),ba.size());


    ba=ui->lineEdit->text().toLatin1();
    copytoshuzu(anytestset.VAAP.text,ba.data(),ba.size());
    ba=ui->lineEdit_2->text().toLatin1();
    copytoshuzu(anytestset.VAPH.text,ba.data(),ba.size());
    ba=ui->lineEdit_3->text().toLatin1();
    copytoshuzu(anytestset.VAFR.text,ba.data(),ba.size());
    //VB
     ba=ui->lineEdit_4->text().toLatin1();
     copytoshuzu(anytestset.VBAP.text,ba.data(),ba.size());
     ba=ui->lineEdit_5->text().toLatin1();
     copytoshuzu(anytestset.VBPH.text,ba.data(),ba.size());
     ba=ui->lineEdit_6->text().toLatin1();
     copytoshuzu(anytestset.VBFR.text,ba.data(),ba.size());
     //VC
      ba=ui->lineEdit_7->text().toLatin1();
      copytoshuzu(anytestset.VCAP.text,ba.data(),ba.size());
      ba=ui->lineEdit_8->text().toLatin1();
      copytoshuzu(anytestset.VCPH.text,ba.data(),ba.size());
      ba=ui->lineEdit_9->text().toLatin1();
      copytoshuzu(anytestset.VCFR.text,ba.data(),ba.size());
      //VZ
       ba=ui->lineEdit_10->text().toLatin1();
       copytoshuzu(anytestset.VZAP.text,ba.data(),ba.size());
       ba=ui->lineEdit_11->text().toLatin1();
       copytoshuzu(anytestset.VZPH.text,ba.data(),ba.size());
       ba=ui->lineEdit_12->text().toLatin1();
       copytoshuzu(anytestset.VZFR.text,ba.data(),ba.size());
       //IA
        ba=ui->lineEdit_13->text().toLatin1();
        copytoshuzu(anytestset.IAAP.text,ba.data(),ba.size());
        ba=ui->lineEdit_14->text().toLatin1();
        copytoshuzu(anytestset.IAPH.text,ba.data(),ba.size());
        ba=ui->lineEdit_15->text().toLatin1();
        copytoshuzu(anytestset.IAFR.text,ba.data(),ba.size());
        //IB
         ba=ui->lineEdit_16->text().toLatin1();
         copytoshuzu(anytestset.IBAP.text,ba.data(),ba.size());
         ba=ui->lineEdit_17->text().toLatin1();
         copytoshuzu(anytestset.IBPH.text,ba.data(),ba.size());
         ba=ui->lineEdit_18->text().toLatin1();
         copytoshuzu(anytestset.IBFR.text,ba.data(),ba.size());
         //IC
          ba=ui->lineEdit_19->text().toLatin1();
          copytoshuzu(anytestset.ICAP.text,ba.data(),ba.size());
          ba=ui->lineEdit_20->text().toLatin1();
          copytoshuzu(anytestset.ICPH.text,ba.data(),ba.size());
          ba=ui->lineEdit_21->text().toLatin1();
          copytoshuzu(anytestset.ICFR.text,ba.data(),ba.size());
          //步长
          ba=ui->lineEdit_22->text().toLatin1();
          copytoshuzu(anytestset.Istep.text,ba.data(),ba.size());
          ba=ui->lineEdit_23->text().toLatin1();
          copytoshuzu(anytestset.Vstep.text,ba.data(),ba.size());
          ba=ui->lineEdit_24->text().toLatin1();
          copytoshuzu(anytestset.Pstep.text,ba.data(),ba.size());
          ba=ui->lineEdit_25->text().toLatin1();
          copytoshuzu(anytestset.Fstep.text,ba.data(),ba.size());
          ba=ui->lineEdit_26->text().toLatin1();
          copytoshuzu(anytestset.Steptime.text,ba.data(),ba.size());
          ba=ui->lineEdit_27->text().toLatin1();
          copytoshuzu(anytestset.Stepnum.text,ba.data(),ba.size());
          //变化相
          ba=ui->lineEdit_31->text().toLatin1();
          copytoshuzu(anytestset.VariatyVA.text,ba.data(),ba.size());
          ba=ui->lineEdit_32->text().toLatin1();
          copytoshuzu(anytestset.VariatyVB.text,ba.data(),ba.size());
          ba=ui->lineEdit_33->text().toLatin1();
          copytoshuzu(anytestset.VariatyVC.text,ba.data(),ba.size());
          ba=ui->lineEdit_34->text().toLatin1();
          copytoshuzu(anytestset.VariatyVZ.text,ba.data(),ba.size());
          ba=ui->lineEdit_3U0->text().toLatin1();
          copytoshuzu(anytestset.VZ3U0.text,ba.data(),ba.size());
          ba=ui->lineEdit_35->text().toLatin1();
          copytoshuzu(anytestset.VariatyIA.text,ba.data(),ba.size());
          ba=ui->lineEdit_36->text().toLatin1();
          copytoshuzu(anytestset.VariatyIB.text,ba.data(),ba.size());
          ba=ui->lineEdit_37->text().toLatin1();
          copytoshuzu(anytestset.VariatyIC.text,ba.data(),ba.size());

          //故障时间
          ba=ui->lineEdit_28->text().toLatin1();
          copytoshuzu(anytestset.Prefault.text,ba.data(),ba.size());
          ba=ui->lineEdit_29->text().toLatin1();
          copytoshuzu(anytestset.Infault.text,ba.data(),ba.size());
          ba=ui->lineEdit_30->text().toLatin1();
          copytoshuzu(anytestset.Postfault.text,ba.data(),ba.size());

          ba=ui->lineEdit_41->text().toLatin1();
          copytoshuzu(anytestset.Faultduraion.text,ba.data(),ba.size());

          //测试模式
          ba=ui->lineEdit_38->text().toLatin1();
          copytoshuzu(anytestset.Testmode.text,ba.data(),ba.size());
          ba=ui->lineEdit_39->text().toLatin1();
          copytoshuzu(anytestset.ChangeItem.text,ba.data(),ba.size());
          settingover=true;
}


//退出
void Anytest::on_pushButton_2_clicked()
{
  // 关闭 并将值发送给主界面
    if(settingover)
   {
     emit sendsettingvalue((void *)&anytestset,5); //发送数据
   }
  else  emit sendclose(5);
}



void Anytest::Setanytestset(ANYTESTSET setvalue)
{


    ui->lineEdit_40->setText(QString(QLatin1String(setvalue.name)));
    ui->lineEdit->setText(QString(QLatin1String(setvalue.VAAP.text)));
    ui->lineEdit_2->setText(QString(QLatin1String(setvalue.VAPH.text)));
    ui->lineEdit_3->setText(QString(QLatin1String(setvalue.VAFR.text)));
    ui->lineEdit_4->setText(QString(QLatin1String(setvalue.VBAP.text)));
    ui->lineEdit_5->setText(QString(QLatin1String(setvalue.VBPH.text)));
    ui->lineEdit_6->setText(QString(QLatin1String(setvalue.VBFR.text)));
    ui->lineEdit_7->setText(QString(QLatin1String(setvalue.VCAP.text)));
    ui->lineEdit_8->setText(QString(QLatin1String(setvalue.VCPH.text)));
    ui->lineEdit_9->setText(QString(QLatin1String(setvalue.VCFR.text)));
    ui->lineEdit_10->setText(QString(QLatin1String(setvalue.VZAP.text)));
    ui->lineEdit_11->setText(QString(QLatin1String(setvalue.VZPH.text)));
    ui->lineEdit_12->setText(QString(QLatin1String(setvalue.VZFR.text)));
    ui->lineEdit_13->setText(QString(QLatin1String(setvalue.IAAP.text)));
    ui->lineEdit_14->setText(QString(QLatin1String(setvalue.IAPH.text)));
    ui->lineEdit_15->setText(QString(QLatin1String(setvalue.IAFR.text)));
    ui->lineEdit_16->setText(QString(QLatin1String(setvalue.IBAP.text)));
    ui->lineEdit_17->setText(QString(QLatin1String(setvalue.IBPH.text)));
    ui->lineEdit_18->setText(QString(QLatin1String(setvalue.IBFR.text)));
    ui->lineEdit_19->setText(QString(QLatin1String(setvalue.ICAP.text)));
    ui->lineEdit_20->setText(QString(QLatin1String(setvalue.ICPH.text)));
    ui->lineEdit_21->setText(QString(QLatin1String(setvalue.ICFR.text)));

    ui->lineEdit_22->setText(QString(QLatin1String(setvalue.Istep.text)));
    ui->lineEdit_23->setText(QString(QLatin1String(setvalue.Vstep.text)));
    ui->lineEdit_24->setText(QString(QLatin1String(setvalue.Pstep.text)));
    ui->lineEdit_25->setText(QString(QLatin1String(setvalue.Fstep.text)));
    ui->lineEdit_26->setText(QString(QLatin1String(setvalue.Steptime.text)));
    ui->lineEdit_27->setText(QString(QLatin1String(setvalue.Stepnum.text)));
    //变化项
     ui->lineEdit_31->setText(QString(QLatin1String(setvalue.VariatyVA.text)));
     ui->lineEdit_32->setText(QString(QLatin1String(setvalue.VariatyVB.text)));
     ui->lineEdit_33->setText(QString(QLatin1String(setvalue.VariatyVC.text)));
     ui->lineEdit_34->setText(QString(QLatin1String(setvalue.VariatyVZ.text)));
     ui->lineEdit_3U0->setText(QString(QLatin1String(setvalue.VZ3U0.text)));
     ui->lineEdit_35->setText(QString(QLatin1String(setvalue.VariatyIA.text)));
     ui->lineEdit_36->setText(QString(QLatin1String(setvalue.VariatyIB.text)));
     ui->lineEdit_37->setText(QString(QLatin1String(setvalue.VariatyIC.text)));

     ui->lineEdit_28->setText(QString(QLatin1String(setvalue.Prefault.text)));
     ui->lineEdit_29->setText(QString(QLatin1String(setvalue.Infault.text)));
     ui->lineEdit_30->setText(QString(QLatin1String(setvalue.Postfault.text)));
     ui->lineEdit_41->setText(QString(QLatin1String(setvalue.Faultduraion.text)));

     ui->lineEdit_38->setText(QString(QLatin1String(setvalue.Testmode.text)));
     ui->lineEdit_39->setText(QString(QLatin1String(setvalue.ChangeItem.text)));
}


void Anytest::closeEvent(QCloseEvent *event)
{
    emit sendclose(5);
}




Anytest::~Anytest()
{
    delete ui;
}
