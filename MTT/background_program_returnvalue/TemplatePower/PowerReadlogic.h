#ifndef POWERREADLOGIC_H
#define POWERREADLOGIC_H
void outputchar();
void ReadTripTimeLogic(int fd);
void logic_info(int fd,char info_result1,char info_result2,char info_result3);

float CheckAngle(QVector<ACTSTATE>  vect,int type);
#endif // POWERREADLOGIC_H
