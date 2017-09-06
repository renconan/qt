#include "format.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <QString>
#include <QDebug>


//���ID���ڣ��򲻲���
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
    if(next2!=NULL) //���غ�ID��
     return;

    struct SETTINGNODELIST *tmp;
    if((tmp=(struct SETTINGNODELIST *)malloc(sizeof(struct SETTINGNODELIST)))==NULL)
    {
     printf("can not malloc space\n");
     exit(0);
    }
    tmp->next=NULL;
   // tmp->set=value;
    strcpy(tmp->set.ID,value.ID);
    strcpy(tmp->set.name,value.name);
    strcpy(tmp->set.value,value.value);
    //strcpy(tmp->set.ref,value.ref);

    struct SETTINGNODELIST *pre,*next;  //�ҵ���ǰ
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
         //  <<"ref:"<<tmp->set.ref;
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
         memcpy(&value,&tmp->set,sizeof(SETTING));
         tmp->set.isprint=false;
         return ;
       }
          tmp=tmp->next;
    }
}

void setsettinglinknoprint(struct SETTINGNODELIST *head) //����δ����ӡ
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


