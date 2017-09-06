#ifndef FORMAT_H
#define FORMAT_H
#define SETTINGFILE  "setting.mtp"
#define BUFFSIZE 512
#include <QString>
typedef struct{
    bool isprint; //�Ƿ���ģ���ļ���   ����ӡ 1:δ����ӡ 0�Ѿ���ӡ
    char name[50];
    char ID[50];
    char value[50];
   // char ref[512];
}SETTING;

typedef struct
{
 bool valid;
 float errorvalue; //���ֵ
 float errorvalue_Negtive; //���ֵ
}ASSESSMENT;     //������ֻ��һ������ �������������  ��������ṹ��ʾ


typedef struct
{
   ASSESSMENT relerror;
   ASSESSMENT abserror;
   char CompareID[50]; //�Ƚϵ�ID��
   int assessandor; //���� ���뻹�ǻ�
}ASSESSORROR;      //������صĻ�����   ������Ǿ������


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
 };  //��ֵ������


 struct SETTINGNODEIDLIST  //�м��������ŵ�����
 {
     SETTINGID set;
     struct SETTINGNODEIDLIST *next;
 };



 typedef struct{
     char text[100];
     float value;
 }VARIABLE; //��������� �� �Ը�����ȡ��


 //���� ���ѹ ��ֵ ��λ ����ֵ����
typedef struct
{
 bool valid; //�Ƿ���Ч
 VARIABLE StartValue; //��������ʼֵ
 VARIABLE EndValue;   //��������ʼֵ
 VARIABLE StepValue;   //����
 VARIABLE HoldTime;   //����ʱ��
 ASSESSORROR assesstripvalue;
 float  tripvalue;
}TRIPVALUEVARIABLE;

// ��ѹ�����ķ�ֵ ��λ����ʱ��
typedef struct
{
    bool valid; //�Ƿ���Ч
    int UseMeasureValue; ////ʹ�ò���ֵ
    VARIABLE EndVaule;
    VARIABLE HoldTime; //����ʱ��
    ASSESSORROR assesstriptime;
    float  triptime;
}TRIPTIMEVARIABLE;


