#include "import.h"
#include "ui_import.h"
#include <QStringList>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QTextCodec>
#include "Nodelinkmanage.h"
#include "Config.h"
#include <QStyledItemDelegate>
#include <QProcess>
#include <QFileInfo>
#include <QMoveEvent>
#include <QMessageBox>
#include <QFileInfo>


bool Importthreadrunstate=false;
extern QString SerialRead;//readfunctionfile() //读功能配置文件读取的序列号
extern QString serials[100];//CheckAuthrizeFile(char *templatefile)//校验授权文件读取的序列号

class NoFocusDelegate2 :public QStyledItemDelegate
{
    void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const;
};

void NoFocusDelegate2::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
     QStyleOptionViewItem itemOption(option);
     if (itemOption.state & QStyle::State_HasFocus)
     {
        itemOption.state = itemOption.state^QStyle::State_HasFocus;
     }
     QStyledItemDelegate::paint(painter, itemOption, index);
}



Import::Import(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Import)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));

    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    setFixedSize(800,480);
    showFullScreen();
    templatelink=(struct NODELINK *)malloc(sizeof(struct NODELINK));
    templatelink->next=NULL;

    AuthorizeTime=0; //授权时间
    ExpireTime=0;     //已经使用时间





   ui->listWidget->setStyleSheet("background-image: url(:/pic/backgound.JPG);");

    ui->listWidget->setIconSize(QSize(60,60));
    //设置Qui->listWidget的显示模式
    ui->listWidget->setViewMode(QListView::IconMode);
      //设置Qui->listWidget中的单元项不可被拖动
    ui->listWidget->setMovement(QListView::Static);
      //设置Qui->listWidget中的单元项的间距
    ui->listWidget->setSpacing(15);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->setItemDelegate(new NoFocusDelegate2());

    ui->listWidget->setFocusPolicy(Qt::NoFocus);

    QFont font("wenquanyi",16);
    ui->listWidget->setFont(font);

}



void Import::jiamiXor(char *str, char key2,char *result)
{

    for(int i=0;i<strlen(str); i++)
    result[i]= ~str[i]+key2;
    result[strlen(str)]='\0';
}
void Import::jiemiXor(char *str, char key2,char *result)
{   char tmpchar;
    for(int i=0;i<strlen(str); i++)
     {
        tmpchar=str[i]-key2;
        result[i]=~tmpchar;
     }
    result[strlen(str)]='\0';
}


unsigned long Import::Check(char *str)
{   unsigned int  i=0x01;
    unsigned int  m,j;
    unsigned long  Recheck=0;
    char *p1=str;
    for(m=0;m<strlen(str);m++)
    { i=0x01;
      for(j=0;j<=7;j++)
      { if(*p1&i)
        {
         Recheck++;
        }
        i=i<<1;
      }
      p1++;
    }
    return  Recheck;
}

