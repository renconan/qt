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

    struct SECTMENTSETNODE *next_1=head->next;
    while(next_1!=NULL)
    {
        if(strcmp(next_1->value.name,value.name)==0)
        {
            memcpy(&next_1->value,&value,sizeof(struct SECTMENTSET));
            return;
        }
        next_1=next_1->next;
    }


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
void insertnodeIndex(struct SECTMENTSETNODE *head,struct SECTMENTSET value,int id)
{

    if(head==NULL)
    return;



    struct SECTMENTSETNODE *pre,*next;  //找到当前
    pre=head;
    next=pre->next;
    int cnt = 0;

//    if((id-1)== 0)
//    {
//        next = head->next;
//    }

    while(((id-1)>cnt))
    {
     pre=next;
     next=next->next;
     cnt++;
    }

    struct SECTMENTSETNODE *tmp;
    if((tmp=(struct SECTMENTSETNODE *)malloc(sizeof(struct SECTMENTSETNODE)))==NULL)
    {
     printf("can not malloc space\n");
     exit(0);
    }
    tmp->next=NULL;
    memcpy(&tmp->value,&value,sizeof(struct SECTMENTSET));
    tmp->id=id;
    tmp->next=next->next;
    next->next=tmp;


}





//删除ID 的节点
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


void printnode(struct SECTMENTSETNODE *head)
{
    if(head==NULL||head->next==NULL)
        return;
    struct SECTMENTSETNODE *next=head->next;
    while(next!=NULL)
    {
        qDebug()<<__func__<<__LINE__<<next->value.name;
        qDebug()<<__func__<<__LINE__<<next->value.curset.valid\
        <<next->value.volset.valid\
        <<next->value.dirset.valid\
        <<next->value.freset.valid\
        <<next->value.anytestset.valid;

        next=next->next;
    }


}

/*
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
*/
