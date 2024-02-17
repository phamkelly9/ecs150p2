# User-level thread library

## Summary

By implementing the following API, we have effectively demonstrated the inner
workings of a simple user-level thread library. The API utilizes `queues`in
order to mimic a `round-robin` scheduling process for the execution of
`threads`. On top of this, our library is also able to handle access to shared
resources through the use of `semaphores`. Finally, we give users the option
to enable `preemption` when running threads, which allows for the interruption
of the current thread.

## `queue` API

The foundation of a user-level thread library is a basic system. A FIFO
design is used so that incoming threads can be scheduled properly for execution.
For the design of the struct, we decided to go with a `singly linked list`
structure.

A linked list allows us to access the total length of the queue, while being
able to return the newest and oldest thread. These functionalities can all be
done within O(1) in a linked list. We decided that a singly linked list would be
all that is needed, as we have no need to refer to the previous node, just the
next one. For simplicity, we refrained from going with a doubly linked list.

### `queue` Data Structures

**`Node` struct**

- `void* data` - the data of the node, in our case, the thread.
- `Node* next` - the node following the current thread.

**`queue` struct**

- `int length` - contains the total length of the queue, incremented when a node is
  enqueued, and decremented when a node is dequeue.
- `Node* head` - the oldest node in the queue.
- `Node* tail` - the newest node in the queue.

### `queue` Functions

- `queue_create()` - allocates an empty `queue` for use by setting the initial
  `head` and `tail` to NULL, so the first `Node` enqueued can be assigned both
  of these values later.

- `queue_destroy(queue_t queue)` - frees an empty queue

- `queue_enqueue(queue_t queue, void *data)` - allocates a Node to be
  inserted. Structure of the passed in queue is modified depending on the
  length, and the allocated `node` is assigned the `data` parameter.

- `queue_dequeue(queue_t queue, void **data)` - assigns the `data` parameter to
  the head of the queue, then is modified to make the next Node in the
  queue the new head.

- `queue_delete(queue_t queue, void *data)` - searches the queue for a Node
  with a matching `data` value. When a match is found, the function removes the
  Node, adjusting pointers, freeing memory, and updating the queue length.

- `queue_iterate(queue_t queue, queue_func_t func)` - iterates through the
  queue by utilizing `node->next` to traverse. On each node, call
  `func` on the given node until there are no more nodes in the queue.

- `queue_length(queue_t queue)` - returns `queue->length`, the number of nodes
  in the queue.

### Designing `queue_tester.c`

The goal of `queue_tester.c` was to reach each edge case possible in every one
of the `queue` API functions. To achieve this, we expanded upon the base cases
given in `queue_tester_example.c`, adding a test function that reaches every edge
case possible in each function. To assure that each case passed as expected, we
used the `TEST_ASSERT()` function to output whether or not cases are passing.

## `uthread API`

The uthread API allows for the creation, scheduling, and destruction
of threads. We use a ready queue to handle the scheduling of threads, and a
thread control block in order to store information about each thread.

### `uthread` Data Structure

**`uthread_tcb` struct**

- `uthread_ctx_t context` - a thread's execution context. Used for when context
  switching occurs between threads, and is initialized through the use of
  `uthread_ctx_init()`, located in `context.c`.
- `int size` - the size of the thread control block. Initialized to the
  maximum `UTHREAD_STACK_SIZE` of 32768 when a thread is created.
- `void* sp` - the pointer to the top of the stack of a given `context`,
  allocated by calling `uthread_ctx_alloc_stack()`.
- `t_state state` - the current state of a given thread, with possible options
  being, `READY`, `RUNNING`, `BLOCKED`, and `ZOMBIE`.

### `uthread` Global Variables

- `static queue_t ready_queue` - a `queue` for threads to be scheduled for
  execution.
- `uthread_ctx_t *idle_context` - stores the idle context that is created when
  `uthread_run()` is first called.
- `struct uthread_tcb *current_thread` - a thread control block that is set to
  the thread at the head of the `queue`.
- `bool preempt` - stores the value of `preempt` when `uthread_run()` is first called.

