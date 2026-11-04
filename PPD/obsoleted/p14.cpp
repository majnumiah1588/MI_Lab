// #include <iostream>
// #include <algorithm>
// #include <mpi.h>
// using namespace std;

// int main(int argc, char** argv) {
//     MPI_Init(&argc, &argv);
//     int rank, size;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     int local_arr[3] = {10, 20, 30};
//     if (rank == 1) { local_arr[0]=40; local_arr[1]=50; local_arr[2]=60; }
//     if (rank == 2) { local_arr[0]=70; local_arr[1]=80; local_arr[2]=90; }
//     if (rank == 3) { local_arr[0]=100; local_arr[1]=110; local_arr[2]=120; }

//     int local_sum = local_arr[0] + local_arr[1] + local_arr[2];
//     int local_max = max(local_arr[0], max(local_arr[1], local_arr[2]));
//     int local_min = min(local_arr[0], min(local_arr[1], local_arr[2]));

//     int global_sum, global_max, global_min;

//     MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
//     MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
//     MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

//     // Lokal sums print (P0 theke shuru kore)
//     if (rank == 0) cout << "Local sums:" << endl;
//     MPI_Barrier(MPI_COMM_WORLD);
//     cout << "P" << rank << " = " << local_sum << endl;
//     MPI_Barrier(MPI_COMM_WORLD);

//     // Global result print
//     if (rank == 0) {
//         cout << "Global Sum       = " << global_sum << endl;
//         cout << "Global Maximum   = " << global_max << endl;
//         cout << "Global Minimum   = " << global_min << endl;
//         cout << "Average          = " << (double)global_sum / (size * 3) << endl;
//     }

//     MPI_Finalize();
//     return 0;
// }

// /*
// mpic++ p14.cpp -o p14
// mpirun -np 4 ./p14
// */

// // Parallel Processing Lab এর ১, ৬, ৯, ১২, ১৩, ১৪ ল্যাব এর জন্য করতে হবে না।