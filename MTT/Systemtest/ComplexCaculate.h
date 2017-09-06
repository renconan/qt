#ifndef COMPLEXCACULATE_H
#define COMPLEXCACULATE_H
#include "Defpam.h"


COMPLEX ComlexAdd(COMPLEX A,COMPLEX B);
COMPLEX ComlexSub(COMPLEX A,COMPLEX B);
COMPLEX ComlexMux(COMPLEX A,COMPLEX B);
COMPLEX ComlexDiv(COMPLEX A,COMPLEX B);

COMPLEX   VaCaculate(float Kl,float KlAngle,float Ia,float Res,float ResAngle);
COMPLEX   VbCaculate(float Kl,float KlAngle,float Ia,float Res,float ResAngle);
COMPLEX   VcCaculate(float Kl,float KlAngle,float Ia,float Res,float ResAngle);



COMPLEX V1LnCaculate(float Kl,float Ia,float R,float Un);
COMPLEX V2LnCaculate(float Kl,float Ia,float R,float Un);

COMPLEX V1LLCaculate(float Ia,float R,float Un);
COMPLEX V2LLCaculate(float Ia,float R,float Un);

#endif // COMPLEXCACULATE_H
