/*
Utilizacion de popen (ver shellpipe.c) con fifos con nombre    

*/

#include "unpipc.h"
#include <pthread.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void threadToRun()
    {
    printf ("The system is going to shut down....\n");
    printf ("My id is: %d",(int)pthread_self());
    return 0;
    }
    

int main(int argc, char **argv)
{
    pthread_t p;
    pthread_create(&p,NULL,threadToRun,NULL);
    
    pthread_join(p,NULL);
}
    
    
