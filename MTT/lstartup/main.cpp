#include <stdlib.h>
#include <stdio.h>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <Gpio.h>
#include <QTime>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include "em335x_drivers.h"
#include "Uartset.h"
#include "Config.h"
#include "Defpam.h"
#include "KEY.h"


#define DIR1 "/mnt/usb1"
#define DIR2 "/mnt/usb2"
#define LEDNUM GPIO8
float DIR1_TOTAL=0;
float DIR1_USED=0;
float DIR1_FREE=0;
char Appname[512]; //可执行程序的名字
char Parameters[512];

int gpiofd=0;
int FlagHasdata=0; //文件夹里面有数据标志

char ReadSerialNumber[512];
char CmpSerialNumber[512];


TEMPRORARYDATA *temprorarydata;

void delaymsec(int msec)
{
    QTime n=QTime::currentTime();

    QTime now;
    do{
      now=QTime::currentTime();
    }while (n.msecsTo(now)<=msec);

}

void FlashLed()
{
   while(1)
 { GPIO_OutClear(gpiofd, LEDNUM);   //灭
  delaymsec(500);

  GPIO_OutSet(gpiofd, LEDNUM);   //灭
  delaymsec(500);
  }

}


void jiamiXor(char *str, char key2,char *result)
{

    for(int i=0;i<strlen(str); i++)
    result[i]= ~str[i]+key2;
    result[strlen(str)]='\0';
}

void jiemiXor(char *str, char key2,char *result)
{   char tmpchar;
    for(int i=0;i<strlen(str); i++)
     {
        tmpchar=str[i]-key2;
        result[i]=~tmpchar;
     }
    result[strlen(str)]='\0';
}


unsigned long Check(char *str)
{ unsigned int  i=0x01;
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


int CheckTimefileValid(QString Serailnum,QString time)
{

    //检查/etc/tmp/目录下有没有这个授权文件，如果有则不能授权
    QDir dir;

    QByteArray bytearray1=Serailnum.toLatin1();
    char       *string1=bytearray1.data();

    QByteArray bytearray2=time.toLatin1();
    char       *string2=bytearray2.data();

    char  Cmpstring[512];
    sprintf(Cmpstring,"%s %s",string1,string2);



    dir.setPath("/etc/tmp");
    if(dir.exists()==false)
    {
      system("mkdir -p /etc/tmp/");
    }

    char *UsedTimefilename="/etc/tmp/timeinfo.cfg";
    FILE *fptime=NULL;
    char tmpchar1[512];
    char tmpchar2[512];
    fptime=fopen(UsedTimefilename,"r");
    if(fptime==NULL)
    {

       fptime=fopen(UsedTimefilename,"a+");  //若文件不存在则创建该文件,并且写入信息
       if(fptime!=NULL)
      { fprintf(fptime,"%s\n",Cmpstring);
        fclose(fptime);

        return 1;
      }
      else
       return -1;
    }

    while(fgets(tmpchar1,512,fptime)!=NULL)
    {
        int  length=0;

        memset(tmpchar2,0,512);
        char *p2=tmpchar1;

        while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
        {
            tmpchar2[length]=*p2;
            p2++;
            length++;
        }
        tmpchar2[length]='\0';


        if(memcmp(Cmpstring,tmpchar2,strlen(Cmpstring))==0) //相等
        {

              fclose(fptime);
              return -1;
        }

    }

    fclose(fptime);

    {

    FILE *fptime2=fopen(UsedTimefilename,"a+");

    fprintf(fptime2,"%s\n",Cmpstring);
    fclose(fptime2);
    return 1;
    }

}


int TestSpace(char *dir)
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
        char temp[64];
        float v1=0,v2=0,v3=0;
        sscanf(buf,"%s %f  %f  %f ",temp,&v1,&v2,&v3);
        //    /dev/sda4 7624 153 7471 2%
        if(v1>0)
          return 0;
        DIR1_TOTAL=v1;
        DIR1_USED=v2;
        DIR1_FREE=v3;
        return -1;
}



