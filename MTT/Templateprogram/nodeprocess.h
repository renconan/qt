#ifndef NODEPROCESS_H
#define NODEPROCESS_H

#include <QString>
struct SECTMENTSETNODE *Nodecreate();
void insertnode(struct SECTMENTSETNODE *head,struct SECTMENTSET value,int id);
void deletenode(struct SECTMENTSETNODE *head,char *name);
void printnode(struct SECTMENTSETNODE *head);
bool getsectfromid(struct SECTMENTSETNODE *head,struct SECTMENTSET &value,int id);
bool getnodevaluefromname(struct SECTMENTSETNODE *head,char  *name,struct SECTMENTSET &set);
struct SECTMENTSETNODE *getsectnodefromid(struct SECTMENTSETNODE *head,int id);
void setsectfromid(struct SECTMENTSETNODE *head,int id,struct SECTMENTSET value);

void printsectment(struct SECTMENTSET *sectment);
void copysectmentlink(struct SECTMENTSETNODE *dest,struct SECTMENTSETNODE *src);

void setdone(struct SECTMENTSETNODE *head);


#endif // NODEPROCESS_H
