// Problem_18: Divide dataset among MPI processes. Each process generates a local histogram,
// and MPI_Allreduce combines all local histograms into a global histogram.
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rnk, npr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    MPI_Comm_size(MPI_COMM_WORLD, &npr);

    int dat[16] = {1,2,2,3, 2,3,4,4, 1,4,5,5, 0,2,5,9};
    int lh[10] = {0};      /* local hist  */
    int gh[10];            /* global hist */

    /* 1. this rank counts only its own share */
    for (int i = rnk; i < 16; i += npr)
        lh[dat[i]]++;

    /* 2. combine all local histograms into gh on every rank */
    MPI_Allreduce(lh, gh, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    /* 3. rank 0 prints */
    if (rnk == 0) {
        int tot = 0;
        printf("Value  Frequency\n");
        for (int b = 0; b < 10; b++) {
            printf("%d      %d\n", b, gh[b]);
            tot += gh[b];
        }
        printf("\nTotal = %d\n", tot);
    }

    MPI_Finalize();
    return 0;
}

/*
mpicc p18.c -o p18
mpirun -np 4 ./p18

Tree-based Algorithm : MPI_SUM
P0 er local_hist = [0, 1, 2, 1, 0, 0, 0, 0, 0, 0]
index 0: P0(0) + P1(0) + P2(0) + P3(1) = 1
*/