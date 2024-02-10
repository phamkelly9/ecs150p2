#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <libuthread/queue.h>

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
    queue_destroy(q);

    fprintf(stderr, "*** TEST queue_destroy ***\n");

    TEST_ASSERT(queue_destroy(q) == 0);

    // create a queue, insert, then try to destroy 
    queue_t q2;
    q2 = queue_create();
    int data = 3;

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
    TEST_ASSERT(queue_delete(q, &data1) != NULL);

    // try to delete from an empty queue
    queue_t q2;
    q2 = queue_create();
    TEST_ASSERT(queue_delete(q2, &data2) == -1);
}

void test_queue_enqueue(void){
    // try inserting into a non-initialized queue
    queue_t q;
    int data = 3;

    fprintf(stderr, "*** TEST queue_enqueue into a non-initialized queue ***\n");

    TEST_ASSERT(queue_enqueue(q, &data) == -1);

    // try inserting null data
    queue_t q2;
    q2 = queue_create();
    int data2 = NULL;

    fprintf(stderr, "*** TEST queue_enqueue NULL data ***\n");

    TEST_ASSERT(queue_enqueue(q2, &data2) == -1);
}

void test_queue_dequeue(void){

}

void test_queue_delete(void){
    
}


int main(void)
{
	test_create();
	test_queue_simple();
    test_queue_destroy();
    test_queue_delete();
    test_queue_enqueue();
    test_queue_dequeue();
    
	return 0;
}
