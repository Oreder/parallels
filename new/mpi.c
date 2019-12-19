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

    int size;
    ceil *matrix;
    MPI_Request *req;
    int workers = nprocs - 1;

    if (myid == 0)
    {
        matrix = read_matrix(argv[1], &size);
        
        int avg = (int)(size / workers);
        req = malloc(workers * sizeof(MPI_Request));

        for (int id = 1; id <= workers; ++id)
        {
            int start = (id-1) * avg,
                nrows = (id == workers)? (size - start): avg; 
            MPI_Isend(&nrows, 1, MPI_INT, id, FROM_MASTER, comm, &req[id-1]);
            MPI_Isend(&size, 1, MPI_INT, id, FROM_MASTER, comm, &req[id-1]);
            MPI_Isend(matrix + start * size, nrows * size, MPI_DOUBLE, id, FROM_MASTER, comm, &req[id-1]);
        }

        MPI_Waitall(workers, req, MPI_STATUSES_IGNORE);

        // code here

        destroy_matrix(matrix);
        free(req);
    }
    else
    {
        int nrows;
        MPI_Recv(&nrows, 1, MPI_INT, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
        MPI_Recv(&size, 1, MPI_INT, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);

        matrix = init_matrix(nrows, size);
        MPI_Recv(matrix, nrows * size, MPI_DOUBLE, MASTER, FROM_MASTER, comm, MPI_STATUSES_IGNORE);
        
        if (myid == 3)
        {
            printf("Matrix %d x %d:\n", nrows, size);
            mdisp(matrix, nrows, size);
            ceil *c = getColumn(matrix, nrows, size, 2);
        
            vdisp(c, nrows);
            destroy_vector(c);
        }
        destroy_matrix(matrix);
    }
    
    MPI_Finalize();

    return 0;
}