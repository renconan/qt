#include "exportreport.h"
#include "ui_exportreport.h"
#include <QFile>
#include <QTime>
#include <QTextCodec>
#include <QMessageBox>
#define DIR1 "/mnt/usb1"
#define DIR2 "/mnt/usb2"
#define DIR3 "/mnt/usb3"
#define DIR4 "/mnt/usb4"


#include <QDir>

float DIR1_TOTAL=0;
float DIR1_USED=0;
float DIR1_FREE=0;
int   Mode=1;
int CRCCODE=0;
char *basicprogram[18]={
    "Manual","Templateprogram","mainconfig.cfg",\
    "Middle","Phase","Time",
    "Power","Voltage",\
    "Differ","Sysconfig","zone",\
    "Harmonic","Systemtest","dizhou",\
    "Maindows","fanshixian","Sin","Zero"
};

char *Templateprogram[6]=
{
    "TemplateCurrent","TemplatePhase","TemplateVoltage","TemplatePower","Templatedizhou","TemplateHarmonic"
};






ExportReport::ExportReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportReport)
{
    ui->setupUi(this);
    setWindowTitle(QString(" "));
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setWindowIcon(QIcon(" "));
    setFont(font);
       if(Mode==1)
       {
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
       }
       else if(Mode==2)
       {
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(true);
        ui->radioButton_3->setChecked(false);
       }
       else
       {
           ui->radioButton->setChecked(false);
           ui->radioButton_2->setChecked(false);
           ui->radioButton_3->setChecked(true);
       }

    ui->label->setAlignment(Qt::AlignCenter);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    setContextMenuPolicy(Qt::NoContextMenu);
    setWindowIcon(QIcon(""));
    setFixedSize(435,300);

ui->label_2->hide();
}

ExportReport::~ExportReport()
{
    delete ui;
}


int ExportReport::TestSpace(char *dir)
{

        FILE *file;
        char buf[1024];
        char cmd[128];
        memset(cmd,0,128);
        sprintf(cmd,"df -m|grep %s |awk '{print $1,$2,$3,$4,$5}' ",dir);
        file=popen(cmd,"r");
        if(file)
        {
        memset(buf,0,sizeof(buf));
        fgets(buf,sizeof(buf),file);
        pclose(file);
        }
        else
        {
        printf("file==NULL\n");
        return -1;
        }
        if(strlen(buf)<5)//没有这个目录
        return -1;
        char temp[512];
        float v1=0,v2=0,v3=0;
        sscanf(buf,"%s %f  %f  %f ",temp,&v1,&v2,&v3);
        if(v1>0)
        return 0;

        return -1;

}

void usersleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void ExportReport::on_pushButton_clicked()
{
char Usbdir[512];
        int UsbdiskState=0;
        UsbdiskState+=(TestSpace(DIR1)==-1?0:1);
        UsbdiskState+=(TestSpace(DIR2)==-1?0:2);
        UsbdiskState+=(TestSpace(DIR3)==-1?0:4);
        UsbdiskState+=(TestSpace(DIR4)==-1?0:8);
        if(UsbdiskState==0&&(Mode==1||Mode==2))//2016-5-20 chen
             {
                sleep(1);
                UsbdiskState+=(TestSpace(DIR1)==-1?0:1);
                UsbdiskState+=(TestSpace(DIR2)==-1?0:2);
                UsbdiskState+=(TestSpace(DIR3)==-1?0:4);
                UsbdiskState+=(TestSpace(DIR4)==-1?0:8);
                if(UsbdiskState==0)
                ui->label->setText(QString("Please Insert USB flash disk!"));
                system("sync");
                char cmd[512];
                sprintf(cmd,"umount %s",Usbdir);
                system(cmd);
                 return;
             }
//        if(UsbdiskState==0)
//         {
//             ui->label->setText("Please Insert USB flash disk");
//             return;
//         }



        switch(UsbdiskState)
        {
        case 1:
            sprintf(Usbdir,"/mnt/usb1");
            break;
        case 2:
            sprintf(Usbdir,"/mnt/usb2");
            break;
        case 4:
            sprintf(Usbdir,"/mnt/usb3");
            break;
        case 8:
            sprintf(Usbdir,"/mnt/usb4");
            break;
        }

        if(Mode==1)
        {

            ui->label->setText("Report is exporting, please wait!");
            char cmd[512];
            usersleep(2);
//            if(CRCCODE==1)
//            {
//                sprintf(cmd,"mv -f ./CRC.dat %s",Usbdir);
//                system(cmd);
//            }
            sprintf(cmd,"cp -f ./report.txt %s",Usbdir);
            system(cmd);
            sprintf(cmd,"cp -f ./TemplateDir/*.mrp %s",Usbdir);
            system(cmd);            
            ui->label->setText("Report is exported,\n please pull out USB flash disk!");
            system("sync");
            sprintf(cmd,"umount %s",Usbdir);
            system(cmd);
        }
        else if(Mode==3)
        {
                if(QFile::exists("./report.txt")||QFile::exists("./TemplateDir/*.mrp"))
               {
                ui->label->setText(QString("Report is deleting，please wait！"));
                char cmd[512];
                usersleep(2);
                sprintf(cmd,"rm -f ./report.txt");
                system(cmd);
                sprintf(cmd,"rm -f ./TemplateDir/*.mrp");
                system(cmd);
                ui->label->setText("Report is deleted!");
                system("sync");
              }
                else
                {
                    ui->label->setText(QString("Report does not exist！"));
                }

         }
        else
        {

            ui->label->setText(QString("Program is updating, please wait!"));

            usersleep(2);
           /* sprintf(cmd,"mv -f %s/APPDIR/* ./",Usbdir);
            system(cmd);
            sprintf(cmd,"mv -f %s/TEMPLATEDIR/* ./TemplateDir/",Usbdir);
            system(cmd);
            sprintf(cmd,"mv -f %s/AUTHORIZEDIR/* ./AuthorizeDir/",Usbdir);
            system(cmd);
            */

            char UsbupdateDir[512];
            sprintf(UsbupdateDir,"%s/APPDIR/",Usbdir);
            QString s= QString(QLatin1String(UsbupdateDir));
            QDir dir=QDir(UsbupdateDir);
            ui->label_2->setText(s);
            if(!dir.exists())
            {
                QMessageBox::warning(this,"Warning","Upgrade file does not exist!",QMessageBox::Ok);
                return;
            }
            dir.setFilter(QDir::Files);
            QFileInfoList list = dir.entryInfoList();
            for(int i=0;i<list.size();i++)
            {
                QFileInfo fileInfo = list.at(i);
                if(fileInfo.isDir())
                    continue;
                if(fileInfo.fileName()=="."||fileInfo.fileName()=="..")
                continue;

                QString filename=fileInfo.fileName();
                if(filename.contains(".dat",Qt::CaseSensitive)||filename.contains(".mtp",Qt::CaseInsensitive))
                {
                   //模板定值文件或者授权文件 将文件移动至 ./AuthorizeDir/
                    char cmd[512];
                    QString fileabsulatepath=fileInfo.absoluteFilePath();
                    QByteArray ba=fileabsulatepath.toLatin1();
                    char *filepath=ba.data();
                    sprintf(cmd,"mv -f %s ./AuthorizeDir/",filepath);
                    system(cmd);
                    system("sync");//同步命令   把文件同步进目录


                }               
               else if(filename.contains(".txt",Qt::CaseSensitive))
                {
                     //模板定值文件或者授权文件 将文件移动至 ./HELPFILEDIR/
                     char cmd[512];
                     QString fileabsulatepath=fileInfo.absoluteFilePath();
                     QByteArray ba=fileabsulatepath.toLatin1();
                     char *filepath=ba.data();
                     sprintf(cmd,"mv -f %s ./HELPFILEDIR/",filepath);
                     system(cmd);
                     system("sync");
                }
                else
                {
                    for(int i=0;i<18;i++)
                    {
                      if(QString::compare(filename,basicprogram[i],Qt::CaseSensitive)==0)
                        {
                            char cmd[512];
                            QString fileabsulatepath=fileInfo.absoluteFilePath();
                            QByteArray ba=fileabsulatepath.toLatin1();
                            char *filepath=ba.data();
                            sprintf(cmd,"mv -f %s ./",filepath);
                            system(cmd);
                            system("sync");

                        }//移动至 ./目录
                    }

                    for(int i=0;i<6;i++)
                    {
                      if(QString::compare(filename,Templateprogram[i],Qt::CaseSensitive)==0)
                        {

                            char cmd[512];
                            QString fileabsulatepath=fileInfo.absoluteFilePath();
                            QByteArray ba=fileabsulatepath.toLatin1();
                            char *filepath=ba.data();
                            sprintf(cmd,"mv -f %s ./TemplateDir/",filepath);
                            system(cmd);
                            system("sync");
                        }//移动至 ./TemplateDir/
                    }

                }
            }


            ui->label->setText(QString("Program is updated,\nplease pull out USB flash disk!"));
            system("sync");
            char cmd[512];
            sprintf(cmd,"umount %s",Usbdir);
            system(cmd);

        }




}


void ExportReport::on_pushButton_2_clicked()
{
   close();
}

void ExportReport::on_radioButton_clicked()
{
   Mode=1;
}

void ExportReport::on_radioButton_2_clicked()
{
 Mode=2;
}


void ExportReport::on_radioButton_3_clicked()
{
    Mode=3;
}
