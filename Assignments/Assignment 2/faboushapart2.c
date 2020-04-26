/*
 ============================================================================
 Name        : Firas Aboushamalah, 250920750
 Class       : CS 3305
 Description : Part 2 for Assignment 2
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>       //include libraries
#include <string.h>

#define BUFFER_SIZE 1024


int main(int argc, char *argv[]) {



	char inputFile[] = "cpu_scheduling_input_file.txt";
	FILE *input_file = fopen(inputFile, "r");
	char outputFile[] = "cpu_scheduling_output_file.txt";
	FILE *output_file = fopen(outputFile, "a");
	char line[256], *SPACE = " ";
	int timequantum;
	float count = 0;
	int numqueues = 0;
	int numprocesses = 0;
	int avgsum=0;
	int totalsum=0;

	if(input_file == NULL) {
		printf("File does not exist");
		return 0;
	}


	// open file to write to

	if(output_file == NULL) {
		printf("Error when opening file");
		return 0;
	}

	char **waitqueue = malloc (1000*sizeof(char*));
	char **processqueue = malloc (1000*sizeof(char*));
	char **turnpqueue = malloc (1000*sizeof(char*));
	int *fcqueue = malloc (1000*sizeof(int));
	int  *turnqueue = malloc (1000*sizeof(int));
	int *sjfqueue = malloc (1000*sizeof(int));



	while (fgets(line, sizeof(line), input_file)) {
		char *token = strtok(line, SPACE); // q
		numqueues++;

		//skipping the first q string to get the value
		token = strtok(NULL, SPACE);

		// skip the tq string to get the value
		token = strtok(NULL, SPACE);

		// getting the tq value
		token = strtok(NULL, SPACE);

		//converting the tq value into an int
	    timequantum = atoi(token);

		while (token != NULL) {
			if (token != NULL){

				//getting the first process and putting it in each of the queues
				token = strtok(NULL, SPACE);

				processqueue[numprocesses] = token;

				waitqueue[numprocesses] = token;

				turnpqueue[numprocesses] = token;


			}
			if (token != NULL){
				token = strtok(NULL, SPACE);

				fcqueue[numprocesses] = atoi(token);

				sjfqueue[numprocesses] = atoi(token);

				turnqueue[numprocesses] = atoi(token);
				numprocesses++;
				count++;
				token = strtok(NULL, SPACE);
			}
		}


		        fprintf(output_file,"Ready Queue %d Applying FCFS Scheduling: \n",numqueues);
		        printf("Ready Queue %d Applying FCFS Scheduling: \n", numqueues);
		        fprintf(output_file,"Order of selection by CPU: \n");
		        printf("Order of selection by CPU: \n");

		        // calculating the first come first serve

		        for(int i=0; i<numprocesses; i++)
		        {
		            fprintf(output_file,"%s ", processqueue[i]);
		            printf("%s ", processqueue[i]);

		        }

		        //calculating the individual waiting times for each of the processes
		        fprintf(output_file,"\n\nIndividual waiting time for each process \n");
		        printf("\n\nIndividual waiting time for each process \n");


				for(int i=0; i < numprocesses; i++)
				{
					fprintf(output_file,"%s = %d\n",processqueue[i],totalsum);
					printf("%s = %d\n",processqueue[i],totalsum);
					avgsum+=totalsum;
					totalsum+=processqueue[i];


				}

	}
}


