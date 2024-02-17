# User-level thread library

## Summary

## `queue` API

The foundation of a user-level thread library is a basic system. A FIFO
design is used so that incoming threads can be scheduled properly for execution.
For the design of the struct, we decided to go with a singly linked list
structure. 

A linked list allows us to access the total length of the queue,
while being able to return the newest and oldest thread. We decided that a
singly linked list would be all that is needed, as we have no need to refer to
the previous node, just the next one. For simplicity, we refrained from going
with a doubly linked list.

### `queue` API Data Structures

**`Node` struct**

- `void* data` - the data of the node, in our case, the thread
- `Node* next` - the node following the current thread

**`queue` struct**

- `int length` - contains the total length of the queue, incremented when a node is
  enqueued, and decremented when a node is dequeued
- `Node* head` - the oldest node in the queue
- `Node* tail` - the newest node in the queue

### `queue` API Functions

- `queue_create()` - allocates an empty `queue` for use by setting the initial
  `head` and `tail` to NULL, so the first `Node` enqueued can be assigned both
  of these values later.
- `queue_destroy(queue_t queue)` - frees an empty `queue`
- `queue_enqueue(queue_t queue, void *data)` - allocates a `Node` to be
  inserted. Structure of the passed in `queue` is modified depending on the
  length, and the allocated `Node` is assigned the `data` parameter.
- `queue_dequeue(queue_t queue, void **data)` - assigns the `data` parameter to
  the `head` of the `queue`, then is modified to make the next `Node` in the
  `queue` the new `head`.
- `queue_delete(queue_t queue, void *data)` - searches the `queue` for a `Node`
  with a matching `data` value. When a match is found, the function removes the
  `Node`, adjusting pointers, freeing memory, and updating the queue length.
- `queue_iterate(queue_t queue, queue_func_t func)` - 

### Designing `queue_tester.c`

## Uthread API

## Semaphore API

## Preemption
