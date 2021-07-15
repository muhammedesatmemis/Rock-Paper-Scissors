#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


#define		ROCK			0		// these macros has been written to figure out well
#define		SCISSORS		1
#define		PAPER			2


int calculate_point(int choice, int other1, int other2);
int compare_with_rock(int other1, int other2);
int compare_with_scissors(int other1, int other2);
int compare_with_paper(int other1, int other2);
void play_game(void);


void *ThreadPlayer1(void* param); // this thread function produce random number
void *ThreadPlayer2(void* param); // this thread function produce random number
void *ThreadPlayer3(void* param); // this thread function produce random number

void increment_counter(void);  // this function increment the counter  when a thread is called.

int counter = 0; // it is sgared object in each thread

pthread_mutex_t g_mutex;

int main()
{	

	play_game();
	printf("Game finished\n");

	return 0;
}

int count_RPS[3] = {0}; // the array keeps the count of ROCK, PAPER, SCISSORS

void play_game(void) 
{
	printf("The game has launced\n");
	srand((unsigned int)time(NULL)); // The program should be started with the new random numbers so We have to change the seed value for that everytime we run it 

	const char* game_names[3] = { "ROCK", "SCISSORS", "PAPER" };  // this array keeps the name of actions in the game
	
	
	int point_thread1 = 0, point_thread2 = 0, point_thread3 = 0;  // threads points
	
	int result;
	
	if ((result = pthread_mutex_init(&g_mutex, NULL)) != 0) {  // mutex object is so important to prevent that two or more thread access to same area
		exit(EXIT_FAILURE);
	}
	
	printf("3 threads will be created\nThe game starts\n--\n--\n");
	while (point_thread1 != 5 && point_thread2 != 5 && point_thread3 != 5) {
		
		int item1, item2, item3;
		pthread_t tid1, tid2, tid3;
		
		if ((result = pthread_create(&tid1, NULL, ThreadPlayer1, &item1)) != 0) {  // we passed argument named 'item', this item keeps random number value from the thread
			exit(EXIT_FAILURE);
		}

		
		if ((result = pthread_create(&tid2, NULL, ThreadPlayer2, &item2)) != 0) {
			exit(EXIT_FAILURE);
		}

		if ((result = pthread_create(&tid3, NULL, ThreadPlayer3, &item3)) != 0) {
			exit(EXIT_FAILURE);
		}

		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);
		pthread_join(tid3, NULL);


		printf("%d. Turn, 1.Thread: %s, 2.Thread: %s, 3.Thread: %s\n", counter / 3, game_names[item1], game_names[item2], game_names[item3]);
		
		

		if (calculate_point(item1, item2, item3) == 2) {
			++point_thread1;
			printf("1.Thread win, Score: ");
		}

		else if (calculate_point(item2, item1, item3) == 2) {
			++point_thread2;
			printf("2.Thread win, Score: ");
		}

		else if (calculate_point(item3, item1, item2) == 2) {
			++point_thread3;
			printf("3.Thread win, Score: ");
		}
		else {
			printf("Draw, Score: ");
		}
		

		printf("%d - %d - %d\n--\n--\n", point_thread1, point_thread2, point_thread3);
	}
	
	printf("how many times were selected: ROCK: %d, PAPER: %d, SCISSORS: %d\n--\n--\n", count_RPS[ROCK], count_RPS[PAPER], count_RPS[SCISSORS]);
	printf("1.Thread terminated \n2.Thread terminated  \n3.Thread terminated\n");
	
	
	pthread_mutex_destroy(&g_mutex); // end of the process, we need to destruct mutex object
	
	printf("Threads are joined by main process\n");

}


void increment_counter(void)  
{
	pthread_mutex_lock(&g_mutex); // we locked the area for the other threads
	++counter;
	pthread_mutex_unlock(&g_mutex); // we unlocked the area for the other threads
}



void *ThreadPlayer1(void* param) 
{

	increment_counter();
	*((int *)param) = rand() % 3;  // this statement means: the argument that came to the thread function has been casted as 'int *' and then the argument is dereferenced and appointed random value
	++count_RPS[*((int *)param)];
	
	return NULL;
}

void *ThreadPlayer2(void* param) 
{

	increment_counter();
	*((int*)param) = rand() % 3;
	++count_RPS[*((int *)param)];
	
	return NULL;
}

void *ThreadPlayer3(void* param) 
{

	increment_counter();
	*((int*)param) = rand() % 3;
	++count_RPS[*((int *)param)];
	
	return NULL;
}


int calculate_point(int choice, int other1, int other2) // the aim of this function is compare a selection from ROCK, PAPER, SCISSORS with the other selections. return value can be '2'(it means that it won) 
{
	int point = 0;
	if (choice == ROCK)
		point += compare_with_rock(other1, other2);
	else if (choice == SCISSORS)
		point += compare_with_scissors(other1, other2);
	else
		point += compare_with_paper(other1, other2);


	return point;
}

int compare_with_rock(int other1, int other2)
{
	int point = 0;
	if (other1 == SCISSORS)
		++point;
	else if (other1 == PAPER)
		--point;

	if (other2 == SCISSORS)
		++point;
	else if (other2 == PAPER)
		--point;

	return point;
}

int compare_with_scissors(int other1, int other2)
{
	int point = 0;
	if (other1 == PAPER)
		++point;
	else if (other1 == ROCK)
		--point;

	if (other2 == PAPER)
		++point;
	else if (other2 == ROCK)
		--point;

	return point;
}

int compare_with_paper(int other1, int other2)
{
	int point = 0;
	if (other1 == ROCK)
		++point;
	else if (other1 == SCISSORS)
		--point;

	if (other2 == ROCK)
		++point;
	else if (other2 == SCISSORS)
		--point;

	return point;
}