### `uthread` Functions

- `struct uthread_tcb *uthread_current(void)` - returns the thread currently
  stored in global variable `current_thread`.

- `void uthread_yield(void)` - yields the current thread by inserting it into
  the back of the `ready_queue`. After this is done, we set the next thread
  available in the queue to the current thread, and perform a context switch.
  To context switch, we call `uthread_ctx_switch()`, which allows us to save the
  `yielding_thread` context, and call `swapcontext()` to move on to the next
  thread to be executed.

- `void uthread_exit(void)` - exits the current thread by modifying the state,
  and calling `uthread_ctx_destroy_stack`, which frees the passed in pointer to
  the top of the stack of the thread's context. After this is done, we free the
  current thread and yield to allow for the execution of other threads in the
  ready queue.

- `int uthread_create(uthread_func_t func, void *arg)` - utilizes aforementioned
  `uthread_ctx_alloc_stack()` and `uthread_ctx_init()` in order to create a new
  thread. After intiialization, enqueues the newly created thread into the back
  of the ready queue.

- `int uthread_run(bool preempt, uthread_func_t func, void *arg)` - creates the
  ready queue to be used for scheduling threads. Manually creates the idle
  thread separate from `uthread_create()` in order to save the context and TCB
  as a global variable. The initial thread is then created, and we enter a while
  loop that calls `uthread_yield()` until there are no more threads to be
  scheduled.

- `void uthread_block(void)` - sets the current thread's state to `BLOCKED`,
  then yields to allow for the execution of `READY` threads.
- `void uthread_unblock(struct uthread_tcb *uthread)` - sets the passed in
  thread's state to `READY`, and inserts the thread to the end of the ready queue.

## `semaphore` API

Our semaphore API allows for multiple threads to access common resources. We
first define a queue for blocked threads that are waiting to run `waiting` and
an internal count in our `semaphore` struct. This is to keep track of all the
blocked threads waiting in line to access the resource and to also keep track of
the number of resources available.

### `semaphore` Functions

- `sem_create()` - allocates a memory for a semaphore structure and returns
  `NULL` if the creation was not successful. A waiting queue is also initialized
  with `queue_create()` and `count` is assigned to the argument passed through
  the function.

- `sem_destroy()` - checks first if there are any waiting threads on the queue
  and chooses to destroy the semaphore only if there are none blocked. It also
  checks if the semaphore is `NULL.` After both of these checks, it calls
  `queue_destroy()` to destroy the waiting queue and following that, frees the
  memory allocated for the semaphore.

- `sem_down()` - checks when no resources are available, in order to disable
  preemption to prevent interruptions, and block and enqueue the current thread
  into the `waiting` queue. After leaving the critical section, it reenables
  preemption and decrements the count to indicate that a resource is taken.

- `sem_up()` - increments the count to show that a resource is released,
  dequeues the first thread from the waiting queue, and unblocks that thread.
  Preemption is again disabled and reenabled both these times when entering the
  critical section.

## `preemption` API

The purpose of these functions serves to allow the system to take control and
interrupt a thread or process and force it to yield.

### `preemption` Functions

- `handler()` - is the signal handler for the SIGVTALRM signal. When SIGVTALRM
  is received (at 100 times per second), it forces the currently running thread
  to yield with `uthread_yield().`

- `preempt_disable()` - disables preemption by blocking the `SIGVTALRM` signal
  using a mask. Called before any `queue_dequeue()` or `queue_enqueue()` is called.

- `preempt_enable()` - enables preemption by unblocking the `SIGVTALRM` signal.
  Called after any instances of `queue_dequeue()` or `queue_enqueue()`.

- `preempt_start()` - sets up the signal handler for `SIGVTALRM,` and also
  initializes a timer to fire the signal at 100 HZ.

### `test_preempt.c`

`test_preempt.c` effectively tests preemption by testing its ability to break
out of a process running infinitely. It generates a thread that runs a while
loop where the condition will always be met and relies on preemption called in
`uthread_run()` to take it out of this continuous loop.
