#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <math.h>
#include <KEY.h>
#include <QTimer>
#include <QProcess>
#include <QDir>

#include "Config.h"
#include "thread1.h"
#include "Gpio.h"
#include "Uartset.h"
#include "Testcom2.h"
#include "Nodelinkmanage.h"

#include <QSemaphore>
#include "em335x_drivers.h"
#define LEDNUM GPIO8

extern char SerialNumber[512];
extern unsigned char result[70],receive_data[70];
QString serialnumbers,dspnumbers,datenumbers;
QString a1,a2,a3,a4,a5,a6;


extern int current_language;  //1中文  2英文
QSemaphore sem(1);
TEMPRORARYDATA *temprorarydata=NULL;
extern int A_SumStepnum;
bool Threadrunstate;
bool ManulIsSendcmd=false;
int save_f;
float LogicResolution;
bool SoftStartEndflag=false;


extern double max_output_v_a;   //,max_f_v_a=50,min_f_v_a=0.5,output_offset_v_a=0,amp_v_a=360;
extern double max_output_v_b;   //,max_f_v_b=50,min_f_v_b=0.5,output_offset_v_b=0,amp_v_b=120;
extern double max_output_v_c;   //,max_f_v_c=50,min_f_v_c=0.5,output_offset_v_c=0,amp_v_c=240;
extern double max_output_v_z;
extern double max_output_i_a;//,max_f_i_a=50,min_f_i_a=0.5,output_offset_i_a=0,amp_i_a=75;
extern double max_output_i_b;//,max_f_i_b=50,min_f_i_b=0.5,output_offset_i_b=0,amp_i_b=195;
extern double max_output_i_c;//,max_f_i_c=50,min_f_i_c=0.5,output_offset_i_c=0,amp_i_c=315;

float ChangeV1Value,ChangeV1Phase,ChangeV1Frec;
float ChangeV2Value,ChangeV2Phase,ChangeV2Frec;
float ChangeV3Value,ChangeV3Phase,ChangeV3Frec;
float ChangeVzValue,ChangeVzPhase,ChangeVzFrec;
float ChangeI1Value,ChangeI1Phase,ChangeI1Frec;
float ChangeI2Value,ChangeI2Phase,ChangeI2Frec;
float ChangeI3Value,ChangeI3Phase,ChangeI3Frec;

float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;
int   T4;
float TripValue,TripTime;
int StartTestFlag;
int   sys[4];
int binOut = 0;
//串口设置如下
thread1::thread1(QObject *parent) :
    QThread(parent)
{
    temprorarydata=new TEMPRORARYDATA;
    ManualOutputFlag=false;

}

void thread1::WriteSerialNumber(char *serialnumcmp)
{



    FILE *fp2=NULL;
    fp2=fopen("/etc/tmp/SerialNum","w+");
    if(fp2==NULL)
    {
       printf("fopen  SerialNum  error\n");
       return ;
    }

    fwrite(serialnumcmp,strlen(serialnumcmp),1,fp2);
    fclose(fp2);

}


/*

      //将字符串编码�6进制数字,适用于所有字符（包括中文�
      //@param str 字符�
      //@return 返回16进制字符�

void thread1::static String strToHexStr(String str) {
        // 根据默认编码获取字节数组
        byte[] bytes = str.getBytes();
        StringBuilder sb = new StringBuilder(bytes.length * 2);
        // 将字节数组中每个字节拆解��6进制整数
        for (int i = 0; i < bytes.length; i++) {
            sb.append(hexString.charAt((bytes[i] & 0xf0) >> 4));
            sb.append(hexString.charAt((bytes[i] & 0x0f) >> 0));
        }
        return sb.toString();
    }
    */

void thread1::connectdsp()
{
    unsigned char Hignnum=0;
    unsigned char Lownum=0;
    unsigned char SerailNumberCMP[8];
    for(int i=0;i<8;i++)
    {
        if(SerialNumber[i*2]>='0'&&SerialNumber[i*2]<='9')
        {
          Hignnum=SerialNumber[i*2]-0x30;//0x30   16jinzhide '0'
        }
        else if(SerialNumber[i*2]>='a'&&SerialNumber[i*2]<='f')
        {
            Hignnum=SerialNumber[i*2]-87;
        }
        else if(SerialNumber[i*2]>='A'&&SerialNumber[i*2]<='F')
        {
            Hignnum=SerialNumber[i*2]-55;
        }

        if(SerialNumber[i*2+1]>='0'&&SerialNumber[i*2+1]<='9')
        {
          Lownum=SerialNumber[i*2+1]-0x30;
        }
        else if(SerialNumber[i*2+1]>='a'&&SerialNumber[i*2+1]<='f')
        {
            Lownum=SerialNumber[i*2+1]-87;
        }
        else if(SerialNumber[i*2+1]>='A'&&SerialNumber[i*2+1]<='F')
        {
            Lownum=SerialNumber[i*2+1]-55;
        }
     SerailNumberCMP[i]= Hignnum*16+Lownum;
    }
//    temprorarydatainit();
//    UartsInit();


//    emit SIG_showmainface();//显示主界�

//    online(temprorarydata->UartDev[0].uartfd);
//    artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);

//    for(int i=0;i<8;i++)
//    {
//       if(result[7+i]!=SerailNumberCMP[i]) //比较16进制的序列号 如果序列号不�则机器不能使�线程停止
//       {
//        artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
//       return;
//       }
//    }
//    WriteSerialNumber(SerialNumber);
    //解析result中的序列号，DSP�
    QString serial[16];
    for(int i=0;i<8;i++)
    {
        unsigned char hignnum=0;
        unsigned char lownum=0;
        hignnum=((result[7+i]&0xf0)>>4);
         lownum=((result[7+i]&0x0f)>>0);
        if(i<7)
        {
          hignnum=hignnum+0x30;//0x30   16jinzhide '0'
          lownum=lownum+0x30;//0x30   16jinzhide '0'
          serial[i*2]=QString(hignnum);
          serial[i*2+1]=QString(lownum);
        }
        else if(i==7)
        {
            hignnum=hignnum+0x30;//0x30   16jinzhide '0'
            lownum=lownum+55;
            serial[i*2]=QString(hignnum);
            serial[i*2+1]=QString(lownum);
        }

    }
    for(int i=0;i<16;i++)
    serialnumbers+=serial[i];
    //解析DSP�
    QString dsp[6];
    for(int i=0;i<3;i++)
    {
        unsigned char higndsp,lowdsp=0;
        higndsp=((result[15+i]&0xf0)>>4)+0x30;
         lowdsp=((result[15+i]&0x0f)>>0)+0x30;
         dsp[i*2]=QString(higndsp);
         dsp[i*2+1]=QString(lowdsp);
    }
    dspnumbers=dsp[0]+dsp[1]+'.'+dsp[2]+dsp[3]+'.'+dsp[4]+dsp[5];
    //解析校准日期
    QString date[38];
    for(int i=0;i<19;i++)
    {
        unsigned char higndate,lowdate=0;
        higndate=((result[18+i]&0xf0)>>4)+0x30;
         lowdate=((result[18+i]&0x0f)>>0)+0x30;
         date[i*2]=QString(higndate);
         date[i*2+1]=QString(lowdate);
    }
    datenumbers=date[4]+date[5]+date[2]+date[3]+date[0]+date[1]+\
                date[10]+date[11]+date[8]+date[9]+date[6]+date[7];//按照.p文件中的日期顺序来解�

    //发送信号给窗口，显示主界面    
//    artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
//    close(temprorarydata->UartDev[0].uartfd);
//    free(temprorarydata);
//    temprorarydata=NULL;


}

