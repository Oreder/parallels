#ifndef EQUATIONS_H
#define	EQUATIONS_H

#include "LU.h"

typedef struct
{
    matrix* A;
    vector* x;
    vector* b;
} equations;

void freeEquations(equations *);
void solve(equations *, env);
void inverse(equations *, env, matrix *);

#endif

