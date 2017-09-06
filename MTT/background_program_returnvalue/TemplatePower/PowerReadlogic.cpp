#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <QDebug>
#include "Testcom2.h"
#include "Defpam.h"
#include "Gpio.h"
#include "Config.h"
#include "em335x_drivers.h"
#include "Powerseq.h"
#include <QDebug>

extern int   starttest;

extern int   V1DC,V2DC,V3DC,I1DC,I2DC,I3DC,SequencerCount;
extern float LogicResolution,SystemFrec,Angle1,Angle2;
extern int exit_par,save_f;
extern int sys[4];
extern char   V1Output,V2Output,V3Output,I1Output,I2Output,I3Output;
extern float V_coarse_step,V_fine_step,I_coarse_step,I_fine_step;
extern unsigned char result[70],receive_data[70];
extern double out_time,change_timedata;
extern TEMPRORARYDATA *temprorarydata;
extern float mta;



extern int faulttype;

extern float FixCurrent,FixVoltage,MSAngle,StartPhase,EndPhase,OutPhase,StepPhase,StepTime;



extern int StartPhaseActState;  //0δ����,1����
extern int SearchType;  //��������  0 ��ʱ��  1 ˳ʱ��

extern QVector<ACTSTATE> AngleActStateVect; //

      extern   float Triptime;


void outputchar()
{

    char outchar[512];
    switch(faulttype)
    {
    case 1:
        sprintf(outchar,"Fault Type:A-E, IA Phase:%.3f",OutPhase);
        break;

    case 2:
        sprintf(outchar,"Fault Type:B-E, IB Phase:%.3f",OutPhase);
        break;

    case 3:
        sprintf(outchar,"Fault Type:C-E, IC Phase:%.3f",OutPhase);
        break;

    case 4:
        sprintf(outchar,"Fault Type:A-B-C, IABC Phase:%.3f",OutPhase);
        break;
    }

    fprintf(stdout,outchar);
    fflush(stdout);

}



int  Buzzer(char Address1,char Address2,char Address3)
  {//sys[1]:���أ�sys[2]:��Ъ��sys[3]:����
    if(sys[1]==1)
      {//������Ϣ
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1����
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2����
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3����
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1����
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2����
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3����
        if((Address3&0x20)!=0x00)
          return 1;//ֱ������
      }
    else if(sys[1]==0)
      {//������Ϣ
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1����
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2����
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3����
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1����
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2����
        if((Address3&0x40)!=0x00&&V3Output==1)
          return -1;//V3����
        if((Address3&0x20)!=0x00)
          return 1;//ֱ������
      }
    if(sys[2]==1)
      {//��Ъ��Ϣ
        if((Address1&0x10)!=0x00)
          return 1;//I1��Ъ
        if((Address1&0x04)!=0x00)
          return 1;//I2��Ъ
        if((Address1&0x01)!=0x00)
          return 1;//I3��Ъ
        if((Address2&0x08)!=0x00)
          return 1;//V1��Ъ
        if((Address2&0x04)!=0x00)
          return 1;//V2��Ъ
        if((Address3&0x80)!=0x00)
          return 1;//V3��Ъ
      }
    else if(sys[2]==0)
      {//��Ъ��Ϣ
        if((Address1&0x10)!=0x00)
          return -1;//I1��Ъ
        if((Address1&0x04)!=0x00)
          return -1;//I2��Ъ
        if((Address1&0x01)!=0x00)
          return -1;//I3��Ъ
        if((Address2&0x08)!=0x00)
          return -1;//V1��Ъ
        if((Address2&0x04)!=0x00)
          return -1;//V2��Ъ
        if((Address3&0x80)!=0x00)
          return -1;//V3��Ъ
      }
    if(sys[3]==1)
      {//������Ϣ
        if((Address1&0x80)!=0x00)
          return 1;//I1����
        if((Address1&0x40)!=0x00)
          return 1;//I2����
        if((Address1&0x20)!=0x00)
          return 1;//I3����
        if((Address2&0x20)!=0x00)
          return 1;//V1����
        if((Address2&0x40)!=0x00)
          return 1;//V2����
        if((Address2&0x01)!=0x00)
          return 1;//V3����
      }
    else if(sys[3]==0)
      {//������Ϣ
        if((Address1&0x80)!=0x00)
          return -1;//I1����
        if((Address1&0x40)!=0x00)
          return -1;//I2����
        if((Address1&0x20)!=0x00)
          return -1;//I3����
        if((Address2&0x20)!=0x00)
          return -1;//V1����
        if((Address2&0x40)!=0x00)
          return -1;//V2����
        if((Address2&0x01)!=0x00)
          return -1;//V3����
      }
    return 0;
  }


