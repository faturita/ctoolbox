/* Modulo de manejo de sonido */

#include <sys/ioctl.h>
#include </usr/include/linux/kd.h>

void sonido(int frec,int dura)
    {
    ioctl(0,KIOCSOUND,(int)(1190000/frec));
    usleep(dura*500);
    ioctl(0,KIOCSOUND,40); //deberia ir 0 aca pero no funciona!!!
    }


void musicagolpe(int i)
    {
    int pid;
    if (i) pid=fork();
    else pid=0; 
    
    if (pid==0)
	{
        sonido(293, 300);
	sonido(311, 300);
        sonido(329, 300);
        sonido(522, 500);
        sonido(329, 300);
        sonido(522, 400);
        sonido(329, 300);
        sonido(522, 900);
        usleep(300*500);
        sonido(522, 300);
        sonido(586, 300);
	sonido(622, 250);
        sonido(658, 250);
        sonido(522, 250);
        sonido(586, 300);
        sonido(658, 550);
        usleep(30*500);
        sonido(522, 300);
        sonido(586, 500);
        usleep(50*500);
        sonido(522, 400);
        usleep(150*500);
        sonido(392, 500);
        usleep(100*500);
        sonido(261, 500);
	kill (getpid());
	if (i) exit(1);
	}
    }

void musicafunebre(int i)
    {
    int pid;
    if (i) pid=fork();
    else pid=0; 
    
    if (pid==0)
	{
        sonido(293,800);
        usleep(100*500);
        sonido(293,500);
        usleep(100*500);
        sonido(293,300);
        usleep(100*500);
        sonido(293,500); 
        usleep(100*500);
        sonido(349,500);
        sonido(329,300);
        sonido(329,500);
        sonido(293,300);
        sonido(293,500);
        sonido(277,300);
        sonido(293,800);
        kill (getpid());
	if (i) exit(1);
	}
    }

void musicacomiendo(int i)
    {
    int pid;
    if (i) pid=fork();
    else pid=0; 
    
    if (pid==0)
	{
	sonido(261,10);
        sonido(329,10);
	kill (getpid());
	if (i) exit(1);
	}
    }

void musicacomiendo1(int i)
    {
    int pid;
    if (i) pid=fork();
    else pid=0; 
    
    if (pid==0)
	{
	sonido(523,10);
	sonido(587,5);
	kill (getpid());
	if (i) exit(1);
	}
    }
    



void musicacomiendo2(int i)
    {
    int pid;
    if (i) pid=fork();
    else pid=0; 
    
    if (pid==0)
	{
	sonido(800,100);
	sonido(600,100);
	sonido(250,200);
	kill (getpid());
	if (i) exit(1);
	}
    }

void musicasole(int i)
    {
    int pid;
    if (i) pid=fork();
    else pid=0; 
    
    if (pid==0)
	{
	sonido(329, .5 *700);
	sonido(440, .5 *700);
	sonido(329, .5 *700);
	sonido(440, 1 *700);
	sonido(329, .5 *700);
	sonido(440, .5 *700);
	sonido(523, .75 *700);
	sonido(523, .25 *700);
	sonido(493, .75 *700);
	sonido(329, .25 *700);
	sonido(493, .5 *700);
	sonido(329, .5 *700);
	sonido(493, 1 *700);
	sonido(329, .5 *700);
	sonido(493, .5 *700);
	sonido(587, .75 *700);
	sonido(587, .25 *700);
	sonido(523, .75 *700);
	sonido(329, .25 *700);
	sonido(523, .5 *700);
    	sonido(329, .5 *700);
	sonido(523, 1 *700);
	sonido(329, .5 *700);
	sonido(523, .5 *700);
	sonido(659, 1.5 *700);
	sonido(698, .5 *700);
	sonido(659, .5 *700);
	sonido(587, .5 *700);
	sonido(523, .75 *700);
	kill (getpid());
	if (i) exit(1);
	}
    }
