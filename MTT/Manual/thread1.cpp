#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <KEY.h>
#include "Config.h"
#include "thread1.h"
#include <QSemaphore>
#include <QDebug>
#include "em335x_drivers.h"
#define LEDNUM GPIO8
int current_language=1;  //1����  2Ӣ��
QSemaphore sem(1);
TEMPRORARYDATA *temprorarydata;
extern float A_SumStepnum;
bool Threadrunstate=true;
bool ManulIsSendcmd=false;
int save_f;

bool SoftStartEndflag=false;
double A_V1ampPerStepError=0;  //电压VA每步变化的误差�
double A_V2ampPerStepError=0;  //电压VB每步变化的误差�
double A_V3ampPerStepError=0;  //电压VB每步变化的误差�
double A_VzampPerStepError=0;  //电压VZ每步变化的误差�

double A_I1ampPerStepError=0;
double A_I2ampPerStepError=0;
double A_I3ampPerStepError=0;

extern double max_output_v_a;   //,max_f_v_a=50,min_f_v_a=0.5,output_offset_v_a=0,amp_v_a=360;
extern double max_output_v_b;   //,max_f_v_b=50,min_f_v_b=0.5,output_offset_v_b=0,amp_v_b=120;
extern double max_output_v_c;   //,max_f_v_c=50,min_f_v_c=0.5,output_offset_v_c=0,amp_v_c=240;
extern double max_output_v_z;
extern double max_output_i_a;//,max_f_i_a=50,min_f_i_a=0.5,output_offset_i_a=0,amp_i_a=75;
extern double max_output_i_b;//,max_f_i_b=50,min_f_i_b=0.5,output_offset_i_b=0,amp_i_b=195;
extern double max_output_i_c;//,max_f_i_c=50,min_f_i_c=0.5,output_offset_i_c=0,amp_i_c=315;

float ChangeV1Value,ChangeV1Phase,ChangeV1Frec;
float ChangeV2Value,ChangeV2Phase,ChangeV2Frec;
float ChangeV3Value,ChangeV3Phase,ChangeV3Frec;
float ChangeVzValue,ChangeVzPhase,ChangeVzFrec;
float ChangeI1Value,ChangeI1Phase,ChangeI1Frec;
float ChangeI2Value,ChangeI2Phase,ChangeI2Frec;
float ChangeI3Value,ChangeI3Phase,ChangeI3Frec;

float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
int   T4;
float TripValue,TripTime;
int StartTestFlag;
int   sys[4]={0,0,0,0};
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
//AM33E chuankou  s2  s1
//MPF300 chuankou  S1  S3
    
    memset(temprorarydata,0,sizeof(temprorarydata));
    int i=0;
    {
    temprorarydata->UartDev[i].valid=1;
    temprorarydata->UartDev[i].uartfd=0;
    temprorarydata->UartDev[i].baud=19200;
    temprorarydata->UartDev[i].datanum=8;
    temprorarydata->UartDev[i].parity='N';
    temprorarydata->UartDev[i].stopbit=1;
    sprintf(temprorarydata->UartDev[i].describ,"/dev/ttyS2");
    sprintf(temprorarydata->UartDev[i].devname,"/dev/ttyS2");
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

    //串口接收队列
    for(i=0;i<MAX_UARTDEV_NUM;i++)
    {
    temprorarydata->UartRcv[i].readsequence=0;
    temprorarydata->UartRcv[i].writesequence=0;
    }
    temprorarydata->keypadopenflag=false;
    temprorarydata->helpopenflag=false;
    temprorarydata->Warningopenflag=false;



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
        return -1;//设置失败
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
    switch (databits) /*?è??????????*/
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

    options.c_cflag |= (PARODD | PARENB); /* ?è???????§?é*/
    options.c_iflag |= INPCK;             /* Enable parity checking */
    break;
    case 'e':
    case 'E':
    options.c_cflag |= PARENB;     /* Enable parity */
    options.c_cflag &= ~PARODD;   /* ×????????§?é*/
    options.c_iflag |= INPCK;       /* Enable parity checking */
    break;
    case 'M':
    case 'm':
    options.c_cflag |= PARENB;
    options.c_cflag |= CMSPAR;     /*×?????Mark ???é */
    options.c_iflag &= ~INPCK;     /* Disnable parity checking */
    break;
    default:
    fprintf(stderr,"Unsupported parity\n");
    return (0);
    }
    /* ?è????????*/
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

void str2STR(char *str)  //命令字符全转大写
{
    while(*str)
    {
    if(*str>='a' && *str<='z')
    *str-=0x20;
    str++;
    }
}


void thread1::Manualoutput()
{
 sem.acquire(1);
 ManualOutputFlag=true;
 sem.release(1);


}