void showdspstate(int state)
{

    char outchar[512];
    switch(state)
    {
    case 0: strcpy(outchar,"WARNING=Run normally"); break;
    case 1: strcpy(outchar,"WARNING=IA overload"); break;
    case 2: strcpy(outchar,"WARNING=IABC temperature protect"); break;
    case 3: strcpy(outchar,"WARNING=IC resume"); break;
    case 4: strcpy(outchar,"WARNING=IB overload"); break;
    case 5: strcpy(outchar,"WARNING=IC closedown"); break;
    case 6: strcpy(outchar,"WARNING=IA resume"); break;
    case 7: strcpy(outchar,"WARNING=IC overload"); break;
    case 8: strcpy(outchar,"WARNING=IABC power protection"); break;
    case 9: strcpy(outchar,"WARNING=IB resume"); break;
    case 10: strcpy(outchar,"WARNING=VA overload"); break;

    case 12: strcpy(outchar,"WARNING=VC resume "); break;
    case 13: strcpy(outchar,"WARNING=VB overload "); break;
    case 14: strcpy(outchar,"WARNING=VABC temperature protection"); break;
    case 15: strcpy(outchar,"WARNING=VA resume"); break;
    case 16: strcpy(outchar,"WARNING=VC overload"); break;
    case 17: strcpy(outchar,"WARNING=VABC power protection"); break;
    case 18: strcpy(outchar,"WARNING=Vz overload"); break;
    case 19: strcpy(outchar,"WARNING=direct current overload"); break;
    case 20: strcpy(outchar,"WARNING=IABC power protection"); break;
    case 21: strcpy(outchar,"WARNING=VC resume"); break;
    }

    fprintf(stdout,outchar);
    fflush(stdout);
}



