#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define pi 3.141592653589793

double f(double);
void timestamp();

int main(int argc, char *argv[])
{
    double a;
    double b;
    double error;
    double exact;
    int i;
    int master = 0;
    double my_a;
    double my_b;
    double step;
    int my_id;
    int my_n;
    double my_total;
    int n;
    int p;
    int p_num;
    int source;
    MPI_Status status;
    int tag;
    int target;
    double total;
    double wtime;
    double x;

    a =  0.0;
    b = 10.0;
    n = atoi(argv[1]);  // 1000000
    exact = 0.49936338107645674464;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    MPI_Comm_size(MPI_COMM_WORLD, &p_num);

    if(my_id == master)
    {
    /*
     * We want N to be the total number of evaluations.
     * If necessary, we adjust N to be divisible by the number of processes.
     */
        my_n = n / (p_num - 1);
        if (my_n % 2 == 1) my_n++;
        
        printf("\n  C/MPI version\n");
        printf("  Estimate an integral of f(x) from A to B.\n");
        printf("  Function: f(x) = 50 / (pi * (2500 * x * x + 1))\n\n");
        printf("  A = %f\tB = %f\tN = %d\n", a, b, n);
        printf("  EXACT = %.16f\n", exact);
        printf("\n  Use MPI to divide the computation among multiple processes.\n");
        
        wtime = MPI_Wtime();
        timestamp();
    }

    source = master;
    MPI_Bcast(&my_n, 1, MPI_INT, source, MPI_COMM_WORLD);

    /*
     * Process 0 assigns each process a subinterval of [A,B].
     */
    if(my_id == master)
    {
        step = (b - a) / (double)(p_num - 1);
        for (p = 1; p <= p_num - 1; p++)
        {
            my_a = a + (double)(p - 1) * step;
            my_b = my_a + step;

            target = p;
            tag = 1;
            MPI_Send(&my_a, 1, MPI_DOUBLE, target, tag, MPI_COMM_WORLD);

            tag = 2;
            MPI_Send(&my_b, 1, MPI_DOUBLE, target, tag, MPI_COMM_WORLD);
        }

        total = 0.0;
        my_total = 0.0;
    }
    else
    {
        /*
         * Processes receive MY_A, MY_B, and compute their part of the integral.
         */
        source = master;
        tag = 1;
        MPI_Recv(&my_a, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);

        tag = 2;
        MPI_Recv(&my_b, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);

        step = (my_b - my_a) / (double)(my_n);
        my_total = 0.0;

        for (i = 0; i < (int)(my_n / 2); i++)
        {
            x = my_a + (double)(i) * step * 2.0;
            my_total +=  f(x) + 4.0 * f(x + step) + f(x + 2.0 * step);
        }

        my_total *= step / 3.0;

        printf("  Process %d contributed MY_TOTAL = %.16f\n", my_id, my_total);
    }

    /*
     * Each process sends its value to the master process.
     */
    MPI_Reduce(&my_total, &total, 1, MPI_DOUBLE, MPI_SUM, master, MPI_COMM_WORLD);

    /*
     * Compute the weighted estimate.
     */
    if(my_id == master)
    {
        error = fabs(total - exact);
        wtime = MPI_Wtime() - wtime;

        printf("\n  Estimate = %.16f\n", total);
        printf("  Error = %e\n\n", error);
        printf("  Time = %f\n\n", wtime);
    }

    MPI_Finalize();
    
    if(my_id == master)
        timestamp();

    return 0;
}


double f (double x)
{
    return 50.0 / (pi * (2500.0 * x * x + 1.0));
}

/*
 * TIMESTAMP prints the current YMDHMS date as a time stamp
 */
void timestamp()
{
#define TIME_SIZE 40
    static char time_buffer[TIME_SIZE];
    
    time_t now = time (NULL);
    const struct tm *tm = localtime (&now);

    strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);
#undef TIME_SIZE

    printf ("%s\n", time_buffer);
}