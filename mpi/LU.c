#include "LU.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <mpi.h>

#define KCOLUMN 1
#define COL2UPDATE 2

#define SEND_COLUMN 0
#define RECEIVE_COLUMN 1

void updateColumn(vector* c, vector* l, int k) 
{
    int i;
    for (i = k+1; i <= c->size; i++)
    {
        setVecValue(c, i, getVecValue(c, i) - getVecValue(l, i) * getVecValue(c, k));
    }
}

void sendColumn(vector* column, int procid)
{
    MPI_Send(&column->size, 1, MPI_INT, procid, 1, MPI_COMM_WORLD);
    MPI_Send(column->values, column->size, MPI_DOUBLE, procid, 2, MPI_COMM_WORLD);
    MPI_Send(&column->id, 1, MPI_INT, procid, 3, MPI_COMM_WORLD);
}

env init(int argc, char** argv)
{
    env e;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&e.numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&e.myid);
    return e;
}

vector* receiveColumn(MPI_Status *status)
{
    int size;
    vector* column;

    MPI_Recv(&size, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, status);
    column = createVector(size);

    MPI_Recv(column->values, size, MPI_DOUBLE, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, status);
    MPI_Recv(&column->id, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, status);

    return column;
}

int getProcIdByColumn(int column, int numprocs)
{
    return column % (numprocs-1) + 1;
}

void sendDimensions(int* rows, int* cols, env e)
{
    int procid;
    for(procid = 1; procid < e.numprocs; ++procid)
    {
        MPI_Send(rows, 1, MPI_INT, procid, 1, MPI_COMM_WORLD);
        MPI_Send(cols, 1, MPI_INT, procid, 2, MPI_COMM_WORLD);
    }
}
void receiveDimensions(int* rows, int* cols)
{
    MPI_Status status;

    MPI_Recv(rows, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(cols, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
}

matrix* decompose(matrix* m, env e)
{
    int procid;
    int j, k, s;
    vector *column, *kcolumn, *recvKcolumn, *recvColumn;
    MPI_Status status;
    int cols = m->cols;
    int myid = e.myid, numprocs = e.numprocs;

    for(k=1; k <= cols-1; k++)
    {
        if (myid == 0)
        {
            // STEP 1
            for (s = k+1; s <= m->rows; s++) 
                setMatValue(m, s, k, getMatValue(m, s, k) / getMatValue(m, k, k));
            
            kcolumn = getColumn(m, k);
            for (procid = 1; procid < numprocs; ++procid)
                sendColumn(kcolumn, procid);

            freeVector(kcolumn);
        }
        else
        {
            recvKcolumn = receiveColumn(&status);
        }
        
        // STEP 2
        j = k + 1;
        if (myid == 0)
        {
            int procid;
            int columns2work = cols - k;
            int counter = 0;
            while(j <= cols)
            {
                for(procid = 1; procid < numprocs; ++procid)
                {
                    if (j <= cols) 
                    {
                        ++counter;
                        column = getColumn(m, j);
                        sendColumn(column, procid);
                        freeVector(column);
                    }
                    ++j;
                }
            }

            j = 0;
            while(j < columns2work)
            {
                column = receiveColumn(&status);
                setColumn(m, column, column->id);
                freeVector(column);
                ++j;
            }
        }
        else
        {
            for(j = k + myid; j <= cols; j = j + numprocs-1)
            {
                recvColumn = receiveColumn(&status);
                updateColumn(recvColumn, recvKcolumn, recvKcolumn->id);
                sendColumn(recvColumn, 0);
                freeVector(recvColumn);
            }
        }

        if (myid != 0)
        {
            freeVector(recvKcolumn);
        }
    }
    return m;
}


matrix* decomposeNormal(matrix* m, env e)
{
    matrix *m2 = copyMatrix(m);
    int i, j, k, s;
    for(k = 1; k <= m->cols-1; k++)
    {
        // STEP 1
        for (s = k+1; s <= m->rows; s++)
        {
            setMatValue(m2, s, k, getMatValue(m2, s, k)/getMatValue(m2, k, k));
        }
        // STEP 2
        for (i = k+1; i <= m->cols; i++)
        {
            for (j = k+1; j <= m->rows; j++)
            {
                setMatValue(m2, i, j, getMatValue(m2, i, j) - getMatValue(m2, i, k) * getMatValue(m2, k, j));
            }
        }
    }
    return m2;
}

void final()
{
    MPI_Finalize();
}