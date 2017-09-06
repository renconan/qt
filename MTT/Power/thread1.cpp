#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include "Config.h"
#include <QSemaphore>
#include "Testcom2.h"
#include "thread1.h"
#include "Curseq.h"
#include <QVector>


TEMPRORARYDATA *temprorarydata;
#define LEDNUM GPIO8

QSemaphore sem(1);


int StartPhaseActState=0;  //0δ����,1����
int SearchType=0;  //��������  0 ��ʱ��  1 ˳ʱ��

QVector<ACTSTATE> AngleActStateVect; //


extern bool SoftStartEndflag;



extern bool Threadrunstate;


extern  float Postfaulttime;
extern  int   dcouttype;





int GPIO_OutEnable(int fd, unsigned int dwEnBits)
{
        int 				rc;
        struct double_pars	dpars;

        dpars.par1 = EM335X_GPIO_OUTPUT_ENABLE;		// 0
        dpars.par2 = dwEnBits;

        rc = write(fd, &dpars, sizeof(struct double_pars));
        return rc;
}

int GPIO_OutDisable(int fd, unsigned int dwDisBits)
{
        int 				rc;
        struct double_pars	dpars;

        dpars.par1 = EM335X_GPIO_OUTPUT_DISABLE;	// 1
        dpars.par2 = dwDisBits;

        rc = write(fd, &dpars, sizeof(struct double_pars));
        return rc;
}

int GPIO_OutSet(int fd, unsigned int dwSetBits)
{
        int 				rc;
        struct double_pars	dpars;

        dpars.par1 = EM335X_GPIO_OUTPUT_SET;	// 2
        dpars.par2 = dwSetBits;

        rc = write(fd, &dpars, sizeof(struct double_pars));
        return rc;
}

int GPIO_OutClear(int fd, unsigned int dwClearBits)
{
        int 				rc;
        struct double_pars	dpars;

        dpars.par1 = EM335X_GPIO_OUTPUT_CLEAR;	// 3
        dpars.par2 = dwClearBits;

        rc = write(fd, &dpars, sizeof(struct double_pars));
        return rc;
}

int GPIO_PinState(int fd, unsigned int* pPinState)
{
        int 				rc;
        struct double_pars	dpars;

        dpars.par1 = EM335X_GPIO_INPUT_STATE;	// 5
        dpars.par2 = *pPinState;

        rc = read(fd, &dpars, sizeof(struct double_pars));
        if(!rc)
        {
                *pPinState = dpars.par2;
        }
        return rc;
}


void SetLED()
{
   char cmdstr[512];
   sprintf(cmdstr,"echo 1 > /sys/class/leds/user_led0/brightness");
   system(cmdstr);
}

void ClearLED()
{
    char cmdstr[512];
    sprintf(cmdstr,"echo 0 > /sys/class/leds/user_led0/brightness");
    system(cmdstr);
}



void temprorarydatainit()
{

    memset(temprorarydata,0,sizeof(temprorarydata));
    int i=0;
    {
    temprorarydata->UartDev[i].valid=1;
    temprorarydata->UartDev[i].uartfd=0;
    temprorarydata->UartDev[i].baud=19200;
    temprorarydata->UartDev[i].datanum=8;
    temprorarydata->UartDev[i].parity='N';
    temprorarydata->UartDev[i].stopbit=1;
    sprintf(temprorarydata->UartDev[i].describ,"/dev/ttyS1");
    sprintf(temprorarydata->UartDev[i].devname,"/dev/ttyS1");
    temprorarydata->UartDev[i].rs485Mode=0;
    }
    i=1;
    {
    temprorarydata->UartDev[i].valid=1;
    temprorarydata->UartDev[i].uartfd=0;
    temprorarydata->UartDev[i].baud=9600;
    temprorarydata->UartDev[i].datanum=8;
    temprorarydata->UartDev[i].parity='N';
    temprorarydata->UartDev[i].stopbit=1;
    sprintf(temprorarydata->UartDev[i].describ,"/dev/ttyS3");
    sprintf(temprorarydata->UartDev[i].devname,"/dev/ttyS3");
    temprorarydata->UartDev[i].rs485Mode=0;
    }

    //���ڽ��ն���
    for(i=0;i<MAX_UARTDEV_NUM;i++)
    {
    temprorarydata->UartRcv[i].readsequence=0;
    temprorarydata->UartRcv[i].writesequence=0;
    }
    temprorarydata->keypadopenflag=false;
    temprorarydata->helpopenflag=false;

}


