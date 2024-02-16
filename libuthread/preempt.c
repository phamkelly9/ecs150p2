#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100


struct sigaction sa;

sigaddset(&sa, SIGVTALRM);
sigaction(SIGVTALRM, &sa, NULL);
ITIMER_VIRTUAL



void preempt_disable(void)
{
	/* TODO Phase 4 */
	sigaddset(&sa, SIGVTALRM);
	// sigemptyset addset, procmask to block signals

}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	// sigemptyset, unblock

}

void preempt_start(bool preempt)
{
	(void)preempt;
	/* TODO Phase 4 */
	// set up signal action struct 
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
}

