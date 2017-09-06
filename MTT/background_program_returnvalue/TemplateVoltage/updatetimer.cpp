#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include <QFile>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>
#include <time.h>
#include <signal.h>


#include "Config.h"
#include "Defpam.h"
#include <QSemaphore>
#include "Testcom2.h"
#include "Volseq.h"          //电流测试的seq


#include "VolReadlogic.h"

#include "Gpio.h"
#include "em335x_drivers.h"
#include "Uartset.h"
#include "updatetimer.h"

extern TEMPRORARYDATA *temprorarydata;

extern long auto_i,IRQcnt,auto_count,tice;
extern int   starttest;


extern float StartCurrent,EndCurrent,ActStep,ReturnStep,StepTime;
extern float LogicResolution,SystemFrec;
extern float TripValue,TripValue1,ReturnValue,ReturnValue1,TripTime,ReturnQuotiety;
extern  float ChangeValue,timeout;

extern int   TestItem1,TestItem2,TestItem3,OutputMode,TestObject;
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern int seq_count;





void auto_f(int signo)  //INTEL 工控机定时器中断 干什么用
 {
   if(auto_i++<tice||starttest==0)
     {
      return;
   }

   auto_i=0;
   IRQcnt=1;
 }




void startimer()
{
     struct itimerval value, ovalue;
     signal(SIGALRM, auto_f);
     value.it_value.tv_sec = 0;
     value.it_value.tv_usec = 1000;
     value.it_interval.tv_sec =0;
     value.it_interval.tv_usec = 1000;
     setitimer(ITIMER_REAL, &value, &ovalue);

}

void stoptimer()
{
    struct itimerval value, ovalue;
    signal(SIGALRM, auto_f);
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec =0;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &value, &ovalue);

}


void DisplayAutoValue(int fd)
{


    switch(seq_count)
         {
           case 1:
             if(TestObject==1)
               ChangeValue=ChangeValue-ActStep;
             else if(TestObject==2)
               ChangeValue=ChangeValue+ActStep;

             break;
           case 2:
             if(TestObject==1)
               ChangeValue=ChangeValue+ReturnStep;
             else if(TestObject==2)
               ChangeValue=ChangeValue-ReturnStep;
             break;
         }
}



void UpdateAutoValue(int fd)
{


    if(auto_count==0)
      {
        auto_count++;
        auto_i=0;
        IRQcnt=0;
        tice=(long)(StepTime*1000);
        return;
      }

    if(auto_count==IRQcnt)
      {
        IRQcnt=0;
        auto_count=1;
       DisplayAutoValue(fd);
      }


}



