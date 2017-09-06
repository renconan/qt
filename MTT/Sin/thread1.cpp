#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <KEY.h>
#include <QDebug>
#include "Config.h"
#include <QSemaphore>
#include <termios.h>

#include "Testcom2.h"
#include "thread1.h"
#include "Volseq.h"
#include "Defpam.h"
#include "em335x_drivers.h"
const float EPSINON =1.0e-04;
TEMPRORARYDATA *temprorarydata;
#define LEDNUM GPIO8

QSemaphore sem(1);

extern float prefaulttime;

extern float StopValue; //动作时间的用户手动输入的值
extern int   TripTimeType;   //测动作时间的方式
extern float StopValueHoldTime;//动作时间的保持时间

long num=0;

extern int   ACRelayType;

int TripValueSteps=0;
int ReturnSteps=0;

extern bool Threadrunstate;
extern  float Postfaulttime;
extern  int   dcouttype;
extern bool SoftStartEndflag;




extern double V1ampPerStepError;  //电压VA每步变化的误差值
extern double V2ampPerStepError;  //电压VB每步变化的误差值
extern double V3ampPerStepError;  //电压VB每步变化的误差值
extern double VzampPerStepError;  //电压VZ每步变化的误差值

extern double I1ampPerStepError;
extern double I2ampPerStepError;
extern double I3ampPerStepError;

extern double max_output_v_a;   //,max_f_v_a=50,min_f_v_a=0.5,output_offset_v_a=0,amp_v_a=360;
extern double max_output_v_b;   //,max_f_v_b=50,min_f_v_b=0.5,output_offset_v_b=0,amp_v_b=120;
extern double max_output_v_c;   //,max_f_v_c=50,min_f_v_c=0.5,output_offset_v_c=0,amp_v_c=240;
extern double max_output_v_z;
extern double max_output_i_a;//,max_f_i_a=50,min_f_i_a=0.5,output_offset_i_a=0,amp_i_a=75;
extern double max_output_i_b;//,max_f_i_b=50,min_f_i_b=0.5,output_offset_i_b=0,amp_i_b=195;
extern double max_output_i_c;//,max_f_i_c=50,min_f_i_c=0.5,output_offset_i_c=0,amp_i_c=315;

extern float StartValue,EndValue,ActStep,ReturnStep,StepTime;
extern int   ACRelayType,DCRelayType;
extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
float changedate;

float checkifzero(float x)
{

    if ((x >= -EPSINON) && (x <= EPSINON))
        return 0;
    else
        return x;
}


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

    //串口接收队列
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


