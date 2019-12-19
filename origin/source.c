/*
 * Matrix inverse by method advanced LU decomposition (PA = LU)
 */

#include "io.h"
#include "LU.h"
#include "alloc.h"
#define Tolerance 1e-6

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Error: Command line <*.exe> <input-file> <output-file>\n");
        return E_CMD;
    }

    // read matrix from file
    double **src = NULL;
    int size = readMatrix(argv[1], &src);
    
    switch (size)
    {
    case E_IO:
        printf("Error: File can not access\n");
        break;
    case E_MEM:
        printf("Error: Not enough memory\n");
        break;
    case E_SIZE:
        printf("Error: Invalid size of matrix\n");
        break;
    default:;   // Quick way to fix error: a label can only be part of a statement and a declaration is not a statement
        // display(size, src);
        double **IA = LUPInverse(size, src, Tolerance);

        if (IA)
        {
            // display(size, IA);

            if (validInverse(size, src, IA, Tolerance))
                writeMatrix(argv[2], size, IA);
            
            destroy_matrix(size, IA);
        }

        destroy_matrix(size, src);
    }
    
    return (size > 0);
}