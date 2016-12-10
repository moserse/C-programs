#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* minimum and maximum duration of use for an allocated block of memory */
#define MIN_DURATION      3
#define MAX_DURATION     25

/* minimum and maximum allocation request size */
#define MIN_REQUEST_SIZE    3
#define MAX_REQUEST_SIZE  100

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  number of times to repeat each experiment (ie. number of runs).  The
  third parameter is the duration of the each simulation run. The
  forth parameter is a random number seed. Here is an example command
  line:

  ./hw7 10000 100 2000 1234

  This means that your program should initialize physical memory to
  10,000 units, perform 100 runs with each run taking 2000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1234.
*/


int main(int argc, char** argv){


  int i, j, duration, size; //randomly generated
  int arg2, arg3, seed;  //command arguments soon to be converted
  unsigned int memsize;

  int probe;

  int numProbes, allocFails, totalFrags; //ints that i'm gonna print

  // STRTOL(3) converts arguments into ints
  memsize = strtol(argv[1], NULL, 10);
  arg2 = strtol(argv[2], NULL, 10);
  arg3 = strtol(argv[3], NULL, 10);
  seed = strtol(argv[4], NULL, 10);

  numProbes = 0;
  allocFails = 0;
  totalFrags = 0;


  //initializing memory with size of cmd arg1
  mem_init(memsize);

  srand(seed); //random num init

  printf("\nAllocation simulations: first fit, next fit, best fit\n");
  //printf("\nArguments for this run:\n");
  //printf(" mem size:  %d\n times to repeat: %d\n duration: %d\n seed: %d\n", memsize, arg2, arg3, seed);

  printf("Beginning process...\n\n");

  //did not use strategies in a loop like i could have...

 




                                           //FIRST FIT
  mem_strategy_t first = FIRST; 

  for (i = 0; i < arg2; i++){
    for (j = 0; j < arg3; j++){

      duration = rand() % (25 + 1 - 3) + 3;  //random numbers for size and duration
      size = rand() % (100 + 1 - 3) + 3;    
     
      probe = mem_allocate(first,size,duration);

      if (probe >= 1) numProbes += probe;
      else allocFails++;

      mem_single_time_unit_transpired();
      
    }
  
    totalFrags += mem_fragment_count(50);
    
  }

  printf("FIRST FIT: \n");
  printf("avg num probes = %d\navg alloc fails = %d\n", (numProbes/arg2) , (allocFails/arg2));
  printf("avg num fragments = %d\n\n", (totalFrags/arg2));

  numProbes = 0;
  allocFails = 0;
  totalFrags = 0;

  mem_clear();






                                           //NEXT FIT
  mem_strategy_t next = NEXT;

  for (i = 0; i < arg2; i++){
    for (j = 0; j < arg3; j++){

      duration = rand() % (25 + 1 - 3) + 3;
      size = rand() % (100 + 1 - 3) + 3;

      probe = mem_allocate(next,size,duration);

      if (probe >= 1) numProbes += probe;
      else allocFails++;

      mem_single_time_unit_transpired();

    }

    totalFrags += mem_fragment_count(50);

  }
  
  printf("NEXT FIT: \n");
  printf("avg num probes = %d\navg alloc fails = %d\n", (numProbes/arg2) , (allocFails/arg2));
  printf("avg num fragments = %d\n\n", (totalFrags/arg2));

  numProbes = 0;
  allocFails = 0; 
  totalFrags = 0;

  mem_clear();





  
                                           //BEST FIT
  mem_strategy_t best = BEST;

  for (i = 0; i < arg2; i++){
    for (j = 0; j < arg3; j++){

      duration = rand() % (25 + 1 - 3) + 3;
      size = rand() % (100 + 1 - 3) + 3;

      //printf("duration = %d, size = %d\n", duration, size);

      probe = mem_allocate(best,size,duration);

      if (probe >= 1) numProbes += probe;
      else allocFails++;

      mem_single_time_unit_transpired();

    }

    totalFrags += mem_fragment_count(50);
  
  } 

  printf("BEST FIT: \n");
  printf("avg num probes = %d\navg alloc fails = %d\n", (numProbes/arg2) , (allocFails/arg2));
  printf("avg num fragments = %d\n\n", (totalFrags/arg2));


  mem_clear();

  mem_free(); //invalid next size (normal)?????

  

  
  return 0;

}
