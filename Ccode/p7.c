// Problem_7: Distributed vector computation using MPI_Isend and MPI_Irecv.
// Compare execution time with equivalent blocking MPI_Send/MPI_Recv.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 8, send_data[8], recv_data[8];
    int dest = (rank == 0) ? 1 : 0;
    int i;

    for (i = 0; i < N; i++)
        send_data[i] = (rank == 0) ? (i + 1) * 10 : (i + 1);

    // Blocking version
    double start_b = MPI_Wtime();
    MPI_Send(send_data, N, MPI_INT, dest, 0, MPI_COMM_WORLD);
    MPI_Recv(recv_data, N, MPI_INT, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    double time_blocking = MPI_Wtime() - start_b;

    // Non-blocking version
    MPI_Request req_s, req_r;
    double start_nb = MPI_Wtime();
    MPI_Isend(send_data, N, MPI_INT, dest, 1, MPI_COMM_WORLD, &req_s);
    MPI_Irecv(recv_data, N, MPI_INT, dest, 1, MPI_COMM_WORLD, &req_r);
    MPI_Wait(&req_s, MPI_STATUS_IGNORE);
    MPI_Wait(&req_r, MPI_STATUS_IGNORE);
    double time_non_blocking = MPI_Wtime() - start_nb;

    printf("P%d received: ", rank);
    for (i = 0; i < N; i++) printf("%d ", recv_data[i]);
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Nonblocking communication completed.\n\nSample performance:\n");
        printf("Blocking communication time     = %f s\n", time_blocking);
        printf("Nonblocking communication time  = %f s\n", time_non_blocking);
    }

    MPI_Finalize();
    return 0;
}

// mpicc p7.c -o p7
// mpirun -np 2 ./p7