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
uthread_ctx_t *idle_context;
struct uthread_tcb *current_thread;
static queue_t zombie_queue;

struct uthread_tcb *uthread_current(void)
{
    return current_thread;
}

void uthread_yield(void)
{
	// get currently active and running thread
	struct uthread_tcb *yielding_thread = uthread_current();
	
	if(queue_length(queue) > 0)
	{
		// enqueue current thread back into ready queue
		if(yielding_thread->state == RUNNING){
			yielding_thread->state = READY;
			queue_enqueue(queue, yielding_thread);
		}

		// dequeue next thread at front of ready queue
		struct uthread_tcb *next_thread; 
		queue_dequeue(queue, (void**)&next_thread);
		next_thread->state = RUNNING;
		current_thread = next_thread;

		// perform context switch
		uthread_ctx_switch(&yielding_thread->context, &next_thread->context);
	}
	
}

void uthread_exit(void)
{
	struct uthread_tcb *current_thread = uthread_current();

	if(current_thread != NULL && &current_thread->context != idle_context)
	{
		current_thread->state = ZOMBIE;

		// add thread to zombie queue for cleanup at the end of thread_run()
		queue_enqueue(zombie_queue, current_thread);

		uthread_yield();
	}

}

int uthread_create(uthread_func_t func, void *arg)
{
    struct uthread_tcb *thread = malloc(sizeof(struct uthread_tcb));

	if(thread == NULL)
		return -1;
	
	// uthread_ctx_alloc_stack() returns a pointer to the top of the stack
	void *thread_sp = uthread_ctx_alloc_stack();

	if(thread_sp == NULL)
		return -1;

	thread->state = READY;
	thread->sp = thread_sp;
	thread->size = UTHREAD_STACK_SIZE;

	uthread_ctx_t *newContext = malloc(sizeof(uthread_ctx_t));

	if(newContext == NULL)
		return -1;

	// initialize new uthread_ctx_t to be used in the newly created thread
	if(uthread_ctx_init(newContext, thread->sp, func, arg) == -1)
		return -1;
	
	thread->context = *newContext;

	if(queue_enqueue(queue, thread) == -1)
		return -1;

	return 0;
}

// iterate through each thread in zombie queue to destroy
void collect_zombie(queue_t queue, void* data)
{
    struct uthread_tcb *zombie_thread = (struct uthread_tcb*)data;
	queue_dequeue(queue, (void**)&zombie_thread);

	// destroy current thread's stack
    uthread_ctx_destroy_stack(zombie_thread->sp);
    free(zombie_thread);
} 

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    // create ready queue
    queue = queue_create();

    if(queue == NULL)
        return -1;

    // create zombie queue
    zombie_queue = queue_create();

    if(zombie_queue == NULL)
        return -1;

    // create idle thread 
    struct uthread_tcb*idle_thread = malloc(sizeof(struct uthread_tcb));

    if(idle_thread == NULL)
        return -1;

    void *idle_thread_sp = uthread_ctx_alloc_stack();
    idle_thread->state = READY;
    uthread_ctx_t *new_idle_context = malloc(sizeof(uthread_ctx_t));

    if(uthread_ctx_init(new_idle_context, idle_thread_sp, func, arg) == -1)
        return -1;

    if(queue_enqueue(queue, idle_thread) == -1)
        return -1;

    idle_context = new_idle_context;
    current_thread = idle_thread;

    // create initial thread
    if(uthread_create(func, arg) == -1)
        return -1;

    preempt_start(preempt);

    // infinite loop until there are no more threads ready to run
    while(queue_length(queue) > 0)
    {
        uthread_yield();
    }

    // delete zombies
    if(queue_iterate(zombie_queue, collect_zombie) == -1)
        return -1;

    if(preempt)
        preempt_stop();

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