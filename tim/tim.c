#include "helper.h"

/* 
 * A simple example of using pthread_getcpuclockid() to get the running time of
 * a thread. Try increasing the number of times we iterate in the for loop of
 * t_proc(). You can try, for example, increasing it to 100,000, or 100,000,000.
 *
 * Note that the thread must be alive for us to make the call. We cannot make the call
 * after a thread exits (the call will return an error).
 */

void *t_proc(void *arg) {
    /* Do something */
    int i, j = 0;
    for(i = 0; i < 100; i++) {
	if(i%500 == 0) {
	    j++;
	}
    }

    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);

    if(retcode) {
	errp("pthread_getcpuclockid", retcode);
    }
    else {
	pclock("Thread cpu time: ", cid);
    }
}

int main() {
    pthread_t t;
    int retcode;
    clockid_t cid;

    retcode = pthread_create(&t, NULL, &t_proc, NULL);
    if(retcode) {
	errp("pthread_create", retcode); return 0;
    }
    
    retcode = pthread_join(t, NULL);
    if(retcode) {
	errp("pthread_join", retcode);
    }


    return 0;
}
