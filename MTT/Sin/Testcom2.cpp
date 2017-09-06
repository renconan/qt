
#include <string.h>
#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include "Config.h"
#include "Defpam.h"

#ifdef ARMLINUX
#include   <unistd.h> //write read
#endif



extern TEMPRORARYDATA *temprorarydata;
extern unsigned  char result[70],receive_data[70];
extern double out_time,change_timedata;
extern float LogicResolution;
extern float FreqRevision,VaPhaseRevision,VbPhaseRevision,VcPhaseRevision,VzPhaseRevision;
extern float VzPhaseRevision,IaPhaseRevision,IbPhaseRevision,IcPhaseRevision;
extern float V1RelaPhase,V2RelaPhase,V3RelaPhase,VzRelaPhase,I1RelaPhase,I2RelaPhase,I3RelaPhase;

unsigned char ID=0x00,response[3];
double max_output_v_a;   //,max_f_v_a=50,min_f_v_a=0.5,output_offset_v_a=0,amp_v_a=360;
double max_output_v_b;   //,max_f_v_b=50,min_f_v_b=0.5,output_offset_v_b=0,amp_v_b=120;
double max_output_v_c;   //,max_f_v_c=50,min_f_v_c=0.5,output_offset_v_c=0,amp_v_c=240;
double max_output_v_z;
double max_output_i_a;//,max_f_i_a=50,min_f_i_a=0.5,output_offset_i_a=0,amp_i_a=75;
double max_output_i_b;//,max_f_i_b=50,min_f_i_b=0.5,output_offset_i_b=0,amp_i_b=195;
double max_output_i_c;//,max_f_i_c=50,min_f_i_c=0.5,output_offset_i_c=0,amp_i_c=315;


unsigned char data_i_a[70],data_i_b[70],data_v_b[70],data_v_a[70],data_v_c[70],data_v_z[70],data_i_c[70];
unsigned char alive_status=0;
extern  float faultduration;

extern float StartValue,EndValue,ActStep,ReturnStep,StepTime;
extern int   ACRelayType,DCRelayType;
extern float MAX_V_VALUEDATA_DC,MAX_V_VALUEDATA_AC,MIN_V_VALUEDATA_DC,MIN_V_VALUEDATA_AC;
extern float MAX_I_VALUEDATA_DC,MAX_I_VALUEDATA_AC,MIN_I_VALUEDATA_DC,MIN_I_VALUEDATA_AC;



int is_digit(unsigned char ch)
{
    if(ch>='0'&&ch<='9')
        return 1;
    else
        return 0;
}
int is_space(unsigned char ch)
{
    if(ch==' ')
        return 1;
    else
        return 0;
}

double my_atof(unsigned char *s)
{
    double power,value;
    int i,sign;
    assert(s!=NULL);//?D??×?·???ê?·??a??
    for(i=0;is_space(s[i]);i++);//3yè￥×?·????°μ?????
    sign=(s[i]=='-')?-1:1;
    if(s[i]=='-'||s[i]=='+')//òaê?óD·?o????í?°??ò???
        i++;
    for(value=0.0;is_digit(s[i]);i++)//????D?êyμ???μ?êy×?
        value=value*10.0+(s[i]-'0');
    if(s[i]=='.')
        i++;
    for(power=1.0;is_digit(s[i]);i++)//????D?êyμ?oóμ?êy×?
    {
        value=value*10.0+(s[i]-'0');
        power*=10.0;
    }
    return sign*value/power;
}


unsigned long pow_my(int base,int n)
{
    unsigned long result=1;
    while(n-->0)
    {
        result *= base;
    }
    return result;
}

/*-----------------------------------------------------------------------------
  oˉêy??:      serial_read
  2?êy:        int fd,char *str,unsigned int len,unsigned int timeout
  ·μ???μ:      ?ú1???μ?ê±???ú?áè?êy?Y￡?3?ê±?òí?3?￡?3?ê±ê±???ams??±e
  ?èê?:        ?òfd?èê?·?μ????ú?óê?êy?Y￡?3�?è?alen￡???è?str￡?timeout ?a3?ê±ê±??
 *-----------------------------------------------------------------------------*/


int serial_read(int fd, unsigned char *str, unsigned int len,unsigned char *cmp_data,int cmp_len)
{





        int ret;		//?????áμ??á1?
        int sret;		//select?à???á1?
        int readlen = 0;	//êμ?ê?áμ?μ?×??úêy
        unsigned char * ptr;
        unsigned int ii=50000;

        ptr = str;
               while(readlen<len&&ii>0)
               {





                               ret=read(fd,ptr,1);
                               if(ret <0)
                               {
                                       perror("read err:");
                                       break;
                               }
                               else if(ret == 0)
                               {
                                   ii--;
                                   continue;
                               }
                               if(readlen<cmp_len)
                               {
                                   if((*ptr^(*cmp_data))!=0)
                                   {


                                     cmp_data-=readlen;
                                     ptr-=readlen;
                                     readlen=0;
                                     continue;
                                   }
                                   cmp_data++;

                               }
                                ii=50000;
                               readlen += ret;									//?üD??áμ?3�?è
                               ptr     += ret;


               }
        return readlen;
}






#ifdef ARMLINUX
//check_f ê?·?DèòaD￡?é  ?÷òaDèòa???a??oˉêy
int receive_send(int fd,unsigned char *Send_str,int Send_len,unsigned char *cmp_data,int Receive_len,int cmp_len,int check_f)
  {



    unsigned char send_checksum,receive_checksum;
    unsigned char *ptr,*ptr1;
    int reclen=0;
    unsigned char buff[512];

    int i=0;
    ptr=Send_str;
    ptr1=Send_str;


    send_checksum=0,receive_checksum=0;
    if(Send_len!=0)
          {
               for(i=0;i<Send_len;i++)
                {

                send_checksum=*ptr^send_checksum;
                ptr++;
                }



             *(Send_str+Send_len)=send_checksum;
            write(fd,Send_str,Send_len+1); //·￠?í×?·?3?è￥


          }



        if(Receive_len!=0)
          {

             reclen= serial_read(fd,receive_data,Receive_len+check_f,cmp_data,cmp_len);

             if(reclen==(Receive_len+check_f))
              {
               if(memcmp(cmp_data,receive_data,cmp_len)==0) //è?1?±è??μ?×?·?2??? ?ò·μ??-1
                  return  1;
                else return -1;

              }


         else return -1;

          }


        return 1;
  }

int transmit(int fd, unsigned char *buffer,int length, int length2)
  {
    unsigned char ackpacket[5]={0x0f,0x5a,0xf0,0xa5,0x00};
    unsigned char send_data1[70]={0xa5,0xa5,0xa5,0xa5};
    int send_len,re_len,cmp_len;
    int i;
    send_len=length+1;
    if(length2)
      re_len=(length2-1)+5;
    else
      re_len=5;
    cmp_len=5;
    send_data1[4]=0x00; //?aà?ê?·?óD?êìa
    buffer=buffer+4;
    for(i=0;i<length-4;i++)
      {
        send_data1[5+i]=*buffer;  // ·￠?íμ?êy?Ysend_data1
        buffer++;
      }

    if (send_len>50 && send_data1[8]==0x03)
       {

        send_data1[10]=0x00;  //Ua+Ub+Uc (for Voltage Generator Z only)

        }



    while(1)
       {
         i=receive_send(fd,send_data1,send_len,ackpacket,re_len,cmp_len,0);

         if(i==1)
           {
             for(i=0;i<length2;i++)
             result[i]=receive_data[5+i]; //receive_data ?°5??ê?ó??eD?o?
             memset(receive_data,0,70);
             return(0);
           }
        memset(receive_data,0,70);
       }
  } /*μ??1,μ?á÷ART?à???μ????*/
int chang_amp(double out_value,double *max_output_data)
    {
     int i;
     if(out_value>*max_output_data)
       out_value=*max_output_data;
     i=(int)(out_value/(*max_output_data)*32767);

     if(abs(i)>32766)
       i=i/abs(i)*32766;
     return(i);
    }
//2016-06-01
int chang_ampStep(double out_value,double *max_output_data)
{
     int i;
     if(out_value>*max_output_data)
       out_value=*max_output_data;
     i=(int)(out_value/(*max_output_data)*8388608);

     if(abs(i)>8388608)
       i=i/abs(i)*8388608;
     return(i);
}
/*ê?????D?êy-->ê?áù??????μ?D?êy*/
void frec(double x,int length,unsigned char *buffer)
    {
        char data1,data2;
        int i;
        x=x-(int)x;
        for(i=0;i<length;i++)
        {
        x=x*16;

        data1=(int)(x);
        data1=data1<<4;

        x=x-(int)x;
        x=x*16;
        data2=(int)(x);
        buffer[i]=data1|data2;
        x=x-(int)x;
        }
        buffer[i]=NULL;
        return;
    }

void frec1( const float f,  int BitsInt, int Bytes,unsigned char *rt)
  {
        unsigned char	intBytes[5]={0,0,0,0,0};	//float Bytes[4];
        int i;
        union {
                char	B[4];
                long	L;
        }  CB;
        short	i1, i2;
        // integer part
        //IntPart = abs((int)f)*(int)pow(2, (16-BitsInt));
        /*for(int i=0; i<2; i++)
                intBytes[i] =0x00;// BytePart[1-i];*/

        // fraction part
        CB.L =(long)((fabs(f)-abs((int)f))*pow(2, (Bytes*8-BitsInt)));

        i2 = BitsInt/8;
        i1 = Bytes - i2;	// bytes of fraction part

        for (i=0; i<i1; i++)
                intBytes[i+i2] += CB.B[i1-i-1];
        // singed value
        /*if (Signed)
        {
                if (f<0 && intBytes[0] < 112)
                        intBytes[0] += 112;		// 0x80
                else  if (f>0 && intBytes[0] >= 112)
                        intBytes[0] -= 112;		// 0x80
        }*/
        for(i=0; i<Bytes;i++)
                *rt++ = intBytes[i];
        return;
  }

 char *strncat1( char *buffer,char *buffer1)
    {
     int i,j;
     for(i=0;buffer[i]!=NULL;)
     i++;
     for(j=0;buffer1[j]!=NULL;j++,i++)
     buffer[i]=buffer1[j];
     buffer[i]=NULL;
     return(buffer);
    }

unsigned char autoc(unsigned char *p)
{
 unsigned  char data,temp;
  if( memcmp(p,"0x",2)==0 )
     {
      temp=*(p+2);
      if(temp>='0'&&temp<='9')
         data=(temp-48)<<4;
      else if(temp>='A'&&temp<='F')
         data=(temp-55)<<4;
      else if(temp>='a'&&temp<='f')
         data=(temp-87)<<4;
      temp=*(p+3);
      if(temp>='0'&&temp<='9')
         data=data|(temp-48);
      else if(temp>='A'&&temp<='F')
         data=data|(temp-55);
      else if(temp>='a'&&temp<='f')
         data=data|(temp-87);
      return(data);
     }
   else
      return(0);
}



//  void artExec(int fd,  char *command_str,unsigned char *result1,int max_result_length)



