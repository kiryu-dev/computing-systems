#include <math.h>
#include <stdio.h>
#include <sys/time.h>

const double eps = 1E-6;
const int n0 = 100000000;

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double func(double x) { return log(1 + x) / x; }

int main() {
    const double a = 0.1;
    const double b = 1.0;
    int n = n0, k;
    double sq[2], delta = 1;
    double t = wtime();
    for (k = 0; delta > eps; n *= 2, k ^= 1) {
        double h = (b - a) / n;
        double s = 0.0;
        for (int i = 0; i < n; ++i) {
            s += func(a + h * (i + 0.5));
        }
        sq[k] = s * h;
        if (n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
    }
    t = wtime() - t;
    printf("Result: %.12f; Runge rule: EPS %e, n %d\nElapsed time (sec.): %.6f\n", sq[k] * sq[k], eps, n / 2, t);
    return 0;
}