#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;

    // Номер процесса (0, 1, ..., p - 1) в глобальном коммуникаторе MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Число процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int root = 0;

    int count = 1; // 1B
    char *rbuf;
    if (rank == root) {
        rbuf = malloc(sizeof(*rbuf) * count * size);
    }
    char *sbuf = malloc(sizeof(*sbuf) * count);
    
    for (int i = 0; i < count; ++i) {
        sbuf[i] = rank;
    }

    double t = MPI_Wtime();
    if (root == rank) {
        for (int i = 0; i < size; ++i) {
            if (root == i) {
                memcpy(rbuf + root * count * sizeof(char), sbuf, sizeof(char) * count);
            } else {
                MPI_Recv(rbuf + i * count * sizeof(char), count, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    } else {
        MPI_Send(sbuf, count, MPI_CHAR, root, 0, MPI_COMM_WORLD);
    }
    t = MPI_Wtime() - t;

    if (root == rank) {
        printf("rank = %d\n", rank);
        printf("time = %.6f\n", t);
        for (int i = 0; i < count * size; ++i) {
            printf("%d\n", rbuf[i]);
        }
    }

    free(sbuf);
    if (rank == root) {
        free(rbuf);
    }
    MPI_Finalize();
    return 0;
}