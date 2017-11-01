#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "helper.h"

/* 
 * 
 * Example of multithreading using pthreads. Specifically, a thread does blocking
 * input using scanf().
 * 
 * One thread executes out(), which repeatedly outputs the value of its
 * argument till it becomes non-zero.
 *
 * The other thread executes in(), which blocks on scanf() waiting for user
 * input. It then exits.
 */


void *out(void *arg) {
    int *i = (int *)arg;
    while(1) {
	/* Warning -- the following sleep() call blocks the entire process.
	 * May be better to use custom thr_sleep() from helper.c instead.
	 */
	sleep(1);

	printf("out: i = %d\n", *i);

	if(*i) return;
    }
}

void *in(void *arg) {
    int *i = (int *)arg;
    int retcode;

    printf("Enter an integer value for i when you are ready.\n");
    fflush(stdout);
    retcode = scanf("%d", i);
    if(retcode < 0) {
	perror("scanf"); return;
    }
    else if(retcode == 0) {
    	errp("scanf() returned 0", 0); return;
    }
    else {
    	/* Successfully read */
    	return ;
    }
}

int main() {
    pthread_t t_in, t_out;
    int retcode;
    int i = 0;

    retcode = pthread_create(&t_out, NULL, &out, (void *)&i);
    if(retcode) {
	errp("pthread_create(out)", retcode);
	return 0;
    }

    retcode = pthread_create(&t_in, NULL, &in, (void *)&i);
    if(retcode) {
	errp("pthread_create(in)", retcode);
	return 0;
    }

    retcode = pthread_join(t_in, NULL);
    if(retcode) {
	errp("pthread_join(in)", retcode);
	return 0;
    }

    retcode = pthread_join(t_out, NULL);
    if(retcode) {
	errp("pthread_join(out)", retcode);
	return 0;
    }

    return 0;
}
