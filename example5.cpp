// MPI example that will show how to broadcast an entire array to a set of nodes

// includes
#include <iostream>
#include <mpi.h>

// the master process in this group
void master(void)
{
  // allocate memory for an array of numbers
  int *array = new int[10];

  // give the array a set of numbers
  for (unsigned int i = 0; i < 10; i++)
    array[i] = i;

  // broadcast the set of numbers to all the other processes
  MPI_Bcast(array, 10, MPI_INT, 0, MPI_COMM_WORLD);
  std::cout << "rank 0 broadcasted to all processes" << std::endl;

  // delete the memory for the array
  delete[] array;
}

// the slave processes in this group
void slave(const int rank)
{
  // allocate memory for an array of numbers
  int *array = new int[10];

  // take part in the broadcast to get the array of numbers and print out the array of numbers
  MPI_Bcast(array, 10, MPI_INT, 0, MPI_COMM_WORLD);
  std::cout << "rank " << rank << " recieved numbers: ";
  for (unsigned int i = 0; i < 10; i++)
    std::cout << array[i] << ", ";
  std::cout << std::endl;

  // delete the memory for the array
  delete[] array;
}

// entry point into our program
int main(int argc, char **argv)
{
  // initialise MPI
  MPI_Init(NULL, NULL);

  // get the size of the world and our rank in the world
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // rank 0 is the master all others are slaves
  if (rank == 0)
    master();
  else
    slave(rank);

  // finalise MPI and return control to the OS
  MPI_Finalize();
  return 0;
}