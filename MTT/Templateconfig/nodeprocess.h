#ifndef NODEPROCESS_H
#define NODEPROCESS_H

#include <QString>
struct SECTMENTSETNODE *Nodecreate();
void insertnode(struct SECTMENTSETNODE *head,struct SECTMENTSET value,int id);
void insertnodeIndex(struct SECTMENTSETNODE *head,struct SECTMENTSET value,int id);

void deletenode(struct SECTMENTSETNODE *head,char *name);
void printnode(struct SECTMENTSETNODE *head);
//void  getsettingfromid(struct SETTINGNODELIST *head,char *ID,SETTING &value);//Í¨¹ýIDÕÒµ½setting;
bool getnodevaluefromname(struct SECTMENTSETNODE *head,char  *name,struct SECTMENTSET &set);



#endif // NODEPROCESS_H
