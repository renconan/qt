#include "ComplexCaculate.h"
#include "Defpam.h"
#include <math.h>
#include <QDebug>

extern float SystemVolt;
extern float zz,vv;
COMPLEX ComlexAdd(COMPLEX A,COMPLEX B)
{
    COMPLEX sum;
    sum.x=A.x+B.x;
    sum.y=A.y+B.y;
    return sum;
}


COMPLEX ComlexSub(COMPLEX A,COMPLEX B)
{
    COMPLEX sum;
    sum.x=A.x-B.x;
    sum.y=A.y-B.y;
    return sum;
}


COMPLEX ComlexMux(COMPLEX A,COMPLEX B)
{
    COMPLEX sum;
    sum.x=A.x*B.x-A.y*B.y;
    sum.y=A.x*B.y+A.y*B.x;
    return sum;
}

COMPLEX ComlexDiv(COMPLEX A,COMPLEX B)
{
    COMPLEX sum;
    float a=A.x;
    float b=A.y;
    float c=B.x;
    float d=B.y;

    sum.x=(a*c+b*d)/(c*c+d*d);
    sum.y=(b*c-a*d)/(c*c+d*d);
    return sum;
}

//KL为实数 VA VB VC 计算
COMPLEX   VaCaculate(float Kl,float KlAngle,float Ia,float Res,float ResAngle)
{
    COMPLEX B;
    COMPLEX C;
    COMPLEX D;
    COMPLEX IA;
    COMPLEX KL;
    COMPLEX R;
    COMPLEX IaRMux;
    COMPLEX IaRDMux;
    COMPLEX sum;

    KL.x=Kl*cos(KlAngle*3.14159/180);
    KL.y=Kl*sin(KlAngle*3.14159/180);
    R.x=Res*cos(ResAngle*3.14159/180);
    R.y=Res*sin(ResAngle*3.14159/180);

    B.x=3*KL.x+1;
    B.y=3*KL.y;
    C.x=3*KL.x+2;
    C.y=3*KL.y;
    D.x=3*KL.x+3;
    D.y=3*KL.y;

    float AngleC=atan(C.y/C.x)*180/3.1415+180;



    float IAgnle=0-ResAngle-AngleC;

    if(IAgnle>180)
        IAgnle=IAgnle-180;
    else if(IAgnle<-180)
        IAgnle=IAgnle+180;



    IA.x=Ia*cos(IAgnle*3.14159/180);
    IA.y=Ia*sin(IAgnle*3.14159/180);

    IaRMux=ComlexMux(IA,R);
    IaRDMux=ComlexMux(IaRMux,D);
    sum.x=0.333*IaRDMux.x;
    sum.y=0.333*IaRDMux.y;
    return sum;
}

COMPLEX   VbCaculate(float Kl,float KlAngle,float Ia,float Res,float ResAngle)
{

    // Ub = 0.333*R*(B-1)*Ia - 86.604*B/D-j50
    //      Tmp1              Tmp2       Tmp3;
    COMPLEX B;
    COMPLEX C;
    COMPLEX D;
    COMPLEX IA;
    COMPLEX KL;
    COMPLEX R;

    KL.x=Kl*cos(KlAngle*3.14159/180);
    KL.y=Kl*sin(KlAngle*3.14159/180);
    R.x=Res*cos(ResAngle*3.14159/180);
    R.y=Res*sin(ResAngle*3.14159/180);

    B.x=3*KL.x+1;
    B.y=3*KL.y;
    C.x=3*KL.x+2;
    C.y=3*KL.y;
    D.x=3*KL.x+3;
    D.y=3*KL.y;

    float AngleC=atan(C.y/C.x)*180/3.1415+180;
    if(AngleC>180)
        AngleC=AngleC-180;
    else if(AngleC<-180)
        AngleC=AngleC+180;

    float IAgnle=0-ResAngle-AngleC;
    if(IAgnle>180)
        IAgnle=AngleC-180;
    else if(IAgnle<=-180)
        IAgnle=IAgnle+180;

    IA.x=Ia*cos(IAgnle*3.14159/180);
    IA.y=Ia*sin(IAgnle*3.14159/180);


    COMPLEX IaRMux;
    COMPLEX B_1; //B 减去1的复数
    COMPLEX IaRB_1Mux;
    COMPLEX Tmp1;
    B_1.x=B.x-1;
    B_1.y=B.y;
    IaRMux=ComlexMux(IA,R);
    IaRB_1Mux=ComlexMux(IaRMux,B_1);
    Tmp1.x=0.333*IaRB_1Mux.x;
    Tmp1.y=0.333*IaRB_1Mux.y;

    zz=IAgnle;


    COMPLEX BdivD=ComlexDiv(B,D);
    COMPLEX Tmp2;

    Tmp2.x=-1.5*SystemVolt*BdivD.x;
    Tmp2.y=-1.5*SystemVolt*BdivD.y;

    COMPLEX Tmp3;
    Tmp3.x=0;
    Tmp3.y=-0.866*SystemVolt;

    COMPLEX sum=ComlexAdd(Tmp1,Tmp2);
    COMPLEX sum2=ComlexAdd(sum,Tmp3);

    return sum2;
}


