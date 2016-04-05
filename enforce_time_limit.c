//--------------------------------------------------------------------------
// Project: 
// Programatically limit the execution time of a C/C++ function
//
// Prerequisites: 
// Must be running on a UNIX machine
//
// Independent testing info:
// Compile on commandline: gcc enforce_time_limit -o test
// run on commandline: ./test <Time to wait (s)>
//
// Author: Jordan Bonilla
// Date  : April 2016
// License: All rights Reserved. See LICENSE.txt
//--------------------------------------------------------------------------\

#include <unistd.h> // sleep, fork, getpid
#include <stdio.h> // printf
#include <stdlib.h> // atoi
#include <signal.h> // kill

/* Create a child thread that will kill the parent thread after the
   specified time limit has been exceeded */
void enforce_time_limit(int time_limit) {
	int parent_id = getpid();
    pid_t child_id = fork();
    // The fork call creates a lignering child thread that will 
    // kill the parent thread after the time limit has exceeded
    // If it hasn't already terminated.
    if(child_id == 0) // "I am the child thread"
    {
		sleep(time_limit);
        if( kill(parent_id, SIGTERM) == 0) {
        	printf("enforce_time_limit.c: Program terminated"
        	 " for taking longer than %d seconds\n", time_limit);
        }
        // Ensure that parent was actually terminated
        sleep(2);
        if( kill(parent_id, SIGKILL) == 0) {
        	printf("enforce_time_limit.c: Program terminated"
        	 " for taking longer than %d seconds\n", time_limit);
        } 
        // Child thread has done its job. Terminate now.
        exit(0);
    }
    else // "I am the parent thread"
    {
	    // Allow the parent thread to continue doing what it was doing
		return;
	}
}

// Entry point for your own C/C++ program.
int main(int argc, char ** argv) {
	// Call "enforce_time_limit" at the very beginning of your program
	int max_exeuction_time_in_seconds = 10;
	enforce_time_limit(max_exeuction_time_in_seconds);

	// Proceed with your program
	if(argc != 2) {
		printf("./test <time_to_sleep (s)>\n");
		return -1;
	}
	int time_to_sleep = atoi(argv[1]);

	// Visually show elapsed time
	for(int i = 0; i < time_to_sleep; ++i) {
		sleep(1);
		printf("Time elapsed (s): %d\n", i + 1);
	}
	return 0;

}
