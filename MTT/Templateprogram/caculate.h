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


#endif // CACULATE_H