unsigned int Import::TemplateCrcNum(char *Filename)
{

    unsigned  long CRC=0;
    QFileInfo fileinfo(Filename);
    QDateTime LastModified=fileinfo.lastModified();
    QString str=LastModified.toString("yyyy:MM:dd:mm:ss"); //不要小时 小时在嵌入式linux始终晚8个小时
    QByteArray  ba= str.toLatin1();
    char *TimeModified=ba.data();

    CRC=Check(TimeModified);
    return CRC;
}
//2016-12-7  pushbuten2 双击校验授权文件显示，注销这个，使用下面的配合新的校验方法
/*
int Import::CheckAuthrizeFile(char *templatefile)
{
    char TemplateFile[512];
    char AuthorizeFile[512];
    sprintf(TemplateFile,"./AuthorizeDir/%s.mtp",templatefile);
    sprintf(AuthorizeFile,"./AuthorizeDir/%s.dat",templatefile);

    unsigned long Checksum=0;
    if(!QFile::exists(TemplateFile))
        return -1;
    if(!QFile::exists(AuthorizeFile))
        return -2;

    QFileInfo templatefileinfo(TemplateFile);
    unsigned long filesize=templatefileinfo.size();
    QDateTime modifiedtime=templatefileinfo.lastModified().toUTC();


    FILE *fp=NULL;
    fp=fopen(AuthorizeFile,"r");
    if(fp==NULL)
    {  printf("fopen error\n");
       return -3;
    }
    char key=0x13;
    char miwen[1024];
    char buf[1024];
    int sequnence=0;
    char *p2=NULL;
    char miwen_2[1024];
    char length=0;
    while(fgets(miwen,BUFFSIZE,fp)!=NULL)
    {
        length=0;
        memset(buf,0,1024);
        p2=miwen;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
        {
            miwen_2[length]=*p2;
            p2++;
            length++;
        }
        miwen_2[length]='\0';
        jiemiXor(miwen_2,key,buf);

        if(memcmp(buf,"SERIALNUM=",strlen("SERIALNUM="))==0&&sequnence==0)
        {
            Checksum+=Check(buf);

            char *p1=buf+strlen("SERIALNUM=");
            int len=0;
            char temp[512];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
            temp[len]='\0';
          if(memcmp(SerialNumber,temp,strlen(SerialNumber))!=0)
          {
            fclose(fp);
            return -4;
          }

        sequnence=1;
        }
        else if(sequnence==1&&memcmp(buf,"FILENAME=",strlen("FILENAME="))==0)
        {

            Checksum+=Check(buf);

            char *p1=buf+strlen("FILENAME=");
            int len=0;
            char temp[512];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';

          char cmpchar[512];
          sprintf(cmpchar,"%s.dat",templatefile);
          if(memcmp(cmpchar,temp,strlen(temp))!=0)
          {
            fclose(fp);
            return -5;
          }
           sequnence=2;
        }
        else if(sequnence==2&&memcmp(buf,"AUTHORIZETIME=",strlen("AUTHORIZETIME="))==0)
        {
            Checksum+=Check(buf);
            int len=0;
            char temp[512];
            char *p1=buf+strlen("AUTHORIZETIME=");
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';
          QDateTime  currentdatetime=QDateTime::currentDateTime();
          QDateTime Authorizetime=QDateTime::fromString(temp,"yyyy.M.d.h.m.s");
          if(currentdatetime.toUTC()<Authorizetime)
          {
              fclose(fp);  ////授权文件已经过期
              return -6;
          }

          sequnence=3;
        }

        else if(sequnence==3&&memcmp(buf,"EXPIRETIME=",strlen("EXPIRETIME="))==0)
        {
            Checksum+=Check(buf);
            int len=0;
            char temp[512];
            char *p1=buf+strlen("EXPIRETIME=");
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';
          QDateTime  currentdatetime=QDateTime::currentDateTime();
          QDateTime Expiretime=QDateTime::fromString(temp,"yyyy.M.d.h.m.s");
          if(currentdatetime.toUTC()>Expiretime)
          {
              fclose(fp);  ////授权文件已经过期
              return -6;
          }
          sequnence=4;
        }



        else if(sequnence==4&&memcmp(buf,"MODIFIEDTIME=",strlen("MODIFIEDTIME="))==0)
       {
           Checksum+=Check(buf);
           int len=0;
           char temp[512];
           char *p1=buf+strlen("MODIFIEDTIME=");
           while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
           {
               temp[len]=*p1;
               p1++;
              len++;
           }
         temp[len]='\0';

         //int year,month,day,hour,minute,second;
         //int year_A,month_A,day_A,hour_A,minute_A,second_A;
         //sscanf(temp,"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&minute,&second);
         //year_A=modifiedtime.date().year();
         //month_A=modifiedtime.date().month();
         //day_A=modifiedtime.date().day();
         //hour_A=modifiedtime.time().hour();
         //minute_A=modifiedtime.time().minute();
         //second_A=modifiedtime.time().second();
         //bool timevalid=year==year_A&&month==month_A&&day==day_A&&\
                       //hour==hour_A&&minute&&minute_A&&second==second_A;


         QDateTime timetmp= QDateTime::fromString(temp,"yyyy-M-d h:m:s");
         if(abs(timetmp.secsTo(modifiedtime))>5)
        // if(timevalid==false)
         {
           fclose(fp);
           return -7;
         }

         sequnence=5;

       }

        else if(sequnence==5&&memcmp(buf,"FILESIZE=",strlen("FILESIZE="))==0)
      {
          Checksum+=Check(buf);
          int len=0;
          char temp[512];
          char *p1=buf+strlen("FILESIZE=");
          while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
          {
              temp[len]=*p1;
              p1++;
             len++;
          }
        temp[len]='\0';
        unsigned long  fileinfo_filesize=atol(temp);
        if(filesize!=fileinfo_filesize)
        {
            fclose(fp);
            return -8;
        }

        sequnence=6;

      }






        else if(sequnence==6&&memcmp(buf,"CHECKCODE=",strlen("CHECKCODE="))==0)
        {
         char *p1=buf+strlen("CHECKCODE=");

         unsigned long CRC=atol(p1);
         if(CRC!=Checksum)
         {
             fclose(fp);
             return -9;
         }
        }


    }
    fclose(fp);
    return 1;
}
*/
int Import::CheckAuthrizeFile(char *templatefile)//校验授权文件
{
    char TemplateFile[512];
    char AuthorizeFile[512];
    sprintf(TemplateFile,"./AuthorizeDir/%s.mtp",templatefile);
    sprintf(AuthorizeFile,"./AuthorizeDir/%s.dat",templatefile);

    unsigned long Checksum=0;
    if(!QFile::exists(TemplateFile))
        return -1;
    if(!QFile::exists(AuthorizeFile))
        return -2;


    QFileInfo templatefileinfo(TemplateFile);
    unsigned long filesize=templatefileinfo.size();
    QDateTime modifiedtime=templatefileinfo.lastModified().toUTC();//模板文件的最后修改时间


    FILE *fp=NULL;
    fp=fopen(AuthorizeFile,"r");
    if(fp==NULL)
    {  printf("fopen error\n");
       return -3;
    }
    char key=0x13;
    char miwen[1024];
    char buf[1024];
    int sequnence=0;
    char *p2=NULL;
    char miwen_2[1024];
    char length=0;
    int i=0;
     int j=0;
    while(fgets(miwen,BUFFSIZE,fp)!=NULL)
    {
        length=0;
        memset(buf,0,1024);
        p2=miwen;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
        {
            miwen_2[length]=*p2;
            p2++;
            length++;
        }
        miwen_2[length]='\0';
        jiemiXor(miwen_2,key,buf);


        if(/*sequnence==1&&*/memcmp(buf,"FILENAME=",strlen("FILENAME="))==0)
        {

            Checksum+=Check(buf);

            char *p1=buf+strlen("FILENAME=");
            int len=0;
            char temp[512];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';

          char cmpchar[512];
          sprintf(cmpchar,"%s.dat",templatefile);
          if(memcmp(cmpchar,temp,strlen(temp))!=0)
          {
            fclose(fp);
            return -5;
          }
           sequnence=2;
        }
        else if(sequnence==2&&memcmp(buf,"AUTHORIZETIME=",strlen("AUTHORIZETIME="))==0)
        {
            Checksum+=Check(buf);
            int len=0;
            char temp[512];
            char *p1=buf+strlen("AUTHORIZETIME=");
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';
          QDateTime  currentdatetime=QDateTime::currentDateTime();
          QDateTime Authorizetime=QDateTime::fromString(temp,"yyyy.M.d.h.m.s");
          if(currentdatetime.toUTC()<Authorizetime)
          {
              fclose(fp);  ////授权文件已经过期
              return -6;
          }

          sequnence=3;
        }

        else if(sequnence==3&&memcmp(buf,"EXPIRETIME=",strlen("EXPIRETIME="))==0)
        {
            Checksum+=Check(buf);
            int len=0;
            char temp[512];
            char *p1=buf+strlen("EXPIRETIME=");
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';
          QDateTime  currentdatetime=QDateTime::currentDateTime();
          QDateTime Expiretime=QDateTime::fromString(temp,"yyyy.M.d.h.m.s");
          if(currentdatetime.toUTC()>Expiretime)
          {
              fclose(fp);  ////授权文件已经过期
              return -6;
          }
          sequnence=4;
        }

        else if(sequnence==4&&memcmp(buf,"MODIFIEDTIME=",strlen("MODIFIEDTIME="))==0)
       {
           Checksum+=Check(buf);
           int len=0;
           char temp[512];
           char *p1=buf+strlen("MODIFIEDTIME=");
           while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
           {
               temp[len]=*p1;
               p1++;
              len++;
           }
         temp[len]='\0';

         /*
         int year,month,day,hour,minute,second;
         int year_A,month_A,day_A,hour_A,minute_A,second_A;
         sscanf(temp,"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&minute,&second);
         year_A=modifiedtime.date().year();
         month_A=modifiedtime.date().month();
         day_A=modifiedtime.date().day();
         hour_A=modifiedtime.time().hour();
         minute_A=modifiedtime.time().minute();
         second_A=modifiedtime.time().second();
         bool timevalid=year==year_A&&month==month_A&&day==day_A&&\
                       hour==hour_A&&minute&&minute_A&&second==second_A;

                       */

         QDateTime timetmp= QDateTime::fromString(temp,"yyyy-M-d h:m:s");//timetmp  授权文件中的最后修改时间

         if(abs(timetmp.secsTo(modifiedtime))>5)//modifiedtime  模板文件的最后修改时间
         //if(timevalid==false)
         {
           fclose(fp);
           return -7;
         }
         sequnence=5;
       }
        else if(sequnence==5&&memcmp(buf,"FILESIZE=",strlen("FILESIZE="))==0)
      {
          Checksum+=Check(buf);
          int len=0;
          char temp[512];
          char *p1=buf+strlen("FILESIZE=");
          while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
          {
              temp[len]=*p1;
              p1++;
             len++;
          }
        temp[len]='\0';
        unsigned long  fileinfo_filesize=atol(temp);
        if(filesize!=fileinfo_filesize)
        {
            fclose(fp);
            return -8;
        }
        sequnence=6;
      }
//        else if(sequnence==6&&memcmp(buf,"CHECKCODE=",strlen("CHECKCODE="))==0)
//        {
//         char *p1=buf+strlen("CHECKCODE=");

//         unsigned long CRC=atol(p1);
//         if(CRC!=Checksum)
//         {
//             fclose(fp);
//             return -9;
//         }
//         sequnence=7;
//        }
        else if(sequnence==6&&memcmp(buf,"SERIALNUM=",strlen("SERIALNUM="))==0)
        {
            Checksum+=Check(buf);
            char *p1=buf+strlen("SERIALNUM=");
            int len=0;
            char temp[512];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
            temp[len]='\0';
            serials[i]=QString(temp);
            i++;
        /*  if(memcmp(SerialNumber,temp,strlen(SerialNumber))!=0)
          {
            fclose(fp);
            return -4;
          }
     sequnence=1;*/
        }
    }

   //比较序列号
    while(j<100&&(!serials[j].isEmpty()))
    {
     QString num = serials[j];
     if(SerialRead==num)
     {
         fclose(fp);
         return 1;
     }
     j++;
    }
    if(serials[j].isEmpty())
    {
      fclose(fp);
      return -4;
    }
    fclose(fp);
    return 1;
}


