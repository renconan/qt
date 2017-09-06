#ifndef DFDTSEQ_H
#define DFDTSEQ_H

void   OutputSequencer1(int fd);

void   OutputSequencer3(int fd);
void   OutputSequencer4(int fd);
void   OutputSequencer5(int fd);

void   output_signal_end(int fd);
double gettime(double x,double y,double z);
#endif // DFDTSEQ_H
