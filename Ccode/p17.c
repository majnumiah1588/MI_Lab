// Problem_17: Each MPI process generates data for every other process.
// Implement using MPI_Alltoall and analyze communication cost vs number of processes.
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MXP 8

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rnk, npr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    MPI_Comm_size(MPI_COMM_WORLD, &npr);

    int msg = (argc > 1) ? atoi(argv[1]) : 1;

    int snd[MXP * msg], rcv[MXP * msg];

    for (int idx = 0; idx < npr * msg; idx++)
        snd[idx] = rnk * 10 + idx;

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();
    MPI_Alltoall(snd, msg, MPI_INT, rcv, msg, MPI_INT, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    double dt = MPI_Wtime() - t0;

    if (rnk == 0)
        printf("P = %d, Msg/process = %d, Time = %f s\n", npr, msg, dt);

    MPI_Finalize();
    return 0;
}

// mpicc p17.c -o p17
// mpirun -np 4 ./p17          # default msg = 1
// mpirun -np 4 ./p17 1000     # msg = 1000