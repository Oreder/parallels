#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"

matrix* createMatrix(int rows, int cols) 
{
    matrix* m;
    int j;
    m = (matrix*) malloc(sizeof(matrix));
    m->rows = rows;
    m->cols = cols;
    m->values = (cell**) malloc(cols*sizeof(cell*));
    for (j = 0; j < m->cols; ++j) 
    {
        m->values[j] = (cell*) malloc(rows*sizeof(cell));
    }
    return m;
}

matrix* zeroMatrix(int rows, int cols)
{
    matrix* m;
    int i, j;
    m = createMatrix(rows, cols);
    for(i = 1; i <= m->rows; ++i) 
    {
        for (j = 1; j <= m->cols; ++j)
            setMatValue(m, i, j, 0);
    }
    return m;
}

matrix* oneMatrix(int rows, int cols)
{
    matrix* m;
    int i;
    m = zeroMatrix(rows, cols);
    for(i = 1; i <= m->rows; ++i) 
    {
        setMatValue(m, i, i, 1);
    }
    return m;
}

matrix* sampleMatrix(int rows, int cols)
{
    matrix* m;
    m = (matrix*) malloc(sizeof(matrix));
    m->rows = rows;
    m->cols = cols;
    m->values = 0;
    return m;
}

matrix* copyMatrix(matrix * m)
{
    int i, j;
    matrix* m2 = zeroMatrix(m->rows, m->cols);
    for(i = 1; i <= m->rows; ++i) 
    {
        for (j = 1; j <= m->cols; ++j) 
        {
            setMatValue(m2, i, j, getMatValue(m, i, j));
        }
    }
    return m2;
}

matrix* createMatrixFromArray(const double** a, int rows, int cols)
{
    int i, j;
    matrix* m = createMatrix(rows, cols);
    for(i = 1; i < rows; ++i)
    {
        for(j = 1; j < rows; ++j)
        {
            setMatValue(m, i, j, a[i-1][j-1]);
        }
    }
    return m;
}

double getMatValue(matrix* m, int row, int col)
{
    return m->values[col-1][row-1];
}

void setMatValue(matrix* m, int row, int col, cell value)
{
    m->values[col-1][row-1] = value;
}

matrix* setMatrix(matrix* m, matrix *src)
{
    int i, j;
    for (i = 1; i <= m->rows; ++i)
        for (j = 1; j <= m->cols; ++j)
            setMatValue(m, i, j, getMatValue(src, i, j));
    return m;
}

void displayMatrix(matrix* m)
{
    int i, j;
    cell value;
    for(i = 1; i <= m->rows; ++i)
    {
        for (j = 1; j <= m->cols; ++j)
        {
            value = getMatValue(m, i, j);
            printf("%.2f\t", value);
        }
        printf("\n");
    }
}

void freeMatrix(matrix* m)
{
    int i;
    if (m->values != 0)
    {
        for (i = 0; i < m->cols; ++i)
            free(m->values[i]);
        free(m->values);
    }
    free(m);
}

vector* getRow(matrix* m, int row)
{
    vector* v = createVector(m->rows);
    int i;
    for(i = 1; i <= m->cols; ++i)
    {
        setVecValue(v, i, getMatValue(m, row, i));
    }
    v->id = row;
    return v;
}

vector* getColumn(matrix* m, int column)
{
    vector* v = createVector(m->cols);
    memcpy((void*)v->values, (const void*)m->values[column-1], (size_t)(m->cols*sizeof(cell)));
    v->id = column;
    return v;
}

void setColumn(matrix* m, vector* c, int column)
{
    memcpy((void*)m->values[column-1], (const void*)c->values, (size_t)(c->size*sizeof(cell)));
}

matrix* readMatrixFromFile(const char * fileName)
{
    int rows, cols, i, j;
    cell buffer;
    matrix* m;
    FILE *fs = fopen(fileName, "r");
    fscanf(fs, "%d\t%d\n", &rows, &cols);
    m = createMatrix(rows, cols);
    for(i = 1; i <= m->rows; ++i)
    {
        for (j = 1; j <= m->cols; ++j)
        {
            fscanf(fs, "%lf\t", &buffer);
            setMatValue(m, i, j, buffer);
        }
        fscanf(fs, "\n");
    }
    fclose(fs);
    return m;
}

void writeMatrixToFile(const char * fileName, matrix* m)
{
    int i, j;
    cell value;
    FILE *fs = fopen(fileName, "w+");
    fprintf(fs, "%d\t%d\n", m->rows, m->cols);
    for(i = 1; i <= m->rows; ++i)
    {
        for (j = 1; j <= m->cols; ++j)
        {
            value = getMatValue(m, i, j);
            fprintf(fs, "%.2f\t", value);
        }
        fprintf(fs, "\n");
    }
    fclose(fs);
}
