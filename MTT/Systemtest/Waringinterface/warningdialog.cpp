#include "warningdialog.h"
#include "ui_warningdialog.h"
#include <stdlib.h>
#include <stdio.h>
extern int current_language;  //1����  2Ӣ��
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
            sprintf(temp1,"���ϵ�ѹ����ϵͳ��ѹ,");
        else if(num==2)
            sprintf(temp1,"���ϵ�ѹС��ϵͳ��ѹ,");
        sprintf(temp2,"��ı������ֵ.");
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
