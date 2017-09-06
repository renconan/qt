#ifndef CONFIG_H
#define CONFIG_H

#ifndef ARMLINUX
#define ARMLINUX
#endif
#define  coarse_fine_devision  0.1  //???ì??o?a???ì???????ì??à??oy
#define ARMTEST 1

#include <QString>
void initDesc();
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
    ST_KeyValue  Amp;
    ST_KeyValue  Phase;
    ST_KeyValue  Freq;
    INT_KeyValue  checked;
    ST_KeyValue  OutputValue;//
    INT_KeyValue  Output;//on off 控制位
    INT_KeyValue  DC;

};
extern CChannel m_Ch[7];

extern INT_KeyValue  A_Variable;
extern INT_KeyValue  A_TestMode;
extern ST_KeyValue  A_CurStep,A_VolStep,A_PhaStep,A_FreStep,A_StepTime,A_StepNum,A_PreFaultTime,A_FaultTime,A_PostFaultTime;
extern INT_KeyValue  starttest;
extern ST_KeyValue logic_delaytime,SysFrec,SysVolt;


#endif // CONFIG_H
