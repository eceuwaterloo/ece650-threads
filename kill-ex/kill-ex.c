#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include "helper.h"

/* 
 *
 * Example of pthread_kill() to check whether a thread is still alive,
 * and pthread_cancel() to kill it.
 */

void *p(void *arg) {
    printf("Thread %d up and running.\n", pthread_self());
    fflush(stdout);
    thr_sleep(1, 0);
    printf("Thread %d exiting.\n", pthread_self());
    fflush(stdout);
}

int main() {
    pthread_t t;
    int ret;

    pthread_create(&t, NULL, &p, (void *)NULL);

    thr_sleep(0, 500000000);

    if(ret = pthread_kill(t, 0)) {
	printf("main thread: kill() 1 returned error: %s\n", strerror(ret));
	fflush(stdout);
    }
    else {
	printf("main thread: child running well.\n"); fflush(stdout);
    }

    thr_sleep(0, 250000000);

    if(ret = pthread_kill(t, 0)) {
	printf("main thread: kill() 2 returned error: %s\n", strerror(ret));
	fflush(stdout);
    }
    else {
	ret = pthread_cancel(t);
	if(!ret) {
	    printf("main thread: child thread cancelled successfully.\n");
	    fflush(stdout);
	}
	else {
	    printf("main thread: cancel returned error: %s\n", strerror(ret));
	    fflush(stdout);
	}
    }

    if(ret = pthread_kill(t, 0)) {
	printf("main thread: kill() 3 returned error: %s\n", strerror(ret));
	fflush(stdout);
    }

    return 0;
}
