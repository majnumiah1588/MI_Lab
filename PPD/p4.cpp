#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) cout << "Please run with exactly 2 processes." << endl;
        MPI_Finalize();
        return 0;
    }

    int iterations = 5;
    int sizes[] = {64, 256, 1024, 4096, 16384, 65536, 1048576};
    int num_sizes = 7;

    for (int s = 0; s < num_sizes; s++) {
        int msg_size = sizes[s];
        char* buffer = new char[msg_size];

        if (rank == 0) {
            MPI_Send(buffer, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(buffer, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double start_time = MPI_Wtime();

        for (int i = 0; i < iterations; i++) {
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

            cout << "Message size     : " << msg_size << " bytes" << endl;
            cout << "Iterations       : " << iterations << endl;
            cout << "Total round trips: " << iterations << endl;
            cout << "Average round-trip: " << avg_round_trip << " seconds" << endl;
            cout << "Average one-way  : " << avg_one_way << " seconds" << endl;
            cout << "------------------------------------------------" << endl;
        }

        delete[] buffer;
    }

    if (rank == 0) {
        cout << "Repeat the experiment for 64 bytes, 256 bytes, 1 KB, 4 KB, 16 KB, 64 KB, and 1 MB." << endl;
    }

    MPI_Finalize();
    return 0;
}
//mpic++ p4.cpp -o p4
//mpirun -np 2 ./p4