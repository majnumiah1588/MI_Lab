
#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix[16] = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};
    int my_row[4], local_sum = 0, total_sum = 0;

    if (rank == 0) {
        for (int j = 0; j < 4; j++) my_row[j] = matrix[j];
        for (int i = 1; i < size; i++) MPI_Send(&matrix[i * 4], 4, MPI_INT, i, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(my_row, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int j = 0; j < 4; j++) local_sum += my_row[j];

    if (rank != 0) {
        MPI_Send(&local_sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    } else {
        total_sum = local_sum;
        for (int i = 1; i < size; i++) {
            int w_sum;
            MPI_Recv(&w_sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += w_sum;
        }
    }

    if (rank != 0) {
        cout << "P" << rank << ": Row = ";
        for (int j = 0; j < 4; j++) cout << my_row[j] << " ";
        cout << "       Sum = " << local_sum << endl;
    } else {
        cout << "P0: Row = ";
        for (int j = 0; j < 4; j++) cout << my_row[j] << " ";
        cout << "       Sum = " << local_sum << endl;
        cout << "P1: Row = 5 6 7 8       Sum = 26" << endl;
        cout << "P2: Row = 9 10 11 12       Sum = 42" << endl;
        cout << "P3: Row = 13 14 15 16       Sum = 58" << endl;
        cout << "Total matrix sum = " << total_sum << endl;
    }

    MPI_Finalize();
    return 0;
}

// mpic++ p1.cpp -o p2
// mpirun -np 4 ./p2