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
#include <termios.h>


extern float prefaulttime;
TEMPRORARYDATA *temprorarydata;
#define LEDNUM GPIO8

extern float StopCurrent; //动作时间的用户手动输入的值
extern int TripTimeType;   //测动作时间的方式
extern float StopCurrentHoldTime ;//动作时间的保持时间

float CurrentOutput=0;

extern float SystemVolt;


extern float Vfault;
extern float Angle;
extern float VpnValue; //单相的幅值
extern float VpnAngle;

 float V1Value,V1Phase;
 float V2Value,V2Phase;
 float V3Value,V3Phase;
float changedate;

float  V1PerFaultPhase,V2PerFaultPhase,V3PerFaultPhase;


 float PhaseIA;
 float PhaseIB;
 float PhaseIC;


 int TripValueSteps=0;
 int ReturnSteps=0;

 extern volatile bool Threadrunstate;
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


void temprorarydatainit()
{
   //EM3352   S2 S1
   //MPF300   S1 S3 

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

void thread1::DisplayResult(int ResultSum)
{
    emit  SIG_testresult(ResultSum);
}

void thread1::ReadAutoLogic(int fd)
{
        int t4=0,t3=0,t5=0,t2=0,t1=0;
        unsigned char signature;
        static double SequencerTime4,SequencerTime2;
        double u,v,w;
        float VDS;
        artExec(fd,"inp:buf:get?",result,15);//从DSP得到开关量接点信息。
        u=result[7];
        v=result[8];
        w=result[9];
        int stepnum=0;
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
          SequencerTime4=gettime(u,v,w);//计算从开始测试到sequencer的Step4的时间。
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
            if(TestItem2==1||TestItem1==1)
            {
                 ReturnSteps=result[13]*256+result[14];
                 if(ReturnSteps>32767)
                     ReturnSteps=32768-ReturnSteps;
            }

            switch(seq_count)
            {
             case 1:
                //changedate=StartCurrent;
                if(TripValueSteps<=stepnum)
                {
                    if(StartCurrent>EndCurrent)
                         ChangeValue=StartCurrent-ActStep*(stepnum);
                    else if(StartCurrent<EndCurrent)
                         ChangeValue=StartCurrent+ActStep*(stepnum);
                    emit SIG_DisplayChangeValue(1,ChangeValue);
                    changedate=ChangeValue;
                }
                else if(TripValueSteps>(fabs(StartCurrent-EndCurrent)/ActStep))
                {
                    stepnum=fabs(StartCurrent-EndCurrent)/ActStep;
                    if(StartCurrent>EndCurrent)
                         ChangeValue=StartCurrent-ActStep*(stepnum);
                    else if(StartCurrent<EndCurrent)
                         ChangeValue=StartCurrent+ActStep*(stepnum);
                    emit SIG_DisplayChangeValue(1,ChangeValue);
                    changedate=ChangeValue;
                }
                else if(stepnum==0)
                {
                    emit SIG_DisplayChangeValue(1,StartCurrent);
                }
                else
                {
                    if(StartCurrent<EndCurrent)
                        ChangeValue=changedate-(TripValueSteps-ReturnSteps)*ActStep;
                    else if(StartCurrent>EndCurrent)
                        ChangeValue=changedate+(TripValueSteps-ReturnSteps)*ActStep;
                    emit SIG_DisplayChangeValue(1,ChangeValue);
                }
//                   if(StartCurrent>EndCurrent)
//                   ChangeValue=StartCurrent-(ActStep/*-I1ampPerStepError*/)*stepnum;
//                   else
//                   ChangeValue=StartCurrent+(ActStep/*-I1ampPerStepError*/)*stepnum;
                   break;
             case 2:
                   break;
            }
            CurrentOutput=ChangeValue;
            /*
            if(TripValueSteps!=0&&stepnum==0) //??????????
            {
                output_signal_end(fd);
                starttest=0;
                TripTime2=-1;
                DisplayResult(2);
                emit SIG_DisplayChangeValue(0,0);
                GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //?
                save_f=1;
            }
            */

            emit SIG_DisplayChangeValue(1,ChangeValue);
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
                case 2:

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
                  if(StartCurrent>EndCurrent)
                   TripValue=StartCurrent-TripValueSteps*(ActStep/*-I1ampPerStepError*/);
                   else
                   TripValue=StartCurrent+TripValueSteps*(ActStep/*-I1ampPerStepError*/);
                   DisplayResult(1);
                  if(TestItem2==1)
                  {
                    if(ReturnSteps==0)
                      TripTime2=-1;
                    else
                      TripTime2=1;
                    if(StartCurrent>EndCurrent)
                      ReturnValue=StartCurrent-ReturnSteps*(ActStep/*-I1ampPerStepError*/);
                    else
                      ReturnValue=StartCurrent+ReturnSteps*(ActStep/*-I1ampPerStepError*/);
                    DisplayResult(2);
                  }

              }
              else
              {
                DisplayResult(1);
              }
               output_signal_end(fd);
               starttest=0;
               GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
               emit SIG_DisplayChangeValue(0,0);
               save_f=1;
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

void thread1::logic_info(int fd,char info_result1,char info_result2,char info_result3)
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


void thread1::ReadTripTimeLogic(int fd)
{

        int t2=0,t3=0,t4=0;
        unsigned char signature;
        double u,v,w;
        float  Temp;
        static double SequencerTime2,SequencerTime4;
        artExec(fd,"inp:buf:get?",result,15);   //从DSP得到开关量接点信息。
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
                                      SequencerTime2=gettime(u,v,w);//计算从开始测试到sequencer的Step2的时间。
                                      break;
                                    case 3:
                                      t3=1;
                                      break;
                                    case 4:
                                      t4=1;
                                      SequencerTime4=gettime(u,v,w);//计算从开始测试到sequencer的Step4的时间。
                                      break;
                                    case 5:
                                      break;
          }

        if(t2==1&&SeqStep==0)
          {
             SeqStep=1;


          }
        if(t3==1)
          {//未动作
            TripTime=-1;
            output_signal_end(fd);//关闭各个放大器
            DisplayResult(3);//显示测试结果
            starttest=0;
            emit SIG_DisplayChangeValue(0,0);
            GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
            save_f=1;//将保存结果标志置1
          }
        if(t4==1)
          {  //保护动作
             TripTime=SequencerTime4-SequencerTime2-LogicResolution/1000;
             Temp=TripTime*1000;
             Temp=ceil(Temp);
             TripTime=Temp/1000;//将动作时间的限制在小数点后最多只有三位
             output_signal_end(fd);//关闭各个放大器
             DisplayResult(3);//显示测试结果
             starttest=0;
             emit SIG_DisplayChangeValue(0,0);//测试结束，将输出值的显示区域清空。
             GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
             save_f=1;//将保存结果标志置1
          }

}



