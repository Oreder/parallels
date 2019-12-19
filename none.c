#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define pi 3.141592653589793

double f(double);
double cpu_time();
void timestamp();

int main(int argc, char *argv[])
{
    double a;
    double b;
    double error;
    double exact;
    int i;
    double step;
    int n;
    int p_num;
    double total;
    double x;

    a =  0.0;
    b = 10.0;
    n = atoi(argv[1]);  // 1000
    exact = 0.49936338107645674464;
    
    timestamp();
    double start = cpu_time();
    if (n % 2 == 1) n++;
    
    step = (b - a) / (double)(n);
    total = 0.0;

    for (i = 0; i < (int)(n / 2); i++)
    {
        x = a + (double)(i) * step * 2.0;
        total +=  f(x) + 4.0 * f(x + step) + f(x + 2.0 * step);
    }

    total *= step / 3.0;
    error = fabs(total - exact);

    printf("\n  Estimate = %.16f\n", total);
    printf("  Error = %e\n\n", error);
    printf("  Time = %f\n\n", cpu_time() - start);

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
  return (double)clock() / (double) CLOCKS_PER_SEC;
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