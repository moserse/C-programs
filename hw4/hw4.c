#include <stdio.h>
#include "dpsim.h"

/*
	STEFAN MOSER, 340HW4, 10-31
	PROF. BRENT MUNSELL
*/

/**************************************************

no semiphores, only mutexes.

Main Function:
	int main( int argc, char** argv )

------------------------------------------------
In this function perform the following steps:
------------------------------------------------
1. Create the following variables:
	- main_thread (pthread_t)
	- status (join status value)
2. Create a main_thread 
	- If the return value != 0, then display an error message and 
	  immediately exit program with status value 1.
3. Join the main_thread
	- If the return value != 0, then display an error message and
	  immediately exit the program with status value 2.
4. Display join status value.
5. Exit program.

*/

int main( int argc, char** argv ) {

	pthread_t main_thread;
	int status; 


	if ( pthread_create(&main_thread, NULL, (void*) th_main, NULL) != 0 ){
		printf("pthread create error\n");
		status = 1;
	}

	if ( pthread_join(main_thread, &status) != 0) {
		printf("pthread join error\n");
		status = 2;
	}

	printf("status = %d\n", status);


	printf("\nDONE\n");
		

	return 0;

} // end main function