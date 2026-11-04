// Problem_8: Two-process communication that can cause deadlock with blocking sends.
// Modify to avoid deadlock using MPI_Sendrecv.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int my_msg = (rank == 0) ? 100 : 200, recv_msg = 0;
    int dest = (rank == 0) ? 1 : 0;

    // ---- Unsafe version (deadlock-prone) ----
    if (rank == 0) {
        printf("Unsafe version:\nP0: Waiting to send 100\n");
        MPI_Send(&my_msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == 1) {
        printf("P1: Waiting to send 200\n");
        MPI_Send(&my_msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) printf("Potential deadlock detected.\nCorrected version:\n");
    MPI_Barrier(MPI_COMM_WORLD);

    // ---- Corrected version using MPI_Sendrecv ----
    MPI_Sendrecv(&my_msg, 1, MPI_INT, dest, 1,
                 &recv_msg, 1, MPI_INT, dest, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (rank == 0) {
        printf("P0 sent 100\n");
        MPI_Barrier(MPI_COMM_WORLD);
        printf("P1 received 100\n");
        MPI_Barrier(MPI_COMM_WORLD);
        printf("P1 sent 200\n");
        MPI_Barrier(MPI_COMM_WORLD);
        printf("P0 received 200\n");
        MPI_Barrier(MPI_COMM_WORLD);
        printf("Communication completed without deadlock.\n");
    }

    MPI_Finalize();
    return 0;
}

// mpicc p8.c -o p8
// mpirun -np 2 ./p8