bool Import::CheckSerailNumber(char *templatefile)  //检查是序列是否匹配
{
    char TemplateFile[512];
    char AuthorizeFile[512];
    sprintf(TemplateFile,"./AuthorizeDir/%s.mtp",templatefile);
    sprintf(AuthorizeFile,"./AuthorizeDir/%s.dat",templatefile);

    unsigned long Checksum=0;
    if(!QFile::exists(TemplateFile))
        return false;
    if(!QFile::exists(AuthorizeFile))
        return false;

    FILE *fp=NULL;
    fp=fopen(AuthorizeFile,"r");
    if(fp==NULL)
    {  printf("fopen error\n");
       return false;
    }
    char key=0x13;
    char miwen[1024];
    char buf[1024];
    int sequnence=0;
    char *p2=NULL;
    char miwen_2[1024];
    char length=0;
    while(fgets(miwen,BUFFSIZE,fp)!=NULL)
    {
        length=0;
        memset(buf,0,1024);
        p2=miwen;
        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
        {
            miwen_2[length]=*p2;
            p2++;
            length++;
        }
        miwen_2[length]='\0';
        jiemiXor(miwen_2,key,buf);

        if(memcmp(buf,"SERIALNUM=",strlen("SERIALNUM="))==0&&sequnence==0)
        {
            Checksum+=Check(buf);

            char *p1=buf+strlen("SERIALNUM=");
            int len=0;
            char temp[512];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
            temp[len]='\0';
          if(memcmp(SerialNumber,temp,strlen(SerialNumber))!=0)
          {
            fclose(fp);
            return false;
          }

        sequnence=1;
        }
        else if(sequnence==1&&memcmp(buf,"FILENAME=",strlen("FILENAME="))==0)
        {

            Checksum+=Check(buf);

            char *p1=buf+strlen("FILENAME=");
            int len=0;
            char temp[512];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
               len++;
            }
          temp[len]='\0';
          char cmpchar[512];
          sprintf(cmpchar,"%s.dat",templatefile);
          if(memcmp(cmpchar,temp,strlen(temp))!=0)
          {
            fclose(fp);
            return false;
          }
           sequnence=2;
        }
    }
    fclose(fp);
    return true;
}


