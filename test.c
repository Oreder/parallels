// Swap 2 arrays by mpi
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define MASTER  0
#define FROM_MASTER 1
#define FROM_WORKER 2

int disp(const char *s, const int size, int a[])
{
    printf("\n%s:", s);
    for (int i = 0; i < size; i++)
        printf("  %d", a[i]);
}

void swap(int *x, int *y)
{
    int z = *x;
    *x = *y;
    *y = z;
}

int main(int argc, char** argv) 
{
    const int n = 17;
    int a[17], b[17], i, count, offset, avetask;
    // int a[] = { 1, 9, 2, 1, 3, 4, 7, 5, 6, 3, 7, 5, 8, 2, 9, 0, 6 };
    // int b[] = { 9, 2, 6, 3, 4, 7, 4, 6, 0, 7, 5, 8, 3, 9, 0, 6, 1 };

    // const int n = sizeof(a) / sizeof(int);

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    
    int worker;        // get worker-id
    MPI_Comm_rank(MPI_COMM_WORLD, &worker);
    
    int ntask;      // get total processes
    MPI_Comm_size(MPI_COMM_WORLD, &ntask);
    
    MPI_Status stt;  // recept status

    // We are assuming at least 2 processes for this task
    if (ntask < 2)
    {
        printf("World size must be at least two for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int nworkers = ntask - 1;

    if (worker == MASTER)
    {
        for (i = 0; i < n; i++)
        {
            a[i] = rand() % 9;
            b[i] = a[i] + 1;
        }

        disp("A", n, a);
        disp("B", n, b);

        // separate processes
        
        avetask = n / nworkers;
        offset = 0;

        for (int dest = 1; dest <= nworkers; dest++)
        {
            count = (dest == nworkers)? (n - offset) : avetask;
            MPI_Send(&offset   ,     1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&count    ,     1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&a[offset], count, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&b[offset], count, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            printf("\nMaster sends %d to %d with offset: %d", count, dest, offset);
            offset += avetask;
        }

        // wait for results from all worker tasks
        for (int src = 1; src <= nworkers; src++)
        {
            count = (src == nworkers)? (n - offset) : avetask;
            MPI_Recv(&offset   ,     1, MPI_INT, src, FROM_WORKER, MPI_COMM_WORLD, &stt);
            MPI_Recv(&a[offset], count, MPI_INT, src, FROM_WORKER, MPI_COMM_WORLD, &stt);
            MPI_Recv(&b[offset], count, MPI_INT, src, FROM_WORKER, MPI_COMM_WORLD, &stt);
        }

        // display
        disp("A", n, a);
        disp("B", n, b);
        printf("\n");
    }
    else    // workers
    {
        MPI_Recv(&offset, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &stt);
        MPI_Recv(&count , 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &stt);
        MPI_Recv(&a , count, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &stt);
        MPI_Recv(&b , count, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &stt);

        for (i = 0; i < count; i++)
            swap(a + i, b + i);

        MPI_Send(&offset, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&a, count, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&b, count, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
