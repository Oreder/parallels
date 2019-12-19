#include "LU.h"
#include <math.h>
#include "alloc.h"

/* INPUT: N - dismension; src - source matrix; 
 *        Tol - small tolerance number to detect failure when the matrix is near degenerate
 * OUTPUT: IA is the inverse of the initial matrix
 * NOTICE:       
 * In this function, matrix A is changed, it contains a copy of both matrices L-E and U as A=(L-E)+U 
 * such that P*A=L*U. The permutation matrix is not stored as a matrix, but in an integer vector P of size (N+1)
 * containing column indexes where the permutation matrix has "1". The last element P[N]=S+N, where S 
 * is the number of row exchanges needed for determinant computation, det(P)=(-1)^S
 * Here, P is array, which is filled in LUP Decomposition.
 */
double **LUPInverse(const int N, double **src, const double Tol)
{
    // Make a copy
    double **A = mcpy(N, src);  
    // if (!A) return NULL;

    // Decomposition
    double maxA, *ptr, absA;

    int *P = init_array(N+1);     // pivot
    // if (!P) return NULL;
    for (int i = 0; i <= N; i++)
        P[i] = i;               // Unit permutation matrix, P[k] initialized with k

    for (int i = 0; i < N; i++) 
    {
        maxA = 0.0;
        int imax = i;

        for (int k = i; k < N; k++)
        {
            if ((absA = fabs(A[k][i])) > maxA) 
            { 
                maxA = absA;
                imax = k;
            }
        }

        if (maxA < Tol) return 0;   // failure, matrix is degenerate

        if (imax != i) 
        {
            // pivoting P
            int j = P[i];
            P[i] = P[imax];
            P[imax] = j;

            // pivoting rows of A
            ptr = A[i];
            A[i] = A[imax];
            A[imax] = ptr;

            // counting pivots starting from N (for determinant)
            P[N]++;
        }

        for (int j = i+1; j < N; j++) 
        {
            A[j][i] /= A[i][i];

            for (int k = i+1; k < N; k++)
                A[j][k] -= A[j][i] * A[i][k];
        }
    }

    // Decomposition done: A = (L - E) + U

    double **IA = init_matrix(N);
    if (!IA) return NULL;

    for (int j = 0; j < N; j++) 
    {
        for (int i = 0; i < N; i++) 
        {
            if (P[i] == j) 
                IA[i][j] = 1.0;
            else
                IA[i][j] = 0.0;

            for (int k = 0; k < i; k++)
                IA[i][j] -= A[i][k] * IA[k][j];
        }

        for (int i = N-1; i >= 0; i--) 
        {
            for (int k = i+1; k < N; k++)
                IA[i][j] -= A[i][k] * IA[k][j];

            IA[i][j] = IA[i][j] / A[i][i];
        }
    }

    destroy_array(P);       // free pivot
    destroy_matrix(N, A);   // free copy

    return IA;
}

/*  INPUT: size - dismension; A, IA - matrix and its invertion
 * OUTPUT: correct inverse matrix or not
 */
int validInverse(const int size, double **A, double **IA, double tol)
{
    int valid = 1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            double sum = 0;
            for (int k = 0; k < size; k++)
                sum +=  A[i][k] * IA[k][j];
            
            valid &= (i == j && fabs(sum - 1) < tol) || (i != j && fabs(sum) < tol);
        }
    }

    return valid;
}