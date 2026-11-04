#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int all_data[16] = {1,2,2,3, 2,3,4,4, 1,4,5,5, 0,2,5,9};
    int local_hist[10] = {0}, global_hist[10];

    for (int i = 0; i < 4; i++) local_hist[all_data[rank * 4 + i]]++;

    MPI_Allreduce(local_hist, global_hist, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Final histogram after MPI_Allreduce:\nValue  Frequency\n";
        int total = 0;
        for (int i = 0; i < 10; i++) {
            cout << i << "      " << global_hist[i] << endl;
            total += global_hist[i];
        }
        cout << "\nTotal number of elements = " << total << endl;
    }

    MPI_Finalize();
    return 0;
}

/*
mpic++ p18.cpp -o p18
mpirun -np 4 ./p18

Tree-based Algorithm : MPI_SUM
P0 এর local_hist = [0, 1, 2, 1, 0, 0, 0, 0, 0, 0]
index ০: P0(0) + P1(0) + P2(0) + P3(1) = 1
*/