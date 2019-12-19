#ifndef VECTOR_H
#define	VECTOR_H

typedef double cell;

typedef struct {
    cell* values;
    int size;
    int id;
} vector;

vector* createVector(int);
vector* zeroVector(int);
vector* setSpecialVector(vector *, int);
vector* sampleVector(int);
vector* createFromArray(cell *, int);
void    freeVector(vector *);
void    setVecValue(vector *, int, cell);
double  getVecValue(vector *, int);
void    displayVector(vector *);
vector* readVectorFromFile(const char *);
void    writeVectorToFile(const char *, vector *);

#endif
