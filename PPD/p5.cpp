#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[4] = {1, 2, 3, 4}, B[4] = {5, 6, 7, 8}, C[4] = {0};
    int local_A[2], local_C[2] = {0};

    MPI_Bcast(B, 4, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        local_A[0] = A[0]; local_A[1] = A[1];
        MPI_Send(&A[2], 2, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Recv(local_A, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int j = 0; j < 2; j++) {
        local_C[j] = (local_A[0] * B[j]) + (local_A[1] * B[2 + j]);
    }

    if (rank == 1) {
        MPI_Send(local_C, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        C[0] = local_C[0]; C[1] = local_C[1];
        int worker_C[2];
        MPI_Recv(worker_C, 2, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        C[2] = worker_C[0]; C[3] = worker_C[1];

        cout << "Matrix C = A x B\n";
        for (int i = 0; i < 4; i++) {
            cout << C[i] << (i % 2 == 0 ? " " : "\n");
        }
        cout << "\n";
    }

    MPI_Finalize();
    return 0;
}

// mpic++ p5.cpp -o p5
// mpirun -np 2 ./p5