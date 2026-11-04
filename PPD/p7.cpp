#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 8, send_data[8], recv_data[8], dest = (rank == 0) ? 1 : 0;

    for (int i = 0; i < N; i++) send_data[i] = (rank == 0) ? (i + 1) * 10 : (i + 1);

    double start_b = MPI_Wtime();
    MPI_Send(send_data, N, MPI_INT, dest, 0, MPI_COMM_WORLD);
    MPI_Recv(recv_data, N, MPI_INT, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    double time_blocking = MPI_Wtime() - start_b;

    MPI_Request req_s, req_r; 
    double start_nb = MPI_Wtime();
    MPI_Isend(send_data, N, MPI_INT, dest, 1, MPI_COMM_WORLD, &req_s);
    MPI_Irecv(recv_data, N, MPI_INT, dest, 1, MPI_COMM_WORLD, &req_r);
    MPI_Wait(&req_s, MPI_STATUS_IGNORE);
    MPI_Wait(&req_r, MPI_STATUS_IGNORE);
    double time_non_blocking = MPI_Wtime() - start_nb;

    cout << "P" << rank << " received: ";
    for (int i = 0; i < N; i++) cout << recv_data[i] << " ";
    cout << "\n";

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "Nonblocking communication completed.\n\nSample performance:\n";
        cout << "Blocking communication time     = " << time_blocking << " s\n";
        cout << "Nonblocking communication time  = " << time_non_blocking << " s\n";
    }

    MPI_Finalize();
    return 0;
}
/*
mpic++ p11.cpp -o p11
mpirun -np 4 ./p11
*/