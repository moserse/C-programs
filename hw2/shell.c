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
#include <string.h>
#include "shell.h"

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could 
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};

// --------------------------------------------
// Implementation for each prototype below
// --------------------------------------------
// void parse( char* line, command_t* p_cmd );

void parse( char* line, command_t* p_cmd ){

	char *token;
	char *end;
	int i;
	char *temp = malloc(sizeof(char) * strlen(line)); //temp to count toks

	strcpy(temp, line);

	p_cmd->path = (char *) malloc(sizeof(getenv("PATH")));
	p_cmd->path = getenv("PATH"); //setting path 

	//counting argc here
	for(token = strtok_r(temp, " ", &end); 
		token!=NULL; token = strtok_r(NULL, " ", &end)){
		//printf("%s\n", token);
		p_cmd->argc++;
	}

			//parsing tokens in here
	if (p_cmd->argc > 0){

		char *vtoken;
		char *end2;
		p_cmd->argv = (char **) malloc((p_cmd->argc+1) * sizeof(char **));

		//printf("this is not a blank command\n\n");


		for(vtoken = strtok_r(line, " ", &end2); vtoken!=NULL; 
			vtoken = strtok_r(NULL, " ", &end2)){

				//printf("%s\n", vtoken);
				p_cmd->argv[i] = vtoken;
				i++;
		}
			p_cmd->argv[i] = NULL;
		
		//find full path should RLY go here..
			find_fullpath(p_cmd->argv[0], p_cmd);
	}


	else{ //in the case of empty string
			/*THIS CAUSES A SEGMENTATION FAULT*/


		printf("This is a blank command\nSegmentation fault inc... Sorry\n");
		/*p_cmd->argv = (char **) malloc(sizeof(char*));
		//printf("got here");
		p_cmd->argv = NULL;
		p_cmd->path = NULL;*/


	}
	//printf("argc = %d\n", p_cmd->argc);

	


}

// int execute( command_t* p_cmd );

int execute (command_t* p_cmd){
	

		if ( p_cmd->argc > 0 ) {

			if ( fork() == 0 ) execv( p_cmd->path, p_cmd->argv );
				
			
				wait(0);

				/*this is pretty irrelevant due to conditionals
				in hw2.c but at the time of writing this i am too
				afraid to remove it*/

			} else if ( p_cmd->argc == -1 ) {

				printf("%s: command not found!\n", p_cmd->argv[0]);
				
				}

	return 0;

}
// int find_fullpath( char* command_name, command_t* p_cmd );

int find_fullpath( char* command_name, command_t* p_cmd){

	char* path_env_variable;
	path_env_variable = getenv( "PATH" );
	char* mypath = (char *) malloc(sizeof(path_env_variable));
	char* temp[1024];
	char *token;
	char *end;
	struct stat buffer;

	//printf("PATH = %s\n", path_env_variable );
	strcpy(mypath, path_env_variable);

//here i am parsing by colons and then finding if the command name
//is in a specific path. then set p_cmd->path to that path. 

	for(token = strtok_r(mypath, ":", &end);
			 token!=NULL; 
			 token = strtok_r(NULL, ":", &end)){

		//printf("%s\n", token);


	//putting it in a temp so i dont mess with token 
		//and then catting a '/' and the command name
		strcpy(temp, token);
		strcat(temp, "/");
		strcat(temp, command_name);

	//checking if exists
		if( stat(temp, &buffer) == 0 && ( S_IFDIR & buffer.st_mode ) ){
				p_cmd->path = temp;
				//printf("here's the location of the file");
				return TRUE;
			}
		else if (stat(temp, &buffer) == 0 && ( S_IFREG & buffer.st_mode ) ){
				p_cmd->path = temp;
				//printf("here's the path: %s\n", p_cmd->path);
				return TRUE;
			}
						
		//emptying temp in case of miss
		memset(temp, 0, 1024);
		//printf("%s\n", temp);

	}
			
	//printf("it doesn't look like %s is in here...\n", command_name);

	return FALSE;
}



// int is_builtin( command_t* p_cmd );

int is_builtin( command_t* p_cmd ){

//going thru commands and checking if any are equal (rly only cd atm..)

	for (int i = 0; valid_builtin_commands[i] != NULL; i++){
			if ( strcmp(p_cmd->argv[0], valid_builtin_commands[i]) == 0 ){
				return TRUE;
			}
	} 
	return FALSE; //outside the for loop
}


// int do_builtin( command_t* p_cmd );


int do_builtin( command_t* p_cmd ){

	char *cwd[1024];

		//making sure the cmd is 'cd'
		if (strcmp(p_cmd->argv[0], valid_builtin_commands[0]) == 0 ){ 

				//printf("HAHAH%s\n", p_cmd->argv[1]);
				//char * dir = p_cmd-argv[1];
				//printf("chdir = %d\n", chdir(p_cmd->argv[1]));
				//printf("--this is the line before i try chdir--\n");
				//printf("--arg for cd is %s\n", p_cmd->argv[1]);

				if (chdir(p_cmd->argv[1]) == 0) {
							//printf("successful on chdir\n");
							getcwd(cwd, sizeof(cwd));
							printf("Changed directory to: \n%s\n", cwd);
							return SUCCESSFUL;
						}
				else{
					printf("\nFailed to change directory\n");
					printf("--If you're trying to go back a directory, try 'cd ..' \n");
					printf("OTHERWISE-- directory does not exist..\n");
					return ERROR;
				}
			}

			return 0; 

}



// void cleanup( command_t* p_cmd );

void cleanup( command_t* p_cmd ){
	
	for (int i = 0; i < p_cmd->argc; i++){
		p_cmd->argv[i] = NULL;
		free(p_cmd->argv[i]);
	}

	p_cmd->argc = 0;
	free(p_cmd->argv);
	p_cmd->argv = NULL;
	p_cmd->path = NULL;
	free(p_cmd->path);


	wait(NULL);

}

