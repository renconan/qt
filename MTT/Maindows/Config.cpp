#include "Config.h"
CChannel m_Ch[7];
CAnytest cAnytest;
AddHarmonicData addHarmonicData;
INT_KeyValue  starttest;
char* OnOff(int num,bool on)
{
    int tmp;
    tmp = 1<<(num);
    QString qstrtmp =QString::number(tmp,16);
    if(qstrtmp.size()<2)
        qstrtmp ="0"+qstrtmp;
    if(on)
    qstrtmp = "out:dig:on(0x"+qstrtmp +")";
    else
        qstrtmp = "out:dig:off(0x"+qstrtmp +")";
    QByteArray ba = qstrtmp.toLocal8Bit();
    //qDebug()<<ba.data();
    return ba.data();
}

void initDesc()
{
    for(int i=0;i!=7;i++)
    {
        m_Ch[i].dib=0;
        if(i>=0 && i<3)
        {
            if(i==0)m_Ch[i].dib=4;
            m_Ch[i].type = 0;
            m_Ch[i].desc = i+'A';
        }
        else if(i==3)
        {
            m_Ch[i].type = 0;
            m_Ch[i].desc = 'Z';
        }
        else
        {
            if(i==4)m_Ch[i].dib=5;
            m_Ch[i].type = 1;
            m_Ch[i].desc = i-4+'A';
        }
    }


}
CWindowsBase::CWindowsBase(QString ModelID,QWidget *parent):
    m_ModelID(ModelID),
    QWidget(parent)
{

}