void UartsInit()
{
    int i;
    for(i=0;i<MAX_UARTDEV_NUM;i++)
    {
     UartInit(i);
    }

}
void UartInit(int uartno)
{


  int i;
  i=uartno;
 {
    temprorarydata->UartDev[i].uartfd=-1;
    if((temprorarydata->UartDev[i].valid=='1')||(temprorarydata->UartDev[i].valid==1))
    {
    temprorarydata->UartDev[i].uartfd=OpenDev(temprorarydata->UartDev[i].devname);
    if(temprorarydata->UartDev[i].uartfd>0)
    {
    set_speed(temprorarydata->UartDev[i].uartfd,temprorarydata->UartDev[i].baud);
    set_Parity(temprorarydata->UartDev[i].uartfd,temprorarydata->UartDev[i].datanum,temprorarydata->UartDev[i].parity,temprorarydata->UartDev[i].stopbit);
    }
    }

  }
}

#ifdef ARMLINUX
int speed_arr[] = {B115200,B38400, B19200, B9600, B4800, B2400, B1200, B300,
                                B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {115200, 38400,  19200,  9600,  4800,  2400,  1200,  300, 38400,
                                19200,  9600, 4800, 2400, 1200,  300, };
#endif


void my_itoa( int val,char *buf)
{
char *p;
char *firstdig;
char temp;
unsigned digval;
p = buf;

if (val<0) {
*p++ = '-';
val = (unsigned int)(-(int)val);
}

firstdig = p;

do {
digval = (unsigned) (val % 10);
val /= 10;


*p++ = (char) (digval + '0');
} while (val > 0);


*p-- = '\0';

do {
temp = *p;
*p = *firstdig;
*firstdig = temp;
--p;
++firstdig;
} while (firstdig < p);
}


int OpenDev(char *Dev)
{
    int fd=0;
#ifdef ARMLINUX
    fd = open(Dev, O_RDWR );         //| O_NOCTTY | O_NDELAY
    if (-1 == fd)
    {
    sleep(1);
    if((fd = open( Dev, O_RDWR|O_NOCTTY|O_NDELAY))==-1)
    {
    printf("Can't Open Serial Port,dev:%s\r\n",Dev);
    }
    }
    if(fd>0)
    tcflush(fd, TCIFLUSH);
#endif
    return fd;
 }

int set_speed(int fd, int speed)
{
    #ifdef ARMLINUX
        int   i;
        int   status;
        struct termios   Opt;
        tcgetattr(fd, &Opt);
        for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
        {
        if  (speed == name_arr[i])
        {
        tcflush(fd, TCIOFLUSH);
        cfsetispeed(&Opt, speed_arr[i]);
        cfsetospeed(&Opt, speed_arr[i]);
        status = tcsetattr(fd, TCSANOW, &Opt);
        if  (status != 0)
        {
        perror("tcsetattr fd");
        return -1;
        }
        tcflush(fd,TCIOFLUSH);
        return 1;
        }
        }
        return -1;//����ʧ��
#endif
        return 0;
}
int set_Parity(int fd,int databits,char parity,int stopbits)
{
    #ifdef ARMLINUX
    struct termios options;
    if  ( tcgetattr( fd,&options)  !=  0) {
    perror("SetupSerial 1");
    return(0);
    }
    options.c_cflag &= ~CSIZE;
    options.c_iflag &=~(IXON | IXOFF | IXANY);
    options.c_iflag &=~(INLCR | IGNCR | ICRNL);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    switch (databits) /*?��??????????*/
    {
    case 7:
    options.c_cflag |=   CS7 |  CREAD | CLOCAL;
    break;
    case 8:
    options.c_cflag |=    CS8 |  CREAD | CLOCAL;
    break;
    default:
    fprintf(stderr,"Unsupported data size\n"); return (0);
    }
    switch (parity)
    {
    case 'n':
    case 'N':
    options.c_cflag &= ~PARENB;   /* Clear parity enable */
    options.c_iflag = IGNPAR;
    break;
    case 'o':
    case 'O':

    options.c_cflag |= (PARODD | PARENB); /* ?��???????��?��*/
    options.c_iflag |= INPCK;             /* Enable parity checking */
    break;
    case 'e':
    case 'E':
    options.c_cflag |= PARENB;     /* Enable parity */
    options.c_cflag &= ~PARODD;   /* ��????????��?��*/
    options.c_iflag |= INPCK;       /* Enable parity checking */
    break;
    case 'M':
    case 'm':
    options.c_cflag |= PARENB;
    options.c_cflag |= CMSPAR;     /*��?????Mark ???�� */
    options.c_iflag &= ~INPCK;     /* Disnable parity checking */
    break;
    default:
    fprintf(stderr,"Unsupported parity\n");
    return (0);
    }
    /* ?��????????*/
    switch (stopbits)
    {
    case 1:
    options.c_cflag &= ~CSTOPB;
    break;
    case 2:
    options.c_cflag |= CSTOPB;
    break;
    default:
    fprintf(stderr,"Unsupported stop bits\n");
    return (0);
    }
    /* Set input parity option */
    if (parity != 'n')
    options.c_iflag |= INPCK;
    tcflush(fd,TCIFLUSH);


    options.c_oflag = 0;
    options.c_lflag = 0; //non ICANON
    /*
    initialize all control characters
    default values can be found in /usr/include/termios.h, and
    are given in the comments, but we don't need them here
    */
    options.c_cc[VINTR]    = 0;     /* Ctrl-c */
    options.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    options.c_cc[VERASE]   = 0;     /* del */
    options.c_cc[VKILL]    = 0;     /* @ */
    options.c_cc[VEOF]     = 4;     /* Ctrl-d */
    options.c_cc[VTIME]    = 0;     /* inter-character timer, timeout VTIME*0.1 */
    options.c_cc[VMIN]     = 0;     /* blocking read until VMIN character arrives */
    options.c_cc[VSWTC]    = 0;     /* '\0' */
    options.c_cc[VSTART]   = 0;     /* Ctrl-q */
    options.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    options.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    options.c_cc[VEOL]     = 0;     /* '\0' */
    options.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    options.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    options.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    options.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    options.c_cc[VEOL2]    = 0;     /* '\0' */



    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
    perror("SetupSerial 3");
    return (0);
    }
    #endif
    return (1);
}

