#ifndef TESTCOM2_H
#define TESTCOM2_H


int receive_send(unsigned char *Send_str,int Send_len,unsigned char *cmp_data,int Receive_len,int cmp_len,int check_f);
int transmit(unsigned char *buffer,int length, int length2);
int chang_amp(double out_value,double *max_output_data);
void frec(double x,int length,unsigned char *buffer);
void frec1( const float f,  int BitsInt, int Bytes,unsigned char *rt);
float	BytesToFloat(unsigned char bytes[], int BitsInt, int Bytes, int Signed);
unsigned long BytesToUnlong( unsigned char bytes[], int NoBytes);
char *strncat1( char *buffer, char *buffer1);
char autoc(char *p);
void channel_input(int fd,int type,double value, double phan,double frec,int sin_dc_status1,int channel_status,double value_step,double phase_step,double frec_step);
void artExec(int fd, char *command_str, unsigned char *result1,int max_result_length);
int read_max(int fd);
int online(int fd);



#endif // TESTCOM2_H
