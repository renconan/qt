#ifndef CONFIG_H
#define CONFIG_H

#ifndef ARMLINUX
#define ARMLINUX
#endif
#define  coarse_fine_devision  0.1  //???ì??o?a???ì???????ì??à??oy
#include <QStyledItemDelegate>
#include <QWidget>
#define LEDNUM GPIO8
#define MAXFUNCNUM  6 //最大的本地功能测试文件个数
#define Templatedir    "./AuthorizeDir"
#define Templatedir1    "./AuthorizeDir1"
#define MAINCONFIGFILE "./mainconfig.cfg"
/***************************
  模板文件及授权文件路径
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
//    //对象加上int
//    ST_KeyValue &operator+=(const int &a){
//        int tmp;
//        tmp = this->Value.toInt() + a;
//        (*this)=tmp;
//        return *this;
//    }
//    //对象减int
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
    //对象加上int
    ST_KeyValue &operator+=(const float &a){
        float tmp;
        tmp = this->Value.toFloat() + a;
        (*this)=tmp;
        return *this;
    }
    //对象减int
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
    //对象加上int
    INT_KeyValue &operator+=(const int &a){
        int tmp;
        tmp = this->Value.toInt() + a;
        (*this)=tmp;
        return *this;
    }
    //对象减int
    INT_KeyValue &operator-=(const int &a){
        int tmp;
        tmp = this->Value.toInt() - a;
        (*this)=tmp;
        return *this;
    }


    operator int() const{ return Value.toInt();}
};

class HWave
{
public:
    HWave(INT_KeyValue number):Hnumber(number)
    {
        Describ="第"+QString::number(number)+"次谐波";
        HValue=0;
        HPhase=0;
        HPersent=0;
    }

    QString        Describ;
    INT_KeyValue   Hnumber;
    ST_KeyValue    HValue;
    ST_KeyValue    HPhase;
    ST_KeyValue    HPersent;

};

class CChannel
{
public:
    CChannel():type(0)
    {
//    value=0.1;
    }
    ~CChannel()
    {
        QMap<int,HWave*>::iterator iter = m_MapWave.begin();
        while(iter!=m_MapWave.end())
        {
            delete iter.value();
            iter++;
        }
        m_MapWave.clear();

    }

    QString name;
    QString desc;
    int     type;//0--U,1--I,2--DI,3--DO
    int     dib;
    ST_KeyValue  Amp;
    ST_KeyValue  Phase;
    ST_KeyValue  Freq;
    INT_KeyValue  checked;
    ST_KeyValue  OutputValue;
    INT_KeyValue  Output;
    INT_KeyValue  DC;
    QMap<int,HWave*> m_MapWave;//谐波
    char* getOnSign(bool on)
    {
        return OnOff(dib,on);
    }


};
extern CChannel m_Ch[7];

class AddHarmonicData
{
public:
    ST_KeyValue   HarmonicCurrentCount;
    ST_KeyValue   HarmonicCurrent;
    ST_KeyValue   HarmonicCurrentPercent;
    ST_KeyValue   HarmonicCurrentPhase;
    ST_KeyValue   HarmonicVoltageCount;
    ST_KeyValue   HarmonicVoltage;
    ST_KeyValue   HarmonicVoltagePercent;
    ST_KeyValue   HarmonicVoltagePhase;

};
extern AddHarmonicData addHarmonicData;

class CAnytest
{
public:
    INT_KeyValue  Variable;
    INT_KeyValue  TestMode;
    ST_KeyValue   CurStep;
    ST_KeyValue   VolStep;
    ST_KeyValue   PhaStep;
    ST_KeyValue   FreStep;
    ST_KeyValue   StepTime;
    ST_KeyValue   StepNum;
    ST_KeyValue   PreFaultTime;
    ST_KeyValue   FaultTime;
    ST_KeyValue   PostFaultTime;

    ST_KeyValue   logic_delaytime;
    ST_KeyValue   SysFrec;
    ST_KeyValue   SysVolt;

};
extern CAnytest cAnytest;
extern INT_KeyValue  starttest;


#endif // CONFIG_H