int thread1::Buzzer(char Address1,char Address2,char Address3)
  {//sys[1]:过载，sys[2]:间歇，sys[3]:闭锁
    if(sys[1]==1)
      {//过载信息
        if((Address1&0x08)!=0x00&&m_Ch[4].Output==1)
          return 1;//I1过载
        if((Address1&0x02)!=0x00&&m_Ch[5].Output==1)
          return 1;//I2过载
        if((Address2&0x80)!=0x00&&m_Ch[6].Output==1)
          return 1;//I3过载
        if((Address2&0x10)!=0x00&&m_Ch[0].Output==1)
          return 1;//V1过载
        if((Address2&0x02)!=0x00&&m_Ch[1].Output==1)
          return 1;//V2过载
        if((Address3&0x40)!=0x00&&m_Ch[2].Output==1)
          return 1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    else if(sys[1]==0)
      {//过载信息
        if((Address1&0x08)!=0x00&&m_Ch[4].Output==1)
          return -1;//I1过载
        if((Address1&0x02)!=0x00&&m_Ch[5].Output==1)
          return -1;//I2过载
        if((Address2&0x80)!=0x00&&m_Ch[6].Output==1)
          return -1;//I3过载
        if((Address2&0x10)!=0x00&&m_Ch[0].Output==1)
          return -1;//V1过载
        if((Address2&0x02)!=0x00&&m_Ch[1].Output==1)
          return -1;//V2过载
        if((Address3&0x40)!=0x00&&m_Ch[2].Output==1)
          return -1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    if(sys[2]==1)
      {//间歇信息
        if((Address1&0x10)!=0x00)
          return 1;//I1间歇
        if((Address1&0x04)!=0x00)
          return 1;//I2间歇
        if((Address1&0x01)!=0x00)
          return 1;//I3间歇
        if((Address2&0x08)!=0x00)
          return 1;//V1间歇
        if((Address2&0x04)!=0x00)
          return 1;//V2间歇
        if((Address3&0x80)!=0x00)
          return 1;//V3间歇
      }
    else if(sys[2]==0)
      {//间歇信息
        if((Address1&0x10)!=0x00)
          return -1;//I1间歇
        if((Address1&0x04)!=0x00)
          return -1;//I2间歇
        if((Address1&0x01)!=0x00)
          return -1;//I3间歇
        if((Address2&0x08)!=0x00)
          return -1;//V1间歇
        if((Address2&0x04)!=0x00)
          return -1;//V2间歇
        if((Address3&0x80)!=0x00)
          return -1;//V3间歇
      }
    if(sys[3]==1)
      {//闭锁信息
        if((Address1&0x80)!=0x00)
          return 1;//I1闭锁
        if((Address1&0x40)!=0x00)
          return 1;//I2闭锁
        if((Address1&0x20)!=0x00)
          return 1;//I3闭锁
        if((Address2&0x20)!=0x00)
          return 1;//V1闭锁
        if((Address2&0x40)!=0x00)
          return 1;//V2闭锁
        if((Address2&0x01)!=0x00)
          return 1;//V3闭锁
      }
    else if(sys[3]==0)
      {//闭锁信息
        if((Address1&0x80)!=0x00)
          return -1;//I1闭锁
        if((Address1&0x40)!=0x00)
          return -1;//I2闭锁
        if((Address1&0x20)!=0x00)
          return -1;//I3闭锁
        if((Address2&0x20)!=0x00)
          return -1;//V1闭锁
        if((Address2&0x40)!=0x00)
          return -1;//V2闭锁
        if((Address2&0x01)!=0x00)
          return -1;//V3闭锁
      }
    return 0;
  }


void thread1::logic_info(int fd,char info_result1,char info_result2,char info_result3,char info_result4)
  {
     //info_result1为第1路地址的DB8-DB15，info_result2为第2路地址的DB8-DB15，info_result3为第2路地址的DB0-DB7�

  //  printf("%s   %d\n",__func__,__LINE__);

    static int info_count=1;
    int  k1=0;
    //蜂鸣�
    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);
    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //不需要控�
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
    if(Buzzer(info_result1,info_result2,info_result3)==0) //不需要控�
      {
        //运行正常
          emit SIG_dspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&m_Ch[4].Output==1)
          {//info_result1是第1路地址的DB9~DB15�
           emit SIG_dspstate(1);
            //IA过载
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
            emit SIG_dspstate(2);

            //IABC 温度保护
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {   
            //IC间歇
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
        if((info_result1&0x02)==0x02&&m_Ch[5].Output==1)
          {  //info_result1是第1路地址的DB9~DB15�

              emit SIG_dspstate(4);
            //IB过载
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1是第1路地址的DB8~DB15�

              emit SIG_dspstate(5);
           //IC闭锁
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1是第1路地址的DB9~DB15�
              emit SIG_dspstate(6);
          //IA间歇
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
        if((info_result2&0x80)==0x80&&m_Ch[6].Output==1)
          { //info_result2是第2路地址的DB9~DB15�
           //IC过载
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1是第1路地址的DB8~DB15�
          //IABC 电源保护
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1是第1路地址的DB9~DB15�
              emit SIG_dspstate(9);
          //I2间歇  ?温度保护间歇
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
        if((info_result2&0x10)==0x10&&m_Ch[0].Output==1)
          {//info_result2是第2路地址的DB9~DB15�
           //VA过载
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15�

           ;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7�

              //V3间歇
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
        if((info_result2&0x02)==0x02&&m_Ch[1].Output==1)
          {//info_result2是第2路地址的DB9~DB15�
            //V2过载
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2是第2路地址的DB8~DB15� //DB8
           // VABC 温度保护
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2是第2路地址的DB9~DB15�
            emit SIG_dspstate(15);
           //VA间歇
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
        if((info_result3&0x40)==0x40&&m_Ch[2].Output==1)
          { //info_result3是第2路地址的DB0~DB7�

             emit SIG_dspstate(16);
            //VC过载
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2是第2路地址的DB8~DB15� //DB13
            emit SIG_dspstate(17);
            //VABC电源保护
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2是第2路地址的DB8~DB15� //DB13
            emit SIG_dspstate(18);
            //VABC电源保护
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
          {//info_result3是第2路地址的DB0~DB7�
            emit SIG_dspstate(19);

             //直流过载
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15�
          emit SIG_dspstate(20);

            // VABC 温度保护
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7�
            emit SIG_dspstate(21);
           //V3间歇
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

void thread1::read_logic(int fd) //需要emit
 {
    
    unsigned char signature;
    artExec(fd,"inp:buf:get?",result,15);
    signature=int(result[13]&0x7f);
    switch(signature)
      {
        case 1:
          break;
        case 2:
          break;
        case 3:
          break;
        case 4:
          emit  SIG_testresult(false);
          save_f=1;
          break;
        case 5:
          break;
      }
  }

void thread1::ReadAutologic(int fd) //2016-6-3  zidong
 {
    int t2=0,t3=0,t5=0;
    int stepnum=0,tripstepnum=0;
    unsigned char signature;
    artExec(fd,"inp:buf:get?",result,15);
    signature=int(result[13]&0x7f);
    switch(signature)
      {
        case 1:
          break;
        case 2:
          t2=1;
          break;
        case 3:
          t3=1;
          break;
        case 4:
        emit  SIG_testresult(false);
        save_f=1;
          break;
        case 5:
        t5=1;
          break;
      }

    if(t2==1)
    {
        artExec(fd,"seq:status?(step)",result,16);  //�个字�
        stepnum=result[9]*256+result[10];
        tripstepnum=result[11]*256+result[12];
        A_SumStepnum=tripstepnum;

        switch(A_Variable)
          {
            case 1:
              if(m_Ch[0].checked==1)
                {
                  ChangeV1Value=m_Ch[0].Amp+A_VolStep*stepnum;
                }
              if(m_Ch[1].checked==1)
                {
                    ChangeV2Value=m_Ch[1].Amp+A_VolStep*stepnum;
                }
              if(m_Ch[2].checked==1)
                {//V3瑕佸彉鍖?
                  ChangeV3Value=m_Ch[2].Amp+A_VolStep*stepnum;
                }
/*
              if(m_Ch[3].checked==1)
                {//V3瑕佸彉鍖?
                  ChangeVzValue=m_Ch[3].Amp+A_VolStep*stepnum;
                }
*/


              if(m_Ch[4].checked==1)
                {//I1瑕佸彉鍖?
                  ChangeI1Value=m_Ch[4].Amp+A_CurStep*stepnum;
                }
              if(m_Ch[5].checked==1)
                {//I2瑕佸彉鍖?
                  ChangeI2Value=m_Ch[5].Amp+A_CurStep*stepnum;
                }
              if(m_Ch[6].checked==1)
                {//I3瑕佸彉鍖?
                  ChangeI3Value=m_Ch[6].Amp+A_CurStep*stepnum;
                }
              break;

        case 2:
          if(m_Ch[0].checked==1)
            {
              ChangeV1Phase=m_Ch[0].Phase+A_PhaStep*stepnum;
            }
          if(m_Ch[1].checked==1)
            {
                ChangeV2Phase=m_Ch[1].Phase+A_PhaStep*stepnum;
            }
          if(m_Ch[2].checked==1)
            {//V3瑕佸彉鍖?
              ChangeV3Phase=m_Ch[2].Phase+A_PhaStep*stepnum;
            }
/*
          if(m_Ch[3].checked==1)
            {//V3瑕佸彉鍖?
              ChangeVzPhase=m_Ch[3].Phase+A_PhaStep*stepnum;
            }
*/


          if(m_Ch[4].checked==1)
            {//I1瑕佸彉鍖?
              ChangeI1Phase=m_Ch[4].Phase+A_PhaStep*stepnum;
            }
          if(m_Ch[5].checked==1)
            {//I2瑕佸彉鍖?
              ChangeI2Phase=m_Ch[5].Phase+A_PhaStep*stepnum;
            }
          if(m_Ch[6].checked==1)
            {//I3瑕佸彉鍖?
              ChangeI3Phase=m_Ch[6].Phase+A_PhaStep*stepnum;
            }
          break;

        case 3:
          if(m_Ch[0].checked==1)
            {
              ChangeV1Frec=m_Ch[0].Freq+A_FreStep*stepnum;
            }
          if(m_Ch[1].checked==1)
            {
              ChangeV2Frec=m_Ch[1].Freq+A_FreStep*stepnum;
            }
          if(m_Ch[2].checked==1)
            {//V3瑕佸彉鍖?
              ChangeV3Frec=m_Ch[2].Freq+A_FreStep*stepnum;
            }
/*
          if(m_Ch[3].checked==1)
            {//V3瑕佸彉鍖?
              ChangeVzFrec=m_Ch[3].Freq+A_FreStep*stepnum;
            }
*/


          if(m_Ch[4].checked==1)
            {//I1瑕佸彉鍖?
              ChangeI1Frec=m_Ch[4].Freq+A_FreStep*stepnum;
            }
          if(m_Ch[5].checked==1)
            {//I2瑕佸彉鍖?
              ChangeI2Frec=m_Ch[5].Freq+A_FreStep*stepnum;
            }
          if(m_Ch[6].checked==1)
            {//I3瑕佸彉鍖?
              ChangeI3Frec=m_Ch[6].Freq+A_FreStep*stepnum;
            }
          break;
          }

        emit SIG_DisplayAutoChangeValue(1);
    }
    if(t3==1)
      {   //未动�
    TripTime=-1;
    TripValue=-1;
      }
    if(t5=1)
    {
//        output_signal_end(fd);
//        starttest=0;
//        ClearLED();
//        emit SIG_DisplayAutoChangeValue(0);
//        save_f=1;
    }
}

void  thread1::ReadA_TripTimeLogic(int fd)//读测试动作时间的接点变化信息
  {


    int t3=0,t4=0,t5=0,t2=0;
    unsigned char signature;
    double u,v,w;
    static double SequencerTime2;
    static double SequencerTime4;
    float Temp;

    artExec(fd,"inp:buf:get?",result,15);//从DSP得到开关量接点信息�

    u=result[7];
    v=result[8];
    w=result[9];
    signature=int(result[13]&0x7f);
    switch(signature)
      {
        case 1:
          break;
        case 2:
          t2=1;
          SequencerTime2=gettime(u,v,w);   //计算从开始测试到sequencer的Step2的时间�
          break;
        case 3:
          t3=1;
          break;
        case 4:
          t4=1;
          SequencerTime4=gettime(u,v,w);  //计算从开始测试到sequencer的Step4的时间�
          break;
        case 5:
          t5=1;
          break;
      }


    if(t3==1)
      {   //未动�
    TripTime=-1;
    TripValue=-1;
      }
    if(t4==1)
      {//动作
        if(A_TestMode==3)
       { TripTime=SequencerTime4-SequencerTime2-faultduration-logic_delaytime/1000;
         //A_SumStepnum=floor((SequencerTime4-SequencerTime2-faultduration)/A_StepTime);
         //A_SumStepnum=(int)A_SumStepnum;
         TripTime=TripTime-floor(TripTime/A_StepTime)*A_StepTime;
       }
        else if(A_TestMode==2)
        {
         TripTime=SequencerTime4-SequencerTime2-logic_delaytime/1000;
        }

        Temp=TripTime*1000;
        Temp=ceil(Temp);
        TripTime=Temp/1000;//将返回值的限制在小数点后最多只有三�
       }
     if(t5==1)
       {
        output_signal_end(fd);
        GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //�
        //ClearLED();
        starttest=0;
        save_f=1;
        emit SIG_Teststate(0);
        if(A_TestMode==2)
        {

        emit SIG_testresult(false);
        }
        else if(A_TestMode==3)
        {
        emit SIG_DisplayAutoChangeValue(0);
        emit SIG_testresult(false);
        }
      }



  }

int  thread1::checkAutoTestEndvalueValid()
{

    float max_v_valuedata,max_i_valuedata;


    if(m_Ch[0].DC==0||m_Ch[1].DC==0||m_Ch[2].DC==0||m_Ch[3].DC==0||m_Ch[4].DC==0||m_Ch[5].DC==0||m_Ch[6].DC==0)
      {
        max_v_valuedata=MAX_V_VALUEDATA_AC;
        max_i_valuedata=MAX_I_VALUEDATA_AC;

      }
    else if(m_Ch[0].DC==1||m_Ch[1].DC==1||m_Ch[2].DC==1||m_Ch[3].DC==1||m_Ch[4].DC==1||m_Ch[5].DC==1||m_Ch[6].DC==1)
      {
        max_v_valuedata=MAX_V_VALUEDATA_DC;
        max_i_valuedata=MAX_I_VALUEDATA_DC;

      }

 switch(A_Variable)
 {
 case 1:
        if(m_Ch[0].checked==1)
       {
           if(A_VolStep>=0)
           {
               switch(m_Ch[0].DC)
               {
                 case 0:  //VA  交流
                   if(m_Ch[0].Amp+A_StepNum*A_VolStep>MAX_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(1,1);
                                   return -1;
                   }
                   break;
                 case 1:  //VA  直流
                   if(m_Ch[0].Amp+A_StepNum*A_VolStep>MAX_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(1,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(A_VolStep<0)
           {
               switch(m_Ch[0].DC)
               {
                 case 0:  //VA  交流
                   if(m_Ch[0].Amp+A_StepNum*A_VolStep<MIN_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(1,2);
                                      return -1;
                   }
                   break;
                 case 1:  //VA  直流
                   if(m_Ch[0].Amp+A_StepNum*A_VolStep<MIN_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(1,2);
                                      return -1;
                   }
                   break;
               }
           }
       }


        if(m_Ch[1].checked==1)
       {
           if(A_VolStep>=0)
           {
               switch(m_Ch[1].DC)
               {
                 case 0:  //VB  交流
                   if(m_Ch[1].Amp+A_StepNum*A_VolStep>MAX_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(2,1);
                                   return -1;
                   }
                   break;
                 case 1:  //VB  直流
                   if(m_Ch[1].Amp+A_StepNum*A_VolStep>MAX_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(2,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(A_VolStep<0)
           {
               switch(m_Ch[1].DC)
               {
                 case 0:  //VB  交流
                   if(m_Ch[1].Amp+A_StepNum*A_VolStep<MIN_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(2,2);
                                      return -1;
                   }
                   break;
                 case 1:  //VB  直流
                   if(m_Ch[1].Amp+A_StepNum*A_VolStep<MIN_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(2,2);
                                      return -1;
                   }
                   break;
               }
           }
       }
        if(m_Ch[2].checked==1)
       {
           if(A_VolStep>=0)
           {
               switch(m_Ch[2].DC)
               {
                 case 0:  //VC  交流
                   if(m_Ch[2].Amp+A_StepNum*A_VolStep>MAX_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(3,1);
                                   return -1;
                   }
                   break;
                 case 1:  //VC  直流
                   if(m_Ch[2].Amp+A_StepNum*A_VolStep>MAX_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(3,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(A_VolStep<0)
           {
               switch(m_Ch[2].DC)
               {
                 case 0:  //VC  交流
                   if(m_Ch[2].Amp+A_StepNum*A_VolStep<MIN_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(3,2);
                                      return -1;
                   }
                   break;
                 case 1:  //VC  直流
                   if(m_Ch[2].Amp+A_StepNum*A_VolStep<MIN_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(3,2);
                                      return -1;
                   }
                   break;
               }
           }
       }
        if(m_Ch[3].checked==1)
       {
           if(A_VolStep>=0)
           {
               switch(m_Ch[3].DC)
               {
                 case 0:  //Vz  交流
                   if(m_Ch[3].Amp+A_StepNum*A_VolStep>MAX_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(4,1);
                                   return -1;
                   }
                   break;
                 case 1:  //Vz  直流
                   if(m_Ch[3].Amp+A_StepNum*A_VolStep>MAX_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(4,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(A_VolStep<0)
           {
               switch(m_Ch[3].DC)
               {
                 case 0:  //Vz  交流
                   if(m_Ch[3].Amp+A_StepNum*A_VolStep<MIN_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(4,2);
                                      return -1;
                   }
                   break;
                 case 1:  //Vz  直流
                   if(m_Ch[3].Amp+A_StepNum*A_VolStep<MIN_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(4,2);
                                      return -1;
                   }
                   break;
               }
           }
       }

        if(m_Ch[4].checked==1)
       {
           if(A_CurStep>=0)
           {
               switch(m_Ch[4].DC)
               {
                 case 0:  //IA  交流
                   if(m_Ch[4].Amp+A_StepNum*A_CurStep>MAX_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(5,1);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  直流
                   if(m_Ch[4].Amp+A_StepNum*A_CurStep>MAX_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(5,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(A_CurStep<0)
           {
               switch(m_Ch[4].DC)
               {
                 case 0:  //IA  交流
                   if(m_Ch[4].Amp+A_StepNum*A_CurStep<MIN_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(5,2);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  直流
                   if(m_Ch[4].Amp+A_StepNum*A_CurStep<MIN_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(5,2);
                                   return -1;
                   }
                   break;
               }
           }
       }

        if(m_Ch[5].checked==1)
       {
           if(A_CurStep>=0)
           {
               switch(m_Ch[5].DC)
               {
                 case 0:  //IA  交流
                   if(m_Ch[5].Amp+A_StepNum*A_CurStep>MAX_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(6,1);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  直流
                   if(m_Ch[5].Amp+A_StepNum*A_CurStep>MAX_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(6,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(A_CurStep<0)
           {
               switch(m_Ch[5].DC)
               {
                 case 0:  //IA  交流
                   if(m_Ch[5].Amp+A_StepNum*A_CurStep<MIN_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(6,2);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  直流
                   if(m_Ch[5].Amp+A_StepNum*A_CurStep<MIN_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(6,2);
                                   return -1;
                   }
                   break;
               }
           }
       }
        if(m_Ch[6].checked==1)
       {
           if(A_CurStep>=0)
           {
               switch(m_Ch[6].DC)
               {
                 case 0:  //IA  交流
                   if(m_Ch[6].Amp+A_StepNum*A_CurStep>MAX_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(7,1);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  直流
                   if(m_Ch[6].Amp+A_StepNum*A_CurStep>MAX_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(7,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(A_CurStep<0)
           {
               switch(m_Ch[6].DC)
               {
                 case 0:  //IA  交流
                   if(m_Ch[6].Amp+A_StepNum*A_CurStep<MIN_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(7,2);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  直流
                   if(m_Ch[6].Amp+A_StepNum*A_CurStep<MIN_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(7,2);
                                   return -1;
                   }
                   break;
               }
           }
       }
     break;


 case 2:

     break;

 case 3:
            if(m_Ch[0].checked==1)
            {
                if(A_FreStep>=0)
                {
                if((m_Ch[0].Freq+A_StepNum*A_FreStep)>1000)
              {  emit SIG_SendWarning(1,1);
                return -1;
              }
                }
                else  if(A_FreStep<0)
                {
                    if((m_Ch[0].Freq+A_StepNum*A_FreStep)<0)
                   { emit SIG_SendWarning(1,2);
                    return -1;
                   }
                }
            }


             if(m_Ch[1].checked==1)
            {
                if(A_FreStep>=0)
                {
                if((m_Ch[1].Freq+A_StepNum*A_FreStep)>1000)
               { emit SIG_SendWarning(2,1);
                return -1;
               }
                }
                else  if(A_FreStep<0)
                {
                    if((m_Ch[1].Freq+A_StepNum*A_FreStep)<0)
                   { emit SIG_SendWarning(2,2);
                     return -1;
                   }
                }
            }
             if(m_Ch[2].checked==1)
            {
                if(A_FreStep>=0)
                {
                if((m_Ch[2].Freq+A_StepNum*A_FreStep)>1000)
               { emit SIG_SendWarning(3,1);
                 return -1;
               }
                }
                else  if(A_FreStep<0)
                {
                    if((m_Ch[2].Freq+A_StepNum*A_FreStep)<0)
                  {  emit SIG_SendWarning(3,2);
                    return -1;
                  }
                }
            }
             if(m_Ch[3].checked==1)
            {
                if(A_FreStep>=0)
                {
                if((m_Ch[3].Freq+A_StepNum*A_FreStep)>1000)
              {  emit SIG_SendWarning(4,1);
                return -1;
              }
                }
                else  if(A_FreStep<0)
                {
                    if((m_Ch[3].Freq+A_StepNum*A_FreStep)<0)
                   { emit SIG_SendWarning(4,2);
                    return -1;
                   }
                }
            }

             if(m_Ch[4].checked==1)
            {
                if(A_FreStep>=0)
                {
                if((m_Ch[4].Freq+A_StepNum*A_FreStep)>1000)
              {  emit SIG_SendWarning(5,1);
                return -1;
              }
                }
                else  if(A_FreStep<0)
                {
                    if((m_Ch[4].Freq+A_StepNum*A_FreStep)<0)
                  {  emit SIG_SendWarning(5,2);
                    return -1;
                  }
                }
            }

             if(m_Ch[5].checked==1)
            {
                if(A_FreStep>=0)
                {
                if((m_Ch[5].Freq+A_StepNum*A_FreStep)>1000)
               { emit SIG_SendWarning(6,1);
                return -1;
               }
                }
                else  if(A_FreStep<0)
                {
                    if((m_Ch[5].Freq+A_StepNum*A_FreStep)<0)
                   { emit SIG_SendWarning(6,2);
                    return -1;
                   }
                }
            }
             if(m_Ch[6].checked==1)
            {
                if(A_FreStep>=0)
                {
                if((m_Ch[6].Freq+A_StepNum*A_FreStep)>1000)
              {  emit SIG_SendWarning(7,1);
                return -1;
              }
                }
                else  if(A_FreStep<0)
                {
                    if((m_Ch[6].Freq+A_StepNum*A_FreStep)<0)
                   { emit SIG_SendWarning(7,2);
                    return -1;
                   }
                }
            }
     break;
 }

 return 0;



}


void thread1::CaculateError(){

    float y;
    y=chang_amp(A_VolStep,&max_output_v_a);
    A_V1ampPerStepError=A_VolStep-(double)(max_output_v_a*y/32767);

    y=chang_amp(A_VolStep,&max_output_v_b);
    A_V2ampPerStepError=A_VolStep-(double)(max_output_v_b*y/32767);

    y=chang_amp(A_VolStep,&max_output_v_c);
    A_V3ampPerStepError=A_VolStep-(double)(max_output_v_c*y/32767);


    y=chang_amp(A_VolStep,&max_output_v_z);
    A_VzampPerStepError=A_VolStep-(double)(max_output_v_z*y/32767);


    y=chang_amp(A_CurStep,&max_output_i_a);
    A_I1ampPerStepError=A_CurStep-(double)(max_output_i_a*y/32767);

    y=chang_amp(A_CurStep,&max_output_i_b);
    A_I2ampPerStepError=A_CurStep-(double)(max_output_i_b*y/32767);

    y=chang_amp(A_CurStep,&max_output_i_c);
    A_I3ampPerStepError=A_CurStep-(double)(max_output_i_c*y/32767);

}




void thread1::key_deal_test_manual(int fd)
{

    if(starttest==0)
    {
        save_f=0;
       emit SIG_Teststate(1);
       if(A_TestMode==1)
           {
               if(A_Variable==2)
               {
                 m_Ch[0].checked=0;
                 emit SIG_setVanocheck();
               }

              GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);//�
              //SetLED();
              emit SIG_testresult(true);

              ChangeOutputValue();              
              artExec(fd,"out:dig:on(0x10)",result,0);
              artExec(fd,"out:dig:on(0x08)",result,0);
              output_signal_manual(0,fd);
              StartTestFlag=1;
              starttest=1;
              T4=0;

           }
        else if(A_TestMode==2)
          {

             GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);//�
              //SetLED();
              emit SIG_testresult(true);

              ChangeOutputValue();              
              artExec(fd,"out:dig:on(0x10)",result,0);
              artExec(fd,"out:dig:on(0x08)",result,0);
              output_signal_manual(2,fd);
              starttest=1;
              T4=0;

          }

        else if(A_TestMode==3)
          {
              if(A_Variable==2)
              {
                m_Ch[0].checked=0;
                emit  SIG_setVanocheck();
              }


              if(checkAutoTestEndvalueValid()==-1)
              {

                  temprorarydata->Warningopenflag=true;
                  return;

              }


              GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);//�
              //SetLED();
              emit SIG_testresult(true);

              ChangeOutputValue();
              ////检查参数是否超出了边界
              artExec(fd,"out:dig:on(0x30)",result,0);
              artExec(fd,"out:dig:on(0x08)",result,0);
              Output_signal_Auto(fd);
              starttest=1;
              T4=0;

          }




    }
  }


void thread1::key_deal_stop_manual(int fd)
  {
    if(starttest==1)
      {
        output_signal_end(fd);
        starttest=0;
        GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //�
        //ClearLED();
        emit SIG_Teststate(0);
        emit SIG_DisplayAutoChangeValue(0);
      }
  }



void thread1::key_response(KEY key)
{

if(key==_NOP) return;

if(key==_TEST)
{


     if(starttest==0)
     {

        output_signal_end(temprorarydata->UartDev[0].uartfd);
        key_deal_test_manual(temprorarydata->UartDev[0].uartfd);
        CaculateError();
        emit SIG_TestState();
     }

}

else if(key==_STOP)
{
     GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
     //ClearLED();
     key_deal_stop_manual(temprorarydata->UartDev[0].uartfd);
      emit SIG_TestState();
     ManualOutputFlag=false;
}


}


thread1::thread1(QObject *parent) :
    QThread(parent)
{
    temprorarydata=new TEMPRORARYDATA;
    ManualOutputFlag=false;
}


void thread1::run()
{

   unsigned char info_flag1,info_flag2,info_flag3,info_flag4;
   KEY key;
   int info_num=0;
     //打开并配置串�

   temprorarydatainit();
   UartsInit();

   temprorarydata->gpio_fd=open("/dev/em335x_gpio", O_RDWR);
   int rc = GPIO_OutEnable(temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
   if(rc < 0)
   {
       printf("GPIO_OutEnable::failed %d\n", rc);
       temprorarydata->gpio_fd=-1;
   }


online(temprorarydata->UartDev[0].uartfd);

    read_max(temprorarydata->UartDev[0].uartfd);

    artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31); 

    if((result[7]&0xf0)==0x60)
         { //300V电压�
           MAX_V_VALUEDATA_AC=300;
           MIN_V_VALUEDATA_AC=0;
           MAX_V_VALUEDATA_DC=424;
           MIN_V_VALUEDATA_DC=-424;
         }
        else if((result[7]&0xf0)==0x80)//
          {
            MAX_V_VALUEDATA_AC=125;
            MIN_V_VALUEDATA_AC=0;
            MAX_V_VALUEDATA_DC=176;
            MIN_V_VALUEDATA_DC=-176;
          }
        else if((result[7]&0xf0)==0x00)
          {
            MAX_V_VALUEDATA_AC=75;
            MIN_V_VALUEDATA_AC=0;
            MAX_V_VALUEDATA_DC=110;

            MIN_V_VALUEDATA_DC=-110;
          }
        if((result[7]&0x0f)==0x01)//
          {
            MAX_I_VALUEDATA_AC=30;
            MIN_I_VALUEDATA_AC=0;
            MAX_I_VALUEDATA_DC=5;
            MIN_I_VALUEDATA_DC=-5;
            //MAX_I_VALUEDATA_AC=30;
            //MIN_I_VALUEDATA_AC=0;
            //MAX_I_VALUEDATA_DC=42;
            //MIN_I_VALUEDATA_DC=-42;
          }
        else if((result[7]&0x0f)==0x08)
          {
            MAX_I_VALUEDATA_AC=40;
            MIN_I_VALUEDATA_AC=0;
            MAX_I_VALUEDATA_DC=20;
            MIN_I_VALUEDATA_DC=-20;
//            MAX_I_VALUEDATA_AC=40;
//            MIN_I_VALUEDATA_AC=0;
//            MAX_I_VALUEDATA_DC=56;
//            MIN_I_VALUEDATA_DC=-56;
          }
        else if((result[7]&0x0f)==0x05)
          {
            MAX_I_VALUEDATA_AC=50;
            MIN_I_VALUEDATA_AC=0;
            MAX_I_VALUEDATA_DC=70;
            MIN_I_VALUEDATA_DC=-70;
          }

while(Threadrunstate)
     {
     if(temprorarydata->keypadopenflag==false&&temprorarydata->helpopenflag==false&&\
             temprorarydata->Warningopenflag==false)
     {
     key=GetKey(temprorarydata->UartDev[1].uartfd);  //这里需要修�
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
       if(info_num++>=10)
     {
          emit SIG_TestState();
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //不需要更�
       info_flag1=result[8];
       info_flag4=result[9];
       artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
       info_flag2=result[8];
       info_flag3=result[9];
       logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //需要emit
       info_num=0;
     }
     }

     else
     {
         if(A_TestMode==1)
         {    read_logic(temprorarydata->UartDev[0].uartfd);  //手动测试没有测试结果
              sem.acquire(1);
             if(ManualOutputFlag==true)
             {

              output_signal_manual(1,temprorarydata->UartDev[0].uartfd);//从新往DSP送一个seq，改变输出的�
              ManualOutputFlag=false;
              ManulIsSendcmd=false;
             }
             sem.release(1);
         }

         else if(A_TestMode==2)
            ReadA_TripTimeLogic(temprorarydata->UartDev[0].uartfd);
         else if(A_TestMode==3)
         {
             ReadAutologic(temprorarydata->UartDev[0].uartfd);
            ReadA_TripTimeLogic(temprorarydata->UartDev[0].uartfd);
         }

           if(info_num++>=20)
           {
             artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);
             info_flag1=result[8];
             info_flag4=result[9];
             artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
             info_flag2=result[8];
             info_flag3=result[9];
             logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);
             info_num=0;
             if(A_TestMode==1&&StartTestFlag==1)
             {
                 artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x08)",result,0);
                 StartTestFlag=0;
             }
           }
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

          //  emit SIG_SendClose();
          //  msleep(100);
}


void thread1::stoprun(){

  Threadrunstate=false;
}


           

