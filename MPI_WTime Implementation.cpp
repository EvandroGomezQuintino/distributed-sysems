// Includes
#include <iostream>
#include <mpi.h>

// Prime counter function
int primeCounter(int n){
  int count = 0;
  for (int a=2; a<n; a++){
    bool isPrime = true;
    for(int c=2; c*c <= a; c++){
      if (a % c == 0) {
        isPrime = false;
  	break;
      }
    }
    if(isPrime) count++;

  }

  return count;
}


// entry point into our program
int main(int argc, char **argv){
     // initialise MPI
    MPI_Init(NULL, NULL);
    
    // variable declaration
    int size, rank;
    int counter1, counter2, counter3, counter4;
    double starttime, endtime;
    double time1, time2, time3, time0;
    
    int *total = new int[4];

    // get access to the size of the world and the rank we have
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
     if(rank == 0){
         starttime = MPI_Wtime();
         counter1 = primeCounter(25^(rank + 2));
         endtime = MPI_Wtime();
         // compute time taken to complete the process
         time0 = endtime - starttime;
         std::cout << "rank 0 has " << counter1 << " Prime Number(s)" << std::endl;
         std::cout << "rank 0 finished in " << time0 << " seconds" << std::endl;
         
         std::cout << "rank 1 finished in " << time1 << " seconds" << std::endl;
         
         std::cout << "rank 2 finished in " << time2 << " seconds" << std::endl;
         
         std::cout << "rank 3 finished in " << time3 << " seconds" << std::endl;
     }
     
     // for second rank
     if(rank == 1){
         starttime = MPI_Wtime();
         counter2 = primeCounter(25^(rank + 2));
         // compute time taken to complete the process
         time1 = endtime - starttime;
         std::cout << "rank 1 has " << counter2 << " Prime Number(s)" << std::endl;
         
     }
     
      // for third rank
     if(rank == 2){
         counter3 = primeCounter(25^(rank + 2));
         // compute time taken to complete the process
         time2 = endtime - starttime;
         std::cout << "rank 2 has " << counter3 << " Prime Number(s)" << std::endl;

     }
     
      // for fourth rank
     if(rank == 3){
         starttime = MPI_Wtime();
         counter4 = primeCounter(25^(rank + 2));
         // compute time taken to complete the process
         time3 = endtime - starttime;
         std::cout << "rank 3 has " << counter4 << " Prime Number(s)" << std::endl;
         
     }
     
     // finalise MPI and return control to the OS
    MPI_Finalize();
    // display name and student number
    std::cout << "Name: Thohiru Omoloye " << std::endl;
    std::cout << "Student Number: 2950574 " << std::endl;
    return 0;


}
