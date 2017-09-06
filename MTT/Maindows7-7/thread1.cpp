#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include <QTimer>
#include <QProcess>
#include <QDir>

#include "Config.h"
#include "thread1.h"
#include "Gpio.h"
#include "Uartset.h"
#include "Testcom2.h"
#include "Nodelinkmanage.h"

 TEMPRORARYDATA *temprorarydata;


 extern char SerialNumber[512];
 extern unsigned char result[70],receive_data[70];
QString serialnumbers,dspnumbers,datenumbers;
 extern bool threadrunstate;  //线程是否正在运行
QString a1,a2,a3,a4,a5,a6;

//串口设置如下
thread1::thread1(QObject *parent) :
    QThread(parent)
{
    temprorarydata=new TEMPRORARYDATA;

}

void thread1::WriteSerialNumber(char *serialnumcmp)
{



    FILE *fp2=NULL;
    fp2=fopen("/etc/tmp/SerialNum","w+");
    if(fp2==NULL)
    {
       printf("fopen  SerialNum  error\n");
       return ;
    }

    fwrite(serialnumcmp,strlen(serialnumcmp),1,fp2);
    fclose(fp2);

}


/*

      //将字符串编码成16进制数字,适用于所有字符（包括中文）
      //@param str 字符串
      //@return 返回16进制字符串

void thread1::static String strToHexStr(String str) {
        // 根据默认编码获取字节数组
        byte[] bytes = str.getBytes();
        StringBuilder sb = new StringBuilder(bytes.length * 2);
        // 将字节数组中每个字节拆解成2位16进制整数
        for (int i = 0; i < bytes.length; i++) {
            sb.append(hexString.charAt((bytes[i] & 0xf0) >> 4));
            sb.append(hexString.charAt((bytes[i] & 0x0f) >> 0));
        }
        return sb.toString();
    }
    */

void thread1::connectdsp()
{
    unsigned char Hignnum=0;
    unsigned char Lownum=0;
    unsigned char SerailNumberCMP[8];
    for(int i=0;i<8;i++)
    {
        if(SerialNumber[i*2]>='0'&&SerialNumber[i*2]<='9')
        {
          Hignnum=SerialNumber[i*2]-0x30;//0x30   16jinzhide '0'
        }
        else if(SerialNumber[i*2]>='a'&&SerialNumber[i*2]<='f')
        {
            Hignnum=SerialNumber[i*2]-87;
        }
        else if(SerialNumber[i*2]>='A'&&SerialNumber[i*2]<='F')
        {
            Hignnum=SerialNumber[i*2]-55;
        }

        if(SerialNumber[i*2+1]>='0'&&SerialNumber[i*2+1]<='9')
        {
          Lownum=SerialNumber[i*2+1]-0x30;
        }
        else if(SerialNumber[i*2+1]>='a'&&SerialNumber[i*2+1]<='f')
        {
            Lownum=SerialNumber[i*2+1]-87;
        }
        else if(SerialNumber[i*2+1]>='A'&&SerialNumber[i*2+1]<='F')
        {
            Lownum=SerialNumber[i*2+1]-55;
        }
     SerailNumberCMP[i]= Hignnum*16+Lownum;
    }
    temprorarydatainit();
    UartsInit();
    emit SIG_showmainface();//显示主界面
    online(temprorarydata->UartDev[0].uartfd);
    artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);

//    for(int i=0;i<8;i++)
//    {
//       if(result[7+i]!=SerailNumberCMP[i]) //比较16进制的序列号 如果序列号不同 则机器不能使用 线程停止
//       {
//        artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
//       return;
//       }
//    }
//    WriteSerialNumber(SerialNumber);
    //解析result中的序列号，DSP号
    QString serial[16];
    for(int i=0;i<8;i++)
    {
        unsigned char hignnum=0;
        unsigned char lownum=0;
        hignnum=((result[7+i]&0xf0)>>4);
         lownum=((result[7+i]&0x0f)>>0);
        if(i<7)
        {
          hignnum=hignnum+0x30;//0x30   16jinzhide '0'
          lownum=lownum+0x30;//0x30   16jinzhide '0'
          serial[i*2]=QString(hignnum);
          serial[i*2+1]=QString(lownum);
        }
        else if(i==7)
        {
            hignnum=hignnum+0x30;//0x30   16jinzhide '0'
            lownum=lownum+55;
            serial[i*2]=QString(hignnum);
            serial[i*2+1]=QString(lownum);
        }

    }
    for(int i=0;i<16;i++)
    serialnumbers+=serial[i];
    //解析DSP号
    QString dsp[6];
    for(int i=0;i<3;i++)
    {
        unsigned char higndsp,lowdsp=0;
        higndsp=((result[15+i]&0xf0)>>4)+0x30;
         lowdsp=((result[15+i]&0x0f)>>0)+0x30;
         dsp[i*2]=QString(higndsp);
         dsp[i*2+1]=QString(lowdsp);
    }
    dspnumbers=dsp[0]+dsp[1]+'.'+dsp[2]+dsp[3]+'.'+dsp[4]+dsp[5];
    //解析校准日期
    QString date[38];
    for(int i=0;i<19;i++)
    {
        unsigned char higndate,lowdate=0;
        higndate=((result[18+i]&0xf0)>>4)+0x30;
         lowdate=((result[18+i]&0x0f)>>0)+0x30;
         date[i*2]=QString(higndate);
         date[i*2+1]=QString(lowdate);
    }
    datenumbers=date[4]+date[5]+date[2]+date[3]+date[0]+date[1]+\
                date[10]+date[11]+date[8]+date[9]+date[6]+date[7];//按照.p文件中的日期顺序来解析

    //发送信号给窗口，显示主界面    
    artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
    close(temprorarydata->UartDev[0].uartfd);
    free(temprorarydata);
    temprorarydata=NULL;


}

void thread1::setcmd(char *cmd)
{
    if(strlen(cmd)==0)
        return;
    strcpy(cmdstr,cmd);
}


void thread1::run()
{
    if(strlen(cmdstr)==0)
        return;
  threadrunstate=true;
  QProcess  *proc=new QProcess;
  proc->start(cmdstr);
  proc->waitForFinished(-1);
  msleep(1500);
  threadrunstate=false;


}

