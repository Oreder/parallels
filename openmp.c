#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#define pi 3.141592653589793

double f(double x);
double cpu_time();
void timestamp();

int main(int argc, char *argv[])
{
    double a = 0.0;
    double b = 10.0;
    double error;
    double exact = 0.49936338107645674464;
    int i;
    int n = atoi(argv[1]); // 1000000
    double total;
    double wtime;
    double x;

    printf("\n  C version\n");
    printf("  Using OpenMP for parallel execution.\n");
    printf("  Estimate an integral of f(x) from A to B.\n");
    printf("  Function: f(x) = 50 / (pi * (2500 * x * x + 1))\n\n");
    printf("  A = %f\tB = %f\tN = %d\n", a, b, n);
    printf("  EXACT = %.16f\n\n", exact);
    double step = (b - a) / (double)(n);

    timestamp();
    wtime = omp_get_wtime();
    omp_set_num_threads(atoi(argv[2]));

    total = 0.0;

    #pragma omp parallel shared(a, b, n) private(i, x) 
    #pragma omp for reduction(+ : total)
    for (i = 0; i < (int)(n / 2); i++)
    {
        x = a + (double)(i) * 2.0 * step;
        total += f(x) + 4.0 * f(x + step) + f(x + 2.0 * step);
    }

    wtime = omp_get_wtime() - wtime;

    total *= step / 3.0;
    error = fabs(total - exact);

    printf("\n  Estimate = %.16f\n", total);
    printf("  Error    = %e\n", error);
    printf("  Time     = %f\n", wtime);
    timestamp();

    return 0;
}

double f(double x)
{
    return 50.0 / (pi * (2500.0 * x * x + 1.0));
}

/*
 * CPU_TIME reports the total CPU time for a program in second
 */
double cpu_time()
{
  return (double)clock() / (double)CLOCKS_PER_SEC;
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