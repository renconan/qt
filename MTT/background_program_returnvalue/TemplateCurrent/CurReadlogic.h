#ifndef CURREADLOGIC_H
#define CURREADLOGIC_H

void ReadAutoLogic(int fd);
void ReadTripTimeLogic(int fd);
void logic_info(int fd,char info_result1,char info_result2,char info_result3);
int  Buzzer(char Address1,char Address2,char Address3);
void showdspstate(int state);

#endif // CURREADLOGIC_H