void artExec(int fd,char *command_str,unsigned char *result,int max_result_length)
    {
      unsigned char *p1,*p2,*p3,*p4,data_cmp[70],data1,data2,data_cmp1[70],*out_data;
      unsigned char sys_data[100],inp_data[100],seq_data[100],amp_data[100],out_data2[100],wavetable[200];
      int i,j,k,l,m,length,status=0;
      double x,max_output,current;
      int y,z;
      long time_out;

      float startvalue,endvalue,actstep;
      int  tripvalue,returnvalue;

      p4=(unsigned char *)command_str;
      p1=(unsigned char *)command_str;
      p2=p1;
      if(memcmp(p1,"sys:",4)==0)//system commands
           {
            for(i=0;i<40;i++)
            sys_data[i]=0x00;
            for(i=0;i<4;i++)
            sys_data[i]=0xa5;
            p1=p1+4;
            if(memcmp(p1,"reset",5)==0)
                {
                 sys_data[4]=0x00;
                 sys_data[5]=0x03;
                 sys_data[6]=0x01;        //command: sys:reset,[01].function:execute a reset on an ART device.two bytes respomse.
                 length=7;
                }
            else if(memcmp(p1,"test?",5)==0)
                {
                 sys_data[4]=0x00;
                 sys_data[5]=0x03;
                 sys_data[6]=0x02;        //command: sys:test? [02].function:start self test of an ART device.two bytes response.
                 length=7;
                }
            else if(memcmp(p1,"cfg?",4)==0)
                {
                 sys_data[4]=0x00;
                 sys_data[5]=0x03;
                 sys_data[6]=0x03;  //command: sys:cfg? [03].function: get system information of an ART device.twenty three bytes respponse.
                 length=7;          //include:ART serial number
                }
            else if(memcmp(p1,"status?",7)==0)
                {
                 sys_data[4]=0x00;
                 sys_data[5]=0x04;
                 sys_data[6]=0x04;  //commande: sys:status?[(<bit_number>)] [04].funtion: get status of an ART device.two bytes response.
                 p1=p1+7;
                 if(p1==NULL)
                 sys_data[7]=0x00;
                 else
                 sys_data[7]=*p1|0x1f;
                 length=8;
                }
            else if(memcmp(p1,"rptrps",6)==0)
                {
                 sys_data[4]=0x00;
                 sys_data[5]=0x03;
                 sys_data[6]=0x05;  //command: sys:rptrps [05].function:request to resend last resopnse.no response.
                 length=7;
                }
                else if(memcmp(p1,"flash:run",9)==0)
                {
                 sys_data[4]=0x00;
                 sys_data[5]=0x03;
                 sys_data[6]=0x0e;
                 length=7;
                }
           transmit(fd,sys_data,length,max_result_length);
           }

 else if(memcmp(p1,"out:",4)==0)  //output commands
            {
              p1=p1+4;

              if(memcmp(p1,"ana:",4)==0)
                {
                 for(i=0;i<39;i++)
                 {
                  out_data2[i]=0x00;
                 }
                 for(i=0;i<4;i++)
                  {
                  out_data2[i]=0xa5;
                  data_v_a[i]=0xa5;
                  data_i_a[i]=0xa5;
                  data_v_b[i]=0xa5;
                  data_i_b[i]=0xa5;
                  data_v_c[i]=0xa5;
                  data_i_c[i]=0xa5;
                  data_v_z[i]=0xa5;
                  }
                 p1=p1+4;
                 if(memcmp(p1,"v(",2)==0)
                 {
                          p2=p1+7;
                          if(memcmp(p2,"on",2)==0)
                             {
                              out_data2[4]=0x00;  //command: out:[ana:]v(<generator>):on OR out:[ana:]on  [11].
                              out_data2[5]=0x04;  //function:Switch on all generators listed in <generator_list>.
                              out_data2[6]=0x11;   //Switch on all defined generators if the 2nd byte is all ones.
                              if(memcmp(p1,"v(1:",4)==0)
                               out_data2[7]=0x00;
                              else if(memcmp(p1,"v(2:",4)==0)
                                out_data2[7]=0x04;
                              else if(memcmp(p1,"v(3:",4)==0)
                                out_data2[7]=0x08;
                              else if(memcmp(p1,"v(4:",4)==0)
                                out_data2[7]=0x0c;
                                p1=p1+4;
                               data1=(p1[0]&0x3)-1;
                               out_data2[7]=out_data2[7]|data1;
                                length=8;
                             transmit(fd,out_data2,length,max_result_length);
                          }
                          else if(memcmp(p2,"off",3)==0)
                              {
                               out_data2[4]=0x00;
                               out_data2[5]=0x04;
                               out_data2[6]=0x12;
                              if(memcmp(p1,"v(1:",4)==0)
                                out_data2[7]=0x00;
                              else if(memcmp(p1,"v(2:",4)==0)
                                out_data2[7]=0x04;
                              else if(memcmp(p1,"v(3:",4)==0)
                                out_data2[7]=0x08;
                              else if(memcmp(p1,"v(4:",4)==0)
                                out_data2[7]=0x0c;
                               p1=p1+4;
                               data1=(p1[0]&0x3)-1;
                               out_data2[7]=out_data2[7]|data1;
                               length=8;
                              transmit(fd,out_data2,length,max_result_length);
                              }

                         else
                         {
                          if(memcmp(p1,"v(1:",4)==0)
                          {
                            p2=p1+4;
                            if(memcmp(p2,"1):",3)==0)
                              {
                              max_output=max_output_v_a;
                              out_data=data_v_a;
                              data1=0x00;
                              }
                            else if(memcmp(p2,"2):",3)==0)
                              {
                              out_data=data_v_b;
                              max_output=max_output_v_b;
                              data1=0x01;
                              }
                            else if(memcmp(p2,"3):",3)==0)
                              {
                              out_data=data_v_c;
                              max_output=max_output_v_c;
                              data1=0x02;
                              }
                            else if(memcmp(p2,"4):",3)==0)
                              {
                              out_data=data_v_z;
                              max_output=max_output_v_z;
                              data1=0x03;
                              }

                           out_data[8]=0x00;
                           out_data[9]=out_data[9]&0xfb;
                           out_data[7]=data1;
                           }
                        else if(memcmp(p1,"v(2:",4)==0)
                           {
                              p2=p1+4;
                            if(memcmp(p2,"1):",3)==0)
                              {
                              out_data=data_i_a;
                              max_output=max_output_i_a;
                              data1=0x04;
                              }
                            else if(memcmp(p2,"2):",3)==0)
                              {
                              out_data=data_i_b;
                              max_output=max_output_i_b;
                              data1=0x05;
                              }
                            else if(memcmp(p2,"3):",3)==0)
                              {
                              out_data=data_i_c;
                              max_output=max_output_i_c;
                              data1=0x06;
                              }
                           out_data[7]=data1;
                           out_data[8]=0x10;
                           out_data[9]=out_data[9]|0x04;
                        }
                        else if(memcmp(p1,"v(3:",4)==0)
                           out_data[7]=0x08;
                        else if(memcmp(p1,"v(4:",4)==0)
                        out_data[7]=0x0c;

                        out_data[4]=0x00;
                        out_data[5]=0x37;
                        out_data[6]=0x10;
                        p1=p1+7;
                        if(memcmp(p1,"sig1:",5)==0)
                          {
                           status=0;
                           p2=out_data+11;
                           k=1;
                          }
                        else if(memcmp(p1,"sig2:",5)==0)
                          {
                           status=1;
                           p2=out_data+23;
                           k=2;
                          }
                          p1=p1+5;
                          if(memcmp(p1,"a(",2)==0)
                                {

                                 p1=p1+2;
                                 for(i=0;*p1!=NULL;i++)
                                   data_cmp[i]=*p1++;
                                   data_cmp[i]='\0';
                                   x=my_atof(data_cmp);

                                   p1=p1-5;
                                   if(memcmp(p1,"step",4)==0)
                                     {
                                        y=chang_ampStep(x,&max_output);
                                        p2[25]=(unsigned char)(y>>8);
                                        p2[24]=(unsigned char)(y>>16);
                                        p2[26]=(unsigned char)y;
                                        p2[27]=0x00;
                                     }
                                   else
                                     {
                                        y=chang_amp(x,&max_output);
                                        p2[1]=(unsigned char)y;
                                        p2[0]=(unsigned char)(y>>8);
                                        p2[2]=0x00;
                                        p2[3]=0x00;
                                     }
                                 }
                          else if(memcmp(p1,"f(",2)==0)
                                {
                                  p1=p1+2;
                                         for(i=0;*p1!=NULL;i++)
                                   data_cmp[i]=*p1++;
                                   data_cmp[i]='\0';              //NULL;
                                   x=my_atof(data_cmp);
                                   y=(int)fabs(x);
                                   p1=p1-5;
                                   if(memcmp(p1,"step",4)==0)
                                     {
                                        p2[29]=(unsigned char)y;      /*   //p3[0];          */
                                        p2[28]=(unsigned char)(y>>8);      /*       //p3[1];        */
                                        //frec(x,2,data_cmp1);
                                        frec1(x,16,5,data_cmp1);
                                        p2[30]=data_cmp1[2];
                                        p2[31]=data_cmp1[3];
                                        p2[32]=data_cmp1[4];
                                        //p2[30]=data_cmp1[0];
                                       // p2[31]=data_cmp1[1];
                                        if (x<0 && p2[28] < 0x80)
                                          p2[28]+=0x80;
                                        else  if (x>0 && p2[28] >= 0x80)
                                          p2[28]-=0x80;
                                     }
                                   else
                                     {
                                        p2[5]=(unsigned char)y;      /*   //p3[0];          */
                                        p2[4]=(unsigned char)(y>>8);      /*       //p3[1];        */
                                       // frec(x,2,data_cmp1);
                                       // p2[6]=data_cmp1[0];
                                       // p2[7]=data_cmp1[1];
                                        frec1(x,16,5,data_cmp1);
                                        p2[6]=data_cmp1[2];
                                        p2[7]=data_cmp1[3];
                                        p2[8]=data_cmp1[4];
                                     }
                                }
                          else if(memcmp(p1,"p(",2)==0)
                                {
                                 p1=p1+2;
                                    for(i=0;*p1!=NULL;i++)
                                   data_cmp[i]=*p1++;
                                   data_cmp[i]='\0';/*NULL;*/
                                   x=my_atof(data_cmp);
                                   y=(int)x;
                                   data2=(unsigned char)y;
                                   data1=(unsigned char)(y>>8);

                                   p1=p1-5;
                                   if(memcmp(p1,"step",4)==0)
                                     {
                                        p2[33]=(data1<<6)|(data2>>2);
                                        p2[34]=data2<<6;
                                        //frec1(x,10,3,data_cmp1);
                                        frec(x,2,data_cmp1);
                                        p2[34]=p2[34]|(data_cmp1[0]>>2);
                                        p2[35]=(data_cmp1[0]<<6)|(data_cmp1[1]>>2);
                                        /*frec1(x,10,3,data_cmp1);
                                        p2[33]=data_cmp1[1];
                                        p2[34]=data_cmp1[2];
                                        p2[35]=data_cmp1[3];*/
                                     }
                                   else
                                     {
                                        p2[9]=(data1<<6)|(data2>>2);
                                        p2[10]=data2<<6;
                                        frec(x,2,data_cmp1);
                                        //frec1(x,10,3,data_cmp1);
                                        p2[10]=p2[10]|(data_cmp1[0]>>2);
                                        p2[11]=(data_cmp1[0]<<6)|(data_cmp1[1]>>2);
                                       /* frec1(x,10,3,data_cmp1);
                                       // p2[9]=data_cmp1[1];
                                        p2[10]=data_cmp1[1];
                                        p2[11]=data_cmp1[2];*/

                                      }
                                }
                          else if(memcmp(p1,"wav(",4)==0)
                                {
                                 p1=p1+4;
                                 if(memcmp(p1,"sin",3)==0)
                                    {
                                     if(k==1)
                                     out_data[10]=out_data[10]&0x00;//0xf0
                                     else if(k==2)
                                     out_data[10]=out_data[10]&0x0f;
                                    }
                                 else if(memcmp(p1,"sum",3)==0)
                                    {
                                     p1=p1+3;
                                     if(k==1)
                                     {
                                     out_data[10]=out_data[10]&0xf0;
                                     out_data[10]=out_data[10]|0x8;
                                     }
                                     else if(k==2)
                                     {
                                     out_data[23]=out_data[23]&0xf;
                                     out_data[23]=out_data[23]|0x80;
                                     }
                                     p1++;
                                     if(memcmp(p1,"harmonic",8)==0)
                                     {
                                      p1=p1+8;
                                      p1++;
                                      for(i=0;p1[i]!=')';)
                                        i++;
                                        i--;
                                        data2=0;
                                        data1=1;
                                       for(j=i;j>=0;j--)
                                        {
                                          data2=data2+(unsigned char)((p1[j]&0xf)*data1);
                                          data1=(unsigned char)(data1*10);
                                        }
                                       if(k==1)
                                       out_data[13]=data2;
                                       else
                                       out_data[25]=data2;
                                     }
                                     /*        //??íê3é       */


                                     length=59;

                                    }
                                 else if(memcmp(p1,"tri",3)==0||memcmp(p1,"square",6)==0)
                                    {
                                     if(memcmp(p1,"square",6)==0)
                                    {
                                     if(k==1)
                                     out_data[10]=(out_data[10]&0xf3)|0x3;
                                     else if(k==2)
                                     out_data[10]=(out_data[10]&0x3f)|0x30;
                                     p1=p1+6;
                                    }
                                     else if(memcmp(p1,"tri",3)==0)
                                     {
                                     if(k==1)
                                     out_data[10]=(out_data[10]&0xf2)|0x2;
                                     else if(k==2)
                                     out_data[10]=(out_data[10]&0x2f)|0x20;
                                     p1=p1+3;
                                     }
                                     if(p1[0]!=')')
                                      {
                                       p1++;
                                       for(i=0;p1[i]!=')';i++)
                                       data_cmp[i]=p1[i];
                                       data_cmp[i]='\0';                 //NULL;
                                       x=my_atof(data_cmp);
                                       x=1/x/2*10000;
                                       y=(int)x;
                                       p2[3]=(unsigned char)(y>>8);             //p3[1];
                                       p2[4]=(unsigned char)y;                //p3[0];
                                       frec(x,1,data_cmp1);
                                       p2[5]=data_cmp1[0];
                                       x=my_atof(data_cmp);
                                       x=1/x*10000;
                                       y=(int)x;
                                       p2[6]=(unsigned char)(y>>8);              //p3[1];
                                       p2[7]=(unsigned char)y;                //p3[0];
                                       frec(x,1,data_cmp1);
                                       p2[8]=data_cmp1[0];
                                      }
                                     else
                                     {
                                      p2[2]=0;
                                      p2[3]=0;
                                      p2[4]=0xc8;
                                      p2[5]=0;
                                      p2[6]=01;
                                      p2[7]=0x90;
                                     }
                                    }

                                 else if(memcmp(p1,"dc",2)==0)
                                    {

                                     if(k==1)
                                     out_data[10]=(out_data[10]&0xf0)|0x6;
                                     else if(k==2)
                                     out_data[10]=(out_data[10]&0xf)|0x60;
                                    }
                                 else if(memcmp(p1,"exp",3)==0)
                                    {
                                     if(k==1)
                                     out_data[10]=(out_data[10]&0xf7)|0x7;
                                     else if(k==2)
                                     out_data[10]=(out_data[10]&0x7f)|0x70;
                                     p1=p1+3;
                                      if(p1[0]!=')')
                                      {
                                       p1++;
                                       for(i=0;p1[i]!=')';i++)
                                       data_cmp[i]=p1[i];
                                       data_cmp[i]='\0';          //NULL;
                                       x=my_atof(data_cmp);
                                       x=x*100000000;
                                       time_out=(long)(x);
                                       p2[3]=(unsigned char)(time_out>>16);
                                       p2[4]=(unsigned char)(time_out>>8);
                                       p2[5]=(unsigned char)(time_out);
                                     }
                                    }
                                 else if(memcmp(p1,"user",4)==0)
                                    {
                                     if(k==1)
                                     {
                                     out_data[10]=(out_data[10]&0xf4)|0x4;
                                     p1=p1+4;
                                     p1++;
                                     for(i=0;p1[i]!=')'&&p1[i]!=',';)
                                        i++;
                                        i--;
                                        data2=0;
                                        data1=1;
                                       for(j=i;j>=0;j--)
                                        {
                                          data2=data2+(unsigned char)((p1[j]&0xf)*data1);
                                          data1=(unsigned char)(data1*10);
                                        }
                                       out_data[13]=data2;
                                      }
                                      else if(k==2)
                                     {
                                     out_data[10]=(out_data[10]&0x4f)|0x40;
                                     p1=p1+4;
                                     p1++;
                                      for(i=0;p1[i]!=')'&&p1[i]!=',';)
                                        i++;
                                        i--;
                                        data2=0;
                                        data1=1;
                                       for(j=i;j>=0;j--)
                                        {
                                          data2=data2+(unsigned char)((p1[j]&0xf)*data1);
                                          data1=(unsigned char)(data1*10);
                                        }
                                       out_data[25]=data2;
                                     }
                                 }
                                  length=59;

                                }
                                length=59;
                   }}
                 else if(memcmp(p1,"i(",2)==0)
                 {

                          p2=p1+7;
                          if(memcmp(p2,"on",2)==0)
                             {
                              out_data2[4]=0x00;
                              out_data2[5]=0x04;
                              out_data2[6]=0x11;
                              if(memcmp(p1,"i(1:1",5)==0)
                               out_data2[7]=0x04;
                              else if(memcmp(p1,"i(1:2",5)==0)
                                out_data2[7]=0x05;
                              else if(memcmp(p1,"i(1:3",5)==0)
                                out_data2[7]=0x06;
                                length=8;
                             transmit(fd,out_data2,length,max_result_length);
                          }
                          else if(memcmp(p2,"off",3)==0)
                              {
                               out_data2[4]=0x00;
                               out_data2[5]=0x04;
                               out_data2[6]=0x12;
                              if(memcmp(p1,"i(1:",4)==0)
                                out_data2[7]=0x00;
                              else if(memcmp(p1,"i(2:",4)==0)
                                out_data2[7]=0x04;
                              else if(memcmp(p1,"i(3:",4)==0)
                                out_data2[7]=0x08;
                              else if(memcmp(p1,"i(4:",4)==0)
                                out_data2[7]=0x0c;
                               p1=p1+4;
                               data1=(p1[0]&0x3)-1;
                               out_data2[7]=out_data2[7]|data1;
                               length=8;
                              transmit(fd,out_data2,length,max_result_length);
                              }

                         else
                         {
                          if(memcmp(p1,"i(1:",4)==0)
                          {
                             p2=p1+4;

                            if(memcmp(p2,"1):",3)==0)
                              {
                              max_output=max_output_i_a;
                              out_data=data_i_a;
                              data1=0x00;
                              }
                            else if(memcmp(p2,"2):",3)==0)
                              {
                              out_data=data_i_b;
                              max_output=max_output_i_b;
                              data1=0x01;
                              }
                            else if(memcmp(p2,"3):",3)==0)
                              {
                              out_data=data_i_c;
                              max_output=max_output_i_c;
                              data1=0x02;
                              }
                           out_data[8]=0x00;
                           out_data[9]=out_data[9]&0xfb;
                           out_data[7]=data1;
                           }
                        else if(memcmp(p1,"i(2:",4)==0)
                           {
                              p2=p1+4;
                            if(memcmp(p2,"1):",3)==0)
                              {
                              out_data=data_i_a;
                              max_output=max_output_i_a;
                              data1=0x04;
                              }
                            else if(memcmp(p2,"2):",3)==0)
                              {
                              out_data=data_i_b;
                              max_output=max_output_i_b;
                              data1=0x05;
                              }
                            else if(memcmp(p2,"3):",3)==0)
                              {
                              out_data=data_i_c;
                              max_output=max_output_i_c;
                              data1=0x06;
                              }
                           out_data[7]=data1;
                           out_data[8]=0x10;
                           out_data[9]=out_data[9]|0x04;
                        }
                        else if(memcmp(p1,"i(3:",4)==0)
                           out_data[7]=0x08;
                        else if(memcmp(p1,"i(4:",4)==0)
                        out_data[7]=0x0c;

                        out_data[4]=0x00;
                        out_data[5]=0x37;
                        out_data[6]=0x10;
                        p1=p1+7;
                        if(memcmp(p1,"sig1:",5)==0)
                          {
                           status=0;
                           p2=out_data+11;
                           k=1;
                          }
                        else if(memcmp(p1,"sig2:",5)==0)
                          {
                           status=1;
                           p2=out_data+23;
                           k=2;
                          }
                          p1=p1+5;
                          if(memcmp(p1,"a(",2)==0)
                                {

                                 p1=p1+2;
                                 for(i=0;*p1!=NULL;i++)
                                   data_cmp[i]=*p1++;
                                   data_cmp[i]='\0';//NULL;
                                   x=my_atof(data_cmp);
                                   p1=p1-5;
                                   if(memcmp(p1,"step",4)==0)
                                     {
                                        y=chang_ampStep(x,&max_output);
                                        p2[25]=(unsigned char)(y>>8);
                                        p2[24]=(unsigned char)(y>>16);
                                        p2[26]=(unsigned char)y;
                                        p2[27]=0x00;
                                     }
                                   else
                                     {
                                       y=chang_amp(x,&max_output);
                                        p2[1]=(unsigned char)y;
                                        p2[0]=(unsigned char)(y>>8);
                                        p2[2]=0x00;
                                        p2[3]=0x00;
                                     }
                                 }
                          else if(memcmp(p1,"f(",2)==0)
                                {
                                  p1=p1+2;
                                         for(i=0;*p1!=NULL;i++)
                                   data_cmp[i]=*p1++;
                                   data_cmp[i]='\0';             //NULL;
                                   x=my_atof(data_cmp);
                                   y=(int)fabs(x);
                                   p1=p1-5;
                                   if(memcmp(p1,"step",4)==0)
                                     {
                                        p2[29]=(unsigned char)y;         //p3[0];
                                        p2[28]=(unsigned char)(y>>8);            //p3[1];
                                        //frec(x,2,data_cmp1);
                                        frec1(x,16,5,data_cmp1);
                                        p2[30]=data_cmp1[2];
                                        p2[31]=data_cmp1[3];
                                        p2[32]=data_cmp1[4];
                                        //p2[30]=data_cmp1[0];
                                       // p2[31]=data_cmp1[1];
                                        if (x<0 && p2[28] < 0x80)
                                          p2[28]+=0x80;
                                        else  if (x>0 && p2[28] >= 0x80)
                                          p2[28]-=0x80;
                                     }
                                   else
                                     {
                                        p2[5]=(unsigned char)y;        //p3[0];
                                        p2[4]=(unsigned char)(y>>8);             //p3[1];
                                       // frec(x,2,data_cmp1);
                                       // p2[6]=data_cmp1[0];
                                       // p2[7]=data_cmp1[1];
                                        frec1(x,16,5,data_cmp1);
                                        p2[6]=data_cmp1[2];
                                        p2[7]=data_cmp1[3];
                                        p2[8]=data_cmp1[4];
                                     }
                                }
                          else if(memcmp(p1,"p(",2)==0)
                                {
                                 p1=p1+2;
                                    for(i=0;*p1!=NULL;i++)
                                   data_cmp[i]=*p1++;
                                   data_cmp[i]='\0';/*NULL;*/
                                   x=my_atof(data_cmp);
                                   y=(int)x;
                                   data2=(unsigned char)y;
                                   data1=(unsigned char)(y>>8);

                                   p1=p1-5;
                                   if(memcmp(p1,"step",4)==0)
                                     {
                                        p2[33]=(data1<<6)|(data2>>2);
                                        p2[34]=data2<<6;
                                        frec(x,2,data_cmp1);
                                        p2[34]=p2[34]|(data_cmp1[0]>>2);
                                        p2[35]=(data_cmp1[0]<<6)|(data_cmp1[1]>>2);
                                     }
                                   else
                                     {
                                        p2[9]=(data1<<6)|(data2>>2);
                                        p2[10]=data2<<6;
                                        frec(x,2,data_cmp1);
                                        p2[10]=p2[10]|(data_cmp1[0]>>2);
                                        p2[11]=(data_cmp1[0]<<6)|(data_cmp1[1]>>2);
                                      }
                                }
                          else if(memcmp(p1,"wav(",4)==0)
                                {
                                 p1=p1+4;
                                 if(memcmp(p1,"sin",3)==0)
                                    {
                                     if(k==1)
                                     out_data[10]=out_data[10]&0x00;//0xf0
                                     else if(k==2)
                                     out_data[10]=out_data[10]&0x0f;
                                    }
                                 else if(memcmp(p1,"sum",3)==0)
                                    {
                                     p1=p1+3;
                                     if(k==1)
                                     {
                                     out_data[10]=out_data[10]&0xf0;
                                     out_data[10]=out_data[10]|0x8;
                                     }
                                     else if(k==2)
                                     {
                                     out_data[23]=out_data[23]&0xf;
                                     out_data[23]=out_data[23]|0x80;
                                     }
                                     p1++;
                                     if(memcmp(p1,"harmonic",8)==0)
                                     {
                                      p1=p1+8;
                                      p1++;
                                      for(i=0;p1[i]!=')';)
                                        i++;
                                        i--;
                                        data2=0;
                                        data1=1;
                                       for(j=i;j>=0;j--)
                                        {
                                          data2=data2+(unsigned char)((p1[j]&0xf)*data1);
                                          data1=(unsigned char)(data1*10);
                                        }
                                       if(k==1)
                                       out_data[13]=data2;
                                       else
                                       out_data[25]=data2;
                                     }
                                     /*        //??íê3é       */


                                     length=59;

                                    }
                                 else if(memcmp(p1,"tri",3)==0||memcmp(p1,"square",6)==0)
                                    {
                                     if(memcmp(p1,"square",6)==0)
                                    {
                                     if(k==1)
                                     out_data[10]=(out_data[10]&0xf3)|0x3;
                                     else if(k==2)
                                     out_data[10]=(out_data[10]&0x3f)|0x30;
                                     p1=p1+6;
                                    }
                                     else if(memcmp(p1,"tri",3)==0)
                                     {
                                     if(k==1)
                                     out_data[10]=(out_data[10]&0xf2)|0x2;
                                     else if(k==2)
                                     out_data[10]=(out_data[10]&0x2f)|0x20;
                                     p1=p1+3;
                                     }
                                     if(p1[0]!=')')
                                      {
                                       p1++;
                                       for(i=0;p1[i]!=')';i++)
                                       data_cmp[i]=p1[i];
                                       data_cmp[i]='\0';                //NULL;
                                       x=my_atof(data_cmp);
                                       x=1/x/2*10000;
                                       y=(int)x;
                                       p2[3]=(unsigned char)(y>>8);             //p3[1];
                                       p2[4]=(unsigned char)y;               //p3[0];
                                       frec(x,1,data_cmp1);
                                       p2[5]=data_cmp1[0];
                                       x=my_atof(data_cmp);
                                       x=1/x*10000;
                                       y=(int)x;
                                       p2[6]=(unsigned char)(y>>8);               //p3[1];
                                       p2[7]=(unsigned char)y;                //p3[0];
                                       frec(x,1,data_cmp1);
                                       p2[8]=data_cmp1[0];
                                      }
                                     else
                                     {
                                      p2[2]=0;
                                      p2[3]=0;
                                      p2[4]=0xc8;
                                      p2[5]=0;
                                      p2[6]=01;
                                      p2[7]=0x90;
                                     }
                                    }

                                 else if(memcmp(p1,"dc",2)==0)
                                    {

                                     if(k==1)
                                     out_data[10]=(out_data[10]&0xf0)|0x6;
                                     else if(k==2)
                                     out_data[10]=(out_data[10]&0xf)|0x60;
                                    }
                                 else if(memcmp(p1,"exp",3)==0)
                                    {
                                     if(k==1)
                                     out_data[10]=(out_data[10]&0xf7)|0x7;
                                     else if(k==2)
                                     out_data[10]=(out_data[10]&0x7f)|0x70;
                                     p1=p1+3;
                                      if(p1[0]!=')')
                                      {
                                       p1++;
                                       for(i=0;p1[i]!=')';i++)
                                       data_cmp[i]=p1[i];
                                       data_cmp[i]='\0';         //NULL;
                                       x=my_atof(data_cmp);
                                       x=x*100000000;
                                       time_out=(long)(x);
                                       p2[3]=(unsigned char)(time_out>>16);
                                       p2[4]=(unsigned char)(time_out>>8);
                                       p2[5]=(unsigned char)(time_out);
                                     }
                                    }
                                 else if(memcmp(p1,"user",4)==0)
                                    {
                                     if(k==1)
                                     {
                                     out_data[10]=(out_data[10]&0xf4)|0x4;
                                     p1=p1+4;
                                     p1++;
                                     for(i=0;p1[i]!=')'&&p1[i]!=',';)
                                        i++;
                                        i--;
                                        data2=0;
                                        data1=1;
                                       for(j=i;j>=0;j--)
                                        {
                                          data2=data2+(unsigned char)((p1[j]&0xf)*data1);
                                          data1=(unsigned char)(data1*10);
                                        }
                                       out_data[13]=data2;
                                      }
                                      else if(k==2)
                                     {
                                     out_data[10]=(out_data[10]&0x4f)|0x40;
                                     p1=p1+4;
                                     p1++;
                                      for(i=0;p1[i]!=')'&&p1[i]!=',';)
                                        i++;
                                        i--;
                                        data2=0;
                                        data1=1;
                                       for(j=i;j>=0;j--)
                                        {
                                          data2=data2+(unsigned char)((p1[j]&0xf)*data1);
                                          data1=(unsigned char)(data1*10);
                                        }
                                       out_data[25]=data2;
                                     }
                                 }
                                  length=59;
                                }
                                length=59;
                   }}
                 else if(memcmp(p1,"mix:",4)==0)
                       {
                        p1=p1+4;
                        if(memcmp(p1,"v(",2)==0)
                               {
                                p1=p1+2;
                                if(p1[0]=='1')
                                  {
                                    p1=p1+2;
                                    if(p1[0]=='1')
                                      {
                                      out_data=data_v_a;
                                      out_data[7]=0x00;
                                      out_data[8]=0x00;
                                      }
                                    if(p1[0]=='2')
                                      {
                                      out_data=data_v_b;
                                      out_data[7]=0x01;
                                      out_data[8]=0x00;
                                      }
                                    if(p1[0]=='3')
                                      {
                                      out_data=data_v_c;
                                      out_data[7]=0x02;
                                      out_data[8]=0x00;
                                      }
                                    if(p1[0]=='4')
                                      {
                                      out_data=data_v_z;
                                      out_data[7]=0x03;
                                      out_data[8]=0x00;
                                      }
                                  }
                                else if(p1[0]=='2')
                                  {
                                     p1=p1+2;
                                    if(p1[0]=='1')
                                      {
                                      out_data=data_v_a;
                                      out_data[7]=0x04;
                                      out_data[8]=0x10;
                                      }
                                    if(p1[0]=='2')
                                      {
                                      out_data=data_v_b;
                                      out_data[7]=0x05;
                                      out_data[8]=0x10;
                                      }
                                    if(p1[0]=='3')
                                      {
                                      out_data=data_v_c;
                                      out_data[7]=0x06;
                                      out_data[8]=0x10;
                                      }
                                  }
                                 else if(p1[0]=='3')
                                  {
                                   p1=p1+2;
                                  }
                                 else if(p1[0]=='4')
                                  {
                                   p1=p1+2;
                                  }
                                out_data[4]=0x00;
                                out_data[5]=0x37;
                                out_data[6]=0x10;
                                p1=p1+2;
                                if(memcmp(p1,"sig1",4)==0)
                                 out_data[9]=out_data[9]&0x4;
                                else if(memcmp(p1,"add",3)==0)
                                 {
                                 out_data[9]=out_data[9]&0x05;
                                 }
                                else if(memcmp(p1,"mult",4)==0)
                                 out_data[9]=out_data[9]&0x06;
                                else if(memcmp(p1,"sum",3)==0)
                                 out_data[9]=out_data[9]&0x07;
                               }
                         else if(memcmp(p1,"i(",2)==0)
                               {
                                p1=p1+2;
                                if(p1[0]=='1')
                                  {
                                    p1=p1+2;
                                    if(p1[0]=='1')
                                      {
                                      out_data=data_i_a;
                                      out_data[7]=0x04;
                                      out_data[8]=0x00;
                                      }
                                    if(p1[0]=='2')
                                      {
                                      out_data=data_i_b;
                                      out_data[7]=0x05;
                                      out_data[8]=0x00;
                                      }
                                    if(p1[0]=='3')
                                      {
                                      out_data=data_i_c;
                                      out_data[7]=0x06;
                                      out_data[8]=0x00;
                                      }
                                  }
                                else if(p1[0]=='2')
                                  {
                                     p1=p1+2;
                                    if(p1[0]=='1')
                                      {
                                      out_data=data_i_a;
                                      out_data[7]=0x04;
                                      out_data[8]=0x10;
                                      }
                                    if(p1[0]=='2')
                                      {
                                      out_data=data_i_b;
                                      out_data[7]=0x05;
                                      out_data[8]=0x10;
                                      }
                                    if(p1[0]=='3')
                                      {
                                      out_data=data_i_c;
                                      out_data[7]=0x06;
                                      out_data[8]=0x10;
                                      }
                                  }
                                 else if(p1[0]=='3')
                                  {
                                   p1=p1+2;
                                  }
                                 else if(p1[0]=='4')
                                  {
                                   p1=p1+2;
                                  }
                                out_data[4]=0x00;
                                out_data[5]=0x37;
                                out_data[6]=0x10;
                                p1=p1+2;
                                if(memcmp(p1,"sig1",4)==0)
                                 out_data[9]=out_data[9]&0x4;
                                else if(memcmp(p1,"add",3)==0)
                                 {
                                 out_data[9]=out_data[9]&0x05;
                                 }
                                else if(memcmp(p1,"mult",4)==0)
                                 out_data[9]=out_data[9]&0x06;
                                else if(memcmp(p1,"sum",3)==0)
                                 out_data[9]=out_data[9]&0x07;
                               }

                                length=59;
                         transmit(fd,out_data,length,max_result_length);

                           }
                 else if(memcmp(p1,"on",2)==0)
                 {
                  out_data2[4]=0x00;
                  out_data2[5]=0x04;
                  out_data2[6]=0x11;
                  out_data2[7]=0x00;
                  length=8;
                  transmit(fd,out_data2,length,max_result_length);
                  out_data2[7]=0x04;
                  transmit(fd,out_data2,length,max_result_length);
                 }
                 else if(memcmp(p1,"off",3)==0)
                       {
                         //μ??1μ?á÷???à·?±e1?±?
                         length=8;
                         out_data2[4]=0x00;
                         out_data2[5]=0x04;
                         out_data2[6]=0x12;
                         out_data2[7]=0xc0;
                        transmit(fd,out_data2,length,max_result_length);
                         out_data2[7]=0xc1;
                        transmit(fd,out_data2,length,max_result_length);
                         out_data2[7]=0xc2;
                        transmit(fd,out_data2,length,max_result_length);
                         out_data2[7]=0xc4;
                        transmit(fd,out_data2,length,max_result_length);
                         out_data2[7]=0xc5;
                        transmit(fd,out_data2,length,max_result_length);
                         out_data2[7]=0xc6;
                        transmit(fd,out_data2,length,max_result_length);
                       }
                /*else if(memcmp(p1,"clr",3)==0)
                       {
                         out_data2[4]=0x00;
                         out_data2[5]=0x03;
                         out_data2[6]=0x13;
                         length=7;
                        transmit(fd,out_data2,length,max_result_length);
                       }030627*/
                /* else if(memcmp(p1,"user:app",8)==0)
                       {
                        p1=p1+9;
                                   out_data2[6]=0x15;
                                   l=8;
                        for(i=0;p1[0]!=',';i++)
                                   data_cmp[k]=p1[k];
                                   data_cmp[k]=';';//NULL;
                                   p1=p1+k+1;
                                   i=atoi(data_cmp);
                                   out_data2[7]=i;
                        for(j=0;p1[0]!=')';)
                        {

                             for(i=0;p1[0]!=','&&p1[0]!=')';i++)
                                   data_cmp[i]=p1[i];
                                   data_cmp[i]='\0';//NULL;
                                   x=my_atof(data_cmp);
                                   y=chang_amp(x,&max_output);
                                   out_data2[l++]=(unsigned char)y;
                                   out_data2[l++]=(unsigned char)(y>>8);
                                   p1=p1+k;
                                   if(p1[0]!=')')
                                     p1=p1+1;

                         }
                          out_data2[4]=(l-4)>>8;
                          out_data2[5]=(l-4);
                          length=l;
                        transmit(fd,out_data2,length,max_result_length);
                       }030627*/
                 /*else if(memcmp(p1,"user:init",9)==0)
                       {
                        p1=p1+10;
                        p2=p1;
                        out_data2[6]=0x14;
                        i=0;
                        j=0;
                        m=9;
                        l=10;
                        for(i=0;p2[0]!=')';i++)
                           {
                           for(j=0;p2[0]!=';';)
                             {
                               j++;
                               for(k=0;p2[k]!=','&&p2[k]!=')';k++)
                                   data_cmp[k]=p2[k];
                                   data_cmp[k]=';';//NULL;
                                   p2=p2+k+1;
                                   time_out=atoi(data_cmp);
                                   out_data2[l++]=(unsigned char)(time_out>>8);
                                   out_data2[l++]=(unsigned char)(time_out);
                                for(k=0;p2[k]!=','&&p2[k]!=';'&&p2[k]!=')';k++)
                                   data_cmp[k]=p2[k];
                                   data_cmp[k]=';';       //NULL;

                                   p2=p2+k;
                                   if(p2[0]!=';'&&p2[0]!=')')
                                     p2=p2+1;
                                   x=my_atof(data_cmp)/10000;
                                   y=(int)x;
                                   if(y>0)
                                     {
                                      out_data2[l++]=0x80;
                                      out_data2[l++]=0x00;
                                      out_data2[l++]=0x00;
                                     }
                                   else
                                   {
                                   frec(x,3,data_cmp1);
                                   out_data2[l++]=data_cmp1[0];
                                   out_data2[l++]=data_cmp1[1];
                                   out_data2[l++]=data_cmp1[2];
                                   }
                             }
                             if(p2[0]!=')')
                             p2=p2+1;
                             out_data2[m]=j+1;
                             m=l;
                           }
                           i++;
                           if(i<127)
                             {
                             out_data2[7]=i;
                             out_data2[8]=0;
                             }
                           else
                             {
                              out_data2[7]=127;
                              out_data2[8]=i-127;
                             }
                           out_data2[4]=(l-4)>>8;
                           out_data2[5]=(l-4);
                       length=l;
                       transmit(fd,out_data2,length,max_result_length);
                       }030627*/
                /* else if(memcmp(p1,"user:clr",8)==0)
                       {
                        p1=p1+8;
                        out_data2[4]=0x00;
                        out_data2[5]=0x04;
                        out_data2[6]=0x16;
                        if(p1[0]=='(')
                        {
                        for(k=0;p1[0]!=')';k++)
                                   data_cmp[k]=p1[k];
                                   data_cmp[k]=';';//NULL;
                                   //p1=p1+k+1;
                                   i=atoi(data_cmp);
                                   out_data2[7]=i;
                        }
                        else
                        out_data2[7]=0x7f;
                        length=8;
                        transmit(fd,out_data2,length,max_result_length);
                       }030627*/
                 /*else if(memcmp(p1,"user:mem?",9)==0)
                       {
                        p1=p1+9;
                        out_data2[4]=0x00;
                        out_data2[5]=0x04;
                        out_data2[6]=0x17;
                        if(p1[0]=='(')
                        {
                        for(k=0;p1[0]!=')';k++)
                                   data_cmp[k]=p1[k];
                                   data_cmp[k]=';';//NULL;
                                   //p1=p1+k+1;
                                   i=atoi(data_cmp);
                                   out_data2[7]=i;
                        }
                        else
                         out_data2[7]=0x7f;
                        length=8;
                        transmit(fd,out_data2,length,max_result_length);
                       }  030627*/
        /*	else if(memcmp(p1,"wtbl:app(",9)==0)
                       {
                        p1=p1+9;


                                   wavetable[0]=0xa5;
                                   wavetable[1]=0xa5;
                                   wavetable[2]=0xa5;
                                   wavetable[3]=0xa5;

                                   wavetable[6]=0x18;

                        for(k=0;p1[k]!=',';k++)
                                   data_cmp[k]=p1[k];
                                   data_cmp[k]=';';//NULL;
                                   p1=p1+k+1;
                                   i=atoi(data_cmp);
                                   wavetable[7]=i;
                                   l=8;
                        for(;p1[0]!=')';)
                        {

                             for(i=0;p1[i]!=','&&p1[i]!=')';i++)
                                   data_cmp[i]=p1[i];
                                   data_cmp[i]='\0';//NULL;
                                   x=my_atof(data_cmp);

                                   y=(int)(x*32767);
                                    if(abs(y)>32767)
                                       y=y/abs(y)*32767;
                                    //y=chang_amp(x,&max_output);
                                   wavetable[l++]=(unsigned char)(y>>8);
                                   wavetable[l++]=(unsigned char)(y);
                                   p1=p1+i;
                                   if(p1[0]!=')')
                                     p1=p1+1;

                         }
                          wavetable[4]=(l-4)>>8;
                          wavetable[5]=(l-4);
                          length=l;
                          transmit(fd,wavetable,length,max_result_length);
                           } 030627*/
                         }
              else if(memcmp(p1,"dig:",4)==0)
                {
                 p1=p1+4;
                 for(i=0;i<39;i++)
                 {
                  out_data2[i]=0x00;
                 }
                 for(i=0;i<4;i++)
                  {
                  out_data2[i]=0xa5;
                  }
                 if(memcmp(p1,"on(",3)==0)
                    {
                     p1=p1+3;
                     out_data2[4]=0x00;
                     out_data2[5]=0x05;
                     out_data2[6]=0x19;
                     out_data2[7]=0x0;
                     out_data2[8]=0x0;
                     /*if(p1[0]=='1'&&p1[2]=='0')
                     out_data2[8]=0x01;
                     else if(p1[0]=='0'&&p1[2]=='1')
                     out_data2[8]=0x02;
                     else if(p1[0]=='1'&&p1[2]=='1')
                     out_data2[8]=0x03;*/
                     out_data2[8]=autoc(p1);
                     length=9;
                     transmit(fd,out_data2,length,max_result_length);
                    }
                 else if(memcmp(p1,"off(",4)==0)
                    {
                     p1=p1+4;
                     out_data2[4]=0x00;
                     out_data2[5]=0x05;
                     out_data2[6]=0x1a;
                     /*if(p1[0]=='1'&&p1[2]=='0')
                     out_data2[8]=0x01;
                     else if(p1[0]=='0'&&p1[2]=='1')
                     out_data2[8]=0x02;
                     else if(p1[0]=='1'&&p1[2]=='1')
                     out_data2[8]=0x03;
                    //out_data2[8]=p1[0];   */
                     out_data2[8]=autoc(p1);
                     length=9;
                     transmit(fd,out_data2,length,max_result_length);
                    }
                /*else if(memcmp(p1,"set(",4)==0)
                    {
                     p1=p1+4;
                     out_data2[4]=0x00;
                     out_data2[5]=0x05;
                     out_data2[6]=0x1b;
                     out_data2[7]=0x0;
                     for(i=0;p1[i]!=')';i++)
                     {
                      if(p1[i]==',')
                        i++;
                      out_data2[8]=out_data2[8]|(p1[i]&0xf);
                     }
                     //out_data2[8]=p1[0]&0xf;
                     length=9;
                     transmit(fd,out_data2,length,max_result_length);
                    }030627*/
                          //transmit(fd,out_data2,length,max_result_length);
                }
              else if(memcmp(p1,"cfg?",4)==0)
                   {
                     out_data2[4]=0x00;
                     out_data2[5]=0x03;
                     out_data2[6]=0x1c;
                     length=7;
                   transmit(fd,out_data2,length,max_result_length);
                   }
              else if(memcmp(p1,"returnlimit",11)==0)
              {
                            startvalue=StartValue;
                            endvalue=EndValue;
                            actstep=ActStep;
                            tripvalue=fabs(startvalue-endvalue)/actstep+1;


                            if(ACRelayType==1)
                            {
                                if(startvalue<endvalue)
                                    returnvalue=startvalue/actstep;
                                else if(startvalue>endvalue)
                                    returnvalue=(MAX_V_VALUEDATA_AC-startvalue)/actstep;
                            }
                            else if(ACRelayType==2)
                            {
                                if(startvalue<endvalue)
                                    returnvalue=startvalue/actstep;
                                else if(startvalue>endvalue)
                                    returnvalue=(MAX_I_VALUEDATA_AC-startvalue)/actstep;
                            }
                            else if(DCRelayType==1)
                            {
                                if(startvalue<endvalue)
                                    returnvalue=startvalue/actstep;
                                else if(startvalue>endvalue)
                                    returnvalue=(MAX_V_VALUEDATA_DC-startvalue)/actstep;
                            }
                            else if(DCRelayType==2)
                            {
                                if(startvalue<endvalue)
                                    returnvalue=startvalue/actstep;
                                else if(startvalue>endvalue)
                                    returnvalue=(MAX_I_VALUEDATA_DC-startvalue)/actstep;
                            }
                  for(i=0;i<39;i++)
                  {
                   out_data2[i]=0x00;
                  }
                  for(i=0;i<4;i++)
                   {
                   out_data2[i]=0xa5;
                   }

                  out_data2[4]=0x00;
                  out_data2[5]=0x07;
                  out_data2[6]=0x82;
                  out_data2[7]=(unsigned char)(tripvalue>>8);
                  out_data2[8]=(unsigned char)(tripvalue);
                  out_data2[9]=(unsigned char)((returnvalue+32768)>>8);
                  out_data2[10]=(unsigned char)(returnvalue+32768);

                  length=11;
                transmit(fd,out_data2,length,max_result_length);
              }
             }
    else if(memcmp(p1,"inp:",4)==0)
            {
             for(i=0;i<40;i++)
             inp_data[i]=0x00;
             for(i=0;i<4;i++)
             inp_data[i]=0xa5;
             inp_data[i]=0x00;
             p1=p1+4;
             if(memcmp(p1,"ana:",4)==0)
                 {
                 p1=p1+4;
                 }
             else if(memcmp(p1,"dig(0):get?",8)==0)
                 {
                 inp_data[5]=0x04;
                 inp_data[6]=0x24;
                 inp_data[7]=0x00;
                 length=8;
                 transmit(fd,inp_data,length,max_result_length);
                }
             else if(memcmp(p1,"dig(1):get?",8)==0)
                 {
                 inp_data[5]=0x04;
                 inp_data[6]=0x24;
                 inp_data[7]=0x01;
                 length=8;
                 transmit(fd,inp_data,length,max_result_length);
                 }
            else if(memcmp(p1,"dig:interval(",13)==0)  //?a1?á?·?±??ê?￡
                 {
                   p1=p1+13;
                   inp_data[5]=0x07;
                   inp_data[6]=0x2d;
                   inp_data[7]=0x00;
                   inp_data[8]=0x00;

                   time_out=(long)(1000*LogicResolution/50);

                   inp_data[7]=(unsigned char)(time_out>>16);
                   inp_data[8]=(unsigned char)(time_out>>8);
                   inp_data[9]=(unsigned char)(time_out); //?ú?μí3éè??à????ó?°?a1?á?·?±??ê?±oó
                  // inp_data[9]=0x3c;  //3ms mxg 2003_3_4
                   inp_data[10]=0x01;  //0x01￡o2?±￡′??óμ????ˉêy?Y￡?0x00:±￡′??óμ????ˉêy?Y?￡
                   length=11;
                   transmit(fd,inp_data,length,max_result_length);
                 }
            else if(memcmp(p1,"cfg?",4)==0)
                {
                 p1=p1+4;
                 inp_data[5]=0x03;
                 inp_data[6]=0x2c;
                 length=7;
                transmit(fd,inp_data,length,max_result_length);
                }
            else if(memcmp(p1,"buf:",4)==0)
                {
                 p1=p1+4;
                 if(memcmp(p1,"get?",4)==0)
                    {
                     inp_data[5]=0x03;
                     inp_data[6]=0x29;
                     length=7;
                    transmit(fd,inp_data,length,max_result_length);
                    }
                 else if(memcmp(p1,"clr",3)==0)
                    {
                     inp_data[5]=0x03;
                     inp_data[6]=0x2a;
                     length=7;
                    transmit(fd,inp_data,length,max_result_length);
                    }
                 else if(memcmp(p1,"sam(",4)==0)
                    {
                     p1=p1+4;
                     inp_data[5]=0x05;
                     inp_data[6]=0x28;
                     if(memcmp(p1,"output",6)==0)
                       {
                        inp_data[7]=0x60;
                        p1=p1+7;
                        if(memcmp(p1,"v(",2)==0)
                          {
                           inp_data[7]=inp_data[7]|(((p1[0]-1)&0xf)<<2);
                           p1=p1+3;
                           inp_data[7]=inp_data[7]|((p1[0]-1)&0x03);
                           inp_data[8]=0x00;
                          }
                       }
                     else if(memcmp(p1,"input",5)==0)
                       {
                        inp_data[7]=0x00;
                        inp_data[8]=0x00;
                       }
                     else if(memcmp(p1,"bin",3)==0)
                       {
                        p1=p1+4;
                        inp_data[7]=0x80;
                        for(i=0;p1[i]!=')';)
                        i++;
                        i--;
                        data2=0;
                        data1=1;
                        for(j=i;j>=0;j--)
                        {
                         data2=data2+(unsigned char)((p1[j]&0xf)*data1);
                         data1=(unsigned char)(data1*10);
                        }
                        inp_data[8]=data2;
                       }
                     length=9;
                    transmit(fd,inp_data,length,max_result_length);
                    }
                }
               else if(memcmp(p1,"dig(",4)==0)
                   {
                    p1=p1+4;
                    for(j=0;p1[j]!=')';)
                     j++;
                    p2=p1+j+2;
                     if(memcmp(p2,"thres(",6)==0)
                        {
                         inp_data[5]=0x06;
                         inp_data[6]=0x25;
                          p2=p2+6;
                         inp_data[7]=p1[0]&0xf;
                                         //p1=p1+2;

                         x=my_atof(p2);
                         y=(int)x;
                         inp_data[8]=(unsigned char)y;             //p3[0];
                                 //frec(x,1,data_cmp1);
                         inp_data[9]=0x00;            //data_cmp1[0];

                         length=10;
                        transmit(fd,inp_data,length,max_result_length);

                        }

                     else if(memcmp(p2,"thres?",6)==0)
                        {
                         inp_data[5]=0x04;
                         inp_data[6]=0x26;
                         while((p1-1)[0]!=')')
                         {
                         inp_data[7]=p1[0]&0xf;
                         p1=p1+2;
                         length=8;
                        transmit(fd,inp_data,length,max_result_length);
                         }
                        }
                   }
                }

 else if(memcmp(p1,"amp:",4)==0)   //Amplifier commands
            {
             for(i=0;i<40;i++)
             amp_data[i]=0x00;
             p1=p1+4;
              amp_data[0]=0xa5;
              amp_data[1]=0xa5;
              amp_data[2]=0xa5;
              amp_data[3]=0xa5;
              amp_data[4]=0x00;
              if(memcmp(p1,"def(",4)==0)
                    {
                     p1=p1+4;
                     p2=p1;
                     for(i=0;p1[i]!=')';p2++,i++)
                       ;
                      p2=p2-5;
                      if(memcmp(p2,"reset",5)==0)
                        {
                         amp_data[5]=0x04;
                         amp_data[6]=0x31;
                         data1=(p1[0]&0x07)<<2;
                         data2=p1[2]&0x03;
                         data1=data1|data2;
                         p1=p1+4;
                         if(memcmp(p1,"int",3)==0)
                            data1=data1&0x7f;
                         else if(memcmp(p1,"ext",3)==0)
                            data1=data1|0x80;
                          amp_data[7]=data1;
                          length=8;
                        }
                      else
                          {
                           amp_data[5]=0x04;
                           amp_data[6]=0x30;
                           data1=(p1[0]&0x07)<<2;
                           data2=p1[2]&0x03;
                           data1=data1|data2;
                           p1=p1+4;
                           if(memcmp(p1,"int",3)==0)
                            data1=data1&0x7f;
                         else if(memcmp(p1,"ext",3)==0)
                            data1=data1|0x80;
                           p1=p1+4;
                           if(memcmp(p1,"v,",2)==0)
                           {data1=data1&0xbf;status=1;}
                           else if(memcmp(p1,"i,",3)==0)
                           {data1=data1|0x40;status=2;}
                           amp_data[7]=data1;
                           p1=p1+2;
                           x=my_atof(p1);
                           y=(int)x;
                           amp_data[8]=(unsigned char)y;                 //p3[0];
                           x=x-(int)x;
                           frec(x,1,data_cmp1);
                           amp_data[9]=data_cmp1[0];
                           for(i=0;p1[i]!=',';)
                           i++;
                           p1=p1+i+1;
                           x=my_atof(p1);
                           y=(int)x;
                           data1=(unsigned char)y;                //p3[0];
                           data2=(unsigned char)(y>>8);            //p3[1];
                           p3[0]=data2;
                           p3[1]=data1;
                           if(status==1)
                              {
                                y=y<<6;
                                amp_data[10]=p3[0];
                                amp_data[11]=p3[1];
                                frec(x,3,data_cmp1);
                                p3[0]=0;
                                p3[1]=data_cmp1[0];
                                p3[2]=data_cmp1[1];
                                p3[3]=data_cmp1[2];
                                time_out=time_out<<6;
                                amp_data[11]=amp_data[11]|p3[0];
                                amp_data[12]=p3[1];
                              }
                             else if(status==2)
                              {
                              amp_data[10]=p3[0];
                              frec(x,2,data_cmp1);
                              amp_data[11]=data_cmp1[0];
                              amp_data[12]=data_cmp1[1];
                              }
                              out_data=amp_data+13;
                              k=0;
                              for(j=0;j<2;j++)
                              {
                              for(i=0;p1[i]!=',';)
                              {i++;}
                              p1=p1+i+1;
                              x=my_atof(p1);
                              y=(int)x;
                                       //p3=&y;
                              out_data[k]=(unsigned char)(y>>8);               //p3[1];
                              k++;
                              out_data[k]=(unsigned char)y;                   //p3[0];
                              k++;
                              frec(x,2,data_cmp1);
                              out_data[k]=data_cmp1[0];
                              k++;
                              out_data[k]=data_cmp1[1];
                              k++;
                              }
                              for(i=0;p1[i]!=',';)
                              i++;
                              p1=p1+i+1;
                              x=my_atof(p1);
                              y=(int)x;
                                  //p3=&y;
                              out_data[k]=(unsigned char)y;//p3[0];
                              k++;
                              frec(x,1,data_cmp1);
                              out_data[k]=data_cmp1[0];
                              length=23;
                          }
                    }
              else if(memcmp(p1,"def?(",5)==0)
                    {
                     p1=p1+5;
                     amp_data[5]=0x04;
                     amp_data[6]=0x32;
                     if(p1[0]=='1')
                       {
                         switch(p1[2])
                           {
                             case '1':
                                    amp_data[7]=0x00;
                                    break;
                             case '2':
                                    amp_data[7]=0x01;
                                    break;
                             case '3':
                                    amp_data[7]=0x02;
                                    break;
                           }
                       }
                     else if(p1[0]=='2')
                       {
                         switch(p1[2])
                           {
                             case '1':
                                   amp_data[7]=0x04;
                                   break;
                             case '2':
                                   amp_data[7]=0x05;
                                   break;
                             case '3':
                                   amp_data[7]=0x06;
                                   break;
                           }
                       }
                     length=8;
                    }

              else if(memcmp(p1,"route(",6)==0)
                    {
                     p1=p1+6;
                     amp_data[5]=0x04;
                     amp_data[6]=0x33;
                     amp_data[7]=0x00;
                     if(memcmp(p1,"v,",2)==0)
                        {
                        data1=0x00;
                        }
                     else if(memcmp(p1,"i,",2)==0)
                        data1=0x11;
                     p1=p1+2;
                     if(memcmp(p1,"clr",3)==0)
                     amp_data[7]=0x80;
                     amp_data[7]=amp_data[7]|data1;
                     length=8;
                    }
              else if(memcmp(p1,"route?(",7)==0)
                    {
                     p1=p1+7;
                     amp_data[5]=0x04;
                     amp_data[6]=0x34;
                     if(memcmp(p1,"v,",2)==0)
                     data1=0x00;
                     if(memcmp(p1,"i,",2)==0)
                     data1=0x01;
                     amp_data[7]=data1;
                     length=8;
                    }
              else if(memcmp(p1,"list?",5)==0)
                    {
                     p1=p1+5;
                     amp_data[5]=0x04;
                     amp_data[6]=0x35;
                     length=7;
                    }
              else if(memcmp(p1,"ctrl(",5)==0)
                    {
                     p1=p1+5;
                     amp_data[5]=0x0c;
                     if(memcmp(p1,"v,",2)==0)
                     amp_data[6]=0x36;
                     else if(memcmp(p1,"i,",2)==0)
                     amp_data[6]=0x37;
                     p1=p1+2;
                     amp_data[7]=p1[0]&0x7;
                     p1=p1+2;
                     for(i=0;p1[i]!=')'&&p1[i]!=',';)
                              {i++;}
                              p1=p1+i;
                              x=my_atof(p1);
                     y=(long int)x;
                     amp_data[8]=(unsigned char)(y>>8);
                     amp_data[9]=(unsigned char)(y>>24);
                     amp_data[10]=(unsigned char)(y>>16);
                     if(p1[0]==',')
                      {
                      p1++;
                      for(i=0;p1[i]!=')';)
                              {i++;}
                              x=my_atof(p1);
                       y=(int)x;
                       amp_data[11]=(unsigned char)(y>>8);
                       amp_data[12]=(unsigned char)y;
                      frec(x,1,data_cmp1);
                      amp_data[13]=data_cmp1[0];
                      }
                     length=14;
                    }
              else if(memcmp(p1,"ctrl?(",6)==0)
                    {
                     p1=p1+6;
                     amp_data[5]=0x04;
                     amp_data[6]=0x38;
                     if(memcmp(p1,"v,",2)==0)
                     data1=0x00;
                     if(memcmp(p1,"i,",2)==0)
                     data1=0x01;
                     amp_data[7]=data1;
                     length=8;
                    }
              else if(memcmp(p1,"on(",3)==0)
                   {
                     p1=p1+3;
                     amp_data[5]=0x04;
                     amp_data[6]=0x39;
                     amp_data[7]=p1[0]&0xf;
                     length=8;
                   }
              else if(memcmp(p1,"off(",4)==0)
                   {
                     p1=p1+4;
                     amp_data[5]=0x04;
                     amp_data[6]=0x3a;
                     amp_data[7]=p1[0]&0xf;
                     length=8;
                   }
               transmit(fd,amp_data,length,max_result_length);
            }
      else if(memcmp(p1,"seq:",4)==0)
            {
              for(i=0;i<40;i++)
              seq_data[i]=0x00;

              seq_data[0]=0xa5;
              seq_data[1]=0xa5;
              seq_data[2]=0xa5;
              seq_data[3]=0xa5;
             p1=p1+4;
             if(memcmp(p1,"begin",5)==0)
                {
                 seq_data[4]=0x00;
                 seq_data[5]=0x03;
                 seq_data[6]=0x40;
                 length=7;
                }
             else if(memcmp(p1,"end",3)==0)
                {
                 seq_data[4]=0x00;
                 seq_data[5]=0x03;
                 seq_data[6]=0x41;
                 length=7;
                }
             else if(memcmp(p1,"clr",3)==0)
                {
                 seq_data[4]=0x00;
                 seq_data[5]=0x03;
                 seq_data[6]=0x42;
                 length=7;
                }
             else if(memcmp(p1,"resumetime(",11)==0)
                {
                  p1=p1+11;
                  seq_data[5]=0x06;
                  seq_data[6]=0x50;

                  for(i=0;p1[i]!=')';i++)
                                   data_cmp[i]=p1[i];
                                   data_cmp[i]='\0';                //NULL;
                            //p1=p1+i;
                                   x=my_atof(data_cmp);

                        time_out=(long)(1000000*x/50);
                            //p3=&time_out;
                        seq_data[7]=(unsigned char)(time_out>>16);   //p3[0];
                        seq_data[8]=(unsigned char)(time_out>>8);    //p3[4];
                        seq_data[9]=(unsigned char)(time_out);      //p3[3];
                length=10;
                }
             else if(memcmp(p1,"wait(",5)==0)
                                          {

                                           p1=p1+5;
                                           seq_data[4]=0x00;
                                           seq_data[5]=0x0F;
                                           seq_data[6]=0x43;
                                           seq_data[7]=0;
                                           seq_data[8]=0;
                                           seq_data[9]=0;
                                           seq_data[10]=0;
                                           seq_data[11]=0;
                                           seq_data[12]=0;
                                           seq_data[13]=0;
                                           seq_data[14]=0;
                                           seq_data[15]=0;
                                           seq_data[16]=0;
                                           seq_data[17]=0;
                                           seq_data[18]=0;

                                           if(memcmp(p1,"and",3)==0||memcmp(p1,"or",2)==0)
                                           {
                                           if(memcmp(p1,"and",3)==0)
                                            {
                                             p1=p1+3;
                                             seq_data[7]=seq_data[7]|0x80;
                                            }
                                           else if(memcmp(p1,"or",2)==0)
                                            {
                                            p1=p1+2;
                                            seq_data[7]=seq_data[7]|0xc0;
                                            }

                                           if(memcmp(p1,"bin(",4)==0)
                                              {
                                                  p1=p1+4;
                                                  switch(p1[0])
                                                 {
                                                  case '0':{
                                                            break;
                                                           }
                                                  case '1':{
                                                            seq_data[9]=seq_data[9]|0x01;

                                                            break;
                                                           }
                                                  case '2':{
                                                            seq_data[9]=seq_data[9]|0x01;
                                                            seq_data[11]=seq_data[11]|0x01;
                                                            break;
                                                           }
                                                 }
                                                  p1=p1+3;
                                                if(memcmp(p1,"bin(",4)==0)
                                                {
                                                  p1=p1+4;
                                                  switch(p1[0])
                                                 {
                                                  case '0':{
                                                            break;
                                                           }
                                                  case '1':{
                                                            seq_data[9]=seq_data[9]|0x02;
                                                            break;
                                                           }
                                                  case '2':{
                                                            seq_data[9]=seq_data[9]|0x02;
                                                            seq_data[11]=seq_data[11]|0x02;
                                                            break;
                                                           }
                                                 }
                                                  p1=p1+3;
                                               if(memcmp(p1,"bin(",4)==0)
                                              {
                                                  p1=p1+4;
                                                  switch(p1[0])
                                                 {
                                                  case '0':{
                                                            break;
                                                           }
                                                  case '1':{
                                                            seq_data[9]=seq_data[9]|0x04;

                                                            break;
                                                           }
                                                  case '2':{
                                                            seq_data[9]=seq_data[9]|0x04;
                                                            seq_data[11]=seq_data[11]|0x04;
                                                            break;
                                                           }
                                                 }
                                                p1=p1+3;
                                                if(memcmp(p1,"bin(",4)==0)
                                              {
                                                  p1=p1+4;
                                                  switch(p1[0])
                                                 {
                                                  case '0':{
                                                            break;
                                                           }
                                                  case '1':{
                                                            seq_data[9]=seq_data[9]|0x08;

                                                            break;
                                                           }
                                                  case '2':{
                                                            seq_data[9]=seq_data[9]|0x08;
                                                            seq_data[11]=seq_data[11]|0x08;
                                                            break;
                                                           }
                                                 }
                                                p1=p1+3;
                                              }}}}

                                           if((seq_data[9]&0x0f)==0)
                                                seq_data[7]=seq_data[7]&0xf;

                                            data1=p1[0]+1;  //移动到 orbin(1),bin(1),bin(1),bin(1),2,1,0,1) 中的2

                                            seq_data[7]=seq_data[7]|((data1&0x03)<<4);
                                            if(p1[2]=='1'||p1[2]=='2')    //延时有效
                                            {    if(p1[2]=='1')
                                               {
                                                 seq_data[7]=seq_data[7]|0x04;
                                                 time_out=(long)(1000*faultduration);
                                                 seq_data[15]=(unsigned char)(time_out>>16);//p3[0];
                                                 seq_data[16]=(unsigned char)(time_out>>8);//p3[4];
                                                 seq_data[17]=(unsigned char)(time_out);//p3[3];
                                               }
                                                else if(p1[2]=='2')
                                                {
                                                    seq_data[7]=seq_data[7]|0x04;
                                                    time_out=(long)(1000*faultduration);
                                                    seq_data[15]=(unsigned char)(time_out>>16);//p3[0];
                                                    seq_data[16]=(unsigned char)(time_out>>8);//p3[4];
                                                    seq_data[17]=(unsigned char)(time_out);//p3[3];
                                                    seq_data[18]=1;
                                                }


                                            }

                                            if(p1[3]==',')
                                              {
                                                  p1=p1+4;
                                               for(i=0;p1[i]!=',';i++)
                                                     data_cmp[i]=p1[i];
                                                     data_cmp[i]='\0';//NULL;
                                                  p1=p1+i;
                                                  if(out_time==-1)
                                                    x=my_atof(data_cmp);
                                                  else if(out_time!=-1)
                                                    x=out_time;
                                                  time_out=(long)(1000*x);
                                                  seq_data[12]=(unsigned char)(time_out>>16);//p3[0];
                                                  seq_data[13]=(unsigned char)(time_out>>8);//p3[4];
                                                  seq_data[14]=(unsigned char)(time_out);//p3[3];
                                                  data1=(p1[1]+1)&0x03|0x8;
                                                  seq_data[7]=seq_data[7]|data1;
                                                  }
                                           }

                                           else
                                            {
                                                   seq_data[7]=0x08;
                                                   for(i=0;p1[i]!=',';i++)
                                                             data_cmp[i]=p1[i];
                                                             data_cmp[i]='\0';     //NULL;
                                                   p1=p1+i;

                                                  if(out_time==-1)
                                                    x=my_atof(data_cmp);
                                                  else if(out_time!=-1)
                                                    x=out_time;
                                                  time_out=(long)(1000*x); /*//*/
                                                  seq_data[12]=(unsigned char)(time_out>>16);   /*  //p3[0];  */
                                                  seq_data[13]=(unsigned char)(time_out>>8);    /*  //p3[4];  */
                                                  seq_data[14]=(unsigned char)(time_out);       /*  //p3[3];   */
                                                  data1=(p1[1]+1)&0x03;
                                                  seq_data[7]=seq_data[7]|data1;
                                            }

                                           length=19;
                                          }


             else if(memcmp(p1,"status?(",8)==0)
                {
                 p1=p1+8;
                 seq_data[4]=0x00;
                 seq_data[5]=0x03;
                 if(memcmp(p1,"step",4)==0)
                 seq_data[6]=0x44;
                 else if(memcmp(p1,"mem",3)==0)
                 seq_data[6]=0x45;
                 length=7;
                }
             else if(memcmp(p1,"brk",3)==0)
                {
                 seq_data[4]=0x00;
                 seq_data[5]=0x03;
                 seq_data[6]=0x46;
                 length=7;
                }
             else if(memcmp(p1,"exec",4)==0)
                {
                 seq_data[4]=0x00;
                 seq_data[5]=0x03;
                 seq_data[6]=0x47;
                 length=7;
                }
             else if(memcmp(p1,"stop",4)==0)
                {
                 seq_data[4]=0x00;
                 seq_data[5]=0x03;
                 seq_data[6]=0x48;
                 length=7;
                }
             else if(memcmp(p1,"mode",4)==0)
                {
                 seq_data[4]=0x00;
                 seq_data[5]=0x04;
                 seq_data[6]=0x49;
                 seq_data[7]=0x00;
                 length=8;
                }
               transmit(fd,seq_data,length,max_result_length);
          }
        else if(memcmp(p1,"others:",7)==0)
          {
            for(i=0;i<4;i++)
              seq_data[i]=0xa5;
            seq_data[i]=0x00;
            p1=p1+7;
            if(memcmp(p1,"steptime(",9)==0)
              {
                p1=p1+9;
                seq_data[5]=0x06;
                seq_data[6]=0x57;
                /*for(i=0;p1[i]!=')';i++)
                  data_cmp[i]=p1[i];*/
                for(i=0;*p1!=NULL;i++)
                  data_cmp[i]=*p1++;
                data_cmp[i]='\0';       /*          //NULL;     */
                /*   //p1=p1+i;         */
                //x=my_atof(data_cmp);
                x=change_timedata;
                time_out=(long)(1000000*x/50);
                        /*    //p3=&time_out;              */
                seq_data[7]=(unsigned char)(time_out>>16);/*   //p3[0];      */
                seq_data[8]=(unsigned char)(time_out>>8); /*   //p3[4];       */
                seq_data[9]=(unsigned char)(time_out);    /*   //p3[3];     */
                length=10;
              }
            else if(memcmp(p1,"ana:",4)==0)
              {
                  seq_data[5]=0x07;
                  seq_data[6]=0x53;
                  p1=p1+4;
                  if(memcmp(p1,"v(1:1",5)==0)
                    {
                      seq_data[7]=0x00;
                      x=V1RelaPhase;
                    }
                  else if(memcmp(p1,"v(1:2",5)==0)
                    {
                      seq_data[7]=0x01;
                      x=V2RelaPhase;
                    }
                  else if(memcmp(p1,"v(1:3",5)==0)
                    {
                      seq_data[7]=0x02;
                      x=V3RelaPhase;
                    }
                   else if(memcmp(p1,"v(1:4",5)==0)
                   {
                                     seq_data[7]=0x03;
                                     x=VzRelaPhase;
                    }

                  else if(memcmp(p1,"i(1:1",5)==0)
                    {
                      seq_data[7]=0x04;
                      x=I1RelaPhase;
                    }
                  else if(memcmp(p1,"i(1:2",5)==0)
                    {
                      seq_data[7]=0x05;
                      x=I2RelaPhase;
                    }
                  else if(memcmp(p1,"i(1:3",5)==0)
                    {
                      seq_data[7]=0x06;
                      x=I3RelaPhase;
                    }
                  p1=p1+7;
                  if(memcmp(p1,"rel_phase(",10)==0)
                    {
                      y=(int)x;
                      data2=(unsigned char)y;
                      data1=(unsigned char)(y>>8);
                      seq_data[8]=(data1<<6)|(data2>>2);
                      seq_data[9]=data2<<6;
                      frec(x,2,data_cmp1);
                      seq_data[9]=seq_data[9]|(data_cmp1[0]>>2);
                      seq_data[10]=(data_cmp1[0]<<6)|(data_cmp1[1]>>2);

                    }
                  length=11;


              }
            else if(memcmp(p1,"revision:",9)==0)
              {//?μ?ê?￠?à??213￥2?êy07-10-08
                seq_data[5]=0x03;
                seq_data[6]=0x59;
                length=7;
              }
            transmit(fd,seq_data,length,max_result_length);
          }
      p1=p4;
      return;

    }




