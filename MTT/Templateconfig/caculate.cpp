﻿#include "caculate.h"
#include <QStack>
#include <QChar>
#include <QString>
#include <QtCore>
#include <exception>
#include "format.h"
#include "settingnodelink.h"
#include <QString>



QStack<QChar> expStack;//后缀表达式栈
QStack<QChar> opStack;//运算符栈
QString postfix;//存储后缀表达式


void init()
{
    if(!expStack.isEmpty())
        expStack.clear();
    if(!opStack.isEmpty())
        opStack.clear();
    opStack.push('#');


}

int  getLevel(const QChar &oper)
{
    switch(oper.cell())
    {
    case '#':
    case '(':return 0;
    case '+':
    case '-':return 1;
    case '*':
    case '/':
    case '%':return 2;
    }
    return 0;
}
//转换为后缀表达式，实现最主要功能
void toPostfix(QString exp)
{
   // QString exp =QString("((2*3/2-2*5)/2-5*1.2)*2-2*4");
    int j=0;
    for(int i=0;i<exp.length();i++)
    {
        if(exp[i].isDigit()||exp[i]=='.')
        {
            postfix.push_back(exp[i]);
        }
        else if(exp[i]=='(')
        {
            opStack.push(exp[i]);
        }
        else if(exp[i]==')')
        {
            postfix.push_back(' ');
            while(opStack.top()!='(')
            {
                postfix.push_back(opStack.pop());

            }
            opStack.pop();
        }
        else if(getLevel(exp[i])>getLevel(opStack.top()))
        {
            postfix.push_back(' ');
            opStack.push(exp[i]);
        }
        else
        {
            postfix.push_back(' ');
            while(getLevel(exp[i])<=getLevel(opStack.top()))
                postfix.push_back(opStack.pop());
            opStack.push(exp[i]);
        }
    }
    while(opStack.top()!='#')
    {
        QChar c = opStack.pop();
        postfix.push_back(' ');
        postfix.push_back(c);
    }

}


float evaluation()
{
    QString tem;
    QStack<double> ans;

    for(int i=0;i<postfix.size();i++)
    {
        if(postfix[i].isDigit()||postfix[i]=='.')
            tem.push_back(postfix[i]);
        else if(postfix[i]==' ')
        {
            if(!tem.isEmpty())
            {
                ans.push(tem.toDouble());
                tem.clear();
            }
        }
        else
        {
            double a,b;
            a=ans.pop();
            b=ans.pop();
            switch(postfix[i].cell())
            {
            case '+':ans.push(b+a);break;
            case '-':ans.push(b-a);break;
            case '*':ans.push(b*a);break;
            case '/':ans.push(b/a);break;
            case '%':ans.push((int)a%(int)b);break;
            }

        }

    }

    return ans.top();
}


float CaculatefromExp(QString exp)
{
    float result;
    init();
    toPostfix(exp);

    result=evaluation();
    init();
    return result;

}



void StrReplace( char *pSrcIn,char *pDstOut, char *pStart, const char *pSrcRpl, const char *pDstRpl)
{

    char   *pi=pSrcIn;
    char   *po=pDstOut;
    char *p = NULL;
    // 找到下一个替换点
    p=pStart;
    if(p!=NULL)
    {
    if(strlen(p)==strlen(pSrcIn)) //如果在首地址上
   {
     char Tempchar1[512];
     char Tempchar2[512];
    memset(Tempchar1,'\0',512);
    strcpy(Tempchar1,p+strlen(pSrcRpl));
    sprintf(Tempchar2,"%s%s",pDstRpl,Tempchar1);
    strcpy(po, Tempchar2);
   }
    else //不在首地址上
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
    // 如果没有需要拷贝的字符串,说明循环应该结束
    }
}





float RelaceExp(char *exp,struct SETTINGNODELIST *settingnodelist) //表达式中ID出现的次数
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
           if(Valid==true)
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
        p1=strstr(p3,nextsetting->set.ID); //是否存在一样的字段

       // if(p3!=NULL&&*p3!='\0'&&p1!=NULL&&strlen(p1)!=strlen(p3))
        while(p3!=NULL&&*p3!='\0'&&p1!=NULL)
        {
          if(p1!=NULL)
          {
           int len=strlen(nextsetting->set.ID); //此ID的长度
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
     if(ok)  //替换后可直接转换数字
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



