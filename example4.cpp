// example MPI program that will show how the broadcast function works

// includes
#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
  // initialise MPI
  MPI_Init(NULL, NULL);

  // get the size of the world and our rank
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // try a broadcast but init the value in the master only
  if (rank == 0)
  {
    int message = 16;
    MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);
    std::cout << "rank 0 broadcast message of: " << message << std::endl;
  }
  else
  {
    int recieve = 0;
    MPI_Bcast(&recieve, 1, MPI_INT, 0, MPI_COMM_WORLD);
    std::cout << "rank " << rank << " recieved broadcast message of " << recieve << std::endl;
  }

  // finalise MPI and return to the OS
  MPI_Finalize();
  return 0;
}