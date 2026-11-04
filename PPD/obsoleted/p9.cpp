// #include <iostream>
// #include <algorithm>
// #include <mpi.h>
// using namespace std;

// int main(int argc, char** argv) {
//     MPI_Init(&argc, &argv);
//     int rank, size;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     int full_arr[8] = {45, 12, 78, 3, 56, 23, 89, 10};
//     int local[2], recv[2], send[2];

//     MPI_Scatter(full_arr, 2, MPI_INT, local, 2, MPI_INT, 0, MPI_COMM_WORLD);
//     sort(local, local + 2);

//     cout << "P" << rank << ": " << local[0] << " " << local[1] << endl;

//     MPI_Barrier(MPI_COMM_WORLD);
//     if (rank == 0) cout << "After data exchange and merge/exchange steps:\n";

//     for (int step = 0; step < size; step++) {
//         if ((step % 2 == 0 && rank % 2 == 0 && rank < size - 1) || (step % 2 == 1 && rank % 2 == 1 && rank < size - 1)) {
//             MPI_Send(local, 2, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
//             MPI_Recv(recv, 2, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//             if (local[0] > recv[0]) {
//                 send[0] = local[0]; send[1] = local[1];
//                 local[0] = recv[0]; local[1] = recv[1];
//                 MPI_Send(send, 2, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
//             } else {
//                 MPI_Send(local, 2, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
//             }
//         } else if ((step % 2 == 0 && rank % 2 == 1) || (step % 2 == 1 && rank % 2 == 0 && rank > 0)) {
//             MPI_Recv(recv, 2, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//             MPI_Send(local, 2, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
//             MPI_Recv(local, 2, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//         }
//         sort(local, local + 2);
//     }

//     int sorted[8];
//     MPI_Gather(local, 2, MPI_INT, sorted, 2, MPI_INT, 0, MPI_COMM_WORLD);

//     if (rank == 0) {
//         cout << "Sorted array: ";
//         for (int i = 0; i < 8; i++) cout << sorted[i] << " ";
//         cout << endl;
//     }

//     MPI_Finalize();
//     return 0;
// }

// // Parallel Processing Lab এর ১, ৬, ৯, ১২, ১৩, ১৪ ল্যাব এর জন্য করতে হবে না।