#include <iostream>
#include <mpi.h>

int main(int argc, char **argv){
    MPI_Init(NULL, NULL);

    int world_size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::cout << "rank, world size: " << rank << ", " << world_size << std::endl;

  // finalise MPI. this is the last thing that must happen before the end of the mpi program
  MPI_Finalize();

  // all is good at this point so return true
  return 0;

}
