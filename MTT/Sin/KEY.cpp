#include "KEY.h"
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <QDebug>

KEY GetKey(int fd)
{

       int rd_len=0;
       KEY key=_NOP;
       int max_fd;
       unsigned int buff[10]={0};
       struct timeval timeout={0,0};
        max_fd=fd+1;
        fd_set rfdsUart;
        FD_ZERO(&rfdsUart);
        FD_SET(fd,&rfdsUart);
         if(select(max_fd,&rfdsUart,NULL,NULL,&timeout)>0)
        {

         if(FD_ISSET(fd,&rfdsUart)) //����sock�Ƿ�ɶ������Ƿ�������������
         {
          rd_len=read(fd,buff,1);
          FD_ZERO(&rfdsUart);// ��մ��ڽ��ն˿ڼ�
          FD_SET(fd,&rfdsUart);// ���ô��ڽ��ն˿ڼ�
          if(rd_len<=0)
          {
          return _NOP;
          }


         switch(buff[0]) //buff[3]?a?��?��
          {
        case 0x23: key=_coarse_left;break;
        case 0x24: key=_coarse_right;break;
        case 0x25: key=_fine_left;break;
        case 0x26: key=_fine_right;break;
        case 0x27: key=_enter;break;
        case 0x28: key=_TEST;break;
        case 0x29:key=_STOP;break;
        default:break;
          }
       // qDebug()<<__func__<<__LINE__<<buff[0];

       }
      }


   return key;
}