void Deletefile(char type)
{

    char filename[512];
    if(type==1)
       strcpy(filename,"/mnt/usb1/Deletefile.txt");
    else
       strcpy(filename,"/mnt/usb2/Deletefile.txt");

    FILE *fp=NULL;
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
    return;
    }
    char buffer[1024];
    char *p2=buffer;
    int  length=0;
    char buf[1024];
    while(fgets(buffer,1024,fp)!=NULL)
    {

       length=0;
       p2=buffer;

       while((*p2!='\0')&&(*p2!=0x0d)&&(*p2!=0x0a))
       {
           buf[length]=*p2;
           p2++;
           length++;
       }
       buf[length]='\0';

       if(memcmp(buf,"RemoveFileName=",strlen("RemoveFileName="))==0)
       {
           length=0;
           char *p1=buf;
           char buff[1024];
           p1=p1+strlen("RemoveFileName=");
           while(*p1!='\0'&&*p1!=0x0d)
           {
               buff[length]=*p1;
               length++;
               p1++;
           }
           buff[length]='\0';
           if(length>0) //执行删除文件操作
           {
             length=0;
             char *p2=buff;
             char  removefile[512];
             int len=0;
             while(*p2!='\0')
             {
                 if(*p2!=',')
                 {
                     removefile[len]=*p2;
                     p2++;
                     len++;
                 }
                 else if(*p2==',')
                 {
                     removefile[len]='\0';
                     p2++;
                     len=0;
                     char Tmp[1024];
                     if(type==1)
                     sprintf(Tmp,"/mnt/usb1/AUTHORIZEDIR/%s",removefile);
                     else
                     sprintf(Tmp,"/mnt/usb2/AUTHORIZEDIR/%s",removefile);
                     char cmd[512];
                     sprintf(cmd,"rm -f %s",Tmp);
                     system(cmd);
                 }
             }
             if(len>0)
             {
                 removefile[len]='\0';
                 len=0;
                 char Tmp[1024];
                 if(type==1)
                 sprintf(Tmp,"/mnt/usb1/AUTHORIZEDIR/%s",removefile);
                 else
                 sprintf(Tmp,"/mnt/usb2/AUTHORIZEDIR/%s",removefile);
                 char cmd[512]; 
                 sprintf(cmd,"rm -f %s",Tmp);
                 system(cmd);
             }
           }

        break;
       }

    }
    fclose(fp);
}



bool CheckSerial(char *serialnumcmp)
{

    FILE *fp2=NULL;
    fp2=fopen("/etc/tmp/SerialNum","r");
    if(fp2==NULL)
    {
       printf("fopen error\n"); //如果这个文件为空则表示 是第一更新镜像，里面没有写入序列号
       return false;
    }
    fgets(ReadSerialNumber,1024,fp2);
    fclose(fp2);

    if(strlen(ReadSerialNumber)==0) //如果这个文件为空则表示 是第一更新镜像，里面没有写入序列号
    {
        return true;
    }

    if(memcmp(ReadSerialNumber,serialnumcmp,16)==0)
    {
        return true;
    }
    else
        return false;


}




void SetSystemtime(char *filename)
{

    QString Time_SerialNum,Time_Systemtime;
    int year=0,month=0,day=0,hour=0,minute=0,second=0;
    unsigned long Checksum=0;
    FILE *fp=NULL;

    if(QFile::exists(filename)==false)
         return;

    fp=fopen(filename,"r");
    if(fp==NULL)
    {
       printf("fopen error\n");
       return ;
    }
    char key=0x13;
    char miwen[1024];
    char buf[1024];
    int sequnence=0;
    char *p2=NULL;
    char miwen_2[1024];
    char length=0;

    while(fgets(miwen,1024,fp)!=NULL)
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
                CmpSerialNumber[len]=*p1;
                p1++;
               len++;
            }
            temp[len]='\0';
            CmpSerialNumber[len]='\0';
            Time_SerialNum=QString(temp);
            sequnence=1;
        }
        else if(sequnence==1&&memcmp(buf,"SYSTEMTIME=",strlen("SYSTEMTIME="))==0)
        {

            Checksum+=Check(buf);
            char *p1=buf+strlen("SYSTEMTIME=");
            int len=0;
            char temp[512];
            while((*p1!='\0')&&(*p1!=0x0d)&&(*p1!=0x0a))
            {
                temp[len]=*p1;
                p1++;
                len++;
            }
           temp[len]='\0';
           Time_Systemtime=QString(temp);
           sequnence=2;
        }

        else if(sequnence==2&&memcmp(buf,"CHECKCODE=",strlen("CHECKCODE="))==0)
        {
         char *p1=buf+strlen("CHECKCODE=");

         unsigned long CRC=atol(p1);
         if(CRC!=Checksum)
         {
             fclose(fp);
             return ;
         }
         }
    }
    fclose(fp);

    if(CheckSerial(CmpSerialNumber)==false)
      return;



    if(CheckTimefileValid(Time_SerialNum,Time_Systemtime)==-1)
      return;



    QByteArray ba=Time_Systemtime.toLatin1();
    char *buffer=ba.data();
    sscanf(buffer,"%d.%d.%d.%d.%d.%d",&year,&month,&day,&hour,&minute,&second);

    time_t  t;
    struct  tm nowtime;
    nowtime.tm_sec=second;
    nowtime.tm_min=minute;
    nowtime.tm_hour=hour;
    nowtime.tm_mday=day;
    nowtime.tm_mon=month-1;
    nowtime.tm_year=year-1900;

    nowtime.tm_isdst=-1;
    t=mktime(&nowtime);
    stime(&t);
    system("hwclock -w");

    char rmcmd[512];
    sprintf(rmcmd,"rm -f %s",filename);
    system(rmcmd);


}



