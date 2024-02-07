#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

struct Node {
	void* data;
	struct Node* next;
    struct Node* prev; 
};

struct queue {
	int length;
    struct Node* head; 
    struct Node* tail;  
};

queue_t queue_create(void)
{
	struct queue* queue = (struct queue*)malloc(sizeof(struct queue*));

	// Return NULL in case of failure when allocating the new queue.
	if(queue == NULL){
		return NULL;
	}

	// empty queue, set initial head and tail to NULL, and length to 0
	queue->head = NULL;
	queue->tail = NULL;
	queue->length = 0;

	// Return pointer to new empty queue
	return queue; 
}

int queue_destroy(queue_t queue)
{
	if(queue->length == 0){
    	free(queue);
		return 0;
	}

	// handle @queue is NULL or @queue is not empty case
	else
		return -1;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	// no restriction on big-o
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	// no restriction on big-o
}

int queue_length(queue_t queue)
{
	if(queue != NULL)
		return queue->length;
	else
		return -1;
}

