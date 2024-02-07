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
	struct Node* node = (struct Node*)malloc(sizeof(struct Node*));
	/* pointer? */

	// return -1 in case of memory allocation error
	if(node == NULL){
		return -1;
	}

	node->data = data;

	// if queue is currently empty, then set current node to head AND tail
	if(queue->length == 0){
		queue->head = node;
		queue->tail = node;
		
		node->prev = NULL;
		node->next = NULL;
	}
	else{

		// set current tail to new node's previous node
		node->prev = queue->tail;
        node->next = NULL;
		
		// set new node to new tail
        queue->tail->next = node;
        queue->tail = node;
    }

    // Increment queue length
    queue->length++;

    return 0; 
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */

	struct Node* node = (struct Node*)malloc(sizeof(struct Node*));

	/* If queue is empty, return -1 */
	if(queue->length == 0){
		/* Should there be a * here? */
		data = NULL; 
		return -1;
	}

	/* Point to front node of queue */
	node = queue->head->data;

	/* Points to node next to current node */
	struct Node *tmp = queue->head;
	queue->head = queue->head->next;

	/* Decrement queue */
	queue->length--;

	/* Free/delete temporary node */
	free(tmp);

	/* Success */
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	// no restriction on big-o
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	// no restriction on big-o
	/* If queue is empty */
	if(queue->length == 0){
		return -1;
	}

	/* Set current node to head */
	struct Node *node = queue->head;

	/* Iterate through each node */
	while(node != NULL) {
		node = node->next;
	}

	/* Success */
	return 0;
}

int queue_length(queue_t queue)
{
	if(queue != NULL)
		return queue->length;
	else
		return -1;
}

