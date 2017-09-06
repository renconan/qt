#ifndef CONFIG_H
#define CONFIG_H

#ifndef ARMLINUX
#define ARMLINUX
#endif
#define  coarse_fine_devision  0.1  //???��??o?a???��???????��??��??oy
#include <QStyledItemDelegate>
#include <QWidget>
#define LEDNUM GPIO8
#define MAXFUNCNUM  6 //���ı��ع��ܲ����ļ�����
#define Templatedir    "./AuthorizeDir"
#define Templatedir1    "./AuthorizeDir1"
#define MAINCONFIGFILE "./mainconfig.cfg"
/***************************
  ģ���ļ�����Ȩ�ļ�·��
  AuthorizeDir
***************************/

#include <QString>
void initDesc();
char* OnOff(int num,bool on);

class CWindowsBase:public QWidget
{
    Q_OBJECT

public:
    explicit CWindowsBase(QString ModelID,QWidget *parent = 0);
    //~CWindowsBase(){}
    QString m_ModelID;
    virtual int StartTestState(){}
    virtual int StopTestState(){}
    virtual void UpDatePara(QMap<QString,float>*){}

signals:
    void SIG_closeFrom();
};


struct ST_KeyValue
{
    QString Key;
    QString Value;
    float GetValue()
    {
        //return atof(Value.c_str());
        return Value.toFloat();
    }

//    ST_KeyValue& operator = (int fvalue)
//    {
//        this->Value = QString::number(fvalue);
//        return *this;
//    }
//    //�������int
//    ST_KeyValue &operator+=(const int &a){
//        int tmp;
//        tmp = this->Value.toInt() + a;
//        (*this)=tmp;
//        return *this;
//    }
//    //�����int
//    ST_KeyValue &operator-=(const int &a){
//        int tmp;
//        tmp = this->Value.toInt() - a;
//        (*this)=tmp;
//        return *this;
//    }


//    operator int() const{ return Value.toInt();}


    ST_KeyValue& operator = (float fvalue)
    {
        this->Value = QString::number(fvalue,'g');
        return *this;
    }
    //�������int
    ST_KeyValue &operator+=(const float &a){
        float tmp;
        tmp = this->Value.toFloat() + a;
        (*this)=tmp;
        return *this;
    }
    //�����int
    ST_KeyValue &operator-=(const float &a){
        float tmp;
        tmp = this->Value.toFloat() - a;
        (*this)=tmp;
        return *this;
    }


    operator float() const{ return Value.toFloat();}

};
struct INT_KeyValue
{
    QString Key;
    QString Value;
    int GetValue()
    {
        //return atof(Value.c_str());
        return Value.toInt();
    }

    INT_KeyValue& operator = (int fvalue)
    {
        this->Value = QString::number(fvalue);
        return *this;
    }
    //�������int
    INT_KeyValue &operator+=(const int &a){
        int tmp;
        tmp = this->Value.toInt() + a;
        (*this)=tmp;
        return *this;
    }
    //�����int
    INT_KeyValue &operator-=(const int &a){
        int tmp;
        tmp = this->Value.toInt() - a;
        (*this)=tmp;
        return *this;
    }


    operator int() const{ return Value.toInt();}
};

class CChannel
{
public:
    CChannel():type(0)
    {
//    value=0.1;
    }

    QString name;
    QString desc;
    int     type;//0--U,1--I,2--DI,3--DO
    int     dib;
    ST_KeyValue  Amp;
    ST_KeyValue  H_Phase;
    ST_KeyValue  Freq;
    INT_KeyValue  checked;
    ST_KeyValue  OutputValue;
    INT_KeyValue  Output;
    INT_KeyValue  DC;
    char* getOnSign(bool on)
    {
        return OnOff(dib,on);
    }


};
extern CChannel m_Ch[7];

extern INT_KeyValue  A_Variable;
extern INT_KeyValue  A_TestMode;
extern ST_KeyValue  A_CurStep,A_VolStep,A_PhaStep,A_FreStep,A_StepTime,A_StepNum,A_PreFaultTime,A_FaultTime,A_PostFaultTime;
extern INT_KeyValue  starttest;
extern ST_KeyValue logic_delaytime,SysFrec,SysVolt;
extern ST_KeyValue  HarmonicCurrentStart,H_Ferequencer1;
extern INT_KeyValue   H_CurrentOutputSel;
extern ST_KeyValue HarmonicStart;
extern ST_KeyValue HarmonicEnd;
extern ST_KeyValue HarmonicStep;
extern ST_KeyValue HarmonicCount,FixHarmonic,H_Current,H_Phase;
extern ST_KeyValue  H_Ferequencer;


#endif // CONFIG_H
