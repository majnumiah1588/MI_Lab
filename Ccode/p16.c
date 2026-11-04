// Problem_16: Each MPI process stores a local value. Use MPI_Scan to calculate prefix sum
// across all processes and compare it with a manually implemented point-to-point solution.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int val = (rank + 1) * 10, scan_res = 0, manual_res = 0;
    int i;

    // ---- Manual point-to-point prefix sum ----
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




    // ---- MPI_Scan prefix sum ----
    MPI_Barrier(MPI_COMM_WORLD);
    double start_scan = MPI_Wtime();

    MPI_Scan(&val, &scan_res, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end_scan = MPI_Wtime();

    printf("P%d: Local value = %d, Prefix sum = %d\n", rank, val, scan_res);

    // ---- Print prefix result in rank order ----
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) printf("\nPrefix result: ");

    for (i = 0; i < size; i++) {
        if (rank == i) {
            printf("%d ", manual_res);
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (rank == size - 1) printf("\n\n");

    if (rank == 0) {
        double t_manual = end_manual - start_manual; 
        double t_scan = end_scan - start_scan; 
        printf("Manual Point-to-Point time = %f s\n", t_manual);
        printf("MPI_Scan time              = %f s\n", t_scan);
        printf("Better method              = %s\n",
               (t_scan < t_manual ? "MPI_Scan" : "Manual"));
    }

    MPI_Finalize();
    return 0;
}

/*
mpicc p16.c -o p16
mpirun -np 4 ./p16
*/