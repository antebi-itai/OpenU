/*****************************************************************************
                The Open University - OS course
 ****************************************************************************/

/*
 * binsem.c
 *
 *      Author: 		Itai Antebi
 *
 *      ID Number:		204817498
 *
 *      Description:	Simple Binary Semaphores library for user-level threads.
 *
 */


#include <signal.h>

#include "binsem.h"


// Initializes a binary semaphore.
 void binsem_init(sem_t *s, int init_val){
	if (init_val == 0){
		*s = 0;
	}
	else{
		*s = 1;
	}
}

// Freeing the semaphore.
void binsem_up(sem_t *s){
	*s = 1;
}

// Attempting to lock the semaphore. In case of failure the thread will yield.
int binsem_down(sem_t *s){
	while (xchg(s,0) == 0){
		if (raise(SIGALRM) != 0){
			return -1;
		}
	}
	return 0;
}
