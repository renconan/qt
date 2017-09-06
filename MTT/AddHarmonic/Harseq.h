#ifndef HARSEQ_H
#define HARSEQ_H
void ValueCurentFrec();
void ValueVoltageFrec();
void GetReviosnValue(int fd);
void output_signal_end(int fd);
void OutputSequencer(int fd);
double gettime(double x,double y,double z);

#endif // HARSEQ_H
