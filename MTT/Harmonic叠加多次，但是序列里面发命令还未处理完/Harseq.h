#ifndef HARSEQ_H
#define HARSEQ_H
void ValueCurentFrec(float value,float frec);
void ValueVoltageFrec(float value,float frec);
void GetReviosnValue(int fd);
void output_signal_end(int fd);
void OutputSequencer(int fd);
double gettime(double x,double y,double z);

#endif // HARSEQ_H
