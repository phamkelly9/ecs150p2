#include <stddef.h>
#include <stdlib.h>

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
	/* TODO Phase 3 */
	sem_t* sem = (sem_t*)malloc(sizeof(sem_t));

	sem->count = count;

	if(sem == NULL) {
		return NULL;
	}

	return *sem;

}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	spinlock_lock(sem->lock);  
   	while (sem->count == 0) { 
		//queue 
      uthread_block();
   	}  
   	sem->count -= 1;  
   	spinlock_unlock(sem->lock)
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	spinlock_lock(sem->lock); //preempt disable???
	sem->count++;

	if(queue_length(sem->waiting) > 0){
		struct uthread_tcb *unblocked;
		queue_dequeue(sem->waiting, (void**)&unblocked);
		uthread_unblock(unblocked);
	}

	spinlock_unlock(sem->lock);
	//enable
	return 0;
}

