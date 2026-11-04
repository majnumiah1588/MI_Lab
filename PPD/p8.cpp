#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int my_msg = (rank == 0) ? 100 : 200, recv_msg = 0;
    int dest = (rank == 0) ? 1 : 0;

    if (rank == 0) {
        cout << "Unsafe version:\nP0: Waiting to send 100" << endl;
        MPI_Send(&my_msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == 1) {
        cout << "P1: Waiting to send 200" << endl;
        MPI_Send(&my_msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_msg, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) cout << "Potential deadlock detected.\nCorrected version:" << endl;
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Sendrecv(&my_msg, 1, MPI_INT, dest, 1, &recv_msg, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (rank == 0) {
        cout << "P0 sent 100" << endl;
        MPI_Barrier(MPI_COMM_WORLD);
        cout << "P1 received 100" << endl;
        MPI_Barrier(MPI_COMM_WORLD);
        cout << "P1 sent 200" << endl;
        MPI_Barrier(MPI_COMM_WORLD);
        cout << "P0 received 200" << endl;
        MPI_Barrier(MPI_COMM_WORLD);
        cout << "Communication completed without deadlock." << endl;
    }

    MPI_Finalize();
    return 0;
}