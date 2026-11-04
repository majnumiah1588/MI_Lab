#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int val = (rank + 1) * 10, scan_res = 0, manual_res = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    double start_manual = MPI_Wtime();

    if (rank == 0) {
        manual_res = val;
        if (size > 1) MPI_Send(&manual_res, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&manual_res, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        manual_res += val;
        if (rank < size - 1) MPI_Send(&manual_res, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end_manual = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    double start_scan = MPI_Wtime();

    MPI_Scan(&val, &scan_res, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end_scan = MPI_Wtime();

    cout << "P" << rank << ": Local value = " << val 
         << ", Prefix sum = " << scan_res << endl;

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) cout << "\nPrefix result: ";
    
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            cout << manual_res << " ";
            cout.flush();
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (rank == size - 1) cout << "\n\n";

    if (rank == 0) {
        double t_manual = end_manual - start_manual;
        double t_scan = end_scan - start_scan;
        cout << "Manual Point-to-Point time = " << t_manual << " s" << endl;
        cout << "MPI_Scan time              = " << t_scan << " s" << endl;
        cout << "Better method              = " 
             << (t_scan < t_manual ? "MPI_Scan" : "Manual") << endl;
    }

    MPI_Finalize();
    return 0;
}

/*
mpic++ p16.cpp -o p16
mpirun -np 4 ./p16
*/