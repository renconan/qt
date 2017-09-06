#include "Config.h"
CChannel m_Ch[7];
INT_KeyValue  A_Variable;
INT_KeyValue  A_TestMode;
ST_KeyValue  A_CurStep,A_VolStep,A_PhaStep,A_FreStep,A_StepTime,A_StepNum,A_PreFaultTime,A_FaultTime,A_PostFaultTime;
INT_KeyValue  starttest;
ST_KeyValue logic_delaytime,SysFrec,SysVolt;

void initDesc()
{
    for(int i=0;i<7;i++)
    {
        if(i>=0 && i<3)
        {
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
            m_Ch[i].type = 1;
            m_Ch[i].desc = i-4+'A';
        }
    }

    starttest = 0;
}


