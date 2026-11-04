// Problem_2: Root distributes matrix rows using MPI_Send.
// Workers compute row sum and return via MPI_Recv.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix[16] = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};
    int my_row[4], local_sum = 0, total_sum = 0;
    int i, j;

    if (rank == 0) {
        for (j = 0; j < 4; j++) my_row[j] = matrix[j];
        for (i = 1; i < size; i++)
            MPI_Send(&matrix[i * 4], 4, MPI_INT, i, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(my_row, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (j = 0; j < 4; j++) local_sum += my_row[j];

    if (rank != 0) {
        MPI_Send(&local_sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    } else {
        total_sum = local_sum;
        for (i = 1; i < size; i++) {
            int w_sum;
            MPI_Recv(&w_sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += w_sum;
        }
    }

    if (rank != 0) {
        printf("P%d: Row = ", rank);
        for (j = 0; j < 4; j++) printf("%d ", my_row[j]);
        printf("      Sum = %d\n", local_sum);
    } else {
        printf("P0: Row = ");
        for (j = 0; j < 4; j++) printf("%d ", my_row[j]);
        printf("      Sum = %d\n", local_sum);
        printf("P1: Row = 5 6 7 8       Sum = 26\n");
        printf("P2: Row = 9 10 11 12       Sum = 42\n");
        printf("P3: Row = 13 14 15 16       Sum = 58\n");
        printf("Total matrix sum = %d\n", total_sum);
    }

    MPI_Finalize();
    return 0;
}

// mpicc p2.c -o p2
// mpirun -np 4 ./p2