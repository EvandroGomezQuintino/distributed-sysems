#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>
#include <assert.h>

/**
 * Sum method that takes a reference to an array and array size
 * @param array pointer to arry
 * @param int array_size Side of the array
 *
 * @return Sum of the array
 **/
int sum(const float *array, const int array_size){
	int sum = 0;
	for (unsigned int i = 0; i < array_size; i++)
    	sum+= array[i];

	return sum;
}

/**
 * Sum differences function
 * @param array array pointer
 * 
 * @param int array_size size of the array
 * 
 * @param int overall_mean Overall mean of the dataset
 * 
 * @return int 
 **/
int sumDifferences(const float *array, const int array_size, const int overall_mean){

	int sumOfDifference = 0;
	for (unsigned int i = 0; i < array_size ; i++)
    	sumOfDifference += pow(array[i] - overall_mean, 2);

	return sumOfDifference;						
}


/**
 * printing out an array to console
 * @param array array pointer
 * 
 * @param int array_size side the array
 **/
void printArray(const float *array, const int array_size) {
	std::cout << "Printing array to console ";
	for(unsigned int i = 0; i < array_size; i++)
		std::cout << array[i] << ", ";
	std::cout << std::endl;
}

/**
 * Create random numbers
*/
float *generate_rand_nums(int num_elements) {
	float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
	assert(rand_nums != NULL);
	int i;
	for (i = 0; i < num_elements; i++)
		rand_nums[i] = rand() % 50 + 1;	// random numbers from 1 to 50

	return rand_nums;
}


// entry point into our program
int main(int argc, char **argv) {
	// initialise MPI
	MPI_Init(NULL, NULL);
	
	// get the size of the world and our rank in the world
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int num_elements_per_proc = atoi(argv[1]);
	srand(time(NULL));
	float *rand_nums = NULL;
	float standard_deviation = 0;
	float overall_differences = 0;

	
	// determine whose master and slave
	if(rank == 0){
		int partition_size = size;
		rand_nums = generate_rand_nums(num_elements_per_proc * size);		// generate random numbers
		// broadcast the partition size to all nodes
		// MPI_Bcast(&partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	}

	float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
  	assert(sub_rand_nums != NULL);

	// scatter the partitions to each node
	MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums, num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

	// int partition_size_recv = 0;
    // MPI_Bcast(&partition_size_recv, 1, MPI_INT, 0, MPI_COMM_WORLD);
	// std::cout << "Rank: " << rank << " received partition size: " << partition_size_recv << " from master" << std::endl;

	float overall_mean = 0;
	// calculate mean for this node
  	float sub_mean = sum(sub_rand_nums, num_elements_per_proc);
	// reduce the sum
	MPI_Reduce(&sub_mean, &overall_mean, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		// compute the overall average
		float overall_average = overall_mean/ (size * num_elements_per_proc);
		std::cout << "Overall Average: " << overall_average << std::endl;

		// Broadcast the overrall average to all nodes
		MPI_Bcast(&overall_average, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	}

	

	// Receive the overall average from cordinator
	float overall_average_recv = 0;
    MPI_Bcast(&overall_average_recv, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

	// compute the differences
	float differences = sumDifferences(sub_rand_nums, num_elements_per_proc, overall_average_recv);

	// Reduce the differences by SUM operation
	MPI_Reduce(&differences, &overall_differences, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		// print out the dataset
		printArray(rand_nums, size * num_elements_per_proc);

		//print the overrall mean
		std::cout << "Overall Mean: " << overall_mean << std::endl;

		// compute the standard deviation
		standard_deviation = overall_differences / (num_elements_per_proc * size);
		std::cout << "Standard Deviation: " << standard_deviation << std::endl;

		
	}

	
	delete[] rand_nums;
	delete[] sub_rand_nums;
	// finalise MPI and return control to the OS
	MPI_Finalize();
	return 0;  
}