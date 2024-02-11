#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <../libuthread/queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

void test_queue_destroy(void){
    // create queue then destroy
    queue_t q;
    q = queue_create();

    fprintf(stderr, "*** TEST queue_destroy ***\n");

    TEST_ASSERT(queue_destroy(q) == 0);

    // create a queue, insert, then try to destroy 
    queue_t q2;
    q2 = queue_create();
    int data = 3;
    queue_enqueue(q2, &data);

    fprintf(stderr, "*** TEST queue_destroy, non-empty queue ***\n");

    TEST_ASSERT(queue_destroy(q) == -1);
}

void test_queue_delete(void){
    // adds 3 nodes to linked list, and deletes data1
    queue_t q;
    q = queue_create();

    fprintf(stderr, "*** TEST queue_delete ***\n");

	int data3 = 3;
    int data2 = 2;
    int data1 = 1;
    queue_enqueue(q, &data3);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data1);
    TEST_ASSERT(queue_delete(q, &data1) == 0);

    // try to delete from an empty queue
    queue_t q2;
    q2 = queue_create();

    fprintf(stderr, "*** TEST queue_delete from empty queue ***\n");

    TEST_ASSERT(queue_delete(q2, &data2) == -1);

    // try to delete from NULL queue
    queue_t q3 = NULL;
    queue_enqueue(q3, &data3);

    fprintf(stderr, "*** TEST queue_delete from empty queue ***\n");

    TEST_ASSERT(queue_delete(q3, &data3) == -1);

    // try to delete NULL data
    void* data5 = NULL;

    fprintf(stderr, "*** TEST queue_delete NULL data ***\n");

    TEST_ASSERT(queue_delete(q, data5) == -1);
}

void test_queue_enqueue(void){
    // try inserting into a non-initialized queue
    queue_t q = NULL;
    int data = 3;

    fprintf(stderr, "*** TEST queue_enqueue into a non-initialized queue ***\n");

    TEST_ASSERT(queue_enqueue(q, &data) == -1);

    // try inserting null data
    q = queue_create();
    void* data2 = NULL;
    fprintf(stderr, "*** TEST queue_enqueue NULL data ***\n");

    TEST_ASSERT(queue_enqueue(q, data2) == -1);

    // try inserting 2 objects
    int data3 = 3;
    queue_enqueue(q, &data3);
    fprintf(stderr, "*** TEST queue_enqueue 2 objects ***\n");

    TEST_ASSERT(queue_enqueue(q, &data) == 0);
}

void test_queue_dequeue(void){
    // try dequeue from a non-initialized queue
    queue_t q = NULL;
    int two = 2;
    int *ptr = &two; 

    fprintf(stderr, "*** TEST queue_dequeue from a non-initialized queue ***\n");

    TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);

    // Ensure that ptr remains unchanged if fail
    TEST_ASSERT(ptr != NULL);
    TEST_ASSERT(*ptr == 2); 

    // try dequeue using NULL data
    int *ptr2 = NULL;
    
    fprintf(stderr, "*** TEST queue_dequeue with NULL data ***\n");

    TEST_ASSERT(queue_dequeue(q, (void**)&ptr2) == -1);
}

void test_queue_length(void){
    // try check length of non-initialized queue
    queue_t q = NULL;

    fprintf(stderr, "*** TEST queue_length from a non-initialized queue ***\n");

    TEST_ASSERT(queue_length(q) == -1);

    // check length of a normal queue
    q = queue_create();
    int data = 0;
    int data2 = 2;

    queue_enqueue(q, &data);
    queue_enqueue(q, &data2);
    
    fprintf(stderr, "*** TEST queue_length ***\n");

    TEST_ASSERT(queue_length(q) == 2);
}

// increments given items
static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

void test_queue_iterate(void){
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    // Initialize the queue and enqueue items 
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    fprintf(stderr, "*** TEST queue_iterate ***\n");

    // Increment every item of the queue, delete item '42' 
    queue_iterate(q, iterator_inc);

    TEST_ASSERT(data[0] == 2);
    TEST_ASSERT(queue_length(q) == 9);

    // iterate an empty queue
    queue_t q2;
    q2 = queue_create();

    fprintf(stderr, "*** TEST queue_iterate on empty queue***\n");

    TEST_ASSERT(queue_iterate(q2, iterator_inc) == -1);
}

int main(void)
{
	test_create();
	test_queue_simple();
    test_queue_destroy();
    test_queue_delete();
    test_queue_enqueue();
    test_queue_dequeue();
    test_queue_length();
    test_queue_iterate();
    
	return 0;
}
