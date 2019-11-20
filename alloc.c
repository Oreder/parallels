#include "alloc.h"

double **init_matrix(const int size)
{
    double **tmp = (double **)malloc(size * sizeof(double *));
    if (!tmp)
        return NULL;
    
    for (int i = 0; i < size; ++i)
        tmp[i] = (double *)malloc(size * sizeof(double));

    return tmp;
}

double **mcpy(const int size, double **matrix)
{
    double **tmp = init_matrix(size);
    if (tmp)
    {
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
                tmp[i][j] = matrix[i][j];
        }
    }
    return tmp;
}

void destroy_matrix(const int size, double **matrix)
{
    for (int i = 0; i < size; ++i)
        free(matrix[i]);
    free(matrix);
}

int *init_array(const int size)
{
    return (int *)malloc(size * sizeof(size));
}

void destroy_array(int *array)
{
    free(array);
}