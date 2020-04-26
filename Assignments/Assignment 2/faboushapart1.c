/*
 ============================================================================
 Name        : Firas Aboushamalah, 250920750
 Class       : CS 3305
 Description : Part I for Assignment 2
 ============================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

//global variables for parent and thread to access
int x = 10, y = 20, z = 0;

//function that the thread calls to calculate the sum
void *calculate(void *num) {

	z = x + y;
	return 0;
}


int main(int argc, char *argv[]) {

	pid_t parent;  //creating parent variable process
//	int x = 10, y = 20, z = 0;


	//Parent
	if ((parent = fork() > 0)) {
		printf("Parent waiting for child.\n");
		wait(NULL);
		printf("The value of z in parent from child's computation of %d + %d = %d\n", x, y, z);

		//creating thread
		pthread_t thread2;

		printf("From Parent: Going to create Thread to compute x + y = z...\n");

		pthread_create(&thread2, NULL, calculate, NULL);
		//terminating threads
		pthread_join(thread2, NULL);
		printf("From Parent: Thread complete - %d + %d = %d\n", x, y, z);
		}

	//Child
	else if (parent == 0){

		z = x + y;
		printf("Child Adding %d (x) + %d (y) = %d (z) \n", x, y, z);
		exit(0);
	}

	return 0;
}


