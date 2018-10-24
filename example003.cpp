// MPI example that will show to send a number of items using send and receive

// Includes
#include <iostream>
#include <mpi.h>

// method that will act as the master for this program
void master(void)
{

    // get the memory for 10 integers
    int *array = new int[10];

    //fill the array with values and send to the slave process
    for (unsigned int i = 0; i < 10; i++)
        array[i] = i;
    MPI_Send(array, 10, MPI_INT, 1, 0, MPI_COMM_WORLD);
    std::cout << "master sent to process 1" << std::endl;

    // fill the array again and send to the second
    for (unsigned int i = 0; i < 10; i++)
        array[i] = i + 10;
    MPI_Send(array, 10, MPI_INT, 2, 0, MPI_COMM_WORLD);
    std::cout << "master sent to process 2" << std::endl;

    // fill the array again and send to the third
    for (unsigned int i = 0; i < 10; i++)
        array[i] = i + 20;
    MPI_Send(array, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);
    std::cout << "master sent to process 3" << std::endl;

    // delete the memory for the array
    delete[] array;
}

// method that will act as slave for this program
void slave(int rank)
{
    // get memory for 10 integers
    int *array = new int[10];

    // receive a set of numbers from the master and print them to console
    MPI_Recv(array, 10, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "rank " << rank << " received numbers: ";
    for (unsigned int i = 0; i < 10; i++)
        std::cout << array[i] << ", ";
    std::cout << std::endl;

    // delete the memory for slave
    delete[] array;
}

int main(int argc, char **argv)
{
    // initialize MPI
    MPI_Init(NULL, NULL);

    // Get the size of the rank
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // process zero is the master all other processes are slave
    if (rank == 0)
        master();
    else
        slave(rank);

    // Finalize MPI and return to OS
    MPI_Finalize();
    return 0;
}