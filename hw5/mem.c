#include <stdlib.h>   // for malloc() and free()
#include <stdio.h>
#include "mem.h"

/*
  Physical memory array. This is a static global array for all functions in this file.  
  An element in the array with a value of zero represents a free unit of memory.
*/
static unsigned int* memory;

/*
 The size (i.e. number of units) of the physical memory array. This is a static global 
 variable used by functions in this file.  

 */
static unsigned int mem_size;


/*
 The last_placement_position variable contains the end position of the last 
 allocated unit used by the next fit placement algorithm.  
 */
static unsigned int last_placement_position;

/*
  Using the memory placement algorithm, strategy, allocate size
  units of memory that will reside in memory for duration time units.

  If successful, this function returns the number of contiguous blocks
  (a block is a contiguous "chuck" of units) of free memory probed while 
  searching for a suitable block of memory according to the placement 
  strategy specified.  If unsuccessful, return -1.

  If a suitable contiguous block of memory is found, the units of this 
  block must be set to the value, duration.
 */
int mem_allocate(mem_strategy_t strategy, unsigned int size, unsigned int duration){

  int i, j, k, temp, out, done;
  int smallest; //for best fit
  int probcnt = 0;

 //printf("last_placement_position = %d\n", last_placement_position);
 // printf("memsize = %d\n", mem_size);


                              //FIRST FIT  
  if ( strategy == 0 ) {

    temp = 0;
    i = 0;
    k = 0;

                               //IT MIGHT BE SPAGHETTI BUT IT GETS THE JOB DONE
      for ( i ; i < mem_size; i++ ){

        if ( memory[i] == 0 ){
                     j = i;         //This is where i set j, acting as i temp index
                     probcnt++;
          while(memory[j] == 0){
              j++;
              k++; 
          }        
          temp = i;     
            if ( k >= size ) {
              for ( i; i < (temp + size); i++ ) memory[i] = duration;
              return probcnt;   
            } 
            i=j; k = 0; 
        } 

      }

    return -1; //page fault 

  }




                          //NEXT FIT
  if (strategy == 1){
   
    k = 0;
    i = last_placement_position;
    temp = 0;

      for ( i; i < mem_size; i++ ){

        if ( memory[i] == 0){
                    j = i;         //This is where i set j, acting as i temp index
                    probcnt++;
          while(memory[j] == 0){
            j++;
            k++;
          }
          temp = i;
            if ( k >= size ) {
              //printf("got here ");
              for ( i; i < (temp+size); i++ ) memory[i] = duration;
              last_placement_position = (i+1)%mem_size;
              return probcnt;
            
           }
           k = 0;
           i = j;
           last_placement_position = i;
           last_placement_position %= mem_size;
        }
      }

    return -1; 

  }



                        //BEST FIT
  if (strategy == 2){

    temp = 0;
    i = 0;
    smallest = size;

                                              //the most spaghetti, the least functional

    for ( i; i < mem_size; i++ ){

        if ( memory[i] == 0 ){
                     j = i;         //This is where i set j, acting as i temp index
                     probcnt++;
          for ( k = 0; k < smallest; k++ ){
            if (memory[j] > 0){
              i+=k;
              k = smallest + 3; //random 3 so we can break outta here & skip next cond
            }  
            j++;    
          } temp = i;
            if ( k == smallest ) {
              //printf("got here ");
              for ( i; i < (temp+size); i++ ) memory[i] = duration;
              return probcnt; //upon success
             
            } 
            smallest++;
            i = temp+k;
        }
      }

    return -1; //page fault

    }


    return 0; //irrelevant but removes warning
}


/*
  Go through all of memory and decrement all positive-valued entries.
  This simulates one unit of time having transpired.
 */
int mem_single_time_unit_transpired(){

  int i;

  for (i = 0; i < mem_size; i++){

    if( memory[i] > 0 )   memory[i] -= 1;

  }

  return 0;

}

/*
  Return the number of fragments in memory.  A fragment is a
  contiguous free block of memory of size less than or equal to
  frag_size.
 */
int mem_fragment_count(int frag_size){

  int i, count, space; 

  count = 0;
  space = 0;

  for (i = 0; i < mem_size; i++){

    while ( memory[i] == 0 ){
      i++;
      space++; //incrementing space of part
    } 
    if(space <= frag_size) count++;
    space = 0;
  }


  return count;

}

/*
  Set the value of zero to all entries of memory.
 */
void mem_clear(){

  int i;

  for (i = 0; i < mem_size; i++){
    memory[i] = 0;
  }

  printf("\n***Mem cleared***\n\n");

}

/*
 Allocate physical memory to size. This function should 
 only be called once near the beginning of your main function.
 */
void mem_init( unsigned int size ){

	memory = malloc( sizeof(unsigned int)*size );
	mem_size = size;

}

/*
 Deallocate physical memory. This function should 
 only be called once near the end of your main function.
 */
void mem_free(){

	free( memory );

}

/*
 Use to debug your program.
 */
void mem_print(){
	//printf("memprint function\n");

  for (int i = 0; i < mem_size; i++){
    printf("%d", memory[i]);
  }
  printf("\n\n");
}


