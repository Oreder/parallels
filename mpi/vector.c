#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"

vector* createVector(int size) 
{
    vector* v = (vector*)malloc(sizeof(vector));
    v->values = (cell*)calloc(size,sizeof(cell));
    v->size = size;
    return v;
}

vector* zeroVector(int size) 
{
    int i;
    vector* m = createVector(size);
    for(i = 0; i < m->size; ++i) 
        m->values[i] = 0;
    return m;
}

vector* setSpecialVector(vector *v, int spec)
{
    int i;
    for (i = 0; i < v->size; ++i)
        v->values[i] = 0;
    v->values[spec] = 1;
    return v;
}

vector* sampleVector(int rows)
{
    vector* v;
    v = (vector*) malloc(sizeof(vector));
    v->size = rows;
    v->values = 0;
    return v;
}

void displayVector(vector* v)
{
    int i;
    double value;
    for (i = 0; i < v->size; ++i)
    {
        value = v->values[i];
        printf("%lf\t", value);
    }
    printf("\n");
}

vector* createFromArray(cell* values, int size)
{
    int i;
    vector* v = createVector(size);
    for (i = 0; i < size; i++)
        v->values[i] = values[i];
    return v;
}

void freeVector(vector* v)
{
    if (v->values != 0)
        free(v->values);
    free(v);
}

void setVecValue(vector* v, int pos, double value)
{
    v->values[pos-1] = value;
}

double getVecValue(vector* v, int pos)
{
    return v->values[pos-1];
}

vector* readVectorFromFile(const char* fileName)
{
    int rows, i;
    cell buffer;
    vector* v;
    FILE *fs = fopen(fileName, "r");
    fscanf(fs, "%d\n", &rows);
    v = createVector(rows);
    for(i = 1; i <= v->size; ++i)
    {
        fscanf(fs, "%lf\n", &buffer);
        setVecValue(v, i, buffer);
    }
    fclose(fs);
    return v;
}

void writeVectorToFile(const char* fileName, vector* v)
{
    int i;
    cell value;
    FILE *fs = fopen(fileName, "w+");
    fprintf(fs, "%d\n", v->size);
    for(i = 1; i <= v->size; ++i)
    {
        value = getVecValue(v, i);
        fprintf(fs, "%.2f\n", value);
    }
    fclose(fs);
}