void logic_info(int fd,char info_result1,char info_result2,char info_result3)
  {

    static int info_count=1;
    int  k1=0;
    if(starttest==0)
    artExec(fd,"out:dig:off(0x80)",result,0);
    if(starttest==1)
      {
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //����Ҫ�ؼ�
          artExec(fd,"out:dig:on(0x80)",result,0);
        else
          artExec(fd,"out:dig:off(0x80)",result,0);
      }

    if(Buzzer(info_result1,info_result2,info_result3)==0) //����Ҫ�ؼ�
      {
        //��������
          showdspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
             showdspstate(1);
            //IA����
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
              showdspstate(2);

            //IABC �¶ȱ���
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //IC��Ъ
                showdspstate(3);
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
        if((info_result1&0x02)==0x02&&I2Output==1)
          {  //info_result1�ǵ�1·��ַ��DB9~DB15��

                showdspstate(4);
            //IB����
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1�ǵ�1·��ַ��DB8~DB15��

                showdspstate(5);
           //IC����
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
                showdspstate(6);
          //IA��Ъ
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
        if((info_result2&0x80)==0x80&&I3Output==1)
          { //info_result2�ǵ�2·��ַ��DB9~DB15��
           //IC����
                showdspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1�ǵ�1·��ַ��DB8~DB15��
          //IABC ��Դ����
                showdspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1�ǵ�1·��ַ��DB9~DB15��
                showdspstate(9);
          //I2��Ъ  ?�¶ȱ�����Ъ
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
        if((info_result2&0x10)==0x10&&V1Output==1)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
           //VA����
                showdspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��
                showdspstate(11);
              //V2����
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3�ǵ�2·��ַ��DB0~DB7��

              //V3��Ъ
                showdspstate(12);
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
        if((info_result2&0x02)==0x02&&V2Output==1)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
            //V2����
                showdspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��  //DB8
           // VABC �¶ȱ���
              showdspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
              showdspstate(15);
           //VA��Ъ
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
        if((info_result3&0x40)==0x40&&V3Output==1)
          { //info_result3�ǵ�2·��ַ��DB0~DB7��

               showdspstate(16);
            //VC����
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��  //DB13
              showdspstate(17);
            //VABC��Դ����
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2�ǵ�2·��ַ��DB9~DB15��
              showdspstate(18);


          //Vz����

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
          {//info_result3�ǵ�2·��ַ��DB0~DB7��
              showdspstate(19);

             //ֱ������
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2�ǵ�2·��ַ��DB8~DB15��
            showdspstate(20);

            // VABC �¶ȱ���
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3�ǵ�2·��ַ��DB0~DB7��
              showdspstate(21);
           //V3��Ъ
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

  }


float CheckAngle(QVector<ACTSTATE>  vect,int type)
{
  if(type==1)  //�ҵ�һ���߽�Ƕ�
  {
      int first=vect.at(0).Triped;
      int i=1;
      for(i=1;i<vect.size();i++)
       {
        if(vect.at(i).Triped!=first)
            return vect.at(i).Angle;
       }
       if(i=vect.size())
       {
           return -1; //δ�ҵ��߽��
       }
  }



}






void  ReadTripTimeLogic(int fd)
{

       int t3=0,t4=0,t5=0;
       unsigned char signature;
       double u,v,w;

       static double SequencerTime4;
       artExec(fd,"inp:buf:get?",result,15);//��DSP�õ��������ӵ���Ϣ��
       u=result[7];
       v=result[8];
       w=result[9];
       signature=int(result[13]&0x7f);
       switch(signature)
         {
           case 1:
             break;
           case 2:
             break;
           case 3:
             t3=1;
             break;
           case 4:
             t4=1;
             SequencerTime4=gettime(u,v,w);//����ӿ�ʼ���Ե�sequencer��Step4��ʱ�䡣
             break;
           case 5:
             t5=1;
             break;
         }

       if(t3==1)
         {  //δ����
           Triptime=-1;
         }

       if(t4==1)
         {//����
           Triptime=1;
         }


       if(t5==1)
       {

           output_signal_end(fd);

           if(Triptime==-1)
            {

                ACTSTATE tmp;
                tmp.Triped=0;
                tmp.Angle=OutPhase;
                AngleActStateVect.append(tmp);
                if(OutPhase==StartPhase)
                {
                  if(SearchType==0)
                  OutPhase=OutPhase+StepPhase;
                  else
                  OutPhase=OutPhase-StepPhase;
                  artExec(fd,"out:dig:on(0x08)",result,0);
                  outputchar();
                  OutputSequencer(fd);

                  return;
                }

                if(CheckAngle(AngleActStateVect,1)!=-1)  //�ҵ��˵�һ���߽�Ƕ�
                {
                    if(Angle1==-1000)  //�ҵ���һ���߽�Ƕ�  ,��������һ���߽��
                    {
                     if(AngleActStateVect.at(0).Triped==0)   //  //�����һ��û�ж�������ô������ǰ����
                     {
                     int size=AngleActStateVect.size();
                     Angle1=AngleActStateVect.at(size-1).Angle; //�����һ����û�����ģ���ô�߽��Ϊ���һ��
                     ACTSTATE tmp2=AngleActStateVect.at(size-1);
                     AngleActStateVect.clear();
                     AngleActStateVect.append(tmp2);
                     OutPhase=OutPhase+StepPhase;
                     artExec(fd,"out:dig:on(0x08)",result,0);

                     outputchar();
                     OutputSequencer(fd);

                     SearchType=0;


                     }
                     else  if(AngleActStateVect.at(0).Triped==1)  //�����һ����������ô���෴�ķ�������
                     {

                         int size=AngleActStateVect.size();
                         Angle1=AngleActStateVect.at(size-2).Angle;  //�����ڶ���Ϊ�߽��1
                         ACTSTATE tmp2=AngleActStateVect.at(0);
                         AngleActStateVect.clear();
                         AngleActStateVect.append(tmp2);

                         OutPhase=StartPhase-StepPhase;
                         artExec(fd,"out:dig:on(0x08)",result,0);

                         outputchar();
                         OutputSequencer(fd);

                         SearchType=1;
                     }
                    }
                    else              //�ҵ��ڶ����߽��,��� �����˳�
                    {
                     int size=AngleActStateVect.size();
                     float SecondAngle;
                     SecondAngle=AngleActStateVect.at(size-2).Angle;
                     Angle2=SecondAngle;
                     SearchType=0;
                     save_f=1;
                     GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                     starttest=0;
                     output_signal_end(fd);
                     AngleActStateVect.clear();
                    }
                }
                else  //û���ҵ��������
                {       if(SearchType==0)
                        OutPhase=OutPhase+StepPhase;
                        else
                        OutPhase=OutPhase-StepPhase;
                        artExec(fd,"out:dig:on(0x08)",result,0);

                        outputchar();
                        OutputSequencer(fd);


                }


            }

           else if(Triptime==1)
           {


               ACTSTATE tmp;
               tmp.Triped=1;
               tmp.Angle=OutPhase;
               AngleActStateVect.append(tmp);
               if(OutPhase==StartPhase)
               {
                   if(SearchType==0)
                   OutPhase=OutPhase+StepPhase;
                   else
                   OutPhase=OutPhase-StepPhase;
                 artExec(fd,"out:dig:on(0x08)",result,0);

                 outputchar();
                 OutputSequencer(fd);

                 return;
               }


               if(CheckAngle(AngleActStateVect,1)!=-1)  //�ҵ��˵�һ���߽�Ƕ�
               {
                   if(Angle1==-1000)  //�ҵ���һ���߽�Ƕ�  ,��������һ���߽��
                   {
                    if(AngleActStateVect.at(0).Triped==0)   //  //�����һ��û�ж�������ô������ǰ����
                    {
                    int size=AngleActStateVect.size();
                    Angle1=AngleActStateVect.at(size-1).Angle; //�����һ����û�����ģ���ô�߽��Ϊ���һ��
                    ACTSTATE tmp2=AngleActStateVect.at(size-1);
                    AngleActStateVect.clear();
                    AngleActStateVect.append(tmp2);

                    OutPhase=OutPhase+StepPhase;
                    artExec(fd,"out:dig:on(0x08)",result,0);

                    outputchar();
                    OutputSequencer(fd);

                    SearchType=0;

                    }
                    else  if(AngleActStateVect.at(0).Triped==1)  //�����һ����������ô���෴�ķ�������
                    {

                        int size=AngleActStateVect.size();
                        Angle1=AngleActStateVect.at(size-2).Angle;  //�����ڶ���Ϊ�߽��1
                        ACTSTATE tmp2=AngleActStateVect.at(0);
                        AngleActStateVect.clear();
                        AngleActStateVect.append(tmp2);

                        OutPhase=StartPhase-StepPhase;
                        artExec(fd,"out:dig:on(0x08)",result,0);

                        outputchar();
                        OutputSequencer(fd);

                        SearchType=1;

                    }
                   }
                   else              //�ҵ��ڶ����߽��,��� �����˳�
                   {
                    int size=AngleActStateVect.size();
                    float SecondAngle;
                    SecondAngle=AngleActStateVect.at(size-2).Angle;
                    Angle2=SecondAngle;
                    SearchType=0;
                    save_f=1;
                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //��
                    starttest=0;
                    output_signal_end(fd);

                    AngleActStateVect.clear();

                   }
               }
               else  //û���ҵ��������
               {       if(SearchType==0)
                       OutPhase=OutPhase+StepPhase;
                       else
                       OutPhase=OutPhase-StepPhase;
                       artExec(fd,"out:dig:on(0x08)",result,0);

                       outputchar();
                       OutputSequencer(fd);

               }


           }


       }


}
