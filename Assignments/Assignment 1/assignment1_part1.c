/*
 ============================================================================
 Name        : Firas Aboushamalah, 250920750
 Class       : CS 3305
 Description : Part A for assignment one
 ============================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

int main() {
	pid_t child_1, child_2, child_11;
	int status;


	//PARENT PROCESS AFTER CREATING CHILD 1
	if ((child_1 = fork()) > 0) {

		//waiting for child_1 to complete before moving on to child 2
		wait(NULL);

		//CHILD 2
		if ((child_2 = fork()) == 0) {
			printf("parent (PID %d) created child_2 (PID %d)\nchild_2 (PID %d) is calling an external program external_program.out and leaving child_2...\n", getppid(), getpid(), getpid());
			status = execl("external_program.out", "external_program.out", NULL);  //calling external program in same directory
			if (status != 0) {  //if the execl returns something it failed
		        perror("exec");
		        printf("main: errno is %d\n",errno);
			}
		}

		//PARENT OF CHILD 2
		else {
			wait(NULL);  //parent waiting for child 2 to complete so it doesn't become orphan
		}
	}

	//CHILD 1
	else if (child_1 == 0){
		printf("parent process (PID %d) created child_1 (PID %d)\n", getppid(), getpid());
		printf("parent (PID %d) is waiting for child_1 (PID %d) to complete before creating child_2\n", getppid(), getpid());

		//CHILD 1.1
		if ((child_11 = fork()) == 0) {
			printf("child_1 (PID %d) created child_1.1 (PID %d)\n", getppid(), getpid());
			printf("child_1 (PID %d) is now completed\n", getppid());
			exit(1);
		}

		//PARENT OF CHILD 1.1
		else {
			wait(NULL);  //waiting for child 2 so that child 2 does not have same parent ID
		}
	}


}
