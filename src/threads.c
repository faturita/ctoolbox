/*
Utilizacion de popen (ver shellpipe.c) con fifos con nombre    

*/

#include <stdio.h>
#include "unpipc.h"
#include <pthread.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void * threadToRun(void *arg)
    {
    printf ("The system is going to shut down....\n");
    while (1) {
    printf ("My id is: %d\n",(int)pthread_self());
    }
    return;
    }
    

int main(int argc, char **argv)
{
    pthread_t p,s;
    int policy;
    struct sched_param param;
    
    pthread_create(&p,NULL,&threadToRun,NULL);
    pthread_create(&s,NULL,&threadToRun,NULL);
    
    pthread_getschedparam(p, &policy, &param);
    
    printf ("Sched policy %d, %d, %d,%d\n", SCHED_RR, SCHED_FIFO,SCHED_OTHER,policy); 
    
    param.sched_priority = 0;
    pthread_setschedparam( p, SCHED_RR, &param);
    param.sched_priority = 0;
    pthread_setschedparam(s, SCHED_RR, &param);
    
    pthread_getschedparam(s, &policy, &param);
    
    printf ("Sched policy %d,%d\n",policy,param.sched_priority); 
    
    while (1) {
    printf ("Main id%d\n", (int)pthread_self());
    }
    pthread_join(p,NULL);
}
    
    
