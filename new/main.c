#include "matrix.h"
#include <mpi.h>
#define MASTER      0
#define FROM_MASTER 1
#define FROM_WORKER 2

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    
    int nprocs;
    MPI_Comm_size(comm, &nprocs);
    
    int myid;    
    MPI_Comm_rank(comm, &myid);

    int nrows, ncols, avg;
    ceil *matrix;
    MPI_Request *req;
    int workers = nprocs - 1;

    if (myid == 0)
    {
        matrix = read_matrix(argv[1], &ncols);
        nrows = ncols;
        transpose(matrix, nrows, ncols);

        avg = (int)(nrows / workers);
        req = malloc(workers * sizeof(MPI_Request));
        
        for (int id = 1; id <= workers; ++id)
        {
            int start = (id-1) * avg,
                mrows = (id == workers)? (nrows - start): avg;

            MPI_Isend(&mrows, 1, MPI_INT, id, FROM_MASTER, comm, &req[id-1]);
            MPI_Isend(&ncols, 1, MPI_INT, id, FROM_MASTER, comm, &req[id-1]);
            MPI_Isend(matrix + start * ncols, mrows * size, MPI_DOUBLE, id, FROM_MASTER, comm, &req[id-1]);
        }

        MPI_Waitall(workers, req, MPI_STATUSES_IGNORE);
        free(req);
    }
    else
    {
        MPI_Recv(&nrows, 1, MPI_INT, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
        MPI_Recv(&ncols, 1, MPI_INT, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);

        matrix = init_matrix(nrows, ncols);
        MPI_Recv(matrix, nrows * ncols, MPI_DOUBLE, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
    }

    // LU decomposition

    ceil *kcol;
    int i, length;

    for (int k = 0; k < nrows; ++k)
    {
        if (myid == 0)
        {
            // STEP 1: eliminate the matrix elements below the main diagonal
            for (int s = k+1; s < ncols; s++) 
                setMatrixValue(matrix, ncols, k, s, getMatrixValue(matrix, ncols, k, s) / getMatrixValue(matrix, nrows, k, k));
            
            kcol = getColumn(matrix, nrows, size, k);
            for (int id = 1; id <= workers; ++id)
            {
                // send this kcol to others
                MPI_Send(&k, 1, MPI_INT, id, FROM_MASTER, comm);
                MPI_Send(&nrows, 1, MPI_INT, id, FROM_MASTER, comm);
                MPI_Send(kcol, nrows, MPI_DOUBLE, id, FROM_MASTER, comm);
            }
        }
        else
        {
            // receive kcol from master
            MPI_Recv(&i, 1, MPI_INT, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
            MPI_Recv(&length, 1, MPI_INT, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
            
            kcol = init_vector(length);
            MPI_Recv(kcol, length, MPI_DOUBLE, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
        }

        // STEP 2: eliminate all the matrix elements below the main diagonal
        int j;
        if (myid == 0)
        {
            int cols2work = size - k;
            j = k+1;
            while (j < size)
            {
                for (int id = 1; id <= workers; ++id)
                {
                    if (j < size)
                    {
                        // send this kcol to others
                        ceil *jcol = getColumn(matrix, nrows, size, j);
                        MPI_Send(&j, 1, MPI_INT, id, FROM_MASTER, comm);
                        MPI_Send(jcol, nrows, MPI_DOUBLE, id, FROM_MASTER, comm);
                        destroy_vector(jcol);
                    }
                    ++j;
                }
            }

            j = 0;
            while (j < cols2work)
            {
                // receive column from workers
                MPI_Recv(&i, 1, MPI_INT, MPI_ANY_SOURCE, FROM_WORKER, comm, MPI_STATUSES_IGNORE);
                MPI_Recv(&length, 1, MPI_INT, MPI_ANY_SOURCE, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
            
                ceil *receiveColumn = init_vector(length);
                MPI_Recv(receiveColumn, length, MPI_DOUBLE, MPI_ANY_SOURCE, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
                
                // update source
                for (int p = 0; p < length; ++p)
                    setMatrixValue(matrix, size, p, i, getVectorValue(receiveColumn, p));
                
                destroy_vector(receiveColumn);
                ++j;
            }
            
            destroy_vector(kcol);
        }
        else
        {
            for (int s = k + myid; s < nrows; s += workers)
            {
                MPI_Recv(&j, 1, MPI_INT, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
                
                ceil *receiveColumn = init_vector(length);
                MPI_Recv(receiveColumn, length, MPI_DOUBLE, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
                
                // update j-th column by k-th column
                for (int p = k+1; p < length; ++p)
                {
                    ceil value = getVectorValue(receiveColumn, p) - getVectorValue(kcol, p) * getVectorValue(receiveColumn, k);
                    setVectorValue(receiveColumn, p, value);
                }

                // feedback to main process
                MPI_Send(&j, 1, MPI_INT, MASTER, FROM_WORKER, comm);
                MPI_Send(&length, 1, MPI_INT, MASTER, FROM_WORKER, comm);
                MPI_Send(receiveColumn, length, MPI_DOUBLE, MASTER, FROM_WORKER, comm);
                
                destroy_vector(receiveColumn);
            }

            destroy_vector(kcol);
        }
    }
            
    if (myid == 0)
    {
        mdisp(matrix, nrows, size);
        // ceil *c = getColumn(matrix, nrows, size, 2);
    
        // vdisp(c, nrows);
        // destroy_vector(c);
    }

    destroy_matrix(matrix);
    MPI_Finalize();

    return 0;
}