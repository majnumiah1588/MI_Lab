// Problem_5: Distribute matrix rows from root to other processes using MPI_Send.
// Each process performs its portion of matrix multiplication and sends result back to root.
#include <stdio.h>
#include <mpi.h>

#define n 4

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int r, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int A[n*n], B[n*n], C[n*n];

    if (r == 0) {
        for (int i = 0; i < n*n; i++) { A[i] = i + 1; B[i] = i + 1; }
    }

    MPI_Bcast(B, n*n, MPI_INT, 0, MPI_COMM_WORLD);

    int la[n], lc[n];

    if (r == 0) {
        for (int j = 0; j < n; j++) la[j] = A[r*n + j];
        for (int s = 1; s < p; s++)
            MPI_Send(&A[s*n], n, MPI_INT, s, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(la, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int j = 0; j < n; j++) {
        int sum = 0;
        for (int k = 0; k < n; k++) sum += la[k] * B[k*n + j];
        lc[j] = sum;
    }

    if (r == 0) {
        for (int j = 0; j < n; j++) C[r*n + j] = lc[j];
        for (int s = 1; s < p; s++)
            MPI_Recv(&C[s*n], n, MPI_INT, s, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("C = A x B:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) printf("%5d ", C[i*n + j]);
            printf("\n");
        }
    } else {
        MPI_Send(lc, n, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

// mpicc p5.c -o p5
// mpirun -np 2 ./p5