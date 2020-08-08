/*****************************************************************************
                The Open University - OS course
 ****************************************************************************/

/*
 * ut.c
 *
 *      Author: 		Itai Antebi
 *
 *      ID Number:		204817498
 *
 *      Description:	Simple library for creating & scheduling user-level threads.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <ucontext.h>

#include "ut.h"

// Number of threads
volatile int ThreadNum;
// The current running thread's number
volatile int currThreadNum;
// An array of pointers to the threads
ut_slot_t **thread_table;


// Create the threads table
int ut_init(int tab_size){
	int i;
	// Modify the table size if it is out of range
	if (tab_size < MIN_TAB_SIZE || tab_size > MAX_TAB_SIZE){
		tab_size = MAX_TAB_SIZE;
	}
	// Save the number of threads globally
	ThreadNum = tab_size;

	// Allocate memory for the thread table
	size_t byte_tab_size = tab_size * sizeof(ut_slot_t*);
	thread_table = malloc(byte_tab_size);
	if (thread_table == NULL){
		return SYS_ERR;
	}

	// Initialize all thread pointers to NULL
	for (i=0; i<ThreadNum; i++){
		thread_table[i] = NULL;
	}

	currThreadNum = 0;
	return 0;
}

// Add a new thread to the threads table
tid_t ut_spawn_thread(void (*func)(int), int arg){
	ut_slot_t *new_thread;
	tid_t tid = currThreadNum;
	int i;

	// Check if the thread table is already full
	if (thread_table[ThreadNum-1] != NULL){
		return TAB_FULL;
	}

	// Create a new thread
	new_thread = malloc(sizeof(ut_slot_t));
	if (new_thread == NULL){
		return SYS_ERR;
	}
	new_thread->vtime = 0;
	new_thread->func = func;
	new_thread->arg = arg;
	// The new thread's context
	new_thread->uc.uc_stack.ss_size = STACKSIZE;
	new_thread->uc.uc_link = NULL;
	// The context's stack
	new_thread->uc.uc_stack.ss_sp = malloc(STACKSIZE);
	if (new_thread->uc.uc_stack.ss_sp == NULL){
		return SYS_ERR;
	}

	// Set all threads' context's
	for (i=0; i<ThreadNum; i++){
		if (getcontext(&(new_thread->uc)) < 0){
			return SYS_ERR;
		}
		makecontext (&(new_thread->uc), (void (*)(void)) new_thread->func, 1, new_thread->arg);
	}

	// Append thread's location to the thread table
	thread_table[tid] = new_thread;
	// Update the current thread number
	currThreadNum = (currThreadNum % ThreadNum) + 1;
	// Return the thread id
	return tid;
}

// Captures the signals and operates accordingly
void handler(int signal){
	// Schedule the next thread
	if (signal == SIGALRM){
		alarm(1);
		currThreadNum = (currThreadNum +1) % ThreadNum;
		if (swapcontext(&(thread_table[currThreadNum]->uc), &(thread_table[(currThreadNum +1) % ThreadNum]->uc)) < 0){
			perror("Error swapping context after handler received SIGALRM: ");
			exit(1);
		}
	}

	// Updates CPU time of thread
	if (signal == SIGVTALRM){
		thread_table[(currThreadNum +1) % ThreadNum]->vtime += 100;
	}
}

//  Starts running the threads
int ut_start(void){
	struct sigaction sa;
	struct itimerval itv;

	// Initialize time interrupt signal (for scheduler)
	sa.sa_flags = SA_RESTART;
	sigfillset(&sa.sa_mask);
	sa.sa_handler = handler;
	if (sigaction(SIGALRM, &sa, NULL) < 0){
		return SYS_ERR;
	}
	if (sigaction(SIGVTALRM, &sa, NULL) < 0){
		return SYS_ERR;
	}

	// Set up virtual timer for thread CPU time calculation
	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 100000;
	itv.it_value = itv.it_interval;

	// Start signals
	alarm(1);
	if (setitimer(ITIMER_VIRTUAL, &itv, NULL) < 0){
		return SYS_ERR;
	}

	// Swap to thread of TID 0
	currThreadNum = -1;
	if (setcontext(&(thread_table[currThreadNum +1]->uc)) < 0){
		return SYS_ERR;
	}
	return 0;
}

// Returns the CPU-time consumed by the given thread.
unsigned long ut_get_vtime(tid_t tid){
	unsigned long vtime;
	// CPU time consumed by thread (milliseconds)
	vtime = thread_table[tid]->vtime;
	return vtime;
}
