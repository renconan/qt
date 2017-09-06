#ifndef FORMAT_H
#define FORMAT_H
#define SETTINGFILE  "setting.mtp"
#define BUFFSIZE 512
#include <QString>
typedef struct{
    bool isprint; //是否在模板文件中   被打印 1:未被打印 0已经打印
    char name[50];
    char ID[50];
    char value[50];
   // char ref[512];
}SETTING;

typedef struct
{
 bool valid;
 float errorvalue; //误差值
 float errorvalue_Negtive; //误差值
}ASSESSMENT;     //相对误差只有一个变量 绝对误差有两个  都用这个结构表示


typedef struct
{
   ASSESSMENT relerror;
   ASSESSMENT abserror;
   char CompareID[50]; //比较的ID号
   int assessandor; //评估 是与还是或
}ASSESSORROR;      //评估相关的机构体   相对误差还是绝对误差


typedef struct{
    int index;
    char name[50];
    char ID[50];
    char value[50];
  //  char ref[512];
}SETTINGID;



 struct SETTINGNODELIST
 {
     SETTING set;
     struct SETTINGNODELIST *next;
 };  //定值的链表


 struct SETTINGNODEIDLIST  //有加上索引号的链表
 {
     SETTINGID set;
     struct SETTINGNODEIDLIST *next;
 };



 typedef struct{
     char text[100];
     float value;
 }VARIABLE; //如果是整数 则 对浮点数取整


 //电流 或电压 幅值 相位 触发值变量
typedef struct
{
 bool valid; //是否有效
 VARIABLE StartValue; //电流的起始值
 VARIABLE EndValue;   //电流的起始值
 VARIABLE StepValue;   //步长
 VARIABLE HoldTime;   //步长时间
 ASSESSORROR assesstripvalue;
 float  tripvalue;
}TRIPVALUEVARIABLE;

// 电压电流的幅值 相位触发时间
typedef struct
{
    bool valid; //是否有效
    int UseMeasureValue; ////使用测试值
    VARIABLE EndVaule;
    VARIABLE HoldTime; //保持时间
    ASSESSORROR assesstriptime;
    float  triptime;
}TRIPTIMEVARIABLE;


//电流的定值
typedef struct{
   bool valid;
   bool done;  //是否要做
   char name[128];
   VARIABLE Testitem;
   VARIABLE output;
   VARIABLE Faultduraion;
   VARIABLE Vfault;
   VARIABLE Angle;//灵敏角

   TRIPVALUEVARIABLE TripValue;
   TRIPTIMEVARIABLE TripTime;
}CURRENTSET;

 typedef struct{
     bool valid;
     bool done;  //是否要做
     char name[128];
     int  Vpn_Vpp;
     VARIABLE Testitem;
     VARIABLE FaultType;
     VARIABLE Faultduraion;
     TRIPVALUEVARIABLE TripValue;
     TRIPTIMEVARIABLE TripTime;
  }VOLTAGESET;


 typedef struct{
    bool valid;
    bool done;  //是否要做
    char name[128];
    int IAch; //IA是否变化
    int IBch;
    int ICch;
    int VAch;
    int VBch;
    int VCch;
    VARIABLE Testitem;
    VARIABLE Faultduraion;
    VARIABLE FixVoltage;
    VARIABLE FixCurrent;
    TRIPVALUEVARIABLE TripValue;
    TRIPTIMEVARIABLE  TripTime;
 }PHRASESET;





//方向的变量
 typedef struct{
     bool valid;
     VARIABLE Ifault;
     VARIABLE vfault;
     VARIABLE mta;
     VARIABLE faulttype;
     //2015-5-25 加入步长值 和步长时间 更改为线性测试
     VARIABLE step;
     VARIABLE steptime;

     ASSESSORROR assessmta;
     float actangle1;
     float actangle2;
     float phrasemta;
 }TRIPMTAVARIABLE; //灵敏角   测两个边界角


//方向的定值
typedef struct{
     bool valid;
     bool done;  //是否要做
     char name[128];
     TRIPMTAVARIABLE TripMta;
 }DIRECTIONSET;



 //谐波的定值//2016-12-13 新增加harmonic
 typedef struct{
      bool valid;
      bool done;  //是否要做
      char name[128];
      int IAch; //IA是否变化
      int IBch;
      int ICch;
      VARIABLE HarmonicCount;
      VARIABLE Current;
      VARIABLE Phase;
      TRIPVALUEVARIABLE TripValue;
  }HARMONICSET;

//触发频率
typedef struct
 {
     bool valid;
     VARIABLE FreEndValueStart;
     VARIABLE FreEndValueEnd;
     VARIABLE FreStep; //步长
     VARIABLE HoldTime;
     ASSESSORROR assesstripvalue;
     float tripfrec;
 }FRETRIPVALUEVARIABLE;

 //触发时间
 typedef struct
  {
      bool valid;
      VARIABLE FreEndValue; //停止频率
      VARIABLE HoldTime;  //Max输出时间
      ASSESSORROR assesstriptime;
      float triptime;
  }FRETRIPTIMEVARIABLE;

//dfdt的参数
 typedef struct
 {
    bool valid;
    VARIABLE FreEndValue; //停止频率
    VARIABLE DfdtStart;
    VARIABLE DfdtEnd;
    VARIABLE DfdtStep;
    ASSESSORROR assessdfdttrip;
    float tripdfdt;
 }DFDTTRIPVARIABLE;


 typedef struct
  {
     bool valid;
     VARIABLE dfdt; //停止频率
     VARIABLE stopfreq; //触发时间的停止频率
     VARIABLE Duration;  //稳定性测试的持续时间
     ASSESSORROR assessdfdttriptime;
     float tripdfdttime;
  }DFDTTRIPTIMEVARIABLE;






