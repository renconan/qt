#ifndef MANSEQ_H
#define MANSEQ_H

void ChangeOutputValue();

void output_signal_manual(int k,int fd);

void output_signal_end(int fd);
void Output_signal_Auto(int fd);
double gettime(double x,double y,double z);
#endif // MANSEQ_H
