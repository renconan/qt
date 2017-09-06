#include "addharmonic.h"
#include "ui_addharmonic.h"

AddHarmonic::AddHarmonic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddHarmonic)
{
    ui->setupUi(this);
}

AddHarmonic::~AddHarmonic()
{
    delete ui;
}
