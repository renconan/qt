#include "warningdialog.h"
#include "ui_warningdialog.h"
#include <stdlib.h>
#include <stdio.h>
extern int current_language;  //1中文  2英文
Warningdialog::Warningdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Warningdialog)
{
    ui->setupUi(this);
    QFont font("wenquanyi",16);
    setFont(font);

}

Warningdialog::~Warningdialog()
{
    delete ui;
}

void Warningdialog::showwarning(int num,float SystemVolt)
{
    char temp1[400];
    char temp2[100];
    if(current_language==1)
    {
        if(num==1)
            sprintf(temp1,"故障电压超过系统电压,");
        else if(num==2)
            sprintf(temp1,"故障电压小于系统电压,");
        sprintf(temp2,"请改变电流的值.");
    }
    else
    {
        if(num==1)
            sprintf(temp1,"The voltage is more than %.3fV,",SystemVolt);
        else if(num==2)
            sprintf(temp1,"The voltage is else than %.3fV,",SystemVolt/2);
        sprintf(temp2,"please change the fixed current value.");
    }
    ui->label->setText(QString(QLatin1String(temp1)));
    ui->label_2->setText(QString(QLatin1String(temp2)));
}
