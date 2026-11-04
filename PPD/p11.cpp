#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int iter = 0, thresh = 0, block = 0;
    if (rank == 0) {
        cout << "Root process reads:\n";    iter = 1000, thresh = 50, block = 256;
    }
    MPI_Bcast(&iter, 1, MPI_INT, 0, MPI_COMM_WORLD);    
    MPI_Bcast(&thresh, 1, MPI_INT, 0, MPI_COMM_WORLD);  
    MPI_Bcast(&block, 1, MPI_INT, 0, MPI_COMM_WORLD);
    cout << "P" << rank << " received: Iterations = " << iter << ", Threshold = " << thresh << ", Block size = " << block << endl;
    MPI_Finalize();
    return 0;
}
// mpic++ p11.cpp -o p11
// mpirun -np 4 ./p11