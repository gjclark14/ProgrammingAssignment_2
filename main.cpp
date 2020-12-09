#include <iostream>
#include "Utilities.cpp"
#include "mpi.h"

void sendReceiveMPIExample(int &argc, char** &argv);


int main(int argc, char** argv) {

    if(argc > 1) {
        if(std::string(argv[1]) == "-r") {
            Utilities::generateSparseMatrix("matrix0.txt");
            Utilities::generateSparseMatrix("matrix1.txt");
        } else if(std::string(argv[1]) == "-c") {
            Utilities::clearLoggedFiles();
        } else {
        }
    }
    // sendReceiveMPIExample(argc, argv);

    return 0;
}

void sendReceiveMPIExample(int &argc, char** &argv) {
    // reference to the int and the vector of strings
    MPI_Init(&argc, &argv);
    int rank; // A process is identified by its rank
    int size; // A size is a size

    //  MPI_COMM_WORLD is the default communicator whose group contains
    // all initial processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("I am rank %d of size %d \n", rank, size);

    int number = 69;
    printf("Process %d has number =%d\n", rank, number);
    if(rank == 0) {
        number = -1;
        printf("At rank %d, value of number is %d\n", rank, number);
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if(rank == 1) {
        printf("At rank %d, value of number is %d\n", rank, number);
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received number %d from process 0\n", rank, number);
    }
    MPI_Finalize();
}