void thread1::setcmd(char *cmd)
{
    if(strlen(cmd)==0)
        return;
    strcpy(cmdstr,cmd);
}







void thread1::Manualoutput()
{
 sem.acquire(1);
 ManualOutputFlag=true;
 sem.release(1);


}
//����
void thread1::onRecevieIndex(int index)
{
  WhichIndex=index;
}

//����ģ�鶼��
int thread1::Buzzer(char Address1,char Address2,char Address3)//共有
  {//sys[1]:杩囪浇锛宻ys[2]:闂存瓏锛宻ys[3]:闂�
    if(sys[1]==1)
      {//杩囪浇淇℃伅
        if((Address1&0x08)!=0x00&&m_Ch[4].Output==1)
          return 1;//I1杩囪�
        if((Address1&0x02)!=0x00&&m_Ch[5].Output==1)
          return 1;//I2杩囪�
        if((Address2&0x80)!=0x00&&m_Ch[6].Output==1)
          return 1;//I3杩囪�
        if((Address2&0x10)!=0x00&&m_Ch[0].Output==1)
          return 1;//V1杩囪�
        if((Address2&0x02)!=0x00&&m_Ch[1].Output==1)
          return 1;//V2杩囪�
        if((Address3&0x40)!=0x00&&m_Ch[2].Output==1)
          return 1;//V3杩囪�
        if((Address3&0x20)!=0x00)
          return 1;//鐩存祦杩囪浇
      }
    else if(sys[1]==0)
      {//杩囪浇淇℃伅
        if((Address1&0x08)!=0x00&&m_Ch[4].Output==1)
          return -1;//I1杩囪�
        if((Address1&0x02)!=0x00&&m_Ch[5].Output==1)
          return -1;//I2杩囪�
        if((Address2&0x80)!=0x00&&m_Ch[6].Output==1)
          return -1;//I3杩囪�
        if((Address2&0x10)!=0x00&&m_Ch[0].Output==1)
          return -1;//V1杩囪�
        if((Address2&0x02)!=0x00&&m_Ch[1].Output==1)
          return -1;//V2杩囪�
        if((Address3&0x40)!=0x00&&m_Ch[2].Output==1)
          return -1;//V3杩囪�
        if((Address3&0x20)!=0x00)
          return 1;//鐩存祦杩囪浇
      }
    if(sys[2]==1)
      {//闂存瓏淇℃伅
        if((Address1&0x10)!=0x00)
          return 1;//I1闂存�
        if((Address1&0x04)!=0x00)
          return 1;//I2闂存�
        if((Address1&0x01)!=0x00)
          return 1;//I3闂存�
        if((Address2&0x08)!=0x00)
          return 1;//V1闂存�
        if((Address2&0x04)!=0x00)
          return 1;//V2闂存�
        if((Address3&0x80)!=0x00)
          return 1;//V3闂存�
      }
    else if(sys[2]==0)
      {//闂存瓏淇℃伅
        if((Address1&0x10)!=0x00)
          return -1;//I1闂存�
        if((Address1&0x04)!=0x00)
          return -1;//I2闂存�
        if((Address1&0x01)!=0x00)
          return -1;//I3闂存�
        if((Address2&0x08)!=0x00)
          return -1;//V1闂存�
        if((Address2&0x04)!=0x00)
          return -1;//V2闂存�
        if((Address3&0x80)!=0x00)
          return -1;//V3闂存�
      }
    if(sys[3]==1)
      {//闂攣淇℃伅
        if((Address1&0x80)!=0x00)
          return 1;//I1闂�
        if((Address1&0x40)!=0x00)
          return 1;//I2闂�
        if((Address1&0x20)!=0x00)
          return 1;//I3闂�
        if((Address2&0x20)!=0x00)
          return 1;//V1闂�
        if((Address2&0x40)!=0x00)
          return 1;//V2闂�
        if((Address2&0x01)!=0x00)
          return 1;//V3闂�
      }
    else if(sys[3]==0)
      {//闂攣淇℃伅
        if((Address1&0x80)!=0x00)
          return -1;//I1闂�
        if((Address1&0x40)!=0x00)
          return -1;//I2闂�
        if((Address1&0x20)!=0x00)
          return -1;//I3闂�
        if((Address2&0x20)!=0x00)
          return -1;//V1闂�
        if((Address2&0x40)!=0x00)
          return -1;//V2闂�
        if((Address2&0x01)!=0x00)
          return -1;//V3闂�
      }
    return 0;
  }

