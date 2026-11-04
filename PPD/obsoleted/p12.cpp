// #include <iostream>
// #include <mpi.h>
// using namespace std;

// int main(int argc, char** argv) {
//     MPI_Init(&argc, &argv);
//     int rank, size;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     int full[8] = {1, 2, 3, 4, 5, 6, 7, 8};
//     int local[2], result[2], gathered[8];

//     MPI_Scatter(full, 2, MPI_INT, local, 2, MPI_INT, 0, MPI_COMM_WORLD);

//     if (rank == 0) cout << "After MPI_Scatter:\n";
//     cout << "P" << rank << ": " << local[0] << " " << local[1] << endl;
//     MPI_Barrier(MPI_COMM_WORLD);

//     for (int i = 0; i < 2; i++) result[i] = local[i] * local[i];
//     if (rank == 0) cout << "\nOperation: y[i] = x[i]^2\n\nSample Output\n";

//     cout << "P" << rank << ": " << result[0] << " " << result[1] << endl;

//     MPI_Gather(result, 2, MPI_INT, gathered, 2, MPI_INT, 0, MPI_COMM_WORLD);

//     if (rank == 0) {
//         cout << "\nGathered result: ";
//         for (int i = 0; i < 8; i++) cout << gathered[i] << " ";
//         cout << endl;
//     }

//     MPI_Finalize();
//     return 0;
// }
// /*
// mpic++ p12.cpp -o p12
// mpirun -np 4 ./p12
// */

// // Parallel Processing Lab এর ১, ৬, ৯, ১২, ১৩, ১৪ ল্যাব এর জন্য করতে হবে না।