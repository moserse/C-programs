
#include <stdio.h>	// IO functions
#include <stdlib.h> 	// std lib function
#include <unistd.h>	// close function
#include <pthread.h>	// creating, running, joining threads
#include <string.h>	// string processing 
#include "web.h"	// our code :)


//STEFAN MOSER HW3


/*

HW3: CSCI 340 Operating Systems

Developing a client that creates a thread for each
URL specified in the url.txt file. In each thread: 
1) a TCP/IP socket will be established, 2) read all
the characters in the webpage, and 3) count the number 
of anchor tags.

After all threads have been joined, the number of anchor 
tags will be written to a file named "url_a_num.txt".

*/

// ---------------------------------------------
// Global variable shared by all threads
// This is not thread safe.
web_t* web_array;


// ---------------------------------------------
// Function prototypes to read and write url files
int read_url_file( char* file_path, web_t* web_array );
int write_url_file( char* file_path, web_t* web_array );

void th_run( int* i );

int main( int argc, char *argv[] ) {

	pthread_t thread1;

	int i;
	int k;

	//getting cwd for file write
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	//printf("pwd is %s\n", cwd);

	// malloc web_t array with 100 elements
	web_array = malloc( sizeof( web_t )*100 );

	// -------------------------------------------------
	// Verify the correct number of arguments are provided
	// when the application is executed.

	if ( argc == 2 ) { 

		int num_urls = read_url_file( argv[1], web_array );

		if ( DEBUG )  { 

			printf("Number of urls in text file = %d\n", num_urls);

			for ( i=0; i<num_urls; i++ ) {

				printf("URL=%s\n", web_array[i].url ); 
				printf("CNT=%d\n", web_array[i].anchor_cnt );
				printf("FD=%d\n", web_array[i].socket_fd );

			}
		}





		int cnt[num_urls];

		for (k = 0; k < num_urls; k++ ){

			cnt[k] = k; //allows for k to move around, not stay in same mem location

			if (pthread_create(&thread1, NULL, (void *) th_run, (void * ) &cnt[k]) 
					!= 0 ) {
				printf("pthread didn't happen"); return FAIL; }


			if ( pthread_join(thread1, NULL) != 0 ) {
				printf("pthread didn't join"); return FAIL; }

		}

		//exit(1);

		write_url_file(cwd, web_array);

		

	} else {
		printf( "URL file is not specified!\n" );
		printf( "Example usage: %s %s\n", argv[0], "url.txt" );
	}

	printf("Threads all completed, Check url_a_num.txt to confirm...\n\n");

	return OK;
     
} // end main function



void th_run( int* i ) {

	// -------------------------------------
	// Please do not modify this code
	// -------------------------------------

	if ( open_sock( &web_array[ (*i) ] ) == OK ) {

		parse( &web_array[ (*i) ] );
		close_sock( &web_array[ (*i) ] );

	} else printf( "[%s] network connection failed\n", web_array[ (*i) ].url );


	pthread_exit( NULL );

} // end th_run function




		//only problem with this is it putting in more 
		//slots in the text file than needed..

int write_url_file( char* file_path, web_t* web_array ) {

	int i;
	FILE *urls;
	//int web_array_len;
	//web_array_len = sizeof(web_array) / sizeof(web_array[0]);

	strcat(file_path, "/url_a_num.txt");
	urls = fopen(file_path, "w");


	if (urls == NULL || file_path == NULL) return FAIL;


		for (i = 0; i < sizeof(web_array); i++){

			fprintf(urls,"%s, %d\n", 
					web_array[i].url, web_array[i].anchor_cnt );

			}	

	return OK;

} // end write_url_file function




int read_url_file( char* file_path, web_t* web_array ) {

	// -------------------------------------
	// Please do not modify this code
	// -------------------------------------

	int url_cnt = 0;

	FILE* fhnd;

	fhnd = fopen( file_path, "r" );

	char line[50];

	if ( fhnd != NULL ) {

    	while ( fgets( line, sizeof( line ), fhnd ) != NULL ) {
			line[strlen(line)-1] = '\0';
			web_array[url_cnt].url = malloc(sizeof(char)*100);
			sprintf( web_array[url_cnt].url, "www.%s", line );
			web_array[url_cnt].anchor_cnt = 0;
			web_array[url_cnt++].socket_fd = -1;
    	}

	} else url_cnt = FAIL;

	fclose( fhnd );

	return url_cnt;

} // end read_url_file function