//电压闭锁值得参数
 typedef struct
 {
     bool valid;
     VARIABLE Dfdt;
     VARIABLE FreEndValue; //停止频率
     VARIABLE VStart;
     VARIABLE VEnd;
     VARIABLE VStep;
     VARIABLE HoldTime;
     int VSelect;  //电压选择输出 1 Vpn 2 Vpp
     ASSESSORROR assessvblock;
     float VBlockTrip;
 }VBLOCKVARIABLE;


 //频率的定值
 typedef struct{
     bool valid;
     bool done;  //是否要做
     char name[128];
     VARIABLE TestItem;
     VARIABLE Faultduraion;
     FRETRIPVALUEVARIABLE TripValue;
     FRETRIPTIMEVARIABLE TripTime;
     DFDTTRIPVARIABLE    Tripdfdt;
     DFDTTRIPTIMEVARIABLE Tripdfdttime;
     VBLOCKVARIABLE      TripVBlock;   //电压闭锁值
 }FRECURRENCESET;

 //任意测试的定值
  typedef struct{
      bool valid;
      bool done;  //是否要做
      char name[128];
       VARIABLE VAAP; //VA的幅值
       VARIABLE VAPH; //VA的相位
       VARIABLE VAFR; //VA的频率
       VARIABLE VBAP; //VA的幅值
       VARIABLE VBPH; //VA的相位
       VARIABLE VBFR; //VA的频率
       VARIABLE VCAP; //VA的幅值
       VARIABLE VCPH; //VA的相位
       VARIABLE VCFR; //VA的频率
       VARIABLE VZAP; //VA的幅值
       VARIABLE VZPH; //VA的相位
       VARIABLE VZFR; //VA的频率
       VARIABLE IAAP; //VA的幅值
       VARIABLE IAPH; //VA的相位
       VARIABLE IAFR; //VA的频率
       VARIABLE IBAP; //VA的幅值
       VARIABLE IBPH; //VA的相位
       VARIABLE IBFR; //VA的频率
       VARIABLE ICAP; //VA的幅值
       VARIABLE ICPH; //VA的相位
       VARIABLE ICFR; //VA的频率
       VARIABLE Istep; //电流步长
       VARIABLE Vstep; //电压步长
       VARIABLE Pstep; //相位步长
       VARIABLE Fstep; //频率步长
       VARIABLE Steptime; //步长时间
       VARIABLE Stepnum;  //步数

       VARIABLE Prefault; //故障前时间
       VARIABLE Infault;  //故障时间
       VARIABLE Postfault; //故障后时间
       VARIABLE Faultduraion;
       VARIABLE VariatyVA; //变化相
       VARIABLE VariatyVB;
      VARIABLE VariatyVC;
      VARIABLE VariatyVZ;
      VARIABLE VZ3U0;
      VARIABLE VariatyIA;
      VARIABLE VariatyIB;
      VARIABLE VariatyIC;
      VARIABLE Testmode; //测试模式
      VARIABLE ChangeItem; //变化项目 幅值、相位 频率
     float triptime;
 }ANYTESTSET;








//报告结构体定义
 typedef struct
 {
  bool valid;
  float triptime;
 }ANYTESTREPORT;

 typedef struct
 {
  bool valid;
  int testitem;
  float tripvalue;
  float triptime;
 }CURRENTREPORT; //电流测试 报告

 typedef struct
 {
  bool  valid;
  int   testitem;
  float tripvalue;
  float triptime;
 }VOLTAGEREPORT; //电流测试 报告

 typedef struct
 {
  bool  valid;
  int   testitem;
  float tripvalue;
  float triptime;
 }PHRASEREPORT; //相位测试 报告

 typedef struct
 {
  bool  valid;
  float tripvalue;
 }HARMONICREPORT; //谐波测试 报告2017-1-6


 typedef struct
 {
     bool valid;
    float actangle1;
    float actangle2;
    float phrasemta;
 }DIRECTIONREPORT; //方向测试 报告

 typedef struct
 {
  bool valid;
  int   testitem;
  float tripfrec;
  float triptime; //触发时间
  float dfdttrip; //dfdt
  float dfdttriptime; //dfdttime
  float Vblocktrip; //电压闭锁值
 }FREQUENCYREPORT; //频率测试 报告




//一段的测试结果
typedef struct
 {
   int id;  //段的id
   ANYTESTREPORT    anytest;
   CURRENTREPORT    current;
   VOLTAGEREPORT    voltage;
   DIRECTIONREPORT  direction;
   FREQUENCYREPORT  frec;
   PHRASEREPORT     phrase;
   HARMONICREPORT     harmonic;//2017-1-6
 }SECTREPORT;

 struct REPORTLINK
{
  int id;      //段的ID
  SECTREPORT value; //一段的测试结果
  struct REPORTLINK *next;
};


struct SECTMENTSET
{
VOLTAGESET  volset;
CURRENTSET  curset;
DIRECTIONSET dirset;
FRECURRENCESET freset;
ANYTESTSET     anytestset;
PHRASESET      phraseset; //相位的设置
HARMONICSET      harmonicset; //谐波的设置//2016-12-13 新增加harmonic
char name[128];
};

 struct SECTMENTSETNODE
 {
 int id;
 struct SECTMENTSET value;
 struct SECTMENTSETNODE *next;
};

#endif // FORMAT_H
