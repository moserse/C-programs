/*
	STEFAN MOSER, 340HW4, 10-31
	PROF. BRENT MUNSELL
*/
#include "dpsim.h"
#include <signal.h>



static const unsigned int NUM_PHILOSPHERS = 5;
static const unsigned int NUM_CHOPSTICKS = 5;

static int chopsticks[5];
static int chopcopy[5];
static pthread_mutex_t mutex[5];
static pthread_t philosphers[5];


void* th_main( void* th_main_args ) {

	// ---------------------------------------
	// TODO: you add your implementation here

	int i;
	int cnt[NUM_PHILOSPHERS];
	int g;



													//step 1
	for (i = 0; i < NUM_CHOPSTICKS; i++){
		chopsticks[i] = -1;
		chopcopy[i] = -1;
		//printf("%d\n", chopcopy[i]);
		pthread_mutex_init( &mutex[i], NULL );
	} 

													//step 2
	for (i = 0; i < NUM_PHILOSPHERS; i++){
		cnt[i] = i;
		//delay(100);
		if ( pthread_create(&philosphers[i], NULL, 
								(void *)th_phil, cnt[i]) != 0)
			printf("Pthread did not create.."), exit(1);
	}




												//step 3
	//printf("\nWe have gotten to the main infinite loop..\n\n");
	/*printf("chopstick contents 1 %d, %d, %d, %d, %d\n", chopcopy[0], 
								chopcopy[1], chopcopy[2], 
								chopcopy[3], chopcopy[4]);*/
	printf("\n\n");
	//for (i = 0; i < 5; i++) chopcopy[i] = chopsticks[i];
	while(1){


		memcpy(chopcopy, chopsticks, 5 * sizeof(int));
		
								//step 3 in form of conditional (not in while)
		if( chopcopy[0] == 0 && 
			chopcopy[1] == 1 && 
			chopcopy[2] == 2 && 
			chopcopy[3] == 3 && 
			chopcopy[4] == 4)	{

			printf("\n\nFINAL chopstick contents = %d, %d, %d, %d, %d\n", chopcopy[0], 
								chopcopy[1], chopcopy[2], 
								chopcopy[3], chopcopy[4]);

			printf("\nDEADLOCK ACHIEVE\n\n");

											//step 4
				for (i = 0; i < NUM_PHILOSPHERS; i++)
		 			pthread_kill(philosphers[i], 0);

		 									//step 5 
		 			pthread_exit(NULL);	
		} else {


			/*printf("chopstick contents %d, %d, %d, %d, %d\n", chopcopy[0], 
								chopcopy[1], chopcopy[2], 
								chopcopy[3], chopcopy[4]);*/
 			

			for (g = 0; g < 5; g++){
				if ( chopcopy[g] == chopcopy[(g+1) % 5] && 
						//mutex[g] == mutex[(g+1 % 5)] &&
						chopcopy[g] != -1){
					printf("Philosopher %d is eating\n", g);
				}
			}
			
			delay(300);
			//memcpy(chopcopy, chopsticks, 5 * sizeof(int));

		}	

	}



	

} // end th_main function


void* th_phil( void* th_phil_args ) {

	//printf("we got in th_phil\n");
	int id = (int) th_phil_args; 

	while(1){
		//printf("we got in the philospher loop\n");
		delay(500);
		eat(id);
		//memcpy(chopcopy, chopsticks, 5 * sizeof(int));

	}

} // end th_phil function



void delay( long nanosec ) {

	struct timespec t_spec;

	t_spec.tv_sec = 0;
	t_spec.tv_nsec = nanosec;

	nanosleep( &t_spec, NULL );

} // end think function


void eat( int phil_id ) {


	//printf("we into eat\n");
	//printf("phil_id in eat = %d\n", phil_id);


	//phil 0 	L-1 R-0
	//phil 1 	L-2 R-1
	//phil 2 	L-3 R-2
	//phil 3 	L-4 R-3
	//phil 4 	L-0 R-4


	//picking up right chopstick
	pthread_mutex_lock(&mutex[phil_id]);
	chopsticks[phil_id] = phil_id;
	//printf("Philosopher %d has picked up right\n", phil_id);
 
	delay(100);

	//picking up left chopstick
	pthread_mutex_lock(&mutex[ (phil_id + 1) % 5 ]);
	chopsticks[ (phil_id + 1) % 5 ] = phil_id;
	//printf("Philosopher %d has picked up left\n", phil_id);

	//printf("Philosopher %d is eating\n", phil_id);
	delay(20000);



	//releasing chopsticks in reverse order of which they are picked up,
	//order of chopsticks as well as mutex, array manip

	//releasing left chopstick
	chopsticks[ (phil_id + 1) % 5 ] = -1;
	pthread_mutex_unlock(&mutex[ (phil_id + 1) % 5 ]);
	
	//delay(100);

	//releasing right chopstick
	chopsticks[phil_id] = -1;
	pthread_mutex_unlock(&mutex[phil_id]);

	//printf("Philosopher %d has set both sticks down\n", phil_id);







} // end eat function