void delayMS(int ms)
{
    QTime n;
    n=QTime::currentTime();
    QTime now;
      do{
           now=QTime::currentTime();
       }   while (n.msecsTo(now)<=ms);
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




thread1::thread1(QObject *parent) :
    QThread(parent)
{

    temprorarydata=new TEMPRORARYDATA;

}

void thread1::DisplayResult(int ReslutSum)
{
   emit SIG_testresult(ReslutSum);
}


void thread1::ReadAutoLogic(int fd)
{ 
        int t1=0,t2=0,t3=0,t4=0,t5=0;
        unsigned char signature=0;
        static double SequencerTime2,SequencerTime4;
        double u,v,w;
        float VDS,Temp;
        float ChangeStep=0;
        int stepnum=0;
        artExec(fd,"inp:buf:get?",result,15);
        u=result[7];
        v=result[8];
        w=result[9];
        signature=int(result[13]&0x7f);
        switch(signature)
          {
            case 1:
              t1=1;
              break;
            case 2:
              SequencerTime2=gettime(u,v,w);
              t2=1;
              break;
            case 3:
              t3=1;
              break;
            case 4:
              SequencerTime4=gettime(u,v,w);
              t4=1;
              break;
            case 5:
              t5=1;
              break;
          }


       if(t2==1)
       {
           artExec(fd,"seq:status?(step)",result,16);  //加4个字节
           stepnum=result[9]*256+result[10];
           if(stepnum>32767)
               stepnum=32768-stepnum;
           TripValueSteps=result[11]*256+result[12];
           if(TripValueSteps>32767)
               TripValueSteps=32768-TripValueSteps;
           if(TestItem2==1)
           {
                ReturnSteps=result[13]*256+result[14];
                if(ReturnSteps>32767)
                    ReturnSteps=32768-ReturnSteps;
           }


        switch(seq_count)
        {
        case 1:
  /*          if(ACRelayType==1)  // dianya
            {
            if(TestObject==1)
                 ChangeValue=StartValue-(ActStep-V1ampPerStepError)*stepnum;
            else if(TestObject==2)
                 ChangeValue=StartValue+(ActStep-V1ampPerStepError)*stepnum;
            emit SIG_DisplayChangeValue(1,ChangeValue);
            }
            else  //dianliu
            {

                if(TestObject==1)
                     ChangeValue=StartValue-(ActStep-I1ampPerStepError)*stepnum;
                else if(TestObject==2)
                     ChangeValue=StartValue+(ActStep-I1ampPerStepError)*stepnum;
                emit SIG_DisplayChangeValue(1,ChangeValue);
            }
*/
            /*
            if(TripValueSteps!=0&&stepnum==0)
            {
                output_signal_end(fd);
                starttest=0;
                GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                emit SIG_DisplayChangeValue(0,0);
                save_f=1;
            }
            */

            if(TripValueSteps<=stepnum)
            {
                if(TestObject==1)
                     ChangeValue=StartValue-ActStep*(stepnum);
                else if(TestObject==2)
                     ChangeValue=StartValue+ActStep*(stepnum);
                emit SIG_DisplayChangeValue(1,ChangeValue);
                changedate=ChangeValue;
            }
            else if(TripValueSteps>=(fabs(StartValue-EndValue)/ActStep))
            {
                stepnum=fabs(StartValue-EndValue)/ActStep;
                if(TestObject==1)
                     ChangeValue=StartValue-ActStep*(stepnum);
                else if(TestObject==2)
                     ChangeValue=StartValue+ActStep*(stepnum);
                emit SIG_DisplayChangeValue(1,ChangeValue);
                changedate=ChangeValue;
            }
            else
            {
                if(TestObject==2)
                    ChangeValue=changedate-(TripValueSteps-ReturnSteps)*ActStep;
                else if(TestObject==1)
                    ChangeValue=changedate+(TripValueSteps-ReturnSteps)*ActStep;
                emit SIG_DisplayChangeValue(1,ChangeValue);
            }            
            break;

        }
       }


        if(t3==1)
          {
            switch(seq_count)
              {
                case 1:
                  TripTime1=-1;
                  TripTime2=-1;
                  TripTime3=-1;

                  break;
              }
          }


        if(t4==1)
          {
            switch(seq_count)
              {
                case 1:
                  TripTime1=1;                 
                  break;
                case 2:
                  break;
              }
          }
        if(t5==1)
          {
            switch(seq_count)
              {
                case 1:
                  if(TripTime1==1)
                  {

                      if(ACRelayType==1)
                     {

                      if(TestObject==1)
                           TripValue=StartValue-TripValueSteps*(ActStep);
                      else if(TestObject==2)
                           TripValue=StartValue+TripValueSteps*(ActStep);
                     }
                      else
                      {

                          if(TestObject==1)
                               TripValue=StartValue-TripValueSteps*(ActStep);
                          else if(TestObject==2)
                               TripValue=StartValue+TripValueSteps*(ActStep);

                      }

                        //TripValue=TripValue;

                      DisplayResult(1);

                      if(TestItem2==1)
                      {
                          if(ReturnSteps==0)
                            TripTime2=-1;
                          else
                            TripTime2=1;
/*                        if(ACRelayType==1)
                        {
                            if(TestObject==1)
                            {
                                ReturnValue=StartValue-ReturnSteps*(ActStep);
                                if((fabs(MAX_V_VALUEDATA_AC-StartValue)/ActStep)==ReturnSteps)
                                  TripTime2=-1;
                            }
                            else if(TestObject==2)
                            {
                                ReturnValue=StartValue+ReturnSteps*(ActStep);
                                if((fabs(StartValue)/ActStep)==ReturnSteps)
                                    TripTime2=-1;
                            }
                        }
                        else if(ACRelayType==2)
                        {
                            if(TestObject==1)
                            {
                                ReturnValue=StartValue-ReturnSteps*(ActStep);
                                if((fabs(MAX_I_VALUEDATA_AC-StartValue)/ActStep)==ReturnSteps)
                                  TripTime2=-1;
                            }
                            else if(TestObject==2)
                            {
                                ReturnValue=StartValue+ReturnSteps*(ActStep);
                                if((fabs(StartValue)/ActStep)==ReturnSteps)
                                    TripTime2=-1;
                            }
                        }
                        else
                         TripTime2=1;
*/
                       if(ACRelayType==1)
                       { if(TestObject==1)
                          ReturnValue=StartValue-ReturnSteps*(ActStep);
                        else if(TestObject==2)//xiao  ->  da
                          ReturnValue=StartValue+ReturnSteps*(ActStep);
                       }
                        else
                        {
                            if(TestObject==1)
                               ReturnValue=StartValue-ReturnSteps*(ActStep);
                             else if(TestObject==2)
                               ReturnValue=StartValue+ReturnSteps*(ActStep);

                        }

                        DisplayResult(2);
                      }
                  }
                  else
                  {
                      DisplayResult(1);
                  }



                  output_signal_end(fd);
                  starttest=0;
                  ClearLED();
                  emit SIG_DisplayChangeValue(0,0);
                  save_f=1;
                  break;
                case 2:
                  break;
              }
          }

}

int  thread1::Buzzer(char Address1,char Address2,char Address3)
  {//sys[1]:¹ýÔØ£¬sys[2]:ŒäÐª£¬sys[3]:±ÕËø
    if(sys[1]==1)
      {//¹ýÔØÐÅÏ¢
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1¹ýÔØ
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2¹ýÔØ
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3¹ýÔØ
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1¹ýÔØ
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2¹ýÔØ
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3¹ýÔØ
        if((Address3&0x20)!=0x00)
          return 1;//Ö±Á÷¹ýÔØ
      }
    else if(sys[1]==0)
      {//¹ýÔØÐÅÏ¢
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1¹ýÔØ
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2¹ýÔØ
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3¹ýÔØ
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1¹ýÔØ
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2¹ýÔØ
        if((Address3&0x40)!=0x00&&V3Output==1)
          return -1;//V3¹ýÔØ
        if((Address3&0x20)!=0x00)
          return 1;//Ö±Á÷¹ýÔØ
      }
    if(sys[2]==1)
      {//ŒäÐªÐÅÏ¢
        if((Address1&0x10)!=0x00)
          return 1;//I1ŒäÐª
        if((Address1&0x04)!=0x00)
          return 1;//I2ŒäÐª
        if((Address1&0x01)!=0x00)
          return 1;//I3ŒäÐª
        if((Address2&0x08)!=0x00)
          return 1;//V1ŒäÐª
        if((Address2&0x04)!=0x00)
          return 1;//V2ŒäÐª
        if((Address3&0x80)!=0x00)
          return 1;//V3ŒäÐª
      }
    else if(sys[2]==0)
      {//ŒäÐªÐÅÏ¢
        if((Address1&0x10)!=0x00)
          return -1;//I1ŒäÐª
        if((Address1&0x04)!=0x00)
          return -1;//I2ŒäÐª
        if((Address1&0x01)!=0x00)
          return -1;//I3ŒäÐª
        if((Address2&0x08)!=0x00)
          return -1;//V1ŒäÐª
        if((Address2&0x04)!=0x00)
          return -1;//V2ŒäÐª
        if((Address3&0x80)!=0x00)
          return -1;//V3ŒäÐª
      }
    if(sys[3]==1)
      {//±ÕËøÐÅÏ¢
        if((Address1&0x80)!=0x00)
          return 1;//I1±ÕËø
        if((Address1&0x40)!=0x00)
          return 1;//I2±ÕËø
        if((Address1&0x20)!=0x00)
          return 1;//I3±ÕËø
        if((Address2&0x20)!=0x00)
          return 1;//V1±ÕËø
        if((Address2&0x40)!=0x00)
          return 1;//V2±ÕËø
        if((Address2&0x01)!=0x00)
          return 1;//V3±ÕËø
      }
    else if(sys[3]==0)
      {//±ÕËøÐÅÏ¢
        if((Address1&0x80)!=0x00)
          return -1;//I1±ÕËø
        if((Address1&0x40)!=0x00)
          return -1;//I2±ÕËø
        if((Address1&0x20)!=0x00)
          return -1;//I3±ÕËø
        if((Address2&0x20)!=0x00)
          return -1;//V1±ÕËø
        if((Address2&0x40)!=0x00)
          return -1;//V2±ÕËø
        if((Address2&0x01)!=0x00)
          return -1;//V3±ÕËø
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
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //²»ÐèÒª¿ØŒþ
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
    if(Buzzer(info_result1,info_result2,info_result3)==0) //²»ÐèÒª¿ØŒþ
      {
        //ÔËÐÐÕý³£
          emit SIG_dspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB9~DB15¡£
           emit SIG_dspstate(1);
            //IA¹ýÔØ
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
            emit SIG_dspstate(2);

            //IABC ÎÂ¶È±£»€
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //ICŒäÐª
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
          {  //info_result1ÊÇµÚ1Â·µØÖ·µÄDB9~DB15¡£

              emit SIG_dspstate(4);
            //IB¹ýÔØ
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB8~DB15¡£

              emit SIG_dspstate(5);
           //IC±ÕËø
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB9~DB15¡£
              emit SIG_dspstate(6);
          //IAŒäÐª
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
          { //info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
           //IC¹ýÔØ
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB8~DB15¡£
          //IABC µçÔŽ±£»€
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1ÊÇµÚ1Â·µØÖ·µÄDB9~DB15¡£
              emit SIG_dspstate(9);
          //I2ŒäÐª  ?ÎÂ¶È±£»€ŒäÐª
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
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
           //VA¹ýÔØ
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB8~DB15¡£
              emit SIG_dspstate(11);
              //V2±ÕËø
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3ÊÇµÚ2Â·µØÖ·µÄDB0~DB7¡£

              //V3ŒäÐª
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
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
            //V2¹ýÔØ
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB8~DB15¡£  //DB8
           // VABC ÎÂ¶È±£»€
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
            emit SIG_dspstate(15);
           //VAŒäÐª
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
          { //info_result3ÊÇµÚ2Â·µØÖ·µÄDB0~DB7¡£

             emit SIG_dspstate(16);
            //VC¹ýÔØ
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB8~DB15¡£  //DB13
            emit SIG_dspstate(17);
            //VABCµçÔŽ±£»€
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB9~DB15¡£
            emit SIG_dspstate(18);


          //Vz¹ýÔØ

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
          {//info_result3ÊÇµÚ2Â·µØÖ·µÄDB0~DB7¡£
            emit SIG_dspstate(19);

             //Ö±Á÷¹ýÔØ
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2ÊÇµÚ2Â·µØÖ·µÄDB8~DB15¡£
          emit SIG_dspstate(20);

            // VABC ÎÂ¶È±£»€
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {
            emit SIG_dspstate(21);
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



void thread1::ReadTripTimeLogic(int fd)
{



        int t2=0,t3=0,t4=0;
        unsigned char signature;
        double u,v,w;
        static double SequencerTime2,SequencerTime4;
        float  Temp;
        artExec(fd,"inp:buf:get?",result,15);//ŽÓDSPµÃµœ¿ª¹ØÁ¿œÓµãÐÅÏ¢¡£
        u=result[7];
        v=result[8];
        w=result[9];
        signature=int(result[13]&0x7f);
        switch(signature)
          {
            case 1:
              SeqStep=0;
              break;
            case 2:
              t2=1;
              SequencerTime2=gettime(u,v,w);//ŒÆËãŽÓ¿ªÊŒ²âÊÔµœsequencerµÄStep2µÄÊ±Œä¡£
              break;
            case 3:
              t3=1;
              break;
            case 4:
              t4=1;
              SequencerTime4=gettime(u,v,w);//ŒÆËãŽÓ¿ªÊŒ²âÊÔµœsequencerµÄStep4µÄÊ±Œä¡£
              break;
            case 5:
              break;
          }
        if(t2==1&&SeqStep==0)
          {
            SeqStep=1;

          }
        if(t3==1)
          {
            TripTime3=-1;
            output_signal_end(fd);
            DisplayResult(3);
            starttest=0;
            emit SIG_DisplayChangeValue(0,0);
          //  GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
            ClearLED();
            save_f=1;
          }
        if(t4==1)
          {

             TripTime3=SequencerTime4-SequencerTime2-LogicResolution/1000;
             Temp=TripTime3*1000;
             Temp=ceil(Temp);
             TripTime3=Temp/1000;
             output_signal_end(fd);
             DisplayResult(3);
             starttest=0;

             emit SIG_DisplayChangeValue(0,0);
           //  GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
             ClearLED();
             save_f=1;
          }

}



void thread1::CaculateError()
{



    float y;
    y=chang_amp(ActStep,&max_output_v_a);
    V1ampPerStepError=ActStep-(double)(max_output_v_a*y/32767);

    y=chang_amp(ActStep,&max_output_v_b);
    V2ampPerStepError=ActStep-(double)(max_output_v_b*y/32767);

    y=chang_amp(ActStep,&max_output_v_c);
    V3ampPerStepError=ActStep-(double)(max_output_v_c*y/32767);


    y=chang_amp(ActStep,&max_output_i_a);
    I1ampPerStepError=ActStep-(double)(max_output_i_a*y/32767);

    y=chang_amp(ActStep,&max_output_i_b);
    I2ampPerStepError=ActStep-(double)(max_output_i_b*y/32767);

    y=chang_amp(ActStep,&max_output_i_c);
    I3ampPerStepError=ActStep-(double)(max_output_i_c*y/32767);


}




void thread1::key_deal_test_voltage(int fd)
{

        emit SIG_Teststate(1);
        TestObject=1;
        if(StartValue<EndValue)
          TestObject=2;
         ReturnStep=ActStep;


        if(starttest==0&&input_par==0)
          {
            emit SIG_DisplayChangeValue(0,0);
            emit SIG_testresult(0);
           // GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);  //亮
            SetLED();
            V1Output=0;
            V2Output=0;
            V3Output=0;
            I1Output=0;
            I2Output=0;
            I3Output=0;
            if(ACRelayType==1)
          {
            //  artExec(fd,"out:dig:on(0x04)",result,0);
              switch(OutputMode)
              {
                case 1:
                   V1Output=1;
                    break;
                case 2:
                   V2Output=1;
                    break;
                case 3:
                   V3Output=1;
                    break;
                case 4:
                   V1Output=1;
                   V2Output=1;
                    break;
                case 5:
                   V2Output=1;
                   V3Output=1;
                    break;
                case 6:
                   V1Output=1;
                   V3Output=1;
                    break;
               }

          }

            else
            {
             switch(OutputMode)
             {
             case 1:
                I1Output=1;
                artExec(fd,"out:dig:on(0x10)",result,0);
                 break;
             case 2:
                I2Output=1;
                artExec(fd,"out:dig:on(0x20)",result,0);
                 break;
             case 3:
                I3Output=1;
                artExec(fd,"out:dig:on(0x40)",result,0);
                 break;
             case 4:
                I1Output=1;
                I2Output=1;
                I3Output=1;
                artExec(fd,"out:dig:on(0x74)",result,0);
                 break;
             }
            }

             if(TestItem1==1)
               {

                        seq_count=1;
                        ChangeValue=StartValue;
                        artExec(fd,"out:dig:on(0x08)",result,0);
                        OutputSequencer1(fd);
                        starttest=1;
               }

                    else if(TestItem1==0&&TestItem3==1)
                      {
                        ChangeValue=StopValue;
                        TripValue1=StopValue;
                        starttest=1;
                        seq_count=3;
                        artExec(fd,"out:dig:on(0x08)",result,0);
                        OutputSequencer3(fd);
                        emit SIG_DisplayChangeValue(1,ChangeValue);
                      }
                  }
}


void thread1::key_deal_stop_voltage(int fd)
{

    if(starttest==1)
         {
             output_signal_end(fd);
             auto_count=0;
             auto_i=0;
             IRQcnt=0;
             starttest=0;
             SIG_DisplayChangeValue(0,0);
         }
}


void thread1::key_response(KEY key)
{

 if(key==_NOP)   return;

 if(key==_TEST)
{
     if(starttest==0)
     {
        if((TestItem1==1||TestItem3==1)&&starttest==0) //2009-10-10
        {
          output_signal_end(temprorarydata->UartDev[0].uartfd);
          key_deal_test_voltage(temprorarydata->UartDev[0].uartfd);
          CaculateError();
          emit SIG_TestState();

        }
     }
}
else if(key==_STOP)
{
    //  GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
     ClearLED();
      key_deal_stop_voltage(temprorarydata->UartDev[0].uartfd);
      emit SIG_TestState();

}

}


void thread1::stoprun(){


 Threadrunstate=false;

}



void thread1::run()
{
   KEY key;
   int rc=0;
   unsigned char info_flag1,info_flag2,info_flag3,info_flag4;
   temprorarydatainit();
   UartsInit();

   /*
  temprorarydata->gpio_fd= open("/dev/em335x_gpio", O_RDWR);
  printf("open file = %d\n", temprorarydata->gpio_fd);

           rc = GPIO_OutEnable( temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
           if(rc < 0)
           {
                   printf("GPIO_OutEnable::failed %d\n", rc);
                   temprorarydata->gpio_fd=-1;
           }

   */


    ClearLED();
    online(temprorarydata->UartDev[0].uartfd);

  read_max(temprorarydata->UartDev[0].uartfd);
  artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);

  if((result[7]&0xf0)==0x60)
    { //125V电压板
      MAX_V_VALUEDATA_AC=300;
      MIN_V_VALUEDATA_AC=0;
      MAX_V_VALUEDATA_DC=424;
      MIN_V_VALUEDATA_DC=-424;
    }
      else if((result[7]&0xf0)==0x80)
        { //125VµçÑ¹°å
          MAX_V_VALUEDATA_AC=125;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=176;
          MIN_V_VALUEDATA_DC=-176;
        }
      else if((result[7]&0xf0)==0x00)
        { //75VµçÑ¹°å
          MAX_V_VALUEDATA_AC=75;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=110;
          MIN_V_VALUEDATA_DC=-110;
        }
      if((result[7]&0x0f)==0x01)
        { //30AµçÁ÷°å
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
        { //40AµçÁ÷°å

          MAX_I_VALUEDATA_AC=40;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=56;
          MIN_I_VALUEDATA_DC=-56;
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
         emit SIG_TestState();

         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //²»ÐèÒªžüžÄ
         info_flag1=result[8];
         info_flag4=result[9];
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
         info_flag2=result[8];
         info_flag3=result[9];   
         logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);
     }
     else if(starttest==1)
     {

     if((TestItem1==1&&seq_count==1)||(TestItem2==1&&seq_count==2))
     {
       ReadAutoLogic(temprorarydata->UartDev[0].uartfd); //¶Á¿ª¹ØÁ¿×ŽÌ¬
     }
     else if(TestItem3==1&&seq_count==3)
     ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);  //ÔÚ²âÊÔ¶¯×÷Ê±ŒäÊ±¶Á¿ª¹ØÁ¿×ŽÌ¬¡¢

      artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //²»ÐèÒªžüžÄ
      info_flag1=result[8];
      info_flag4=result[9];
      artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
      info_flag2=result[8];
      info_flag3=result[9];
      logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);

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


