#ifndef DEFPAM_H
#define DEFPAM_H

#define BUFFSIZE 512

#define ACTIVE 0 //��Ծ״̬
#define INACTIVE -1 //�ǻ�Ծ״̬
#define VALID 0 //��Ч
#define INVALID -1 //��Ч
#define FULL 0
#define EMPTY -1
#define CRF          	0x0D	//�س���'\r'
#define LFF          	0x0A	//���з�'\n'
#define BACKSPACE   	0x08	//�˸��
#define SPACEBAR    	0x20	//�ո��
#define TAB        	0x09	//�Ʊ��



typedef unsigned char uint8 ;
typedef unsigned short uint16;

#define MAX_UARTDEV_NUM 2	 //ϵͳ���5 �������豸 ttyS*+TTYUSB
#define MAX_UART_BUFFLEN 1024 //Ӧ�ò㴮�ڽ��ջ��峤��

typedef struct {
uint8 valid;          //�Ƿ����� '1'���ã���0���ر�
uint8  uartfd;        //�豸��,�˺Ž��ᶯ̬����
char devname[16]; //�豸��
int  baud;	  //������ Ĭ��9600
uint8  datanum; //Ĭ�� 8
char parity;  //Ĭ�� N
uint8  stopbit; //Ĭ��1
uint8 rs485Mode;//�Ƿ�����RS485ģʽ  ��1�� 485����0�� 232
char describ[32];
}UARTDEV;

typedef struct{
char rcvbuff[MAX_UART_BUFFLEN];//��󳤶�
int  rcvlength;//���ڽ��ճ���
int  writesequence;	//��д����
int  readsequence;	//��д����
}UARTRCV;





typedef struct _TEMPRORARYDATA{
UARTDEV   UartDev[MAX_UARTDEV_NUM]; //�����豸����
UARTRCV   UartRcv[MAX_UARTDEV_NUM]; //���յ�������

int gpio_fd;
}TEMPRORARYDATA;






struct NODELINK
{
    char name[50];
    int ID;
    struct NODELINK *next;
};





typedef struct _FuncFile{
    char filename[50];   //Ӣ������
    char cnfilename[50]; //��������
}FuncFile;







#endif // DEFPAM_H


