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
#include "helper.h"

/* 
 * 
 * The producer-consumer problem using pthreads. This one is safe. It uses a mutex
 * to protect critical sections. Specifically, the pthread_yield() calls within cons()
 * do not cause any problems.
 */

#define BUF_SZ 	10
#define VAL_MIN	1
#define VAL_MAX	100

pthread_t tc, tp;

typedef struct pc_typ {
    unsigned int head, tail;
    unsigned int buf[BUF_SZ];
    pthread_mutex_t lk;
} pc_typ;

void *prod(void *arg) {
    pc_typ *d = (pc_typ *)arg;

    while(1) {
	unsigned int h, t;

	mulock(LOCK, &(d->lk));
	h = d->head; t = d->tail;
	mulock(UNLOCK, &(d->lk));

	if(((h + 1)%BUF_SZ) == t) {
	    continue;
	}

	int val = (rand() % (VAL_MAX - VAL_MIN + 1)) + VAL_MIN;
	printf("prod: buf[%d] <-- %d\n", h, val); fflush(stdout);

	mulock(LOCK, &(d->lk));
	(d->buf)[(d->head)++] = val;

	if((d->head) >= BUF_SZ) {
	    (d->head) %= BUF_SZ;
	}
	mulock(UNLOCK, &(d->lk));
    }
}

void *cons(void *arg) {
    pc_typ *d = (pc_typ *)arg;

    while(1) {
	unsigned int h, t;

	mulock(LOCK, &(d->lk));
	h = d->head; t = d->tail;
	mulock(UNLOCK, &(d->lk));

	if(h == t) {
	    continue;
	}

	int val = (d->buf)[t];
	printf("cons: buf[%d] == %d\n", t, val); fflush(stdout);

	mulock(LOCK, &(d->lk));
	(d->tail)++;

	pthread_yield();
	pthread_yield();
	pthread_yield();

	if((d->tail) >= BUF_SZ) {
	    (d->tail) %= BUF_SZ;
	}
	mulock(UNLOCK, &(d->lk));

    }
}

int main() {
    pc_typ d;
    int retcode = 0;

    bzero(&d, sizeof(pc_typ));
    srand(getpid());

    retcode = pthread_mutex_init(&(d.lk), NULL);
    if(retcode) {
	fprintf(stderr, "mutex_init, %s\n", strerror(retcode));
	return 0;
    }


    retcode = pthread_create(&tc, NULL, &cons, (void *)&d);
    if(retcode) {
	fprintf(stderr, "pthread_create(cons), %s\n", strerror(retcode));
	return 0;
    }

    retcode = pthread_create(&tp, NULL, &prod, (void *)&d);
    if(retcode) {
	fprintf(stderr, "pthread_create(prod), %s\n", strerror(retcode));
	return 0;
    }

    retcode = pthread_join(tc, NULL);
    if(retcode) {
	fprintf(stderr, "pthread_join(cons), %s\n", strerror(retcode));
	return 0;
    }

    retcode = pthread_join(tp, NULL);
    if(retcode) {
	fprintf(stderr, "pthread_join(prod), %s\n", strerror(retcode));
	return 0;
    }

    return 0;
}
