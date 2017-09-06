#include "importthread.h"
#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>

#include <QTimer>
#include <QProcess>
#include <QDir>
#include "Config.h"
#include "Nodelinkmanage.h"

extern bool Importthreadrunstate;


Importthread::Importthread(QObject *parent) :
    QThread(parent)
{
}

void Importthread::setcmd(char *cmd)
{
    if(strlen(cmd)==0)
        return;
    strcpy(cmdstr,cmd);
}


void Importthread::run()
{
    if(strlen(cmdstr)==0)
        return;
  Importthreadrunstate=true;
  QProcess  *proc=new QProcess;
  proc->start(cmdstr);
  proc->waitForFinished(-1);
  msleep(1500);
  Importthreadrunstate=false;
}

