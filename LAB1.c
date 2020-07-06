/* 1. Explain what pthread is doing in this program
   Pthreads are POSIX threads that provide over 60 different functions involving threads. These pthreads have identifiers,
   a set of registers, the program counter, and a set of attributes. For this program specifically we use the functions pthread_create
   and pthread_cancel. The first function creates a new thread which has a thread identifier as it's first parameter. This
   can be used in subsequent pthread functions. The pthread_create function is used to create the child thread for this lab from
   the main thread. Pthread_cancel is the second function that sends a cancellation request to the thread passed in the function.
   The function determines if the thread is canceled by it's state and type. Pthread_cancel is used to cancel the child thread
   for this lab which occurs after the enter key has been pressed. 
   
   2. Explain why the sleeping thread can print its periodic messages while the main thread is waiting for keyboard input. 
   The child thread is able to print it's periodic message because of the ability of multithreading that allows
   many threads in the same process. This allows thread switching between the main thread that is waiting on the keyboard input
   and the child thread that sleeps every 3 seconds. This process can be done through thread yields that voluntarily give up CPU time 
   allowing for efficient thread switches in a process because they share an address space. 
*/ 

// Pthread.h is a header file that provides pthread functions 
// Unistd.h is a header file that provides access to POSIX os API
// Stdio.h is a header file that provides standard input/output functions
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

// Constant set at 1 for while statements
#define TRUE 1

// Child thread function passed into pthread_create as the
// start_routine() parameter to invoke new thread
// Has generic pointer as return type of function and parameter and
// static since it's only seen in declared file
static void *child(void *ignored) 
{
	// While loop that is in an infinite loop, always true
	// Prints out message of start of sleep then sleeps for
	// 3 seconds and prints out after sleep
	while(TRUE)
	{
		printf("Start of 3 second sleep for child\n");
		sleep(3);
		printf("Child is done sleeping 3 seconds.\n");	
	}
	// return NULL because of return type void *
	return NULL;
}

// Main function that creates the main thread
int main(int argc, char *argv[])
{
	// Used to check when the user presses the enter key
	char input; 
	// Used in while loop waiting for the user to hit enter key
	int condition = 1; 
	// ID of the new thread to be created
	pthread_t child_thread;
	// int value used to check success of pthread functions
	int code;
	// Creates new thread from main thread.
	// Passes in the address of child_thread,
	// NULL as attr argument means default attributes,
	// child function passed as start routine,
	// and NULL passed as sole parameter child function
	// return value stored in code, 0 if successful 
	code = pthread_create(&child_thread, NULL, child, NULL);
	
	// Check to see if child thread creation was successful
	if(code)
	{
		fprintf(stderr,"pthread_create failed with code %d\n",code);
	}
	
	// while loop that is waiting for keyboard input
	// Always true until the condition is set to 0 when it detects enter key hit
	while(condition)
	{
		// input receives keyboard input
		input = getchar();
		// if keyboard input is enter it should be equivalent to new line
		if(input == '\n')
		{
			// Set condition to  0 to break out of while loop
			condition = 0;
		}
	}
	
	// Kill child thread with pthread_cancel function that takes thread ID as parameter
	// Returns a value of 0 if successful. Stored in code. 
	code = pthread_cancel(child_thread);
	printf("Wait for 5 seconds to check child thread is terminated.\n");
	// Main thread waits for 5 seconds using sleep
	sleep(5);
	printf("5 seconds have passed!\n");
	
	// Checks if the child thread was successfully terminated
	// Prints out message if it succeeded or failed
	if(code)
	{
		printf("Failed to cancel and terminate child thread!\n");
	}
	else
	{
		printf("Successfully terminated child thread!\n");
	}
	// return 0 should terminate the main thread
	return 0;
}
