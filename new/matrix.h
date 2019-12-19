#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdio.h>
#include <stdlib.h>
typedef double ceil;

ceil* init_vector(const int);
ceil* init_matrix(const int, const int);
ceil  getVectorValue(ceil *, const int);
void  setVectorValue(ceil *, const int, ceil);
ceil  getMatrixValue(ceil *, const int, const int, const int);
void  setMatrixValue(ceil *, const int, const int, const int, ceil);
ceil* getColumn(ceil *, const int, const int, const int);
ceil* getRow(ceil *, const int, const int, const int);
ceil* cpy_matrix(ceil *, const int, const int);
void  destroy_vector(ceil *);
void  destroy_matrix(ceil *);

void transpose(ceil *, const int, const int);

ceil* read_matrix(const char *, int *);
void  write_matrix(const char *, ceil *, const int);
void  vdisp(ceil *, const int);
void  mdisp(ceil *, const int, const int);

#endif