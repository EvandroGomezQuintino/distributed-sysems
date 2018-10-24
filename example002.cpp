
// includes
#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{

    // initialize mpi
    MPI_Init(NULL, NULL);

    // Get access to the size and our rank
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // if we are rank 0 then send an integer to another three processes the others should just receive

    int message;
    if (rank == 0)
    {
        // send a message to three other processes

        message = 16;
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "rank 0 sending to rank 1" << std::endl;
        message = 32;
        MPI_Send(&message, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        std::cout << "rank 0 sending to rank 2" << std::endl;
        message = 48;
        MPI_Send(&message, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        std::cout << "rank 0 sending to rank 3" << std::endl;
    }
    else
    {
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "rank " << rank << " received message of " << message << std::endl;
    }

    // finalise MPI and return to the OS
    MPI_Finalize();
    return 0;
}
