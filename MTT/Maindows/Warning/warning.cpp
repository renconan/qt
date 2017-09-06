#include "warning.h"
#include "ui_warning.h"
#include <QTextCodec>
extern int current_language;  //1中文  2英文
Warning::Warning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Warning)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);


    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
}

void Warning::Setwarning(int type,QString smg)
{
    if(current_language==2)
    {
     if(type==1)
       {
        ui->label_2->setText(QString("Please re set the step size and step number"));
        ui->label->setText(smg);
       }
     else
       {
        ui->label->setText(QString("Please set the VA VB VC frequency to the system frequency,\n and then select Vz=3U0"));
        ui->label_2->setText("");
       }
    }
    else if(current_language==1)
    {
      if(type==1)
       {
         ui->label_2->setText(QString("请重新设置步长和步数"));
         ui->label->setText(smg);
       }
      else
       {
         ui->label->setText(QString("请将VA VB VC频率设置为系统频率，然后再选择Vz=3U0"));
         ui->label_2->setText("");
       }
    }
}

Warning::~Warning()
{
    delete ui;
}

void Warning::on_pushButton_clicked()
{
emit WarningWindowClose();
this->close();
}

void Warning::closeEvent(QCloseEvent *event)
 {
    emit WarningWindowClose();
     this->close();
 }
