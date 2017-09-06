#ifndef CACULATEVAR_H
#define CACULATEVAR_H

#include "format.h"

#include "format.h"
#include "caculate.h"

float RelaceExp(char *exp); //Ìæ»» Ê½ÖÐµÄID
void strrpl(char* pDstOut, char* pSrcIn, const char* pSrcRpl, const char* pDstRpl);
void CaculateVariable(struct SETTINGNODELIST *settingnodelist,VARIABLE &exp);

int CheckTextValid(struct SETTINGNODELIST *settinglink,QString str);



void StrReplace(char *pSrcIn,char *pDstOut, char *pStart, const char *pSrcRpl, const char *pDstRpl);


#endif // CACULATEVAR_H
