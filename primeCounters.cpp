// includes
#include <iostream>
#include <mpi.h>
#include <math.h>


// Prime counter function
int primeCounter(int n) {

	int count=0;

    for (int a=2 ; a < n ; a++) {

        bool isPrime = true;

        for (int c=2 ; c*c <= a ; c++) {

            if(a % c == 0) {

                isPrime = false;

                break;

            }

        }

        if(isPrime) count++;

    }

	return count;

}

double run(const int rank){
    double starttime, endtime;
     starttime = MPI_Wtime();	// start time
     primeCounter(pow(25, (rank + 2)));
     endtime = MPI_Wtime();		// end time
     // compute time taken to complete the process
     return endtime - starttime;
}

// used for printing out an array to console
void printArray(const double *array, const int rank, const int array_size) {
	// print Name and processor if rank is 0
	if (rank == 0){
		std::cout << "Name: Farida Umar" << std::endl;		
		std::cout << "No.: 2882624" << std::endl;		
		std::cout << "uP: Your processor info" << std::endl;		
	}
}

// method that will be used by the master process
void master(const int world_size) {
	// allocate arrays for a set of data to gather and the overall gathered data
	double *send = new double[1];
	double *recieve = new double[world_size];
  
	// populate our data to send and state what we are sending
    send[0] = run(0);
  	printArray(send, 0, 1);
  
  	// take part in the gather to get all of the prime count from the nodes
  	MPI_Gather(send, 1 , MPI_DOUBLE, recieve, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  	// print out the durations gathered
  	std::cout << "Results: ";
  	for(unsigned int i = 0; i < world_size; i++)
    	std::cout << recieve[i] << ", ";
	std::cout << std::endl;  
  
  	// deallocate arrays
	delete[] send;
	delete[] recieve;
}

// method that will be used by the slave process
void slave(const int rank, const int world_size) {
	// allocate an array for a set of data to send
	double *send = new double[1];

	// populate our data to send and state what we are sending
  
    send[0] = run(rank);
	printArray(send, rank, 1);
  
	// take part in the gather to get all of the integers from the nodes
	MPI_Gather(send, 1, MPI_DOUBLE, NULL, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);  
  
	// deallocate the array
	delete[] send;
}

// entry point into our program
int main(int argc, char **argv) {
	// initialise MPI
	MPI_Init(NULL, NULL);
  
	// get the size of the world and our rank in the world
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
	// determine whose master and slave
	if(rank == 0)
    	master(size);
  	else
    	slave(rank, size);
  
  	// finalise MPI and return control to the OS
	MPI_Finalize();
	return 0;  
}
