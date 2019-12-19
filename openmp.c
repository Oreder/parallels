# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <omp.h>

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
    int n = 10000000;
    double total;
    double wtime;
    double x;

    printf("  \nC version\n");
    printf("  Using OpenMP for parallel execution.\n");
    printf("  Estimate an integral of f(x) from A to B.\n");
    printf("  Function: f(x) = 50 / (pi * (2500 * x * x + 1))\n\n");
    printf("  A = %f\tB = %f\tN = %d\n", a, b, n);
    printf("  EXACT = %.16f\n\n", exact);

    timestamp();
    wtime = omp_get_wtime();
    total = 0.0;

    #pragma omp parallel shared(a, b, n) private(i, x) 
    #pragma omp for reduction(+ : total)
    for (i = 0; i < n; i++)
    {
        x = ((double)(n - i - 1) * a + (double)(i) * b) / (double)(n - 1);
        total += + f(x);
    }

    wtime = omp_get_wtime() - wtime;

    total =(b - a) * total /(double) n;
    error = fabs(total - exact);

    printf("\n");
    printf("  Estimate = %.16f\n", total);
    printf("  Error    = %e\n", error);
    printf("  Time     = %f\n", wtime);
    timestamp();

  return 0;
}

double f (double x)
{
    double pi = 3.141592653589793;
    return 50.0 /(pi *(2500.0 * x * x + 1.0));
}

/*
 * CPU_TIME reports the total CPU time for a program in second
 */
double cpu_time()
{
  return (double) clock() /(double) CLOCKS_PER_SEC;;
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

    strftime (time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);
#undef TIME_SIZE

    printf ("%s\n", time_buffer);
}