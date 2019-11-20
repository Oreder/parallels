#ifndef __IO_H__
#define __IO_H__
#include <stdio.h>
#define E_CMD   -1
#define E_IO    -2
#define E_MEM   -3
#define E_SIZE  -4

int readMatrix(const char *, double ***);
int writeMatrix(const char *, const int, double **);
void display(const int, double **);

#endif