// Includes
#include <iostream>
#include <mpi.h>

int worldsWorstworldsWorstHash1 (int ip) {
	for (int l=1; l<100; l++) {
		ip = ip * l % 254;
	}
	return ip;
}

int worldsWorstworldsWorstHash2 (int ip) {
	for (int l=1; l<50; l++) {
		ip = ((ip * l) + 2) % 254;
	}
	return ip;
}

// entry point to our program
int main(int argc, char **argv) {
    // initialise MPI
    MPI_Init(NULL, NULL);

    // get access to the size of the world and the rank we have
    int size, rank, hashInput;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
    // one process is the master and the other are slaves
    if(rank == 0){
        hashInput = worldsWorstworldsWorstHash1(233) - worldsWorstworldsWorstHash2(422);

        // Send the difference of the hash input to others
        MPI_Send(&hashInput, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "rank 0 send " << hashInput << " to process 1" << std::endl;

        hashInput = worldsWorstworldsWorstHash1(hashInput) - worldsWorstworldsWorstHash2(hashInput);
        // Send the difference of the hash input to next process
        MPI_Send(&hashInput, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        std::cout << "rank 0 send " << hashInput << " to process 2" << std::endl;

        hashInput = worldsWorstworldsWorstHash1(hashInput) - worldsWorstworldsWorstHash2(hashInput);
        // Send the difference of the hash input to next process
        MPI_Send(&hashInput, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        std::cout << "rank 0 send " << hashInput << " to process 3" << std::endl;

        // printing out the last 2 hash values
        std::cout << "\nworldsWorstHash1(HashInput3): " << worldsWorstworldsWorstHash1(hashInput) << std::endl;
        std::cout << "worldsWorstHash2(HashInput3): " << worldsWorstworldsWorstHash2(hashInput) << "\n" << std::endl;
    }else{
        MPI_Recv(&hashInput, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "rank " << rank << " recieved Hash: " << hashInput << std::endl;
    }
  
    // finalise MPI and return control to the OS
    MPI_Finalize();
    // display name and student number
    std::cout << "Name: Thohiru Omoloye " << std::endl;
    std::cout << "Student Number: 2950574 " << std::endl;
    return 0;
}
