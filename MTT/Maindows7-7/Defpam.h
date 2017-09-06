#ifndef DEFPAM_H
#define DEFPAM_H

#define BUFFSIZE 512

#define ACTIVE 0 //活跃状态
#define INACTIVE -1 //非活跃状态
#define VALID 0 //有效
#define INVALID -1 //无效
#define FULL 0
#define EMPTY -1
#define CRF          	0x0D	//回车符'\r'
#define LFF          	0x0A	//换行符'\n'
#define BACKSPACE   	0x08	//退格符
#define SPACEBAR    	0x20	//空格符
#define TAB        	0x09	//制表符



typedef unsigned char uint8 ;
typedef unsigned short uint16;

#define MAX_UARTDEV_NUM 2	 //系统最多5 个串口设备 ttyS*+TTYUSB
#define MAX_UART_BUFFLEN 1024 //应用层串口接收缓冲长度

typedef struct {
uint8 valid;          //是否启用 '1'启用，’0‘关闭
uint8  uartfd;        //设备号,此号将会动态分配
char devname[16]; //设备名
int  baud;	  //波特率 默认9600
uint8  datanum; //默认 8
char parity;  //默认 N
uint8  stopbit; //默认1
uint8 rs485Mode;//是否启用RS485模式  ’1‘ 485，’0‘ 232
char describ[32];
}UARTDEV;

typedef struct{
char rcvbuff[MAX_UART_BUFFLEN];//最大长度
int  rcvlength;//串口接收长度
int  writesequence;	//读写序列
int  readsequence;	//读写序列
}UARTRCV;





typedef struct _TEMPRORARYDATA{
UARTDEV   UartDev[MAX_UARTDEV_NUM]; //串口设备参数
UARTRCV   UartRcv[MAX_UARTDEV_NUM]; //接收到的数据

int gpio_fd;
}TEMPRORARYDATA;






struct NODELINK
{
    char name[50];
    int ID;
    struct NODELINK *next;
};





typedef struct _FuncFile{
    char filename[50];   //英文名字
    char cnfilename[50]; //中文名字
}FuncFile;







#endif // DEFPAM_H