int UartSend(int fd,char *cmd,int length)
{
    int len;
    if(length==0)
    len=strlen(cmd);
    else
    len=length;
    #ifdef ARMLINUX
    write(fd,cmd,len);
    #endif
    return 0;
}
int UartFlush(int fd)
{
    #ifdef ARMLINUX
    tcflush(fd, TCIFLUSH);
    #endif
    return 0;
}

void str2STR(char *str)  //�����ַ�ȫת��д
{
    while(*str)
    {
    if(*str>='a' && *str<='z')
    *str-=0x20;
    str++;
    }
}



thread1::thread1(QObject *parent) :
    QThread(parent)
{
    temprorarydata=new TEMPRORARYDATA;
}



int  thread1::Buzzer(char Address1,char Address2,char Address3)
  {//sys[1]:���أ�sys[2]:��Ъ��sys[3]:����
    if(sys[1]==1)
      {//������Ϣ
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1����
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2����
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3����
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1����
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2����
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3����
        if((Address3&0x20)!=0x00)
          return 1;//ֱ������
      }
    else if(sys[1]==0)
      {//������Ϣ
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1����
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2����
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3����
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1����
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2����
        if((Address3&0x40)!=0x00&&V3Output==1)
          return -1;//V3����
        if((Address3&0x20)!=0x00)
          return 1;//ֱ������
      }
    if(sys[2]==1)
      {//��Ъ��Ϣ
        if((Address1&0x10)!=0x00)
          return 1;//I1��Ъ
        if((Address1&0x04)!=0x00)
          return 1;//I2��Ъ
        if((Address1&0x01)!=0x00)
          return 1;//I3��Ъ
        if((Address2&0x08)!=0x00)
          return 1;//V1��Ъ
        if((Address2&0x04)!=0x00)
          return 1;//V2��Ъ
        if((Address3&0x80)!=0x00)
          return 1;//V3��Ъ
      }
    else if(sys[2]==0)
      {//��Ъ��Ϣ
        if((Address1&0x10)!=0x00)
          return -1;//I1��Ъ
        if((Address1&0x04)!=0x00)
          return -1;//I2��Ъ
        if((Address1&0x01)!=0x00)
          return -1;//I3��Ъ
        if((Address2&0x08)!=0x00)
          return -1;//V1��Ъ
        if((Address2&0x04)!=0x00)
          return -1;//V2��Ъ
        if((Address3&0x80)!=0x00)
          return -1;//V3��Ъ
      }
    if(sys[3]==1)
      {//������Ϣ
        if((Address1&0x80)!=0x00)
          return 1;//I1����
        if((Address1&0x40)!=0x00)
          return 1;//I2����
        if((Address1&0x20)!=0x00)
          return 1;//I3����
        if((Address2&0x20)!=0x00)
          return 1;//V1����
        if((Address2&0x40)!=0x00)
          return 1;//V2����
        if((Address2&0x01)!=0x00)
          return 1;//V3����
      }
    else if(sys[3]==0)
      {//������Ϣ
        if((Address1&0x80)!=0x00)
          return -1;//I1����
        if((Address1&0x40)!=0x00)
          return -1;//I2����
        if((Address1&0x20)!=0x00)
          return -1;//I3����
        if((Address2&0x20)!=0x00)
          return -1;//V1����
        if((Address2&0x40)!=0x00)
          return -1;//V2����
        if((Address2&0x01)!=0x00)
          return -1;//V3����
      }
    return 0;
  }

