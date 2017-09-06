#ifndef SETTINGNODELINK_H
#define SETTINGNODELINK_H
#include <QString>
#include "format.h"

void insertsetnode(struct SETTINGNODELIST *head,SETTING value);
QString getnamefromid(struct SETTINGNODELIST *head,char *ID);
QString getvaluefromid(struct SETTINGNODELIST *head,char *ID);
void printfsetting(struct SETTINGNODELIST *head);
void setsettinglinknoprint(struct SETTINGNODELIST *head); //…Ë÷√Œ¥±ª¥Ú”°
void  getsettingfromid(struct SETTINGNODELIST *head,char *ID,SETTING &value);
#endif // SETTINGNODELINK_H
