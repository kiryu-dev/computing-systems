#include <math.h>
#include <mpi.h>
#include <stdio.h>

const double EPS = 1E-6;
const int n0 = 100000000; // кол-во подотрезков, на которые разбиваем

double func(double x) { return log(1 + x) / x; }

/* serial time (sec.) = 9.938411 */

int main(int argc, char *argv[]) {
    const double a = 0.1;
    const double b = 1.0;
    MPI_Init(&argc, &argv);
    int commsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = n0, k;
    double sq[2], delta = 1.0;
    double t;
    if (rank == 0) {
        t = MPI_Wtime();
    }
    for (k = 0; delta > EPS; n *= 2, k ^= 1) {
        int points_per_proc = n / commsize;
        int lb = rank * points_per_proc;
        int ub = (rank == commsize - 1) ? (n - 1) : (lb + points_per_proc - 1);
        double h = (b - a) / n;
        double s = 0.0;
        for (int i = lb; i <= ub; ++i) {
            s += func(a + h * (i + 0.5));
        }
        /* глобальная сумма будет посчитана в sq[k], у всех процессов
        будет корректно-сформированное значение интеграла */
        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        sq[k] *= h;
        if (n > n0) {
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
        }
    }
    if (rank == 0) {
        t = MPI_Wtime() - t;
        printf(
            "Result: %.12f; Runge rule: EPS %e, n %d\nTime: %lf\n",
            sq[k] * sq[k],
            EPS,
            n / 2,
            t);
    }
    MPI_Finalize();
    return 0;
}