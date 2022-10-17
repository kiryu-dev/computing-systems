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

    int prev = (rank + size - 1) % size;
    int next = (rank + 1) % size;

    int count = 1;
    char* buf = malloc(sizeof(*buf) * size * count);

    char* p = buf + rank * count * sizeof(*p);

    for (int i = 0; i < count; ++i) {
        p[i] = rank;
    }
    int sblock = rank;
    int rblock = prev;

    double t = MPI_Wtime();
    for (int i = 0; i < size - 1; ++i) {
        MPI_Sendrecv(buf + count * sblock, count, MPI_CHAR, next, 0, buf + count * rblock, count, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sblock = (sblock - 1 + size) % size;
        rblock = (rblock - 1 + size) % size;
    }

    t = MPI_Wtime() - t;
    printf("time: %lf\nrank: %d\n", t, rank);
    // if (rank == 15) {
    //     printf("rank: %d\n", rank);
    //     for (int i = 0; i < size * count; i += count) {
    //         printf("%d\n", buf[i]);
    //     }
    // }
    for (int i = 0; i < size * count; i += count) {
        printf("%d\n", buf[i]);
    }

    free(buf);
    MPI_Finalize();
    return 0;
}