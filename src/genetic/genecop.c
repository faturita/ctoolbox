#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include "../Hopfield/mathNeuron.h"

#define PI	3.14159265359

#define THREADS 20

#define TRUE 1
#define FALSE 0

struct parameters {
    int initial_population;
    float crossover_probability;
    float mutation_probability;
};

struct globalMemory {
    int *chromosome;
    float value;
    pthread_t *thread;
    unsigned short isAlive;
    int lock;
} globalMem[THREADS];

int threadnumber = 0;

void mutate(int *chromosome)
{
    int iBeen=getProb(0,22);
    chromosome[iBeen]=~chromosome[iBeen];
}

int * crossover(int *destChromosome, int *chromosome1, int *chromosome2)
{
    int s=0;
    int iBeen=getProb(0,22);
    int *sourceChromosome;
    
    sourceChromosome = chromosome1;
    
    for (s=0;s<22;s++) {
	if (iBeen == s)
	    sourceChromosome = chromosome2;
	destChromosome[s] = sourceChromosome[s];
    }
    
    printf ("X at pos:%d\n", iBeen);
    
    return destChromosome;	
}

void showChromosome(int *chromosome)
{
    int i=0;
    for (i=0;i<22;i++) {
	printf("%1d",chromosome[i]);	
    }
    printf ("\n");
}

void init_chromosome(int *chromosome)
{
    int i=0;
    for(i=0;i<22;i++) {
	chromosome[i]=getProb(0,2);
    }

}

float eval(int *chromosome)
{
    int i=0;
    int val=0;
    float result=0;
    for (i=0;i<22;i++) {
	val = val + (chromosome[i]==1 ? 1 : 0) * (pow(2,i));
    }
    result = -1.0 + val * 3.0 / (pow(2,22) - 1);
    result =(float) (result * sin((double)(10 * PI * result)) + 1.0);
    return result;
}

float exGetGlobalMin(float cota, int checkCota) {
    int s=0;
    float globalMin=globalMem[0].value;
    for (s=0;s<THREADS;s++) {
	if (globalMem[s].value<globalMin && (!checkCota || (cota!=globalMem[s].value) ) ) {
	    globalMin=globalMem[s].value;
	}
    }

    return globalMin;
    
}


float exGetGlobalMax(float cota, int checkCota) {
    int s=0;
    float globalMax=globalMem[0].value;
    for (s=0;s<THREADS;s++) {
	if (globalMem[s].value>globalMax && (!checkCota || (checkCota && cota!=globalMem[s].value) ) ) {
	    globalMax=globalMem[s].value;
	}
    }
    return globalMax;
    
}

float getGlobalMax() {
    return (exGetGlobalMax(0, FALSE));
}

float getGlobalAverage() {
    int s=0,activos=0;
    float globalAverage=0;
    for(s=0;s<THREADS;s++) {
        globalAverage+=globalMem[s].value;
	}
    
    
    return (globalAverage/THREADS);
}

void * done(void * arg)
{
    int *piThreadNumber = (int*) arg;
    int iThreadNumber = (*piThreadNumber);
    printf ("My (%d).\n",iThreadNumber);
    sched_yield();
    globalMem[iThreadNumber].isAlive = 0;
    threadnumber++;
    return;    
}

void * one(void * arg)
{
    int *piThreadNumber = (int*) arg;
    int iThreadNumber = (*piThreadNumber);
    globalMem[iThreadNumber].isAlive = 1;
    threadnumber++;

    while (globalMem[iThreadNumber].isAlive) {
	sched_yield();
	sleep(1);
	
        globalMem[iThreadNumber].value = eval(globalMem[iThreadNumber].chromosome);
	
	// Espera hasta que esten todos listos... o hasta que este lockeado.
	if (threadnumber != THREADS || globalMem[iThreadNumber].lock)
	    continue;
	    
	//printf ("(%d)",(int)pthread_self());
	    

	//if (globalMem[iThreadNumber].value<getGlobalMax()) {

	//    break;
	//}
	
	if (globalMem[iThreadNumber].value==getGlobalMax()) {
	    //printf ("Max Value:%f\n", globalMem[iThreadNumber].value);
	    continue;
	}
	
	mutate(globalMem[iThreadNumber].chromosome);

    }
    printf ("Last Value:%f\n", globalMem[iThreadNumber].value);
    globalMem[iThreadNumber].isAlive = 0;
        
    return NULL;
}

