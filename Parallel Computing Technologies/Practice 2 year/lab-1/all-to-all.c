#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    // Номер процесса (0, 1, ..., p - 1) в глобальном коммуникаторе MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Число процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int count = 1;
    char *sbuf = malloc(sizeof(*sbuf) * count * size);
    char *rbuf = malloc(sizeof(*rbuf) * count * size);
    for (int i = 0; i < count * size; ++i) {
        sbuf[i] = rank;
    }
    int n = 0;
    MPI_Request *reqs = malloc (sizeof(*reqs) * 2 * size);
    double t = MPI_Wtime();
    for (int i = 0; i < size; ++i) {
        MPI_Isend(sbuf + i * count * sizeof(*sbuf), count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[n++]);
    }
    for (int i = 0; i < size; ++i) {
        MPI_Irecv(rbuf + i * count * sizeof(*rbuf), count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[n++]);
    }
    MPI_Waitall(n, reqs, MPI_STATUS_IGNORE);
    t = MPI_Wtime() - t;
	printf ("rank: %d time: %.6f\n", rank, t);	
    for (int i = 0; i < count * size; ++i) {
        printf("%d", rbuf[i]);
    }
    printf ("\n");
    free(sbuf);
    free(rbuf);
    MPI_Finalize();
    return 0;
}