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
#include "em335x_drivers.h"



#define LEDNUM GPIO8

QSemaphore sem(1);
const float EPSINON =1.0e-04;
TEMPRORARYDATA *temprorarydata;
int info_num=0;
int TripValueSteps=0;
extern int starttest;
extern int seq_count;

extern float faultduration;
extern float prefaulttime;
//动作值
extern float StartFrepuency,EndFrepuency,Freqstep,FreqStepTime;
//动作时间
extern int   PickupType;
extern float TriptimeEndfrec; //触发时间的停止频率
extern float TriptimeHoldtime;
//dfdt闭锁值
extern float DfdtEndfrec;
extern float DfdtStart;
extern float DfdtEnd;
extern float DfdtStep;
//dfdt 触发时间
extern int   DfdtTriptimeType; //1 测试结果 2 用户输入
extern float DfdtOFTriptime;
extern float DfdtTriptimefrec; //dfdt触发时间的停止频率
extern float Dfdtholdtime;
extern float DfdtTimefreq; //dfdt  的计时频率
//电压闭锁值
extern float Dfdt_VoltageBlock;
extern float VoltageBlockEndFrec; //电压闭锁值的停止频率
extern float VoltageBlockStart;
extern float VoltageBlockEnd;
extern float VoltageBlockStep;
extern float VoltageBlockHoldTime;
extern int VPNVPP;

extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern float TimeFrequencer;
extern float currentdfdt;
extern float TripFrepuency; //触发频率
extern float dfdttripvalue; //dfdt的触发值
extern float currentvoltage; //当前的电压闭锁值

extern float postfaulttime;
extern int   dcouttype;

extern bool Threadrunstate;


extern bool SoftStartEndflag;

extern float timeTime; //2016-05-20

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





