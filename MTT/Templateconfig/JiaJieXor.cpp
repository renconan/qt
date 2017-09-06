#include "JiaJieXor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void jiamiXor(char *str, char key2,char *result)
{

    for(int i=0;i<strlen(str); i++)
    result[i]= ~str[i]+key2;
    result[strlen(str)]='\0';
}

void jiemiXor(char *str, char key2,char *result)
{   char tmpchar;
    for(int i=0;i<strlen(str); i++)
     {
        tmpchar=str[i]-key2;
        result[i]=~tmpchar;
     }
    result[strlen(str)]='\0';
}


unsigned long Check(char *str)
{ unsigned int  i=0x01;
  unsigned int  m,j;
  unsigned long  Recheck=0;
  char *p1=str;
  for(m=0;m<strlen(str);m++)
  { i=0x01;
    for(j=0;j<=7;j++)
    { if(*p1&i)
      {
       Recheck++;
      }
      i=i<<1;
    }
    p1++;
  }
  return  Recheck;
}

