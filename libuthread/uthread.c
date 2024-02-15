#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "private.h"
#include "uthread.h"
#include "queue.h"
#define UTHREAD_STACK_SIZE 32768

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    ZOMBIE
} t_state;

struct uthread_tcb {
	uthread_ctx_t context;
	int size;
	void* sp;
	t_state state;
};

static queue_t queue;
struct uthread_tcb *uthread_current(void)

{
	if (queue_length(queue) == 0) {
        return NULL; 
    }

    struct uthread_tcb *current_thread;

	queue_dequeue(queue, (void **)&current_thread);

	// returns pointer to current thread 
    return current_thread;
}

void uthread_yield(void)
{
	// get currently active and running thread
	struct uthread_tcb *current_thread = uthread_current();
	current_thread->state = READY;

	// enqueue back into ready queue
	if(current_thread->state != BLOCKED)
    	queue_enqueue(queue, current_thread);

	// if queue is just the initial thread, no need to context switch
	// dequeue next thread at front of ready queue
	struct uthread_tcb *next_thread; 
	queue_dequeue(queue, (void**)&next_thread);
	next_thread->state = RUNNING;

	// perform context switch
	uthread_ctx_switch(&current_thread->context, &next_thread->context);

}

void uthread_exit(void)
{
	struct uthread_tcb *current_thread = uthread_current();
	struct uthread_tcb *next_thread; 

	// dequeue current thread
	queue_dequeue(queue, (void**)&next_thread);

	current_thread->state = ZOMBIE;

	// context switch next with current 
	uthread_ctx_switch(&current_thread->context, &next_thread->context);
	// terminated threads do not need to be inserted back into queue
	uthread_ctx_destroy_stack(current_thread->sp);
	free(&current_thread->context);
	free(current_thread);
}

int uthread_create(uthread_func_t func, void *arg)
{
    struct uthread_tcb *thread = malloc(sizeof(struct uthread_tcb));

    if (thread == NULL) {
        return -1; 
    }
	
	// uthread_ctx_alloc_stack() returns a pointer to the top of the stack
	void *thread_sp = uthread_ctx_alloc_stack();

	if(thread_sp == NULL)
		return -1;

	thread->state = READY;
	thread->sp = thread_sp;
	thread->size = UTHREAD_STACK_SIZE;

	uthread_ctx_t *newContext = malloc(sizeof(uthread_ctx_t));

	// initialize new uthread_ctx_t to be used in the newly created thread
	if(uthread_ctx_init(newContext, thread->sp, func, arg) == -1)
		return -1;
	
	thread->context = *newContext;

	queue_enqueue(queue, thread);
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	// create ready queue
	queue = queue_create();

	// create initial thread 
	if (uthread_create(func, arg) == -1){
		return -1;
	}

	// uthread_current = idleThread;

	// infinite loop until there are no more threads ready to run
	while(queue_length(queue) != 0){
		uthread_yield();
	}

	return 0; 

}

void uthread_block(void)
{
	/* TODO Phase 3 */
	struct uthread_tcb *current_thread = uthread_current();
	current_thread->state = BLOCKED;
	uthread_yield();

}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
	uthread->state = READY; /* Set state of current thread to READY */
	queue_enqueue(queue, uthread);
}
