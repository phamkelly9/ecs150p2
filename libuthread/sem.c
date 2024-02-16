#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	// queue for waiting threads and count
	queue_t waiting;
	int count;
};

sem_t sem_create(size_t count)
{
	sem_t sem = (sem_t)malloc(sizeof(sem_t));

	if(sem == NULL) 
		return NULL;

	sem->count = count;

	sem->waiting = queue_create();

	if(sem->waiting == NULL){
		free(sem);
		return NULL;
	}

	return sem;

}

int sem_destroy(sem_t sem)
{
	if(queue_length(sem->waiting) > 0)
		return -1;

	if(sem == NULL)
		return -1;

	queue_destroy(sem->waiting);
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	if(sem == NULL)
		return -1;
		
   	while (sem->count == 0) { 
		queue_enqueue(sem->waiting, uthread_current());
      	uthread_block();
   	}  

   	sem->count -= 1;  

	return 0;
}

int sem_up(sem_t sem)
{
	if(sem == NULL)
		return -1;

 	//preempt disable???
	
	sem->count +=1;

	if(queue_length(sem->waiting) > 0){
		struct uthread_tcb *unblocked;
		queue_dequeue(sem->waiting, (void**)&unblocked);
		uthread_unblock(unblocked);
	}

	//enable
	return 0;
}

