/*STEFAN MOSER, HW2, CSCI 340 DUE 9-26*/


// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"
#include <string.h>

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//
//
int main( int argc, char** argv ) {

	// ----------------------------------
	// Pseudocode
	// ----------------------------------
	// while ( !done )
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
    //      if (parsed command is "exit")
    //      	done = true
	//  	else if ( is _builtin( ... ) )
	//			do_builtin( ... )
	//		else if ( found in path )
	//  		execute( ... )
	//		else
	//			error()
	// 		cleanup( ... )
	//

									//my code starts here

	//the only thing that doesn't work is blank commands

	command_t cmd;
	int max = 1024;
	int done = FALSE;	
	char line[max];
	char *pos;
	int count = 0;
	printf("\n-------------------------------------------------");
	printf("\n\t\tHere's my shell\n");
	printf("-------------------------------------------------");
	printf("\n\nType a command, or type 'exit' to quit: \n");



	while(!done){

		printf("(%d) >>", count);			
		fgets(line, max, stdin); //reading input			
		if (( pos=strchr(line, '\n')) !=NULL ) *pos = '\0'; //getting rid of \n


		//printf("line is = %s\n", line);
		parse(line, &cmd); //parsing (i hope it works!)
		//printf("\nargv after parse is: %s\n", cmd.argv[0]);

//here's the main conditional

		if ( strcmp(cmd.argv[0], "exit") == 0){

			printf("\nYOU HAVE CHOSEN TO EXIT!\n");
			done = TRUE;

		}

		else if (is_builtin(&cmd) == TRUE){

			//printf("--got to is built conditional\n");
			do_builtin(&cmd);
			count++;

		}

		else if (find_fullpath(cmd.argv[0], &cmd) == TRUE){

			//printf("got here\n");

				execute(&cmd);
				//printf("\n");
				count++;

		}

		else{

			printf("SORRY!! ERROR. '%s' is not a valid command\n", line);
			printf("--Try again or type 'exit' to exit...\n");
			count++;

		}

		cleanup(&cmd);

	}

	//printf("Here's what's in the argv after loop: %s\n", cmd.argv[0]);


	printf( "\nCSCI 340 Homework Assignmet 2: WE GOT TO THE END!\n" );

	return 0;

} // end main function
