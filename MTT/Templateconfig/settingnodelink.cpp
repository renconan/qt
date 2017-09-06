#include "format.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <QString>
#include <QDebug>


//如果ID存在，则不插入
void insertsetnode(struct SETTINGNODELIST *head,SETTING value)
{

    if(head==NULL)
    return;
    struct SETTINGNODELIST *next2;
    next2=head->next;
    while(next2!=NULL)
    {
        if(QString(QLatin1String(next2->set.ID))==QString(QLatin1String(value.ID)))
         break;
        next2=next2->next;
    }
    if(next2!=NULL) //有重合ID号
     return;

    struct SETTINGNODELIST *tmp;
    if((tmp=(struct SETTINGNODELIST *)malloc(sizeof(struct SETTINGNODELIST)))==NULL)
    {
     printf("can not malloc space\n");
     exit(0);
    }
    tmp->next=NULL;
    memcpy(&tmp->set,&value,sizeof(SETTING));


    struct SETTINGNODELIST *pre,*next;  //找到当前
    pre=head;
    next=pre->next;
    while(next!=NULL)
    {
     pre=next;
     next=next->next;
    }
    pre->next=tmp;
}

void printfsetting(struct SETTINGNODELIST *head)
{
    struct SETTINGNODELIST *tmp;
    if(head!=NULL&&head->next!=NULL)
    {
        tmp=head->next;
        while(tmp!=NULL)
        {
            qDebug()<<__func__<<__LINE__<<"ID:"<<tmp->set.ID\
                                       <<"name:"<<tmp->set.name\
                                       <<"value:"<<tmp->set.value;
                                    //  <<"Ref"<<tmp->set.ref;
            tmp=tmp->next;
        }
    }
}

QString getnamefromid(struct SETTINGNODELIST *head,char *ID)
{
 struct SETTINGNODELIST *tmp;

 if(head->next==NULL)
     return QString(QLatin1String(""));

 tmp=head->next;
 while(tmp!=NULL)
 {
     if(memcmp(ID,tmp->set.ID,strlen(ID))==0)
      {
         return QString(QLatin1String(tmp->set.name));
      }
     tmp=tmp->next;
 }
 return QString(QLatin1String(""));
}

QString getvaluefromid(struct SETTINGNODELIST *head,char *ID)
{
    struct SETTINGNODELIST *tmp;

    if(head->next==NULL)
        return QString(QLatin1String(""));
    tmp=head->next;

    while(tmp!=NULL)
    {

         if(strcmp(ID,tmp->set.ID)==0)
       {
          return QString(QLatin1String(tmp->set.value));
       }
          tmp=tmp->next;
    }
    return QString(QLatin1String(""));
}

void setsettinglinknoprint(struct SETTINGNODELIST *head) //设置未被打印
{
    if(head==NULL||head->next==NULL)
        return;
    struct SETTINGNODELIST *next=head->next;
    while(next!=NULL)
    {
        next->set.isprint=true;
        next=next->next;

    }

}





void  getsettingfromid(struct SETTINGNODELIST *head,char *ID,SETTING &value)
{
    struct SETTINGNODELIST *tmp;

    if(head->next==NULL||head==NULL)
        return ;
    tmp=head->next;
    while(tmp!=NULL)
    {

         if(strcmp(ID,tmp->set.ID)==0)
       {
         strcpy(value.ID,tmp->set.ID);
         strcpy(value.name,tmp->set.name);
         strcpy(value.value,tmp->set.value);
        // strcpy(value.ref,tmp->set.ref);
         return ;
       }
          tmp=tmp->next;
    }
}

