#include "CaculateVar.h"
#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>

#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QDir>
#include <QDate>
#include <QTime>

#include "format.h"
#include "settingnodelink.h"

#include "nodeprocess.h"

void strrpl(char *pSrcIn,char *pDstOut,const char* pSrcRpl, const char* pDstRpl)
{
char* pi = pSrcIn;
char* po = pDstOut;
int nSrcRplLen = strlen(pSrcRpl);
int nDstRplLen = strlen(pDstRpl);

char *p = NULL;
int nLen = 0;

do
{
// �ҵ���һ���滻��
p = strstr(pi, pSrcRpl);

if(p != NULL)
{
// ������һ���滻�����һ���滻���м���ַ���
nLen = p - pi;
memcpy(po, pi, nLen);

// ������Ҫ�滻���ַ���
memcpy(po + nLen, pDstRpl, nDstRplLen);
}
else
{
strcpy(po, pi);

// ���û����Ҫ�������ַ���,˵��ѭ��Ӧ�ý���
break;
}

pi = p + nSrcRplLen;
po = po + nLen + nDstRplLen;

} while (p != NULL);
}


void StrReplace( char *pSrcIn,char *pDstOut, char *pStart, const char *pSrcRpl, const char *pDstRpl)
{

    char   *pi=pSrcIn;
    char   *po=pDstOut;
    char *p = NULL;
    // �ҵ���һ���滻��
    p=pStart;
    if(p!=NULL)
    {
    if(strlen(p)==strlen(pSrcIn)) //������׵�ַ��
   {
     char Tempchar1[512];
     char Tempchar2[512];
    memset(Tempchar1,'\0',512);
    strcpy(Tempchar1,p+strlen(pSrcRpl));
    sprintf(Tempchar2,"%s%s",pDstRpl,Tempchar1);
    strcpy(po, Tempchar2);
   }
    else //�����׵�ַ��
    {
       char Tempchar1[512];
       char Tempchar2[512];
       char Tempchar3[512];

       int PreLen=strlen(pSrcIn)-strlen(p);
       memcpy(Tempchar3,pSrcIn,PreLen);
       Tempchar3[PreLen]='\0';

       memset(Tempchar1,'\0',512);
       strcpy(Tempchar1,p+strlen(pSrcRpl));

       sprintf(Tempchar2,"%s%s%s",Tempchar3,pDstRpl,Tempchar1);
       strcpy(po, Tempchar2);

    }

    }
    else
    {      
    strcpy(po, pi);
    // ���û����Ҫ�������ַ���,˵��ѭ��Ӧ�ý���
    }
}




float RelaceExp(char *exp,struct SETTINGNODELIST *settingnodelist) //���ʽ��ID���ֵĴ���
{
    if(settingnodelist==NULL||settingnodelist->next==NULL)
    {
       return -9999;
    }
    struct SETTINGNODELIST *nextsetting=settingnodelist->next;
    char Expin[1024];
    char Expout[1024];
    SETTING setting;
    char *p1;
    strcpy(Expin,exp);

    while(nextsetting!=NULL)
    {


        char *p3=Expin;
        p1=strstr(p3,nextsetting->set.ID);
        while(p3!=NULL&&*p3!='\0'&&p1!=NULL)
        {
          if(p1!=NULL)
          {
           int len=strlen(nextsetting->set.ID);
           bool Valid=*(p1+len)=='+'||*(p1+len)=='-'||*(p1+len)=='/'||*(p1+len)=='*'||*(p1+len)==')'||*(p1+len)=='\0';

           bool Valid2=false;
           char *pstart=p1-1;
           if(strlen(p1)!=strlen(p3))
           {
               if(*pstart=='+'||*pstart=='-'||*(pstart)=='/'||*pstart=='*'||*pstart=='(')
               {
                  Valid2=true;

               }

           }
           else
             Valid2=true;


           if(Valid==true&&Valid2==true)
           {
            float Value;
            getsettingfromid(settingnodelist,nextsetting->set.ID,setting);
            Value=atof(setting.value);
            if(Value>0)
            StrReplace(Expin,Expout,p1,nextsetting->set.ID,setting.value);
            else
            {
             float Value2=-Value;
             char Str[512];
             sprintf(Str,"(0-%f)",Value2);
             StrReplace(Expin,Expout,p1,nextsetting->set.ID,Str);
            }
            memset(Expin,'\0',1024);
            strcpy(Expin,Expout);
            p3=p1+strlen(setting.value);
           }
          else
             p3=p1+strlen(nextsetting->set.ID);
          }
          p1=strstr(p3,nextsetting->set.ID);
        }

        nextsetting=nextsetting->next;
    }

     bool ok;
     QString(Expin).toFloat(&ok);

     if(ok)
     return QString(Expin).toFloat(&ok);
     else
     return CaculatefromExp(QString(Expin));
}

