#include "config.h"
#include "Defpam.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <QString>
#include <QDebug>





void insertnodelink(struct NODELINK *head,char *value,int id)
{

    if(head==NULL||value==NULL)
    return;

    struct NODELINK *next2;
    next2=head->next;
    while(next2!=NULL)
    {
       // if(memcmp(next2->name,value,strlen(next2->name))==0)
        if(memcmp(value,next2->name,strlen(value))==0)
        break;
        next2=next2->next;
    }
    if(next2!=NULL) //有重合ID号
     return;

    struct NODELINK *tmp;
    if((tmp=(struct NODELINK *)malloc(sizeof(struct NODELINK)))==NULL)
    {
     printf("can not malloc space\n");
     exit(0);
    }
    tmp->next=NULL;
    strcpy(tmp->name,value);
    tmp->ID=id;
    struct NODELINK *pre,*next;  //找到当前
    pre=head;
    next=pre->next;
    while(next!=NULL)
    {
     pre=next;
     next=next->next;
    }

    pre->next=tmp;


}


void deletenodelink(struct NODELINK *head,char *value)
{
    if(head==NULL||value==NULL||head->next==NULL)
    return;

    struct NODELINK *pre,*next;
    pre=head;
    next=pre->next;
    while(next!=NULL)
    {
        if(memcmp(value,next->name,strlen(next->name))==0)
        {

         break;
        }
        pre=next;
        next=next->next;
    }
    if(next!=NULL)
    {
    pre=next->next;

}
}

void printfnodelink(struct NODELINK *head)
{
    struct NODELINK *next;
    if(head==NULL||head->next==NULL)
        return;
    next=head->next;
    while(next!=NULL)
    {
        printf("%s  %d  %s  %d\n",__func__,__LINE__,next->name,next->ID);
        next=next->next;
    }



}


bool geditnamefromid(struct NODELINK *head,int id,char *name)
{
    struct NODELINK *next;
    if(head==NULL||head->next==NULL)
        return false;
    next=head->next;

    while(next!=NULL)
    {
        if(next->ID==id)
        {
            strcpy(name,next->name);
            return true;
        }
        next=next->next;
    }
     return false;


}
