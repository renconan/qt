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
float StartValue; //初始电流值
float EndValue;   //终止电流值
float StepValue;  //步长值
float Stemtime;  //步长时间
CurrentTestItem TestItem; //测试项目
int OutputMode;       //输出模式
}CurrentConfig;


typedef struct{
     int TestItem1;
     int TestItem2;
     int TestItem3;
}VoltageTestItem;

typedef struct{

bool  valid;
float StartValue; //初始电流值
float EndValue;   //终止电流值
float StepValue;  //步长值
float Stemtime;  //步长时间
VoltageTestItem TestItem; //测试项目
int OutputMode;       //输出模式
}VoltageConfig;




typedef struct {
CurrentConfig current_config;
VoltageConfig Voltage_config;

}TemplateConfig;








#endif // TEMPLATECONFIG_H
