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
struct itimerval timer;
bool isPreemptEnabled = false;

void handler(int signum) {
    (void) signum;
	// Force currently running thread to yield
    printf("got here\n");
    uthread_yield();
}

void preempt_disable(void)
{
    if(isPreemptEnabled == true){
        sigset_t set;

        // Initialize an empty signal set to SIGVTALRM
        sigemptyset(&set);
        sigaddset(&set, SIGVTALRM);

        // unblock thread 
        sigprocmask(SIG_BLOCK, &set, NULL);
    }
}

void preempt_enable(void)
{
    if(isPreemptEnabled == true){
        sigset_t set;

        // Initialize an empty signal set to SIGVTALRM
        sigemptyset(&set);
        sigaddset(&set, SIGVTALRM);

        // unblock thread 
        sigprocmask(SIG_UNBLOCK, &set, NULL);
    }
}

void preempt_start(bool preempt)
{
	if(preempt){
		preempt_enable();
        isPreemptEnabled = preempt;
        struct sigaction curr_sa;
        struct itimerval curr_timer;

        // Set up the signal handler for SIGVTALRM
        curr_sa.sa_handler = handler;
        sigemptyset(&curr_sa.sa_mask);
        curr_sa.sa_flags = 0;
        sigaction(SIGVTALRM, &curr_sa, &sa);

        // Configure the timer to fire at 100 Hz
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = HZ * 100; 
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = HZ * 100; 

        setitimer(ITIMER_VIRTUAL, &curr_timer, &timer);
    }
}

void preempt_stop(void)
{
    if(isPreemptEnabled){
        sigaction(SIGPROF, &sa, NULL);
        setitimer(ITIMER_VIRTUAL, &timer, NULL);
        isPreemptEnabled = false;
    }
}