COMPLEX   VcCaculate(float Kl,float KlAngle,float Ia,float Res,float ResAngle)
{

    // Ub = 0.333*R*(B-1)*Ia - 86.604*B/D-j50
    //      Tmp1              Tmp2       Tmp3;
    COMPLEX B;
    COMPLEX C;
    COMPLEX D;
    COMPLEX IA;
    COMPLEX KL;
    COMPLEX R;

    KL.x=Kl*cos(KlAngle*3.14159/180);
    KL.y=Kl*sin(KlAngle*3.14159/180);
    R.x=Res*cos(ResAngle*3.14159/180);
    R.y=Res*sin(ResAngle*3.14159/180);

    B.x=3*KL.x+1;
    B.y=3*KL.y;
    C.x=3*KL.x+2;
    C.y=3*KL.y;
    D.x=3*KL.x+3;
    D.y=3*KL.y;

    float AngleC=atan(C.y/C.x)*180/3.1415+180;
    if(AngleC>180)
        AngleC=AngleC-180;
    else if(AngleC<-180)
        AngleC=AngleC+180;

    float IAgnle=0-ResAngle-AngleC;

    if(IAgnle>180)
        IAgnle=AngleC-180;
    else if(IAgnle<-180)
        IAgnle=IAgnle+180;

    IA.x=Ia*cos(IAgnle*3.14159/180);
    IA.y=Ia*sin(IAgnle*3.14159/180);


    COMPLEX IaRMux;
    COMPLEX B_1; //B 减去1的复数
    COMPLEX IaRB_1Mux;
    COMPLEX Tmp1;
    B_1.x=B.x-1;
    B_1.y=B.y;
    IaRMux=ComlexMux(IA,R);
    IaRB_1Mux=ComlexMux(IaRMux,B_1);
    Tmp1.x=0.333*IaRB_1Mux.x;
    Tmp1.y=0.333*IaRB_1Mux.y;


    COMPLEX BdivD=ComlexDiv(B,D);
    COMPLEX Tmp2;

    Tmp2.x=-1.5*SystemVolt*BdivD.x;
    Tmp2.y=-1.5*SystemVolt*BdivD.y;

    COMPLEX Tmp3;
    Tmp3.x=0;
    Tmp3.y=SystemVolt*0.866;

    COMPLEX sum=ComlexAdd(Tmp1,Tmp2);
    COMPLEX sum2=ComlexAdd(sum,Tmp3);

    return sum2;
}



COMPLEX V1LnCaculate(float Kl,float Ia,float R,float Un)
{
    float Varial_B=3*Kl+1;
    float Varial_C=3*Kl+2;
    float Varial_D=3*Kl+3;
    COMPLEX tmp;
    float a=R*(Varial_B-1)*Ia/3;
    float b=(1.5*Un*Varial_B)/Varial_D;
    tmp.x=a-b;
    tmp.y=-0.866*Un;
    return tmp;
}


COMPLEX V2LnCaculate(float Kl,float Ia,float R,float Un)
{
    float Varial_B=3*Kl+1;
    float Varial_C=3*Kl+2;
    float Varial_D=3*Kl+3;

    COMPLEX tmp;
    float a=R*(Varial_B-1)*Ia/3;
    float b=(1.5*Un*Varial_B)/Varial_D;
    tmp.x=a-b;
    tmp.y=0.866*Un;
    return tmp;
}


COMPLEX V1LLCaculate(float Ia,float R,float Un)
{
  COMPLEX tmp;
  tmp.x=-Un/2;
  tmp.y=-R*Ia;
  return tmp;
}



COMPLEX V2LLCaculate(float Ia,float R,float Un)
{
    COMPLEX tmp;
    tmp.x=-Un/2;
    tmp.y=R*Ia;
    return tmp;
}