//����ģ�鶼��
void thread1::logic_info(int fd,char info_result1,char info_result2,char info_result3,char info_result4)
  {
     //info_result1涓虹�璺湴鍧€鐨凞B8-DB15锛宨nfo_result2涓虹�璺湴鍧€鐨凞B8-DB15锛宨nfo_result3涓虹�璺湴鍧€鐨凞B0-DB7�

  //  printf("%s   %d\n",__func__,__LINE__);

    static int info_count=1;
    int  k1=0;
    //铚傞福鍣
    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);
    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //涓嶉渶瑕佹帶�
        {//perror("beeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeep");
          artExec(fd,"out:dig:on(0x80)",result,0);
        }
        else
        {
          artExec(fd,"out:dig:off(0x80)",result,0);
        }


      }
    if(binOut&0x01)
        artExec(fd,"out:dig:on(0x01)",result,0);
    else
        artExec(fd,"out:dig:off(0x01)",result,0);
    if(binOut&0x02)
        artExec(fd,"out:dig:on(0x02)",result,0);
    else
        artExec(fd,"out:dig:off(0x02)",result,0);

    if((info_result4&0x0F)==0x00)
    {
        emit SIG_dspstate(100);
    }
    else
    {
        if((info_result4&0x01)==0x01)
        {
            emit SIG_dspstate(101);
        }
        else if((info_result4&0x01)==0x00)
        {
            emit SIG_dspstate(111);
        }
        if((info_result4&0x02)==0x02)
        {
            emit SIG_dspstate(102);
        }
        else if((info_result4&0x02)==0x00)
        {
            emit SIG_dspstate(112);
        }
        if((info_result4&0x04)==0x04)
        {
            emit SIG_dspstate(103);
        }
        else if((info_result4&0x04)==0x00)
        {
            emit SIG_dspstate(113);
        }
        if((info_result4&0x08)==0x08)
        {
            emit SIG_dspstate(104);
        }
        else if((info_result4&0x08)==0x00)
        {
            emit SIG_dspstate(114);
        }
    }
    if(Buzzer(info_result1,info_result2,info_result3)==0) //涓嶉渶瑕佹帶�
      {
        //杩愯姝ｅ父
          emit SIG_dspstate(0);
        return;
      }
    //new add 2017-7-13
    else if(info_count==1)
      {
        if((info_result1&0x01)==0x01)
          {//info_result1鏄�璺湴鍧€鐨凞B8~DB15�
            //perror("00000000000000000000000000");
           emit SIG_dspstate(1);
            k1=1;
          }
        if((info_result1&0x02)==0x02/*&&m_Ch[4].Output==1*/)
          {//perror("11111111111111111111111");
            emit SIG_dspstate(2);
            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//perror("2222222222222222222222222");
            emit SIG_dspstate(3);
            k1=1;
          }
        if((info_result1&0x08)==0x08)
          {//perror("3333333333333333333333333");
            emit SIG_dspstate(4);
            k1=1;
          }
        if((info_result1&0x10)==0x10&&m_Ch[0].Output==1)
          {//perror("44444444444444444444444444444444");
            emit SIG_dspstate(5);//--
            k1=1;
          }
        if(k1==1)
          {
            //info_count++;
            return;
          }
        /*else
          info_count++*/;
      }

    /*
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&m_Ch[4].Output==1)
          {//info_result1鏄�璺湴鍧€鐨凞B8~DB15�
           emit SIG_dspstate(1);
            //IAoverload
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
            emit SIG_dspstate(2);

            //IABC 温度保护
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //ICjianxie
              emit SIG_dspstate(3);
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==2)
      {
        if((info_result1&0x02)==0x02&&m_Ch[5].Output==1)
          {  //info_result1鏄�璺湴鍧€鐨凞B9~DB15�

              emit SIG_dspstate(4);
            //IBoverload
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1鏄�璺湴鍧€鐨凞B8~DB15�

              emit SIG_dspstate(5);
           //IC闂�
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1鏄�璺湴鍧€鐨凞B9~DB15�
              emit SIG_dspstate(6);
          //IA闂存�
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==3)
      {
        if((info_result2&0x80)==0x80&&m_Ch[6].Output==1)
          { //info_result2鏄�璺湴鍧€鐨凞B9~DB15�
           //IC杩囪�
              emit SIG_dspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1鏄�璺湴鍧€鐨凞B8~DB15�
          //IABC 鐢垫簮淇濇姢
              emit SIG_dspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1鏄�璺湴鍧€鐨凞B9~DB15�
              emit SIG_dspstate(9);
          //I2闂存� ?娓╁害淇濇姢闂存�
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==4)
      {
        if((info_result2&0x10)==0x10&&m_Ch[0].Output==1)
          {//info_result2鏄�璺湴鍧€鐨凞B9~DB15�
           //VA杩囪�
              emit SIG_dspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2鏄�璺湴鍧€鐨凞B8~DB15�

           ;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3鏄�璺湴鍧€鐨凞B0~DB7�

              //V3闂存�
              emit SIG_dspstate(12);
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==5)
      {
        if((info_result2&0x02)==0x02&&m_Ch[1].Output==1)
          {//info_result2鏄�璺湴鍧€鐨凞B9~DB15�
            //V2杩囪�
              emit SIG_dspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2鏄�璺湴鍧€鐨凞B8~DB15�//DB8
           // VABC 娓╁害淇濇姢
            emit SIG_dspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2鏄�璺湴鍧€鐨凞B9~DB15�
            emit SIG_dspstate(15);
           //VA闂存�
            k1=1;
          }
        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }

    else if(info_count==6)
      {
        if((info_result3&0x40)==0x40&&m_Ch[2].Output==1)
          { //info_result3鏄�璺湴鍧€鐨凞B0~DB7�

             emit SIG_dspstate(16);
            //VC杩囪�
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2鏄�璺湴鍧€鐨凞B8~DB15�//DB13
            emit SIG_dspstate(17);
            //VABC鐢垫簮淇濇姢
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2鏄�璺湴鍧€鐨凞B8~DB15�//DB13
            emit SIG_dspstate(18);
            //VABC鐢垫簮淇濇姢
            k1=1;
          }

        if(k1==1)
          {
            info_count++;
            return;
          }
        else
          info_count++;
      }
    else if(info_count==7)
      {
        if((info_result3&0x20)==0x20)
          {//info_result3鏄�璺湴鍧€鐨凞B0~DB7�
            emit SIG_dspstate(19);

             //鐩存祦杩囪浇
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2鏄�璺湴鍧€鐨凞B8~DB15�
          emit SIG_dspstate(20);

            // VABC 娓╁害淇濇姢
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3鏄�璺湴鍧€鐨凞B0~DB7�
            emit SIG_dspstate(21);
           //V3闂存�
            k1=1;
          }
        if(k1==1)
          {
            info_count=1;
            return;
          }
        else
          info_count=1;
      }
*/
  }

void thread1::read_logic(int fd) //闇€瑕乪mit
{

//    unsigned char signature;
//    artExec(fd,"inp:buf:get?",result,15);
//    signature=int(result[13]&0x7f);
//    switch(signature)
//      {
//        case 1:
//          break;
//        case 2:
//          break;
//        case 3:
//          break;
//        case 4:
//          emit  SIG_testresult(false);
//          save_f=1;
//          break;
//        case 5:
//          break;
//      }

 switch(WhichIndex)
 {

    case 0://anytest
    {
        unsigned char signature;
        artExec(fd,"inp:buf:get?",result,15);
        signature=int(result[13]&0x7f);
        switch(signature)
          {
            case 1:
              break;
            case 2:
              break;
            case 3:
              break;
            case 4:
              emit  SIG_testresult(false);
              save_f=1;
              break;
            case 5:
              break;
          }

    }
        break;

    case 1://sin1
    {



    }
        break;

    case 2://time1
    {


    }
      break;


    case 3://dizhou1
    {



    }
     break;

  }

}

