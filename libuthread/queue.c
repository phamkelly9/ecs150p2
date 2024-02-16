#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include <stdio.h>

struct Node {
	void* data;
	struct Node* next;
};

struct queue {
	int length;
    struct Node* head; 
    struct Node* tail;  
};

queue_t queue_create(void)
{
	struct queue* queue = (struct queue*)malloc(sizeof(struct queue*));

	// return NULL in case of failure when allocating the new queue.
	if(queue == NULL){
		return NULL;
	}

	// empty queue, set initial head and tail to NULL, and length to 0
	queue->head = NULL;
	queue->tail = NULL;
	queue->length = 0;

	// return pointer to new empty queue
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

	// return -1 if memory allocation error, or if queue and data are NULL
	if(node == NULL || queue == NULL || data == NULL)
		return -1;
	

	node->data = data;

	// if queue is currently empty, then set current node to head AND tail
	if(queue->length == 0){
		queue->head = node;
		queue->tail = node;
	} else {
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
	struct Node* node = (struct Node*)malloc(sizeof(struct Node*));

	// If queue, data is NULL, or if queue length is 0, return -1 
	if (queue == NULL || data == NULL || queue->length == 0)
		return -1;

	// Point to front node of queue 
	node = queue->head;
	*data = node->data;

	// Points to node next to current node
	queue->head = queue->head->next;

	// Decrement queue 
	queue->length--;

	// Success 
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	// Checks if queue or data are NULL
	if (queue == NULL || data == NULL)
		return -1;

	struct Node *current = queue->head;
	struct Node *previous = NULL;

	// loop until node with matching data is freed
	while (current != NULL) 
	{
		if (current->data == data) 
		{
			// check if matching node is the head
			if (previous == NULL) 
				queue->head = current->next;
			else 
				previous->next = current->next;

			// delete node with matching data
			free(current);

			// decrement queue length
			queue->length--;

			// return 0 on successful delete
			return 0;
		}

		// move to the next node
		previous = current;
		current = current->next;
	}

	// data not found in the queue
	return -1;
}


int queue_iterate(queue_t queue, queue_func_t func)
{
	// if queue or passed in function is NULL, return -1
	if(queue == NULL || func == NULL)
		return -1;

	// if queue is empty, return -1
	if(queue->length == 0)
		return -1;

	// Set current node to head 
	struct Node *node = queue->head;

	// Iterate through each node 
	while(node != NULL) 
	{
		void* a = node->data;
		node = node->next;
		func(queue, a);
	}

	// Success 
	return 0;
}

int queue_length(queue_t queue)
{
	if(queue != NULL)
		return queue->length;
	else
		return -1;
}