int GPIO_PinState(int fd, unsigned int* pPinState)
{
        int rc;
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
    temprorarydata->helponflag=false;

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


thread1::thread1(QObject *parent) :
    QThread(parent)
{
    temprorarydata=new TEMPRORARYDATA;
}


void thread1::DisplayResult(int ReslutSum)
{
 emit SIG_testresult(ReslutSum);
}


int  thread1::ReadAutoLogic(int fd)
 {
        int t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;
        int stepnum=0;
        unsigned char signature;
        static double SequencerTime2,SequencerTime3,SequencerTime4;
        double u,v,w;
        float VDS,Temp;
        artExec(fd,"inp:buf:get?",result,15);//从DSP得到开关量接点信息。
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
              SequencerTime2=gettime(u,v,w);//计算从开始测试到sequencer的Step2的时间。
              break;
            case 3:

              t3=1;
              break;
            case 4:
              SequencerTime4=gettime(u,v,w);//计算从开始测试到sequencer的Step4的时间。
              t4=1;
              break;
            case 5:
              t5=1;
              break;
            case 6:
              t6=1;
              break;
          }
        if(t2==1)
        {
            artExec(fd,"seq:status?(step)",result,16);
            stepnum=result[9]*256+result[10];
            TripValueSteps=result[11]*256+result[12];
            switch(seq_count)
            {
            case 1:
                if(StartFrepuency>EndFrepuency)
                ChangeValue=StartFrepuency-Freqstep*stepnum;
                else ChangeValue=StartFrepuency+Freqstep*stepnum;
                emit SIG_DisplayChangeValue(1,ChangeValue);
                break;
            case 2:
                if(PickupType==1)
                emit SIG_DisplayChangeValue(1,TimeFrequencer);
                else
                emit SIG_DisplayChangeValue(1,TriptimeEndfrec);
                break;
            case 3:

                break;
            case 4:
                /*
                float dfdt;
                if(DfdtTriptimeType==1)
                dfdt=dfdttripvalue;  //触发值作为dfdt
                else
                dfdt=DfdtOFTriptime;
                if(SystemFrec>DfdtTriptimefrec)
                ChangeValue=SystemFrec-dfdt*0.001*stepnum;
                else ChangeValue=SystemFrec+dfdt*0.001*stepnum;
                emit SIG_DisplayChangeValue(1,ChangeValue);
                */

                break;
            case 5:
                break;
            }

        }

        if(t3==1)
          {
            switch(seq_count)
              {
                case 1:          //动作频率
                  TripTime1=-1;
                  DisplayResult(1);
                  break;
                case 2:
                  TripTime2=-1;  //动作时间
                  DisplayResult(2);
                  break;
                case 3:
                  TripTime3=-1;
                  break;
                case 4:
                  break;
                case 5:
                  break;

              }
          }
        if(t4==1)
          {
            switch(seq_count)
              {            
                case 1:
                  TripTime1=1;
                  //VDS=floor((SequencerTime4-faultduration-prefaulttime)/FreqStepTime);
                  if(StartFrepuency>EndFrepuency)
                  TripFrepuency=StartFrepuency-Freqstep*TripValueSteps/*VDS*/;
                  else
                  TripFrepuency=StartFrepuency+Freqstep*TripValueSteps/*VDS*/;
                  //Temp=TripFrepuency*100;
                  //Temp=ceil(Temp);
                  //TripFrepuency=Temp/100;//将动作值的限制在小数点后最多只有两位
                  TimeFrequencer=TripFrepuency;
                  DisplayResult(1);

                  break;
                case 2:
                  TripTime2=SequencerTime4-prefaulttime-LogicResolution/1000;
                  Temp=TripTime2*1000;
                  Temp=ceil(Temp);
                  TripTime2=Temp/1000;//将返回值的限制在小数点后最多只有两位
                  DisplayResult(2);
                  break;
                case 3:

                  TripTime3=1;  //dfdt触发了
                  if(DfdtStart>DfdtEnd)
                  dfdttripvalue=currentdfdt+DfdtStep;
                  else
                  dfdttripvalue=currentdfdt-DfdtStep;
                  DisplayResult(3);
                  break;
                case 4:

                  TripTime4=SequencerTime4-SequencerTime2-faultduration-timeTime-LogicResolution/1000;
                  Temp=TripTime4*1000;
                  Temp=ceil(Temp);
                  TripTime4=Temp/1000;//将返回值的限制在小数点后最多只有两位
                  DisplayResult(4);
                  qDebug()<<__func__<<__LINE__<<TripTime4;
                  break;
                case 5:
                   TripTime5=1;
                break;
              }
          }
        if(t5==1)
          {
            switch(seq_count)
              {
                case 1:

                      output_signal_end(fd); //关闭各个放大器
                      starttest=0;           //整个测试结束
                      //ClearLED();
                      GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                      emit SIG_DisplayChangeValue(0,0);
                      save_f=1;    //将保存结果标志置1
                      break;

              case 2://测试动作时间处理

                output_signal_end(fd); //关闭各个放大器
                starttest=0;           //整个测试结束
                //ClearLED();
                GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                save_f=1;//将保存结果标志置1
                emit SIG_DisplayChangeValue(0,0);

                break;
              case 3:
                   if(TripTime3==-1)
                    {

                        if(DfdtStart>DfdtEnd)
                        {
                            if(currentdfdt>=DfdtEnd)
                            {
                                output_signal_end(fd);//关闭各个放大器
                                emit SIG_DisplayChangeValue(1,currentdfdt);
                                artExec(fd,"out:dig:on(0x08)",result,0);
                                OutputSequencer3(fd);//送测试滑差闭锁的sequencer
                                starttest=1;
                                currentdfdt=currentdfdt-DfdtStep;
                                return 0;
                            }
                            else
                            {
                                output_signal_end(fd);//关闭各个放大器
                                save_f=1;
                                starttest=0;          //整个测试结束
                                //ClearLED();
                                GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                                DisplayResult(3);
                                emit SIG_DisplayChangeValue(0,0);
                            }
                        }
                        else
                        {
                            if(currentdfdt<=DfdtEnd)
                            {
                                output_signal_end(fd);//关闭各个放大器
                                emit SIG_DisplayChangeValue(1,currentdfdt);
                                artExec(fd,"out:dig:on(0x08)",result,0);
                                OutputSequencer3(fd);//送测试滑差闭锁的sequencer
                                starttest=1;
                                currentdfdt=currentdfdt+DfdtStep;
                                return 0;
                            }
                            else
                            {
                                output_signal_end(fd);//关闭各个放大器
                                starttest=0; //整个测试结束
                                save_f=1;
                                //ClearLED();
                                GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                                DisplayResult(3);
                                emit SIG_DisplayChangeValue(0,0);
                            }
                        }
                    }

                   else
                    {

                        output_signal_end(fd);//关闭各个放大器
                        starttest=0;         //整个测试结束
                        //ClearLED();
                        GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                        save_f=1;
                        emit SIG_DisplayChangeValue(0,0);
                    }
                break;
         case 4:

                TripTime4=-1;
                DisplayResult(4);
                qDebug()<<__func__<<__LINE__<<TripTime4;
                break;
            case 5:
                 TripTime5=-1;
                 break;

             }
        }
        if(t6==1)
        {
          switch(seq_count)
           {

          case 4:

            output_signal_end(fd);//关闭各个放大器
            starttest=0; //整个测试结束
            //ClearLED();
            GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
            save_f=1;//将保存结果标志置1
            emit SIG_DisplayChangeValue(0,0);

            break;


           case 5:

            starttest=0;
            output_signal_end(fd);//先关闭各个放大器
            if(VoltageBlockStart>VoltageBlockEnd)
           {
             if(currentvoltage>=VoltageBlockEnd&&TripTime5==-1)
            {

             emit SIG_DisplayChangeValue(1,currentvoltage);
             artExec(fd,"out:dig:on(0x08)",result,0);
             OutputSequencer5(fd);//送测试滑差闭锁的sequencer
             currentvoltage=currentvoltage-VoltageBlockStep;
             starttest=1;

            }
            else
             {
             output_signal_end(fd);//关闭各个放大器
             starttest=0; //整个测试结束
             //ClearLED();
             GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
             save_f=1;//将保存结果标志置1
             emit SIG_DisplayChangeValue(0,0);
             DisplayResult(5);
             }
           }


            else if(VoltageBlockStart<=VoltageBlockEnd)
            {
              if(currentvoltage<=VoltageBlockEnd&&TripTime5==-1)
             {


              emit SIG_DisplayChangeValue(1,currentvoltage);
              artExec(fd,"out:dig:on(0x08)",result,0);
              OutputSequencer5(fd);//送测试滑差闭锁的sequencer
              currentvoltage=currentvoltage+VoltageBlockStep;
              starttest=1;

             }
             else
              {
              output_signal_end(fd);   //关闭各个放大器
              starttest=0;             //整个测试结束
              //ClearLED();
              GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
              save_f=1;               //将保存结果标志置1
              emit SIG_DisplayChangeValue(0,0);
              DisplayResult(5);
              }
            }

            break;
           }
           }
}

