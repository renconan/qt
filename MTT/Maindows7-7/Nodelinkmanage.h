#ifndef NODELINKMANAGE_H
#define NODELINKMANAGE_H

void insertnodelink(struct NODELINK *head,char *value,int id);
void deletenodelink(struct NODELINK *head,char *value);
void printfnodelink(struct NODELINK *head);
bool geditnamefromid(struct NODELINK *head,int id,char *name);

#endif // NODELINKMANAGE_H
