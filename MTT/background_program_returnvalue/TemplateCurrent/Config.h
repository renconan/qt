#ifndef CONFIG_H
#define CONFIG_H

#ifndef ARMLINUX
#define ARMLINUX
#endif

#define LEDNUM GPIO8
#define Logicinfoname "logicinfonmae.txt"
//#define DEBUG

#ifndef DEBUG
#define RUNSTATFILE      "./TemplateDir/runstatetpl.txt"
#define TemplateReport   "./TemplateDir/TemplateReport.txt"
#define SYSSET           "./sysset.txt"
#define TemplateTestState  "./TemplateDir/TemplateTestState.txt"

#else
#define RUNSTATFILE      "./runstatetpl.txt"
#define TemplateReport   "./TemplateReport.txt"
#define SYSSET           "../sysset.txt"
#define  TemplateTestState "./TemplateTestState.txt"
#endif
#endif // CONFIG_H
