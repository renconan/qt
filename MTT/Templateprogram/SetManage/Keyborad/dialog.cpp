#include "dialog.h"
#include "ui_dialog.h"
#include "Defpam.h"
#include <stdlib.h>
#include <stdio.h>
#include <QFile>

#include <QTextCodec>

bool isfirst=false;


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    init();
    firstflag=false;
    ui->toolButton_0->setIcon(QIcon(":/picture/0.png"));
    ui->toolButton_1->setIcon(QIcon(":/picture/1.png"));
    ui->toolButton_2->setIcon(QIcon(":/picture/2.png"));
    ui->toolButton_3->setIcon(QIcon(":/picture/3.png"));
    ui->toolButton_4->setIcon(QIcon(":/picture/4.png"));
    ui->toolButton_5->setIcon(QIcon(":/picture/5.png"));
    ui->toolButton_6->setIcon(QIcon(":/picture/6.png"));
    ui->toolButton_7->setIcon(QIcon(":/picture/7.png"));
    ui->toolButton_8->setIcon(QIcon(":/picture/8.png"));
    ui->toolButton_9->setIcon(QIcon(":/picture/9.png"));
    ui->toolButton_back->setIcon(QIcon(":/picture/back.png"));
    ui->toolButton_sign->setIcon(QIcon(":/picture/sign.png"));
    ui->toolButton_point->setIcon(QIcon(":/picture/dot.png"));
    ui->toolButton_clear->setIcon(QIcon(":/picture/clear.png"));
    ui->toolButton_enter->setIcon(QIcon(":/picture/enter.png"));




    ui->toolButton_0->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_1->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_2->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_3->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_4->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_5->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_6->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_7->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_8->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_9->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_back->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_sign->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_point->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_clear->setFocusPolicy(Qt::NoFocus);
    ui->toolButton_enter->setFocusPolicy(Qt::NoFocus);





    setWindowTitle("Keyboard");
setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
}

void Dialog::setorignaltext(QString text)
{
    ui->display->setText(text);
    isfirst=true;
}


void Dialog::init()
{
this->setWindowIcon(QIcon("picture/20071208004047141.png"));

ui->display->setAlignment(Qt::AlignRight);
ui->display->setFont(QFont("wenquanyi",16,QFont::Bold));
this->setFixedSize(width(),height());
}
void Dialog::on_toolButton_0_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }

    int digitValue =0;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_1_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    int digitValue = 1;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_2_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    int digitValue = 2;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_3_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    int digitValue = 3;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_4_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    int digitValue = 4;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_5_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    int digitValue = 5;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_6_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    int digitValue = 6;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_7_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    int digitValue = 7;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_8_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    int digitValue = 8;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_9_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    int digitValue = 9;
    ui->display->setText(ui->display->text() + QString::number(digitValue));
}
void Dialog::on_toolButton_sign_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    QString text = ui->display->text();
    //double value = text.toDouble();
    if(text.length()>0)
    {
    if(text.at(0)!='-')
    text.prepend(tr("-"));
    else
    text.remove(0, 1);
    }
    else
    {
     text.prepend(tr("-"));
    }
     ui->display->setText(text);
}

void Dialog::on_toolButton_point_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    if (!ui->display->text().contains("."))
     ui->display->setText( ui->display->text() + tr("."));

}

void Dialog::on_toolButton_back_clicked()
{
    if(firstflag==false)
    {
        firstflag=true;
        ui->display->clear();
    }
    QString text = ui->display->text();
    text.chop(1);
    if (text.isEmpty()) {
        text = "0";
    }
      ui->display->setText(text);
}

void Dialog::on_toolButton_clear_clicked()
{

      firstflag=false;
      ui->display->setText("0");
}

void Dialog::on_toolButton_enter_clicked()
{
    QString text=  ui->display->text();
    double value = text.toDouble();
    emit sendvalue(QString::number(value));
    emit sendflag();//wk
    this->close();
}

void Dialog::closeEvent(QCloseEvent *event)
 {
     emit sendflag();
     this->close();

 }






Dialog::~Dialog()
{
    delete ui;
}
