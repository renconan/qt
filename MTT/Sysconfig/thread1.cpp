#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>

#include <QTimer>
#include <QProcess>
#include <QDir>

#include "Config.h"
#include "thread1.h"

#include "Uartset.h"
#include "Testcom2.h"
#include <unistd.h>

//#include "Nodelinkmanage.h"

 TEMPRORARYDATA *temprorarydata;
 extern unsigned char result[70],receive_data[70];
 extern bool threadrunstate;  //线程是否正在运行


//串口设置如下
thread1::thread1(QObject *parent) :
    QThread(parent)
{
    temprorarydata=new TEMPRORARYDATA;
    dcouttype=1;

}

void thread1::setdcoutput(int type)
{
dcouttype=type;

}




void thread1::run()
{
    temprorarydatainit();
    UartsInit();
    online(temprorarydata->UartDev[0].uartfd);
    if(dcouttype==1)
    artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x60)",result,0);
    else if(dcouttype==2)
    artExec(temprorarydata->UartDev[0].uartfd,"out:dig:on(0x60)",result,0);
    else if(dcouttype==3)
  {
      artExec(temprorarydata->UartDev[0].uartfd,"out:dig:on(0x60)",result,0);
      artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x20)",result,0);

  }
    close(temprorarydata->UartDev[0].uartfd);
    close(temprorarydata->UartDev[1].uartfd);
    free(temprorarydata);
    temprorarydata=NULL;
}