int getMinThread(float cota, int checkCota)
{
    int s;
    float fMinValue = exGetGlobalMin(cota, checkCota);
    
    for (s=0;s<THREADS;s++) {
	if ( globalMem[s].value == fMinValue)
	    return s;
    }
    
    return 0;
}

int getMaxThread(float cota, int checkCota)
{
    int s;
    float fMaxValue = exGetGlobalMax(cota, checkCota);
    
    for (s=0;s<THREADS;s++) {
	if ( globalMem[s].value == fMaxValue)
	    return s;
    }
    
    return 0;
}


int init()
{
    
}


void exec_crossover()
{
    int iMaxThread1;
    int iMaxThread2;
    int iMinThread;
    
    iMaxThread1 = getMaxThread(0, FALSE);
    iMaxThread2 = getMaxThread(globalMem[iMaxThread1].value, TRUE);
    iMinThread  = getMinThread(0, FALSE);
    
    globalMem[iMinThread].lock = TRUE;
    globalMem[iMaxThread1].lock = TRUE;
    globalMem[iMaxThread2].lock = TRUE;
    
    sched_yield();
    
    printf ("Crossover (%d<-%dX%d)\n", iMinThread, iMaxThread1, iMaxThread2);
    
    crossover(globalMem[iMinThread].chromosome, globalMem[iMaxThread1].chromosome, globalMem[iMaxThread2].chromosome);
    
    globalMem[iMinThread].lock = FALSE;
    globalMem[iMaxThread1].lock = FALSE;
    globalMem[iMaxThread2].lock = FALSE;
}



int main(int argc, char *argv[])
{
    pthread_t th[THREADS];
    pthread_t thread;
    int s=0, ss=0;
    int iFinishThread;
    int bFinish;
    int *chromosome;

    int policy;
    struct sched_param param;
    
    initRandom();
    
    printf ("Starting EP\n");
    printf ("Starting thread");

    for(s=0;s<THREADS;s++) {
	globalMem[s].chromosome = (int *)malloc( sizeof(int) * 22);
	globalMem[s].value = 0;
	globalMem[s].isAlive = 1;
	globalMem[s].lock = FALSE;
    }
    
    
    for(s=0;s<THREADS;s++) {
    	init_chromosome( globalMem[s].chromosome );
	pthread_create( &(th[s]), NULL, &one, &s);
	pthread_getschedparam(th[s], &policy, &param);
        param.sched_priority = 0;
        pthread_setschedparam( th[s], SCHED_RR, &param);
	
	pthread_detach(th[s]);
	
	sleep(1);
    }
        
    while( 1) {
	sleep(5);
	sched_yield();
	iFinishThread=0;
	
	for(s=0;s<THREADS;s++) {
	    if (!globalMem[s].isAlive) iFinishThread++;
	}
	
	if (iFinishThread == THREADS)
	    break;
	    
	// Condicion de convergencia.......
	bFinish = TRUE;
	for (s=0;s<THREADS;s++) {
	    if (getGlobalMax() != globalMem[s].value)
		bFinish = FALSE;
	}
	
	if (bFinish) break;
	
	exec_crossover();
	
	
    }
    
    for (s=0;s<THREADS;s++)
	pthread_join(th[s],NULL);

    printf ("Maximo:%f\n",getGlobalMax());
//    sleep(20);
    return;

}

