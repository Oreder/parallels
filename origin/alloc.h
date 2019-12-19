#ifndef __ALLOC_H__
#define __ALLOC_H__
#include <stdlib.h>

double **init_matrix(const int);
double **mcpy(const int, double **);
void destroy_matrix(const int, double **);

int *init_array(const int);
void destroy_array(int *);

#endif