bool CheckDir(char *dirpath)
{
int num=0;
QDir dir;
if(dir.exists(dirpath)==false)
    return false;
DIR *dirp;
dirp=opendir(dirpath);
while(dirp)
{
if(readdir(dirp)!=NULL)
++num;
else
break;
}
closedir(dirp);
 if(num==2)
     return false;
 else
     return true;
}


int main(int argc,char *argv[])
{
    int Dir1valid=-1;
    int Dir2valid=-1;
    int rc=0;
    int count=8;
    char Appdir1[512];
    char Appdir2[512];
    char Appdir3[512];
    char USBmoountDIr[512];
    int DirInfo=0;

    temprorarydata=new TEMPRORARYDATA;
    temprorarydatainit();


    gpiofd=open("/dev/em335x_gpio", O_RDWR);
    GPIO_OutClear(gpiofd,GPIO9);//clear beep2016-12-20
    rc = GPIO_OutEnable(gpiofd, 0xffffffff);	//set all GPIO as output
    if(rc < 0)
    {
     printf("GPIO_OutEnable::failed %d\n", rc);
     gpiofd=-1;
    }

    delaymsec(500);
    GPIO_OutClear(gpiofd, GPIO6); //关闭轨迹球的电源
    GPIO_OutClear(gpiofd, GPIO8); //关闭LED灯
    GPIO_OutClear(gpiofd,GPIO9);//clear beep2016-12-20
    while((Dir1valid==-1&&Dir2valid==-1)&&count>0)
    {
        delaymsec(500);
        Dir1valid=TestSpace(DIR1);
        Dir2valid=TestSpace(DIR2);
        count--;
    }

/*
    if(Dir1valid==-1&&Dir2valid==-1) //U盘没有挂上，则直接运行主程序
    {
*/        char cmd[512];
        strcpy(cmd,"./Maindows -qws");
        QProcess *proc=new QProcess;
        proc->startDetached(QString(cmd));

        close(gpiofd);
        free(temprorarydata);
        temprorarydata=NULL;
        return 0;
/*    }
*/

/*
    if(Dir1valid==0&&Dir2valid==-1)
    {
      strcpy(Appdir1,"/mnt/usb1/APPDIR/");
      strcpy(Appdir2,"/mnt/usb1/TEMPLATEDIR/");
      strcpy(Appdir3,"/mnt/usb1/AUTHORIZEDIR/");
      strcpy(USBmoountDIr,"/mnt/usb1");
      Deletefile(1);
      SetSystemtime("/mnt/usb1/APPDIR/Systime.atz");

      QFile file;
      file.setFileName("/mnt/usb1/APPDIR/mainconfig.cfg");
      if(file.exists())
      {
         system("mv -f  /mnt/usb1/APPDIR/mainconfig.cfg /mnt/nandflash");
      }



    }
    else  if(Dir1valid==-1&&Dir2valid==0)
    {
        strcpy(Appdir1,"/mnt/usb2/APPDIR/");
        strcpy(Appdir2,"/mnt/usb2/TEMPLATEDIR/");
        strcpy(Appdir3,"/mnt/usb2/AUTHORIZEDIR/");
        strcpy(USBmoountDIr,"/mnt/usb2");
        Deletefile(2);
        SetSystemtime("/mnt/usb2/APPDIR/Systime.atz");

        QFile file;
        file.setFileName("/mnt/usb2/APPDIR/mainconfig.cfg");
        if(file.exists())
        {
           system("mv -f  /mnt/usb2/APPDIR/mainconfig.cfg /mnt/nandflash");
        }

    }


   if(DirInfo==0)  //挂载上，却没有这样的目录
    {

        char cmd[512];
        sprintf(cmd,"umount %s",USBmoountDIr);
        system(cmd);
        strcpy(cmd,"./Maindows -qws");
        QProcess *proc=new QProcess;
        proc->startDetached(QString(cmd));

        close(gpiofd);
        free(temprorarydata);
        temprorarydata=NULL;
        return 0;

    }
*/
    close(gpiofd);
    free(temprorarydata);
    temprorarydata=NULL;
    return 0;
}
