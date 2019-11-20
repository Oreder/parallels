#include "io.h"
#include "alloc.h"

int readMatrix(const char *fileName, double ***A)
{
    int size = -1;  // if error

    // read matrix from file
    FILE *fs = fopen(fileName, "r");
    if (!fs) return E_IO;

    rewind(fs);
    fscanf(fs, "%d", &size);

    if (size <= 0) return E_SIZE;

    *A = init_matrix(size);
    if (!A) return E_MEM;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
            fscanf(fs, "%lf", &(*A)[i][j]);
    }

    fclose(fs);

    return size;
}

int writeMatrix(const char *fileName, const int size, double **A)
{
    FILE *fs = fopen(fileName, "w");
    if (!fs) return E_IO;

    fprintf(fs, "%d", size);
    for (int i = 0; i < size; ++i)
    {
        fprintf(fs, "\n");
        for (int j = 0; j < size; ++j)
            fprintf(fs, "%.2f\t", A[i][j]);
    }

    return 0;
}

void display(const int size, double **A)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
            printf("\t%.2f", A[i][j]);
        printf("\n");
    }
}