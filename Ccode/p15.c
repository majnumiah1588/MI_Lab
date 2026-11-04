// Problem_15: Trapezoidal rule for numerical integration.
// Each MPI process calculates a portion; MPI_Reduce combines partial results.
#include <stdio.h>
#include <mpi.h>

double f(double x) { return x * x; }

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double a = 0.0, b = 1.0, h = (b - a) / 1000.0, ls = 0.0,gs;
    // Round-robin distribution of interior points
    for (int i = rank + 1; i < 1000; i += size) ls += f(a + i * h);
    MPI_Reduce(&ls, &gs, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); //MPI_Reduce(send,store,count,datatype,operation,receiverRank(root), MPI_Comm);

    if (rank == 0) {
        double result = (gs * 2 + f(a) + f(b)) * h / 2.0;
        double exact = 1.0 / 3.0;
        printf("Numerical integral ~ %f\n", result); 
        printf("Exact integral     = %f\n", exact);
        printf("Absolute error     ~ %f\n", (result > exact ? result - exact : exact - result));
    }

    MPI_Finalize();
    return 0;
}

/*
mpicc p15.c -o p15
mpirun -np 4 ./p15
Round-Robin Distribution  : for (i = rank + 1; i < 1000; i += size)
*/