#include "caculate.h"
#include <QStack>
#include <QChar>
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
        char *ch;
        char Exptmp[512];
        QByteArray ba= exp.toLatin1();
        ch=ba.data();
        if(exp.at(0).toLatin1()=='-')
        {
            sprintf(Exptmp,"0%s",ch);
            toPostfix(QString(Exptmp));
        }
        else
        toPostfix(exp);

        result=evaluation();
        init();
        return result;

}
