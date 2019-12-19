#ifndef MATRIX_H
#define	MATRIX_H
#include "vector.h"

typedef struct {
    cell **values;
    int rows;
    int cols;
} matrix;

matrix* createMatrix(int, int);
matrix* zeroMatrix(int, int);
matrix* oneMatrix(int, int);
matrix* sampleMatrix(int, int);
matrix* copyMatrix(matrix *);
matrix* createMatrixFromArray(const double **, int, int);
double  getMatValue(matrix *, int, int);
void    setMatValue(matrix *, int, int, cell);
matrix* setMatrix(matrix *, matrix *);
void    displayMatrix(matrix *);
void    freeMatrix(matrix *);
vector* getColumn(matrix *, int);
void    setColumn(matrix *, vector *, int);
vector* getRow(matrix *, int);
matrix* readMatrixFromFile(const char *);
void    writeMatrixToFile(const char *, matrix *);

#endif

