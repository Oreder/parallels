#include "matrix.h"

ceil* init_vector(const int size)
{
    return (ceil *)malloc(size * sizeof(ceil));
}

ceil* init_matrix(const int nrows, const int ncols)
{
    return (ceil *)malloc(nrows * ncols * sizeof(ceil));
}

ceil getVectorValue(ceil *vector, const int i)
{
    return *(vector + i);
}

void setVectorValue(ceil *vector, const int i, ceil value)
{
    *(vector + i) = value;
}

ceil getMatrixValue(ceil *matrix, const int size, const int i, const int j)
{
    return *(matrix + i * size + j);
}

void setMatrixValue(ceil *matrix, const int size, const int i, const int j, ceil value)
{
    *(matrix + i * size + j) = value;
}

ceil* getColumn(ceil *matrix, const int nrows, const int ncols, const int j)
{
    ceil *col = init_vector(nrows);
    if (col)
    {
        for (int k = 0; k < nrows; ++k)
            setVectorValue(col, k, getMatrixValue(matrix, ncols, k, j));
    }
    return col;
}

ceil* getRow(ceil *matrix, const int nrows, const int ncols, const int i)
{
    ceil *row = init_vector(ncols);
    if (row)
    {
        for (int k = 0; k < ncols; ++k)
            setVectorValue(row, k, getMatrixValue(matrix, nrows, i, k));
    }
    return row;
}

ceil* cpy_matrix(ceil *matrix, const int nrows, const int ncols)
{
    ceil *tmp = init_matrix(nrows, ncols);
    if (tmp)
    {
        int i = 0, length = nrows * ncols;
        while (i < length)
        {
            *(tmp + i) = *(matrix + i);
        }
    }
    return tmp;
}

void destroy_vector(ceil *vector)
{
    free(vector);
}

void destroy_matrix(ceil *matrix)
{
    free(matrix);
}

void transpose(ceil *matrix, const int nrows, const int ncols)
{
    for (int i = 0; i < nrows; ++i)
    {
        for (int j = 0; j < ncols; ++j)
        {
            // swap [ij] and [ji]
            if (i != j)
            {
                ceil tmp = getMatrixValue(matrix, ncols, i, j);
                setMatrixValue(matrix, ncols, i, j, getMatrixValue(matrix, ncols, j, i));
                setMatrixValue(matrix, ncols, j, i, tmp);
            }
        }
    }
}

ceil* read_matrix(const char *fileName, int *size)
{
    FILE *fs = fopen(fileName, "r");
    if (fs)
    {
        fscanf(fs, "%d", &(*size));

        if (*size > 0)
        {
            ceil *tmp = init_matrix(*size, *size);
            ceil value;
            int i = 0;
            while (fscanf(fs, "%lf", &value) == 1)
            {
                *(tmp + i) = value;
                i++;
            }
            return tmp;
        }
    }
    return NULL;
}

void write_matrix(const char *fileName, ceil *matrix, const int size)
{
    FILE *fs = fopen(fileName, "w");
    if (fs)
    {
        fprintf(fs, "%d\n", size);
        int length = size * size;
        for (int i = 0; i < length; ++i)
        {
            fprintf(fs, "%.3f", *(matrix + i));
            if (i % size == 0)
                fprintf(fs, "\n");
            else
                fprintf(fs, "\t");            
        }
    }
}

void vdisp(ceil *vector, const int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("\t%.2f", getVectorValue(vector, i));
    }
    printf("\n");
}

void mdisp(ceil *matrix, const int nrows, const int ncols)
{
    for (int i = 0; i < nrows; ++i)
    {
        for (int j = 0; j < ncols; ++j)
            printf("\t%.2f", getMatrixValue(matrix, ncols, i, j));
        printf("\n");
    }
}