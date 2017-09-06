#include "addsetting.h"
#include "ui_addsetting.h"
#include  <QMessageBox>
#include <QDebug>
#include <QTime>

Addsetting::Addsetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Addsetting)
{
    ui->setupUi(this);
    setWindowTitle(QString("Add setting"));
    setWindowIcon(QIcon(":/set.ico"));
        setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
}





Addsetting::~Addsetting()
{
    delete ui;
}

int Addsetting::is_digit(char ch)
{
    if(ch>='0'&&ch<='9')
        return 1;
    else
        return 0;
}
int Addsetting::is_space( char ch)
{
    if(ch==' ')
        return 1;
    else
        return 0;
}


bool Addsetting::checkvalue(QString str)
{
    char *ch;
    QByteArray ba=str.toLatin1();
    ch=ba.data();
    char *c=ch;
    if(ch==NULL)
        return false;

    while(*c!='\0')
    {
        if(isdigit(*c)||(*c=='.'&&isdigit(*ch)))
        c++;
       else return false;
    }
    return true;
}


bool Addsetting::checkid(QString str)
{


    char *ch;
    QByteArray ba=str.toLatin1();
    ch=ba.data();
    if(ch==NULL)
        return false;
    if(is_digit(*ch))  //id不能以数字开头
        return false;
    char *c=ch;
    while(*c!='\0')
    {
        if((*c>='1'&&*c<='9')&&(*c>='a'&&*c<='z')&&(*c>='A'&&*c<='Z'))
           { c++;
            continue;
           }
        c++;
    }
    if(*c=='\0')
         return true;
    else return false;
}



void delaymsec(int msec)
{
    QTime n=QTime::currentTime();

    QTime now;
    do{
      now=QTime::currentTime();
    }while (n.msecsTo(now)<=msec);

}




void Addsetting::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty())
     { QMessageBox::information(this, "warning", "Please input name!", QMessageBox::Yes);
      return;
     }
    if(ui->lineEdit_2->text().isEmpty())
     { QMessageBox::information(this, "warning", "Please input value!", QMessageBox::Yes);
      return;
     }
    if(ui->lineEdit_3->text().isEmpty())
     { QMessageBox::information(this, "warning", "Please input ID!", QMessageBox::Yes);
      return;
     }
    /*
    if(ui->lineEdit_4->text().isEmpty())
     { QMessageBox::information(this, "warning", "Please input reference!", QMessageBox::Yes);
      return;
     }
     */

    bool ok;
    QString str=ui->lineEdit_2->text();
    str.toFloat(&ok);
    if(!ok)
     {  QMessageBox::information(this, "warning", "value is invalid!", QMessageBox::Yes);
       return;
     }

    if(!checkid(ui->lineEdit_3->text()))
      {  QMessageBox::information(this, "warning", "ID is invalid!", QMessageBox::Yes);
        return;
      }

    QString name=ui->lineEdit->text();
    QString value=ui->lineEdit_2->text();
    QString ID=ui->lineEdit_3->text();


  //  QString ref=ui->lineEdit_4->text();
  //  qDebug()<<__func__<<__LINE__<<ref;
    emit SIG_sendsetting(name,value,ID);

    delaymsec(50);
    close(); //关闭退出

}


void Addsetting::setsetting(QString name,QString value,QString ID)
{
    ui->lineEdit->setText(name);
    ui->lineEdit_2->setText(value);
    ui->lineEdit_3->setText(ID);
  //  ui->lineEdit_4->setText(ref);

}


void Addsetting::on_pushButton_2_clicked()
{
    close();
}
