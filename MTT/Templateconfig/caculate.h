#ifndef CACULATE_H
#define CACULATE_H
#include <QStack>
#include <QStack>
#include <stdlib.h>
#include <stdio.h>
#include <QChar>

void init();
int  getLevel(const QChar &oper);
void toPostfix();

float evaluation();
float CaculatefromExp(QString exp);
int CheckTextValid(struct SETTINGNODELIST *settingnodelist,QString str);
void StrReplace( char *pSrcIn,char *pDstOut, char *pStart, const char *pSrcRpl, const char *pDstRpl);
float RelaceExp(char *exp,struct SETTINGNODELIST *settingnodelist) ;
#endif // CACULATE_H
