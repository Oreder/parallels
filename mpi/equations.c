#include "equations.h"
#include <stdio.h>

double calculateLsum(equations* eq, int i)
{
    int j;
    double sum = 0.0;
    for(j = 1; j < i; ++j) 
    {
        sum += getMatValue(eq->A, i, j) * getVecValue(eq->x, j);
    }
    return sum;
}

double calculateRsum(equations* eq, int i)
{
    int j;
    double sum = 0.0;
    for(j = i + 1; j <= eq->x->size; ++j)
    {
        sum += getMatValue(eq->A, i, j) * getVecValue(eq->x, j);
    }
    return sum;
}

void solve(equations* eq, env e)
{
    if (e.myid == 0)
    {
        int i;
        eq->x = createVector(eq->b->size);
        
        //STEP 1 - calculating y
        setVecValue(eq->x, 1, getVecValue(eq->b, 1));   //first value of Y
        
        for (i = 2; i <= eq->x->size; ++i)              //for next n values of Y
        {
            setVecValue(eq->x, i, getVecValue(eq->b, i) - calculateLsum(eq, i));
        }
        
        //STEP 2
        setVecValue(eq->x, eq->x->size, getVecValue(eq->x, eq->x->size) / getMatValue(eq->A, eq->x->size, eq->x->size)); //last value of X
        for (i = eq->x->size - 1; i > 0; --i)           //every next value of X
        {
            setVecValue(eq->x, i, (getVecValue(eq->x, i) - calculateRsum(eq, i))/getMatValue(eq->A, i, i));
        }
    }
}

void inverse(equations* eq, env e, matrix *im)
{
    eq->A = decompose(eq->A, e);
    
    if (e.myid == 0)
    {
        int i, j;
        eq->x = createVector(eq->b->size);
        im = createMatrix(eq->A->rows, eq->A->cols);

        for (j = 1; j <= eq->A->rows; ++j)
        {
            //STEP 1 - setup b
            eq->b = setSpecialVector(eq->b, j);
            
            //STEP 2 - calculating y
            setVecValue(eq->x, 1, getVecValue(eq->b, 1));   // first value of Y
            
            for (i = 2; i <= eq->x->size; ++i)              // for next n values of Y
            {
                setVecValue(eq->x, i, getVecValue(eq->b, i) - calculateLsum(eq, i));
            }
            
            //STEP 3 final
            setVecValue(eq->x, eq->x->size, getVecValue(eq->x, eq->x->size) / getMatValue(eq->A, eq->x->size, eq->x->size)); // last value of X
            for (i = eq->x->size - 1; i > 0; --i)           // every next value of X
            {
                setVecValue(eq->x, i, (getVecValue(eq->x, i) - calculateRsum(eq, i))/getMatValue(eq->A, i, i));
            }

            setColumn(im, eq->x, j);
        }
    }
}

void freeEquations(equations* eq) 
{
    if (eq->A != 0)
        freeMatrix(eq->A);
    if (eq->x != 0)
        freeVector(eq->x);
    if (eq->b != 0)
        freeVector(eq->b);
}
