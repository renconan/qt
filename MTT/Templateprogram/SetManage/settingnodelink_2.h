#ifndef SETTINGNODELINK_H
#define SETTINGNODELINK_H
#include <QString>
#include "format.h"

void insertsetnode_2(struct SETTINGNODEIDLIST *head,SETTINGID value);
QString getnamefromid_2(struct SETTINGNODEIDLIST *head,char *ID);
QString getvaluefromid_2(struct SETTINGNODEIDLIST *head,char *ID);
void printfsetting_2(struct SETTINGNODEIDLIST *head);
bool getsettingidfromindex_2(struct SETTINGNODEIDLIST *head,int index,SETTINGID &value);
void setsettingvaluefromindex_2(struct SETTINGNODEIDLIST *head,int index,QString value);
#endif // SETTINGNODELINK_H