void CaculateVariable(struct SETTINGNODELIST *settingnodelist,VARIABLE &exp)
{
    bool ok;
    QString str=QString(exp.text);
    str.toFloat(&ok);
    if(ok)
    {
     exp.value=str.toFloat(&ok);
    }
    else
    {
        char buffer[1024]={'\0'};
        strcpy(buffer,exp.text);

        exp.value=RelaceExp(buffer,settingnodelist);
    }

}

int CheckTextValid(struct SETTINGNODELIST *settingnodelist,QString str)
{

    bool ok;
    if(settingnodelist==NULL||settingnodelist->next==NULL)
    {
       return -1;
    }
    str.toFloat(&ok);
    if(ok==true)
        return 0;




    struct SETTINGNODELIST *nextsetting=settingnodelist->next;
    char Expin[1024];
    char Expout[1024];
    SETTING setting;
    char *p1;
    QByteArray ba=str.toLatin1();
    char *exp=ba.data();
    strcpy(Expin,exp);
    while(nextsetting!=NULL)
    {
        char *p3=Expin;
        p1=strstr(p3,nextsetting->set.ID); //�Ƿ����һ�����ֶ�


        while(p3!=NULL&&*p3!='\0'&&p1!=NULL)
        {
          if(p1!=NULL)
          {
           int len=strlen(nextsetting->set.ID); //��ID�ĳ���
           bool Valid=*(p1+len)=='+'||*(p1+len)=='-'||*(p1+len)=='/'||*(p1+len)=='*'||*(p1+len)==')'||*(p1+len)=='\0';
           bool Valid2=false;
           char *pstart=p1-1;
           if(strlen(p1)!=strlen(p3))
           {
               if(*pstart=='+'||*pstart=='-'||*(pstart)=='/'||*pstart=='*'||*pstart=='(')
               {
                  Valid2=true;

               }

           }
           else
             Valid2=true;



           if(Valid==true&&Valid2==true)
           {
            float Value;
            getsettingfromid(settingnodelist,nextsetting->set.ID,setting);
            Value=atof(setting.value);
            if(Value>0)
            StrReplace(Expin,Expout,p1,nextsetting->set.ID,setting.value);
            else
            {
             float Value2=-Value;
             char Str[512];
             sprintf(Str,"(0-%f)",Value2);
             StrReplace(Expin,Expout,p1,nextsetting->set.ID,Str);
            }
            memset(Expin,'\0',1024);
            strcpy(Expin,Expout);
            p3=p1+strlen(setting.value);
           }
           else
             p3=p1+strlen(nextsetting->set.ID);
          }

          p1=strstr(p3,nextsetting->set.ID);
        }

        nextsetting=nextsetting->next;
    }


     QString(Expin).toFloat(&ok);
     if(ok)            //�滻���ֱ��ת������
     return 1;




     for(int i=0;i<strlen(Expin);i++)
     {

        bool valid=(Expin[i]>='0'&&Expin[i]<='9')||\
                   (Expin[i]=='+'||Expin[i]=='-'||Expin[i]=='*'||Expin[i]=='/'||\
                    Expin[i]=='('||Expin[i]==')'||Expin[i]=='.');
        bool Valid2=Expin[strlen(Expin)-1]=='+'||\
                    Expin[strlen(Expin)-1]=='-'||\
                    Expin[strlen(Expin)-1]=='*'||\
                    Expin[strlen(Expin)-1]=='/'||\
                    Expin[strlen(Expin)-1]=='.'||\
                    Expin[strlen(Expin)-1]=='(';

        if(valid==false||Valid2==true)
            return -1;
     }

     if(Expin[strlen(Expin)-1]==')')
     {
       int i=0;
       for(i=0;i<strlen(Expin);i++)
       {
           if(Expin[i]=='(')
            return 1;
       }
       return -1;
     }

     return 1;

}