// amplifier parmeter change
void channel_input(int fd,int type,double value, double phan,double frec,int sin_dc_status1,int channel_status,double value_step,double phase_step,double frec_step)
  {
    int i;
    char buffer[40],buffer1[100],p1[100],p2[100],buffer4[40];
    char p3[100],buffer3[40];

    if(sin_dc_status1==0)
      value=value*1.414;
    if(type==1)//type=1 -->voltage
      {
        if(channel_status==1)
          {
            for(i=0;i<40;i++)
              data_v_a[i]=0x00;
            strcpy(p1,"out:ana:v(1:1):sig1:");
		if (phan!=360)  //2008-12-25
               phan+=VaPhaseRevision;//07-10-08 16:10,?à??D￡×?
          }
        else if(channel_status==2)
          {
            for(i=0;i<40;i++)
              data_v_b[i]=0x00;
            strcpy(p1,"out:ana:v(1:2):sig1:");
            if (phan!=360)  //2008-12-25
               phan+=VbPhaseRevision;//07-10-08 16:10,?à??D￡×?
          }
        else if(channel_status==3)
          {
            for(i=0;i<40;i++)
              data_v_c[i]=0x00;
            strcpy(p1,"out:ana:v(1:3):sig1:");
              if (phan!=360)  //2008-12-25
               phan+=VcPhaseRevision;//07-10-08 16:10,?à??D￡×?
          }
        else if(channel_status==4)
          {
            for(i=0;i<40;i++)
              data_v_z[i]=0x00;
            strcpy(p1,"out:ana:v(1:4):sig1:");
              if (phan!=360)  //2008-12-25
               phan+=VzPhaseRevision;//07-10-08 16:10,?à??D￡×?
          }

      }
    if(type==0)
      {
        if(channel_status==1)
          {
            for(i=0;i<40;i++)
              data_i_a[i]=0x00;
            strcpy(p1,"out:ana:i(1:1):sig1:");
		if (phan!=360)  //2008-12-25
               phan+=IaPhaseRevision;//07-10-08 16:10,?à??D￡×?
          }
        else if(channel_status==2)
          {
            for(i=0;i<40;i++)
              data_i_b[i]=0x00;
            strcpy(p1,"out:ana:i(1:2):sig1:");
		if (phan!=360)  //2008-12-25
               phan+=IbPhaseRevision;//07-10-08 16:10,?à??D￡×?
          }
        else if(channel_status==3)
          {
            for(i=0;i<40;i++)
              data_i_c[i]=0x00;
            strcpy(p1,"out:ana:i(1:3):sig1:");
		 if (phan!=360)  //2008-12-25
               phan+=IcPhaseRevision;//07-10-08 16:10,?à??D￡×?
          }
      }
    if(phan<0)
      phan+=360;//2007-10-08,?à???aêí?a?T·?o?êy,?ùò?òa??-180μ?180×a?ˉ?a0μ?360????.
    for(i=0;p1[i]!=NULL;i++)
      buffer4[i]=p1[i];
    buffer4[i]=NULL;
    buffer1[0]=NULL;
    // p2=buffer2;
    p2[0]=')';
    p2[1]=NULL;
    gcvt(value,6,buffer);

    strcpy(p1,"a(") ;
    for(i=0;p1[i]!=NULL;i++)
      buffer3[i]=p1[i];
    buffer3[i]=NULL;
    strncat1(buffer1,buffer4);
    strncat1(buffer1,buffer3);
    strncat1(buffer1,buffer);
    strncat1(buffer1,p2);

    artExec(fd,buffer1,result,0);

    if(sin_dc_status1==0)
      value_step=value_step*1.414;
    gcvt(value_step,6,buffer);
    buffer1[0]=NULL;
    strncat1(buffer1,buffer4);
    strncat1(buffer1,buffer3);
    strncat1(buffer1,buffer);
    strcpy(p3,",step");
    strcat(buffer1,p3);
    strncat1(buffer1,p2);
    artExec(fd,buffer1,result,0);

    buffer1[0]=NULL;
    gcvt(frec,7,buffer);
    strcpy(p1,"f(");
    for(i=0;p1[i]!=NULL;i++)
      buffer3[i]=p1[i];
    buffer3[i]=NULL;
    buffer1[0]=NULL;
    strncat1(buffer1,buffer4);
    strncat1(buffer1,buffer3);
    strncat1(buffer1,buffer);
    strncat1(buffer1,p2);
    artExec(fd,buffer1,result,0);

    gcvt(frec_step,6,buffer);
    buffer1[0]=NULL;
    strncat1(buffer1,buffer4);
    strncat1(buffer1,buffer3);
    strncat1(buffer1,buffer);
    strcpy(p3,",step");
    strcat(buffer1,p3);
    strncat1(buffer1,p2);
    artExec(fd,buffer1,result,0);

    buffer1[0]=NULL;

    gcvt(phan,7,buffer);
    strcpy(p1,"p(");
    for(i=0;p1[i]!=NULL;i++)
      buffer3[i]=p1[i];

    buffer3[i]=NULL;
    strncat1(buffer1,buffer4);
    strncat1(buffer1,buffer3);
    strncat1(buffer1,buffer);
    strncat1(buffer1,p2);
    artExec(fd,buffer1,result,0);

    gcvt(phase_step,6,buffer);
    buffer1[0]=NULL;
    strncat1(buffer1,buffer4);
    strncat1(buffer1,buffer3);
    strncat1(buffer1,buffer);
    strcpy(p3,",step");
    strcat(buffer1,p3);
    strncat1(buffer1,p2);
    artExec(fd,buffer1,result,0);

    if(sin_dc_status1==0)
      strcpy(p1,"wav(sin)");
    else
      strcpy(p1,"wav(dc)");
    for(i=0;p1[i]!=NULL;i++)
      buffer3[i]=p1[i];
    buffer3[i]=NULL;
    buffer1[0]=NULL;
    strncat1(buffer1,buffer4);
    strncat1(buffer1,buffer3);
    artExec(fd,buffer1,result,0);
    if(type==1)
      {
        if(channel_status==1)
          {
            strcpy(p1,"out:ana:mix:v(1:1,sig1)");
            for(i=0;p1[i]!=NULL;i++)
              buffer1[i]=p1[i];
            buffer1[i]=NULL;
            artExec(fd,buffer1,result,0);
          }
        else if(channel_status==2)
          {
            strcpy(p1,"out:ana:mix:v(1:2,sig1)");
            for(i=0;p1[i]!=NULL;i++)
              buffer1[i]=p1[i];
            buffer1[i]=NULL;
            artExec(fd,buffer1,result,0);
          }
        else if(channel_status==3)
          {
            strcpy(p1,"out:ana:mix:v(1:3,sig1)");
            for(i=0;p1[i]!=NULL;i++)
              buffer1[i]=p1[i];
            buffer1[i]=NULL;
            artExec(fd,buffer1,result,0);
          }
        else if(channel_status==4)
          {
            strcpy(p1,"out:ana:mix:v(1:4,sig1)");
            for(i=0;p1[i]!=NULL;i++)
              buffer1[i]=p1[i];
            buffer1[i]=NULL;
            artExec(fd,buffer1,result,0);
          }

      }
    if(type==0)
      {
        if(channel_status==1)
          {
            strcpy(p1,"out:ana:mix:i(1:1,sig1)");
            for(i=0;p1[i]!=NULL;i++)
              buffer1[i]=p1[i];
            buffer1[i]=NULL;
            artExec(fd,buffer1,result,0);
          }
        else if(channel_status==2)
          {
            strcpy(p1,"out:ana:mix:i(1:2,sig1)");
            for(i=0;p1[i]!=NULL;i++)
              buffer1[i]=p1[i];
            buffer1[i]=NULL;
            artExec(fd,buffer1,result,0);
          }
        else if(channel_status==3)
          {
            strcpy(p1,"out:ana:mix:i(1:3,sig1)");
            for(i=0;p1[i]!=NULL;i++)
              buffer1[i]=p1[i];
            buffer1[i]=NULL;
            artExec(fd,buffer1,result,0);
          }
      }
  }





