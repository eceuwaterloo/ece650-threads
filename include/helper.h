#ifndef _HELPER_H_
#define _HELPER_H_	1

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>
#include "helper.h"

enum {
    LOCK,
    UNLOCK,
};

extern void errp(char *s, int code);
extern void thr_sleep(time_t sec, long nsec);
extern void mulock(int ul, pthread_mutex_t *m);
#endif
