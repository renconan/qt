#include "tip.h"
#include "ui_tip.h"

Tip::Tip(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tip)
{
    ui->setupUi(this);
}


void Tip::settip(int index)
{
    if(index==1)
    {
        ui->label->setText(tr("Please restore setting time of protection"));
        ui->label_2->setText(tr("then press test button to start the test."));
    }
    else if(index==2)
    {
        ui->label->setText(tr("Please set setting time of protection to zero,"));
        ui->label_2->setText(tr("then press test button to start the test."));
    }


}

Tip::~Tip()
{
    delete ui;
}
