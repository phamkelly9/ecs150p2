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

/* Context switch */
/* Idle thread holds main processes' info, switch into code that's in uthread run */
/* yield -- dequeueing */

/* pull threads out of queue with yield */

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} t_state;

struct uthread_tcb {
	/* TODO Phase 2 */
	int tid; /* Thread ID */
	void* stack;
	int STACK_SIZE;
	void* sp;
	t_state state;

};

static queue_t queue;
struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	if (queue_length(queue) == 0) {
        return NULL; 
    }

    struct uthread_tcb *current_thread;
	/* Dequeue front of queue */
    queue_dequeue(queue, (void **)&current_thread);
	/* Enqueue back in order */
    queue_enqueue(queue, current_thread);
	/* Returns pointer to current thread */ 
    return current_thread;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	/* 1) put current at end of ready queue, dequeue front, context switch */
	struct uthread_tcb *current_thread = uthread_current();
	if (current_thread == NULL) {
        return; 
    }

	current_thread->state = READY;
	/* Dequeue current thread and add back to the end of queue */
	queue_dequeue(queue, (void **)&current_thread);
	queue_enqueue(queue, current_thread);
	
	/* Dequeue front of queue, perform context switch */

}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	struct uthread_tcb *current_thread = uthread_current();
	if (current_thread == NULL) {
        return;
    }

	current_thread->state = TERMINATED;
	queue_delete(queue, current_thread);
	uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	struct uthread_tcb *thread = malloc(sizeof(struct uthread_tcb));
    if (thread == NULL) {
        return -1; 
    }

	/* Create new thread TID, stack, stack size */
	
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
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

