#ifndef UARTSET_H
#define UARTSET_H



void temprorarydatainit();
void UartsInit();
void UartInit(int uartno);
void my_itoa( int val,char *buf);
int  OpenDev(char *Dev);



int set_speed(int fd, int speed);
int set_Parity(int fd,int databits,char parity,int stopbits);
int UartSend(int fd,char *cmd,int length);
int UartFlush(int fd);
void str2STR(char *str);            //ÃüÁî×Ö·ûÈ«×ª´óĞ´



#endif // UARTSET_H