void thread1::logic_info(int fd,char info_result1,char info_result2,char info_result3,char info_result4)
  {

    static int info_count=1;
    int  k1=0;

    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);
    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //����Ҫ�ؼ�
          artExec(fd,"out:dig:on(0x80)",result,0);
        else
          artExec(fd,"out:dig:off(0x80)",result,0);
      }

    if((info_result4&0x0F)==0x00)
    {
        emit SIG_dspstate(100);
    }
    else
    {
        if((info_result4&0x01)==0x01)
        {
            emit SIG_dspstate(101);
        }
        else if((info_result4&0x01)==0x00)
        {
            emit SIG_dspstate(111);
        }
        if((info_result4&0x02)==0x02)
        {
            emit SIG_dspstate(102);
        }
        else if((info_result4&0x02)==0x00)
        {
            emit SIG_dspstate(112);
        }
        if((info_result4&0x04)==0x04)
        {
            emit SIG_dspstate(103);
        }
        else if((info_result4&0x04)==0x00)
        {
            emit SIG_dspstate(113);
        }
        if((info_result4&0x08)==0x08)
        {
            emit SIG_dspstate(104);
        }
        else if((info_result4&0x08)==0x00)
        {
            emit SIG_dspstate(114);
        }
    }
    if(Buzzer(info_result1,info_result2,info_result3)==0) //����Ҫ�ؼ�
      {
        //��������
          emit SIG_dspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
           emit SIG_dspstate(1);
            //IA����
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
            emit SIG_dspstate(2);

            //IABC �¶ȱ���
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //IC��Ъ
              emit SIG_dspstate(3);
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==2)
      {
        if((info_result1&0x02)==0x02&&I2Output==1)
          {  //info_result1�ǵ�1·��ַ��DB9~DB15��

              emit SIG_dspstate(4);
            //IB����
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1�ǵ�1·��ַ��DB8~DB15��

              emit SIG_dspstate(5);
           //IC����
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
              emit SIG_dspstate(6);
          //IA��Ъ
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==3)
      {
        if((info_result2&0x80)==0x80&&I3Output==1)
          { //info_result2�ǵ�2·��ַ��DB9~DB15��
           //IC����
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1�ǵ�1·��ַ��DB8~DB15��
          //IABC ��Դ����
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
              emit SIG_dspstate(9);
          //I2��Ъ  ?�¶ȱ�����Ъ
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==4)
      {
        if((info_result2&0x10)==0x10&&V1Output==1)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
           //VA����
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��
              emit SIG_dspstate(11);
              //V2����
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3�ǵ�2·��ַ��DB0~DB7��

              //V3��Ъ
              emit SIG_dspstate(12);
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==5)
      {
        if((info_result2&0x02)==0x02&&V2Output==1)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
            //V2����
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��  //DB8
           // VABC �¶ȱ���
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
            emit SIG_dspstate(15);
           //VA��Ъ
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }

    else if(info_count==6)
      {
        if((info_result3&0x40)==0x40&&V3Output==1)
          { //info_result3�ǵ�2·��ַ��DB0~DB7��

             emit SIG_dspstate(16);
            //VC����
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��  //DB13
            emit SIG_dspstate(17);
            //VABC��Դ����
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
            emit SIG_dspstate(18);


          //Vz����

            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==7)
      {
        if((info_result3&0x20)==0x20)
          {//info_result3�ǵ�2·��ַ��DB0~DB7��
            emit SIG_dspstate(19);

             //ֱ������
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��
          emit SIG_dspstate(20);

            // VABC �¶ȱ���
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3�ǵ�2·��ַ��DB0~DB7��
            emit SIG_dspstate(21);
           //V3��Ъ
            k1=1;
          }
        if(k1==1)
          {
            info_count=1;
            return;
          }
        else
          info_count=1;
      }

  }

float thread1::CheckAngle(QVector<ACTSTATE>  vect,int type)
{
  if(type==1)  //�ҵ�һ���߽�Ƕ�
  {
      int first=vect.at(0).Triped;
      int i=1;
      for(i=1;i<vect.size();i++)
       {
        if(vect.at(i).Triped!=first)
            return vect.at(i).Angle;
       }
       if(i=vect.size())
       {
           return -1; //δ�ҵ��߽��
       }
  }
  /*
  else if(type==2) //�ҵ�һ���߽�Ƕ�
  {
      int size=vect.size() ;
      if(vect.at(0).Triped!=vect.at(size-1).Triped)  //ǰ�����е�һ���߽�Ƕ�
          return -2; //δ�ҵ��ڶ����߽�
      return vect.at(size-1).Angle;  //�ҵ��ڶ����߽�S
  }

  */


}




void  thread1::ReadTripTimeLogic(int fd)
{


       int t3=0,t4=0,t5=0;
       unsigned char signature;
       double u,v,w;
       static double SequencerTime4;

       artExec(fd,"inp:buf:get?",result,15);//��DSP�õ��������ӵ���Ϣ��
       u=result[7];
       v=result[8];
       w=result[9];
       signature=int(result[13]&0x7f);
       switch(signature)
         {
           case 1:
             break;
           case 2:
             break;
           case 3:
             t3=1;
             break;
           case 4:
             t4=1;
             SequencerTime4=gettime(u,v,w);//����ӿ�ʼ���Ե�sequencer��Step4��ʱ�䡣
             break;
           case 5:
             t5=1;
             break;
         }

       if(t3==1)
         {  //δ����
           output_signal_end(fd);
           ACTSTATE tmp;
           tmp.Triped=0;
           tmp.Angle=OutPhase;
           AngleActStateVect.append(tmp);
           if(OutPhase==StartPhase)
           {
             if(SearchType==0)
             OutPhase=OutPhase+StepPhase;
             else
             OutPhase=OutPhase-StepPhase;

             if(OutPhase>(StartPhase+360)||OutPhase<(StartPhase-360))//2016-6-8
             {
                 //ClearLED();
                 GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                 starttest=0;
                 emit SIG_DisplayChangeValue(0,OutPhase);
                 return;
             }

             artExec(fd,"out:dig:on(0x08)",result,0);

             OutputSequencer(fd);
             emit SIG_DisplayChangeValue(1,OutPhase);
             return;
           }

           if(CheckAngle(AngleActStateVect,1)!=-1)  //�ҵ��˵�һ���߽�Ƕ�
           {
             if(Angle1==-1000)  //�ҵ���һ���߽�Ƕ�  ,��������һ���߽��
             {
                if(AngleActStateVect.at(0).Triped==0)   //  //�����һ��û�ж�������ô������ǰ����
                {
                int size=AngleActStateVect.size();
                Angle1=AngleActStateVect.at(size-1).Angle; //�����һ����û�����ģ���ô�߽��Ϊ���һ��
                ACTSTATE tmp2=AngleActStateVect.at(size-1);
                AngleActStateVect.clear();
                AngleActStateVect.append(tmp2);
                emit SIG_testresult(1);
                OutPhase=OutPhase+StepPhase;
                if(OutPhase>(StartPhase+360)||OutPhase<(StartPhase-360))//2016-6-8
                {
                    //ClearLED();
                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                    starttest=0;
                    emit SIG_DisplayChangeValue(0,OutPhase);
                    return;
                }
                artExec(fd,"out:dig:on(0x08)",result,0);

                OutputSequencer(fd);
                emit SIG_DisplayChangeValue(1,OutPhase);
                SearchType=0;//��ʱ��
                }
                else  if(AngleActStateVect.at(0).Triped==1)  //�����һ����������ô���෴�ķ�������
                {

                    int size=AngleActStateVect.size();
                    Angle1=AngleActStateVect.at(size-2).Angle;  //�����ڶ���Ϊ�߽��1
                    ACTSTATE tmp2=AngleActStateVect.at(0);
                    AngleActStateVect.clear();
                    AngleActStateVect.append(tmp2);
                    emit SIG_testresult(1);
                    OutPhase=StartPhase-StepPhase;
                    if(OutPhase>(StartPhase+360)||OutPhase<(StartPhase-360))//2016-6-8
                    {
                        //ClearLED();
                        GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                        starttest=0;
                        emit SIG_DisplayChangeValue(0,OutPhase);
                        return;
                    }
                    artExec(fd,"out:dig:on(0x08)",result,0);

                    OutputSequencer(fd);
                    emit SIG_DisplayChangeValue(1,OutPhase);
                    SearchType=1;//˳ʱ��
                }
               }
               else              //�ҵ��ڶ����߽��,��� �����˳�
               {
                int size=AngleActStateVect.size();
                float SecondAngle;
                SecondAngle=AngleActStateVect.at(size-2).Angle;
                Angle2=SecondAngle;
                SearchType=0;
                save_f=1;
                GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                //ClearLED();
                starttest=0;
                emit SIG_testresult(2);
                AngleActStateVect.clear();
               }
           }
           else  //û���ҵ��������
           {       if(SearchType==0)
                   OutPhase=OutPhase+StepPhase;
                   else
                   OutPhase=OutPhase-StepPhase;
               if(OutPhase>(StartPhase+360)||OutPhase<(StartPhase-360))//2016-6-8
               {
                   GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                   //ClearLED();
                   starttest=0;
                   emit SIG_DisplayChangeValue(0,OutPhase);
                   return;
               }
                   artExec(fd,"out:dig:on(0x08)",result,0);

                   OutputSequencer(fd);
                   emit SIG_DisplayChangeValue(1,OutPhase);
           }



         }

       if(t4==1)
         {//����

           output_signal_end(fd);
           ACTSTATE tmp;
           tmp.Triped=1;
           tmp.Angle=OutPhase;
           AngleActStateVect.append(tmp);
           if(OutPhase==StartPhase)
           {
               if(SearchType==0)
               OutPhase=OutPhase+StepPhase;
               else
               OutPhase=OutPhase-StepPhase;
               if(OutPhase>(StartPhase+360)||OutPhase<(StartPhase-360))//2016-6-8
               {
                   //ClearLED();
                   GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                   starttest=0;
                   emit SIG_DisplayChangeValue(0,OutPhase);
                   return;
               }
             artExec(fd,"out:dig:on(0x08)",result,0);

             OutputSequencer(fd);
             emit SIG_DisplayChangeValue(1,OutPhase);
             return;
           }


           if(CheckAngle(AngleActStateVect,1)!=-1)  //�ҵ��˵�һ���߽�Ƕ�
           {
               if(Angle1==-1000)  //�ҵ���һ���߽�Ƕ�  ,��������һ���߽��
               {
                if(AngleActStateVect.at(0).Triped==0)   //  //�����һ��û�ж�������ô������ǰ����
                {
                int size=AngleActStateVect.size();
                Angle1=AngleActStateVect.at(size-1).Angle; //�����һ����û�����ģ���ô�߽��Ϊ���һ��
                ACTSTATE tmp2=AngleActStateVect.at(size-1);
                AngleActStateVect.clear();
                AngleActStateVect.append(tmp2);
                emit SIG_testresult(1);
                OutPhase=OutPhase+StepPhase;
                if(OutPhase>(StartPhase+360)||OutPhase<(StartPhase-360))//2016-6-8
                {
                    //ClearLED();
                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                    starttest=0;
                    emit SIG_DisplayChangeValue(0,OutPhase);
                    return;
                }
                artExec(fd,"out:dig:on(0x08)",result,0);

                OutputSequencer(fd);
                emit SIG_DisplayChangeValue(1,OutPhase);
                SearchType=0;
                }
                else  if(AngleActStateVect.at(0).Triped==1)  //�����һ����������ô���෴�ķ�������
                {

                    int size=AngleActStateVect.size();
                    Angle1=AngleActStateVect.at(size-2).Angle;  //�����ڶ���Ϊ�߽��1
                    ACTSTATE tmp2=AngleActStateVect.at(0);
                    AngleActStateVect.clear();
                    AngleActStateVect.append(tmp2);
                    emit SIG_testresult(1);
                    OutPhase=StartPhase-StepPhase;
                    if(OutPhase>(StartPhase+360)||OutPhase<(StartPhase-360))//2016-6-8
                    {
                        //ClearLED();
                        GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                        starttest=0;
                        emit SIG_DisplayChangeValue(0,OutPhase);
                        return;
                    }
                    artExec(fd,"out:dig:on(0x08)",result,0);

                    OutputSequencer(fd);
                    emit SIG_DisplayChangeValue(1,OutPhase);
                    SearchType=1;
                }
               }
               else              //�ҵ��ڶ����߽��,��� �����˳�
               {
                int size=AngleActStateVect.size();
                float SecondAngle;
                SecondAngle=AngleActStateVect.at(size-2).Angle;
                Angle2=SecondAngle;
                SearchType=0;
                save_f=1;
                 GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                 //ClearLED();
                starttest=0;
                emit SIG_testresult(2);
                AngleActStateVect.clear();
               }
           }
           else  //û���ҵ��������
           {       if(SearchType==0)
                   OutPhase=OutPhase+StepPhase;
                   else
                   OutPhase=OutPhase-StepPhase;
               if(OutPhase>(StartPhase+360)||OutPhase<(StartPhase-360))//2016-6-8
               {
                   //ClearLED();
                   GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                   starttest=0;
                   emit SIG_DisplayChangeValue(0,OutPhase);
                   return;
               }
                   artExec(fd,"out:dig:on(0x08)",result,0);

                   OutputSequencer(fd);
                   emit SIG_DisplayChangeValue(1,OutPhase);
           }


         }
}

void thread1::key_deal_test_current(int fd)
{
        if(starttest==0)
          {
            emit SIG_Teststate(1);
            AngleActStateVect.clear();
            Angle1=-1000;
            Angle2=-1000;
            SearchType=0;

            //SetLED();
            GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
            emit SIG_testresult(0);
            starttest=1;
            OutPhase=StartPhase;
            SequencerCount=1;
            emit SIG_DisplayChangeValue(1,OutPhase);
            artExec(fd,"out:dig:on(0x08)",result,0);
            OutputSequencer(fd);
            save_f=0;
          }
}

void thread1::key_deal_stop_current(int fd)
{
    if(starttest==1)
         {
           GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
              //ClearLED();
           output_signal_end(fd);
           starttest=0;
         }
}


void thread1::key_response(KEY key)
{

if(key==_NOP)   return;

if(key==_TEST)
{
     if(starttest==0)
     {
        output_signal_end(temprorarydata->UartDev[0].uartfd);
        key_deal_test_current(temprorarydata->UartDev[0].uartfd);
                emit SIG_TestState();
     }
}

else if(key==_STOP)
{
   key_deal_stop_current(temprorarydata->UartDev[0].uartfd);
           emit SIG_TestState();

}


}

void thread1::stoprun(){

    Threadrunstate=false;
}



void thread1::run()
{
    KEY key;
  //�򿪲����ô���
   unsigned char info_flag1,info_flag2,info_flag3,info_flag4;
  temprorarydatainit();
  UartsInit();
  int rc;


  temprorarydata->gpio_fd= open("/dev/em335x_gpio", O_RDWR);
  printf("open file = %d\n", temprorarydata->gpio_fd);

  rc = GPIO_OutEnable( temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
  if(rc < 0)
  {
          printf("GPIO_OutEnable::failed %d\n", rc);
          temprorarydata->gpio_fd=-1;
  }

     //  GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
  //ClearLED();
  online(temprorarydata->UartDev[0].uartfd);

   read_max(temprorarydata->UartDev[0].uartfd);
  artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);



    if((result[7]&0xf0)==0x60)
       { //125V��ѹ��
         MAX_V_VALUEDATA_AC=300;
         MIN_V_VALUEDATA_AC=0;
         MAX_V_VALUEDATA_DC=424;
         MIN_V_VALUEDATA_DC=-424;
       }
      else if((result[7]&0xf0)==0x80)
        { //125V��ѹ��
          MAX_V_VALUEDATA_AC=125;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=176;
          MIN_V_VALUEDATA_DC=-176;
        }
      else if((result[7]&0xf0)==0x00)
        { //75V��ѹ��
          MAX_V_VALUEDATA_AC=75;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=110;
          MIN_V_VALUEDATA_DC=-110;
        }
      if((result[7]&0x0f)==0x01)
        { //30A������
          MAX_I_VALUEDATA_AC=30;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=42;
          MIN_I_VALUEDATA_DC=-42;
        }
      else if((result[7]&0x0f)==0x08)
        { //40A������
          MAX_I_VALUEDATA_AC=40;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=56;
          MIN_I_VALUEDATA_DC=-56;
        }
      else if((result[7]&0x0f)==0x05)
        { //50A������
          MAX_I_VALUEDATA_AC=50;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=70;
          MIN_I_VALUEDATA_DC=-70;
        }

   while(Threadrunstate)
     {
    if(temprorarydata->keypadopenflag==false&&temprorarydata->helpopenflag==false)
     {
     key=GetKey(temprorarydata->UartDev[1].uartfd);
     key_response(key);

     if(SoftStartEndflag==true)
       {
          if(starttest==1)
          {
              key=_STOP;
          }
          else
          {
              key=_TEST;
          }
          key_response(key);
          SoftStartEndflag=false;
       }

     if(starttest==0)
 {
     emit SIG_TestState();//2016-5-17������ϣ�text���л������޸�
     artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //����Ҫ����
     info_flag1=result[8];
     info_flag4=result[9];
     artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
     info_flag2=result[8];
     info_flag3=result[9];
     logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //��Ҫemit

}
else {
     ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);                        //�ڲ��Զ���ʱ��ʱ��������״̬
     artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //����Ҫ����
     info_flag1=result[8];
     info_flag4=result[9];
     artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
     info_flag2=result[8];
     info_flag3=result[9];
     logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //��Ҫemit
 }
 }
    else
    {

        tcflush(temprorarydata->UartDev[1].uartfd, TCIOFLUSH);
        msleep(500);

    }
}

       starttest=0;
       output_signal_end(temprorarydata->UartDev[0].uartfd);

       artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
       close(temprorarydata->UartDev[1].uartfd);
       close(temprorarydata->UartDev[0].uartfd);


}

