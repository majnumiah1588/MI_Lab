#include <iostream>
#include <mpi.h>
using namespace std;

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

    MPI_Sendrecv(&value, 1, MPI_INT, next, 0, &recv_value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    double time_taken = MPI_Wtime() - start_time;

    cout << "P" << rank << " sends " << value << " to P" << next << endl;
    cout << "P" << rank << " receives " << recv_value << " from P" << prev << endl;

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "Ring communication completed." << endl;
    }

    MPI_Finalize();
    return 0;
}

// mpic++ p1.cpp -o p3
// mpirun -np 4 ./p3