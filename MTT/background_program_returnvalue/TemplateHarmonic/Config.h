#ifndef CONFIG_H
#define CONFIG_H

#ifndef ARMLINUX
#define ARMLINUX
#endif

#define LEDNUM GPIO8
#define Logicinfoname "logicinfonmae.txt"

//#define DEBUG

#ifndef DEBUG
#define RUNSTATFILE    "./TemplateDir/runstatetpl.txt"  //ģ����������״̬ -1 STOP�˳� ��������Ƴ�
#define TemplateReport "./TemplateDir/TemplateReport.txt"
#define TRIPFREQ  "./TemplateDir/tripfreq.txt"
#define SYSSET    "./sysset.txt"
#define TemplateTestState  "./TemplateDir/TemplateTestState.txt"
//������ģ����򵥶����в���ʱrunstatetpl.txt
//�ڷ���ģ��������ʱ ��Ҫ��./TemplateDir/runstatetpl.txt
#else
#define RUNSTATFILE    "./runstatetpl.txt"  //ģ����������״̬ -1 STOP�˳� ��������Ƴ�
#define TemplateReport "./TemplateReport.txt"
#define TRIPFREQ  "./tripfreq.txt"
#define SYSSET    "../sysset.txt"
#define  TemplateTestState "./TemplateTestState.txt"

//������ģ����򵥶����в���ʱrunstatetpl.txt
//�ڷ���ģ��������ʱ ��Ҫ��./TemplateDir/runstatetpl.txt
#endif

#endif // CONFIG_H
