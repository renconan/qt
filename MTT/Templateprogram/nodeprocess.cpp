#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <QDebug>
#include "format.h"




struct SECTMENTSETNODE *Nodecreate()
{
     struct SECTMENTSETNODE *h;
     if((h=(struct SECTMENTSETNODE *)malloc(sizeof(struct SECTMENTSETNODE)))==NULL)
     {
      printf("can not malloc space\n");
      exit(0);
     }
     return h;
}



void insertnode(struct SECTMENTSETNODE *head,struct SECTMENTSET value,int id)
{

    if(head==NULL)
    return;
    struct SECTMENTSETNODE *tmp;
    if((tmp=(struct SECTMENTSETNODE *)malloc(sizeof(struct SECTMENTSETNODE)))==NULL)
    {
     printf("can not malloc space\n");
     exit(0);
    }
    tmp->next=NULL;
    memcpy(&tmp->value,&value,sizeof(struct SECTMENTSET));
    tmp->id=id;
    struct SECTMENTSETNODE *pre,*next;  //找到当前
    pre=head;
    next=pre->next;
    while(next!=NULL)
    {
     pre=next;
     next=next->next;
    }
    pre->next=tmp;
}


//从name中找到 一段的设置
bool getnodevaluefromname(struct SECTMENTSETNODE *head,char  *name,struct SECTMENTSET &set)
{
    if(head==NULL)
        return false;
    if(head->next==NULL)
    return false;
    struct SECTMENTSETNODE *pre,*next;
    pre=head;
    next=pre->next;
    while(next!=NULL)
    {
        if(strcmp(next->value.name,name)==0)
        {

            memcpy(&set,&next->value,sizeof(struct SECTMENTSET));
            return true;
        }
        pre=next;
        next=next->next;
    }
   return false;
}


bool getsectfromid(struct SECTMENTSETNODE *head,struct SECTMENTSET &value,int id)
{
  struct SECTMENTSETNODE *next;
  if(head==NULL||head->next==NULL)
      return false;
  next=head->next;
  while(next!=NULL)
  {
      if(next->id==id)
      {
          memcpy(&value,&next->value,sizeof(struct SECTMENTSET));
          return true;
      }
      next=next->next;
  }
  return false;
}
//删除ID 的节点

void setsectfromid(struct SECTMENTSETNODE *head,int id,struct SECTMENTSET value)
{
    struct SECTMENTSETNODE *next;
    if(head==NULL||head->next==NULL)
        return;
    next=head->next;
    while(next!=NULL)
    {
        if(next->id==id)
        {
            memcpy(&next->value,&value,sizeof(struct SECTMENTSET));
            return;
        }
        next=next->next;
    }
}


struct SECTMENTSETNODE *getsectnodefromid(struct SECTMENTSETNODE *head,int id)
{
    struct SECTMENTSETNODE *next;
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




void deletenode(struct SECTMENTSETNODE *head,char  *name)
{

    if(head==NULL)
        return;

    if(head->next==NULL)
    return;

    struct SECTMENTSETNODE *pre,*next;
    pre=head;
    next=pre->next;
    while(next!=NULL)
    {
        if(strcmp(next->value.name,name)==0)
        {
            pre->next=next->next;
            return;
        }
        pre=next;
        next=next->next;
    }

}

void printnode(struct SECTMENTSETNODE *head)
{
    if(head==NULL||head->next==NULL)
        return;
    struct SECTMENTSETNODE *next=head->next;
    while(next!=NULL)
    {
    qDebug()<<__func__<<__LINE__<<next->value.curset.valid<<next->value.curset.done;
    qDebug()<<__func__<<__LINE__<<next->value.volset.valid<<next->value.volset.done;
    qDebug()<<__func__<<__LINE__<<next->value.freset.valid<<next->value.freset.done;
    qDebug()<<__func__<<__LINE__<<next->value.dirset.valid<<next->value.dirset.done;
    qDebug()<<__func__<<__LINE__<<next->value.phraseset.valid<<next->value.phraseset.done;
    qDebug()<<__func__<<__LINE__<<next->value.anytestset.valid<<next->value.anytestset.done;
    next=next->next;
    }

}


void printsectment(struct SECTMENTSET *sectment)
{


    if(sectment->curset.valid) //电流有效
    {

    }


    if(sectment->volset.valid) //电压有效
    {

    }

    if(sectment->dirset.valid) //方向有效
    {

    }

    if(sectment->freset.valid) //方向有效
    {

    }

    if(sectment->anytestset.valid) //任意测试有效
    {

    }

}

void copysectmentlink(struct SECTMENTSETNODE *dest,struct SECTMENTSETNODE *src)
{
    if(src==NULL||src->next==NULL||dest==NULL)
     return;
   struct SECTMENTSETNODE *next=src->next;
    struct SECTMENTSET value;
   int id=0;
   while(next!=NULL)
   {
       memset(&value,0,sizeof(struct SECTMENTSET));
       memcpy(&value,&next->value,sizeof(struct SECTMENTSET));
       insertnode(dest,value,id);
       id++;
       next=next->next;
   }
}

void setdone(struct SECTMENTSETNODE *head)
{


    if(head==NULL||head->next==NULL)
    return;
    struct SECTMENTSETNODE *next=head->next;
    int Testitem;
    while(next!=NULL)
    {
        if(next->value.anytestset.valid)
        next->value.anytestset.done=true;

        if(next->value.curset.valid)
      {
         next->value.curset.done=true;
         Testitem=atoi(next->value.curset.Testitem.text);
         next->value.curset.TripValue.valid=Testitem&0x01;
         next->value.curset.TripTime.valid=Testitem&0x02;
      }

        if(next->value.volset.valid)
       {
           next->value.volset.done=true;
           Testitem=atoi(next->value.volset.Testitem.text);
           next->value.volset.TripValue.valid=Testitem&0x01;
           next->value.volset.TripTime.valid=Testitem&0x02;
       }

        if(next->value.freset.valid)
       {
          next->value.freset.done=true;
          Testitem=atoi(next->value.freset.TestItem.text);
          next->value.freset.TripValue.valid=Testitem&0x01;
          next->value.freset.TripTime.valid=Testitem&0x02;
          next->value.freset.Tripdfdt.valid=Testitem&0x04;
          next->value.freset.Tripdfdttime.valid=Testitem&0x08;
          next->value.freset.TripVBlock.valid=Testitem&0x10;
       }


        if(next->value.dirset.valid)
       {
         next->value.dirset.done=true;
         next->value.dirset.valid=true;
       }

        if(next->value.phraseset.valid)
       {
         next->value.phraseset.done=true;
         Testitem=atoi(next->value.phraseset.Testitem.text);
         next->value.phraseset.TripValue.valid=Testitem&0x01;
         next->value.phraseset.TripTime.valid=Testitem&0x02;

       }

        next=next->next;
    }



}