void Import::SetSerialNum(char *serial)
{
 strcpy(SerialNumber,serial);
 readtemplatefile();
}



void Import::readtemplatefile() //读本地的模板文件
{
    QStringList list;
    QDir dir(Templatedir);
    QStringList filters;
    filters << "*.mtp" << "*.MTP";
    dir.setNameFilters(filters);
    list=dir.entryList();

    if(list.size()>=1)
    {
        QByteArray ba;
        char *mm;
        int id=0;
        for(int i=0;i<list.size();i++)
        {

            ba=list.at(i).toLatin1();
            mm=ba.data();
            char *tmp=mm;
            char buffer[512];
            int len=0;

            while(memcmp(tmp,".mtp",4)!=0&&memcmp(tmp,".MTP",4)!=0)
            {
                buffer[len]=*tmp;
                tmp++;
                len++;
            }
            buffer[len]='\0';

            //if(CheckSerailNumber(buffer)==true)
            if(CheckAuthrizeFile(buffer)==true)
           {
             insertnodelink(templatelink,buffer,id);
             id++;
           }
        }

        struct NODELINK *next2=templatelink->next;
        while(next2!=NULL)
        {
           QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/pic/Temp.png"), QString((next2->name)));
           pItem->setSizeHint(QSize(80,100));
           pItem->setTextAlignment(Qt::AlignCenter);

           pItem->setToolTip("<font color=blue>"+QString(next2->name)+"</font>");
           ui->listWidget->addItem(pItem);
           if(CheckAuthrizeFile(next2->name)!=1)
           {
            pItem->setBackgroundColor(Qt::gray);
           }
           next2=next2->next;
        }
        //退出 图标
    }

    QListWidgetItem *pItem2=new QListWidgetItem(QIcon(":/pic/Exitout.png"), QString(("Exit")));
    pItem2->setSizeHint(QSize(80,100));
    pItem2->setTextAlignment(Qt::AlignCenter);
    ui->listWidget->addItem(pItem2);
}


