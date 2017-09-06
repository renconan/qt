#include "format.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <QString>
#include <QDebug>


//如果ID存在，则不插入
void insertsetnode_2(struct SETTINGNODEIDLIST *head,SETTINGID value)
{

    if(head==NULL)
    return;
    struct SETTINGNODEIDLIST *next2;
    next2=head->next;
    while(next2!=NULL)
    {
        if(QString(QLatin1String(next2->set.ID))==QString(QLatin1String(value.ID)))
         break;
        next2=next2->next;
    }
    if(next2!=NULL) //有重合ID号
     return;

    struct SETTINGNODEIDLIST *tmp;
    if((tmp=(struct SETTINGNODEIDLIST *)malloc(sizeof(struct SETTINGNODEIDLIST)))==NULL)
    {
     printf("can not malloc space\n");
     exit(0);
    }
    tmp->next=NULL;
    tmp->set.index=value.index;
    strcpy(tmp->set.ID,value.ID);
    strcpy(tmp->set.name,value.name);
    strcpy(tmp->set.value,value.value);
   // strcpy(tmp->set.ref,value.ref);

    struct SETTINGNODEIDLIST *pre,*next;  //找到当前
    pre=head;
    next=pre->next;
    while(next!=NULL)
    {
     pre=next;
     next=next->next;
    }
    pre->next=tmp;
}

void printfsetting_2(struct SETTINGNODEIDLIST *head)
{
    struct SETTINGNODEIDLIST *tmp;
    if(head!=NULL&&head->next!=NULL)
    {
        tmp=head->next;
        while(tmp!=NULL)
        {
            qDebug()<<__func__<<__LINE__<<"ID:"<<tmp->set.ID\
           <<"name:"<<tmp->set.name\
           <<"value:"<<tmp->set.value\
            <<"index:"<<tmp->set.index;
          //  <<"ref:"<<tmp->set.ref;
            tmp=tmp->next;
        }
    }
}

QString getnamefromid_2(struct SETTINGNODEIDLIST *head,char *ID)
{
 struct SETTINGNODEIDLIST *tmp;


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

QString getvaluefromid_2(struct SETTINGNODEIDLIST *head,char *ID)
{
    struct SETTINGNODEIDLIST *tmp;

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

bool getsettingidfromindex_2(struct SETTINGNODEIDLIST *head,int index,SETTINGID &value)
{


    if(head==NULL||head->next==NULL)
    {
        return false;
    }
    struct SETTINGNODEIDLIST *next=head->next;
    while(next!=NULL)
    {
        if(next->set.index==index)
        {
            strcpy(value.ID,next->set.ID);
            strcpy(value.name,next->set.name);
            strcpy(value.value,next->set.value);
           // strcpy(value.ref,next->set.ref);
            value.index=index;
            return true;
        }
        next=next->next;
    }
    return false;
}

void setsettingvaluefromindex_2(struct SETTINGNODEIDLIST *head,int index,QString value)
{
    if(head==NULL||head->next==NULL)
        return;
    struct SETTINGNODEIDLIST *next=head->next;

    QByteArray ba=value.toLatin1();
    char *mm=ba.data();
    while(next!=NULL)
    {
        if(next->set.index==index)
        {
         strcpy(next->set.value,mm);
         return;
        }
        next=next->next;
    }
}
