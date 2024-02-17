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
struct sigaction curr_sa;

sigset_t ss;

struct itimerval old_timer;
struct itimerval timer;

bool isPreemptEnabled = false;

void handler(int signum) {
    (void) signum;

	// Force currently running thread to yield
    uthread_yield();
}

void preempt_disable(void)
{
    if(isPreemptEnabled == true){
        // block thread 
        sigprocmask(SIG_BLOCK, &ss, NULL);
    }
}

void preempt_enable(void)
{
    if(isPreemptEnabled == true){
        // unblock thread 
        sigprocmask(SIG_UNBLOCK, &ss, NULL);
    }
}

void preempt_start(bool preempt)
{
	if(preempt){
		preempt_enable();
        isPreemptEnabled = preempt;

        // Set up the signal handler for SIGVTALRM
        curr_sa.sa_handler = handler;
        sigemptyset(&curr_sa.sa_mask);
        curr_sa.sa_flags = 0;
        sigaction(SIGVTALRM, &curr_sa, &sa);

        sigemptyset(&ss);
        sigaddset(&ss, SIGVTALRM);

        // Configure the timer to fire at 100 Hz
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = HZ * 100; 
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = HZ * 100; 

        setitimer(ITIMER_VIRTUAL, &timer, &old_timer);
    }
}

void preempt_stop(void)
{
    if(isPreemptEnabled){
        sigaction(SIGVTALRM, &sa, NULL);
        setitimer(ITIMER_VIRTUAL, &old_timer, NULL);
        isPreemptEnabled = false;
    }
}

