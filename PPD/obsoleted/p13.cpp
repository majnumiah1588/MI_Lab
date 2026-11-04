// #include <iostream>
// #include <mpi.h>
// using namespace std;

// int main(int argc, char** argv) {
//     MPI_Init(&argc, &argv);
//     int rank, size;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     int mat[5][3] = {{1,2,3}, {4,5,6}, {7,8,9}, {10,11,12}, {13,14,15}};
//     int rows[3] = {2, 2, 1}, displs_row[3] = {0, 2, 4};
//     int my_rows = rows[rank], local[6], final_arr[15];

//     if (rank == 0) {
//         for (int i = 1; i < size; i++) 
//             MPI_Send(mat[displs_row[i]], rows[i] * 3, MPI_INT, i, 0, MPI_COMM_WORLD);
//         for (int i = 0; i < 6; i++) local[i] = mat[0][i];
//     } else {
//         MPI_Recv(local, my_rows * 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//     }

//     for (int i = 0; i < my_rows * 3; i++) local[i] *= 2;

//     if (rank == 0) cout << "P0:\n";
//     for (int i = 0; i < my_rows; i++) {
//         for (int j = 0; j < 3; j++) cout << local[i * 3 + j] << " ";
//         cout << endl;
//     }

//     int counts[3] = {6, 6, 3};
//     int displs[3] = {0, 6, 12};

//     MPI_Gatherv(local, my_rows * 3, MPI_INT, final_arr, counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

//     if (rank == 0) {
//         cout << "Final matrix:\n";
//         for (int i = 0; i < 5; i++) {
//             for (int j = 0; j < 3; j++) cout << final_arr[i * 3 + j] << " ";
//             cout << endl;
//         }
//     }

//     MPI_Finalize();
//     return 0;
// }

// /*
// mpic++ p13.cpp -o p13
// mpirun -np 3 ./p13
// */

// // Parallel Processing Lab এর ১, ৬, ৯, ১২, ১৩, ১৪ ল্যাব এর জন্য করতে হবে না।