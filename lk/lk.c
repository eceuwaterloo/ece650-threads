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
 * An example of using mutex that is shared by multiple threads.
 * We have three reader threads that all synchronize on the mutex, that is
 * initially locked by the main thread.
 *
 * Then, the main thread creates a writer thread one by one, each of which
 * writes a value to the global i, and then unlocks the mutex.
 *
 * NOTE: this is NOT a program that shows effective use of mutexes. Various
 * bad things can and do happen when we run this program. For example, a reader
 * may print out 0 when we expect it to print 10, 20 or 30 (which are the values that
 * the writers write). Another problem is that a reader can starve while blocked on
 * the lock(mutex) call. This happens when two writers unlock the mutex before a reader
 * is able to lock it. The second unlock, therefore, has no effect, and a reader starves.
 */

int i;
pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;

void *wrt(void *arg) {
    int *val = (int *)arg;

    i = *val;
    printf("wrt(): about to unlock\n"); fflush(stdout);
    mulock(UNLOCK, &l);
}

void *rd(void *arg) {
    int *rid = (int *)arg;

    printf("reader %d locking\n", *rid); fflush(stdout);
    mulock(LOCK, &l);
    printf("reader %d read i = %d\n", *rid, i);
    i = 0;
}

int main() {
    i = 0;

    pthread_t w, r[3];
    int rid[3], wval;

    mulock(LOCK, &l);

    rid[0] = 1;
    pthread_create(&(r[0]), NULL, &rd, (void *)&(rid[0]));

    rid[1] = 2;
    pthread_create(&(r[1]), NULL, &rd, (void *)&(rid[1]));

    rid[2] = 3;
    pthread_create(&(r[2]), NULL, &rd, (void *)&(rid[2]));

    wval = 10;
    pthread_create(&w, NULL, &wrt, (void *)&wval);

    pthread_join(w, NULL);

    wval = 20;
    pthread_create(&w, NULL, &wrt, (void *)&wval);

    pthread_join(w, NULL);

    wval = 30;
    pthread_create(&w, NULL, &wrt, (void *)&wval);

    pthread_join(w, NULL);
    pthread_join(r[0], NULL);
    pthread_join(r[1], NULL);

    return 0;
}
