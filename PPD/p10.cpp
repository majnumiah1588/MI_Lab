#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int items[8] = {10, 20, 30, 40, 50, 60, 70, 80}, item;

    if (rank == 0) {
        for (int i = 0; i < 8; i++) {
            MPI_Send(&items[i], 1, MPI_INT, (i % 2 == 0) ? 1 : 2, 0, MPI_COMM_WORLD);
            cout << "P0 produced: " << items[i] << endl;
        }
    } else {
        for (int i = 0; i < 4; i++) {
            MPI_Recv(&item, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            cout << "P" << rank << " consumed: " << item << endl;
        }
    }

    MPI_Finalize();
    return 0;
}