void thread1::ReadAutologic(int fd) //2016-6-3  zidong
 {

    switch(WhichIndex)
    {

       case 0://anytest
       {
        int t2=0,t3=0,t5=0;
        int stepnum=0,tripstepnum=0;
        unsigned char signature;
        artExec(fd,"inp:buf:get?",result,15);
        signature=int(result[13]&0x7f);
        switch(signature)
          {
            case 1:
              break;
            case 2:
              t2=1;
              break;
            case 3:
              t3=1;
              break;
            case 4:
            emit  SIG_testresult(false);
            save_f=1;
              break;
            case 5:
            t5=1;
              break;
          }

        if(t2==1)
        {
            artExec(fd,"seq:status?(step)",result,16);  //鍔涓瓧�
            stepnum=result[9]*256+result[10];
            tripstepnum=result[11]*256+result[12];
            A_SumStepnum=tripstepnum;

            switch(cAnytest.Variable)
              {
                case 1:
                  if(m_Ch[0].checked==1)
                    {
                      ChangeV1Value=m_Ch[0].Amp+cAnytest.VolStep*stepnum;
                    }
                  if(m_Ch[1].checked==1)
                    {
                        ChangeV2Value=m_Ch[1].Amp+cAnytest.VolStep*stepnum;
                    }
                  if(m_Ch[2].checked==1)
                    {//V3鐟曚礁褰夐崠?
                      ChangeV3Value=m_Ch[2].Amp+cAnytest.VolStep*stepnum;
                    }
    /*
                  if(m_Ch[3].checked==1)
                    {//V3鐟曚礁褰夐崠?
                      ChangeVzValue=m_Ch[3].Amp+cAnytest.VolStep*stepnum;
                    }
    */


                  if(m_Ch[4].checked==1)
                    {//I1鐟曚礁褰夐崠?
                      ChangeI1Value=m_Ch[4].Amp+cAnytest.CurStep*stepnum;
                    }
                  if(m_Ch[5].checked==1)
                    {//I2鐟曚礁褰夐崠?
                      ChangeI2Value=m_Ch[5].Amp+cAnytest.CurStep*stepnum;
                    }
                  if(m_Ch[6].checked==1)
                    {//I3鐟曚礁褰夐崠?
                      ChangeI3Value=m_Ch[6].Amp+cAnytest.CurStep*stepnum;
                    }
                  break;

            case 2:
              if(m_Ch[0].checked==1)
                {
                  ChangeV1Phase=m_Ch[0].Phase+cAnytest.PhaStep*stepnum;
                }
              if(m_Ch[1].checked==1)
                {
                    ChangeV2Phase=m_Ch[1].Phase+cAnytest.PhaStep*stepnum;
                }
              if(m_Ch[2].checked==1)
                {//V3鐟曚礁褰夐崠?
                  ChangeV3Phase=m_Ch[2].Phase+cAnytest.PhaStep*stepnum;
                }
    /*
              if(m_Ch[3].checked==1)
                {//V3鐟曚礁褰夐崠?
                  ChangeVzPhase=m_Ch[3].Phase+cAnytest.PhaStep*stepnum;
                }
    */


              if(m_Ch[4].checked==1)
                {//I1鐟曚礁褰夐崠?
                  ChangeI1Phase=m_Ch[4].Phase+cAnytest.PhaStep*stepnum;
                }
              if(m_Ch[5].checked==1)
                {//I2鐟曚礁褰夐崠?
                  ChangeI2Phase=m_Ch[5].Phase+cAnytest.PhaStep*stepnum;
                }
              if(m_Ch[6].checked==1)
                {//I3鐟曚礁褰夐崠?
                  ChangeI3Phase=m_Ch[6].Phase+cAnytest.PhaStep*stepnum;
                }
              break;

            case 3:
              if(m_Ch[0].checked==1)
                {
                  ChangeV1Frec=m_Ch[0].Freq+cAnytest.FreStep*stepnum;
                }
              if(m_Ch[1].checked==1)
                {
                  ChangeV2Frec=m_Ch[1].Freq+cAnytest.FreStep*stepnum;
                }
              if(m_Ch[2].checked==1)
                {//V3鐟曚礁褰夐崠?
                  ChangeV3Frec=m_Ch[2].Freq+cAnytest.FreStep*stepnum;
                }
    /*
              if(m_Ch[3].checked==1)
                {//V3鐟曚礁褰夐崠?
                  ChangeVzFrec=m_Ch[3].Freq+cAnytest.FreStep*stepnum;
                }
    */


              if(m_Ch[4].checked==1)
                {//I1鐟曚礁褰夐崠?
                  ChangeI1Frec=m_Ch[4].Freq+cAnytest.FreStep*stepnum;
                }
              if(m_Ch[5].checked==1)
                {//I2鐟曚礁褰夐崠?
                  ChangeI2Frec=m_Ch[5].Freq+cAnytest.FreStep*stepnum;
                }
              if(m_Ch[6].checked==1)
                {//I3鐟曚礁褰夐崠?
                  ChangeI3Frec=m_Ch[6].Freq+cAnytest.FreStep*stepnum;
                }
              break;
              }

            emit SIG_DisplayAutoChangeValue(1);
        }
        if(t3==1)
          {   //鏈姩浣
        TripTime=-1;
        TripValue=-1;
          }
        if(t5=1)
        {
    //        output_signal_end(fd);
    //        starttest=0;
    //        ClearLED();
    //        emit SIG_DisplayAutoChangeValue(0);
    //        save_f=1;
        }


       }
           break;

       case 1://sin1
       {



       }
           break;

       case 2://time1
       {


       }
         break;


       case 3://dizhou1
       {



       }
        break;





     }

}
//������Ե���
void  thread1::ReadA_TripTimeLogic(int fd)//璇绘祴璇曞姩浣滄椂闂寸殑鎺ョ偣鍙樺寲淇℃�
{

     switch(WhichIndex)
     {

        case 0://anytest
        {

         int t3=0,t4=0,t5=0,t2=0;
         unsigned char signature;
         double u,v,w;
         static double SequencerTime2;
         static double SequencerTime4;
         float Temp;

         artExec(fd,"inp:buf:get?",result,15);//浠嶥SP寰楀埌寮€鍏抽噺鎺ョ偣淇℃伅�

         u=result[7];
         v=result[8];
         w=result[9];
         signature=int(result[13]&0x7f);
         switch(signature)
           {
             case 1:
               break;
             case 2:
               t2=1;
               SequencerTime2=gettime(u,v,w);   //璁＄畻浠庡紑濮嬫祴璇曞埌sequencer鐨凷tep2鐨勬椂闂淬€
               break;
             case 3:
               t3=1;
               break;
             case 4:
               t4=1;
               SequencerTime4=gettime(u,v,w);  //璁＄畻浠庡紑濮嬫祴璇曞埌sequencer鐨凷tep4鐨勬椂闂淬€
               break;
             case 5:
               t5=1;
               break;
           }


         if(t3==1)
           {   //鏈姩浣
         TripTime=-1;
         TripValue=-1;
           }
         if(t4==1)
           {//鍔ㄤ�
             if(cAnytest.TestMode==3)
            { TripTime=SequencerTime4-SequencerTime2-faultduration-cAnytest.logic_delaytime/1000;
              //A_SumStepnum=floor((SequencerTime4-SequencerTime2-faultduration)/cAnytest.StepTime);
              //A_SumStepnum=(int)A_SumStepnum;
              TripTime=TripTime-floor(TripTime/cAnytest.StepTime)*cAnytest.StepTime;
            }
             else if(cAnytest.TestMode==2)
             {
              TripTime=SequencerTime4-SequencerTime2-cAnytest.logic_delaytime/1000;
             }

             Temp=TripTime*1000;
             Temp=ceil(Temp);
             TripTime=Temp/1000;//灏嗚繑鍥炲€肩殑闄愬埗鍦ㄥ皬鏁扮偣鍚庢渶澶氬彧鏈変笁�
            }
          if(t5==1)
            {
             output_signal_end(fd);
             GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //�
             //ClearLED();
             starttest=0;
             save_f=1;
             emit SIG_Teststate(0);
             if(cAnytest.TestMode==2)
             {

             emit SIG_testresult(false);
             }
             else if(cAnytest.TestMode==3)
             {
             emit SIG_DisplayAutoChangeValue(0);
             emit SIG_testresult(false);
             }
           }

        }
            break;

        case 1://sin1
        {



        }
            break;

        case 2://time1
        {


        }
          break;


        case 3://dizhou1
        {



        }
         break;
     }

}
//������Ե���
int  thread1::checkAutoTestEndvalueValid()
{

    float max_v_valuedata,max_i_valuedata;


    if(m_Ch[0].DC==0||m_Ch[1].DC==0||m_Ch[2].DC==0||m_Ch[3].DC==0||m_Ch[4].DC==0||m_Ch[5].DC==0||m_Ch[6].DC==0)
      {
        max_v_valuedata=MAX_V_VALUEDATA_AC;
        max_i_valuedata=MAX_I_VALUEDATA_AC;

      }
    else if(m_Ch[0].DC==1||m_Ch[1].DC==1||m_Ch[2].DC==1||m_Ch[3].DC==1||m_Ch[4].DC==1||m_Ch[5].DC==1||m_Ch[6].DC==1)
      {
        max_v_valuedata=MAX_V_VALUEDATA_DC;
        max_i_valuedata=MAX_I_VALUEDATA_DC;

      }

 switch(cAnytest.Variable)
 {
 case 1:
        if(m_Ch[0].checked==1)
       {
           if(cAnytest.VolStep>=0)
           {
               switch(m_Ch[0].DC)
               {
                 case 0:  //VA  浜ゆ�
                   if(m_Ch[0].Amp+cAnytest.StepNum*cAnytest.VolStep>MAX_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(1,1);
                                   return -1;
                   }
                   break;
                 case 1:  //VA  鐩存�
                   if(m_Ch[0].Amp+cAnytest.StepNum*cAnytest.VolStep>MAX_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(1,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(cAnytest.VolStep<0)
           {
               switch(m_Ch[0].DC)
               {
                 case 0:  //VA  浜ゆ�
                   if(m_Ch[0].Amp+cAnytest.StepNum*cAnytest.VolStep<MIN_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(1,2);
                                      return -1;
                   }
                   break;
                 case 1:  //VA  鐩存�
                   if(m_Ch[0].Amp+cAnytest.StepNum*cAnytest.VolStep<MIN_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(1,2);
                                      return -1;
                   }
                   break;
               }
           }
       }


        if(m_Ch[1].checked==1)
       {
           if(cAnytest.VolStep>=0)
           {
               switch(m_Ch[1].DC)
               {
                 case 0:  //VB  浜ゆ�
                   if(m_Ch[1].Amp+cAnytest.StepNum*cAnytest.VolStep>MAX_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(2,1);
                                   return -1;
                   }
                   break;
                 case 1:  //VB  鐩存�
                   if(m_Ch[1].Amp+cAnytest.StepNum*cAnytest.VolStep>MAX_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(2,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(cAnytest.VolStep<0)
           {
               switch(m_Ch[1].DC)
               {
                 case 0:  //VB  浜ゆ�
                   if(m_Ch[1].Amp+cAnytest.StepNum*cAnytest.VolStep<MIN_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(2,2);
                                      return -1;
                   }
                   break;
                 case 1:  //VB  鐩存�
                   if(m_Ch[1].Amp+cAnytest.StepNum*cAnytest.VolStep<MIN_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(2,2);
                                      return -1;
                   }
                   break;
               }
           }
       }
        if(m_Ch[2].checked==1)
       {
           if(cAnytest.VolStep>=0)
           {
               switch(m_Ch[2].DC)
               {
                 case 0:  //VC  浜ゆ�
                   if(m_Ch[2].Amp+cAnytest.StepNum*cAnytest.VolStep>MAX_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(3,1);
                                   return -1;
                   }
                   break;
                 case 1:  //VC  鐩存�
                   if(m_Ch[2].Amp+cAnytest.StepNum*cAnytest.VolStep>MAX_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(3,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(cAnytest.VolStep<0)
           {
               switch(m_Ch[2].DC)
               {
                 case 0:  //VC  浜ゆ�
                   if(m_Ch[2].Amp+cAnytest.StepNum*cAnytest.VolStep<MIN_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(3,2);
                                      return -1;
                   }
                   break;
                 case 1:  //VC  鐩存�
                   if(m_Ch[2].Amp+cAnytest.StepNum*cAnytest.VolStep<MIN_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(3,2);
                                      return -1;
                   }
                   break;
               }
           }
       }
        if(m_Ch[3].checked==1)
       {
           if(cAnytest.VolStep>=0)
           {
               switch(m_Ch[3].DC)
               {
                 case 0:  //Vz  浜ゆ�
                   if(m_Ch[3].Amp+cAnytest.StepNum*cAnytest.VolStep>MAX_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(4,1);
                                   return -1;
                   }
                   break;
                 case 1:  //Vz  鐩存�
                   if(m_Ch[3].Amp+cAnytest.StepNum*cAnytest.VolStep>MAX_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(4,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(cAnytest.VolStep<0)
           {
               switch(m_Ch[3].DC)
               {
                 case 0:  //Vz  浜ゆ�
                   if(m_Ch[3].Amp+cAnytest.StepNum*cAnytest.VolStep<MIN_V_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(4,2);
                                      return -1;
                   }
                   break;
                 case 1:  //Vz  鐩存�
                   if(m_Ch[3].Amp+cAnytest.StepNum*cAnytest.VolStep<MIN_V_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(4,2);
                                      return -1;
                   }
                   break;
               }
           }
       }

        if(m_Ch[4].checked==1)
       {
           if(cAnytest.CurStep>=0)
           {
               switch(m_Ch[4].DC)
               {
                 case 0:  //IA  浜ゆ�
                   if(m_Ch[4].Amp+cAnytest.StepNum*cAnytest.CurStep>MAX_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(5,1);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  鐩存�
                   if(m_Ch[4].Amp+cAnytest.StepNum*cAnytest.CurStep>MAX_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(5,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(cAnytest.CurStep<0)
           {
               switch(m_Ch[4].DC)
               {
                 case 0:  //IA  浜ゆ�
                   if(m_Ch[4].Amp+cAnytest.StepNum*cAnytest.CurStep<MIN_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(5,2);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  鐩存�
                   if(m_Ch[4].Amp+cAnytest.StepNum*cAnytest.CurStep<MIN_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(5,2);
                                   return -1;
                   }
                   break;
               }
           }
       }

        if(m_Ch[5].checked==1)
       {
           if(cAnytest.CurStep>=0)
           {
               switch(m_Ch[5].DC)
               {
                 case 0:  //IA  浜ゆ�
                   if(m_Ch[5].Amp+cAnytest.StepNum*cAnytest.CurStep>MAX_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(6,1);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  鐩存�
                   if(m_Ch[5].Amp+cAnytest.StepNum*cAnytest.CurStep>MAX_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(6,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(cAnytest.CurStep<0)
           {
               switch(m_Ch[5].DC)
               {
                 case 0:  //IA  浜ゆ�
                   if(m_Ch[5].Amp+cAnytest.StepNum*cAnytest.CurStep<MIN_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(6,2);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  鐩存�
                   if(m_Ch[5].Amp+cAnytest.StepNum*cAnytest.CurStep<MIN_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(6,2);
                                   return -1;
                   }
                   break;
               }
           }
       }
        if(m_Ch[6].checked==1)
       {
           if(cAnytest.CurStep>=0)
           {
               switch(m_Ch[6].DC)
               {
                 case 0:  //IA  浜ゆ�
                   if(m_Ch[6].Amp+cAnytest.StepNum*cAnytest.CurStep>MAX_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(7,1);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  鐩存�
                   if(m_Ch[6].Amp+cAnytest.StepNum*cAnytest.CurStep>MAX_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(7,1);
                                   return -1;
                   }
                   break;
               }
           }
           else  if(cAnytest.CurStep<0)
           {
               switch(m_Ch[6].DC)
               {
                 case 0:  //IA  浜ゆ�
                   if(m_Ch[6].Amp+cAnytest.StepNum*cAnytest.CurStep<MIN_I_VALUEDATA_AC)
                   {
                       emit SIG_SendWarning(7,2);
                                   return -1;
                   }
                   break;
                 case 1:  //IA  鐩存�
                   if(m_Ch[6].Amp+cAnytest.StepNum*cAnytest.CurStep<MIN_I_VALUEDATA_DC)
                   {
                       emit SIG_SendWarning(7,2);
                                   return -1;
                   }
                   break;
               }
           }
       }
     break;


 case 2:

     break;

 case 3:
            if(m_Ch[0].checked==1)
            {
                if(cAnytest.FreStep>=0)
                {
                if((m_Ch[0].Freq+cAnytest.StepNum*cAnytest.FreStep)>1000)
              {  emit SIG_SendWarning(1,1);
                return -1;
              }
                }
                else  if(cAnytest.FreStep<0)
                {
                    if((m_Ch[0].Freq+cAnytest.StepNum*cAnytest.FreStep)<0)
                   { emit SIG_SendWarning(1,2);
                    return -1;
                   }
                }
            }


             if(m_Ch[1].checked==1)
            {
                if(cAnytest.FreStep>=0)
                {
                if((m_Ch[1].Freq+cAnytest.StepNum*cAnytest.FreStep)>1000)
               { emit SIG_SendWarning(2,1);
                return -1;
               }
                }
                else  if(cAnytest.FreStep<0)
                {
                    if((m_Ch[1].Freq+cAnytest.StepNum*cAnytest.FreStep)<0)
                   { emit SIG_SendWarning(2,2);
                     return -1;
                   }
                }
            }
             if(m_Ch[2].checked==1)
            {
                if(cAnytest.FreStep>=0)
                {
                if((m_Ch[2].Freq+cAnytest.StepNum*cAnytest.FreStep)>1000)
               { emit SIG_SendWarning(3,1);
                 return -1;
               }
                }
                else  if(cAnytest.FreStep<0)
                {
                    if((m_Ch[2].Freq+cAnytest.StepNum*cAnytest.FreStep)<0)
                  {  emit SIG_SendWarning(3,2);
                    return -1;
                  }
                }
            }
             if(m_Ch[3].checked==1)
            {
                if(cAnytest.FreStep>=0)
                {
                if((m_Ch[3].Freq+cAnytest.StepNum*cAnytest.FreStep)>1000)
              {  emit SIG_SendWarning(4,1);
                return -1;
              }
                }
                else  if(cAnytest.FreStep<0)
                {
                    if((m_Ch[3].Freq+cAnytest.StepNum*cAnytest.FreStep)<0)
                   { emit SIG_SendWarning(4,2);
                    return -1;
                   }
                }
            }

             if(m_Ch[4].checked==1)
            {
                if(cAnytest.FreStep>=0)
                {
                if((m_Ch[4].Freq+cAnytest.StepNum*cAnytest.FreStep)>1000)
              {  emit SIG_SendWarning(5,1);
                return -1;
              }
                }
                else  if(cAnytest.FreStep<0)
                {
                    if((m_Ch[4].Freq+cAnytest.StepNum*cAnytest.FreStep)<0)
                  {  emit SIG_SendWarning(5,2);
                    return -1;
                  }
                }
            }

             if(m_Ch[5].checked==1)
            {
                if(cAnytest.FreStep>=0)
                {
                if((m_Ch[5].Freq+cAnytest.StepNum*cAnytest.FreStep)>1000)
               { emit SIG_SendWarning(6,1);
                return -1;
               }
                }
                else  if(cAnytest.FreStep<0)
                {
                    if((m_Ch[5].Freq+cAnytest.StepNum*cAnytest.FreStep)<0)
                   { emit SIG_SendWarning(6,2);
                    return -1;
                   }
                }
            }
             if(m_Ch[6].checked==1)
            {
                if(cAnytest.FreStep>=0)
                {
                if((m_Ch[6].Freq+cAnytest.StepNum*cAnytest.FreStep)>1000)
              {  emit SIG_SendWarning(7,1);
                return -1;
              }
                }
                else  if(cAnytest.FreStep<0)
                {
                    if((m_Ch[6].Freq+cAnytest.StepNum*cAnytest.FreStep)<0)
                   { emit SIG_SendWarning(7,2);
                    return -1;
                   }
                }
            }
     break;
 }

 return 0;



}




void thread1::key_deal_test_manual(int fd)
{

    if(starttest==0)
    {
        save_f=0;
       emit SIG_Teststate(1);
       if(cAnytest.TestMode==1)
           {
               if(cAnytest.Variable==2)
               {
                 m_Ch[0].checked=0;
                 emit SIG_setVanocheck();
               }

              GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);//�
              //SetLED();
              emit SIG_testresult(true);

              ChangeOutputValue();
              artExec(fd,"out:dig:on(0x10)",result,0);
              artExec(fd,"out:dig:on(0x08)",result,0);
              output_signal_manual(0,fd);
              StartTestFlag=1;
              starttest=1;
              T4=0;

           }
        else if(cAnytest.TestMode==2)
          {

             GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);//�
              //SetLED();
              emit SIG_testresult(true);

              ChangeOutputValue();
              artExec(fd,"out:dig:on(0x10)",result,0);
              artExec(fd,"out:dig:on(0x08)",result,0);
              output_signal_manual(2,fd);
              starttest=1;
              T4=0;

          }

        else if(cAnytest.TestMode==3)
          {
              if(cAnytest.Variable==2)
              {
                m_Ch[0].checked=0;
                emit  SIG_setVanocheck();
              }


              if(checkAutoTestEndvalueValid()==-1)
              {

                  temprorarydata->Warningopenflag=true;
                  return;

              }


              GPIO_OutSet(temprorarydata->gpio_fd,LEDNUM);//�
              //SetLED();
              emit SIG_testresult(true);

              ChangeOutputValue();
              ////妫€鏌ュ弬鏁版槸鍚﹁秴鍑轰簡杈圭�
              artExec(fd,"out:dig:on(0x30)",result,0);
              artExec(fd,"out:dig:on(0x08)",result,0);
              Output_signal_Auto(fd);
              starttest=1;
              T4=0;

          }




    }
  }


void thread1::key_deal_stop_manual(int fd)
  {
    if(starttest==1)
      {
        output_signal_end(fd);
        starttest=0;
        GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //�
        //ClearLED();
        emit SIG_Teststate(0);
        emit SIG_DisplayAutoChangeValue(0);
      }
  }



void thread1::key_response(KEY key)
{


switch(WhichIndex)
{

   case 0://anytest
   {

    if(key==_NOP) return;

    if(key==_TEST)
    {


         if(starttest==0)
         {

            output_signal_end(temprorarydata->UartDev[0].uartfd);
            key_deal_test_manual(temprorarydata->UartDev[0].uartfd);

            emit SIG_TestButtenState();
         }

    }

    else if(key==_STOP)
    {
         GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);
         //ClearLED();
         key_deal_stop_manual(temprorarydata->UartDev[0].uartfd);
          emit SIG_TestButtenState();
         ManualOutputFlag=false;
    }


   }
       break;

   case 1://sin1
   {



   }
       break;

   case 2://time1
   {


   }
     break;


   case 3://dizhou1
   {



   }
    break;

 }


}



void thread1::run()
{
//    if(strlen(cmdstr)==0)
//        return;
  Threadrunstate=true;
//  QProcess  *proc=new QProcess;
//  proc->start(cmdstr);
//  proc->waitForFinished(-1);
//  msleep(1500);
//  Threadrunstate=false;




/*
   unsigned char info_flag1,info_flag2,info_flag3,info_flag4;
   KEY key;
   int info_num=0;
     //鎵撳紑骞堕厤缃覆鍙
   perror("12");
   temprorarydatainit();
   perror("13");
   UartsInit();
   perror("14");
   temprorarydata->gpio_fd=open("/dev/em335x_gpio", O_RDWR);
   int rc = GPIO_OutEnable(temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
   if(rc < 0)
   {
       printf("GPIO_OutEnable::failed %d\n", rc);
       temprorarydata->gpio_fd=-1;
   }
perror("15");
printf("fd============================%d\n",temprorarydata->UartDev[0].uartfd);
online(temprorarydata->UartDev[0].uartfd);
perror("16");
    read_max(temprorarydata->UartDev[0].uartfd);
perror("17");
    artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);

    if((result[7]&0xf0)==0x60)
         { //300V鐢靛帇鏉
           MAX_V_VALUEDATA_AC=300;
           MIN_V_VALUEDATA_AC=0;
           MAX_V_VALUEDATA_DC=424;
           MIN_V_VALUEDATA_DC=-424;
         }
        else if((result[7]&0xf0)==0x80)//
          {
            MAX_V_VALUEDATA_AC=125;
            MIN_V_VALUEDATA_AC=0;
            MAX_V_VALUEDATA_DC=176;
            MIN_V_VALUEDATA_DC=-176;
          }
        else if((result[7]&0xf0)==0x00)
          {
            MAX_V_VALUEDATA_AC=75;
            MIN_V_VALUEDATA_AC=0;
            MAX_V_VALUEDATA_DC=110;

            MIN_V_VALUEDATA_DC=-110;
          }
        if((result[7]&0x0f)==0x01)//
          {
            MAX_I_VALUEDATA_AC=30;
            MIN_I_VALUEDATA_AC=0;
            MAX_I_VALUEDATA_DC=5;
            MIN_I_VALUEDATA_DC=-5;
            //MAX_I_VALUEDATA_AC=30;
            //MIN_I_VALUEDATA_AC=0;
            //MAX_I_VALUEDATA_DC=42;
            //MIN_I_VALUEDATA_DC=-42;
          }
        else if((result[7]&0x0f)==0x08)
          {
            MAX_I_VALUEDATA_AC=40;
            MIN_I_VALUEDATA_AC=0;
            MAX_I_VALUEDATA_DC=20;
            MIN_I_VALUEDATA_DC=-20;
//            MAX_I_VALUEDATA_AC=40;
//            MIN_I_VALUEDATA_AC=0;
//            MAX_I_VALUEDATA_DC=56;
//            MIN_I_VALUEDATA_DC=-56;
          }
        else if((result[7]&0x0f)==0x05)
          {
            MAX_I_VALUEDATA_AC=50;
            MIN_I_VALUEDATA_AC=0;
            MAX_I_VALUEDATA_DC=70;
            MIN_I_VALUEDATA_DC=-70;
          }
while(Threadrunstate)
     {
     if(temprorarydata->keypadopenflag==false&&temprorarydata->helpopenflag==false&&\
             temprorarydata->Warningopenflag==false)
     {
     key=GetKey(temprorarydata->UartDev[1].uartfd);  //杩欓噷闇€瑕佷慨鏀
     key_response(key);

        if(SoftStartEndflag==true)
          {
             if(starttest==1)
             {
                 key=_STOP;
             }
             else
             {
                 key=_TEST;
             }
             key_response(key);
             SoftStartEndflag=false;
          }



     if(starttest==0)//mei测试
     {
       if(info_num++>=10)
        {
          emit SIG_TestButtenState();
         artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //涓嶉渶瑕佹洿鏀
       info_flag1=result[8];
       info_flag4=result[9];
       artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
       info_flag2=result[8];
       info_flag3=result[9];
       logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //闇€瑕乪mit
       info_num=0;
        }
     }

     else//测试
     {
         if(cAnytest.TestMode==1)
         {    read_logic(temprorarydata->UartDev[0].uartfd);  //鎵嬪姩娴嬭瘯娌℃湁娴嬭瘯缁撴�
              sem.acquire(1);
             if(ManualOutputFlag==true)
             {

              output_signal_manual(1,temprorarydata->UartDev[0].uartfd);//浠庢柊寰€DSP閫佷竴涓猻eq锛屾敼鍙樿緭鍑虹殑鍊
              ManualOutputFlag=false;
              ManulIsSendcmd=false;
             }
             sem.release(1);
         }

         else if(cAnytest.TestMode==2)
            ReadA_TripTimeLogic(temprorarydata->UartDev[0].uartfd);
         else if(cAnytest.TestMode==3)
         {
             ReadAutologic(temprorarydata->UartDev[0].uartfd);
            ReadA_TripTimeLogic(temprorarydata->UartDev[0].uartfd);
         }

           if(info_num++>=20)
           {
             artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);
             info_flag1=result[8];
             info_flag4=result[9];
             artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
             info_flag2=result[8];
             info_flag3=result[9];
             logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);
             info_num=0;
             if(cAnytest.TestMode==1&&StartTestFlag==1)
             {
                 artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x08)",result,0);
                 StartTestFlag=0;
             }
           }
      }

     }
     else
     {

         tcflush(temprorarydata->UartDev[1].uartfd, TCIOFLUSH);
         msleep(500);

     }
}

            starttest=0;
            output_signal_end(temprorarydata->UartDev[0].uartfd);
            artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
            close(temprorarydata->UartDev[1].uartfd);
            close(temprorarydata->UartDev[0].uartfd);
*/
  unsigned char info_flag1,info_flag2,info_flag3,info_flag4;
  KEY key;
  int info_num=0;
    //鎵撳紑骞堕厤缃覆鍙

  temprorarydatainit();
  UartsInit();

  temprorarydata->gpio_fd=open("/dev/em335x_gpio", O_RDWR);
  int rc = GPIO_OutEnable(temprorarydata->gpio_fd, 0xffffffff);	//set all GPIO as output
  if(rc < 0)
  {
      printf("GPIO_OutEnable::failed %d\n", rc);
      temprorarydata->gpio_fd=-1;
  }


online(temprorarydata->UartDev[0].uartfd);

   read_max(temprorarydata->UartDev[0].uartfd);

   artExec(temprorarydata->UartDev[0].uartfd,"sys:cfg?",result,31);

   if((result[7]&0xf0)==0x60)
        { //300V鐢靛帇鏉
          MAX_V_VALUEDATA_AC=300;
          MIN_V_VALUEDATA_AC=0;
          MAX_V_VALUEDATA_DC=424;
          MIN_V_VALUEDATA_DC=-424;
        }
       else if((result[7]&0xf0)==0x80)//
         {
           MAX_V_VALUEDATA_AC=125;
           MIN_V_VALUEDATA_AC=0;
           MAX_V_VALUEDATA_DC=176;
           MIN_V_VALUEDATA_DC=-176;
         }
       else if((result[7]&0xf0)==0x00)
         {
           MAX_V_VALUEDATA_AC=75;
           MIN_V_VALUEDATA_AC=0;
           MAX_V_VALUEDATA_DC=110;

           MIN_V_VALUEDATA_DC=-110;
         }
       if((result[7]&0x0f)==0x01)//
         {
           MAX_I_VALUEDATA_AC=30;
           MIN_I_VALUEDATA_AC=0;
           MAX_I_VALUEDATA_DC=5;
           MIN_I_VALUEDATA_DC=-5;
           //MAX_I_VALUEDATA_AC=30;
           //MIN_I_VALUEDATA_AC=0;
           //MAX_I_VALUEDATA_DC=42;
           //MIN_I_VALUEDATA_DC=-42;
         }
       else if((result[7]&0x0f)==0x08)
         {
           MAX_I_VALUEDATA_AC=40;
           MIN_I_VALUEDATA_AC=0;
           MAX_I_VALUEDATA_DC=20;
           MIN_I_VALUEDATA_DC=-20;
//            MAX_I_VALUEDATA_AC=40;
//            MIN_I_VALUEDATA_AC=0;
//            MAX_I_VALUEDATA_DC=56;
//            MIN_I_VALUEDATA_DC=-56;
         }
       else if((result[7]&0x0f)==0x05)
         {
           MAX_I_VALUEDATA_AC=50;
           MIN_I_VALUEDATA_AC=0;
           MAX_I_VALUEDATA_DC=70;
           MIN_I_VALUEDATA_DC=-70;
         }
       connectdsp();
       emit SIG_DspInfoState();

while(Threadrunstate)
    {
    if(temprorarydata->keypadopenflag==false&&temprorarydata->helpopenflag==false&&\
            temprorarydata->Warningopenflag==false)
    {
    key=GetKey(temprorarydata->UartDev[1].uartfd);  //杩欓噷闇€瑕佷慨鏀
    key_response(key);

       if(SoftStartEndflag==true)
         {
            if(starttest==1)
            {
                key=_STOP;
            }
            else
            {
                key=_TEST;
            }
            key_response(key);
            SoftStartEndflag=false;
         }

//********************************************
    switch(WhichIndex)
    {

       case 0://anytest
       {


        if(starttest==0)
        {
          if(info_num++>=10)
        {
             emit SIG_TestButtenState();
            artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);     //涓嶉渶瑕佹洿鏀
          info_flag1=result[8];
          info_flag4=result[9];
          artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
          info_flag2=result[8];
          info_flag3=result[9];
          logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);  //闇€瑕乪mit
          info_num=0;
        }
        }

        else
        {
            if(cAnytest.TestMode==1)
            {    read_logic(temprorarydata->UartDev[0].uartfd);  //鎵嬪姩娴嬭瘯娌℃湁娴嬭瘯缁撴�
                 sem.acquire(1);
                if(ManualOutputFlag==true)
                {

                 output_signal_manual(1,temprorarydata->UartDev[0].uartfd);//浠庢柊寰€DSP閫佷竴涓猻eq锛屾敼鍙樿緭鍑虹殑鍊
                 ManualOutputFlag=false;
                 ManulIsSendcmd=false;
                }
                sem.release(1);
            }

            else if(cAnytest.TestMode==2)
               ReadA_TripTimeLogic(temprorarydata->UartDev[0].uartfd);
            else if(cAnytest.TestMode==3)
            {
                ReadAutologic(temprorarydata->UartDev[0].uartfd);
               ReadA_TripTimeLogic(temprorarydata->UartDev[0].uartfd);
            }

              if(info_num++>=20)
              {
                artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(0):get?",result,11);
                info_flag1=result[8];
                info_flag4=result[9];
                artExec(temprorarydata->UartDev[0].uartfd,"inp:dig(1):get?",result,11);
                info_flag2=result[8];
                info_flag3=result[9];
                logic_info(temprorarydata->UartDev[0].uartfd,info_flag1,info_flag2,info_flag3,info_flag4);
                info_num=0;
                if(cAnytest.TestMode==1&&StartTestFlag==1)
                {
                    artExec(temprorarydata->UartDev[0].uartfd,"out:dig:off(0x08)",result,0);
                    StartTestFlag=0;
                }
              }
        }


       }
           break;

       case 1://sin1
       {



       }
           break;

       case 2://time1
       {


       }
         break;


       case 3://dizhou1
       {



       }
        break;

      }

//********************************************

    }
    else
    {

        tcflush(temprorarydata->UartDev[1].uartfd, TCIOFLUSH);
        msleep(500);

    }
}
           starttest=0;
           output_signal_end(temprorarydata->UartDev[0].uartfd);

           artExec(temprorarydata->UartDev[0].uartfd,"sys:reset",result,0);
           close(temprorarydata->UartDev[1].uartfd);
           close(temprorarydata->UartDev[0].uartfd);
}


void thread1::stoprun(){

  Threadrunstate=false;
}
