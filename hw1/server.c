
// -----------------------------------
// CSCI 340 - Operating Systems
// Fall 2016
// server.h header file
// Homework 1
//
// -----------------------------------

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server.h"

// ------------------------------------
// Function prototype that creates a socket and 
// then binds it to the specified port_number 
// for incoming client connections
// 
//
// Arguments:	port_number = port number the server 
//				socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//

int bind_port( unsigned int port_number ) {

	// -------------------------------------
	// NOTHING TODO HERE :)
	// -------------------------------------
	// Please do not modify

	int socket_fd;
	int set_option = 1;

    struct sockaddr_in server_address;
     
    socket_fd = socket( AF_INET, SOCK_STREAM, 0 );

    setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set_option, sizeof( set_option ) );

    if (socket_fd < 0) return FAIL;

    bzero( (char *) &server_address, sizeof(server_address) );

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( port_number );

    if ( bind( socket_fd, (struct sockaddr *) &server_address, sizeof(server_address) ) == 0 ) {

    	return socket_fd;

    } else {

    	return FAIL;

    }

} // end bind_port function

// ------------------------------------
// Function prototype that accepts a client
// socket connection
// 
//
// Arguments:	server file descriptor
//
// Return:      Termination status of client
//				( 0 = No Errors, -1 = Error )
//
int accept_client( int server_socket_fd ) {

	int exit_status = OK;

	int client_socket_fd = -1;

	socklen_t client_length; 

	struct sockaddr_in client_address;

	char request[512];

	client_length = sizeof( client_address );

    client_socket_fd = accept( server_socket_fd, (struct sockaddr *) &client_address, &client_length );

	//my code starts here haha

	//pid_t pid;
	
	 //returns 2 ids. 1 is 0 (the child), other is not (parent id)

	
	
	//for (int i = 0; i < 512; i++){ printf(" %c ", request[i]); }
		
	// -------------------------------------
	// TODO:
	// -------------------------------------
	// Modify code to fork a child proces
	// -------------------------------------

		int pid; 
		pid = fork(); //0 means child process
		printf("this is pid.. %d\n", pid);

//here we go...

	if (pid == 0){
		printf("\nTEST of CHILD process\n\n");  

	
	if ( client_socket_fd >= 0 ) {
		
		bzero( request, 512 );
		
		read( client_socket_fd, request, 511 );
		
		if ( DEBUG ) printf("Here is the http message:\n%s\n\n", request );


		
		// -------------------------------------
		// TODO:
		// -------------------------------------
		// Generate the correct http response when a GET or POST method is sent
		// from the client to the server.
		// 
		// In general, you will parse the request character array to:
		// 1) Determine if a GET or POST method was used
		// 2) Then retrieve the key/value pairs (see below)
		// -------------------------------------
		
		
		
		/*
		 ------------------------------------------------------
		 POST method key/value pairs are located in the entity body of the request message
		 ? - indicates the beginning of the key/value pairs
		 & - is used to delimit multiple key/value pairs 
		 = - is used to delimit key and value
		 
		 Example:
		 
		 first=brent&last=munsell
		 
		 two &'s indicated two key/value pairs (first=brent and last=munsell)
		 key = first, value = brent
		 key = last, value = munsell
		 ------------------------------------------------------
		 */

 

		char filler[512]; //contents that will go in html body
		char entity_body[512]; //html body

		int j = 0;
		char URL[512];
		int pos;
						//frankenstein loop that puts important 
						//parts of request in URL array.
						//pretty sure this won't happen on post (strt to 512)
		for (pos = 0; pos < 512; pos++){
			if(request[pos] == '?'){
				while(request[pos+1] != ' '){
					URL[j] = request[pos+1];
					j++;
					pos++;
				}
				pos=512;
			}

		}

		
//DEALING WITH GET 

		if (request[0] == 'G'){
		
			

			char *end;
			char *token1 = strtok_r(URL, "&", &end);
			printf("------It's a GET!------\n\n");
		
		
	//first while strtok getting rid of the &
		while (token1 != NULL){
			char *endtok;
			//printf("current = %s\n", token1);
			char *token2 = strtok_r(token1, "=", &endtok);


		//secod while strtok getting rid of the = and concatinating the result
				while(token2 != NULL){

					char result[20];
					char opentd[] = "<td>";
					char closetd[] = "</td>";
	
					strcpy(result, opentd);
					strcat(result, token2);
					strcat(result, closetd);

					strcat(filler, result);
					//printf("pair = %s\n", result);
					token2 = strtok_r(NULL, "=", &endtok);	
	
			}
			token1 = strtok_r(NULL, "&", &end);
	}
		//building html entity body

		char* open = "<html><body><h2>GET Operation</h2><table cellpadding=5 cellspacing = 5 					border=1><tr>PAIRS";

		char* closer = "</tr></table></body></html>";

		strcpy(entity_body, open);
		strcat(entity_body, filler);
		strcat(entity_body, closer);

			
			
		}


//DEALING WITH POST 
			//doesn't work yet. 

		else if (request[0] == 'P'){
		


			printf("------It's a POST!-------\n\n");

			//for (int y = 0; y < 512; y++){ printf("%c", URL[y]); }
		
			char *end;
			char *token1 = strtok_r(URL, "&", &end);
		
		
	//first while strtok getting rid of the &
		while (token1 != NULL){
			char *endtok;
			//printf("current = %s\n", token1);
			char *token2 = strtok_r(token1, "=", &endtok);


		//secod while strtok getting rid of the = and concatinating the result
				while(token2 != NULL){

					char result[20];
					char opentd[] = "<td>";
					char closetd[] = "</td>";
	
					strcpy(result, opentd);
					strcat(result, token2);
					strcat(result, closetd);

					strcat(filler, result);
					printf("pair = %s\n", result);
					token2 = strtok_r(NULL, "=", &endtok);	
	
			}
			token1 = strtok_r(NULL, "&", &end);
	}
		//building html entity body

		char* open2 = "<html><body><h2>POST Operation</h2><table cellpadding=5 cellspacing = 5 					border=1><tr>PAIRS";

		char* closer2 = "</tr></table></body></html>";

		strcpy(entity_body, open2);
		strcat(entity_body, filler);
		strcat(entity_body, closer2);

	}	



			else { 
			printf("Neither Get nor Post... Error");
			exit(0);
		}

		

		
	// THIS IS AN EXAMPLE ENTITY BODY
	//	char* entity_body = "<html><body><h2>CSIS 614 (Advanced Operating Systems) Project 1</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr></table></body></html>";

		


		char response[512];
		sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen( entity_body ), entity_body );

		
		
		if ( DEBUG ) printf( "%s\n", response );
		
    	write( client_socket_fd, response, strlen( response ) );
		
		close( client_socket_fd );
		
		
	} else {
		
		exit_status = FAIL;
		
	}

	  
	
	if ( DEBUG ) printf("Exit status = %d\n", exit_status );
	
	exit(0);

	}//this bracket ends child process. not 100% sure it should be here but works so far.
	
	return exit_status;
	
}

 // end accept_client function
