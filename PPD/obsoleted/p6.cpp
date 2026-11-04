// #include <iostream>
// #include <mpi.h>
// using namespace std;

// int main(int argc, char** argv) {
//     MPI_Init(&argc, &argv);
//     int rank, size;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     int N = 8, local_N = N / size;
//     int full_array[8] = {10, 20, 30, 40, 50, 60, 70, 80};
//     int local_array[2];

//     MPI_Scatter(full_array, local_N, MPI_INT, local_array, local_N, MPI_INT, 0, MPI_COMM_WORLD);

//     int left_halo = 0, right_halo = 0;

//     if (rank > 0) {
//         MPI_Recv(&left_halo, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//         MPI_Send(&local_array[0], 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD);
//     }
    
//     if (rank < size - 1) {
//         MPI_Recv(&right_halo, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//         MPI_Send(&local_array[local_N - 1], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
//     }

//     cout << "P" << rank << ": ";
//     if (rank > 0) cout << "left halo " << left_halo << ", ";
//     cout << "[";
//     for (int i = 0; i < local_N; i++) cout << local_array[i] << (i == local_N - 1 ? "" : " ");
//     cout << "]";
//     if (rank < size - 1) cout << ", right halo " << right_halo;
//     cout << endl;

//     MPI_Barrier(MPI_COMM_WORLD);
//     if (rank == 0) cout << "Stencil result (using the stated simple boundary convention):\n   ";

//     for (int i = 0; i < local_N; i++) {
//         int left_val = (i == 0) ? left_halo : local_array[i - 1];
//         int right_val = (i == local_N - 1) ? right_halo : local_array[i + 1];
//         cout << left_val + local_array[i] + right_val << " ";
//     }
    
//     MPI_Barrier(MPI_COMM_WORLD);
//     if (rank == size - 1) cout << endl;

//     MPI_Finalize();
//     return 0;
// }

// // Parallel Processing Lab এর ১, ৬, ৯, ১২, ১৩, ১৪ ল্যাব এর জন্য করতে হবে না।