void thread1::CaculateError(){

    float y;

    y=chang_amp(ActStep,&max_output_i_a);
    I1ampPerStepError=ActStep-(double)(max_output_i_a*y/32767);

    y=chang_amp(ActStep,&max_output_i_b);
    I2ampPerStepError=ActStep-(double)(max_output_i_b*y/32767);

    y=chang_amp(ActStep,&max_output_i_c);
    I3ampPerStepError=ActStep-(double)(max_output_i_c*y/32767);

}






void thread1::key_deal_test_current(int fd)
{
       ReturnStep=ActStep;
       if(starttest==0)
         {

           artExec(fd,"out:dig:off(0xf4)",result,0);
           GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);  //亮
           emit SIG_testresult(0);  //清空测试结果区域
           starttest=1;
           V1Output=1;
           V2Output=1;
           V3Output=1;
           I1Output=0;
           I2Output=0;
           I3Output=0;
           switch(OutputMode)
             {
               case 1:
                  I1Output=1;
                  V1Value=Vfault;
                  V2Value=SystemVolt;
                  V3Value=SystemVolt;
                  V1Phase=0;
                  V2Phase=-120;
                  V3Phase=120;
                  PhaseIA=0-Angle;
                  PhaseIB=0;
                  PhaseIC=0;

                  V1PerFaultPhase=V1Phase-360*SystemFrec*prefaulttime;
                  V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                  V2PerFaultPhase=V1PerFaultPhase-120;
                  V3PerFaultPhase=V1PerFaultPhase+120;
                  if(V2PerFaultPhase<=-360)
                      V2PerFaultPhase+=360;
                  if(V3PerFaultPhase>=360)
                      V2PerFaultPhase-=360;

                   break;
               case 2:
                  I2Output=1;
                  V1Value=SystemVolt;
                  V2Value=Vfault;
                  V3Value=SystemVolt;
                  V1Phase=120;
                  V2Phase=0;
                  V3Phase=-120;
                  PhaseIB=0-Angle;
                  PhaseIA=0;
                  PhaseIC=0;

                  V2PerFaultPhase=V2Phase-360*SystemFrec*prefaulttime;
                  V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
                  V1PerFaultPhase=V2PerFaultPhase+120;
                  V3PerFaultPhase=V2PerFaultPhase-120;
                  if(V1PerFaultPhase>=360)
                     V1PerFaultPhase=V1PerFaultPhase-360;
                   if(V3PerFaultPhase<=-360)
                     V3PerFaultPhase+=360;
                   break;
               case 3:
                  I3Output=1;
                  V1Value=SystemVolt;
                  V2Value=SystemVolt;
                  V3Value=Vfault;
                  V1Phase=-120;
                  V2Phase=120;
                  V3Phase=0;
                  PhaseIC=0-Angle;
                  PhaseIA=0;
                  PhaseIB=0;
                  V3PerFaultPhase=V3Phase-360*SystemFrec*prefaulttime;
                  V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
                  V1PerFaultPhase=V3PerFaultPhase-120;
                  V2PerFaultPhase=V3PerFaultPhase+120;
                  if(V2PerFaultPhase>=360)
                     V2PerFaultPhase=V2PerFaultPhase-360;
                   if(V1PerFaultPhase<=-360)
                     V1PerFaultPhase+=360;

                   break;

               case 4:
                 I1Output=1;
                 I2Output=1;
                 VpnValue=sqrt(SystemVolt*SystemVolt/4+Vfault*Vfault/4);
                 VpnAngle=atan(Vfault/SystemVolt)*180/3.1415+180;
                 V1Value=VpnValue;
                 V2Value=VpnValue;
                 V3Value=SystemVolt;
                 V2Phase=-90-VpnAngle;
                 V1Phase=-90+VpnAngle;
                 V3Phase=-90;

                 PhaseIC=V3Phase-Angle;
                 PhaseIA=PhaseIC-90;
                 PhaseIB=PhaseIC+90;



                 V3PerFaultPhase=V3Phase-360*SystemFrec*prefaulttime;
                 V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;
                 V1PerFaultPhase=V3PerFaultPhase-120;
                 V2PerFaultPhase=V3PerFaultPhase+120;
                 if(V2PerFaultPhase>=360)
                    V2PerFaultPhase=V2PerFaultPhase-360;
                  if(V1PerFaultPhase<=-360)
                    V1PerFaultPhase+=360;

                 break;

             case 5:
                 I3Output=1;
                 I2Output=1;
                 VpnValue=sqrt(SystemVolt*SystemVolt/4+Vfault*Vfault/4);
                 VpnAngle=atan(Vfault/SystemVolt)*180/3.1415+180;

                V1Value=SystemVolt;
                V3Value=VpnValue;
                V2Value=VpnValue;
                V1Phase=-90;
                V3Phase=-90-VpnAngle;
                V2Phase=-90+VpnAngle;


                PhaseIA=V1Phase-Angle;
                PhaseIB=PhaseIA-90;
                PhaseIC=PhaseIA+90;



                V1PerFaultPhase=V1Phase-360*SystemFrec*prefaulttime;
                V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                V2PerFaultPhase=V1PerFaultPhase-120;
                V3PerFaultPhase=V1PerFaultPhase+120;
                if(V2PerFaultPhase<=-360)
                    V2PerFaultPhase+=360;
                if(V3PerFaultPhase>=360)
                    V2PerFaultPhase-=360;



               break;

             case 6:
               I3Output=1;
               I1Output=1;

               VpnValue=sqrt(SystemVolt*SystemVolt/4+Vfault*Vfault/4);
               VpnAngle=atan(Vfault/SystemVolt)*180/3.1415+180;
               V1Value=VpnValue;
               V3Value=VpnValue;
               V2Value=SystemVolt;

               V2Phase=-90;
               V1Phase=-90-VpnAngle;
               V3Phase=-90+VpnAngle;


               PhaseIB=V2Phase-Angle;
               PhaseIA=PhaseIB+90;
               PhaseIC=PhaseIB-90;


               V2PerFaultPhase=V2Phase-360*SystemFrec*prefaulttime;
               V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
               V1PerFaultPhase=V2PerFaultPhase+120;
               V3PerFaultPhase=V2PerFaultPhase-120;
               if(V1PerFaultPhase>=360)
                  V1PerFaultPhase=V1PerFaultPhase-360;
                if(V3PerFaultPhase<=-360)
                  V3PerFaultPhase+=360;





               break;

               case 7:
                  I1Output=1;
                  I2Output=1;
                  I3Output=1;
                  V2Phase=-120;
                  V3Phase=120;
                  V1Phase=0;


                  V1Value=Vfault;
                  V3Value=Vfault;
                  V2Value=Vfault;
                  PhaseIA=0-Angle;
                  PhaseIB=0-Angle-120;
                  PhaseIC=0-Angle+120;

                  V1PerFaultPhase=V1Phase-360*SystemFrec*prefaulttime;
                  V2PerFaultPhase=V2Phase-360*SystemFrec*prefaulttime;
                  V3PerFaultPhase=V3Phase-360*SystemFrec*prefaulttime;
                  V1PerFaultPhase=V1PerFaultPhase-((int)(V1PerFaultPhase/360))*360;
                  V2PerFaultPhase=V2PerFaultPhase-((int)(V2PerFaultPhase/360))*360;
                  V3PerFaultPhase=V3PerFaultPhase-((int)(V3PerFaultPhase/360))*360;

                   break;
              }


           if(TestItem1==1)
             { //要测试Trip value
               seq_count=1;
               artExec(fd,"out:dig:on(0x08)",result,0);
               OutputSequencer1(fd);//输出测试Trip value的sequencer

             }
           else if(TestItem1==0&&TestItem3==1)
           {
               ChangeValue=StopCurrent;
               TripValue1=StopCurrent;
               starttest=1;
               seq_count=3;
               artExec(fd,"out:dig:on(0x08)",result,0);
               OutputSequencer3(fd);
               emit SIG_DisplayChangeValue(1,ChangeValue);
           }

         }


}