//�����Ķ�ֵ
typedef struct{
   bool valid;
   bool done;  //�Ƿ�Ҫ��
   char name[128];
   VARIABLE Testitem;
   VARIABLE output;
   VARIABLE Faultduraion;
   VARIABLE Vfault;
   VARIABLE Angle;//������

   TRIPVALUEVARIABLE TripValue;
   TRIPTIMEVARIABLE TripTime;
}CURRENTSET;

 typedef struct{
     bool valid;
     bool done;  //�Ƿ�Ҫ��
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
    bool done;  //�Ƿ�Ҫ��
    char name[128];
    int IAch; //IA�Ƿ�仯
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





//����ı���
 typedef struct{
     bool valid;
     VARIABLE Ifault;
     VARIABLE vfault;
     VARIABLE mta;
     VARIABLE faulttype;
     //2015-5-25 ���벽��ֵ �Ͳ���ʱ�� ����Ϊ���Բ���
     VARIABLE step;
     VARIABLE steptime;

     ASSESSORROR assessmta;
     float actangle1;
     float actangle2;
     float phrasemta;
 }TRIPMTAVARIABLE; //������   �������߽��


//����Ķ�ֵ
typedef struct{
     bool valid;
     bool done;  //�Ƿ�Ҫ��
     char name[128];
     TRIPMTAVARIABLE TripMta;
 }DIRECTIONSET;



 //г���Ķ�ֵ//2016-12-13 ������harmonic
 typedef struct{
      bool valid;
      bool done;  //�Ƿ�Ҫ��
      char name[128];
      int IAch; //IA�Ƿ�仯
      int IBch;
      int ICch;
      VARIABLE HarmonicCount;
      VARIABLE Current;
      VARIABLE Phase;
      TRIPVALUEVARIABLE TripValue;
  }HARMONICSET;

//����Ƶ��
typedef struct
 {
     bool valid;
     VARIABLE FreEndValueStart;
     VARIABLE FreEndValueEnd;
     VARIABLE FreStep; //����
     VARIABLE HoldTime;
     ASSESSORROR assesstripvalue;
     float tripfrec;
 }FRETRIPVALUEVARIABLE;

 //����ʱ��
 typedef struct
  {
      bool valid;
      VARIABLE FreEndValue; //ֹͣƵ��
      VARIABLE HoldTime;  //Max���ʱ��
      ASSESSORROR assesstriptime;
      float triptime;
  }FRETRIPTIMEVARIABLE;

//dfdt�Ĳ���
 typedef struct
 {
    bool valid;
    VARIABLE FreEndValue; //ֹͣƵ��
    VARIABLE DfdtStart;
    VARIABLE DfdtEnd;
    VARIABLE DfdtStep;
    ASSESSORROR assessdfdttrip;
    float tripdfdt;
 }DFDTTRIPVARIABLE;


 typedef struct
  {
     bool valid;
     VARIABLE dfdt; //ֹͣƵ��
     VARIABLE stopfreq; //����ʱ���ֹͣƵ��
     VARIABLE Duration;  //�ȶ��Բ��Եĳ���ʱ��
     ASSESSORROR assessdfdttriptime;
     float tripdfdttime;
  }DFDTTRIPTIMEVARIABLE;






//��ѹ����ֵ�ò���
 typedef struct
 {
     bool valid;
     VARIABLE Dfdt;
     VARIABLE FreEndValue; //ֹͣƵ��
     VARIABLE VStart;
     VARIABLE VEnd;
     VARIABLE VStep;
     VARIABLE HoldTime;
     int VSelect;  //��ѹѡ����� 1 Vpn 2 Vpp
     ASSESSORROR assessvblock;
     float VBlockTrip;
 }VBLOCKVARIABLE;


 //Ƶ�ʵĶ�ֵ
 typedef struct{
     bool valid;
     bool done;  //�Ƿ�Ҫ��
     char name[128];
     VARIABLE TestItem;
     VARIABLE Faultduraion;
     FRETRIPVALUEVARIABLE TripValue;
     FRETRIPTIMEVARIABLE TripTime;
     DFDTTRIPVARIABLE    Tripdfdt;
     DFDTTRIPTIMEVARIABLE Tripdfdttime;
     VBLOCKVARIABLE      TripVBlock;   //��ѹ����ֵ
 }FRECURRENCESET;

 //������ԵĶ�ֵ
  typedef struct{
      bool valid;
      bool done;  //�Ƿ�Ҫ��
      char name[128];
       VARIABLE VAAP; //VA�ķ�ֵ
       VARIABLE VAPH; //VA����λ
       VARIABLE VAFR; //VA��Ƶ��
       VARIABLE VBAP; //VA�ķ�ֵ
       VARIABLE VBPH; //VA����λ
       VARIABLE VBFR; //VA��Ƶ��
       VARIABLE VCAP; //VA�ķ�ֵ
       VARIABLE VCPH; //VA����λ
       VARIABLE VCFR; //VA��Ƶ��
       VARIABLE VZAP; //VA�ķ�ֵ
       VARIABLE VZPH; //VA����λ
       VARIABLE VZFR; //VA��Ƶ��
       VARIABLE IAAP; //VA�ķ�ֵ
       VARIABLE IAPH; //VA����λ
       VARIABLE IAFR; //VA��Ƶ��
       VARIABLE IBAP; //VA�ķ�ֵ
       VARIABLE IBPH; //VA����λ
       VARIABLE IBFR; //VA��Ƶ��
       VARIABLE ICAP; //VA�ķ�ֵ
       VARIABLE ICPH; //VA����λ
       VARIABLE ICFR; //VA��Ƶ��
       VARIABLE Istep; //��������
       VARIABLE Vstep; //��ѹ����
       VARIABLE Pstep; //��λ����
       VARIABLE Fstep; //Ƶ�ʲ���
       VARIABLE Steptime; //����ʱ��
       VARIABLE Stepnum;  //����

       VARIABLE Prefault; //����ǰʱ��
       VARIABLE Infault;  //����ʱ��
       VARIABLE Postfault; //���Ϻ�ʱ��
       VARIABLE Faultduraion;
       VARIABLE VariatyVA; //�仯��
       VARIABLE VariatyVB;
      VARIABLE VariatyVC;
      VARIABLE VariatyVZ;
      VARIABLE VZ3U0;
      VARIABLE VariatyIA;
      VARIABLE VariatyIB;
      VARIABLE VariatyIC;
      VARIABLE Testmode; //����ģʽ
      VARIABLE ChangeItem; //�仯��Ŀ ��ֵ����λ Ƶ��
     float triptime;
 }ANYTESTSET;








//����ṹ�嶨��
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
 }CURRENTREPORT; //�������� ����

 typedef struct
 {
  bool  valid;
  int   testitem;
  float tripvalue;
  float triptime;
 }VOLTAGEREPORT; //�������� ����

 typedef struct
 {
  bool  valid;
  int   testitem;
  float tripvalue;
  float triptime;
 }PHRASEREPORT; //��λ���� ����

 typedef struct
 {
  bool  valid;
  float tripvalue;
 }HARMONICREPORT; //г������ ����2017-1-6


 typedef struct
 {
     bool valid;
    float actangle1;
    float actangle2;
    float phrasemta;
 }DIRECTIONREPORT; //������� ����

 typedef struct
 {
  bool valid;
  int   testitem;
  float tripfrec;
  float triptime; //����ʱ��
  float dfdttrip; //dfdt
  float dfdttriptime; //dfdttime
  float Vblocktrip; //��ѹ����ֵ
 }FREQUENCYREPORT; //Ƶ�ʲ��� ����




//һ�εĲ��Խ��
typedef struct
 {
   int id;  //�ε�id
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
  int id;      //�ε�ID
  SECTREPORT value; //һ�εĲ��Խ��
  struct REPORTLINK *next;
};


struct SECTMENTSET
{
VOLTAGESET  volset;
CURRENTSET  curset;
DIRECTIONSET dirset;
FRECURRENCESET freset;
ANYTESTSET     anytestset;
PHRASESET      phraseset; //��λ������
HARMONICSET      harmonicset; //г��������//2016-12-13 ������harmonic
char name[128];
};

 struct SECTMENTSETNODE
 {
 int id;
 struct SECTMENTSET value;
 struct SECTMENTSETNODE *next;
};

#endif // FORMAT_H
