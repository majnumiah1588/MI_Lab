// Problem_11: Root process reads configuration parameters and broadcasts them to all
// MPI processes. Each process uses the received parameters to perform a computation.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int iter = 0, thresh = 0, block = 0;

    if (rank == 0) {
        printf("Root process reads:\n");
        iter = 1000, thresh = 50, block = 256;
    }

    MPI_Bcast(&iter, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&thresh, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&block, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("P%d received: Iterations = %d, Threshold = %d, Block size = %d\n",
           rank, iter, thresh, block);

    MPI_Finalize();
    return 0;
}

// mpicc p11.c -o p11
// mpirun -np 4 ./p11