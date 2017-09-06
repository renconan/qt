#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <QDebug>
#include "format.h"


struct REPORTLINK *Reportnodecreate()
{
     struct REPORTLINK *h;
     if((h=(struct REPORTLINK *)malloc(sizeof(struct REPORTLINK)))==NULL)
     {
      printf("can not malloc space\n");
      exit(0);
     }
     return h;
}


void insertsectreport(struct REPORTLINK *head,SECTREPORT value,int id)
{

    if(head==NULL)
    return;

    struct REPORTLINK *tmp;
    if((tmp=(struct REPORTLINK *)malloc(sizeof(struct REPORTLINK)))==NULL)
    {
     printf("can not malloc space\n");
     exit(0);
    }
    tmp->next=NULL;
    memcpy(&tmp->value,&value,sizeof(SECTREPORT));
    tmp->id=id;
    struct REPORTLINK *pre,*next;  //找到当前
    pre=head;
    next=pre->next;
    while(next!=NULL)
    {
     pre=next;
     next=next->next;
    }
    pre->next=tmp;
}


bool getsectreportfromid(struct REPORTLINK *head,SECTREPORT &value,int id)
{
  struct REPORTLINK *next;
  if(head==NULL||head->next==NULL)
      return false;
  next=head->next;
  while(next!=NULL)
  {
      if(next->id==id)
      {
          memcpy(&value,&next->value,sizeof(SECTREPORT));
          return true;
      }
      next=next->next;
  }
  return false;
}
//删除ID 的节点

struct REPORTLINK *getreportnodefromid(struct REPORTLINK *head,int id)
{
    struct REPORTLINK *next;
    if(head==NULL||head->next==NULL)
        return NULL;
    next=head->next;
    while(next!=NULL)
    {
        if(next->id==id)
        {

            return next;
        }
        next=next->next;
    }
    return NULL;

}

void deletesectreportnode(struct REPORTLINK *head,int id)
{
    if(head==NULL)
        return;
    struct REPORTLINK *pre,*next;
    pre=head;
    next=pre->next;
    while(next!=NULL)
    {
        if(next->id==id)
        {
            break;
        }
        pre=next;
        next=next->next;
    }
    if(next==NULL)
        return;
    pre=next->next;
}


void printreport(struct REPORTLINK *head)
{

    struct REPORTLINK *next=NULL;
    if(head==NULL||head->next==NULL)
        return;
    next=head->next;
    while(next!=NULL)
    {


        if(next->value.current.valid)
        {
          qDebug()<<__func__<<__LINE__<<next->value.current.testitem<<\
                   next->value.current.tripvalue<<next->value.current.triptime;
        }
        if(next->value.voltage.valid)
        {
          qDebug()<<__func__<<__LINE__<<next->value.voltage.testitem<<\
                   next->value.voltage.tripvalue<<\
                   next->value.voltage.triptime;
        }
        if(next->value.direction.valid)
        {
          qDebug()<<__func__<<__LINE__<<next->value.direction.actangle1<<\
                    next->value.direction.actangle2<<\
                    next->value.direction.phrasemta;
        }
        if(next->value.frec.valid)
        {
            qDebug()<<__func__<<__LINE__<<next->value.frec.tripfrec<<\

            next->value.frec.dfdttrip<<\
            next->value.frec.triptime<<\
            next->value.frec.Vblocktrip;
        }
        if(next->value.anytest.valid)
        {
            qDebug()<<__func__<<__LINE__<<next->value.anytest.triptime;
        }

        next=next->next;








    }




}


