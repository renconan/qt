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



extern int StartPhaseActState;  //0未动作,1动作
extern int SearchType;  //搜索类型  0 逆时针  1 顺时针

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
  {//sys[1]:过载，sys[2]:间歇，sys[3]:闭锁
    if(sys[1]==1)
      {//过载信息
        if((Address1&0x08)!=0x00&&I1Output==1)
          return 1;//I1过载
        if((Address1&0x02)!=0x00&&I2Output==1)
          return 1;//I2过载
        if((Address2&0x80)!=0x00&&I3Output==1)
          return 1;//I3过载
        if((Address2&0x10)!=0x00&&V1Output==1)
          return 1;//V1过载
        if((Address2&0x02)!=0x00&&V2Output==1)
          return 1;//V2过载
        if((Address3&0x40)!=0x00&&V3Output==1)
          return 1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    else if(sys[1]==0)
      {//过载信息
        if((Address1&0x08)!=0x00&&I1Output==1)
          return -1;//I1过载
        if((Address1&0x02)!=0x00&&I2Output==1)
          return -1;//I2过载
        if((Address2&0x80)!=0x00&&I3Output==1)
          return -1;//I3过载
        if((Address2&0x10)!=0x00&&V1Output==1)
          return -1;//V1过载
        if((Address2&0x02)!=0x00&&V2Output==1)
          return -1;//V2过载
        if((Address3&0x40)!=0x00&&V3Output==1)
          return -1;//V3过载
        if((Address3&0x20)!=0x00)
          return 1;//直流过载
      }
    if(sys[2]==1)
      {//间歇信息
        if((Address1&0x10)!=0x00)
          return 1;//I1间歇
        if((Address1&0x04)!=0x00)
          return 1;//I2间歇
        if((Address1&0x01)!=0x00)
          return 1;//I3间歇
        if((Address2&0x08)!=0x00)
          return 1;//V1间歇
        if((Address2&0x04)!=0x00)
          return 1;//V2间歇
        if((Address3&0x80)!=0x00)
          return 1;//V3间歇
      }
    else if(sys[2]==0)
      {//间歇信息
        if((Address1&0x10)!=0x00)
          return -1;//I1间歇
        if((Address1&0x04)!=0x00)
          return -1;//I2间歇
        if((Address1&0x01)!=0x00)
          return -1;//I3间歇
        if((Address2&0x08)!=0x00)
          return -1;//V1间歇
        if((Address2&0x04)!=0x00)
          return -1;//V2间歇
        if((Address3&0x80)!=0x00)
          return -1;//V3间歇
      }
    if(sys[3]==1)
      {//闭锁信息
        if((Address1&0x80)!=0x00)
          return 1;//I1闭锁
        if((Address1&0x40)!=0x00)
          return 1;//I2闭锁
        if((Address1&0x20)!=0x00)
          return 1;//I3闭锁
        if((Address2&0x20)!=0x00)
          return 1;//V1闭锁
        if((Address2&0x40)!=0x00)
          return 1;//V2闭锁
        if((Address2&0x01)!=0x00)
          return 1;//V3闭锁
      }
    else if(sys[3]==0)
      {//闭锁信息
        if((Address1&0x80)!=0x00)
          return -1;//I1闭锁
        if((Address1&0x40)!=0x00)
          return -1;//I2闭锁
        if((Address1&0x20)!=0x00)
          return -1;//I3闭锁
        if((Address2&0x20)!=0x00)
          return -1;//V1闭锁
        if((Address2&0x40)!=0x00)
          return -1;//V2闭锁
        if((Address2&0x01)!=0x00)
          return -1;//V3闭锁
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
        if(Buzzer(info_result1,info_result2,info_result3)==1)  //不需要控件
          artExec(fd,"out:dig:on(0x80)",result,0);
        else
          artExec(fd,"out:dig:off(0x80)",result,0);
      }

    if(Buzzer(info_result1,info_result2,info_result3)==0) //不需要控件
      {
        //运行正常
          showdspstate(0);
        return;
      }
    else if(info_count==1)
      {
        if((info_result1&0x08)==0x08&&I1Output==1)
          {//info_result1是第1路地址的DB9~DB15。
             showdspstate(1);
            //IA过载
            k1=1;
          }
        if((info_result1&0x40)==0x40)
          {
              showdspstate(2);

            //IABC 温度保护
            k1=1;
          }
        if((info_result1&0x01)==0x01)
          {
            //IC间歇
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
          {  //info_result1是第1路地址的DB9~DB15。

                showdspstate(4);
            //IB过载
            k1=1;
          }
        if((info_result1&0x20)==0x20)
          {//info_result1是第1路地址的DB8~DB15。

                showdspstate(5);
           //IC闭锁
            k1=1;
          }
        if((info_result1&0x10)==0x10)
          {//info_result1是第1路地址的DB9~DB15。
                showdspstate(6);
          //IA间歇
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
          { //info_result2是第2路地址的DB9~DB15。
           //IC过载
                showdspstate(7);
            k1=1;
          }
        if((info_result1&0x80)==0x80)
          {//info_result1是第1路地址的DB8~DB15。
          //IABC 电源保护
                showdspstate(8);

            k1=1;
          }
        if((info_result1&0x04)==0x04)
          {//info_result1是第1路地址的DB9~DB15。
                showdspstate(9);
          //I2间歇  ?温度保护间歇
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
          {//info_result2是第2路地址的DB9~DB15。
           //VA过载
                showdspstate(10);

            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15。
                showdspstate(11);
              //V2闭锁
            //k1=1;    ??
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7。

              //V3间歇
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
          {//info_result2是第2路地址的DB9~DB15。
            //V2过载
                showdspstate(13);
            k1=1;
          }
        if((info_result2&0x01)==0x01)
          {//info_result2是第2路地址的DB8~DB15。  //DB8
           // VABC 温度保护
              showdspstate(14);
            k1=1;
          }
        if((info_result2&0x08)==0x08)
          {//info_result2是第2路地址的DB9~DB15。
              showdspstate(15);
           //VA间歇
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
          { //info_result3是第2路地址的DB0~DB7。

               showdspstate(16);
            //VC过载
            k1=1;
          }
        if((info_result2&0x20)==0x20)
          {//info_result2是第2路地址的DB8~DB15。  //DB13
              showdspstate(17);
            //VABC电源保护
            k1=1;
          }
        if((info_result2&0x04)==0x04)
          {//info_result2是第2路地址的DB9~DB15。
              showdspstate(18);


          //Vz过载

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
          {//info_result3是第2路地址的DB0~DB7。
              showdspstate(19);

             //直流过载
            k1=1;
          }
        if((info_result2&0x40)==0x40)
          {//info_result2是第2路地址的DB8~DB15。
            showdspstate(20);

            // VABC 温度保护
            k1=1;
          }
        if((info_result3&0x80)==0x80)
          {//info_result3是第2路地址的DB0~DB7。
              showdspstate(21);
           //V3间歇
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
  if(type==1)  //找第一个边界角度
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
           return -1; //未找到边界角
       }
  }



}






void  ReadTripTimeLogic(int fd)
{

       int t3=0,t4=0,t5=0;
       unsigned char signature;
       double u,v,w;

       static double SequencerTime4;
       artExec(fd,"inp:buf:get?",result,15);//从DSP得到开关量接点信息。
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
             SequencerTime4=gettime(u,v,w);//计算从开始测试到sequencer的Step4的时间。
             break;
           case 5:
             t5=1;
             break;
         }

       if(t3==1)
         {  //未动作
           Triptime=-1;
         }

       if(t4==1)
         {//动作
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

                if(CheckAngle(AngleActStateVect,1)!=-1)  //找到了第一个边界角度
                {
                    if(Angle1==-1000)  //找到第一个边界角度  ,继续找下一个边界角
                    {
                     if(AngleActStateVect.at(0).Triped==0)   //  //如果第一个没有动作，那么继续往前搜索
                     {
                     int size=AngleActStateVect.size();
                     Angle1=AngleActStateVect.at(size-1).Angle; //如果第一个是没动作的，那么边界角为最后一个
                     ACTSTATE tmp2=AngleActStateVect.at(size-1);
                     AngleActStateVect.clear();
                     AngleActStateVect.append(tmp2);
                     OutPhase=OutPhase+StepPhase;
                     artExec(fd,"out:dig:on(0x08)",result,0);

                     outputchar();
                     OutputSequencer(fd);

                     SearchType=0;


                     }
                     else  if(AngleActStateVect.at(0).Triped==1)  //如果第一个动作，那么往相反的方向搜索
                     {

                         int size=AngleActStateVect.size();
                         Angle1=AngleActStateVect.at(size-2).Angle;  //倒数第二个为边界角1
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
                    else              //找到第二个边界角,完成 程序退出
                    {
                     int size=AngleActStateVect.size();
                     float SecondAngle;
                     SecondAngle=AngleActStateVect.at(size-2).Angle;
                     Angle2=SecondAngle;
                     SearchType=0;
                     save_f=1;
                     GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                     starttest=0;
                     output_signal_end(fd);
                     AngleActStateVect.clear();
                    }
                }
                else  //没有找到则继续找
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


               if(CheckAngle(AngleActStateVect,1)!=-1)  //找到了第一个边界角度
               {
                   if(Angle1==-1000)  //找到第一个边界角度  ,继续找下一个边界角
                   {
                    if(AngleActStateVect.at(0).Triped==0)   //  //如果第一个没有动作，那么继续往前搜索
                    {
                    int size=AngleActStateVect.size();
                    Angle1=AngleActStateVect.at(size-1).Angle; //如果第一个是没动作的，那么边界角为最后一个
                    ACTSTATE tmp2=AngleActStateVect.at(size-1);
                    AngleActStateVect.clear();
                    AngleActStateVect.append(tmp2);

                    OutPhase=OutPhase+StepPhase;
                    artExec(fd,"out:dig:on(0x08)",result,0);

                    outputchar();
                    OutputSequencer(fd);

                    SearchType=0;

                    }
                    else  if(AngleActStateVect.at(0).Triped==1)  //如果第一个动作，那么往相反的方向搜索
                    {

                        int size=AngleActStateVect.size();
                        Angle1=AngleActStateVect.at(size-2).Angle;  //倒数第二个为边界角1
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
                   else              //找到第二个边界角,完成 程序退出
                   {
                    int size=AngleActStateVect.size();
                    float SecondAngle;
                    SecondAngle=AngleActStateVect.at(size-2).Angle;
                    Angle2=SecondAngle;
                    SearchType=0;
                    save_f=1;
                    GPIO_OutClear(temprorarydata->gpio_fd, LEDNUM);   //灭
                    starttest=0;
                    output_signal_end(fd);

                    AngleActStateVect.clear();

                   }
               }
               else  //没有找到则继续找
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