Import::~Import()
{
    delete ui;
}

void Import::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{

    if(Importthreadrunstate==true)
        return;


    QListWidget *listwidget=item->listWidget();
    int index=listwidget->currentRow();
    if(index==(listwidget->count()-1))
    {
    free(templatelink);
    templatelink=NULL;
/*
    if(importthread!=NULL)
    {

      importthread->terminate();
      importthread->wait();
      free(importthread);
      importthread=NULL;
    }
    */
    close();
    }
    else
    {
        char  tempfile[512];
        char templatename[512];
        if(geditnamefromid(templatelink,index,templatename)==false)
        return;

        switch(CheckAuthrizeFile(templatename))
        {
          case -1:
            item->setBackgroundColor(Qt::gray);
            QMessageBox::warning(this,tr("warning"),tr("The template file does not exist!"),QMessageBox::Ok);
            return;
            break;
          case -2:
               item->setBackgroundColor(Qt::gray);
            QMessageBox::warning(this,tr("warning"),tr("The authorized file does not exist!"),QMessageBox::Ok);
            return;
            break;
          case -3:
               item->setBackgroundColor(Qt::gray);
            QMessageBox::warning(this,tr("warning"),tr("The authorized file can not be opened!"),QMessageBox::Ok);
            return;
            break;
         case -4:
               item->setBackgroundColor(Qt::gray);
          QMessageBox::warning(this,tr("warning"),tr("The authorized file can not be used in this test instrument!"),QMessageBox::Ok);
          return;
          break;
        case -5:
               item->setBackgroundColor(Qt::gray);
         QMessageBox::warning(this,tr("warning"),tr("The authorized file can not be used in this test template file!"),QMessageBox::Ok);
         return;
         break;
        case -6:
               item->setBackgroundColor(Qt::gray);
        QMessageBox::warning(this,tr("warning"),tr("The authorized file has expired!"),QMessageBox::Ok);
        return;
            break;
        case -7:
            item->setBackgroundColor(Qt::gray);
            QMessageBox::warning(this,tr("warning"),tr("The original template  file is modified!"),QMessageBox::Ok);
            return;
            break;
        case -8:
            item->setBackgroundColor(Qt::gray);
            QMessageBox::warning(this,tr("warning"),tr("The original template  file is modified!"),QMessageBox::Ok);
            return;
            break;
        case -9:
            item->setBackgroundColor(Qt::gray);
            QMessageBox::warning(this,tr("warning"),tr("The authorized file is damaged!"),QMessageBox::Ok);
            return;
            break;
        }

     //   item->setBackgroundColor();
        strcat(templatename,".mtp");
        sprintf(tempfile,"./Templateprogram %s",templatename);
        Importthread  *importthread=new Importthread;
        importthread->setcmd(tempfile);
        importthread->start();
    }


}


void Import::moveEvent(QMoveEvent *event)
{
 QPoint oldpos=event->oldPos();
 this->move(oldpos);
}




