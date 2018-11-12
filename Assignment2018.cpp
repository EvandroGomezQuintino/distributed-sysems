/* 

You are given two hash functions (at the bottom of this document).
You are required to create two different mpi programs.
Print your name and student number to the console for all programs you write.

Part 1 (50%)
---

First a sequential ring of 4 nodes...
The initial node (rank 0) uses values 233 and 422 and passes the difference of the two hashes to the next node.

Like so:

HashInput1 = worldsWorstHash1(23) - worldsWorstHash2(42)
HashInput2 = worldsWorstHash1(HashInput1) - worldsWorstHash2(HashInput1)
HashInput3 = worldsWorstHash1(HashInput2) - worldsWorstHash2(HashInput2)

Print the final two hash values worldsWorstHash1(HashInput3) and worldsWorstHash2(HashInput3) to the console.

Note: Any time a message is sent, output the message to the console.


Part 2 (50%)
---

Create a simple arrangement of four nodes:

         +---+
         | 0 |
  +------+---+-------+
  |                  |
  |                  |
+-v--+            +--v-+
| 1  |            |  3 |
+--+-+            +--+-+
  |                  |
  |      +---+       |
  +----->| 2 |<------+
         +-+-+
           |
           |
           +-->

Your program sends the combined hash values worldsWorstHash1(-4224)+worldsWorstHash2(52253) from rank 0 to 1 and 3. (OUTPUT THIS TO CONSOLE)
Rank 1 then sends worldsWorstHash1(valueFromStep1)+worldsWorstHash2(valueFromStep1) to 2. (OUTPUT TO CONSOLE)
Rank 3 sends worldsWorstHash1(valueFromStep1)-worldsWorstHash2(valueFromStep1) to 2. (OUTPUT TO CONSOLE)
Rank 2 sums the two values from 1 and 3. (OUTPUT THIS TO CONSOLE)

(BONUS POINTS) You don't have to, however if you can, try to use groups. http://mpitutorial.com/tutorials/introduction-to-groups-and-communicators/

*/
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

// the master process in this group
void master(void) {
  // variable declarations
  int *hashInput = new int[3];
  // int HashInput1, HashInput2, HashInput3;
  hashInput[0] = worldsWorstworldsWorstHash1(233) - worldsWorstworldsWorstHash2(422);

  // Send the difference of the hash input to others
  MPI_Send(hashInput, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  std::cout << "rank 0 send " << hashInput[0] << " to process 1" << std::endl;

  hashInput[1] = worldsWorstworldsWorstHash1(hashInput[0]) - worldsWorstworldsWorstHash2(hashInput[0]);
  MPI_Send(hashInput, 2, MPI_INT, 2, 0, MPI_COMM_WORLD);
  std::cout << "rank 0 send " << hashInput[1] << " to process 2"  << std::endl;

  hashInput[2] = worldsWorstworldsWorstHash1(hashInput[1]) - worldsWorstworldsWorstHash2(hashInput[1]);
  MPI_Send(hashInput, 3, MPI_INT, 3, 0, MPI_COMM_WORLD);
  std::cout << "rank 0 send " << hashInput[2] << " to process 3" << std::endl;

  // printing out the last 2 hash values
  std::cout << "worldsWorstHash1(HashInput3): " << worldsWorstworldsWorstHash1(hashInput[2]) << std::endl;
  std::cout << "worldsWorstHash2(HashInput3): " << worldsWorstworldsWorstHash2(hashInput[2]) << std::endl;
}

// the slave processes in this group
void slave(const int rank) {
  // get memory for the hash
  int *hashInput = new int[3];
  
  // recieve a hash number from the master and print them to console
  MPI_Recv(hashInput, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  std::cout << "rank " << rank << " recieved Hash: " << hashInput[rank - 1] << std::endl;
  
  // delete the memory for the array
  delete[] hashInput;
}

// entry point to our program
int main(int argc, char **argv) {
  // initialise MPI
  MPI_Init(NULL, NULL);

  // display name and student number
  std::cout << "Name: Thohiru Omoloye " << std::endl;
  std::cout << "Student Number: 2950574 " << std::endl;
  
  // get access to the size of the world and the rank we have
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  // one process is the master and the other are slaves
  if(rank == 0)
    master();
  else
    slave(rank);
  
  // finalise MPI and return control to the OS
  MPI_Finalize();
  return 0;
}