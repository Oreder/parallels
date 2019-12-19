#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "equations.h"

int main(int argc, char** argv) 
{
    clock_t endTime, beginTime = clock();
    int rows = 123654, cols;
    equations eq;
    // matrix *A;
    env e = init(argc, argv);
    if (e.myid == 0) 
    {
        if (argc < 3)
        {
            printf("Error: Command line <lab> <input> <output>\n");
            return (EXIT_FAILURE);
        }

        eq.A = readMatrixFromFile(argv[1]);
        //eq.b = readVectorFromFile(argv[2]);
        eq.b = zeroVector(eq.A->cols);
        eq.b->values[0] = 1;
        eq.x = 0;

        rows = eq.A->rows;
        cols = eq.A->cols;
        sendDimensions(&eq.A->rows, &eq.A->cols, e);

        // A = readMatrixFromFile(argv[1]);
        // rows = A->rows;
        // cols = A->cols;
        // sendDimensions(&A->rows, &A->cols, e);
    }
    else
    {
        receiveDimensions(&rows, &cols);
        eq.A = sampleMatrix(rows, cols);
        eq.b = sampleVector(rows);
        eq.x = 0;
        // A = sampleMatrix(rows, cols);
    }

    eq.A = decompose(eq.A, e);

    if (e.myid == 0)
    {
        // writeMatrixToFile("bin/LU.txt", eq.A);
        // writeVectorToFile(argv[3], eq.x);
        matrix *iA = createMatrix(eq.A->rows, eq.A->cols);
        
        int i, j;
        for (i = 0; i < eq.A->rows; ++i)
        {
            for (j = 0; j < eq.b->size; ++j)
                eq.b->values[j] = 0;
            eq.b->values[i] = 1;

            solve(&eq, e);
            //setColumn(iA, eq.x, i);
            // for (j = 0; j < eq.b->size; ++j)
            //     setMatValue(iA, j, i, eq.x->values[j]);
            displayVector(eq.x);
        }
        
        displayMatrix(iA);
        //writeMatrixToFile(argv[2], iA);
        freeMatrix(iA);
    }

    freeEquations(&eq);
    endTime = clock();
    if (e.myid == 0)
    {
        printf("Size of matrix: %d x %d\n", rows, cols);
        printf("Number processes: %d\n", e.numprocs);
        printf("Total time: %lf\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
    }

    final();
    return (EXIT_SUCCESS);
}

/**
 LU:
 3.000000	4.000000	7.000000	8.000000	1.000000	6.000000
 0.666667	2.333333	3.333333	-4.333333	4.333333	5.000000
 1.000000	0.857143	-0.857143	2.714286	3.285714	-1.285714
 1.666667	-1.142857	5.666667	-24.666667	-10.333333	11.000000
 1.000000	2.142857	15.333333	1.310811		-42.121622	-7.418919
 1.333333	-1.428571	-4.000000	0.081081		-0.613410	-6.442733
 */