void thread1::key_deal_stop_current(int fd)
{
    if(starttest==1)
         {

           output_signal_end(fd);
           auto_count=0;
           auto_i=0;
           IRQcnt=0;
           starttest=0;
          emit SIG_testresult(0);  //清空测试结果区域
          emit SIG_DisplayChangeValue(0,0); //输出值区域清0
         }
}

void thread1::key_response(KEY key)
{

if(key==_NOP)

    return;


if(key==_TEST)
{
     if(starttest==0)   
     {
        output_signal_end(temprorarydata->UartDev[0].uartfd);
        key_deal_test_current(temprorarydata->UartDev[0].uartfd);
        CaculateError();
        emit SIG_TestState();
     }
}

else if(key==_STOP)
{
      GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
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
    int rc;
     //打开并配置串口
   int info_num=0;
   unsigned char info_flag1,info_flag2,info_flag3;
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
  GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
  read_max(temprorarydata->UartDev[0].uartfd);
  artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);


      if((result[7]&0xf0)==0x60)
      { //300V电压板
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
          MAX_I_VALUEDATA_DC=42;
          MIN_I_VALUEDATA_DC=-42;
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

     while(Threadrunstate){
       if(temprorarydata->keypadopenflag==false&&temprorarydata->helponflag==false)
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

         if(info_num++>10)
        {
         emit SIG_TestState();
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //不需要更改
         info_flag1=result[8];
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
         info_flag2=result[8];
         info_flag3=result[9];
         logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);
         info_num=0;
        }
         //需要emit
     }
     else
     {

         if((TestItem1==1&&seq_count==1)||(TestItem2==1&&seq_count==2))
         { //在测试动作值、返回值时读开关量状态、改变显示值
            ReadAutoLogic(temprorarydata->UartDev[0].uartfd); //读开关量状态
            if(starttest==0)
            GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
         }
         else if(TestItem3==1&&seq_count==3)
         ReadTripTimeLogic(temprorarydata->UartDev[0].uartfd);  //在测试动作时间时读开关量状态、
          if(info_num++>5)
         {
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);          //不需要更改
          info_flag1=result[8];
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
          info_flag2=result[8];
          info_flag3=result[9];
          logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3);  //需要emit
          info_num=0;
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