unsigned long BytesToUnlong( unsigned char bytes[], int NoBytes)
{
        unsigned long value=0;		//, value1=0, value2=0;
        if ( NoBytes <= 0 ||
                 NoBytes > 4 )
                return -1;

        for (int i=0; i<NoBytes; i++)
        {
                value +=(unsigned long)(*(bytes+i)*(pow_my(256,(NoBytes-i-1))));	// =bytes[0]*256*...+bytes[1]*...
        }

        return value;
}



float	BytesToFloat(unsigned char bytes[], int BitsInt, int Bytes, int Signed)
{
        float rtn =(float)BytesToUnlong( bytes, Bytes);
        if (Signed && *bytes > 128)
                rtn -= (float)pow(256, Bytes);
        rtn /= (float)pow(2, (Bytes*8-BitsInt));
        return rtn;
}




void online(int fd)   //online
 {
    int i,k;
       unsigned char  send_str[17]={0xa5,0xa5,0xa5,0xa5,0xff,0x00,0x0c,0x0f};
       unsigned char  receive_str1[4]={0xf0,0xf0,0xf0,0xf0};
       unsigned char  receive_str2[10]={0x0f,0x5a,0xf0,0xa5,0xff,0x0f,0x5a,0xf0,0xa5,0x00};

       while(1)
        {
          receive_send(fd,send_str,0,receive_str1,12,4,1);
          for(i=0;i<8;i++)
             send_str[8+i]=receive_data[4+i];
          send_str[16]=ID;
          k=receive_send(fd,send_str,17,receive_str2,10,10,0);

          if(k==1)
             break;
       }
 }

