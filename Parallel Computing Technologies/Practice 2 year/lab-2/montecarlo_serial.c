#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

const int n = 1E7;

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double getrand() { return (double)rand() / RAND_MAX; }

double func(double x, double y) { return pow(exp(x + y), 2); }

int main(int argc, char *argv[])
{
    srand(time(0));
    int in = 0;
    double s = 0;
    double t = wtime();
    for (int i = 0; i < n; ++i) {
        double x = getrand(); // x in (0, 1)
        double y = getrand(); // y in (0, 1 - x)
        if (y < (1.0 - x)) {
            ++in;
            s += func(x, y);
        }
    }
    double v = in / n;
    double res = v * s / in;
    t = wtime() - t;
    printf("Result: %.12f, n %d\nElapsed time (sec.): %.6f\n", res, n, t);
    return 0;
}