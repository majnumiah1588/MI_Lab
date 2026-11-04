#include <iostream>
#include <cstdlib>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int msg_size = (argc > 1) ? atoi(argv[1]) : 1;

    int* send_buf = new int[size * msg_size];
    int* recv_buf = new int[size * msg_size];

    for (int i = 0; i < size * msg_size; i++) send_buf[i] = (rank * 10) + i;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    
    MPI_Alltoall(send_buf, msg_size, MPI_INT, recv_buf, msg_size, MPI_INT, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    double time_taken = MPI_Wtime() - start;

    if (rank == 0) {
        cout << "P = " << size << ", Msg Size = " << msg_size 
             << ", Time = " << time_taken << " s" << endl;
    }

    delete[] send_buf;
    delete[] recv_buf;
    MPI_Finalize();
    return 0;
}

/*
mpic++ p17.cpp -o p17
mpirun -np 4 ./p17 1000    1000 for msg_size , argv[0]="./p17", argv[1]="1000"  
this part for argv(argument vector)=["./p17", "1000"]
argc(argument count)=len(argv)
*/