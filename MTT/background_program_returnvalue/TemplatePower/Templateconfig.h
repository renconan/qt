#ifndef TEMPLATECONFIG_H
#define TEMPLATECONFIG_H

#define TEMPLATEFILE "./Template.conf"

typedef struct{
     int TestItem1;
     int TestItem2;
     int TestItem3;
}CurrentTestItem;

typedef struct{
bool valid;
float StartValue; //��ʼ����ֵ
float EndValue;   //��ֹ����ֵ
float StepValue;  //����ֵ
float Stemtime;  //����ʱ��
CurrentTestItem TestItem; //������Ŀ
int OutputMode;       //���ģʽ
}CurrentConfig;


typedef struct{
     int TestItem1;
     int TestItem2;
     int TestItem3;
}VoltageTestItem;

typedef struct{

bool  valid;
float StartValue; //��ʼ����ֵ
float EndValue;   //��ֹ����ֵ
float StepValue;  //����ֵ
float Stemtime;  //����ʱ��
VoltageTestItem TestItem; //������Ŀ
int OutputMode;       //���ģʽ
}VoltageConfig;




typedef struct {
CurrentConfig current_config;
VoltageConfig Voltage_config;

}TemplateConfig;








#endif // TEMPLATECONFIG_H
