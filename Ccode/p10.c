// Problem_10: Producer-consumer system - P0 generates data, P1/P2 receive and process
// using point-to-point communication.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int items[8] = {10, 20, 30, 40, 50, 60, 70, 80}, item;
    int i;

    if (rank == 0) {
        for (i = 0; i < 8; i++) {
            MPI_Send(&items[i], 1, MPI_INT, (i % 2 == 0) ? 1 : 2, 0, MPI_COMM_WORLD);
            printf("P0 produced: %d\n", items[i]);
        }
    } else {
        for (i = 0; i < 4; i++) {
            MPI_Recv(&item, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("P%d consumed: %d\n", rank, item);
        }
    }

    MPI_Finalize();
    return 0;
}

// mpicc p10.c -o p10
// mpirun -np 3 ./p10