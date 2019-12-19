#ifndef LU_H
#define	LU_H

#include "matrix.h"

typedef struct {
    matrix* m;
    int pid;
} result;

typedef struct {
    int myid;
    int numprocs;
} env;

env  init(int, char**);
void sendDimensions(int *, int *, env);
void receiveDimensions(int *, int *);

matrix* decompose(matrix *, env);
matrix* decomposeNormal(matrix *, env);
void final();

#endif
