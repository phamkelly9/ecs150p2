/*
 * Semaphore simple test
 *
 * Test the synchronization of three threads, by having them print messages in
 * a certain order.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <sem.h>
#include <uthread.h>
#include <private.h>


volatile bool loop = true;

static void thread3(void* arg) 
{
	(void)arg;

	loop = false;
	printf("broke loop\n");

}

static void thread2(void *arg)
{
	(void)arg;

	int i = 0;
	while(loop) {
		++i;
	}
	printf("%d\n", i);
}

static void thread1(void *arg)
{
	(void)arg;

	uthread_create(thread2, NULL);
	uthread_create(thread3, NULL);

	printf("thread1\n");
}

int main(void)
{


	uthread_run(true, thread1, NULL);

	return 0;
}
