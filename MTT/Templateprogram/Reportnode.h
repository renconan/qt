#ifndef REPORTNODE_H
#define REPORTNODE_H

struct REPORTLINK *Reportnodecreate();
void insertsectreport(struct REPORTLINK *head,SECTREPORT value,int id);
bool getsectreportfromid(struct REPORTLINK *head,SECTREPORT &value,int id);
struct REPORTLINK *getreportnodefromid(struct REPORTLINK *head,int id);
void deletesectreportnode(struct REPORTLINK *head,int id);
void printreport(struct REPORTLINK *head);

#endif // NODEPROCESS_H