int  thread1::Buzzer(char Address1,char Address2,char Address3)
  {//sys[1]:过载，sys[2]:间歇，sys[3]:闭锁
    if(sys[1]==1)
      {//过载信息
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1过载
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2过载
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3过载
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1过载
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2过载
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    else if(sys[1]==0)
      {//过载信息
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1过载
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2过载
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3过载
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1过载
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2过载
        if((Address3&0x40)!=0x00&&V3Output==1)
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

    static int info_count=1;
    int  k1=0;


    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);
    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //不需要控件
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
    if(Buzzer(info_result1,info_result2,info_result3)==0) //不需要控件
      {
        //运行正常
          emit SIG_dspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1是第1路地址的DB9~DB15。
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
        if((info_result1&0x02)==0x02&&I2Output==1)
          {  //info_result1是第1路地址的DB9~DB15。

              emit SIG_dspstate(4);
            //IB过载
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1是第1路地址的DB8~DB15。

              emit SIG_dspstate(5);
           //IC闭锁
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1是第1路地址的DB9~DB15。
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
        if((info_result2&0x80)==0x80&&I3Output==1)
          { //info_result2是第2路地址的DB9~DB15。
           //IC过载
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1是第1路地址的DB8~DB15。
          //IABC 电源保护
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1是第1路地址的DB9~DB15。
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
        if((info_result2&0x10)==0x10&&V1Output==1)
          {//info_result2是第2路地址的DB9~DB15。
           //VA过载
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15。
              emit SIG_dspstate(11);
              //V2闭锁
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7。

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
        if((info_result2&0x02)==0x02&&V2Output==1)
          {//info_result2是第2路地址的DB9~DB15。
            //V2过载
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2是第2路地址的DB8~DB15。  //DB8
           // VABC 温度保护
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2是第2路地址的DB9~DB15。
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
        if((info_result3&0x40)==0x40&&V3Output==1)
          { //info_result3是第2路地址的DB0~DB7。

             emit SIG_dspstate(16);
            //VC过载
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2是第2路地址的DB8~DB15。  //DB13
            emit SIG_dspstate(17);
            //VABC电源保护
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2是第2路地址的DB9~DB15。
            emit SIG_dspstate(18);


          //Vz过载

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
          {//info_result3是第2路地址的DB0~DB7。
            emit SIG_dspstate(19);

             //直流过载
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15。
          emit SIG_dspstate(20);

            // VABC 温度保护
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7。
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


void thread1::key_deal_test_current(int fd)
{

        if(starttest==0)
          {
            emit SIG_Teststate(1);
            emit SIG_testresult(0);
            TripTime1=-1;
            TripTime2=-1;
            TripTime3=-1;
            TripTime4=-1;
            TripTime5=-1;

            V1Output=1;
            V2Output=1;
            V3Output=1;
            I1Output=1;
            I2Output=1;
            I3Output=1;

           artExec(fd,"out:dig:on(0x74)",result,0);

            if(TestItem1==1)
              {
                 starttest=1;
                 seq_count=1;

                 //SetLED();
                 GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);  //亮
                 V1Output=1;
                 V2Output=1;
                 V3Output=1;

                 artExec(fd,"out:dig:on(0x08)",result,0);
                 OutputSequencer1(temprorarydata->UartDev[0].uartfd);//输出测试动作频率的sequencer
              }
            else if(TestItem1==0&&TestItem2==1)
              {

                starttest=1;
                  seq_count=2;
                  V1Output=1;
                  V2Output=1;
                  V3Output=1;
                  TimeFrequencer=TriptimeEndfrec; //计时频率为用户输入的频率值
                  artExec(fd,"out:dig:on(0x08)",result,0);

                  OutputSequencer2(temprorarydata->UartDev[0].uartfd);
                  GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);  //亮
                  //SetLED();


            }
            else if(TestItem1==0&&TestItem2==0&&TestItem3==1)
              {
                  starttest=1;
                  seq_count=3;

                  //SetLED();
                  GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);  //亮
                  V1Output=1;
                  V2Output=1;
                  V3Output=1;

                  currentdfdt=DfdtStart;
                  emit SIG_DisplayChangeValue(1,currentdfdt);
                  artExec(fd,"out:dig:on(0x08)",result,0);
                  OutputSequencer3(temprorarydata->UartDev[0].uartfd);
                  if(DfdtStart>DfdtEnd) //若DFDT是下降的
                  currentdfdt=currentdfdt-DfdtStep;
                  else
                  currentdfdt=currentdfdt+DfdtStep;
              }
            else if(TestItem1==0&&TestItem2==0&&TestItem3==0&&TestItem4==1)
            {


                 //SetLED();
                GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);  //亮
                 V1Output=1;
                 V2Output=1;
                 V3Output=1;
                 starttest=1;
                 seq_count=4;
                 if(DfdtTriptimeType==1)
                 emit SIG_DisplayChangeValue(1,dfdttripvalue);
                 else
                 emit SIG_DisplayChangeValue(1,DfdtOFTriptime);

                 artExec(fd,"out:dig:on(0x08)",result,0);
                 OutputSequencer4(temprorarydata->UartDev[0].uartfd);

             }
            else if(TestItem1==0&&TestItem2==0&&TestItem3==0&&TestItem4==0&&TestItem5==1)
            {
                 GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);
                   //SetLED();
                 V1Output=1;
                 V2Output=1;
                 V3Output=1;
                 starttest=1;
                 seq_count=5;
                 currentvoltage=VoltageBlockStart;

                 artExec(fd,"out:dig:on(0x08)",result,0);
                 OutputSequencer5(temprorarydata->UartDev[0].uartfd);
                 emit SIG_DisplayChangeValue(1,currentvoltage);
                 if(VoltageBlockStart>VoltageBlockEnd)
                  currentvoltage=currentvoltage-VoltageBlockStep;
                 else
                  currentvoltage=currentvoltage+VoltageBlockStep;
            }

            emit SIG_TestState();
          }
}

void thread1::key_deal_stop_current(int fd)
{
    if(starttest==1)
          {
            starttest=0;
            output_signal_end(fd);
            emit SIG_DisplayChangeValue(0,0);  //输出值结果清空
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

     }
}

else if(key==_STOP)
{
      GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
      //ClearLED();
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
  int rc=0;
  //打开并配置串口
  unsigned char info_flag1,info_flag2,info_flag3,info_flag4;
  temprorarydatainit();
  UartsInit();

  temprorarydata->gpio_fd= open("/dev/em335x_gpio", O_RDWR);
  printf("open file = %d\n", temprorarydata->gpio_fd);
  rc = GPIO_OutEnable(temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
  if(rc < 0)
  {
          printf("GPIO_OutEnable::failed %d\n", rc);
          temprorarydata->gpio_fd=-1;
  }


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
        { //125V电压板
          MAX_V_VALUEDATA_AC=125;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=176;
          MIN_V_VALUEDATA_DC=-176;
        }
      else if((result[7]&0xf0)==0x00)
        { //75V电压板
          MAX_V_VALUEDATA_AC=75;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=110;
          MIN_V_VALUEDATA_DC=-110;
        }
      if((result[7]&0x0f)==0x01)
        { //30A电流板
          MAX_I_VALUEDATA_AC=30;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=5;
          MIN_I_VALUEDATA_DC=-5;
//          MAX_I_VALUEDATA_AC=30;
//          MIN_I_VALUEDATA_AC=0;
//          MAX_I_VALUEDATA_DC=42;
//          MIN_I_VALUEDATA_DC=-42;
        }
      else if((result[7]&0x0f)==0x08)
        { //40A电流板
          MAX_I_VALUEDATA_AC=40;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=56;
          MIN_I_VALUEDATA_DC=-56;
        }
      else if((result[7]&0x0f)==0x05)
        { //50A电流板
          MAX_I_VALUEDATA_AC=50;
          MIN_I_VALUEDATA_AC=0;
          MAX_I_VALUEDATA_DC=70;
          MIN_I_VALUEDATA_DC=-70;
        }

  while(Threadrunstate)
  {
      if(temprorarydata->keypadopenflag==false&&temprorarydata->helponflag==false)
    { key=GetKey(temprorarydata->UartDev[1].uartfd);
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
         if(info_num++>=5){
         emit SIG_TestState();
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //不需要更改
         info_flag1=result[8];
         info_flag4=result[9];
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
         info_flag2=result[8];
         info_flag3=result[9];
         logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //需要emit

          }
          }
     else {

              ReadAutoLogic(temprorarydata->UartDev[0].uartfd);
               if(info_num++>=5)
              {
              artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //不需要更改
              info_flag1=result[8];
              info_flag4=result[9];
              artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
              info_flag2=result[8];
              info_flag3=result[9];
              logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //需要emit

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

}