int read_max(int fd)
 {
    int SIGNED=1;

       memset(result,0,sizeof(result));
       max_output_v_a=0;
        max_output_v_b=0;
         max_output_v_c=0;
         max_output_i_a=0;
         max_output_i_b=0;
         max_output_i_c=0;




       artExec(fd,"amp:def?(1:1)",result,24);//from ART read amplifer_va's parameter.
       response[0]=result[10];
       response[1]=result[11];
       response[2]=result[12];


       max_output_v_a=BytesToFloat(response,10,3,SIGNED);//get va's max absolute output level.



       artExec(fd,"amp:def?(1:2)",result,24); //from ART read amplifer_vb's parameter.
       response[0]=result[10];
       response[1]=result[11];
       response[2]=result[12];


       max_output_v_b=BytesToFloat(response,10,3,SIGNED);//get vb's max absolute output level.

       artExec(fd,"amp:def?(1:3)",result,24); //from ART read amplifer_vc's parameter.
       response[0]=result[10];
       response[1]=result[11];
       response[2]=result[12];

       max_output_v_c=BytesToFloat(response,10,3,SIGNED);//get vc's max absolute output level.



       artExec(fd,"amp:def?(1:4)",result,24); //from ART read amplifer_vc's parameter.
          response[0]=result[10];
          response[1]=result[11];
          response[2]=result[12];
       max_output_v_z=BytesToFloat(response,10,3,SIGNED);//get vb's max absolute output level.





       artExec(fd,"amp:def?(2:1)",result,24);//from ART read amplifer_i's parameter.
       response[0]=result[10];
       response[1]=result[11];
       response[2]=result[12];
       max_output_i_a=BytesToFloat(response,8,3,SIGNED);//get i1's max absolute output level.



       artExec(fd,"amp:def?(2:2)",result,24);//from ART read amplifer_i's parameter.
       response[0]=result[10];
       response[1]=result[11];
       response[2]=result[12];

       max_output_i_b=BytesToFloat(response,8,3,SIGNED);//get i2's max absolute output level.
       artExec(fd,"amp:def?(2:3)",result,24);//from ART read amplifer_i's parameter.
       response[0]=result[10];
       response[1]=result[11];
       response[2]=result[12];

       max_output_i_c=BytesToFloat(response,8,3,SIGNED);//get i3's max absolute output level.




       //07-10-08 16:15???ó?à???μ?êD￡×?￡?
       artExec(fd,"others:revision:get",result,59);//?á?μ?ê?￠?à??213￥2?êy
       response[0]=result[7];
       response[1]=result[8];
       response[2]=result[9];

       FreqRevision=BytesToFloat(response,4,3,SIGNED);//?μ?ê213￥2?êy
       response[0]=result[10];
       response[1]=result[11];
       response[2]=result[12];
       VaPhaseRevision=BytesToFloat(response,4,3,SIGNED);//Va?à??213￥2?êy
       response[0]=result[13];
       response[1]=result[14];
       response[2]=result[15];

       VbPhaseRevision=BytesToFloat(response,4,3,SIGNED);//Vb?à??213￥2?êy
       response[0]=result[16];
       response[1]=result[17];
       response[2]=result[18];
       VcPhaseRevision=BytesToFloat(response,4,3,SIGNED);//Vc?à??213￥2?êy

       response[0]=result[22];
       response[1]=result[23];
       response[2]=result[24];
       IaPhaseRevision=BytesToFloat(response,4,3,SIGNED);//Ia?à??213￥2?êy
       response[0]=result[25];
       response[1]=result[26];
       response[2]=result[27];
       IbPhaseRevision=BytesToFloat(response,4,3,SIGNED);//Ib?à??213￥2?êy
       response[0]=result[28];
       response[1]=result[29];
       response[2]=result[30];
       IcPhaseRevision=BytesToFloat(response,4,3,SIGNED);//Ic?à??213￥2?êy

       return (1);
 }

#endif
