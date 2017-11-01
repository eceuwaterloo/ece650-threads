#include "helper.h"

/*
 * Helper routines for pthreads
 */

void pclock(char *msg, clockid_t cid) {
    struct timespec ts;
    printf("%s", msg);
    if (clock_gettime(cid, &ts) == -1) {
	perror("clock_gettime"); exit(0);
    }
    printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);
}

void errp(char *s, int code) {
    fprintf(stderr, "Error: %s -- %s\n", s, strerror(code));
}

void thr_sleep(time_t sec, long nsec) {
    struct timeval now;
    struct timezone tz;
    struct timespec ts;
    int retcode;

    pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    retcode = pthread_mutex_lock(&m);
    if(retcode) {
	fprintf(stderr, "Error: mutex_lock\n");
	exit(0);
    }

    gettimeofday(&now, &tz);
    ts.tv_sec = now.tv_sec + sec + (nsec / 1000000000L);
    ts.tv_nsec = now.tv_usec * 1000 + (nsec % 1000000000L);
    if(ts.tv_nsec > 1000000000L) {
	(ts.tv_sec)++; (ts.tv_nsec) -= 1000000000L;
    }

    retcode = pthread_cond_timedwait(&cond, &m, &ts);
    if(retcode != ETIMEDOUT) {
	if(retcode == 0) {
	    fprintf(stderr, "pthread_cond_timedwait, (zero)\n");
	}
	else {
	    fprintf(stderr, "pthread_cond_timedwait, %s\n", strerror(retcode));
	    exit(0);
	}
    }

    retcode = pthread_mutex_unlock(&m);
    if(retcode) {
	fprintf(stderr, "mutex_unlock, %s\n", strerror(retcode)); exit(0);
    }

    retcode = pthread_cond_destroy(&cond);
    if(retcode) {
	fprintf(stderr, "mutex_cond_destroy, %s\n", strerror(retcode)); fflush(stdout);
    }

    retcode = pthread_mutex_destroy(&m);
    if(retcode) {
	fprintf(stderr, "mutex_destroy, %s\n", strerror(retcode)); exit(0);
    }
}

void mulock(int ul, pthread_mutex_t *m) {
    int retcode = 0;
    char myErrStr[100];

    if(ul) {
	/* unlock */
	strcpy(myErrStr, "mutex_unlock");
	retcode = pthread_mutex_unlock(m);
    }
    else {
	/* lock */
	strcpy(myErrStr, "mutex_lock");
	retcode = pthread_mutex_lock(m);
    }
    
    if(retcode) {
	fprintf(stderr, "%s, %s\n", myErrStr, strerror(retcode));
	exit(0);
    }
}
