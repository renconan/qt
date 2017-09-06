#ifndef THREAD1_H
#define THREAD1_H
#include <QThread>


#ifdef ARMLINUX
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <math.h>
#include <dirent.h>
#include <sys/vfs.h>
#endif
#include <QTimer>
#include "Defpam.h"
#include "Config.h"
#include "Testcom2.h"
#include "differseq.h"
#include "KEY.h"



class thread1 : public QThread
{
    Q_OBJECT
public:
    explicit thread1(QObject *parent = 0);
    void run();

    void key_response(KEY key);  //��Ҫ����
    int  Buzzer(char Address1,char Address2,char Address3);
    void logic_info(int fd,unsigned char info_result1,unsigned char info_result2,unsigned char info_result3,unsigned char info_result4);
    void ReadTripTimeLogic(int fd);
    void key_deal_test_current(int fd);
    void key_deal_stop_current(int fd);
    void DisplayResult(int ReslutSum);

    void OutputAngle(int fd);
    void OutputAngle1(int fd);
    void OutputAngle2(int fd);
    void OutputAngle3(int fd);
    void OutputAngle4(int fd);
    void OutputAngle5(int fd);
    void OutputAngle6(int fd);
    void OutputAngle7(int fd);
    void OutputAngle8(int fd);
    void Getresult();

    void stoprun();


signals:
    void   SIG_dischange(float value);
    void   SIG_binarystate(int state);
    void   SIG_dspstate(int state);
    void   SIG_testresult(int num);
    void   SIG_sendwarning(int num); //sectnum�κ� ���ʹ�����Ϣ 1 ��ѹ����ϵͳ��ѹ 2 С��ϵͳ��ѹһ��
    void   SIG_Teststate(int);
    void   SIG_SendClose();
    void   SIG_TestState();
};

#endif // THREAD1_H
