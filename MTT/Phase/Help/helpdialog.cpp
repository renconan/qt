#include "helpdialog.h"
#include "ui_helpdialog.h"
#include <QFile>

Helpdialog::Helpdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Helpdialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    QFont font("wenquanyi",13);
    setFont(font);
        showFullScreen();
         ui->pushButton->setFocusPolicy(Qt::NoFocus);
}

Helpdialog::~Helpdialog()
{
    delete ui;
}

void Helpdialog::SetHelpfilename(char *filename)
{
    char Helpfilename[512];
    sprintf(Helpfilename,"./HELPFILEDIR/%s",filename);
    QFile file(Helpfilename);
    if(file.exists()==false)
        return;

        FILE *fp=NULL;
        fp=fopen(Helpfilename,"r");
        if(fp==NULL)
        {
        return;
        }
        char buffer[1024];

        while(fgets(buffer,1024,fp)!=NULL)
        {

            char buffer2[2048];
            char *p2=buffer;
            int length=0;
            while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
            {
                  buffer2[length]=*p2;
                  p2++;
                  length++;
            }
               buffer2[length]='\0';
            ui->textEdit->append(QString(buffer2));
        }
        fclose(fp);
}

void Helpdialog::on_pushButton_clicked()
{
    emit HelpWindowClose();
    close();
}
