// Problem_4: Ping-pong between two MPI processes using MPI_Send and MPI_Recv.
// Test message sizes from a few bytes to several megabytes; analyze latency and bandwidth.
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) printf("Please run with exactly 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    int iterations = 5;
    int sizes[] = {64, 256, 1024, 4096, 16384, 65536, 1048576};
    int num_sizes = 7;
    int s, i;

    for (s = 0; s < num_sizes; s++) {
        int msg_size = sizes[s];
        char* buffer = (char*)malloc(msg_size);

        // Warm-up
        if (rank == 0) {
            MPI_Send(buffer, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(buffer, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double start_time = MPI_Wtime();

        for (i = 0; i < iterations; i++) {
            if (rank == 0) {
                MPI_Send(buffer, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(buffer, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Recv(buffer, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(buffer, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }

        double end_time = MPI_Wtime();

        if (rank == 0) {
            double total_time = end_time - start_time;
            double avg_round_trip = total_time / iterations;
            double avg_one_way = avg_round_trip / 2.0;

            printf("Message size     : %d bytes\n", msg_size);
            printf("Iterations       : %d\n", iterations);
            printf("Total round trips: %d\n", iterations);
            printf("Average round-trip: %f seconds\n", avg_round_trip);
            printf("Average one-way  : %f seconds\n", avg_one_way);
            printf("------------------------------------------------\n");
        }

        free(buffer);
    }

    if (rank == 0) {
        printf("Repeat the experiment for 64 bytes, 256 bytes, 1 KB, 4 KB, 16 KB, 64 KB, and 1 MB.\n");
    }

    MPI_Finalize();
    return 0;
}

// mpicc p4.c -o p4
// mpirun -np 2 ./p4