
// Problem_3: Ring topology - each process sends to next, receives from previous.
// Measure communication time for different message sizes.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;
    int value = 100, recv_value;

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    MPI_Sendrecv(&value, 1, MPI_INT, next, 0,
                 &recv_value, 1, MPI_INT, prev, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    double time_taken = MPI_Wtime() - start_time;

    printf("P%d sends %d to P%d\n", rank, value, next);
    printf("P%d receives %d from P%d\n", rank, recv_value, prev);

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Ring communication completed.\n");
    }

    MPI_Finalize();
    return 0;
}

// mpicc p3.c -o p3
// mpirun -np 4 ./p3                