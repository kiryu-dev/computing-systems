#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;

    // Номер процесса (0, 1, ..., p - 1) в глобальном коммуникаторе MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Число процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int root = 0;
    int count = 1;
    char* buf = malloc(sizeof(*buf) * count);

    if (rank == root) {
        for (int i = 0; i < count; ++i) {
            buf[i] = i;
        }
    }

    double t = MPI_Wtime();
    if (rank == root) {
        for (int i = 0; i < size; ++i) {
            if (i == rank) continue;
            MPI_Send(buf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(buf, count, MPI_CHAR, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    t = MPI_Wtime() - t;

    printf("rank: %d\ntime: %lf\n", rank, t);
    for (int i = 0; i < count; ++i) {
        printf("%d\n", buf[i]);
    }

    free(buf);
    MPI_Finalize();
    return 0;
}