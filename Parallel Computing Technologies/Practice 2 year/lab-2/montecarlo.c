#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int N = 1E8;

double getrand() { return (double)rand() / RAND_MAX; }

double func(double x, double y) { return pow(exp(x + y), 2); }

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, commsize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    srand(rank);
    int in = 0;
    double s = 0;
    double t;
    if (rank == 0) {
        t = MPI_Wtime();
    }
    for (int i = rank; i < N; i += commsize) {
        double x = getrand(); // x in (0, 1)
        double y = getrand(); // y in (0, 1 - x)
        if (y < (1.0 - x)) {
            ++in;
            s += func(x, y);
        }
    }
    int gin = 0;
    MPI_Reduce(&in, &gin, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double gsum = 0.0;
    MPI_Reduce(&s, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        t = MPI_Wtime() - t;
        double v = (double)gin / N;
        double res = v * gsum / gin;
        printf("Result: %.12f, N %d\nTime: %lf\n", res, N, t);
    }
    MPI_Finalize();
    return 0;
}