// #include <iostream>
// #include <mpi.h>

// using namespace std;

// int main(int argc, char** argv) {
//     MPI_Init(&argc, &argv);

//     int rank, size;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     int N = 12;
//     int local_N = N / size;

//     int* full_array = nullptr;
//     if (rank == 0) {
//         full_array = new int[N]{10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
//     }

//     int local_array[3];

//     MPI_Scatter(full_array, local_N, MPI_INT, local_array, local_N, MPI_INT, 0, MPI_COMM_WORLD);

//     int right_boundary_sent = 0;
//     int left_boundary_received = 0;

//     if (rank < size - 1) {
//         right_boundary_sent = local_array[local_N - 1];
//         MPI_Send(&right_boundary_sent, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
//         cout << "P" << rank << ": Right boundary sent = " << right_boundary_sent << endl;
//     }

//     if (rank > 0) {
//         MPI_Recv(&left_boundary_received, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//         cout << "P" << rank << ": Left boundary received = " << left_boundary_received << endl;
//     }

//     MPI_Barrier(MPI_COMM_WORLD);

//     if (rank == 0) {
//         cout << "Boundary exchange completed successfully." << endl;
//     }

//     if (rank == 0) delete[] full_array;
//     MPI_Finalize();
//     return 0;
// }

// // mpic++ p1.cpp -o p1
// // mpirun -np 4 ./p1

// Parallel Processing Lab এর ১, ৬, ৯, ১২, ১৩, ১৪ ল্যাব এর জন্য করতে হবে না।