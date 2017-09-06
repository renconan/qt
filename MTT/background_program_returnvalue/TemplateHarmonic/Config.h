#ifndef CONFIG_H
#define CONFIG_H

#ifndef ARMLINUX
#define ARMLINUX
#endif

#define LEDNUM GPIO8
#define Logicinfoname "logicinfonmae.txt"

//#define DEBUG

#ifndef DEBUG
#define RUNSTATFILE    "./TemplateDir/runstatetpl.txt"  //模板程序的运行状态 -1 STOP退出 测试完成推出
#define TemplateReport "./TemplateDir/TemplateReport.txt"
#define TRIPFREQ  "./TemplateDir/tripfreq.txt"
#define SYSSET    "./sysset.txt"
#define TemplateTestState  "./TemplateDir/TemplateTestState.txt"
//这里在模板程序单独进行测试时runstatetpl.txt
//在放入模板界面程序时 需要用./TemplateDir/runstatetpl.txt
#else
#define RUNSTATFILE    "./runstatetpl.txt"  //模板程序的运行状态 -1 STOP退出 测试完成推出
#define TemplateReport "./TemplateReport.txt"
#define TRIPFREQ  "./tripfreq.txt"
#define SYSSET    "../sysset.txt"
#define  TemplateTestState "./TemplateTestState.txt"

//这里在模板程序单独进行测试时runstatetpl.txt
//在放入模板界面程序时 需要用./TemplateDir/runstatetpl.txt
#endif

#endif // CONFIG_H
