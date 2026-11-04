#include <iostream>
#include <mpi.h>
using namespace std;

double f(double x) { return x * x; }

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.0, b = 1.0, h = (b - a) / 1000.0, local_sum = 0.0;

    for (int i = rank + 1; i < 1000; i += size) local_sum += f(a + i * h);

    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double result = (global_sum * 2 + f(a) + f(b)) * h / 2.0;
        double exact = 1.0 / 3.0;
        cout << "Numerical integral ≈ " << result << endl;
        cout << "Exact integral     = " << exact << endl;
        cout << "Absolute error     ≈ " << (result > exact ? result - exact : exact - result) << endl;
    }

    MPI_Finalize();
    return 0;
}


/*
mpic++ p15.cpp -o p15
mpirun -np 4 ./p15
Round-Robin Distribution  : for (int i = rank + 1; i < 1000; i += size)
*/